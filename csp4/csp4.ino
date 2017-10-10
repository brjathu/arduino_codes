#include "DualVNH5019MotorShield.h"


#define red 45
#define green 47
#define blue 49

#define whiteThreshold 850
#define bgThreshold


int count = 0;
int color[] = {red, green, blue};
int sensor[] = {62, 63, 64, 65, 66, 67, 68, 69};
int pidColor[] = {0, 0, 0, 0, 0, 0, 0, 0};
int pidColorAvg[] = {0, 0, 0, 0, 0, 0, 0, 0};


int rgbMax[3][8] = {0};
int colorArray[3][8] = {0};

int colorError = 0;
int colorErrorLast = 0;
int speedColor;
int colFollor = 0;




int speedEncoder = 0;
float last_errorEncoder = 0;
float errorEncoder = 0;

float last_errorR = 0;
float last_errorL = 0;


#define EncoderLPinA 18//20
#define EncoderLPinB 19//19
#define EncoderRPinA 20
#define EncoderRPinB 21
long countL = 0;
long countR = 0;



#define redLed 15


DualVNH5019MotorShield md;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //   md.init();
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  pinMode(redLed, OUTPUT);


  pinMode(EncoderLPinA, INPUT);
  pinMode(EncoderLPinB, INPUT);
  pinMode(EncoderRPinA, INPUT);
  pinMode(EncoderRPinB, INPUT);


  attachInterrupt(digitalPinToInterrupt(EncoderLPinA), readEncoderL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EncoderRPinA), readEncoderR, CHANGE);

  calibirate2();

}

void loop() {









  //  goBlue();
  //   goRed();
  //goGreen();


  //testGreen();
  //
  //  Serial.println();
  //  delay(100);


  goRed();
//pidCal(1);
//
//    Serial.print(pidColor[0]);
//    Serial.print("\t");
//    Serial.print(pidColor[1]);
//    Serial.print("\t");
//    Serial.print(pidColor[2]);
//    Serial.print("\t");
//    Serial.print(pidColor[3]);
//    Serial.print("\t");
//    Serial.print(pidColor[4]);
//    Serial.print("\t");
//    Serial.print(pidColor[5]);
//    Serial.print("\t");
//    Serial.print(pidColor[6]);
//    Serial.print("\t");
//    Serial.print(pidColor[7]);
//    Serial.print("\t");
//    Serial.print(count);
//    Serial.print("\t");
//    Serial.println();
//  //
//  //  //
//    delay(20);




}


void pidCal(int colCode) {
  count = 0;
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    pidTest2(colCode);
    delay(2);
  }
  for (int j = 0; j < 8; j++) {
    pidColor[j] = pidColorAvg[j] / 3;
    if (pidColor[j] == 1) {
      count++;
    }
    pidColorAvg[j] = 0;
    sum = sum + 1000 * pidColor[j] * j;
  }

  if (count != 0) {
    colorError = 3500 - (sum / count);
  }
  else {
    colorError = 0;
  }
}


void pidTest2(int col) {
  for (int k = 0; k < 3; k++) {
    digitalWrite(color[k], HIGH);
    delay(10);
    for (int i = 0; i < 8; i++) {
      //      colorArray[k][i] = map(getRead(3, sensor[i]),0 , rgbMax[k][i], 0, 1024);
      colorArray[k][i] = getRead(3, sensor[i]);
      if (k == 0) {
        colorArray[k][i] += 50;
      }
      if (k == 2) {
        colorArray[k][i] += 40;
      }
    }
    delay(10);
    digitalWrite(color[k], LOW);

  }

  for (int i = 0; i < 8; i++) {

    if (checkColorTest4(i) == col) {
      pidColorAvg[i] += 1;
    }
  }
}




int checkColorTest4(int pin) {

  if ((colorArray[0][pin] + colorArray[1][pin] + colorArray[2][pin]) / 3 > (rgbMax[0][pin] + rgbMax[1][pin] + rgbMax[2][pin] ) / 3  ) {
    return -1;

  }
  else {
    int x[] = {colorArray[0][pin], colorArray[1][pin], colorArray[2][pin]};
    int max = findMax(x);

    if (max == 0) {

      return 0;

    }
    else if (max == 1) {

      return 1;

    }
    else if (max == 2) {
      return 2;
    }
    else {
      return 3;
    }
  }


}


