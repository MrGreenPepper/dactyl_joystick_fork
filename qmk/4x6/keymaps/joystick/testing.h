
void rangeCheck(uint8_t axes){
	int16_t original;
	unit8_t axesPin;

	if (axes == 'x') {
		axesPin = xPin;
		original = xOrigin;
	} else {
		axesPin = yPin;
		original = yOrigin;
	}


 	if ( axisCoordinate(axesPin, original)< 10) {
			tap_code(KC_P0);

	 }
	if (10<axisCoordinate(axesPin, original) && axisCoordinate(axesPin, original)< 20) {
				tap_code(KC_P1);
	}

	if (20<axisCoordinate(axesPin, original) && axisCoordinate(axesPin, original)< 30) {
				tap_code(KC_P2);
	}

	if (30<axisCoordinate(axesPin, original)&& axisCoordinate(axesPin, original) < 40) {
				tap_code(KC_P3);
	}

	if (40<axisCoordinate(axesPin, original) && axisCoordinate(axesPin, original)< 50) {
				tap_code(KC_P4);
	}

	if (50<axisCoordinate(axesPin, original) && axisCoordinate(axesPin, original)< 60) {
				tap_code(KC_P5);
	}

	if (60<axisCoordinate(axesPin, original) && axisCoordinate(axesPin, original)< 70) {
				tap_code(KC_P6);
	}

	if (70<axisCoordinate(axesPin, original) && axisCoordinate(axesPin, original)< 80) {
				tap_code(KC_P7);
	}

	if (80<axisCoordinate(axesPin, original) && axisCoordinate(axesPin, original)< 90) {
				tap_code(KC_P8);
	}

	if (90<axisCoordinate(axesPin, original) && axisCoordinate(axesPin, original)< 101) {
				tap_code(KC_P9);
	} 
}


void detailedCheck(uint8_t axes, int8_t checkNumber){
	int16_t original;
	unit8_t axesPin;

	if (axes = 'x') {
		axesPin = xPin;
		original = xOrigin;
	} else {
		axesPin = yPin;
		original = yOrigin;
	}

	checkNumber = checkNumber * 10 - 1;


 	
	if (axisCoordinate(axesPin, original)== checkNumber+1) {
				tap_code(KC_P0);
	}

	if (axisCoordinate(axesPin, original)== checkNumber+2) {
				tap_code(KC_P1);
	}

	if (axisCoordinate(axesPin, original)== checkNumber+3) {
				tap_code(KC_P2);
	}

	if (xisCoordinate(axesPin, original) == checkNumber+4) {
				tap_code(KC_P3);
	}

	if (axisCoordinate(axesPin, original)== checkNumber+5) {
				tap_code(KC_P4);
	}

	if (axisCoordinate(axesPin, original)== checkNumber+6) {
				tap_code(KC_P5);
	}

	if (axisCoordinate(axesPin, original)== checkNumber+7) {
				tap_code(KC_P6);
	}

	if (axisCoordinate(axesPin, original)== checkNumber+8) {
				tap_code(KC_P7);
	}

	if (axisCoordinate(axesPin, original)== checkNumber+9) {
				tap_code(KC_P8);
	}

	if (axisCoordinate(axesPin, original)== 1checkNumber+10) {
				tap_code(KC_P9);
	} 
}
