#include "DualVNH5019MotorShield.h"


#define red 45
#define green 47
#define blue 49

#define whiteThreshold 970
#define bgThreshold 60

int color[] = {red, green, blue};
//int sensor[] = {54,55, 56, 57, 58, 59,60,61};
int sensor[] = {62, 63, 64, 65, 66, 67, 68, 69};
int pidColor[] = {0, 0, 0, 0, 0, 0, 0, 0};

int redMax[] = {0, 0, 0, 0, 0, 0, 0, 0};
int greenMax[] = {0, 0, 0, 0, 0, 0, 0, 0};
int blueMax[] = {0, 0, 0, 0, 0, 0, 0, 0};


int rgbMax[3][8] = {0};
int colorError = 0;
int colorErrorLast = 0;
int speedColor;


//DualVNH5019MotorShield md;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //   md.init();
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);


  calibirate2();

}

void loop() {

  //pidColorGo(1);
  //  md.setSpeeds(180+speedColor,180-speedColor);
  //delay(150);
  //md.setBrakes(400,400);
  //delay(10);
  //
//  checkColorTest3(69);
//  Serial.println();
//  delay(100);



    pidColorFollow(1);
    Serial.print(pidColor[0]);
    Serial.print("\t");
    Serial.print(pidColor[1]);
    Serial.print("\t");
    Serial.print(pidColor[2]);
    Serial.print("\t");
    Serial.print(pidColor[3]);
    Serial.print("\t");
    Serial.print(pidColor[4]);
    Serial.print("\t");
    Serial.print(pidColor[5]);
    Serial.print("\t");
    Serial.print(pidColor[6]);
    Serial.print("\t");
    Serial.print(pidColor[7]);
    Serial.print("\t");
    Serial.print(colorError);
    Serial.print("\t");
    Serial.println();
    delay(10);


  //
  //for (int i=0;i<8;i++){
  //  Serial.print(analogRead(sensor[i]));
  //   Serial.print("\t");
  //}
  //Serial.println();
  //delay(100);
}


//void pidColorGo(int color) {
//  pidColorFollow(color);
//  speedColor = constrain(0.015 * colorError + 0  * (colorError - colorErrorLast) , -150, 150); //10
//  colorErrorLast = colorError;
//}


