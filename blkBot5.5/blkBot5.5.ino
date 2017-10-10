#include <avr/io.h>
#include <avr/interrupt.h>
#include "DualVNH5019MotorShield.h"
#include <SharpIR.h>
#include <Servo.h>
#include <QTRSensors.h>
#include <EEPROM.h>
#include <NewPing.h>
DualVNH5019MotorShield md;




/////////////////////////////////////////left sonar
#define triggerLeft  50
#define echoLeft     51
#define maxDistance 300
int distanceLeft;
int detectBlack;
NewPing sonarLeft(triggerLeft, echoLeft, maxDistance);
///////////////////////////////////////////////right sonar
#define triggerRight  52
#define echoRight     53
int distanceRight;
int detectWhite;
NewPing sonarRight(triggerRight, echoRight, maxDistance);

/////////////////////////////////////////////////////////////////////IR
#define IRL A2
#define IRR A3
#define IRF A4
#define IRDigi 22


#define model 1080
SharpIR sharpl(IRL, 25, 93, model);
SharpIR sharpr(IRR, 25, 93, model);
SharpIR sharpf(IRF, 25, 93, model);
int disLeft = 0;
int disRight = 0;
int disF = 0;
int digiOut = 0;

int disL = 0;
int disR = 0;

int object = 0; /// to find an object infront
//*********************************************************************//





//////////////////////////////////////////////////////////////indicators

#define blackLed 24 //green led
#define whiteLed 26 //yellow led
#define optionalLed 28 // red led
//*********************************************************************//



/////////////////////////////////////////////////////////PID
float Kp = 6;
float Ki = 0.02;
float Kd = 70;
float pTerm, iTerm, dTerm, integrated_error, last_error;
#define   GUARD_GAIN   10.0
float error = 0;
const float K = 1;//1.9
int speed = 0;


float errorTurn = 0;
float last_errorTurn = 0;
int speedTurn = 0;

int speedEncoder = 0;
float last_errorEncoder = 0;
float errorEncoder = 0;

float last_errorR = 0;
float last_errorL = 0;

//*********************************************************************//


///////////////////////////////////////////////////////////////SensorArray
#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2
int sensorSum = 0;

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {
  23, 25, 27, 29, 31, 33, 35, 37
},
NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

//*********************************************************************//



/////////////////////////////////////////////////////////////////////////////ARM
Servo microServo;
Servo bigServo;
//*********************************************************************//




////////////////////////////////////////////////////////////////////////////Encoders
#define EncoderLPinA 18//20
#define EncoderLPinB 16//19
#define EncoderRPinA 19
#define EncoderRPinB 17
long countL = 0;
long countR = 0;
//*********************************************************************//




//////////////////////////////////////extras
int x = 1;
int turn = 0;
int pos = 0;
int disLimit = 29;
int disLimitF = 15;



int pidMax = 100;
int pidEncoderMax = 10;
//*********************************************************************//






////////////////////////////////////////////////////////////////////////MAP
int arena[7][7] = {0};
int notFilled [7][2] = {0};
int initialDiri [7][7] = {0};
int initialDirj [7][7] = {0};
int dirBot[2];
int deadEnd[10][2];
int deadFound = 0;
int nextJunction = 0;
int boxCount = 0;
int i = 0;
int j = 0;
//********************************************************************



/////////////////////////////////////////////////////////////////////////////////////////////////EGGS
int eggCord[2][2] = {0};
int eggsFound = 0;
int endFound = 0;


//**************************************************************************************************************





//////////////////////////////////////////////////////////////////////////////////////////////FLOODFILL
//int Maze[7][7] = {{6, 5, 4, 3, 4, 5, 6},
//  {5, 4, 3, 2, 3, 4, 5},
//  {4, 3, 2, 1, 2, 3, 4},
//  {3, 2, 1, 0, 1, 2, 3},
//  {4, 3, 2, 1, 2, 3, 4},
//  {5, 4, 3, 2, 3, 4, 5},
//  {6, 5, 4, 3, 4, 5, 6}
//};



int Maze[7][7] = {0};
//int wallMaze[7][7][4] = {0};

