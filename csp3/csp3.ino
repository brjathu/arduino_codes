void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(45,OUTPUT);
pinMode(47,OUTPUT);
pinMode(49,OUTPUT);



}

void loop() {
digitalWrite(45,HIGH); // put your main code here, to run repeatedly:
delay(10);
Serial.print(analogRead(54)+50);
Serial.print("\t");
digitalWrite(45,LOW);
delay(10);


digitalWrite(47,HIGH); // put your main code here, to run repeatedly:
delay(10);
Serial.print(analogRead(54));
Serial.print("\t");
digitalWrite(47,LOW);
delay(10);


digitalWrite(49,HIGH); // put your main code here, to run repeatedly:
delay(10);
Serial.print(analogRead(54)+40);
Serial.println("\t");
digitalWrite(49,LOW);
delay(100);

}
