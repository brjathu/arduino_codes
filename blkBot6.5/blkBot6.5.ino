#include <NewPing.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "SharpIR.h"
#include "DualVNH5019MotorShield.h"


#include "pid.h"
DualVNH5019MotorShield md;


/////////////////////////////////////////front sonar
#define triggerForward  31
#define echoForward     30
#define maxDistanceForward 300
float distanceForward = 0;
float distanceForwardLast = 0;
NewPing sonarFrontForward(triggerForward, echoForward, maxDistanceForward);

/////////////////////////////////////////left front sonar
//#define triggerLFront  23
//#define echoLFront     22
//#define maxDistance 180
//float distanceLeftFront = 0;
//float distanceLeftFrontLast = 0;
//NewPing sonarLeftFront(triggerLFront, echoLFront, maxDistance);
/////////////////////////////////////////////////right front sonar
//#define triggerRFront  25
//#define echoRFront     24
//float distanceRightFront = 0;
//float distanceRightFrontLast = 0;
//NewPing sonarRightFront(triggerRFront, echoRFront, maxDistance);
//
///////////////////////////////////////////left back sonar
//#define triggerLBack  29
//#define echoLBack     28
//#define maxDistance 200
//float distanceLeftBack = 0;
//float distanceLeftBackLast = 0;
//NewPing sonarLeftBack(triggerLBack, echoLBack, maxDistance);
/////////////////////////////////////////////////right back sonar
//#define triggerRBack  27
//#define echoRBack     26
//float distanceRightBack = 0;
//float distanceRightBackLast = 0;
//NewPing sonarRightBack(triggerRBack, echoRBack, maxDistance);
//
//int pidMax;
//float error1;
//float error2;

/////////////////////////////////////////////////////////PID
float Kp = 5;//6
float Ki = 0;//0.02;
float Kd = 125;//70;
float pTerm, iTerm, dTerm, integrated_error, last_error;
float pTerm2, iTerm2, dTerm2, integrated_error2, last_error2;

#define   GUARD_GAIN   10.0
float error = 0;
const float K = 1;//1.9
int speed1 = 0;
int speed2 = 0;
int speed = 0;


//////////////////////////////////////////////encoder PID
int speedEncoder = 0;
float last_errorEncoder = 0;
float errorEncoder = 0;

float last_errorR = 0;
float last_errorL = 0;

int temp = 0;
float errorTurn = 0;
float last_errorTurn = 0;
int speedTurn = 0;
/////////////////////////////////////////////////////////////////////////////0-


////////////////////////////////////////////////////////////////////////////Encoders
#define EncoderLPinA 18//20
#define EncoderLPinB 19//19
#define EncoderRPinA 20
#define EncoderRPinB 21
long countL = 0;
long countR = 0;

/////////////////////////////////////////////////////////////////////////////Turning
int rspeed;
int turnValue = 428;  //450
int straightValue = 2400;
//*********************************************************************//

#define redLed 15
#define orangeLed 14


void setup() {
  // put your setup code here, to run once:
  md.init();
  Serial.begin(115200);
  //pinMode(53,OUTPUT);

  //encoders
  countL = 0;
  countR = 0;
  pinMode(EncoderLPinA, INPUT);
  pinMode(EncoderLPinB, INPUT);
  pinMode(EncoderRPinA, INPUT);
  pinMode(EncoderRPinB, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(orangeLed, OUTPUT);
  digitalWrite(redLed, LOW);
  digitalWrite(orangeLed, LOW);
  digitalWrite(EncoderLPinA, LOW);
  digitalWrite(EncoderLPinB, LOW);
  digitalWrite(EncoderRPinA, LOW);
  digitalWrite(EncoderRPinB, LOW);
  attachInterrupt(digitalPinToInterrupt(EncoderLPinA), readEncoderL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EncoderRPinA), readEncoderR, CHANGE);
  sonarForward();
  sonarForward();
  pid();
  //md.setSpeeds(220,250);
  //delay(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(distanceForward);
  Serial.print(",");
  Serial.print(distanceLeftFront);
  Serial.print(",");
  Serial.print(distanceRightFront);
  Serial.print("\t");
  Serial.print(distanceLeftBack);
  Serial.print(",");
  Serial.print(distanceRightBack);
  Serial.println("\t");
  //  Serial.print(error1);
  //  Serial.print("\t");
  //  Serial.print(error2);
  //  Serial.print("\t");
  //  Serial.print(error);
  //  Serial.print("\t");
  //    Serial.print(countL);
  //    Serial.print("\t");
  //    Serial.print(countR);
  //    Serial.println("\t");
  //pid();
  //  forwardEncoder();
  checkWalls();
}