int wallMaze[7][7][4] =  {{{1, 1, 1, 1}, {1, 1, 1, 1} , {1, 1, 1, 1} , {1, 1, 1, 1} , {1, 1, 1, 1}, {1, 1, 1, 1} , {1, 1, 1, 1}},
  {{1, 1, 1, 1} , {1, 1, 1, 1}, {1, 1, 1, 1} , {1, 1, 1, 1} , {1, 1, 1, 1}, {1, 1, 1, 1} , {1, 1, 1, 1}},
  {{1, 1, 1, 1 }, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
  {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
  {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
  {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
  {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}}
};


//int wallMaze[7][7][4] =  {{{0, 1, 1, 1}, {0, 1, 0, 1} , {1, 0, 0, 1} , {0, 1, 1, 1} , {0, 1, 0, 1}, {0, 1, 0, 1} , {1, 0, 0, 1}},
//  {{0, 0, 1, 1} , {0, 0, 0, 1}, {0, 1, 0, 0} , {0, 1, 0, 1} , {0, 1, 0, 1}, {1, 0, 0, 1} , {1, 0, 1, 0}},
//  {{1, 1, 0, 0 }, {1, 0, 1, 0}, {0, 0, 1, 1}, {0, 1, 0, 1}, {1, 0, 0, 1}, {1, 0, 1, 0}, {1, 0, 1, 0}},
//  {{0, 0, 1, 1}, {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 1, 1}, {1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}},
//  {{1, 0, 1, 0}, {0, 0, 1, 1}, {1, 1, 0, 1}, {0, 0, 1, 0}, {1, 1, 0, 0}, {0, 0, 1, 1}, {1, 0, 0, 1}},
//  {{0, 0, 1, 0}, {1, 1, 0, 0}, {0, 0, 1, 1}, {0, 1, 0, 0}, {0, 1, 0, 1}, {1, 1, 0, 0}, {1, 0, 1, 0}},
//  {{0, 1, 1, 0}, {0, 1, 0, 1}, {1, 1, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 0}}
//};

int tempWalls[4] = {0};

//mazewalls2 = [[['1001'],['1000'],['1100'],['1001'],['1100'],['1001'],['1110']],\
//            [['0101'],['0101'],['0101'],['0011'],['1100'],['0011'],['1100']],\
//            [['0101'],['0101'],['0011'],['1010'],['0000'],['1000'],['0110']],\
//            [['0001'],['0000'],['1010'],['1110'],['0101'],['0011'],['1100']],\
//            [['0101'],['0101'],['1101'],['1001'],['0010'],['1000'],['0110']],\
//            [['0101'],['0011'],['0110'],['0011'],['1100'],['0011'],['1100']],\
//            [['0001'],['1010'],['1010'],['1110'],['0011'],['1010'],['0110']]]




int floodfillArr[49][2] = {0};
int c = 0;
int boxCountFF = 0;;

int distinatonFF[2] = {3, 3};

//*****************************************************************************************************

void filterZero() {
  int ct = 0;
  for (int k = 0; k < 7; k++) {
    for (int m = 0; m < 7; m++) {
      if (arena[k][m] == 0) {
        notFilled[ct][0] = k;
        notFilled[ct][1] = m;
        ct = ct + 1;
      }
    }
  }
}







//*******************************************************************************************************
void fillAdjustFF(int x, int y) {



  if (wallMaze[x][y][0] == 0  && Maze[x][y + 1] == 0       &&   !( distinatonFF[0] == x && distinatonFF[1] == y + 1 && boxCountFF > 1  )         ) {
    Maze[x][y + 1] = Maze[x][y] + 1;
    floodfillArr[c][0] = x;
    floodfillArr[c][1] = y + 1;
    c = c + 1;
    boxCountFF += 1;
  }
  if (wallMaze[x][y][1] == 0  && Maze[x + 1][y] == 0         &&   !( distinatonFF[0] == x + 1 && distinatonFF[1] == y && boxCountFF > 1  )         ) {
    Maze[x + 1][y] = Maze[x][y] + 1;

    floodfillArr[c][0] = x + 1;
    floodfillArr[c][1] = y;
    c = c + 1;
    boxCountFF += 1;
  }
  if (wallMaze[x][y][2] == 0  && Maze[x][y - 1] == 0         &&   !( distinatonFF[0] == x && distinatonFF[1] == y - 1 && boxCountFF > 1  )         ) {
    Maze[x][y - 1] = Maze[x][y] + 1;

    floodfillArr[c][0] = x;
    floodfillArr[c][1] = y - 1;

    c = c + 1;
    boxCountFF += 1;
  }
  if (wallMaze[x][y][3] == 0  && Maze[x - 1][y] == 0        &&   !( distinatonFF[0] == x - 1 && distinatonFF[1] == y && boxCountFF > 1  )         ) {
    Maze[x - 1][y] = Maze[x][y] + 1;
    floodfillArr[c][0] = x - 1;
    floodfillArr[c][1] = y;
    c = c + 1;
    boxCountFF += 1;
  }




}

void floodFillMain() {

  Maze[distinatonFF[0]][distinatonFF[1]] = 0;
  boxCountFF = 0;
  floodfillArr[0][0] = distinatonFF[0];
  floodfillArr[0][1] = distinatonFF[1];

  c = 1;

  for (int r = 0; r < 49; r++) {

    fillAdjustFF(floodfillArr[r][0], floodfillArr[r][1]);

    Serial3.println(r);
  }

  Serial3.println("done");
}




void shiftRight(int *Array) {
  int a0 = *Array;
  *Array = *(Array + 1);
  *(Array + 1) = *(Array + 2);
  *(Array + 2) = *(Array + 3);
  *(Array + 3) = a0;
}

void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial3.println("M1 fault");
    while (1);
  }
  if (md.getM2Fault())
  {
    Serial3.println("M2 fault");
    while (1);
  }
}




void setup()
{
  Serial.begin(9600);
  //pinMode( 15, INPUT_PULLUP );
  Serial3.println("Dual VNH5019 Motor Shield");
  md.init();



  //ir
  pinMode (IRL, INPUT);
  pinMode (IRR, INPUT);
  pinMode (IRF, INPUT);
  pinMode (IRDigi, INPUT);


  //indicatorLeds
  pinMode (blackLed, OUTPUT);
  pinMode (whiteLed, OUTPUT);
  pinMode (optionalLed, OUTPUT);

  pinMode(15, INPUT_PULLUP);

  //encoders
  countL = 0;
  countR = 0;
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


  //arm
  microServo.attach(45);
  bigServo.attach(44);

  bigServo.write(20);
  delay(1000);
  microServo.write(140 );
  delay(1000);


//  //  sensorArray
//  delay(500);
//  digitalWrite(optionalLed, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
//  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
//  {
//    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
//  }
//  digitalWrite(optionalLed, LOW);     // turn off Arduino's LED to indicate we are through with calibration
//
//  // print the calibration minimum values measured when emitters were on
//  Serial3.begin(9600);
//  for (int i = 0; i < NUM_SENSORS; i++)
//  {
//    Serial3.print(qtrrc.calibratedMinimumOn[i]);
//    Serial3.print(' ');
//  }
//  Serial3.println();
//
//  // print the calibration maximum values measured when emitters were on
//  for (int i = 0; i < NUM_SENSORS; i++)
//  {
//    Serial3.print(qtrrc.calibratedMaximumOn[i]);
//    Serial3.print(' ');
//  }
//  Serial3.println();
//  delay(500);
//



  EEPROM.write(0, 0);
  boxCount = 0;


  //direction
  dirBot[0] = 1;
  dirBot[1] = 0;

  delay(1000);
}


void loop(){
  md.setSpeeds(200,200);
  Serial.println(countL);
}

//
//void loop()
//{
//
//
//  if (EEPROM.read(0) == 0) {
//    while (1) {
//      if (boxCount == 49 ) { //or ( eggsFound==2 && endFound==1)
//        digitalWrite(optionalLed, HIGH);
//        EEPROM.write(0, 1);
//
//        delay(20000);
//        digitalWrite(optionalLed, LOW);
//
//        break;
//      }
//
//      if (i == 6 && j == 3) {
//        endFound = 1;
//      }
//      delay(100);
//
//      //     checkEggs();
//
//      Serial3.print(i);
//      Serial3.print(",");
//      Serial3.print(j);
//      Serial3.print("\t");
//      Serial3.print(dirBot[0]);
//      Serial3.print(",");
//      Serial3.print(dirBot[1]);
//      Serial3.print("\t");
//      Serial3.print(error);
//      Serial3.print("\t");
//      Serial3.print(disL);
//      Serial3.print("\t");
//      Serial3.print(disR);
//      Serial3.print("\t");
//      Serial3.print(disF);
//      Serial3.print("\t");
//      Serial3.print(disLeft);
//      Serial3.print("\t");
//      Serial3.print(disRight);
//      Serial3.print("\t");
//      Serial3.print(distanceLeft);
//      Serial3.print("\t");
//      Serial3.print(distanceRight);
//      Serial3.print("\t");
//      Serial3.print(digiOut);
//      Serial3.print("\t");
//      Serial3.print(sensorSum);
//      Serial3.print("\t");
//      Serial3.print(speed);
//      Serial3.print("\t");
//      Serial3.print(countL);
//      Serial3.print("\t");
//      Serial3.print(countR);
//      Serial3.println("    ");
//      Serial3.print(boxCount);
//      Serial3.println("    ");
//      Serial3.print(eggsFound);
//      Serial3.println("    ");
//      Serial3.print(endFound);
//      Serial3.println("    ");
//      printArena();
//      Serial3.println();
//      Serial3.println();
//      Serial3.println();
//      printWallMaze();
//      Serial3.println();
//      Serial3.println();
//      Serial3.println();
//      sensorUpdate();
//      checkWalls();
//      checkEggs();
//
//
//    }
//  }
//
//
//  if (EEPROM.read(0) == 1) {
//
//    while (1) {
//      i = 0;
//      j = 0;
//
//      dirBot[0] = 1;
//      dirBot[1] = 0;
//      int dis1 = 0;
//      int dis2 = 0;
//
//
//      /////////////////////////////////////////////////////calculating first distance(first eggs then second)
//      distinatonFF[0] = eggCord[0][0];
//      distinatonFF[1] = eggCord[0][1];
//      floodFillMain();
//      dis1 += Maze[0][0];
//      Serial3.println(dis1);
//
//      distinatonFF[0] = eggCord[1][0];
//      distinatonFF[1] = eggCord[1][1];
//      floodFillMain();
//      dis1 += Maze[eggCord[0][0]][eggCord[0][1]];
//      Serial3.println(dis1);
//
//      distinatonFF[0] = 6;
//      distinatonFF[1] = 3;
//      floodFillMain();
//      dis1 += Maze[eggCord[1][0]][eggCord[1][1]];
//
//      Serial3.println(dis1);
//      Serial3.println();
//      Serial3.println();
//
//
//      /////////////////////////////////////////////////////////////calculation second distance(second eggs then first eggs)
//      distinatonFF[0] = eggCord[1][0];
//      distinatonFF[1] = eggCord[1][1];
//      floodFillMain();
//      dis2 += Maze[0][0];
//      Serial3.println(dis2);
//
//
//
//      distinatonFF[0] = eggCord[0][0];
//      distinatonFF[1] = eggCord[0][1];
//      floodFillMain();
//      dis2 += Maze[eggCord[1][0]][eggCord[1][1]];
//      Serial3.println(dis2);
//
//      distinatonFF[0] = 6;
//      distinatonFF[1] = 3;
//      floodFillMain();
//      dis2 += Maze[eggCord[0][0]][eggCord[0][1]];
//
//
//      if (dis1 <= dis2) {
//        distinatonFF[0] = eggCord[0][0];
//        distinatonFF[1] = eggCord[0][1];
//        floodFillMain();
//        while (!(i == distinatonFF[0] &&   j == distinatonFF[1])) {
//          checkWallsFF();
//          Serial3.print(i);
//          Serial3.print("\t");
//          Serial3.print(j);
//          Serial3.print("\t");
//          printMaze();
//
//        }
//
//        distinatonFF[0] = eggCord[1][0];
//        distinatonFF[1] = eggCord[1][1];
//        floodFillMain();
//        while (!(i == distinatonFF[0] &&   j == distinatonFF[1])) {
//          checkWallsFF();
//          Serial3.print(i);
//          Serial3.print("\t");
//          Serial3.print(j);
//          Serial3.print("\t");
//          printMaze();
//        }
//
//
//        distinatonFF[0] = 6;
//        distinatonFF[1] = 3;
//        floodFillMain();
//        while (!(i == distinatonFF[0] &&   j == distinatonFF[1])) {
//          checkWallsFF();
//          Serial3.print(i);
//          Serial3.print("\t");
//          Serial3.print(j);
//          Serial3.print("\t");
//          printMaze();
//        }
//      }
//      else {
//
//        distinatonFF[0] = eggCord[1][0];
//        distinatonFF[1] = eggCord[1][1];
//        floodFillMain();
//        while (!(i == distinatonFF[0] &&   j == distinatonFF[1])) {
//          checkWallsFF();
//          Serial3.print(i);
//          Serial3.print("\t");
//          Serial3.print(j);
//          Serial3.print("\t");
//          printMaze();
//
//        }
//
//        distinatonFF[0] = eggCord[0][0];
//        distinatonFF[1] = eggCord[0][1];
//        floodFillMain();
//        while (!(i == distinatonFF[0] &&   j == distinatonFF[1])) {
//          checkWallsFF();
//          Serial3.print(i);
//          Serial3.print("\t");
//          Serial3.print(j);
//          Serial3.print("\t");
//          printMaze();
//        }
//
//        distinatonFF[0] = 6;
//        distinatonFF[1] = 3;
//        floodFillMain();
//        while (!(i == distinatonFF[0] &&   j == distinatonFF[1])) {
//          checkWallsFF();
//          Serial3.print(i);
//          Serial3.print("\t");
//          Serial3.print(j);
//          Serial3.print("\t");
//          printMaze();
//        }
//
//      }
//
//      EEPROM.write(0, 0);
//      digitalWrite(optionalLed, HIGH);
//      delay(20000);
//      digitalWrite(optionalLed, LOW);
//    }
//  }
//
//}
//
//
//








void checkEggs() {
  sensorSum = 0;
  qtrrc.readLine(sensorValues);
  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    sensorSum += sensorValues[i];
  }
  if (sensorSum < 6000) {
    digitalWrite(optionalLed, HIGH);
    delay(1000);
    digitalWrite(optionalLed, LOW);
    if (eggsFound == 0) {
      eggCord[0][0] = i;
      eggCord[0][1] = j;
      eggsFound = 1;
    }

    if (eggsFound == 1 && eggCord[0][0] != i &&  eggCord[0][1] != j) {
      eggCord[0][0] = i;
      eggCord[0][1] = j;
      eggsFound = 2;
    }


  }
}




