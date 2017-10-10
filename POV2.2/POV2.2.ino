#include "font.h"
const int LEDpins[] = {10,9,8,7,6,5,4,3};
int rows= 8;            // Total LED's in a row
const int charHeight = 8;
const int charWidth = 5;
 
int i;

//for bluetooth
char junk;
String inputString="ENTC";


void setup() {      
  Serial.begin(9600);
  for (i = 0; i < rows; i++){
      pinMode(LEDpins[i], OUTPUT);    
  }  
}

void loop() {

Serial.println(analogRead(A0));

  if(Serial.available()){
  while(Serial.available())
    {
      char inChar = (char)Serial.read(); //read the input
      inputString += inChar;        //make a string of the characters coming on serial
      if(inChar=='#'){
        inputString="";
      }
    }
    Serial.println(inputString);
  }

  while(analogRead(A0)<100)
    {
     digitalWrite(13, LOW);   // set the LED off
    }
    digitalWrite(13, HIGH);
    
    for (int i=inputString.length()-1;i>-1;i++){
           printLetterboven(inputString[i]); 
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