void pidTurn() {
  errorTurn = turnValue - (abs(countR) + abs(countL)) / 2;    //480
  speedTurn = constrain(0.1 * errorTurn + 1 * (errorTurn - last_errorTurn) , -50, 50);
  last_errorTurn = errorTurn;
}


void turnLeft() { /// turn1
  countL = 0;
  countR = 0;
  while ((abs(countR) + abs(countL)) / 2 <= (turnValue - 8)) {
    pidTurn();
    md.setM1Speed(-speedTurn - 200);
    md.setM2Speed(speedTurn + 250);
  }
  md.setBrakes(400, 400);
  delay(1000);
  countL = 0;
  countR = 0;
}


void turnRight() {
  countL = 0;
  countR = 0;
  while ((abs(countR) + abs(countL)) / 2 <= turnValue) {
    pidTurn();
    md.setM1Speed(speedTurn + 250);
    md.setM2Speed(-speedTurn - 250);
  }
  md.setBrakes(400, 400);
  delay(1000);
  countL = 0;
  countR = 0;
}


void forwardCorrection() {
  delay(10);
  sonarForward();
  if (distanceForward >= 12) {
    countL = 0;
    countR = 0;
    while (abs(countR + countL) < 400) {
      md.setSpeeds(200  , 250  );

    }
  }
  else if (distanceForward >= 11) {
    countL = 0;
    countR = 0;
    while (abs(countR + countL) < 200) {
      md.setSpeeds(200  , 250  );

    }
  }
  md.setBrakes(400, 400);
  delay(300);

}

void forwardEncoder() {
  countL = 0;
  countR = 0;

  while (abs(countL + countR) < straightValue) {

    //digitalWrite(redLed, LOW);


    if (0 < distanceForward && distanceForward <= 12) {
      md.setBrakes(400, 400);
//      digitalWrite(redLed, LOW);
      break;
    }
    pidEncoder();
    // md.setSpeeds(190 - speed1 - speed2 - speedEncoder, 215 + speed1 + speed2 + speedEncoder);
    md.setSpeeds(200 , 250 );    //200,250


  }
}
void forward() {
  //if (temp==1){
  // digitalWrite(redLed, LOW);
  // temp=0;
  // }

  countL = 0;
  countR = 0;

  while (abs(countL + countR) < straightValue) {

    //digitalWrite(redLed, LOW);
    pid();

    if (0 < distanceForward && distanceForward <= 12) {
      md.setBrakes(400, 400);
      digitalWrite(orangeLed, HIGH);
      delay(1000);
      digitalWrite(orangeLed,LOW);
      break;
    }
    pidEncoder();

    if (error2 >= 3 && error2 < -4) {
      md.setSpeeds(200 - speed - speedEncoder, 250 + speed + speedEncoder);
    }
    else if (error2 <= -3 && error2 > -4) {
      md.setSpeeds(250 - speed - speedEncoder, 200 + speed + speedEncoder);
    }
    else if (error2 >= 4) {
      md.setSpeeds(180 - speed - speedEncoder, 250 + speed + speedEncoder);
    }
    else if (error2 <= -4) {
      md.setSpeeds(250 - speed - speedEncoder, 200 + speed + speedEncoder);
    }
    else {
      // md.setSpeeds(190 - speed1 - speed2 - speedEncoder, 215 + speed1 + speed2 + speedEncoder);
      md.setSpeeds(200 - speed - speedEncoder, 250 + speed + speedEncoder);    //200,250
    }

  }
  //digitalWrite(redLed, LOW);
  //      md.setBrakes(400, 400);
  //      delay(100);
}