void updatePosition() {
  i = i + dirBot[0];
  j = j + dirBot[1];
}


void turnTo(int k, int c) {
  //  if (dirBot[0] == initialDiri[k][c] && dirBot[1] == initialDirj[k][c]) {
  //    delay(10);
  //  }
  //  else {
  //    turnRight(250);
  //    if (dirBot[0] == initialDiri[k][c] && dirBot[1] == initialDirj[k][c]) {
  //      delay(10);
  //    }
  //    else {
  //      turnRight(250);
  //      if (dirBot[0] == initialDiri[k][c] && dirBot[1] == initialDirj[k][c]) {
  //        delay(10);
  //      }
  //      else {
  //        turnRight(250);
  //        if (dirBot[0] == initialDiri[k][c] && dirBot[1] == initialDirj[k][c]) {
  //          delay(10);
  //        }
  //      }
  //    }
  //  }


  for (int l = 0; l < 3; l++) {
    if (dirBot[0] == initialDiri[k][c] && dirBot[1] == initialDirj[k][c]) {
      break;
    }
    turnRight(250);

  }
}



void checkWalls() {
  int temp1 = dirBot[0];
  int temp2 = dirBot[1];
  int temp12[2] = {temp1, temp2};

  if ((disL < disLimit && disR > disLimit  && disF > disLimitF)) {

    if (arena[i][j] == 0) {
      tempWalls[0] = 0;
      tempWalls[1] = 0;
      tempWalls[2] = 0;
      tempWalls[3] = 1;

      while (!(temp12[0] == 0 && temp12[1] == 1)) {
        rotateVector(temp12);
        shiftRight(tempWalls);
      }
      wallMaze[i][j][0] = tempWalls[0];
      wallMaze[i][j][1] = tempWalls[1];
      wallMaze[i][j][2] = tempWalls[2];
      wallMaze[i][j][3] = tempWalls[3];

    }





    if ((arena[i + dirBot[0]][j + dirBot[1]] == 0)) {
      if (arena[i][j] == 0) {
        boxCount = boxCount + 1;
      }
      arena[i][j] = 1;
      forward();
    }
    else {
      updateDir(1);
      if (arena[i + dirBot[0]][j + dirBot[1]] == 0) {
        dirBot[0] = temp1;
        dirBot[1] = temp2;
        if (arena[i][j] == 0) {
          boxCount = boxCount + 1;
        }
        arena[i][j] = 1;
        turnRight(250);
        forward();
      }
      else {
        dirBot[0] = temp1;
        dirBot[1] = temp2;
        if (arena[i][j] == 0) {
          boxCount = boxCount + 1;
        }
        digitalWrite(optionalLed, HIGH);
        delay(1000);
        digitalWrite(optionalLed, LOW);
        forward();
      }
    }


    //     |
  }
  else if (disL > disLimit  && disR > disLimit  && disF < disLimitF ) {


    if (arena[i][j] == 0) {
      tempWalls[0] = 1;
      tempWalls[1] = 0;
      tempWalls[2] = 0;
      tempWalls[3] = 0;

      while (!(temp12[0] == 0 && temp12[1] == 1)) {
        rotateVector(temp12);
        shiftRight(tempWalls);
      }
      wallMaze[i][j][0] = tempWalls[0];
      wallMaze[i][j][1] = tempWalls[1];
      wallMaze[i][j][2] = tempWalls[2];
      wallMaze[i][j][3] = tempWalls[3];

    }


    forwardCorrection();


    //
    //    if (arena[i][j] == 1) {
    //      turnTo(i, j);
    //    }
    //    else {
    //      initialDiri[i][j] = dirBot[0];
    //      initialDirj[i][j] = dirBot[1];
    //    }
    updateDir(0);
    if (arena[i + dirBot[0]][j + dirBot[1]] == 0) {
      dirBot[0] = temp1;
      dirBot[1] = temp2;
      if (arena[i][j] == 0) {
        boxCount = boxCount + 1;
      }
      arena[i][j] = 1;
      turnLeft(250);
      forward();
    }
    else {
      dirBot[0] = temp1;
      dirBot[1] = temp2;
      updateDir(1);
      if (arena[i + dirBot[0]][j + dirBot[1]] == 0) {
        dirBot[0] = temp1;
        dirBot[1] = temp2;
        if (arena[i][j] == 0) {
          boxCount = boxCount + 1;
        }
        arena[i][j] = 1;
        turnRight(250);
        forward();
      }
      else {
        dirBot[0] = temp1;
        dirBot[1] = temp2;
        if (arena[i][j] == 0) {
          boxCount = boxCount + 1;
        }
        digitalWrite(optionalLed, HIGH);
        delay(1000);
        digitalWrite(optionalLed, LOW);
        turnLeft(250);
        forward();
      }
    }
    //      _
    //
  }
  else if (disL > disLimit  && disR < disLimit  && disF > disLimitF ) { //wall at right

    if (arena[i][j] == 0) {
      tempWalls[0] = 0;
      tempWalls[1] = 1;
      tempWalls[2] = 0;
      tempWalls[3] = 0;

      while (!(temp12[0] == 0 && temp12[1] == 1)) {
        rotateVector(temp12);
        shiftRight(tempWalls);
      }
      wallMaze[i][j][0] = tempWalls[0];
      wallMaze[i][j][1] = tempWalls[1];
      wallMaze[i][j][2] = tempWalls[2];
      wallMaze[i][j][3] = tempWalls[3];

    }

    updateDir(0);
    if (arena[i + dirBot[0]][j + dirBot[1]] == 0) {
      dirBot[0] = temp1;
      dirBot[1] = temp2;
      if (arena[i][j] == 0) {
        boxCount = boxCount + 1;
      }
      arena[i][j] = 1;
      turnLeft(250);
      forward();
    }
    else {
      dirBot[0] = temp1;
      dirBot[1] = temp2;
      if (arena[i + dirBot[0]][j + dirBot[1]] == 0) {
        if (arena[i][j] == 0) {
          boxCount = boxCount + 1;
        }
        arena[i][j] = 1;
        forward();
      }
      else {
        if (arena[i][j] == 0) {
          boxCount = boxCount + 1;
        }
        digitalWrite(optionalLed, HIGH);
        delay(1000);
        digitalWrite(optionalLed, LOW);
        turnLeft(250);
        forward();
      }
    }



    //            |
  }


  //***********************************************************************************************************
  else if (disL < disLimit  && disR > disLimit  && disF < disLimitF ) {


    if (arena[i][j] == 0) {
      tempWalls[0] = 1;
      tempWalls[1] = 0;
      tempWalls[2] = 0;
      tempWalls[3] = 1;

      while (!(temp12[0] == 0 && temp12[1] == 1)) {
        rotateVector(temp12);
        shiftRight(tempWalls);
      }
      wallMaze[i][j][0] = tempWalls[0];
      wallMaze[i][j][1] = tempWalls[1];
      wallMaze[i][j][2] = tempWalls[2];
      wallMaze[i][j][3] = tempWalls[3];

    }



    forwardCorrection();


    if (arena[i][j] == 0) {
      boxCount = boxCount + 1;
    }


    arena[i][j] = 1;
    turnRight(250);
    forward();
    //     _
    //    |


  }
  else if (disL < disLimit  && disR < disLimit  && disF > disLimitF ) {


    if (arena[i][j] == 0) {
      tempWalls[0] = 0;
      tempWalls[1] = 1;
      tempWalls[2] = 0;
      tempWalls[3] = 1;

      while (!(temp12[0] == 0 && temp12[1] == 1)) {
        rotateVector(temp12);
        shiftRight(tempWalls);
      }
      wallMaze[i][j][0] = tempWalls[0];
      wallMaze[i][j][1] = tempWalls[1];
      wallMaze[i][j][2] = tempWalls[2];
      wallMaze[i][j][3] = tempWalls[3];

    }



    if (arena[i][j] == 0) {
      boxCount = boxCount + 1;
    }
    arena[i][j] = 1;
    forward();


    //    |   |
  }
  else if (disL > disLimit  && disR < disLimit  && disF < disLimitF ) {



    if (arena[i][j] == 0) {
      tempWalls[0] = 1;
      tempWalls[1] = 1;
      tempWalls[2] = 0;
      tempWalls[3] = 0;

      while (!(temp12[0] == 0 && temp12[1] == 1)) {
        rotateVector(temp12);
        shiftRight(tempWalls);
      }
      wallMaze[i][j][0] = tempWalls[0];
      wallMaze[i][j][1] = tempWalls[1];
      wallMaze[i][j][2] = tempWalls[2];
      wallMaze[i][j][3] = tempWalls[3];

    }




    forwardCorrection();



    if (arena[i][j] == 0) {
      boxCount = boxCount + 1;
    }
    arena[i][j] = 1;
    //    digitalWrite(optionalLed,HIGH);
    //    delay(2000);
    turnLeft(250);
    forward();

    //digitalWrite(optionalLed,HIGH);
    //     _
    //      |


  }

  //****************************************************************************************************************
  else if (disL < disLimit  && disR < disLimit  && disF < disLimitF ) {


    if (arena[i][j] == 0) {
      tempWalls[0] = 1;
      tempWalls[1] = 1;
      tempWalls[2] = 0;
      tempWalls[3] = 1;

      while (!(temp12[0] == 0 && temp12[1] == 1)) {
        rotateVector(temp12);
        shiftRight(tempWalls);
      }
      wallMaze[i][j][0] = tempWalls[0];
      wallMaze[i][j][1] = tempWalls[1];
      wallMaze[i][j][2] = tempWalls[2];
      wallMaze[i][j][3] = tempWalls[3];

    }


    forwardCorrection();

    deadFound = 1;
    nextJunction = 0;
    if (arena[i][j] == 0) {
      boxCount = boxCount + 1;
    }
    arena[i][j] = 1;
    turnRight(250);
    turnRight(250);
    //turnAbout(250);
    forward();
    //   _
    //  | |
  }
  else {
    if (arena[i][j] == 0) {
      boxCount = boxCount + 1;
    }
    arena[i][j] = 1;
    turnLeft(250);
  }



  updatePosition();


}







