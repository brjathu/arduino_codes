/* rawSend.ino Example sketch for IRLib2
 *  Illustrates how to send a code Using raw timings which were captured
 *  from the "rawRecv.ino" sample sketch.  Load that sketch and
 *  capture the values. They will print in the serial monitor. Then you
 *  cut and paste that output into the appropriate section below.
 */
#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender

IRsendRaw mySender;


#define RAW_DATA_LEN 68
uint16_t rawData[RAW_DATA_LEN]={
  8550, 4626, 442, 678, 446, 678, 494, 1750, 
  442, 682, 494, 654, 466, 654, 470, 626, 
  494, 630, 494, 1750, 442, 1802, 442, 706, 
  418, 1802, 442, 1802, 446, 1802, 442, 1802, 
  442, 1806, 442, 1802, 442, 682, 438, 1806, 
  442, 678, 442, 1830, 490, 606, 442, 682, 
  442, 678, 442, 682, 438, 1826, 494, 630, 
  490, 1754, 518, 582, 514, 1754, 518, 1726, 
  518, 1730, 490, 1000};


String mcu;
  

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.println("\nSoftware serial test started");
}

void loop() {
  while (Serial1.available() > 0) {
    mcu = printFun();

    if (mcu.indexOf("/LED=AC1") != -1)  {
      mySender.send(rawData,RAW_DATA_LEN,36);
    }
  }

}



String printFun() {

  delay(100);

  String response = "";
  while (Serial1.available() != 0) {
    response += char(Serial1.read());
  }
  Serial.println(response);
  return response;
}
