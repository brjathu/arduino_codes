#include <QTRSensors.h>
#include <Servo.h>
#include <SharpIR.h> 

Servo servo;


#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2000 // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

#define model 20150    //SharpIR model no

//left
#define L 11  //left
#define Lb 12  //back
#define Lf 13   //fow

//right
#define R 10  //right
#define Rb 9 //back
#define Rf 8  //fow

#define up 26
#define down 27
#define lift_enbl 3

int potPinp = A5;
int potPini = A6;
int potPind = A7;
float v_kp = 0;       // variable to store the value coming from the sensor
float v_kd = 0;
float v_ki = 0;

SharpIR Sharp(A0, 25, 93, model);    /*if any error change the mode;*/
// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) { 32, 34, 36, 38, 40, 42, 44, 46},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);
   
unsigned int sensorValues[NUM_SENSORS];
unsigned int sensorVal[NUM_SENSORS];
unsigned int position; 




float kp ;
float kd ;
float ki ;
int error = 0;
int intigral = 0;
int lastError = 0;
int proportional = 0;
int derivative = 0;

int lmSpeed=70;
int rmSpeed=70;
int baseSpeedR=150;
int baseSpeedL=150;
float pidSpeed=0;

int maxSpeed=200;



float frontDis=6;
int loc=0;// 

void setup()
{
servo.attach(4);
//pinMode(potPinp,INPUT);
//pinMode(potPini,INPUT);
//pinMode(potPind, INPUT);


pinMode(L,OUTPUT);
pinMode(R,OUTPUT);

servo.write(140);


Serial.begin(9600);
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  Serial.print( "Calibrate");
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
      // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  //Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  //Serial.println();
  //Serial.println();
  delay(1000);
}

int a=0;

void loop(){




        getDis();     /* Check the distance is correct - NOT CHECKED */
        while(frontDis>20){
          getDis();
              if (loc==0 || loc==1 || loc==4 || loc==6 || loc==8 || loc==10){
              goForward();
              
             }
          
        }

        if (loc==0){
          stopBot();
          delay(1500);
          ringCatch();
          liftUp();
          loc=1;
          delay(1000);

          analogWrite(R,0);
          digitalWrite(Rb,LOW);
          digitalWrite(Rf,LOW);
          
          analogWrite(L,100);
          digitalWrite(Lb,LOW);
          digitalWrite(Lf,HIGH);
          delay(100);

          stopBot();
        }

        else if (loc==1){
          stopBot();
          ringDrop();
          //turnRight(50); //50 is the delay time.180 turn
//          while("find a junction"){
//            goForward();
//          }
//          //turn 60 or something  right
//         loc=4;
        }



//  if (a==0){
//    ringCatch();
//    liftUp();
//    a=1;
//    delay(2500);
//    liftDown();
//  }

    
//        //getDis();     /* Check the distance is correct - NOT CHECKED */
//        while(frontDis>5){
//              if (loc==0 || loc==1 || loc==4 || loc==6 || loc==8 || loc==10){
//              goForward();
//              //getDis();
//             }
//          
//        }
//
//        if (loc==0){
//          ringCatch();
//          liftUp();
//          loc=1;
//        }
//
//        else if (loc==1){
//          ringDrop();
//          turnRight(50); //50 is the delay time.180 turn
//          while("find a junction"){
//            goForward();
//          }
//          //turn 60 or something  right
//         loc=4;
//        }
//
//        else if (loc==4){
//          //get the ring
//          //turn 180
//          while("find a junction"){
//            goForward();
//          }
//          //turn to the distination
//          loc=6;
//        }
//
//        else if (loc==6){
//          //drop the ring
//          //turn 180
//          while("find a junction"){
//            goForward();
//          }
//          //turn 60 or something left 
//          loc=8;
//        }
//
//        else if (loc==8){
//          //get the ring
//          //turn 180
//          while("find a junction"){
//            goForward();
//          }
//          //turn to the distination
//          loc=10;
//        }
//
//
//        else if (loc==10){
//          //drop the ring
//        stopBot();
//        }
//        
delay(50);
}