void checkWallsFF() {


  int temp1 = dirBot[0];
  int temp2 = dirBot[1];


  if ((disL < disLimit && disR > disLimit  && disF > disLimitF)) {

    updateDir(1);


    if (       (Maze[i + dirBot[0]][j + dirBot[1]] >= Maze[i + temp1][j + temp2 ])        ) {// &&     Maze[i + temp1][j + temp2 ] < Maze[i][j]
      arena[i][j] = 1;
      dirBot[0] = temp1;
      dirBot[1] = temp2;
      forward();
    }
    else if ((Maze[i + dirBot[0]][j + dirBot[1]] < Maze[i + temp1][j + temp2 ] )          ) {  // &&     Maze[i + dirBot[0]][j + dirBot[1]] < Maze[i][j]
      arena[i][j] = 1;
      updateDir(0);
      turnRight(250);
      forward();
    }
    else {
      digitalWrite(optionalLed, HIGH);
      delay(100000);
    }


    //     |
  }



  else if (disL > disLimit  && disR > disLimit  && disF < disLimitF ) {

    updateDir(0);
    digitalWrite(optionalLed, HIGH);

    if (       (Maze[i + dirBot[0]][j + dirBot[1]] <= Maze[i + (-dirBot[0])] [j + (-dirBot[1]) ])       ) {  //  &&     Maze[i + dirBot[0]][j + dirBot[1] ] < Maze[i][j]
      dirBot[0] = temp1;
      dirBot[1] = temp2;
      arena[i][j] = 1;
      turnLeft(250);
      forward();
    }
    else if (        (Maze[i + dirBot[0]][j + dirBot[1]] > Maze[i + (-dirBot[0])] [j + (-dirBot[1])])            ) {       // &&     (  Maze[i + (-dirBot[0])] [j + (-dirBot[1]) ] < Maze[i][j]  )
      dirBot[0] = temp1;
      dirBot[1] = temp2;
      arena[i][j] = 1;
      turnRight(250);
      forward();
    }
    else {
      digitalWrite(optionalLed, HIGH);
      delay(100000);
    }

    //      _
    //
  }
  else if (disL > disLimit  && disR < disLimit  && disF > disLimitF ) { //wall at right
    updateDir(0);
    if   (        (Maze[i + dirBot[0]][j + dirBot[1]] <= Maze[i + temp1] [j + temp2])           )   {// &&     (  Maze[i + dirBot[0]] [j + dirBot[1] ] < Maze[i][j]  )
      dirBot[0] = temp1;
      dirBot[1] = temp2;
      arena[i][j] = 1;
      turnLeft(250);
      forward();
    }
    else  if (        (Maze[i + dirBot[0]][j + dirBot[1]] > Maze[i + temp1] [j + temp2])           ) {//  &&    ( ( Maze[i + temp1] [j + temp2])  < Maze[i][j]  )
      dirBot[0] = temp1;
      dirBot[1] = temp2;
      arena[i][j] = 1;
      forward();
    }
    else {
      digitalWrite(optionalLed, HIGH);
      delay(100000);

    }
  }



  //            |



  //***********************************************************************************************************
  else if (disL < disLimit  && disR > disLimit  && disF < disLimitF ) {

    arena[i][j] = 1;
    turnRight(250);
    forward();
    //     _
    //    |
  }
  else if (disL < disLimit  && disR < disLimit  && disF > disLimitF ) {

    arena[i][j] = 1;

    forward();


    //    |   |
  }
  else if (disL > disLimit  && disR < disLimit  && disF < disLimitF ) {
    arena[i][j] = 1;

    //    digitalWrite(optionalLed,HIGH);
    //    delay(2000);
    turnLeft(250);
    forward();

    //digitalWrite(optionalLed,HIGH);
    //     _
    //      |


  }

  //****************************************************************************************************************
  else if (disL < disLimit  && disR < disLimit  && disF < disLimitF ) {
    arena[i][j] = 1;
    turnRight(250);
    turnRight(250);
    forward();
    //   _
    //  | |
  }
  else {
    arena[i][j] = 1;
    turnLeft(250);
  }

  updatePosition();

  //  if (Maze[i][j] >= 10) {
  //    Maze[i][j] += 10;
  //  }
  //  else {
  //    Maze[i][j] = 10;
  //  }

}

