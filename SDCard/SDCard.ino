///*
//  SD card basic file example
//
// This example shows how to create and destroy an SD card file
// The circuit:
// * SD card attached to SPI bus as follows:
// ** MOSI - pin 11
// ** MISO - pin 12
// ** CLK - pin 13
// ** CS - pin 4
//
// created   Nov 2010
// by David A. Mellis
// modified 9 Apr 2012
// by Tom Igoe
//
// This example code is in the public domain.
//
// */
//#include <SPI.h>
//#include <SD.h>
//
//File myFile;
//
//void setup() {
//  // Open serial communications and wait for port to open:
//  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
//
//
//  Serial.print("Initializing SD card...");
//
//  if (!SD.begin(4)) {
//    Serial.println("initialization failed!");
//    return;
//  }
//  Serial.println("initialization done.");
//
//  
//
//  // open a new file and immediately close it:
//  Serial.println("Creating example.txt...");
//  myFile = SD.open("example.txt", FILE_WRITE);
//  myFile.println("hey i am jathu");
//  myFile.close();
//pinMode(9,OUTPUT);
//
//}
//
//void loop() {
//  // nothing happens after setup finishes.
//  digitalWrite(9,HIGH);
//}
//
//











#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;

void setup(){
tmrpcm.speakerPin = 9;
Serial.begin(9600);
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;
}

tmrpcm.setVolume(2);
tmrpcm.play("rain.wav");
}

void loop(){  }

