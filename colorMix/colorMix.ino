 #define S0 4
#define S1 5
#define S2 6
#define S3 7
#define red 8
#define green 10
#define blue 9
#define sensorOut 8
int frequency = 0;



char junk;
String inputString="";

String str;
void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  
  pinMode(sensorOut, INPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  inputString.reserve(200);
  Serial.begin(9600);
}
void loop() {
//
//analogWrite(green,255);
if(Serial.available()){
  while(Serial.available())
    {
      char inChar = (char)Serial.read(); //read the input
      inputString += inChar;        //make a string of the characters coming on serial
    }
    Serial.println(inputString);
    while (Serial.available() > 0)  
    { junk = Serial.read() ; }      // clear the serial buffer
    if(inputString == "r"){         //in case of 'a' turn the LED on
       analogWrite(red,255);
          analogWrite(green,0);
          analogWrite(blue,0);
    }else if(inputString == "g"){   //incase of 'b' turn the LED roff
      analogWrite(red,0);
          analogWrite(green,255);
          analogWrite(blue,0);
    }else if(inputString == "b"){   //incase of 'b' turn the LED off
      analogWrite(red,0);
          analogWrite(green,0);
          analogWrite(blue,255);
    }else if(inputString == "v"){   //incase of 'b' turn the LED off
          analogWrite(red,255);
          analogWrite(green,0);
          analogWrite(blue,255);
//        }
    }
    else if(inputString == "y"){   //incase of 'b' turn the LED off
          analogWrite(red,255);
          analogWrite(green,255);
          analogWrite(blue,255);
//        }
    }

    else if(inputString == "w"){   //incase of 'b' turn the LED off
          analogWrite(red,255);
          analogWrite(green,255);
          analogWrite(blue,255);
//        }
    }
    inputString = "";
//  }

  


}

}