void pidColorFollow(int followColor) {
  int sum = 0;
  int count = 0;
  int col = -5;

  for (int i = 0; i < 8; i++) {
    col = checkColor(sensor[i]);
    if (col == followColor  ) {
      pidColor[i] = 1;
      count++;
    }
    else {
      pidColor[i] = 0;
    }
  }

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



int checkColor(int pin) {
  int x[3];
  for (int i = 0; i < 3; i++) {
    digitalWrite(color[i], HIGH);
    delay(5);
    x[i] = map(getRead(3, pin), 0, rgbMax[i][pin - sensor[0]], 0, 1024);//getRead(1, pin);
    digitalWrite(color[i], LOW);
    delay(5);
  }
  if ((x[0] + x[1] + x[2]) / 3 > whiteThreshold) {
    return -1; /// white
  }
  else {
    int max = findMax(x);
    if (max == 2) {
      return 0; /// Red

    }
    else  {
      if (abs(x[0] - x[1]) < bgThreshold) {
        return 2; /// Blue
      }
      else {
        return 1; /// Green
      }

    }
    
  }
}



int checkColorTest3(int pin) {



  int x[3];
  for (int i = 0; i < 3; i++) {
    digitalWrite(color[i], HIGH);
    delay(5);
    x[i] = map(getRead(3, pin), 0, rgbMax[i][pin - sensor[0]], 0, 1024);
    Serial.print(x[i]);
    Serial.print("   ");
    digitalWrite(color[i], LOW);
    delay(5);
  }
//  Serial.print((x[0] + x[1] + x[2]) / 3);
  if ((x[0] + x[1] + x[2]) / 3 > whiteThreshold) {
    Serial.print("white\t");
    return -1; /// white
  }
  else {
    int max = findMax(x);
    if (max == 0) {
     
//        Serial.print("red\t");
        return 0; /// red
      

    }
    else if (max == 1) {
      
//        Serial.print("Green\t");
        return 1; /// Green
     

    }
    else if (max == 2) {
//      Serial.print("Blue\t");
      return 2; /// blue
    }
    else {
//      Serial.print("error-1");
      return 3; /// error
    }
  }
//  Serial.print("\n");


}



int checkColorTest2(int pin) {
  int x[3];
  for (int i = 0; i < 3; i++) {
    digitalWrite(color[i], HIGH);
    delay(10);
    x[i] = getRead(3, pin);
    Serial.print(x[i]);
    Serial.print("  ");
    digitalWrite(color[i], LOW);
    delay(10);
  }
  Serial.print((x[0] + x[1] + x[2]) / 3);
  if ((x[0] + x[1] + x[2]) / 3 > whiteThreshold) {
    Serial.print("wh");
    return -1; /// white
  }
  else {
    int max = findMax(x);
    if (max == 0) {
      if (abs(x[0] - x[1]) < bgThreshold) {
        Serial.print("bl");
        return 2; /// Blue
      }
      else {
        Serial.print("gr");
        return 1; /// Green
      }

    }
    else if (max == 1) {
      if (abs(x[0] - x[1]) < bgThreshold) {
        Serial.print("bl");
        return 2; /// Blue
      }
      else {
        Serial.print("gr");
        return 1; /// Green
      }

    }
    else if (max == 2) {
      Serial.print("re");
      return 0; /// Red
    }
    else {
      Serial.print("3");
      return 3; /// error
    }
  }
}



int checkColorTest(int pin) {
  int x[3];
  for (int i = 0; i < 3; i++) {
    digitalWrite(color[i], HIGH);
    delay(10);
    x[i] = getRead(5, pin);
    Serial.print(x[i]);
    Serial.print("   ");
    digitalWrite(color[i], LOW);
    delay(10);
  }
  Serial.print((x[0] + x[1] + x[2]) / 3);
  if ((x[0] + x[1] + x[2]) / 3 > whiteThreshold) {
    Serial.print("white\t");
    return -1; /// white
  }
  else {
    int max = findMax(x);
    if (max == 0) {
      if (abs(x[0] - x[1]) < bgThreshold) {
        Serial.print("Blue\t");
        return 2; /// Blue
      }
      else {
        Serial.print("Green\t");
        return 1; /// Green
      }

    }
    else if (max == 1) {
      if (abs(x[0] - x[1]) < bgThreshold) {
        Serial.print("Blue\t");
        return 2; /// Blue
      }
      else {
        Serial.print("Green\t");
        return 1; /// Green
      }

    }
    else if (max == 2) {
      Serial.print("Red\t");
      return 0; /// Red
    }
    else {
      Serial.print("error-1");
      return 3; /// error
    }
  }
  Serial.print("\n");
}




int getRead(int x, int pin) {
  int sum = 0;
  for (int i = 0; i < x; i++) {
    sum += analogRead(pin);
  }
  if (pin == 63) {
    return (sum / x) + 50;
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





void calibirate() {




  //red
  int rm = 0;
  digitalWrite(red, HIGH);
  for (int i = 0; i < 8; i++) {



    for (int j = 0; j < 10; j++) {
      rm = analogRead(sensor[i]);
      if (rm > redMax[i]) {

        redMax[i] = rm;
      }
      delay(10);
    }

    delay(10);
  }
  digitalWrite(red, LOW);
  delay(100);



  //green
  digitalWrite(green, HIGH);
  for (int i = 0; i < 8; i++) {


    for (int j = 0; j < 10; j++) {
      rm = analogRead(sensor[i]);
      if (rm > greenMax[i]) {

        greenMax[i] = rm;
      }
      delay(10);
    }


  }
  digitalWrite(green, LOW);
  delay(100);


  //blue
  digitalWrite(blue, HIGH);
  for (int i = 0; i < 8; i++) {


    for (int j = 0; j < 10; j++) {
      rm = analogRead(sensor[i]);
      if (rm > blueMax[i]) {

        blueMax[i] = rm;
      }
      delay(10);
    }


  }
  digitalWrite(blue, LOW);
  delay(100);



  for (int i = 0; i < 8; i++) {
    Serial.print(redMax[i]);
    Serial.print("  ");
  }
  Serial.println();

  for (int i = 0; i < 8; i++) {
    Serial.print(greenMax[i]);
    Serial.print("  ");
  }
  Serial.println();
  for (int i = 0; i < 8; i++) {
    Serial.print(blueMax[i]);
    Serial.print("  ");
  }
  Serial.println();



}






void calibirate2() {




  //red
  int rm = 0;
  digitalWrite(red, HIGH);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 10; j++) {
      rm = analogRead(sensor[i]);
      if (rm > rgbMax[0][i]) {
        rgbMax[0][i] = rm;
      }
      delay(10);
    }

    delay(10);
  }
  digitalWrite(red, LOW);
  delay(100);



  //green
  digitalWrite(green, HIGH);
  for (int i = 0; i < 8; i++) {


    for (int j = 0; j < 10; j++) {
      rm = analogRead(sensor[i]);
      if (rm > rgbMax[1][i]) {

        rgbMax[1][i] = rm;
      }
      delay(10);
    }


  }
  digitalWrite(green, LOW);
  delay(100);


  //blue
  digitalWrite(blue, HIGH);
  for (int i = 0; i < 8; i++) {


    for (int j = 0; j < 10; j++) {
      rm = analogRead(sensor[i]);
      if (rm > rgbMax[2][i]) {

        rgbMax[2][i] = rm;
      }
      delay(10);
    }


  }
  digitalWrite(blue, LOW);
  delay(100);


  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 8; i++) {
      Serial.print(rgbMax[j][i]);
      Serial.print("  ");
    }
    Serial.println();

  }





}


