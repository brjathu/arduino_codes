#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9,10);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

typedef enum { role_ping_out = 1, role_pong_back } role_e;

const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

role_e role;

const int ledPin= 4;
const int threshold= 5;
float val=0;
int i=0;

void setup(void)
{
  role = role_ping_out;

  Serial.begin(19200);
  printf_begin();
  printf("ROLE: %s\n\r",role_friendly_name[role]);

  radio.begin();

  radio.setRetries(15,15);

  radio.setPayloadSize(8);

    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);

   radio.startListening();

  pinMode(ledPin, OUTPUT);
  analogReference(INTERNAL);
}

void loop(void)
{

  val=analogRead(A0);
  Serial.println(val);
  if (val >= threshold)
  {
  radio.stopListening();
  digitalWrite(ledPin, HIGH);
  
    printf("Now sending command:\n");
    unsigned long data=10;
    bool ok = radio.write( &data, sizeof(unsigned long) );
    
        if (ok)
        printf("ok...\n");
        else
        printf("failed.\n\r");

      radio.startListening();

  delay(5000);
  digitalWrite(ledPin, LOW);
  }
  else
  digitalWrite(ledPin, LOW);


    // Try again 1s later
    delay(10);
 
}
  
// vim:cin:ai:sts=2 sw=2 ft=cpp
