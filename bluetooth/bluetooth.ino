/*
Arduino Turn LED On/Off using Serial Commands
Created April 22, 2015
Hammad Tariq, Incubator (Pakistan)

It's a simple sketch which waits for a character on serial
and in case of a desirable character, it turns an LED on/off.

Possible string values:
a (to turn the LED on)
b (tor turn the LED off)
*/

char junk;
String inputString="";

void setup()                    // run once, when the sketch starts
{
 Serial3.begin(9600);            // set the baud rate to 9600, same should be of your Serial Monitor
 pinMode(13, OUTPUT);
}

void loop()
{
  if(Serial3.available()){
  while(Serial3.available())
    {
      char inChar = (char)Serial3.read(); //read the input
      inputString += inChar;        //make a string of the characters coming on serial
    }
    Serial3.println(inputString);
    while (Serial3.available() > 0)  
    { junk = Serial3.read() ; }      // clear the serial buffer
    if(inputString == "a"){         //in case of 'a' turn the LED on
      digitalWrite(13, HIGH);  
    }else if(inputString == "b"){   //incase of 'b' turn the LED off
      digitalWrite(13, LOW);
    }
    inputString = "";
  }
}
