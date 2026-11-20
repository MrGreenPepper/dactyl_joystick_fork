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