/////////////////////////////////////









void checkWalls2() {
  sonarFront();
  sonarBack();
  sonarForward();
  float angle;
  float disL;
  float disR;
  float disF;
  int disLimit = 25;
  int disLimitF = 14;
  disL = 0.5 * (distanceLeftFront + distanceLeftBack);
  disR = 0.5 * (distanceRightFront + distanceRightBack);
  disF = distanceForward;
  if ((disL - disR) != 0) {
    angle = disF / (disL - disR);
  }
  else {
    angle = 100;
  }

  if (angle > 0 && angle < 2) {
    turnLeft();
    forward();
  }
  else if (angle > 2 || angle < -0.5) {
    forward();
  }
  else if (angle < 0 && angle > -0.5) {
    turnRight();
    forward();
  }
  else {
    delay(5000);
  }

}
void checkWalls() {
  sonarFront();
  sonarBack();
  sonarForward();
  float disL;
  float disR;
  float disF;
  int disLimit = 29;
  int disLimitF = 14;
  //  disL = 0.5 * (distanceLeftFront + distanceLeftBack);
  //  disR = 0.5 * (distanceRightFront + distanceRightBack);

  disL = distanceLeftFront;
  disR = distanceRightFront;
  disF = distanceForward;
  if (disF == 0 || disF < 0) {
    disF = 12;
  }
  if (disL < disLimit && disR > disLimit  && disF > disLimitF) {
    forward();
    //     |
  }
  else if (disL > disLimit  && disR > disLimit  && disF <= disLimitF ) {
    md.setBrakes(400, 400);
    delay(500);
    forwardCorrection();
    sonarFront();
    sonarBack();
    sonarForward();
    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    disR = 0.5 * (distanceRightFront + distanceRightBack);
    disF = distanceForward;
    if (disL > disLimit  && disR > disLimit  && disF <= disLimitF ) {
      turnLeft();
      forward();
    }
    //      _
    //
  }
  else if (disL > disLimit  && disR < disLimit  && disF > disLimitF ) { //wall at right
    md.setBrakes(400, 400);
    delay(500);
    md.setSpeeds(0, 0);
    sonarFront();
    sonarBack();
    sonarForward();
    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    disR = 0.5 * (distanceRightFront + distanceRightBack);
    disF = distanceForward;
    if (disL > disLimit  && disR < disLimit  && disF > disLimitF ) {
      turnLeft();
      forward();
    }
    //            |
  }
  else if (disL < disLimit  && disR > disLimit  && disF <= disLimitF ) {
    md.setBrakes(400, 400);
    delay(500);
    forwardCorrection();
    sonarFront();
    sonarBack();
    sonarForward();
    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    disR = 0.5 * (distanceRightFront + distanceRightBack);
    disF = distanceForward;
    if (disL < disLimit  && disR > disLimit  && disF <= disLimitF ) {
      turnRight();
      forward();
    }
    //     _
    //    |

    //       delay(100);
    //    md.setBrakes(400,400);
    //    delay(1000);
    //
    //    digitalWrite(optionalLed,HIGH);
    //    turnRight(150);
    //    delay(1000);
    //    digitalWrite(optionalLed,LOW);

  }
  else if (disL < disLimit  && disR < disLimit  && disF > disLimitF ) {
    forward();
    //    |   |
  }
  else if (disL > disLimit  && disR < disLimit  && disF <= disLimitF ) {
    temp = 1;
    md.setBrakes(400, 400);
    delay(500);
    forwardCorrection();
    sonarFront();
    sonarBack();
    sonarForward();
    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    disR = 0.5 * (distanceRightFront + distanceRightBack);
    disF = distanceForward;
    if (disL > disLimit  && disR < disLimit  && disF <= disLimitF ) {
      turnLeft();
      //    digitalWrite(orangeLed, HIGH);
      //    digitalWrite(redLed, HIGH);
      //    delay(2000);
      //    digitalWrite(orangeLed, LOW);

      forward();
      //    digitalWrite(orangeLed, HIGH);
      //    delay(5000);
      //    digitalWrite(orangeLed, LOW);
    }
    //forward();
    //     _
    //      |

    //     delay(100);
    //    md.setBrakes(400,400);
    //    delay(1000);
    //
    //    digitalWrite(optionalLed,HIGH);
    //    turnLeft(150);
    //    delay(1000);
    //    digitalWrite(optionalLed,LOW);

  }
  else if (disL < disLimit  && disR < disLimit  && disF < disLimitF ) {
    md.setBrakes(400, 400);
    delay(500);
    sonarFront();
    sonarBack();
    sonarForward();
    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    disR = 0.5 * (distanceRightFront + distanceRightBack);
    disF = distanceForward;
    if (disL < disLimit  && disR < disLimit  && disF < disLimitF ) {
      turnRight();
      turnRight();
    }
    //   _
    //  | |
  }
  else {
    digitalWrite(redLed,HIGH);
  }
}
/////////////////////////////////sonar
void sonarForward() {
  //distanceForwardLast = distanceForward;
  //  float temp1 = 0;
  //  float temp2 = 0;
  //  temp1 = sonarFrontForward.ping_cm();
  //  temp2 = sonarFrontForward.ping_cm();
  //  distanceForward = 0.5 * (temp1 + temp2);
  distanceForward = sonarFrontForward.ping_cm();
  if ((distanceForward >= 40) || (distanceForward == 0)) {
    distanceForward = 40;
  }
}

