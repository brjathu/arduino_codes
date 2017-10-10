#include <SoftwareSerial.h> 
#define terminator 10 // DEC value for a LF(line feed) to skip while loop




String IncDataSerial = "";
 
void setup()
{
  delay(1000);
  Serial.begin(19200);
  Serial1.begin(19200);
 
  // Automatically power up the SIM900.
  pinMode(9, OUTPUT);
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2500);
  digitalWrite(9,LOW);
  delay(3500);
  // End of SIM900 power up.
}
 
void loop()
{
  if (Serial1.available()>0)  // if date is comming from softwareserial port ==> data is comming from gprs shield
  {
    boolean getLF = false;
    while(Serial1.available()>0 && !getLF)  // reading data into string if activity is on port and getLF is false ==> no LF have been send
    {
      char buffer=Serial1.read();  // writing data into char
      IncDataSerial += buffer;
        if (buffer == terminator) {
        getLF = true;
      }
    }
 
    Serial.print(IncDataSerial);  // send string ( char array ) to hardware serial
    Serial.print("\r");   // send a CR because it is missing
    IncDataSerial = "";
  }
  if (Serial.available()>0) // if data is available on hardwareserial port ==> data is comming from PC or notebook
    Serial1.write(Serial.read());  // write it to the GPRS shield
}