void pidTurn() {
  errorTurn = 480 - (abs(countR) + abs(countL)) / 2;
  speedTurn = constrain(0.1 * errorTurn + 1 * (errorTurn - last_errorTurn) , -50, 50);
  last_errorTurn = errorTurn;
}

void turnLeft(int rspeed) { /// turn1
  countL = 0;
  countR = 0;
  while ((abs(countR) + abs(countL)) / 2 < 510) {
    pidTurn();
    md.setM1Speed(-speedTurn - 250);
    md.setM2Speed(speedTurn + 250);
  }
  md.setBrakes(400, 400);
  updateDir(0);
  delay(300);
  countL = 0;
  countR = 0;




}


void turnRight(int rspeed) {
  countL = 0;
  countR = 0;
  while ((abs(countR) + abs(countL)) / 2 < 540) {
    pidTurn();
    md.setM1Speed(speedTurn + 250);
    md.setM2Speed(-speedTurn - 250);
  }
  md.setBrakes(400, 400);
  updateDir(1);
  delay(500);
  countL = 0;
  countR = 0;


}


void turnAbout(int rspeed) {
  countL = 0;
  countR = 0;
  while (countL < (2 * 480)) {//480
    md.setM1Speed(rspeed);
    md.setM2Speed(-rspeed);
  }
  md.setBrakes(400, 400);
  updateDir(2);
  delay(500);
  countL = 0;
  countR = 0;


}