void sonarFront() {
  //  float temp1 = 0;
  //  float temp2 = 0;
  //  distanceLeftFrontLast = distanceLeftFront;
  //  distanceRightFrontLast = distanceRightFront;
  //  temp1 = sonarLeftFront.ping_cm();
  //  temp2 = sonarLeftFront.ping_cm();
  //  distanceLeftFront = 0.5 * (temp1 + temp2);
  //  temp1 = sonarRightFront.ping_cm();
  //  temp2 = sonarRightFront.ping_cm();
  //  distanceRightFront = 0.5 * (temp1 + temp2);
  distanceLeftFront = sonarLeftFront.ping_cm();
  distanceRightFront = sonarRightFront.ping_cm();
  if (distanceLeftFront >= 30 || distanceLeftFront == 0) {
    distanceLeftFront = 30;
  }
  if (distanceRightFront >= 30 || distanceRightFront == 0) {
    distanceRightFront = 30;
  }
}

void sonarBack() {
  //  float temp1 = 0;
  //  float temp2 = 0;
  //  distanceLeftBackLast = distanceLeftBack;
  //  distanceRightBackLast = distanceRightBack;
  //  temp1 = sonarLeftBack.ping_cm();
  //  temp2 = sonarLeftBack.ping_cm();
  //  distanceLeftBack = 0.5 * (temp1 + temp2);
  //  temp1 = sonarRightBack.ping_cm();
  //  temp2 = sonarRightBack.ping_cm();
  //  distanceRightBack =  0.5 * (temp1 + temp2);
  distanceLeftBack = sonarLeftBack.ping_cm();
  distanceRightBack = sonarRightBack.ping_cm();
  if (distanceLeftBack >= 30 || distanceLeftBack == 0) {
    distanceLeftBack = 30;
  }
  if (distanceRightBack >= 30 || distanceRightBack == 0) {
    distanceRightBack = 30;
  }
}


