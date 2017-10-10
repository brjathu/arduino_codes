//3,5,6,9
int x=255;

void setup() {
  // put your setup code here, to run once:
pinMode(3,OUTPUT);
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
pinMode(9,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
analogWrite(3,x);
analogWrite(5,x);
analogWrite(6,x);
analogWrite(9,x);

delay(6000);

x=0;


analogWrite(3,x);
analogWrite(5,x);
analogWrite(6,x);
analogWrite(9,x);
}
