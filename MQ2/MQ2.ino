/*
 * MQ2 -> Arduino
Vcc -> 5V
Gnd -> Gnd
AD -> A0

*/

void setup() { 
  Serial.begin(9600); 
}   
void loop() { 
  float vol; 
  int sensorValue = analogRead(A0); 
  vol=(float)sensorValue/1024*5.0; 
  Serial.println(vol,1); 
  delay(100);
}