void forward() {

  countL = 0;
  countR = 0;

  while ((countR + countL) < 2415) {///2900//2740
    sensorUpdate();
    pid();
    pidEncoder();
    md.setSpeeds(210 - speed - speedEncoder, 190 + speed + speedEncoder);
    //md.setSpeeds(250-speedEncoder,250+speedEncoder);
    if (disF < 10) {
      md.setBrakes(400, 400);
      delay(300);
      countL = 0;
      countR = 0;
      sensorUpdate();
      if (disF < 10) {

        while (abs(countR + countL) < 200) {
          md.setSpeeds(-210 , -190 );
        }
        //md.setBrakes(400, 400);
      }

      break;
    }
  }


  md.setBrakes(400, 400);
  delay(100);
  countL = 0;
  countR = 0;

}







void forwardCorrection() {



  delay(300);
  sensorUpdate();
  if (disF > 8) {
    countL = 0;
    countR = 0;
    while (abs(countR + countL) < 200) {
      md.setSpeeds(210  , 190 );

    }
  }
  md.setBrakes(400, 400);
  delay(300);

}

void sensorUpdate() {
  disLeft = sharpl.distance();
  if (disLeft > 80 || disLeft < 0) {
    disLeft = 80;
  }
  disRight = sharpr.distance();
  if (disRight > 80 || disRight < 0) {
    disRight = 80;
  }
  disF = sharpf.distance();
  if (disF > 80 || disF < 0) {
    disF = 80;
  }
  //digiOut=digitalRead(IRDigi);
  distanceLeft = sonarLeft.ping_cm();
  distanceRight = sonarRight.ping_cm();
  disL = (disLeft + distanceLeft) * 0.5;
  disR = (disRight + distanceRight) * 0.5;

}




