// http://www.bajdi.com
// Self balancing bot:
// µController = ATmega328
// 2 Pololu micro metal gear motors with 60mm wheels + DRV8833 motor controller
// 6DOF MPU6050 sensor 

#include <Wire.h>
#include "Kalman.h" // Source: https://github.com/TKJElectronics/KalmanFilter
#include "I2C.h"

Kalman kalmanX; // Create the Kalman instance

/* IMU Data */
int16_t accX, accY, accZ;
int16_t tempRaw;
int16_t gyroX, gyroY, gyroZ;

float accXangle;//, accYangle; // Angle calculate using the accelerometer
float gyroXangle;//, gyroYangle; // Angle calculate using the gyro
float kalAngleX;//, kalAngleY; // Calculate the angle using a Kalman filter

unsigned long timer;
uint8_t i2cData[14]; // Buffer for I2C data
float CurrentAngle;

// Motor controller pins
const int ENA =10;
const int ENB =11;
const int IN1 = 8;  // (pwm) pin 3 connected to pin AIN1 
const int IN2 = 9;  // (pwm) pin 9 connected to pin AIN2 
const int IN3 = 12; // (pwm) pin 10 connected to pin BIN1  
const int IN4 = 13;  // (pwm) pin 11 connected to pin BIN2 

int speed=0;

// PID
 float Kp = 30; //25
 float Ki = 0;
 float Kd = 8;//8
float pTerm, iTerm, dTerm, integrated_error, last_error, error;
const float K = 1.9;//1.9*1.12;
#define   GUARD_GAIN   10.0

#define runEvery(t) for (static typeof(t) _lasttime;(typeof(t))((typeof(t))millis() - _lasttime) > (t);_lasttime += (t))

int angleO=179.5;//179.5

//MPU6050 accelgyro;

int kpx;
int kdx;
int kix;


void setup() {  
  pinMode(IN1, OUTPUT); // set pins to output
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial.begin(115200);

  Wire.begin();
  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while(i2cWrite(0x19,i2cData,4,false)); // Write to all four registers at once
  while(i2cWrite(0x6B,0x01,true)); // PLL with X axis gyroscope reference and disable sleep mode 

    while(i2cRead(0x75,i2cData,1));
  if(i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while(1);
  }

  delay(100); // Wait for sensor to stabilize

  /* Set kalman and gyro starting angle */
  while(i2cRead(0x3B,i2cData,6));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  accXangle = (atan2(accY,accZ)+PI)*RAD_TO_DEG;

  kalmanX.setAngle(accXangle); // Set starting angle
  gyroXangle = accXangle;
  timer = micros();
}

void loop() {
Serial.print(CurrentAngle);
Serial.print("   ");
Serial.print(Kp);
Serial.print("   ");
Serial.print(Kd);
Serial.print("   ");
Serial.println(Ki);

  runEvery(10)  // run code @ 40 Hz
  {
    dof();
    if (CurrentAngle <= angleO+1 && CurrentAngle >= angleO-1)
    {
      stop();
    }
    else{
    if (CurrentAngle < 240 && CurrentAngle > 120)
    {
      
    Pid();
    Motors();
    }
    else
    {
      stop();
    }
  }
  }
}

void Motors(){
  if (speed > 0)
  { 
    //forward 
   
//    analogWrite(AIN1, speed);
//    analogWrite(AIN2, 0);
//    analogWrite(BIN1, speed);
//    analogWrite(BIN2, 0);

    analogWrite(ENA,speed);
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);

analogWrite(ENB,speed);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);
  }
  else
  { 
    // backward
    speed = map(speed,0,-255,0,255);
//    analogWrite(AIN1, 0);
//    analogWrite(AIN2, speed);
//    analogWrite(BIN1, 0);
//    analogWrite(BIN2, speed);

    analogWrite(ENA,speed);
digitalWrite(IN1,HIGH);
digitalWrite(IN2,LOW);

analogWrite(ENB,speed);
digitalWrite(IN3,HIGH);
digitalWrite(IN4,LOW);
  }
}

void stop()
{
analogWrite(ENA,0);
analogWrite(ENB,0);
}

void Pid(){
 error = angleO - CurrentAngle;  // 180 = level
 if(abs(error)<5){
  Kp=24;
 }
 else{
  Kp=40;
 }

         pTerm = Kp * (error);
  integrated_error += error;
  iTerm = Ki * constrain(integrated_error, -GUARD_GAIN, GUARD_GAIN);
  dTerm = Kd * (error - last_error);
  last_error = error;
  speed = constrain(K*(pTerm + iTerm + dTerm), -255, 255);
}

void dof()
{


  while(i2cRead(0x3B,i2cData,14));
  accX = ((i2cData[0] << 8) | i2cData[1]) ;
  accY = ((i2cData[2] << 8) | i2cData[3]) ;
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  tempRaw = ((i2cData[6] << 8) | i2cData[7]);  
  gyroX = ((i2cData[8] << 8) | i2cData[9]);
  gyroY = ((i2cData[10] << 8) | i2cData[11]);
  gyroZ = ((i2cData[12] << 8) | i2cData[13]);
  accXangle = (atan2(accY,accZ)+PI)*RAD_TO_DEG;
  double gyroXrate = (double)gyroX/131.0;
  CurrentAngle = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros()-timer)/1000000);
  timer = micros();
}