void goRed() {

  pidCal(1);////////////////////////////////////////////////////////

  if (count > 0) {

    if (count == 8) {
      delay(100000);
    }
    if (colFollor == 0) {
      digitalWrite(redLed, HIGH);
      delay(200);
      digitalWrite(redLed, LOW);


      countR = 0;
      countL = 0;


      md.setBrakes(400, 400);
      delay(500);
      md.setSpeeds(-290 , -300 );
      delay(5);
      while (abs(countL + countR) < 120) {
        pidEncoder();
//        pidCal(1);////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//        speedColor = constrain( 2 * colorError + 0  * (colorError - colorErrorLast) , -40, 40);
        md.setSpeeds(-150 + speedEncoder , -180 - speedEncoder ); //200,250
      }



      delay(10);

      colFollor = 1;


    }
    // 2,1.5
    //0.008, 0.08

    speedColor = constrain( 2 * colorError + 1.5  * (colorError - colorErrorLast) , -200, 200);
    colorErrorLast = colorError;

    md.setSpeeds(290 - speedColor, 310 + speedColor);
    delay(20);
    md.setBrakes(200, 200);
  }

  else {

    colFollor = 0;
    countR = 0;
    countL = 0;
    md.setBrakes(400, 400);
    delay(500);
    md.setSpeeds(290 , 300 );
    delay(5);
    while (count == 0) {
//      md.setBrakes(100,100);
      pidEncoder();
      pidCal(1);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // md.setSpeeds(190 - speed1 - speed2 - speedEncoder, 215 + speed1 + speed2 + speedEncoder);
      md.setSpeeds(70 - speedEncoder , 90 + speedEncoder ); //200,250
      delay(5);
      md.setBrakes(400,400);
      
    }

    md.setBrakes(400, 400);
    delay(500);
  }



}

void goBlue() {
  testBlue();

  if (count > 0) {

    if (count == 8) {
      delay(100000);
    }
    if (colFollor == 0) {



      countR = 0;
      countL = 0;


      md.setBrakes(400, 400);
      delay(500);
      md.setSpeeds(-290 , -300 );
      delay(10);
      while (abs(countL + countR) < 70) {
        testBlue();
        pidEncoder();
        speedColor = constrain( 2 * colorError + 0  * (colorError - colorErrorLast) , -40, 40);
        md.setSpeeds(-150 + speedEncoder - speedColor , -180 - speedEncoder + speedColor); //200,250
      }



      delay(10);

      colFollor = 1;


    }
    // 2,1.5
    //0.008, 0.08

    speedColor = constrain( 2 * colorError + 1.5  * (colorError - colorErrorLast) , -200, 200);
    colorErrorLast = colorError;

    md.setSpeeds(290 - speedColor, 310 + speedColor);
    delay(10);
    md.setBrakes(300, 300);
  }

  else {
    colFollor = 0;
    countR = 0;
    countL = 0;
    md.setBrakes(400, 400);
    delay(500);
    md.setSpeeds(290 , 300 );
    delay(10);
    while (count == 0) {
      pidEncoder();
      testBlue();
      // md.setSpeeds(190 - speed1 - speed2 - speedEncoder, 215 + speed1 + speed2 + speedEncoder);
      md.setSpeeds(90 - speedEncoder , 110 + speedEncoder ); //200,250
    }

    md.setBrakes(400, 400);
    delay(500);
  }
}


void testBlue() {
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);

  int blueVal = 0;
  count = 0;
  for (int i = 0; i < 8; i++) {
    blueVal = analogRead(sensor[i]);
    if ((rgbMax[0][i] + rgbMax[1][i] + rgbMax[2][i] ) / 3 - blueVal > 170) {
      Serial.print("blue");
      count++;
      pidColor[i] = 1;
      Serial.print("\t");
    }
    else {
      pidColor[i] = 0;
      Serial.print("white");
      Serial.print("\t");
    }

  }



  int sum = 0;
  for (int i = 0; i < 8; i++) {
    sum = sum + 1000 * pidColor[i] * i;
  }
  if (count != 0) {
    colorError = 3500 - (sum / count);
  }
  else {
    colorError = 0;
  }





}




void goGreen() {
  testGreen();

  if (count > 0) {

    if (count == 8) {
      delay(100000);
    }
    if (colFollor == 0) {



      countR = 0;
      countL = 0;


      md.setBrakes(400, 400);
      delay(500);
      md.setSpeeds(-290 , -300 );
      delay(10);
      while (abs(countL + countR) < 70) {
        testGreen();
        pidEncoder();
        speedColor = constrain( 2 * colorError + 0  * (colorError - colorErrorLast) , -40, 40);
        md.setSpeeds(-150 + speedEncoder - speedColor , -180 - speedEncoder + speedColor); //200,250
      }



      delay(10);

      colFollor = 1;


    }
    // 2,1.5
    //0.008, 0.08

    speedColor = constrain( 2 * colorError + 1.5  * (colorError - colorErrorLast) , -200, 200);
    colorErrorLast = colorError;

    md.setSpeeds(290 - speedColor, 310 + speedColor);
    delay(10);
    md.setBrakes(300, 300);
  }

  else {
    colFollor = 0;
    countR = 0;
    countL = 0;
    md.setBrakes(400, 400);
    delay(500);
    md.setSpeeds(290 , 300 );
    delay(10);
    while (count == 0) {
      pidEncoder();
      testGreen();
      // md.setSpeeds(190 - speed1 - speed2 - speedEncoder, 215 + speed1 + speed2 + speedEncoder);
      md.setSpeeds(90 - speedEncoder , 110 + speedEncoder ); //200,250
    }

    md.setBrakes(400, 400);
    delay(500);
  }
}











