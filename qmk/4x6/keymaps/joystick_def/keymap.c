#include QMK_KEYBOARD_H

#include "analog.h"
#include "axes.h"
//#include "testing.h"
//on off = 1 0
int8_t rangeCheckx = 0;
int8_t rangeChecky = 0;
//ten steps down from the number
int8_t detailedCheckx = 0;
int8_t detailedChecky = 0;


uint8_t xPin  = F5;
uint8_t yPin  = F4;   // VRy // B5
uint8_t swPin = D1;  // SW



int8_t xPolarity = 1;
int8_t yPolarity = 1;

uint8_t cursorTimeout = 100;
uint16_t lastCursor = 0;

int16_t xOrigin, yOrigin;

void matrix_init_keymap(void) {
	timer_init();
    // init pin? Is needed?
    // setPinInputHigh(swPin);
    // Account for drift
    xOrigin = analogReadPin(xPin);
    yOrigin = analogReadPin(yPin);
}




void matrix_scan_user() {
 if (timer_elapsed(lastCursor) > cursorTimeout) {
        lastCursor = timer_read();
		//axesEvaluation(xPin, xOrigin, yPin, yOrigin);
		
		
		/*if(detailedCheckx > 0){
			detailedCheck(xPin, detailedCheck);
		}*/


/*
		if (axisCoordinate(xPin, xOrigin) < 51) {
			tap_code(KC_RIGHT);

			if (axisCoordinate(xPin, xOrigin) < 1) {
				tap_code(KC_RIGHT);
				tap_code(KC_RIGHT);
				tap_code(KC_RIGHT);
			}

		}

		if (axisCoordinate(xPin, xOrigin) > 55) {
			tap_code(KC_LEFT);

			if (axisCoordinate(xPin, xOrigin) > 99) {
				tap_code(KC_LEFT);
				tap_code(KC_LEFT);
				tap_code(KC_LEFT);
			}
		}

		if (axisCoordinate(yPin, yOrigin) < 47) {
			tap_code(KC_UP);
				if (axisCoordinate(yPin, yOrigin) < 1) {
					tap_code(KC_UP);
					tap_code(KC_UP);
					tap_code(KC_UP);
					tap_code(KC_UP);
					tap_code(KC_UP);
				}

 		}

		if (axisCoordinate(yPin, yOrigin) > 51) {
			tap_code(KC_DOWN);
				if (axisCoordinate(yPin, yOrigin) > 99) {
					tap_code(KC_DOWN);
					tap_code(KC_DOWN);
					tap_code(KC_DOWN);
					tap_code(KC_DOWN);
					tap_code(KC_DOWN);
				}
 		}*/
 	}
}

#define layer0 0
#define layer1 1
#define layer2 2
#define layer3 3
#define layer4 4
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[layer0] = LAYOUT(
KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_LSFT, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_RSFT, KC_LCTL, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_QUOT, LSFT(KC_8), LSFT(KC_9), RALT(KC_7), RALT(KC_0), KC_NO, KC_NO, KC_ENT, KC_TAB, MO(1), KC_BSPC, KC_SPC, KC_DEL, MO(2), KC_NO, KC_LALT, KC_LGUI),
[layer1] = LAYOUT(
RALT(KC_Q), LSFT(KC_3), LSFT(KC_4), LSFT(KC_5), LSFT(KC_6), RALT(KC_MINS), KC_GRV, KC_PGDN, KC_UP, KC_PGUP, LSFT(KC_GRV), KC_NO, KC_TRNS, LSFT(KC_1), KC_UNDS, LSFT(KC_0), KC_NUBS, LSFT(KC_7), KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, KC_TRNS, KC_TRNS, LSFT(KC_EQL), LSFT(KC_NUHS), LSFT(KC_2), KC_NUHS, RALT(KC_NUBS), KC_VOLD, KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLU, KC_MINS, KC_NUBS, LSFT(KC_NUBS), RALT(KC_8), RALT(KC_9), KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TO(3), KC_NO, KC_TRNS, KC_TRNS),
[layer2] = LAYOUT(
KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_PSLS, KC_P7, KC_P8, KC_P9, KC_PPLS, KC_NO, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_PAST, KC_P4, KC_P5, KC_P6, KC_PCMM, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_PMNS, KC_P1, KC_P2, KC_P3, KC_PEQL, KC_NO, KC_SLEP, KC_WAKE, KC_P0, KC_PDOT, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_TRNS, KC_TRNS),
[layer3] = LAYOUT(
KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_P3, KC_P3, KC_P3, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, TO(0), KC_NO, KC_NO, KC_NO, TO(4), KC_NO, KC_NO, KC_NO),
[layer4] = LAYOUT(
KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_P4, KC_P4, KC_P4, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, TO(3), KC_NO, KC_NO, KC_NO, TO(0), KC_NO, KC_NO, KC_NO)
};