void getDis(){
  frontDis=Sharp.distance(); /// get the distance from sharp ir
}

void ringDrop(){
//  analogWrite(3,100);
//  digitalWrite(up,LOW);
//  digitalWrite(down,HIGH);
//  delay(200);
  servo.write(140);//change the angle.
  delay(50);
}
void ringCatch(){
  servo.write(108);
  delay(50);
}
void liftUp(){
  analogWrite(lift_enbl,200);
  digitalWrite(up,HIGH);
  digitalWrite(down,LOW);
  delay(4000);
  analogWrite(lift_enbl,0);
    digitalWrite(up,LOW);
  digitalWrite(down,LOW);
}

void liftDown(){
  analogWrite(lift_enbl,100);
  digitalWrite(down,HIGH);
  digitalWrite(up,LOW);
  delay(1650);
  analogWrite(lift_enbl,0);
  digitalWrite(down,LOW);
  digitalWrite(up,LOW);
}


void goForward(){
//   v_kp = analogRead(potPinp);
//  v_ki = analogRead(potPini);
//  v_kd = analogRead(potPind);
  //Serial.println(val);
  //delay(1);
//  kp=v_kp/11;
//  kd=v_kd/880;
//  ki=v_ki/880;
  kp = 10;
  kd = 1;
  ki =0;
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  unsigned int position = qtrrc.readLine(sensorValues);
  delay(5);
  //Serial.println("sensorValues");
  for (unsigned char i = 0; i < NUM_SENSORS; i++){
    sensorVal[i]=1000-sensorValues[7-i];

  }
  
  Error();
  derivative =-error+lastError;
  lastError=error;
  intigral+=error;
  pidSpeed=kp*error+kd*derivative+ki*intigral;
  pidSpeed=pidSpeed/1000;
  lmSpeed=baseSpeedL+pidSpeed*90/120;
  rmSpeed=baseSpeedR-pidSpeed;

  
  
  
  
  if(lmSpeed<0){
    lmSpeed=0;
  }
  else if(lmSpeed>maxSpeed){
    lmSpeed=maxSpeed;
  }

  if(rmSpeed<0){
    rmSpeed=0;
  }
  else if(rmSpeed>maxSpeed){
    rmSpeed=maxSpeed;
  }
  
  forward(lmSpeed,rmSpeed);

}

void Error(){
  error=0;
  for (int i = 0; i < 4; i++){
    error-=sensorVal[i];
  }
  for (int i = 4; i < 8; i++){
    error+=sensorVal[i];
  }
  }


void forward(int speedL,int speedR){
 
analogWrite(R,speedR);
digitalWrite(Rb,LOW);
digitalWrite(Rf,HIGH);

analogWrite(L,speedL);
digitalWrite(Lb,LOW);
digitalWrite(Lf,HIGH);

}

void stopBot(){
    digitalWrite(R,LOW);
    digitalWrite(Rb,LOW);
    digitalWrite(Rf,LOW);
    
    digitalWrite(L,LOW);
    digitalWrite(Lb,LOW);
    digitalWrite(Lf,LOW);
  
}


void turnLeft(int angle){

while("senosr 1/2 ==1"){
  analogWrite(R,40);
  analogWrite(L,0);
  
digitalWrite(Rb,LOW);
digitalWrite(Rf,HIGH);


digitalWrite(Lb,LOW);
digitalWrite(Lf,LOW);
}
}

void turnRight(int angle){

while("senosr 7/8 ==1"){
analogWrite(R,0);


digitalWrite(Rb,LOW);
digitalWrite(Rf,LOW);

analogWrite(L,40);
digitalWrite(Lb,LOW);
digitalWrite(Lf,HIGH);

}

  
}
