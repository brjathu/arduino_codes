#define DEBUG true


void setup() {
  // put your setup code here, to run once:
Serial3.begin(115200);
Serial.begin(9600);
pinMode(15, INPUT_PULLUP);
pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
if(Serial3.available()){
  if(Serial3.find("+IPD,"))
    {
     delay(1000); // wait for the serial buffer to fill up (read all the serial data)
     // get the connection id so that we can then disconnect
     int connectionId = Serial3.read()-48; // subtract 48 because the read() function returns 
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48
          
     Serial3.find("pin="); // advance cursor to "pin="
     
     int pinNumber = (Serial3.read()-48)*10; // get first number i.e. if the pin 13 then the 1st number is 1, then multiply to get 10
     pinNumber += (Serial3.read()-48); // get second number, i.e. if the pin number is 13 then the 2nd number is 3, then add to the first number
     
     digitalWrite(pinNumber, !digitalRead(pinNumber)); // toggle pin    
     
     // make close command
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     
     sendData(closeCommand,1000,DEBUG); // close connection
    }
  }
}






String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    Serial3.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(Serial3.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = Serial3.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}
