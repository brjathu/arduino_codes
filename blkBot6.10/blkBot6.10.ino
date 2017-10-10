#include <NewPing.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <SharpIR.h>
#include "DualVNH5019MotorShield.h"

DualVNH5019MotorShield md;
/////////////////////////////////////////front sonar
#define triggerForward  31
#define echoForward     30
#define maxDistanceForward 300
float distanceForward = 0;
float distanceForwardLast = 0;
NewPing sonarFrontForward(triggerForward, echoForward, maxDistanceForward);

/////////////////////////////////////////left front sonar
#define triggerLFront  23
#define echoLFront     22
#define maxDistance 180
float distanceLeftFront = 0;
float distanceLeftFrontLast = 0;
NewPing sonarLeftFront(triggerLFront, echoLFront, maxDistance);
///////////////////////////////////////////////right front sonar
#define triggerRFront  25
#define echoRFront     24
float distanceRightFront = 0;
float distanceRightFrontLast = 0;
NewPing sonarRightFront(triggerRFront, echoRFront, maxDistance);

/////////////////////////////////////////left back sonar
#define triggerLBack  29
#define echoLBack     28
#define maxDistance 200
float distanceLeftBack = 0;
float distanceLeftBackLast = 0;
NewPing sonarLeftBack(triggerLBack, echoLBack, maxDistance);
///////////////////////////////////////////////right back sonar
#define triggerRBack  27
#define echoRBack     26
float distanceRightBack = 0;
float distanceRightBackLast = 0;
NewPing sonarRightBack(triggerRBack, echoRBack, maxDistance);

int pidMax;
float error1;
float error2;
int doubleWall = 0;
/////////////////////////////////////////////////////////PID
float Kp = 5;//6
float Ki = 0;//0.02;
float Kd = 250;//70;
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
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////Encoders
#define EncoderLPinA 18//20
#define EncoderLPinB 19//19
#define EncoderRPinA 20
#define EncoderRPinB 21
long countL = 0;
long countR = 0;

/////////////////////////////////////////////////////////////////////////////Turning
int rspeed;
int turnValue = 425;  //450
int straightValue = 2400; //2400 for 35 ///2055 for 30
int delayValue = 500;
///////////////////////////////////////////////////////////////////////////MAP
int arena[6][6] = {0};
int dirBot[2];
int i = 0;
int j = 0;
//*********************************************************************//

#define redLed 15
#define orangeLed 14




////////////////////////////////////////////////////////////////////////color variables
#define red 45
#define green 47
#define blue 49

#define whiteThreshold 850
#define bgThreshold


int count = 0;
int color[] = {red, green, blue};
int sensor[] = {62, 63, 64, 65, 66, 67, 68, 69};
int pidColor[] = {0, 0, 0, 0, 0, 0, 0, 0};


int rgbMax[3][8] = {0};
int rgbMin[3][8] = {0};
int colorArray[3][8] = {0};

int colorError = 0;
int colorErrorLast = 0;
int speedColor;
int colFollor = 0;


//////////////////////////////////////////////////////////////////////////










