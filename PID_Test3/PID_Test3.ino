#include <QTRSensors.h>

#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

#define ENA 10
#define ENB 11

#define IN1 12
#define IN2 13
#define IN3 9
#define IN4 8

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) { 3,4, 5, 6, A0, A1, A2,A3},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);
   
unsigned int sensorValues[NUM_SENSORS];
unsigned int position; 

float kp = 40;
float kd = 0;
float ki = 0;
int error = 0;
int lastError = 0;
int proportional = 0;
int derivative = 0;

int lmSpeed=70;
int rmSpeed=70;
int baseSpeed=100;
float pidSpeed=0;

int maxSpeed=200;

void setup()
{
pinMode(ENA,OUTPUT);
pinMode(ENB,OUTPUT);

Serial.begin(9600);
  
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
      // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}


void loop()
{



 for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
 
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  unsigned int position = qtrrc.readLine(sensorValues);
  
  error=3500-position;
  derivative =lastError-error;
  lastError=error;
  
  pidSpeed=40*error+20*derivative;
  pidSpeed=pidSpeed/1000;
  lmSpeed=baseSpeed+pidSpeed;
  rmSpeed=baseSpeed-pidSpeed;

  if(lmSpeed<0){
    lmSpeed=0;
  }
  else if(lmSpeed>maxSpeed){
    lmSpeed=maxSpeed;
  }

  if(rmSpeed<0){
    rmSpeed=0;
  }
  else if(rmSpeed>maxSpeed){
    rmSpeed=maxSpeed;
  }
  
  forward(lmSpeed,rmSpeed);

  Serial.print(position);
  Serial.print( "      ");
  Serial.print(lmSpeed);
  Serial.print( "      ");
  Serial.print(rmSpeed);
  Serial.print( "      ");
  Serial.println(pidSpeed);

  delay(50);
}




void forward(int speedL,int speedR){
 
analogWrite(ENA,speedL);
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);

analogWrite(ENB,speedR);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);

}


void backward(int speed){


analogWrite(ENA,speed);
digitalWrite(IN1,HIGH);
digitalWrite(IN2,LOW);

analogWrite(ENB,speed);
digitalWrite(IN3,HIGH);
digitalWrite(IN4,LOW);

}



void left(int speed){



analogWrite(ENA,0);
digitalWrite(IN1,LOW);
digitalWrite(IN2,LOW);

analogWrite(ENB,speed);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);

}


void right(int speed){



analogWrite(ENA, speed);
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);

analogWrite(ENB,0);
digitalWrite(IN3,LOW);
digitalWrite(IN4,LOW);

}

 

