#include <QTRSensors.h>
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>




#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

#define ENA 10
#define ENB 11

#define IN1 A0
#define IN2 A1
#define IN3 A2
#define IN4 A3




// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6, 7, 8, 9, 12},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);
   
unsigned int sensorValues[NUM_SENSORS];
double positionX; 

int error = 0;
int lastError = 0;
int proportional = 0;
int derivative = 0;

double val4=0;
int lmSpeed=70;
int rmSpeed=70;
double pidSpeed=0;

int maxVal=100;


byte ATuneModeRemember=2;
double setpoint=3500;
double kp=2,ki=0.5,kd=2;

double kpmodel=1.5, taup=100, theta[50];
double outputStart=5;
double aTuneStep=50, aTuneNoise=1, aTuneStartValue=100;
unsigned int aTuneLookBack=20;

boolean tuning = false;
unsigned long  modelTime, serialTime;



PID myPID(&positionX, &pidSpeed, &setpoint,kp,ki,kd, DIRECT);
PID_ATune aTune(&positionX, &pidSpeed);

//set to false to connect to the real world
boolean useSimulation = true;

void setup() {
  pinMode(ENA,OUTPUT);
pinMode(ENB,OUTPUT);

Serial.begin(9600);
  
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
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
  Serial.println();
  Serial.println();
  delay(1000);


   if(useSimulation)
  {
    for(byte i=0;i<50;i++)
    {
      theta[i]=outputStart;
    }
    modelTime = 0;
  }
  //Setup the pid 
  myPID.SetMode(AUTOMATIC);

  if(tuning)
  {
    tuning=false;
    changeAutoTune();
    tuning=true;
  }
  
  serialTime = 0;

}

void loop() {
  unsigned long now = millis();
  if(!useSimulation)
  {
  positionX = qtrrc.readLine(sensorValues);
  }


  if(tuning)
  {
    byte val = (aTune.Runtime());
    if (val!=0)
    {
      tuning = false;
    }
    if(!tuning)
    { //we're done, set the tuning parameters
      kp = aTune.GetKp();
      ki = aTune.GetKi();
      kd = aTune.GetKd();
      myPID.SetTunings(kp,ki,kd);
      AutoTuneHelper(false);
    }
  }
  else myPID.Compute();
  
  if(useSimulation)
  {
    theta[30]=pidSpeed;
    if(now>=modelTime)
    {
      modelTime +=100; 
      DoModel();
    }
  }
  else
  {
      lmSpeed+=pidSpeed;
  rmSpeed-=pidSpeed;
  forward(lmSpeed,rmSpeed);
  delay(50);
  }
  
  //send-receive with processing if it's time
  if(millis()>serialTime)
  {
    SerialReceive();
    SerialSend();
    serialTime+=500;
  }

  

  
}




void forward(int speedL,int speedR){


analogWrite(ENA,speedL);
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);

analogWrite(ENB,speedR);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);

}




void changeAutoTune()
{
 if(!tuning)
  {
    //Set the output to the desired starting frequency.
    pidSpeed=aTuneStartValue;
    aTune.SetNoiseBand(aTuneNoise);
    aTune.SetOutputStep(aTuneStep);
    aTune.SetLookbackSec((int)aTuneLookBack);
    AutoTuneHelper(true);
    tuning = true;
  }
  else
  { //cancel autotune
    aTune.Cancel();
    tuning = false;
    AutoTuneHelper(false);
  }
}

void AutoTuneHelper(boolean start)
{
  if(start)
    ATuneModeRemember = myPID.GetMode();
  else
    myPID.SetMode(ATuneModeRemember);
}


void SerialSend()
{
  Serial.print("setpoint: ");Serial.print(setpoint); Serial.print(" ");
  Serial.print("input: ");Serial.print(positionX); Serial.print(" ");
  Serial.print("output: ");Serial.print(pidSpeed); Serial.print(" ");
  if(tuning){
    Serial.println("tuning mode");
  } else {
    Serial.print("kp: ");Serial.print(myPID.GetKp());Serial.print(" ");
    Serial.print("ki: ");Serial.print(myPID.GetKi());Serial.print(" ");
    Serial.print("kd: ");Serial.print(myPID.GetKd());Serial.println();
  }
}

void SerialReceive()
{
  if(Serial.available())
  {
   char b = Serial.read(); 
   Serial.flush(); 
   if((b=='1' && !tuning) || (b!='1' && tuning))changeAutoTune();
  }
}

void DoModel()
{
  //cycle the dead time
  for(byte i=0;i<49;i++)
  {
    theta[i] = theta[i+1];
  }
  //compute the input
  positionX = (kpmodel / taup) *(theta[0]-outputStart) + positionX*(1-1/taup) + ((float)random(-10,10))/100;

}