void pidEncoder() {
  errorEncoder = ((countL) - countR);
  speedEncoder = constrain(0.1 * errorEncoder + 0.2  * (errorEncoder - last_errorEncoder) , -4, 4); //10
  last_errorEncoder = errorEncoder;
}

//void pid() {
//  //  if (dis > 20) {    // right wall following
//  //
//  //    error = (7.5 - disRight);
//  //
//  //    speed = constrain((error * 6 + 50 * (error - last_errorR) ), -pidMax, pidMax);
//  //
//  //    last_errorR = error;
//  //
//  //  }
//  //  else if (disRight > 20) {    // left wall following
//  //
//  //    error = (disLeft - 7.5);
//  //    speed = constrain((error * 6 + 50 * (error - last_errorL) ), -pidMax, pidMax);
//  //
//  //    last_errorL = error;
//  //  }
//  //
//  //
//  //  else {
//  //
//  sonarFront();
//  sonarBack();
//  sonarForward();
//  pidMax = 120;
//  if ((distanceLeftFront == 30) || (distanceLeftBack == 30)) { //right wall follow
//    //  md.setBrakes(400,400);
//    //  delay(5000);
//    error = (7.5 - (0.5 * (distanceRightFront + distanceRightBack))) - (distanceRightFront - distanceRightBack);
//
//    speed = constrain((error * 8 + 160 * (error - last_errorR) ), -pidMax, pidMax);
//
//    last_errorR = error;
//  }
//  else if (((distanceRightFront) == 30) || (distanceRightBack == 30)) { // left wall follow
//    //  md.setBrakes(400,400);
//    //  delay(5000);
//    error = ((0.5 * (distanceLeftBack + distanceLeftFront)) - 7.5) + (distanceLeftFront - distanceLeftBack);
//    speed = constrain((error * 8 + 150 * (error - last_errorL) ), -pidMax, pidMax);
//
//    last_errorL = error;
//  }
//  else if (((distanceRightFront == 30) && (distanceLeftFront == 30)) || ((distanceRightBack == 30) && (distanceLeftBack == 30))) {
////    errorEncoder = ((countL) - countR);
////    speedEncoder = constrain(0.2 * errorEncoder + 0.2  * (errorEncoder - last_errorEncoder) , -50, 50);
////    last_errorEncoder = errorEncoder;
////    speed = speedEncoder;
//    //forwardEncoder();
//    speed = 10;
//  }
//  else {
//    pidMax = 120;
//    error1 = 0.5 * ((distanceLeftFront - distanceRightFront) + (distanceLeftBack - distanceRightBack));
//    error2 = 0.5 * ((distanceLeftFront - distanceLeftBack) - (distanceRightFront - distanceRightBack)); //note
//    error = error1 + error2;
//    pTerm = Kp * (error);
//    integrated_error += error;
//    iTerm = Ki * constrain(integrated_error, -GUARD_GAIN, GUARD_GAIN);
//    dTerm = Kd * (error - last_error);
//    last_error = error;
//    speed = constrain(K * (pTerm + dTerm + iTerm), -pidMax, pidMax);
//    //  pTerm = 6 * (error1);
//    //  integrated_error += error1;
//    //  iTerm = Ki * constrain(integrated_error, -GUARD_GAIN, GUARD_GAIN);
//    //  dTerm = Kd * (error1 - last_error);
//    //  last_error = error1;
//    //  speed1 = constrain(K * (pTerm + dTerm + iTerm), -pidMax, pidMax);
//    //  //----------------------------------------------------------------------------
//    //  pTerm2 = 12 * (error2);
//    //  integrated_error2 += error2;
//    //  iTerm2 = Ki * constrain(integrated_error2, -GUARD_GAIN, GUARD_GAIN);
//    //  dTerm2 = Kd * (error2 - last_error2);
//    //  last_error2 = error2;
//    //  speed2 = constrain(K * (pTerm2 + dTerm2 + iTerm2), -pidMax, pidMax);
//  }
//}














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
