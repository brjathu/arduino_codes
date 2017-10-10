#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define red 9
#define green 10
#define blue 11
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


analogWrite(green,255);
//analogWrite(blue,200);

  


}