int Maze[7][7] = {0};
int tempWalls[4] = {0};
int wallMaze[7][7][4] =  {{{1, 1, 1, 1}, {1, 1, 1, 1} , {1, 1, 1, 1} , {1, 1, 1, 1} , {1, 1, 1, 1}, {1, 1, 1, 1} },
  {{1, 1, 1, 1} , {1, 1, 1, 1}, {1, 1, 1, 1} , {1, 1, 1, 1} , {1, 1, 1, 1}, {1, 1, 1, 1} }},
  {{1, 1, 1, 1 }, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
  {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
  {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
  {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
};

int distinatonFF[2] = {2, 5};
int floodfillArr[36][2] = {0};
int c = 0;
int boxCountFF = 0;

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
  floodfillArr[0][0] = distinatonFF[0];
  floodfillArr[0][1] = distinatonFF[1];

  c = 1;

  for (int r = 0; r < 36; r++) {

    fillAdjustFF(floodfillArr[r][0], floodfillArr[r][1]);

    Serial3.println(r);
  }

  Serial3.println("done");
}


void setup() {
  // put your setup code here, to run once:
  md.init();
  Serial.begin(115200);
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

  dirBot[0] = 1;
  dirBot[1] = 0;
  ///////////////////////////////////////////////////////////////
  sonarForward();
  sonarForward();
  pid();
  digitalWrite(orangeLed, HIGH);
  delay(500);
  digitalWrite(orangeLed, LOW);
  //md.setSpeeds(220,250);
  //delay(50);





  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

//  calibirate2();

}




void shiftRight(int *Array) {
  int a0 = *Array;
  *Array = *(Array + 1);
  *(Array + 1) = *(Array + 2);
  *(Array + 2) = *(Array + 3);
  *(Array + 3) = a0;
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
  //  Serial.println("\t");
  //  Serial.print(error1);
  //  Serial.print("\t");
  //  Serial.print(error2);
  //  Serial.print("\t");
  Serial.print(error);
  Serial.print("\t");
  //    Serial.print(countL);
  //    Serial.print("\t");
  //    Serial.print(countR);
  Serial.println("\t");
  //pid();
  //  forwardEncoder();

  ///////////////////////////////////////////////////////////////////////////
  //  printArena();
  checkWallsRight();
  //////////////////////////////////////pid Encoder tuning
  //  speed=0;
  //  pidEncoder();
  //  forward();
}


void updatePosition() {
  i = i + dirBot[0];
  j = j + dirBot[1];
}

void printArena() {
  int i = 0;
  int j = 0;
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 6; j++) {
      Serial.print(arena[i][j]);
      Serial.print("\t");

    }
    Serial.println();
  }
  Serial.println("--------------------------------------");
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
void pidTurn(int side) {
  //1-left
  //2-right
  if (side == 2) {
    errorTurn = turnValue - (abs(countR) + abs(countL)) / 2;    //480
    speedTurn = constrain(0.1 * errorTurn + 1 * (errorTurn - last_errorTurn) , -50, 50);
    last_errorTurn = errorTurn;
  }
  else if (side == 1) {
    errorTurn = (turnValue - 15) - (abs(countR) + abs(countL)) / 2;  //480
    speedTurn = constrain(0.1 * errorTurn + 1 * (errorTurn - last_errorTurn) , -50, 50);
    last_errorTurn = errorTurn;
  }
}


void turnLeft() { /// turn1
  countL = 0;
  countR = 0;
  while ((abs(countR) + abs(countL)) / 2 <= (turnValue - 15)) {
    pidTurn(1);
    md.setM1Speed(-speedTurn - 250);
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
    pidTurn(2);
    md.setM1Speed(speedTurn + 250);
    md.setM2Speed(-speedTurn - 250);
  }
  md.setBrakes(400, 400);
  delay(1000);
  countL = 0;
  countR = 0;
}

void turnCorrection(int wall) {
  //1-double wall
  //2-left wall
  //3-right wall
  countL = 0;
  countR = 0;
  int constant = 5;
  sonarFront();
  sonarBack();
  if (wall == 1 || wall == 3) {
    if ((10 - distanceRightFront) > 0) {
      while (abs(countR + countL) < constant * (10 - distanceRightFront)) {
        md.setSpeeds(-250, 250);
      }
      md.setBrakes(400, 400);
      delay(250);
    }
    else if ((10 - distanceRightFront) < 0) {
      while (abs(countR + countL) < constant * (distanceRightFront - 10)) {
        md.setSpeeds(250, -250);
      }
      md.setBrakes(400, 400);
      delay(250);
    }
  }
  else if (wall == 2) {
    if ((10 - distanceLeftFront) > 0) {
      while (abs(countR + countL) < constant * (10 - distanceLeftFront)) {
        md.setSpeeds(250, -250);
      }
      md.setBrakes(400, 400);
      delay(250);
    }
    else if ((10 - distanceLeftFront) < 0) {
      while (abs(countR + countL) < constant * (distanceLeftFront - 10)) {
        md.setSpeeds(-250, 250);
      }
      md.setBrakes(400, 400);
      delay(250);
    }
  }
}

void forwardCorrection() {
  delay(10);
  sonarForward();
  if (distanceForward >= 12) {
    countL = 0;
    countR = 0;
    while (abs(countR + countL) < 65 * (distanceForward - 11)) {
      md.setSpeeds(200  , 250);

    }
  }

  else   if (distanceForward <= 6.5) {
    countL = 0;
    countR = 0;
    while (abs(countR + countL) < 65 * (6.5 - distanceForward)) {
      md.setSpeeds(-200 , -250);
    }
  }
  md.setBrakes(400, 400);
  delay(300);
}

void forwardEncoder() {
  countL = 0;
  countR = 0;
  while (abs(countL + countR) < straightValue) {
    sonarForward();
    if (0 < distanceForward && distanceForward <= 12) {
      md.setBrakes(400, 400);
      digitalWrite(orangeLed, HIGH);
      delay(1000);
      digitalWrite(orangeLed, LOW);
      break;
    }
    pidEncoderNoWall();
    md.setSpeeds(220 + speedEncoder , 250 - speedEncoder ); //200,250
  }
  md.setBrakes(400, 400);
}
void forward() {
  countL = 0;
  countR = 0;

  while (abs(countL + countR) < straightValue) {
    pid();
    if (0 < distanceForward && distanceForward <= 12) {
      md.setBrakes(400, 400);
      break;
    }
    pidEncoder();
    if (error2 >= 3 && error2 < -4 && doubleWall == 0) {
      md.setSpeeds(180 - speed - speedEncoder, 350 + speed + speedEncoder);
    }
    else if (error2 <= -3 && error2 > -4 && doubleWall == 0) {
      md.setSpeeds(250 - speed - speedEncoder, 200 + speed + speedEncoder);
    }
    else if (error2 >= 4 && doubleWall == 0) {
      md.setSpeeds(170 - speed - speedEncoder, 380 + speed + speedEncoder);
    }
    else if (error2 <= -4 && doubleWall == 0) {
      md.setSpeeds(300 - speed - speedEncoder, 180 + speed + speedEncoder);
    }
    else if ((distanceLeftFront - distanceLeftBack) < 0 && error < 0 && doubleWall == 2) {
      md.setSpeeds(380 - speed - speedEncoder, 180 + speed + speedEncoder);
      doubleWall = 0;
    }
    else if ((distanceLeftFront - distanceLeftBack) > 0 && error > 3.5 && doubleWall == 2) {
      md.setSpeeds(180 - speed - speedEncoder, 380 + speed + speedEncoder);
      doubleWall = 0;
    }
    else if ((distanceLeftFront - distanceLeftBack) < 0 && error < 0 && doubleWall == 2) {
      md.setSpeeds(180 - speed - speedEncoder, 380 + speed + speedEncoder);
      doubleWall = 0;
    }
    else if ((distanceLeftFront - distanceLeftBack) > 0 && error > 3.5 && doubleWall == 2) {
      md.setSpeeds(380 - speed - speedEncoder, 180 + speed + speedEncoder);
      doubleWall = 0;
    }
    //    else if ((distanceRightFront - distanceRightBack) > 0 && error > 3.5 && doubleWall == 1) {
    //      md.setSpeeds( 250 - speed - speedEncoder, 250 + speed + speedEncoder);
    //      doubleWall = 0;
    //    }
    //            else if ((distanceRightFront - distanceRightBack) > 0 && error <0 && doubleWall == 1) {
    //          md.setSpeeds( 180 - speed - speedEncoder, 380 + speed + speedEncoder);
    //          doubleWall = 0;
    //        }
    //    else if ((distanceRightFront - distanceRightBack) < 0 && error > -3.5 && error < 0 && doubleWall == 1) {
    //      md.setSpeeds( 250 - speed - speedEncoder, 250 + speed + speedEncoder);
    //      doubleWall = 0;
    //    }
    //            else if ((distanceRightFront - distanceRightBack) < 0 && error<-3.5 && doubleWall == 1) {
    //      md.setSpeeds( 180 - speed - speedEncoder, 380 + speed + speedEncoder);
    //      doubleWall = 0;
    //    }
    //            else if (error>=8 && doubleWall == 1) {
    //          md.setSpeeds( 250 - speed - speedEncoder, 220 + speed + speedEncoder);
    //          doubleWall = 0;
    //        }
    //    else if (error < -1 && doubleWall == 1) {
    //      md.setSpeeds( 180 - speed - speedEncoder, 320 + speed + speedEncoder);
    //      doubleWall = 0;
    //    }
    else if (doubleWall == 3) { //4 is right to left
      md.setSpeeds( 180 - speed, 250 + speed);
      doubleWall = 0;
    }
    else if (doubleWall == 4) { //4 is right to left
      md.setSpeeds( 180 - speed, 300 + speed);
      doubleWall = 0;
    }
    else {
      // md.setSpeeds(190 - speed1 - speed2 - speedEncoder, 215 + speed1 + speed2 + speedEncoder);
      md.setSpeeds(290 - speed - speedEncoder, 310 + speed + speedEncoder);    //220,250
    }
  }
  //digitalWrite(redLed, LOW);
  //    md.setBrakes(400, 400);
  //    delay(10000);
}


/////////////////////////////////////
void checkWalls() {
  sonarFront();
  sonarBack();
  sonarForward();
  float disL;
  float disR;
  float disF;
  int disLimit = 29;
  int disLimitF = 20;
  //  disL = 0.5 * (distanceLeftFront + distanceLeftBack);
  //  disR = 0.5 * (distanceRightFront + distanceRightBack);

  disL = distanceLeftFront;
  disR = distanceRightFront;
  disF = distanceForward;


  int temp1 = dirBot[0];
  int temp2 = dirBot[1];
  int temp12[2] = {temp1, temp2};


  if (disL < disLimit && disR > disLimit  && disF > disLimitF) {
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

    arena[i][j] = 1;
    forward();
    //     |
  }
  else if (disL > disLimit  && disR > disLimit  && disF <= disLimitF ) {


    md.setBrakes(400, 400);
    delay(delayValue);
    forwardCorrection();
    sonarFront();
    sonarBack();
    sonarForward();
    //    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    //    disR = 0.5 * (distanceRightFront + distanceRightBack);
    //    disF = distanceForward;
    disL = distanceLeftFront;
    disR = distanceRightFront;
    disF = distanceForward;
    if (disL > disLimit  && disR > disLimit  && disF <= disLimitF ) {
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
      arena[i][j] = 1;
      turnLeft();
      forward();
    }
    //      _
    //
  }
  else if (disL > disLimit  && disR < disLimit  && disF > disLimitF ) { //wall at right


    md.setBrakes(400, 400);
    delay(delayValue);
    md.setSpeeds(0, 0);
    sonarFront();
    sonarBack();
    sonarForward();
    //    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    //    disR = 0.5 * (distanceRightFront + distanceRightBack);
    //    disF = distanceForward;
    disL = distanceLeftFront;
    disR = distanceRightFront;
    disF = distanceForward;

    if (disL > disLimit  && disR < disLimit  && disF > disLimitF ) {
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
      arena[i][j] = 1;
      turnLeft();
      forward();
    }
    //            |
  }
  else if (disL < disLimit  && disR > disLimit  && disF <= disLimitF ) {
    md.setBrakes(400, 400);
    delay(delayValue);
    forwardCorrection();
    sonarFront();
    sonarBack();
    sonarForward();
    //    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    //    disR = 0.5 * (distanceRightFront + distanceRightBack);
    //    disF = distanceForward;
    disL = distanceLeftFront;
    disR = distanceRightFront;
    disF = distanceForward;

    if (disL < disLimit  && disR > disLimit  && disF <= disLimitF ) {
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
      arena[i][j] = 1;
      turnRight();
      forward();
    }
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

    arena[i][j] = 1;
    forward();
    //    |   |
  }
  else if (disL > disLimit  && disR < disLimit  && disF <= disLimitF ) {
    md.setBrakes(400, 400);
    delay(delayValue);
    forwardCorrection();
    sonarFront();
    sonarBack();
    sonarForward();
    //    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    //    disR = 0.5 * (distanceRightFront + distanceRightBack);
    //    disF = distanceForward;
    disL = distanceLeftFront;
    disR = distanceRightFront;
    disF = distanceForward;

    if (disL > disLimit  && disR < disLimit  && disF <= disLimitF ) {
      
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

      arena[i][j] = 1;
      turnLeft();
      forward();
    }


  }
  else if (disL < disLimit  && disR < disLimit  && disF < disLimitF ) {
    md.setBrakes(400, 400);
    delay(delayValue);
    sonarFront();
    sonarBack();
    sonarForward();
    //    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    //    disR = 0.5 * (distanceRightFront + distanceRightBack);
    //    disF = distanceForward;
    disL = distanceLeftFront;
    disR = distanceRightFront;
    disF = distanceForward;

    if (disL < disLimit  && disR < disLimit  && disF < disLimitF ) {
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
      arena[i][j] = 1;
      turnRight();
      forwardCorrection();
      turnRight();
      forward();
    }
    //   _
    //  | |
  }
  else if (disL > disLimit  && disR > disLimit  && disF > disLimitF ) {
    md.setBrakes(400, 400);
    delay(delayValue);
    sonarFront();
    sonarBack();
    sonarForward();
    //    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    //    disR = 0.5 * (distanceRightFront + distanceRightBack);
    //    disF = distanceForward;
    disL = distanceLeftFront;
    disR = distanceRightFront;
    disF = distanceForward;

    if (disL > disLimit  && disR > disLimit  && disF > disLimitF ) {

      if (arena[i][j] == 0) {
      tempWalls[0] = 0;
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
    
      arena[i][j] = 1;
      turnLeft();
      forward();
    }
    //
    //  4-way junc
  }
  else {
    digitalWrite(redLed, HIGH);
    md.setBrakes(400, 400);
    delay(3000);
    digitalWrite(redLed, LOW);
    arena[i][j] = 1;
    forward();
  }
  updatePosition();
}

///////////////////////////////////////////
void checkWallsRight() {
  sonarFront();
  sonarBack();
  sonarForward();
  float disL;
  float disR;
  float disF;
  int disLimit = 29;
  int disLimitF = 20;
  //  disL = 0.5 * (distanceLeftFront + distanceLeftBack);
  //  disR = 0.5 * (distanceRightFront + distanceRightBack);

  disL = distanceLeftFront;
  disR = distanceRightFront;
  disF = distanceForward;

  if (disL < disLimit && disR > disLimit  && disF > disLimitF) {
    arena[i][j] = 1;
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
    //    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    //    disR = 0.5 * (distanceRightFront + distanceRightBack);
    //    disF = distanceForward;
    disL = distanceLeftFront;
    disR = distanceRightFront;
    disF = distanceForward;
    if (disL > disLimit  && disR > disLimit  && disF <= disLimitF ) {
      arena[i][j] = 1;
      turnLeft();
      forward();
    }
    //      _
    //
  }
  else if (disL > disLimit  && disR < disLimit  && disF > disLimitF ) { //wall at right
    //    md.setBrakes(400, 400);
    //    delay(500);
    //    md.setSpeeds(0, 0);
    //    sonarFront();
    //    sonarBack();
    //    sonarForward();
    //    //    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    //    //    disR = 0.5 * (distanceRightFront + distanceRightBack);
    //    //    disF = distanceForward;
    //    disL = distanceLeftFront;
    //    disR = distanceRightFront;
    //    disF = distanceForward;

    //    if (disL > disLimit  && disR < disLimit  && disF > disLimitF ) {
    arena[i][j] = 1;
    //turnLeft();
    forward();
    //    }
    //            |
  }
  else if (disL < disLimit  && disR > disLimit  && disF <= disLimitF ) {
    md.setBrakes(400, 400);
    delay(500);
    forwardCorrection();
    sonarFront();
    sonarBack();
    sonarForward();
    //    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    //    disR = 0.5 * (distanceRightFront + distanceRightBack);
    //    disF = distanceForward;
    disL = distanceLeftFront;
    disR = distanceRightFront;
    disF = distanceForward;

    if (disL < disLimit  && disR > disLimit  && disF <= disLimitF ) {
      arena[i][j] = 1;
      turnRight();
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
    arena[i][j] = 1;
    forward();
    //    |   |
  }
  else if (disL > disLimit  && disR < disLimit  && disF <= disLimitF ) {
    md.setBrakes(400, 400);
    delay(500);
    forwardCorrection();
    sonarFront();
    sonarBack();
    sonarForward();
    //    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    //    disR = 0.5 * (distanceRightFront + distanceRightBack);
    //    disF = distanceForward;
    disL = distanceLeftFront;
    disR = distanceRightFront;
    disF = distanceForward;

    if (disL > disLimit  && disR < disLimit  && disF <= disLimitF ) {
      arena[i][j] = 1;
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


  }
  else if (disL < disLimit  && disR < disLimit  && disF < disLimitF ) {
    md.setBrakes(400, 400);
    delay(500);
    sonarFront();
    sonarBack();
    sonarForward();
    //    disL = 0.5 * (distanceLeftFront + distanceLeftBack);
    //    disR = 0.5 * (distanceRightFront + distanceRightBack);
    //    disF = distanceForward;
    disL = distanceLeftFront;
    disR = distanceRightFront;
    disF = distanceForward;

    if (disL < disLimit  && disR < disLimit  && disF < disLimitF ) {
      arena[i][j] = 1;
      turnRight();
      forwardCorrection();
      turnRight();
      forward();
    }
    //   _
    //  | |
  }
  else {
    digitalWrite(redLed, HIGH);
    md.setBrakes(400, 400);
    delay(3000);
    digitalWrite(redLed, LOW);
    arena[i][j] = 1;
    forward();
  }
  updatePosition();
}
/////////////////////////////////sonar
void sonarForward() {
  //distanceForwardLast = distanceForward;
  //  float temp1 = 0;
  //  float temp2 = 0;
  //  temp1 = sonarFrontForward.ping_cm();
  //  temp2 = sonarFrontForward.ping_cm();
  //  distanceForward = 0.5 * (temp1 + temp2);
  distanceForward = sonarFrontForward.ping();
  distanceForward = distanceForward / 57;
  if ((distanceForward >= 40) || (distanceForward == 0)) {
    distanceForward = 40;
  }
  else if (distanceForward <= 0) {
    distanceForward = 12;
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
  distanceLeftFront = sonarLeftFront.ping();
  distanceLeftFront = distanceLeftFront / 57;
  distanceRightFront = sonarRightFront.ping();
  distanceRightFront = distanceRightFront / 57;
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
  distanceLeftBack = sonarLeftBack.ping();
  distanceLeftBack = distanceLeftBack / 57;
  distanceRightBack = sonarRightBack.ping();
  distanceRightBack = distanceRightBack / 57;
  if (distanceLeftBack >= 30 || distanceLeftBack == 0) {
    distanceLeftBack = 30;
  }
  if (distanceRightBack >= 30 || distanceRightBack == 0) {
    distanceRightBack = 30;
  }
}


void pidEncoder() {
  errorEncoder = (countL - countR);
  speedEncoder = constrain(0.1 * errorEncoder + 0.2  * (errorEncoder - last_errorEncoder) , -1, 1); //10
  last_errorEncoder = errorEncoder;
}

void pidEncoderNoWall() {
  errorEncoder = (countL - countR);
  speedEncoder = constrain(0.05 * errorEncoder + 0.2  * (errorEncoder - last_errorEncoder) , -100, 100); //10
  last_errorEncoder = errorEncoder;
}

void pid() {
  sonarFront();
  sonarBack();
  sonarForward();
  pidMax = 120;
  if ((distanceLeftFront == 30) && (distanceRightBack == 30) && (distanceRightFront < 30)) { //left to right
    doubleWall = 3;
    error = (10 - distanceRightFront);

    speed = constrain((error * 8 + 160 * (error - last_errorL) ), -pidMax - 10, pidMax + 10);

    last_errorR = error;
  }
  else if ((distanceRightFront == 30) && (distanceLeftBack == 30) && (distanceLeftFront < 30)) { //right to left
    doubleWall = 4;
    error = (distanceLeftFront - 10);
    speed = constrain((error * 20 + 250 * (error - last_errorL) ), -pidMax - 10, pidMax + 10);
    last_errorL = error;
  }
  else if (((distanceRightFront == 30) && (distanceLeftFront == 30)) || ((distanceRightBack == 30) && (distanceLeftBack == 30))) {    //no wall
    errorEncoder = (countL - countR);
    speedEncoder = constrain(0.05 * errorEncoder + 0.2  * (errorEncoder - last_errorEncoder) , -100, 100); //10
    last_errorEncoder = errorEncoder;
    speed = speedEncoder;
    //    digitalWrite(redLed,HIGH);
    //forwardEncoder();
    //    digitalWrite(redLed,LOW);
    //  speed = 20;




  }
  else if (((distanceLeftFront == 30) && (distanceRightFront < 30)) || ((distanceLeftBack == 30) && (distanceRightBack < 30))) { //right wall follow
    doubleWall = 1;
    //  md.setBrakes(400,400);
    //  delay(5000);
    //        error = (10 - (0.5 * (distanceRightFront + distanceRightBack))) - (distanceRightFront - distanceRightBack);
    error = (10 - distanceRightFront ) - (distanceRightFront - distanceRightBack);
    //    if (error < 2 && error > -2) {
    speed = constrain((error * 20 + 6 * (error - last_errorL) ), -pidMax - 10, pidMax + 10);
    //
    //    }
    //    else {
    //      speed = constrain((error * 0.9 + 15 * (error - last_errorL) ), -pidMax, pidMax);
    //
    //    }
    last_errorR = error;
  }
  else if (((distanceRightFront == 30) && (distanceLeftFront < 30)) || ((distanceRightBack == 30) && (distanceLeftBack < 30))) { // left wall follow
    doubleWall = 2;
    //  md.setBrakes(400,400);
    //  delay(5000);
    //    error = ((0.5 * (distanceLeftBack + distanceLeftFront)) - 10) + (distanceLeftFront - distanceLeftBack);
    error = (distanceLeftFront - 9.5) + (distanceLeftFront - distanceLeftBack);
    speed = constrain((error * 20 + 250 * (error - last_errorL) ), -pidMax - 10, pidMax + 10);

    last_errorL = error;
  }
  else {
    doubleWall = 0;
    pidMax = 120;
    error1 = 0.5 * ((distanceLeftFront - distanceRightFront) + (distanceLeftBack - distanceRightBack));
    error2 = 0.5 * ((distanceLeftFront - distanceLeftBack) - (distanceRightFront - distanceRightBack)); //note
    error = error1 + error2;
    pTerm = Kp * (error);
    integrated_error += error;
    iTerm = Ki * constrain(integrated_error, -GUARD_GAIN, GUARD_GAIN);
    dTerm = Kd * (error - last_error);
    last_error = error;
    speed = constrain(K * (pTerm + dTerm + iTerm), -pidMax, pidMax);
    //  pTerm = 6 * (error1);
    //  integrated_error += error1;
    //  iTerm = Ki * constrain(integrated_error, -GUARD_GAIN, GUARD_GAIN);
    //  dTerm = Kd * (error1 - last_error);
    //  last_error = error1;
    //  speed1 = constrain(K * (pTerm + dTerm + iTerm), -pidMax, pidMax);
    //  //----------------------------------------------------------------------------
    //  pTerm2 = 12 * (error2);
    //  integrated_error2 += error2;
    //  iTerm2 = Ki * constrain(integrated_error2, -GUARD_GAIN, GUARD_GAIN);
    //  dTerm2 = Kd * (error2 - last_error2);
    //  last_error2 = error2;
    //  speed2 = constrain(K * (pTerm2 + dTerm2 + iTerm2), -pidMax, pidMax);
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
