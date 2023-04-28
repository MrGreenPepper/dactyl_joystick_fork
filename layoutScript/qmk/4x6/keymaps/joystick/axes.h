#include "analog.h"
#include "math.h"
// Set Parameters
uint16_t minAxisValue = 0;
uint16_t maxAxisValue = 1023;

char xPositivKey = KC_RIGHT;
char xNegativKey = KC_LEFT;
char yPositivKey = KC_UP;
char yNegativKey = KC_DOWN;

int8_t angleCorrector = 0;

uint8_t majorAxesRange = 60;
uint8_t minorAxesRange;
 
uint8_t ranges[8];

int16_t axisCoordinate(uint8_t pin, uint16_t origin) {
    int8_t  axesDirection;
    int16_t distanceFromOrigin;
    int16_t range;

    int16_t position = analogReadPin(pin);

    if (origin == position) {
        return 0;
    } else if (origin > position) {
        distanceFromOrigin = origin - position;
        range              = origin - minAxisValue;
        axesDirection          = -1;
    } else {
        distanceFromOrigin = position - origin;
        range              = maxAxisValue - origin;
        axesDirection          = 1;
    }

    float   percent    = (float)distanceFromOrigin / range;
    int16_t coordinate = (int16_t)(percent * 100);
    if (coordinate < 0) {
        return 0;
    } else if (coordinate > 100) {
        return 100 * axesDirection;
    } else {
        return coordinate * axesDirection;
    }
}

int16_t calculatePower(uint16_t power, uint16_t standard, int direction) {	
	int strengh = power * (-direction) + (direction * standard);
	return (strengh / (100 - standard)) * 50;
}

void sendKeyboardSignal(int dirX,int dirY){
	 //if only one direction is ON the others directions value will be 0 thus the product also will be 0

	if((dirX * dirY) == 0){
		if(dirY == 1){
			 tap_code(KC_RIGHT);
			 tap_code(KC_UP);
		}

		else if(dirX == -1){
			 tap_code(KC_LEFT);
			 tap_code(KC_UP);
		}

		else if(dirX == 1){
			 tap_code(KC_RIGHT);
			 tap_code(KC_DOWN);
		}

		else if(dirY == -1){
			 tap_code(KC_LEFT);
			 tap_code(KC_DOWN);
		}



	} else {
		
		if(dirX == -1 && dirY == -1){
			 tap_code(KC_LEFT);
		}

		else if(dirX == 1 && dirY == -1){
			 tap_code(KC_DOWN);
		}

		else if(dirX == -1 && dirY == 1){
			 tap_code(KC_UP);
		}

		else if(dirX == 1 && dirY == 1){
			 tap_code(KC_RIGHT);
		}
		
	}
}
 
void getRanges(){

	minorAxesRange = 90 - majorAxesRange;

	ranges[0] = majorAxesRange;
	ranges[1] = majorAxesRange + minorAxesRange;

	minorAxesRange += angleCorrector;
	majorAxesRange += angleCorrector;
 

	for(uint8_t i = 2; i < 7; i++){
		ranges[i] = ranges[i-1] + majorAxesRange;
		i++;
		ranges[i] = ranges[i-1] + minorAxesRange;
	} 
 
}

void angleToSignal(float alpha){}

 void axesEvaluation(uint8_t xPin, uint16_t xOrigin, uint8_t yPin, uint16_t yOrigin) {
	int directionX = 0;
	int directionY = 0;
	int8_t xPower = 0;
	int8_t yPower = 0;
	int8_t xStandard = 53;
	int8_t yStandard = 49;
	int8_t minimumRange = 3;
	int8_t axesDecider = 90;


	 //define directions and power of the direction
	if (axisCoordinate(xPin, xOrigin) < (xStandard - minimumRange)) {
		directionX = 1;
		xPower = calculatePower(axisCoordinate(xPin, xOrigin), xStandard, directionX);
	}

	if (axisCoordinate(xPin, xOrigin) > (xStandard + minimumRange)) {
		directionX = -1;
		xPower = calculatePower(axisCoordinate(xPin, xOrigin), xStandard, directionX);
	}

	if (axisCoordinate(yPin, yOrigin) < (yStandard - minimumRange)) {
		directionY = 1;
		yPower = calculatePower(axisCoordinate(yPin, yOrigin), yStandard, directionY);
	}

	if (axisCoordinate(yPin, yOrigin) > (yStandard + minimumRange)) {
		directionY = -1;
		yPower = calculatePower(axisCoordinate(yPin, yOrigin), yStandard, directionY);
	}
 
	//correct axes power 
	float powerAngle = asin(xPower/100);
	xPower = powerAngle/90 * 100;

	powerAngle = asin(yPower/100);
	yPower = powerAngle/90 * 100;

 
    // calculate angel and radius
	float r = sqrt((xPower * xPower) + (yPower + yPower));
	float alpha = 0;

	//calculate alpha depending on the quarter
	if(directionX == 1){
		alpha = r * sin(yPower);
		if(directionY == 1){
			alpha = 90 - alpha;
		} 
		
		if(directionY == 0) {
			alpha = 90;
		}

		if(directionY == -1) {	
			alpha = 90 + alpha;
		}
	} 

	if(directionX == -1){
		alpha = r * sin(yPower);
		if(directionY == 1){
			alpha = 270 + alpha;
		} 
		
		if(directionY == 0) {
			alpha = 270;
		}

		if(directionY == -1) {	
			alpha = 270 - alpha;
		}
	} 

	if(directionX == 0){
		if(directionY == 1){
			alpha = 0;
		} 
		
		if(directionY == 0) {
			alpha = 0;
		}

		if(directionY == -1) {	
			alpha = 180;
		}
	} 


	//correct the angle
	//rotate it for easier calculations
	alpha = alpha + (majorAxesRange / 2);	
	if(alpha > 360){
		alpha = alpha - 360;
	} 

	 //define decide between on key and two key action 
	 if((xPower + yPower) < axesDecider) {
		 if(xPower < yPower){	
			directionX = 0;
		 } else {	
			directionY = 0;
		 }
	 }


	angleToSignal(alpha);

	 if(xPower > 45 || yPower > 45){
		sendKeyboardSignal(directionX, directionY);
		sendKeyboardSignal(directionX, directionY);
		sendKeyboardSignal(directionX, directionY);
	 } else {
		sendKeyboardSignal(directionX, directionY);
	 }
}

