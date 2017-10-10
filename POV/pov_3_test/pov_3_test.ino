  // defining the alphabet
  #include "font.h"
  
  // define the Arduino LED pins in use
  const int LEDpins[] = {10,9,8,7,6,5,4,3};
  int rows= 8;		        // Total LED's in a row
  // number of LEDs
   const int charHeight = 8;
   const int charWidth = 5;
 
  int i;
  char textString[] = "I love ENTC";
  String tmp_str;
  
  void setup()
  {
    for (i = 0; i < rows; i++){
      pinMode(LEDpins[i], OUTPUT);    
    //Serial.begin(9600);
    }
  }
  
  void loop()
  {
      
     for (int k=sizeof(textString)-1; k>-1; k--){
      printLetterboven(textString[k]);
      }  
    delay(2); 
  }
  
  void printLetter(char ch)
  {
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
