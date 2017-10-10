#include "sensor.h"


///////////////////////////////////////////////////////////////////////////////////////////pidTurn
float errorTurn = 0;
float last_errorTurn = 0;
int speedTurn = 0;
//**********************************************************************************************



///////////////////////////////////////////////////////////////////////////////////////////pidEncoder
int speedEncoder = 0;
float last_errorEncoder = 0;
float errorEncoder = 0;
//**************************************************************************************************



/////////////////////////////////////////////////////////////////////////////////////////////pidLineFollow
int lineError = 0;
int lineArr[8] = {0};
int lineSum = 0;
int lineCount = 0;
int lineSpeed = 0;
int lineErrorLast = 0;
//**********************************************************************************************

long countL = 0;
long countR = 0;

extern int senVal;
extern int pin;
extern int senCalWhite;
extern int senCalBlack;




void pidLine() {
  sensorRead();
  lineSpeed = ( 1 *  lineError + 0.1  * ( lineError -  lineErrorLast) , -50, 50);
  lineErrorLast = lineError;

}





void pidTurn() {
  errorTurn = 830 - (abs(countR) + abs(countL)) / 2;
  speedTurn = (0.2 * errorTurn + 1 * (errorTurn - last_errorTurn) , -100, 100);
  last_errorTurn = errorTurn;
}


void pidEncoder() {
  errorEncoder = (countL - countR);
  speedEncoder = (0.05 * errorEncoder + 0 * (errorEncoder - last_errorEncoder) , -30, 30);
  last_errorEncoder = errorEncoder;
}
