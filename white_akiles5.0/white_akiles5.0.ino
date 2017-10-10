#include "DualVNH5019MotorShield.h"

DualVNH5019MotorShield md;
#include <Servo.h>


////////////////////////////////////////////////////////////////////////////Encoders
#define EncoderLPinA 18
#define EncoderLPinB 19
#define EncoderRPinA 20
#define EncoderRPinB 21
long countL = 0;
long countR = 0;
//*********************************************************************//

#define yellow 23
#define blue 25
#define green 27
#define white 29
#define red 31

Servo gripper;
Servo arm;
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


int node=0;
int box=0;



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

while(node==0){
  sensorRead();
  pidWhite();


  //right turn condition
  if (lineArr[4]==lineArr[7] && lineArr[5]==lineArr[7] && lineArr[6]==lineArr[7] &&lineArr[3]==lineArr[7] && lineArr[0] != lineArr[7]&& lineArr[1]!=lineArr[7] && countL>800) { 

    delay(100);

    md.setBrakes(400, 400);
    forward(200);
    turnRight();
    delay(200);
    countL=0;
    countR=0;
    node=1;

  }
  
}


while(node==1){
  sensorRead();
  pidLine();
  
  // put T junction condtition with countL>600
  if(){

    forward(200);
    turnLeft();   //left hand rule
    delay(200);
    countL=0;
    countR=0;
    node=2; 
  }
}


while(node==2){
  sensorRead();
  pidBlack();
  // put T junction condtition
  if(){

    //get fornt sonar reading
    if(disF<10){
      //grab the box
      box=1;
    }
    forward(200);
    turnAbout();   //left hand rule
    delay(200);
    countL=0;
    countR=0;
    node=3; 
  }
}


while(node==3){
  sensorRead();
  pidBlack();
  // put T junction condtition and box==0
  if(){
    
    forward(200);
    turnLeft();   //left hand rule
    delay(200);
    countL=0;
    countR=0;
    node=4; 
  }
  

  //t junction and box==1

   if(){
    
    forward(200);
    turnRight();   //left hand rule
    delay(200);
    countL=0;
    countR=0;
    node=8; 
  }
}



while(node==4){
  sensorRead();
  pidBlack();
  // put T junction condtition
  if(){

    //get fornt sonar reading
    if(disF<10){
      //grab the box
      box=1;
    }
    forward(200);
    turnAbout();   //left hand rule
    delay(200);
    countL=0;
    countR=0;
    node=5; 
  }
}


while(node==5){
  sensorRead();
  pidBlack();
  // put T junction condtition and box==0
  if(){
    
    forward(200);
    turnLeft();   //left hand rule
    delay(200);
    countL=0;
    countR=0;
    node=6; 
  }
  

  //t junction and box==1

   if(){
    
    forward(200);
    delay(200);
    countL=0;
    countR=0;
    node=8; 
  }
}


while(node==6){
  sensorRead();
  pidBlack();
  // put T junction condtition
  if(){

    //get fornt sonar reading
    if(disF<10){
      //grab the box
      box=1;
    }
    forward(200);
    turnAbout();   //left hand rule
    delay(200);
    countL=0;
    countR=0;
    node=7; 
  }
}



while(node==7){
  sensorRead();
  pidBlack();
  // put T junction condtition and box==0
  if(){
    
    forward(200);
    delay(200);
    countL=0;
    countR=0;
    node=3; 
  }
  


  //t junction and box==1
   if(){
    
    forward(200);
    turnLeft();
    delay(200);
    countL=0;
    countR=0;
    node=8; 
  }
}



while(node==8){
  sensorRead();
  pidLine();


  //left turn condition
  if () { 

    delay(100);

    md.setBrakes(400, 400);
    forward(200);
    turnLeft();
    delay(200);
    countL=0;
    countR=0;
    node=9;

  }
  
}


while(node==9){
  sensorRead();
  pidBlack();


  //lT junction condition
  if () { 

    delay(100);

    md.setBrakes(400, 400);
    forward(200);
    
//drop the box

  }
  
}


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
  while ((abs(countR) + abs(countL)) / 2 < 760) {
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

    if (senVal[i] >500) {
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

  if ((lineArr[0]==1)&&(lineArr[7]==1)&&(lineCount!=8)){
  if (lineCount != 0) {
    lineError = 3500 -(28000-lineSum) /( 8-lineCount);
  }
  else {
    lineError = 0;

  }
  Serial.print(lineError);
  Serial.print("\t");

}

else if((lineArr[0]==0)&&(lineArr[7]==0)){
   if (lineCount != 0) {
    lineError = 3500 -(lineSum) /( lineCount);
  }
  else {
    lineError = 0;

  }
  Serial.print(lineError);
  Serial.print("\t");

}
  

//void error_white(){}

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
  lineSpeed = constrain( 0.2* lineError +   0 * ( lineError -  lineErrorLast) + 0 * constrain(integratedLineError, -10, 10) , -50, 50);
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

void gripOn(){
    
    for (int pos = 135; pos <= 155; pos += 1) { // goes from 135 degrees to 170 degrees
    // in steps of 1 degree
    gripper.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void gripOff(){
    for (int pos = 155; pos >= 135; pos -= 1) { // goes from 170 degrees to 135 degrees
    gripper.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
  void armDown(){
    for (int pos = 150; pos >= 60; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    arm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void armUp(){
    for (int pos = 60; pos <= 150; pos += 1) { // goes from 180 degrees to 0 degrees
    arm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
 
