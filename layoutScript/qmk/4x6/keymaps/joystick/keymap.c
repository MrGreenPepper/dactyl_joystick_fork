#include QMK_KEYBOARD_H

#include <stdlib.h>
#include <math.h>
#include "analog.h"


/* ---------------- Joystick-Konfiguration ---------------- */

#define JOY_X_PIN F5
#define JOY_Y_PIN F4

/* Totzone um die Mitte in Prozent (0-100) */
#define JOY_DEADZONE 15

/* Wiederholintervall in ms: langsam bei kleiner, schnell bei voller Auslenkung */
#define JOY_INTERVAL_SLOW 300
#define JOY_INTERVAL_FAST 25

/* Kennlinie: 1 = linear, 2 = quadratisch, 3 = kubisch.
 * Je höher, desto länger bleibt der Joystick bei kleiner/mittlerer Auslenkung langsam
 * und wird erst kurz vor dem Anschlag schnell. */
#define JOY_CURVE 3

/* Hoch/Runter langsamer als Links/Rechts, in Prozent des Intervalls.
 * 100 = gleich schnell, 150 = Hoch/Runter braucht 1,5x so lange pro Schritt. */
#define JOY_VERTICAL_SLOWDOWN 150

/* Diagonale nur, wenn die schwächere Achse mindestens so stark ist
 * (in Prozent der stärkeren). 41 entspricht ca. 22,5 Grad -> 8 gleich große Sektoren. */
#define JOY_DIAGONAL_RATIO 41

/* Einbaudrehung des Joysticks in Grad (im Uhrzeigersinn eingebaut = positiv).
 * Wird herausgerechnet, damit "intuitiv nach oben" auch oben ergibt.
 * Wird es schlimmer statt besser, das Vorzeichen umdrehen (-20). 0 = aus. */
#define JOY_ROTATION_DEG 20

/* Zuordnung: Vorzeichen der Achse -> Taste (wie bisher verdrahtet) */
#define JOY_X_NEG KC_UP
#define JOY_X_POS KC_DOWN
#define JOY_Y_NEG KC_RIGHT
#define JOY_Y_POS KC_LEFT

static int16_t  xOrigin, yOrigin;
static uint16_t lastTap = 0;

/* Mitte beim Start kalibrieren (Stick dabei nicht berühren) */
void keyboard_post_init_user(void) {
    int32_t xs = 0, ys = 0;
    for (uint8_t i = 0; i < 16; i++) {
        xs += analogReadPin(JOY_X_PIN);
        ys += analogReadPin(JOY_Y_PIN);
        wait_ms(2);
    }
    xOrigin = xs / 16;
    yOrigin = ys / 16;
}

/* Auslenkung relativ zur Mitte: -100 .. +100 */
static int8_t axisValue(uint8_t pin, int16_t origin) {
    int16_t pos = analogReadPin(pin);
    int32_t v;
    if (pos >= origin) {
        int16_t range = 1023 - origin;
        v = range > 0 ? ((int32_t)(pos - origin) * 100) / range : 0;
    } else {
        int16_t range = origin;
        v = range > 0 ? -((int32_t)(origin - pos) * 100) / range : 0;
    }
    if (v > 100) v = 100;
    if (v < -100) v = -100;
    return (int8_t)v;
}

