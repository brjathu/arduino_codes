#include  <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int SW1 = 7;
int led=8;

void setup(void){
 Serial.begin(9600);
 radio.begin();
 radio.openWritingPipe(pipe);}

void loop(void){
 if (digitalRead(SW1) == HIGH){
  digitalWrite(led,HIGH);
  msg[0] = 111;
  radio.write(msg, 1);
  Serial.println("pressed");
  }
  else{
    digitalWrite(led,LOW);
  }
 
 }