void sensorArray() {
  sensorSum = 0;
  qtrrc.readLine(sensorValues);
  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    sensorSum += sensorValues[i];
  }
  if (sensorSum < 200) {
    digitalWrite(optionalLed, HIGH);
  }
  else {
    digitalWrite(optionalLed, LOW);
  }
}


void pid() {
  if (disL > 20) {    // right wall following

    error = (8 - disR);

    speed = constrain((error * 6 + 50 * (error - last_errorR) ), -pidMax, pidMax);

    last_errorR = error;

  }
  else if (disR > 20) {    // left wall following

    error = (disL - 9);
    speed = constrain((error * 6 + 50 * (error - last_errorL) ), -pidMax, pidMax);

    last_errorL = error;
  }


  else {

    error = (disL - disR);
    pTerm = Kp * (error);
    integrated_error += error;
    iTerm = Ki * constrain(integrated_error, -GUARD_GAIN, GUARD_GAIN);
    dTerm = Kd * (error - last_error);
    last_error = error;
    speed = constrain(K * (pTerm + dTerm + iTerm), -pidMax, pidMax);
  }



  if (disR > 20 && disL > 20) {
    speed = 0;
  }


}




void pidEncoder() {
  errorEncoder = (countL - countR);
  speedEncoder = constrain(40 * errorEncoder + 100 * (errorEncoder - last_errorEncoder) , -pidEncoderMax, pidEncoderMax);
  last_errorEncoder = errorEncoder;
}