void matrix_scan_user(void) {
    if (!is_keyboard_master()) return;

    int16_t xr = axisValue(JOY_X_PIN, xOrigin);
    int16_t yr = axisValue(JOY_Y_PIN, yOrigin);

    /* Einbaudrehung herausrechnen (cos/sin werden vom Compiler vorab berechnet) */
    const int16_t rc = (int16_t)(cos(JOY_ROTATION_DEG * M_PI / 180.0) * 1000);
    const int16_t rs = (int16_t)(sin(JOY_ROTATION_DEG * M_PI / 180.0) * 1000);
    int16_t xs = ((int32_t)xr * rc - (int32_t)yr * rs) / 1000;
    int16_t ys = ((int32_t)xr * rs + (int32_t)yr * rc) / 1000;
    int8_t x = xs > 100 ? 100 : (xs < -100 ? -100 : xs);
    int8_t y = ys > 100 ? 100 : (ys < -100 ? -100 : ys);
    int8_t ax = abs(x);
    int8_t ay = abs(y);
    int8_t mag = ax > ay ? ax : ay;

    if (mag < JOY_DEADZONE) return;

    /* Intervall linear zwischen langsam und schnell */
    /* t = 0..1000 entlang der nutzbaren Auslenkung, dann Kennlinie anwenden */
    uint32_t t = ((uint32_t)(mag - JOY_DEADZONE) * 1000) / (100 - JOY_DEADZONE);
    uint32_t c = t;
    for (uint8_t i = 1; i < JOY_CURVE; i++) c = c * t / 1000;
    uint16_t interval = JOY_INTERVAL_SLOW - (c * (JOY_INTERVAL_SLOW - JOY_INTERVAL_FAST)) / 1000;

    /* x-Achse = Hoch/Runter: wenn sie dominiert, langsamer wiederholen */
    if (ax >= ay) interval = (uint32_t)interval * JOY_VERTICAL_SLOWDOWN / 100;

    if (timer_elapsed(lastTap) < interval) return;
    lastTap = timer_read();

    /* 8 Richtungen: schwächere Achse nur mitnehmen, wenn sie stark genug ist */
    bool useX = ax >= JOY_DEADZONE && (ax >= ay || (int16_t)ax * 100 >= (int16_t)ay * JOY_DIAGONAL_RATIO);
    bool useY = ay >= JOY_DEADZONE && (ay >= ax || (int16_t)ay * 100 >= (int16_t)ax * JOY_DIAGONAL_RATIO);

    if (useX) tap_code(x < 0 ? JOY_X_NEG : JOY_X_POS);
    if (useY) tap_code(y < 0 ? JOY_Y_NEG : JOY_Y_POS);
}

#define layer0 0
#define layer1 1
#define layer2 2
#define layer3 3

/* Erzeugt aus linux_windows_layout.json mit wiring.json */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[layer0] = {
    { KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T },
    { KC_LSFT, KC_A, KC_S, KC_D, KC_F, KC_G },
    { KC_LCTL, KC_Z, KC_X, KC_C, KC_V, KC_B },
    { MO(1), MO(2), LSFT(KC_8), LSFT(KC_9), KC_NO, KC_BSPC },
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC, KC_NO },
    { KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC },
    { KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_RSFT },
    { KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_QUOT },
    { KC_SPC, KC_ENT, RALT(KC_7), RALT(KC_0), KC_NO, KC_NO },
    { KC_TAB, KC_DEL, KC_LALT, KC_LGUI, KC_NO, KC_NO }
},
[layer1] = {
    { KC_MINS, RALT(KC_Q), LSFT(KC_3), LSFT(KC_4), LSFT(KC_5), RALT(KC_MINS) },
    { RALT(KC_RBRC), LSFT(KC_1), KC_UNDS, LSFT(KC_0), LSFT(KC_6), LSFT(KC_7) },
    { KC_NUHS, KC_EQL, LSFT(KC_NUHS), LSFT(KC_2), LSFT(KC_EQL), RALT(KC_NUBS) },
    { KC_TRNS, TO(3), KC_NUBS, LSFT(KC_NUBS), KC_NO, KC_TRNS },
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO },
    { KC_PSLS, KC_P7, KC_P8, KC_P9, KC_PMNS, KC_NO },
    { KC_PAST, KC_P4, KC_P5, KC_P6, KC_PPLS, KC_TRNS },
    { KC_PCMM, KC_P1, KC_P2, KC_P3, KC_PENT, KC_NO },
    { KC_TRNS, KC_TRNS, KC_P0, KC_PDOT, KC_NO, KC_NO },
    { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO }
},
[layer2] = {
    { KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6 },
    { KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12 },
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO },
    { KC_TRNS, KC_TRNS, KC_SLEP, KC_WAKE, KC_NO, KC_TRNS },
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO },
    { KC_NO, KC_PGDN, KC_UP, KC_PGUP, KC_NO, KC_NO },
    { KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, KC_TRNS },
    { KC_VOLD, KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLU, KC_NO },
    { KC_TRNS, KC_TRNS, RALT(KC_8), RALT(KC_9), KC_NO, KC_NO },
    { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO }
},
[layer3] = {
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO },
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO },
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO },
    { TO(0), TO(0), KC_NO, KC_NO, KC_NO, TO(0) },
    { KC_NO, KC_NO, KC_NO, KC_NO, TO(0), KC_NO },
    { KC_PSLS, KC_P7, KC_P8, KC_P9, KC_PMNS, KC_NO },
    { KC_PAST, KC_P4, KC_P5, KC_P6, KC_PPLS, KC_NO },
    { KC_PCMM, KC_P1, KC_P2, KC_P3, KC_PENT, KC_NO },
    { KC_NO, KC_NO, KC_P0, KC_PDOT, KC_NO, KC_NO },
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }
}
};
