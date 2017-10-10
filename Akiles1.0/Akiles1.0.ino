#include "DualVNH5019MotorShield.h"

DualVNH5019MotorShield md;



////////////////////////////////////////////////////////////////////////////Encoders
#define EncoderLPinA 18
#define EncoderLPinB 19
#define EncoderRPinA 20
#define EncoderRPinB 21
long countL = 0;
long countR = 0;
//*********************************************************************//


////////////////////////////////////////////////////////////////////////////////////////////pidTurn
float errorTurn = 0;
float last_errorTurn = 0;
int speedTurn = 0;
//**********************************************************************************************



///////////////////////////////////////////////////////////////////////////////////////////pidEncoder
int speedEncoder = 0;
float last_errorEncoder = 0;
float errorEncoder = 0;
//**************************************************************************************************


/////////////////////////////////////////////////////////////////////////////////////////////sensor
int senVal[8] = {0};
int pin[8] = {62, 63, 64, 65, 66, 67, 68, 69};
int senCalWhite[8] = {0};
int senCalBlack[8] = {0};
//**********************************************************************************************




/////////////////////////////////////////////////////////////////////////////////////////////pidLineFollow
int lineError = 0;
int lineArr[8] = {0};
int lineSum = 0;
int lineCount = 0;
float lineSpeed = 0;
int lineErrorLast = 0;
int integratedLineError = 0;
//**********************************************************************************************

int x = 5;
int iniValT = 0;
int endValT = 0;



void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while (1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while (1);
  }
}

void setup()
{
  Serial.begin(9600);


  pinMode(EncoderLPinA, INPUT);
  pinMode(EncoderLPinB, INPUT);
  pinMode(EncoderRPinA, INPUT);
  pinMode(EncoderRPinB, INPUT);
  digitalWrite(EncoderLPinA, LOW);
  digitalWrite(EncoderLPinB, LOW);
  digitalWrite(EncoderRPinA, LOW);
  digitalWrite(EncoderRPinB, LOW);
  attachInterrupt(digitalPinToInterrupt(EncoderLPinA), readEncoderL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EncoderRPinA), readEncoderR, CHANGE);


  md.init();
  calibirate();
}




void loop()
{

  sensorRead();


  //lineArr[0]==1 && lineArr[1]==1 && lineArr[2]==1 && lineArr[3]==1 && lineArr[4]==1 &&lineArr[5]==1 && lineArr[6]==1 && lineArr[7]==1
  //      if (lineCount == 8) {
  //
  //        md.setBrakes(400, 400);
  //        delay(20000);
  //      }
  //
  //
  if (lineArr[4]==1 &&lineArr[5]==1 && lineArr[6]==1 && lineArr[7]==1 && lineArr[0] == 0 && countL>900) { //right

    delay(100);

    md.setBrakes(400, 400);
    forward(200);
    turnRight();
delay(200);

  }


  if (lineArr[0]==1 &&lineArr[1]==1 && lineArr[2]==1 && lineArr[3]==1 && lineArr[7] == 0&& countL>900) { //right

    delay(100);

    md.setBrakes(400, 400);
    forward(200);
    turnLeft();
delay(200);
  }
  //  if (lineArr[0] == 1  && lineCount > 4 ) { //right
  //    turnLeft();
  //
  //  }
  //
  //    else if ( lineArr[7] == 1)  { //left
  //      countL = 0;
  //      countR = 0;
  //      md.setBrakes(400, 400);
  //      delay(1000);
  //      while ((abs(countR) + abs(countL)) / 2 < 93 + 0) {
  //        sensorRead();
  //        md.setSpeeds(-250 , 250 );
  //      }
  //      countL = 0;
  //      countR = 0;
  //
  //      md.setBrakes(400, 400);
  //      delay(10);
  //    }
  //    else {
  pidLine();
  md.setSpeeds(100 - lineSpeed , 100 + lineSpeed );
  //delay(50);
  //          md.setBrakes(400,400);
  //          delay(50);

  //    }

  //turnLeft();
  //
  //delay(10000000);


}


