#include "DualVNH5019MotorShield.h"
#include "pid.h"
#include "sensor.h"
DualVNH5019MotorShield md;



////////////////////////////////////////////////////////////////////////////Encoders
#define EncoderLPinA 18//20
#define EncoderLPinB 16//19
#define EncoderRPinA 19
#define EncoderRPinB 17
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
int lineSpeed = 0;
int lineErrorLast = 0;
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


  if (x == 10) {
    pidEncoder();
    md.setSpeeds( 200 + speedEncoder, 200 - speedEncoder ); //200,250
  }
  if (x == 5) {
    sensorRead();


    //lineArr[0]==1 && lineArr[1]==1 && lineArr[2]==1 && lineArr[3]==1 && lineArr[4]==1 &&lineArr[5]==1 && lineArr[6]==1 && lineArr[7]==1
    if (lineCount == 8) {

      md.setBrakes(400, 400);
      delay(20000);
    }


    else if (lineArr[0] == 1   ) { //right
      countL = 0;
      countR = 0;
      md.setBrakes(400, 400);
      delay(1000);
      while ((abs(countR) + abs(countL)) / 2 < 930) {
        sensorRead();
        md.setSpeeds(250 , -250);
      }
      countL = 0;
      countR = 0;

      md.setBrakes(400, 400);
      delay(10);
    }

    else if ( lineArr[7] == 1)  { //left
      countL = 0;
      countR = 0;
      md.setBrakes(400, 400);
      delay(1000);
      while ((abs(countR) + abs(countL)) / 2 < 93 + 0) {
        sensorRead();
        md.setSpeeds(-250 , 250 );
      }
      countL = 0;
      countR = 0;

      md.setBrakes(400, 400);
      delay(10);
    }
    else {
      pidLine();
      pidEncoder();
      md.setSpeeds(100 +lineSpeed +speedEncoder, 100-lineSpeed- speedEncoder );

    }
  }

  if (x == 0) {
    x = -1;
    while (abs(countR + countL) < 20000) {
      tCurve(20000, 2, 7);
      pidEncoder;
      md.setSpeeds(200 - speedEncoder, 200  + speedEncoder);
    }
    if (countR > countL) {
      while (countR - countL > 0) {
        md.setSpeeds(200, -200);
      }
      md.setBrakes(400, 400);
    }
    else {
      while (countR - countL < 0) {
        md.setSpeeds(-200, 200);
      }
      md.setBrakes(400, 400);
    }
    md.setBrakes(100, 100);
    delay(5);
    md.setBrakes(200, 200);
    delay(5);
    md.setBrakes(300, 300);
    delay(5);
    md.setBrakes(400, 400);
    delay(5);
  }
  if (x == 2) {
    x = -1;
    while ((abs(countR) + abs(countL)) / 2 < 830) {
      pidTurn();
      md.setSpeeds(250 - speedTurn, -250 + speedTurn);
    }

    md.setBrakes(400, 400);
    delay(5);
  }

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






void readEncoderL()
{
  if (digitalRead(EncoderLPinB) == digitalRead(EncoderLPinA) )
  {
    countL = countL + 1; //may need to redefine positive and negative directions
  }
  else
  {
    countL = countL - 1;
  }
}

void readEncoderR()
{
  if (digitalRead(EncoderRPinB) == digitalRead(EncoderRPinA) )
  {
    countR = countR - 1; //may need to redefine positive and negative directions
  }
  else
  {
    countR = countR + 1;
  }

}







