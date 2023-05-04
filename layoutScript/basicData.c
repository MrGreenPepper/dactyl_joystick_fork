#include QMK_KEYBOARD_H

#include "analog.h"

//insert the signings of your chip pins for your joystick here
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

		/*	if (axisCoordinate(xPin, xOrigin) < 1) {
				tap_code(KC_UP);

			}*/

		}

		if (axisCoordinate(xPin, xOrigin) > 60) {
			tap_code(KC_DOWN);

	/*		if (axisCoordinate(xPin, xOrigin) > 99) {
				tap_code(KC_DOWN);

			}*/
		}

		if (axisCoordinate(yPin, yOrigin) < 40) {
			tap_code(KC_RIGHT);
				/*if (axisCoordinate(yPin, yOrigin) < 1) {
					tap_code(KC_RIGHT);

				}*/

 		}

		if (axisCoordinate(yPin, yOrigin) > 60) {
			tap_code(KC_LEFT);
				/*if (axisCoordinate(yPin, yOrigin) > 99) {
					tap_code(KC_LEFT);
	
				}*/
 		}
 	}
}
