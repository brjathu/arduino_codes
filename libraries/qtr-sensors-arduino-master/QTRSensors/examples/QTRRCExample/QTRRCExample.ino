#include <QTRSensors.h>

#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

#define ENA 10
#define ENB 11

#define IN1 A0
#define IN2 A1
#define IN3 A2
#define IN4 A3

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6, 7, 8, 9, 12},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];


void setup()
{

  
//  delay(500);
//  pinMode(13, OUTPUT);
//  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
//  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
//  {
//    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
//  }
//  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration
//
//  // print the calibration minimum values measured when emitters were on
//  Serial.begin(9600);
//  for (int i = 0; i < NUM_SENSORS; i++)
//  {
//    Serial.print(qtrrc.calibratedMinimumOn[i]);
//    Serial.print(' ');
//  }
//  Serial.println();
//  
//  // print the calibration maximum values measured when emitters were on
//  for (int i = 0; i < NUM_SENSORS; i++)
//  {
//    Serial.print(qtrrc.calibratedMaximumOn[i]);
//    Serial.print(' ');
//  }
//  Serial.println();
//  Serial.println();
//  delay(1000);
}


void loop()
{





  
//  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
//  // To get raw sensor values, call:
//  //  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
//  unsigned int position = qtrrc.readLine(sensorValues);
//
//  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
//  // 1000 means minimum reflectance, followed by the line position
//  for (unsigned char i = 0; i < NUM_SENSORS; i++)
//  {
//    Serial.print(sensorValues[i]);
//    Serial.print('\t');
//  }
//  //Serial.println(); // uncomment this line if you are using raw values
//  Serial.println(position); // comment this line out if you are using raw values
//  
//  delay(250);
}
