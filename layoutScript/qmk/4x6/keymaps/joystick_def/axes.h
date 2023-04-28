// Set Parameters
uint16_t minAxisValue = 0;
uint16_t maxAxisValue = 1023;

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

int16_t calculatePower(uint16_t power, uint16_t standard, uint8_t direction) {	
	uint16_t strengh = power * (-direction) + (direction * standard);
	return (strengh / (100 - standard)) * 50;
}
void sendKeyboardSignal(uint8_t directionX,uint8_t directionY){
	 //if only one direction is ON the others directions value will be 0 thus the product also will be 0

	if(directionX * directionY == 0){
		 if(directionX == -1){
			 tap_code(KC_RIGHT);
			 tap_code(KC_UP);
		}

		if(directionX == 1){
			 tap_code(KC_LEFT);
			 tap_code(KC_DOWN);
		}

		 if(directionY == -1){
			 tap_code(KC_LEFT);
			 tap_code(KC_UP);
		}

		if(directionY == 1){
			 tap_code(KC_RIGHT);
			 tap_code(KC_DOWN);
		}

	} else {
		if(directionX == -1 && directionY == -1){
			 tap_code(KC_LEFT);
		}

		if(directionX == 1 && directionY == -1){
			 tap_code(KC_DOWN);
		}

		 if(directionX == -1 && directionY == 1){
			 tap_code(KC_UP);
		}

		if(directionX == 1 && directionY == 1){
			 tap_code(KC_RIGHT);
		}
	}
}
 

 void axesEvaluation(uint8_t xPin, uint16_t xOrigin, uint8_t yPin, uint16_t yOrigin) {
	 int8_t directionX = 0;
	 int8_t directionY = 0;
	 int8_t xPower = 0;
	 int8_t yPower = 0;
	 int8_t xStandard = 53;
	 int8_t yStandard = 49;
	 int8_t minimumRange = 2;


	 //define directions and power of the direction
	if (axisCoordinate(xPin, xOrigin) < xStandard - minimumRange) {
		directionX = 1;
		xPower = calculatePower(axisCoordinate(xPin, xOrigin), xStandard, directionX);
	}

	if (axisCoordinate(xPin, xOrigin) > xStandard + minimumRange) {
		directionX = -1;
		xPower = calculatePower(axisCoordinate(xPin, xOrigin), xStandard, directionX);
	}

	if (axisCoordinate(yPin, yOrigin) < yStandard - minimumRange) {
		directionY = 1;
		yPower = calculatePower(axisCoordinate(yPin, yOrigin), yStandard, directionY);
	}

	if (axisCoordinate(yPin, yOrigin) > yStandard + minimumRange) {
		directionY = -1;
		yPower = calculatePower(axisCoordinate(yPin, yOrigin), yStandard, directionY);
	}

	 //define decide between on key and two key action 
	 if(xPower > 45 || yPower > 45){
		 sendKeyboardSignal(directionX, directionY);
		 sendKeyboardSignal(directionX, directionY);
	 } else {
		 sendKeyboardSignal(directionX, directionY);
	 }
}

