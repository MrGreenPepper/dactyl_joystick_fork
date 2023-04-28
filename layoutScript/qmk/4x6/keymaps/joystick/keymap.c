#include QMK_KEYBOARD_H

#include "analog.h"



uint8_t xPin  = F5;
uint8_t yPin  = F4;   // VRy // B5
uint8_t swPin = D1;  // SW

// Set Parameters
uint16_t minAxisValue = 0;
uint16_t maxAxisValue = 1023;

int8_t xPolarity = 1;
int8_t yPolarity = 1;

uint8_t cursorTimeout = 100;
uint16_t lastCursor = 0;

int16_t xOrigin, yOrigin;


void matrix_init_keymap(void) {
    // init pin? Is needed?
    // setPinInputHigh(swPin);
    // Account for drift
    xOrigin = analogReadPin(xPin);
    yOrigin = analogReadPin(yPin);
}

int16_t axisCoordinate(uint8_t pin, uint16_t origin) {
    int8_t  direction;
    int16_t distanceFromOrigin;
    int16_t range;

    int16_t position = analogReadPin(pin);

    if (origin == position) {
        return 0;
    } else if (origin > position) {
        distanceFromOrigin = origin - position;
        range              = origin - minAxisValue;
        direction          = -1;
    } else {
        distanceFromOrigin = position - origin;
        range              = maxAxisValue - origin;
        direction          = 1;
    }

    float   percent    = (float)distanceFromOrigin / range;
    int16_t coordinate = (int16_t)(percent * 100);
    if (coordinate < 0) {
        return 0;
    } else if (coordinate > 100) {
        return 100 * direction;
    } else {
        return coordinate * direction;
    }
}

 

void matrix_scan_user() {
 if (timer_elapsed(lastCursor) > cursorTimeout) {
        lastCursor = timer_read();
     

		if (axisCoordinate(xPin, xOrigin) < 40) {
			tap_code(KC_UP);

			if (axisCoordinate(xPin, xOrigin) < 1) {
				tap_code(KC_UP);

			}

		}

		if (axisCoordinate(xPin, xOrigin) > 60) {
			tap_code(KC_DOWN);

			if (axisCoordinate(xPin, xOrigin) > 99) {
				tap_code(KC_DOWN);

			}
		}

		if (axisCoordinate(yPin, yOrigin) < 40) {
			tap_code(KC_RIGHT);
				if (axisCoordinate(yPin, yOrigin) < 1) {
					tap_code(KC_RIGHT);

				}

 		}

		if (axisCoordinate(yPin, yOrigin) > 60) {
			tap_code(KC_LEFT);
				if (axisCoordinate(yPin, yOrigin) > 99) {
					tap_code(KC_LEFT);
	
				}
 		}
 	}
}

#define layer0 0
#define layer1 1
#define layer2 2
#define layer3 3
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* example qwerty
 * +-----------------------------------------+                             +-----------------------------------------+
 * | ESC  |   q  |   w  |   e  |   r  |   t  |                             |   y  |   u  |   i  |   o  |   p  |      |
 * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
 * | TAB  |   a  |   s  |   d  |   f  |   g  |                             |   h  |   j  |   k  |   l  |   ;  |      |
 * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
 * | SHFT |   z  |   x  |   c  |   v  |   b  |                             |   n  |   m  |   ,  |   .  |   /  |      |
 * +------+------+------+------+-------------+                             +-------------+------+------+------+------+
 *               |  [   |   ]  |                                                         |      |      |
 *               +-------------+-------------+                             +-------------+-------------+
 *                             |      |      |                             |      |      |
 *                             |------+------|                             |------+------|
 *                             |      |      |                             |      |      |
 *                             +-------------+                             +-------------+
 *                                           +-------------+ +-------------+
 *                                           |      |      | |      |      |
 *                                           |------+------| |------+------|
 *                                           |      |      | |      |      |
 *                                           +-------------+ +-------------+
 
 
*[_BASE] = LAYOUT( \
*    KC_ESC,  KC_Q,  KC_W,   KC_E,   KC_R,   KC_T,               KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_MINS,   \
*    KC_TAB,  KC_A,  KC_S,   KC_D,   KC_F,   KC_G,               KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,KC_QUOT,   \
*    KC_LSFT, KC_Z,  KC_X,   KC_C,   KC_V,   KC_B,               KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,KC_BSLASH, \
*                    KC_LBRC,KC_RBRC,                                            KC_PLUS,KC_EQL,                    \
*                                    RAISE,  KC_SPC,             KC_ENT, LOWER,                                     \
*                                    KC_TAB, KC_HOME,            KC_END, KC_DEL,                                    \
*                                    KC_BSPC,KC_GRV,             KC_LGUI,KC_LALT                                    \
*),
 */


[layer0] = LAYOUT(
KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_LSFT, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_RSFT, KC_LCTL, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_QUOT, LSFT(KC_8), LSFT(KC_9), RALT(KC_7), RALT(KC_0), KC_NO, KC_NO, KC_ENT, KC_TAB, MO(1), KC_BSPC, KC_SPC, KC_DEL, MO(2), KC_NO, KC_LALT, KC_LGUI),
[layer1] = LAYOUT(
KC_MINS, RALT(KC_Q), LSFT(KC_3), LSFT(KC_4), LSFT(KC_5), RALT(KC_MINS), KC_NO, KC_PGDN, KC_UP, KC_PGUP, KC_NO, KC_NO, KC_TRNS, LSFT(KC_1), KC_UNDS, LSFT(KC_0), LSFT(KC_6), LSFT(KC_7), KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, KC_TRNS, KC_TRNS, KC_EQL, LSFT(KC_NUHS), LSFT(KC_2), LSFT(KC_EQL), RALT(KC_NUBS), KC_VOLD, KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLU, KC_NO, KC_NUBS, LSFT(KC_NUBS), RALT(KC_8), RALT(KC_9), KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TO(3), KC_NO, KC_TRNS, KC_TRNS),
[layer2] = LAYOUT(
KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_PSLS, KC_P7, KC_P8, KC_P9, KC_PMNS, KC_NO, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_PAST, KC_P4, KC_P5, KC_P6, KC_PPLS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_PCMM, KC_P1, KC_P2, KC_P3, KC_PENT, KC_NO, KC_SLEP, KC_WAKE, KC_P0, KC_PDOT, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_TRNS, KC_TRNS),
[layer3] = LAYOUT(
KC_NO, KC_NO, KC_PGDN, KC_UP, KC_PGUP, KC_NO, KC_PSLS, KC_P7, KC_P8, KC_P9, KC_PMNS, KC_NO, KC_NO, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, KC_PAST, KC_P4, KC_P5, KC_P6, KC_PPLS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_PCMM, KC_P1, KC_P2, KC_P3, KC_PENT, KC_NO, KC_NO, KC_NO, KC_P0, KC_PDOT, KC_NO, KC_NO, KC_TRNS, KC_TRNS, TO(0), KC_BSPC, KC_TRNS, KC_TRNS, TO(0), KC_NO, KC_TRNS, KC_TRNS)
};