void testGreen() {
  digitalWrite(red, HIGH);
  digitalWrite(blue, HIGH);

  int greenVal = 0;
  count = 0;
  for (int i = 0; i < 8; i++) {
    greenVal = analogRead(sensor[i]);
    if ((rgbMax[0][i] + rgbMax[1][i] + rgbMax[2][i] ) / 3 - greenVal > 200) {
      Serial.print("green");
      count++;
      pidColor[i] = 1;
      Serial.print("\t");
    }
    else {
      pidColor[i] = 0;
      Serial.print("white");
      Serial.print("\t");
    }

  }



  int sum = 0;
  for (int i = 0; i < 8; i++) {
    sum = sum + 1000 * pidColor[i] * i;
  }
  if (count != 0) {
    colorError = 3500 - (sum / count);
  }
  else {
    colorError = 0;
  }


}






void pidTest() {
  for (int k = 0; k < 3; k++) {


    digitalWrite(color[k], HIGH);


    delay(10);

    for (int i = 0; i < 8; i++) {
      //      colorArray[k][i] = map(getRead(3, sensor[i]),0 , rgbMax[k][i], 0, 1024);
      colorArray[k][i] = getRead(3, sensor[i]);
      if (k == 0) {
        colorArray[k][i] += 50;
      }
      if (k == 2) {
        colorArray[k][i] += 40;
      }
    }
    delay(10);
    digitalWrite(color[k], LOW);

  }

  for (int i = 0; i < 8; i++) {
    Serial.print(colorArray[0][i]);
    Serial.print(" ");
    Serial.print(colorArray[1][i]);
    Serial.print(" ");
    Serial.print(colorArray[2][i]);
    Serial.print("     ");

  }






  //  for (int i = 0; i < 8; i++) {
  //
  //    if ((colorArray[0][i] + colorArray[1][i] + colorArray[2][i]) / 3 > (rgbMax[0][i] + rgbMax[1][i] + rgbMax[2][i] ) / 3  ) {
  //      Serial.print("white\t");
  //
  //    }
  //    else {
  //      int x[] = {colorArray[0][i], colorArray[1][i], colorArray[2][i]};
  //      int max = findMax(x);
  //
  //      if (max == 0) {
  //
  //        Serial.print("red\t");
  //
  //
  //      }
  //      else if (max == 1) {
  //
  //        Serial.print("Green\t");
  //
  //
  //      }
  //      else if (max == 2) {
  //        Serial.print("Blue\t");
  //      }
  //      else {
  //        Serial.print("error-1");
  //      }
  //    }
  //  }
}












































int getRead(int x, int pin) {
  int sum = 0;
  for (int i = 0; i < x; i++) {
    sum += analogRead(pin);
  }
  return (sum / x);
}



int findMax(int n[]) {
  int m = 0;
  int maxVal = 0;
  for (int i = 0; i < 3; i++) {

    if (n[i] > maxVal) {
      maxVal = n[i];
      m = i;
    }
  }
  return m;
}









void calibirate2() {



  //delay(100);
  //for(int k=0;k<3;k++){
  //
  //  int rm = 0;
  //  digitalWrite(color[k], HIGH);
  //  for (int i = 0; i < 8; i++) {
  //    rgbMin[k][i]=1024;
  //    for (int j = 0; j < 10; j++) {
  //      rm = analogRead(sensor[i]);
  //
  //      if (rm < rgbMin[k][i]) {
  //        rgbMin[k][i] = rm;
  //      }
  //      delay(10);
  //    }
  //
  //    delay(10);
  //  }
  //  digitalWrite(color[k], LOW);
  //  delay(100);
  //
  //}
  //
  //
  //  for (int j = 0; j < 3; j++) {
  //    for (int i = 0; i < 8; i++) {
  //      Serial.print(rgbMin[j][i]);
  //      Serial.print("  ");
  //    }
  //    Serial.println();
  //
  //  }
  //
  //
  //delay(5000);
  delay(1000);
  for (int k = 0; k < 3; k++) {

    //red
    int rm = 0;



    digitalWrite(color[k], HIGH);



    delay(10);
    for (int i = 0; i < 8; i++) {
      int sum = 0;
      for (int j = 0; j < 20; j++) {

        rm = analogRead(sensor[i]);
        sum += rm;

        delay(10);
      }
      rgbMax[k][i] = sum / 20;

      delay(10);
    }
    digitalWrite(color[k], LOW);
    delay(100);

  }

  delay(1000);


  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 8; i++) {
      Serial.print(rgbMax[j][i]);
      Serial.print("  ");
    }
    Serial.println();

  }





}









void forwardEncoder() {



  //digitalWrite(redLed, LOW);





}


void pidEncoder() {
  errorEncoder = ((countL) - countR);
  speedEncoder = constrain(0.1 * errorEncoder + 0.2  * (errorEncoder - last_errorEncoder) , -100, 100); //10
  last_errorEncoder = errorEncoder;
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

