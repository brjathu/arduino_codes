#define ENA 32
#define ENB 30

#define IN1 24
#define IN2 22
#define IN3 28
#define IN4 26

char junk;
String inputString="";
int speedBot=130;

void setup()
{
pinMode(ENA,OUTPUT);
pinMode(ENB,OUTPUT);

Serial3.begin(9600);
  
   delay(1000);
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
    if(inputString == "f"){         //in case of 'a' turn the LED on
      forward(speedBot);  
    }else if(inputString == "b"){   //incase of 'b' turn the LED off
      backward(speedBot);
    }else if(inputString == "l"){   //incase of 'b' turn the LED off
      left(speedBot);
    }else if(inputString == "r"){   //incase of 'b' turn the LED off
      right(speedBot);
    }else if(inputString == "s"){   //incase of 'b' turn the LED off
      botStop();
    }
    inputString = "";
  }

  


}




void forward(int speed){


analogWrite(ENA,speed);
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);

analogWrite(ENB,speed);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);

}


void backward(int speed){


analogWrite(ENA,speed);
digitalWrite(IN1,HIGH);
digitalWrite(IN2,LOW);

analogWrite(ENB,speed);
digitalWrite(IN3,HIGH);
digitalWrite(IN4,LOW);

}



void left(int speed){



analogWrite(ENA,0);
digitalWrite(IN1,LOW);
digitalWrite(IN2,LOW);

analogWrite(ENB,speed);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);

}


void right(int speed){



analogWrite(ENA, speed);
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);

analogWrite(ENB,0);
digitalWrite(IN3,LOW);
digitalWrite(IN4,LOW);

}

void botStop(){



analogWrite(ENA, 0);
digitalWrite(IN1,HIGH);
digitalWrite(IN2,HIGH);

analogWrite(ENB,0);
digitalWrite(IN3,HIGH);
digitalWrite(IN4,HIGH);

}

 

