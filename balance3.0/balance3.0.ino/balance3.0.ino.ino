#include <Wire.h>
#include "Kalman.h" // Source: https://github.com/TKJElectronics/KalmanFilter
#include "I2C.h"
#include "DualVNH5019MotorShield.h"


DualVNH5019MotorShield md;
Kalman kalmanY; 

/* IMU Data */
int16_t accX, accY, accZ;
int16_t tempRaw;
int16_t gyroX, gyroY, gyroZ;

float accYangle;//, accYangle; // Angle calculate using the accelerometer
float gyroYangle;//, gyroYangle; // Angle calculate using the gyro
float kalAngleY;//, kalAngleY; // Calculate the angle using a Kalman filter

unsigned long timer;
uint8_t i2cData[14]; // Buffer for I2C data
float CurrentAngle;

int speed;

// PID
 float Kp = 5; 
 float Ki = 0;
 float Kd = 0;
float pTerm, iTerm, dTerm, integrated_error, last_error, error;
const float K = 1.9;//1.9*1.12;
#define   GUARD_GAIN   10.0

#define runEvery(t) for (static typeof(t) _lasttime;(typeof(t))((typeof(t))millis() - _lasttime) > (t);_lasttime += (t))

float angleO=178.8;

//MPU6050 accelgyro;

char junk;
String inputString="";

void setup() {  
  Serial.begin(9600);
  md.init();

//kpx=map(analogRead(A0),0,1023,30,70);
//kdx=map(analogRead(A1),0,1023,-10,20);
//kix=map(analogRead(A2),0,1023,0,20);
//
//    accelgyro.setXGyroOffset(-919);
//    accelgyro.setYGyroOffset(-2148);
//    accelgyro.setZGyroOffset(5544);
//
//     accelgyro.setXAccelOffset(71);
//    accelgyro.setYAccelOffset(-23);
//    accelgyro.setZAccelOffset(-15);

    

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
  accYangle = (atan2(accX,accZ)+PI)*RAD_TO_DEG;

  kalmanY.setAngle(accYangle); // Set starting angle
  gyroYangle = accYangle;
  timer = micros();
// Kp=kpx;
//  Kd=kdx;
//  Ki=kix;
}

void loop() {
Serial.print(CurrentAngle);
Serial.print("   ");
Serial.print(speed);
Serial.println("   ");
//Serial.print(Kd);
//Serial.print("   ");
//Serial.println(Ki);

 
//Serial.println(angleO);

  runEvery(5){
    dof();
    if (CurrentAngle <= angleO+0.1 && CurrentAngle >= angleO-0.1)
    {
      stop();
    }
    else{
    if (CurrentAngle < 260 && CurrentAngle > 100)
    {
      
    Pid();
    md.setSpeeds(-speed,-speed);
    }
    else
    {
      stop();
    }
  }
  }
}



void stop()
{
md.setBrakes(0,0);
}

void Pid(){
  error = angleO - CurrentAngle;  // 180 = level

  pTerm = Kp * (error);
  integrated_error += error;
  iTerm = Ki * constrain(integrated_error, -GUARD_GAIN, GUARD_GAIN);
  dTerm = Kd * (error - last_error);
  last_error = error;
  speed = constrain(K*(pTerm + iTerm + dTerm), -100, 100);
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
//  accXangle = (atan2(accY,accZ)+PI)*RAD_TO_DEG;
//  double gyroXrate = (double)gyroX/131.0;
//  CurrentAngle = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros()-timer)/1000000);

  accYangle = (atan2(accX,accZ)+PI)*RAD_TO_DEG;
  double gyroYrate = (double)gyroY/131.0;
  CurrentAngle = kalmanY.getAngle(accYangle, gyroYrate, (double)(micros()-timer)/1000000);
  timer = micros();
}




////////////////////////motor controller
void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while(1);
  }
}
