#include <Wire.h>
#include <HMC5883L.h>
 #include "font.h"
  

HMC5883L compass;



const int LEDpins[] = {10,9,8,7,6,5,4,3};
int rows= 8;            // Total LED's in a row
const int charHeight = 8;
const int charWidth = 5;
 
int i;
char textString[] = "I love ENTC";

//for bluetooth
char junk;
String inputString="";


void setup() {      
  Serial.begin(9600);
  Wire.begin();
  for (i = 0; i < rows; i++){
      pinMode(LEDpins[i], OUTPUT);    
  }

  Serial.println("Initialize HMC5883L");
  while (!compass.begin())
  {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_75HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);

  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(0, 0);
  
  
}

void loop() {


while(1){
  Vector norm = compass.readNormalize();
  float heading = atan2(norm.YAxis, norm.XAxis);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }
  float headingDegrees = heading * 180/M_PI; 
  Serial.print(headingDegrees);
  Serial.println();
  if(headingDegrees<10 &&  headingDegrees>0){
   digitalWrite(13,HIGH);
   break;
  }
  digitalWrite(13,LOW);
  
       for (int k=sizeof(textString)-1; k>-1; k--){
           printLetterboven(textString[k]);
       }   

}
}


void printLetter(char ch)  {
    // make sure the character is within the alphabet bounds (defined by the font.h file)
    // if it's not, make it a blank character
    if (ch < 32 || ch > 126){
      ch = 32;
      }
    // subtract the space character (converts the ASCII number to the font index number)
    ch -= 32;
    // step through each byte of the character array
    for (int i=0; i<charWidth; i++) {
      byte b = font[ch][i];
      // ventilator draai tegen de klok in
      for (int j=0; j<charHeight; j++) {
        digitalWrite(LEDpins[j], bitRead(b, 7-j));
        }
      delay(1);
      }
    //clear the LEDs
    for (i = 0; i < rows; i++)
      digitalWrite(LEDpins[i] , LOW);
    // space between letters
    delay(1);
  
  }


  void printLetterboven(char ch)
  {
    // make sure the character is within the alphabet bounds (defined by the font.h file)
    // if it's not, make it a blank character
    if (ch < 32 || ch > 126){
      ch = 32;
      }
    // subtract the space character (converts the ASCII number to the font index number)
    ch -= 32;
    // step through each byte of the character array
    for (int i=charWidth-1; i>-1; i--) {
      byte b = font[ch][i];
      // ventilator draai tegen de klok in
      for (int j=0; j<charHeight; j++) {
        digitalWrite(LEDpins[j], bitRead(b,j));
        }
      delay(1);
      }
    //clear the LEDs
    for (i = 0; i < rows; i++)
      digitalWrite(LEDpins[i] , LOW);
    // space between letters
    delay(1);
  
  }  