void grip() {
  bigServo.write(90);
  delay(1000);
  microServo.write(120);
  delay(1000);
  bigServo.write(180);
  delay(1000);
  microServo.write(180);
  delay(1000);
  bigServo.write(0);
  delay(1000);
  microServo.write(120);
  delay(1000);
  bigServo.write(90);
  delay(5000);

  //microServo.write(120);
  //delay(1000);
  //bigServo.write(0);
  //delay(1000);
  //microServo.write(180);
  //delay(1000);
  //bigServo.write(120);
  //delay(1000);
  //microServo.write(120);
  //delay(1000);
  //bigServo.write(90);
  //delay(1000);


}

void colorDetect() {

  if (digiOut == 0 && disF < 10) {
    digitalWrite(whiteLed, HIGH);
    digitalWrite(blackLed, LOW);
    object = 1;
  }
  else if (digiOut == 1 && disF < 10) {
    digitalWrite(blackLed, HIGH);
    digitalWrite(whiteLed, LOW);
    object = 1;
  }
  else {
    digitalWrite(blackLed, LOW);
    digitalWrite(whiteLed, LOW);
    object = 0;
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




void printArena() {
  int i = 0;
  int j = 0;
  for (i = 0; i < 7; i++) {
    for (j = 0; j < 7; j++) {
      Serial3.print(arena[i][j]);
      Serial3.print("\t");

    }
    Serial3.println();
  }
}


void printMaze() {
  int i = 0;
  int j = 0;
  for (i = 0; i < 7; i++) {
    for (j = 0; j < 7; j++) {
      Serial3.print(Maze[i][j]);
      Serial3.print("\t");

    }
    Serial3.println();
  }
}


void printWallMaze() {
  int i = 0;
  int j = 0;
  for (i = 0; i < 7; i++) {
    for (j = 0; j < 7; j++) {
      for (int k = 0; k < 4; k++) {
        Serial3.print(wallMaze[i][j][k]);
      }
      Serial3.print("\t");

    }
    Serial3.println();
  }
}



void updateDir(int side) {
  //side = 0 - Left
  //side = 1 - Right
  //side = 2 - About
  if (dirBot[0] == 0 && dirBot[1] == 1) {
    if (side == 0) {
      dirBot[0] = -1;
      dirBot[1] = 0;
    }
    else if (side == 1) {
      dirBot[0] = 1;
      dirBot[1] = 0;
    }
    else {
      dirBot[0] = 0;
      dirBot[1] = -1;
    }
  }
  else if (dirBot[0] == 1 && dirBot[1] == 0) {
    if (side == 0) {
      dirBot[0] = 0;
      dirBot[1] = 1;
    }
    else if (side == 1) {
      dirBot[0] = 0;
      dirBot[1] = -1;
    }
    else {
      dirBot[0] = -1;
      dirBot[1] = 0;
    }
  }
  else if (dirBot[0] == 0 && dirBot[1] == -1) {
    if (side == 0) {
      dirBot[0] = 1;
      dirBot[1] = 0;
    }
    else if (side == 1) {
      dirBot[0] = -1;
      dirBot[1] = 0;
    }
    else {
      dirBot[0] = 0;
      dirBot[1] = 1;
    }
  }
  else if (dirBot[0] == -1 && dirBot[1] == 0) {
    if (side == 0) {
      dirBot[0] = 0;
      dirBot[1] = -1;
    }
    else if (side == 1) {
      dirBot[0] = 0;
      dirBot[1] = 1;
    }
    else {
      dirBot[0] = 1;
      dirBot[1] = 0;
    }
  }
}





void rotateVector(int *dir) {
  if (*dir == 0 && *(dir + 1) == 1) {
    *dir = 1;
    *(dir + 1) = 0;

  }
  else if (*dir == 1 && *(dir + 1) == 0) {

    *dir = 0;
    *(dir + 1) = -1;

  }
  else if (*dir == 0 && *(dir + 1) == -1) {

    *dir = -1;
    *(dir + 1) = 0;

  }
  else if (*dir == -1 && *(dir + 1) == 0) {

    *dir = 0;
    *(dir + 1) = 1;

  }
}