void forward(int enCount) {
  countL = 0;
  countR = 0;
  delay(200);
  while (abs(countL + countR) / 2 < enCount) {
    md.setSpeeds(150, 150);
  }
  md.setBrakes(400, 400);
  delay(200);
countL = 0;
  countR = 0;
}
void turnLeft() {
  countL = 0;
  countR = 0;
  md.setBrakes(400, 400);
  delay(1000);
  while ((abs(countR) + abs(countL)) / 2 < 630) {
    sensorRead();
    md.setSpeeds(-250 , 250 );
  }
  countL = 0;
  countR = 0;
  md.setBrakes(400, 400);
  delay(10);
}



void turnRight() {
  countL = 0;
  countR = 0;
  md.setBrakes(400, 400);
  delay(1000);
  while ((abs(countR) + abs(countL)) / 2 < 630) {
    sensorRead();
    md.setSpeeds(250 , -250 );
  }
  countL = 0;
  countR = 0;
  md.setBrakes(400, 400);
  delay(10);
}





void sensorRead() {
  lineCount = 0;
  lineSum = 0;
  for (int i = 0; i < 8; i++) {
    senVal[i] = map(analogRead(pin[i]) , senCalBlack[i], senCalWhite[i], 0, 1024);

    if (senVal[i] > 700) {
      lineArr[i] = 1;
      lineSum += 1000 * i;
      lineCount++;
    }
    else {
      lineArr[i] = 0;
    }
    Serial.print(lineArr[i]);
    Serial.print("\t");


  }
  Serial.print(lineCount);
  Serial.print("\t");
  if (lineCount != 0) {
    lineError = 3500 - lineSum / lineCount;
  }
  else {
    lineError = 0;

  }
  Serial.print(lineError);
  Serial.print("\t");

}


int getSensorVal(int x, int pin) {
  int sum = 0;
  for (int i = 0; i < x; i++) {
    sum += analogRead(pin);
  }
  return (sum / x);
}

void calibirate() {
  for (int i = 0; i < 8; i++) {
    senCalWhite[i] = getSensorVal(10, pin[i]);
    Serial.print(senCalWhite[i]);
    Serial.print("\t");
  }
  Serial.println();
  delay(5000);


  for (int i = 0; i < 8; i++) {
    senCalBlack[i] = getSensorVal(10, pin[i]);
    Serial.print(senCalBlack[i]);
    Serial.print("\t");
  }
  Serial.println();
  delay(3000);

}






void tCurve(int enCount, int startVal, int endVal) {

  if (abs(countR + countL) > enCount / 10 * startVal) {
    iniValT = 0;
  }
  else {
    iniValT = (enCount / 10 * startVal - abs(countR + countL)) * 0.07;
  }
  if (abs(countR + countL) < enCount / 10 * endVal) {
    endValT = 0;
  }
  else {
    endValT = (abs(countR + countL) - enCount / 10 * endVal) * 0.15;
  }


}



void pidLine() {
  //  integratedLineError += lineError;
  lineSpeed = constrain( 0.03 * lineError +   0 * ( lineError -  lineErrorLast) + 0 * constrain(integratedLineError, -10, 10) , -50, 50);
  lineErrorLast = lineError;

}





void pidTurn() {
  errorTurn = 830 - (abs(countR) + abs(countL)) / 2;
  speedTurn = constrain(0.2 * errorTurn + 1 * (errorTurn - last_errorTurn) , -100, 100);
  last_errorTurn = errorTurn;
}


void pidEncoder() {
  errorEncoder = (countL - countR);
  speedEncoder = constrain(0.05 * errorEncoder + 0 * (errorEncoder - last_errorEncoder) , -30, 30);
  last_errorEncoder = errorEncoder;
}


void readEncoderL()
{
  if (digitalRead(EncoderLPinB) == digitalRead(EncoderLPinA) )
  {
    countL = countL - 1; //may need to redefine positive and negative directions
  }
  else
  {
    countL = countL + 1;
  }
}

void readEncoderR()
{
  if (digitalRead(EncoderRPinB) == digitalRead(EncoderRPinA) )
  {
    countR = countR + 1; //may need to redefine positive and negative directions
  }
  else
  {
    countR = countR - 1;
  }

}







