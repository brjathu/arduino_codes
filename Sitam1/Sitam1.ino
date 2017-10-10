#define NUM_SENSORS   6     // number of sensors used
#define TIMEOUT       2000 // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2


#define in1 8
#define in2 12
#define in3 10
#define in4 13
#define ENA 11
#define ENB 9


int pins[NUM_SENSORS]={56,57,58,59,60,61};
unsigned int sensorValues[NUM_SENSORS];
unsigned int sensorVal[NUM_SENSORS];
int position; 
int pos=0;
int c=0;


float kp ;
float kd ;
int error = 0;
int intigral = 0;
int lastError = 0;
int proportional = 0;
int derivative = 0;


void setup()
{
pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT);
pinMode(in3,OUTPUT);
pinMode(in4,OUTPUT);
pinMode(ENA,OUTPUT);
pinMode(ENB,OUTPUT);


Serial.begin(9600);

}

void loop(){
digitalWrite(in1,HIGH);
digitalWrite(in2,LOW);
digitalWrite(in3,HIGH);
digitalWrite(in4,LOW);
pid();
Serial.println(error);
//analogWrite( ENB,0);
//analogWrite(  ENA,255);

//pid();
//Serial.println(error);
//
analogWrite( ENB,120-constrain(0.08*error,-70,70));
analogWrite(  ENA,120+constrain(0.08*error,-70,70));

}


void pid(){
  for(int i=0;i<NUM_SENSORS;i++){
    sensorValues[i]=analogRead(pins[i]);
//    Serial.print(sensorValues[i]);
//    Serial.print("\t");
  }
//  Serial.print("\n");
  pos=0;
  c=0;
  for(int i=0;i<NUM_SENSORS;i++){
    if(sensorValues[i]<350){
      pos+=i*1000;
      c++;
    }
  }
  if(c!=0){
  error= 2500-(pos/c);
  }
  else{
    error=0;
  }
  
}




