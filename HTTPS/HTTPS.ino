#include <SoftwareSerial.h>

#define PIN_RX    7
#define PIN_TX    8


String reading="{  \" : 1,  \"latitude\" : 12.983027,  \"longitude\" : 80.2594,  \"timestamp\" : 100,  }";
String out="";
String ok = "OK";
SoftwareSerial mySerial(PIN_RX, PIN_TX);

void setup() {

  Serial.begin(2400);
  delay(1000);
  mySerial.begin(2400);

  
//  while(!out.equals("OK")){
//    mySerial.print("AT+CSQ\r\n");
//    out = printFun();
//    out = out.substring(24,26);
//  }
//  out= "";

  safeAT("AT+CSQ\r\n","OK",24,26);

  
  mySerial.print("at+sapbr=3,1,\"CONTYPE\",\"GPRS\"\r\n");
  printFun();

  mySerial.print("at+sapbr=3,1,\"APN\",\"dialogbb\"\r\n");
  printFun();


  mySerial.print("at+sapbr=1,1\r\n");
  delay(5000);
  printFun();


  mySerial.print("at+sapbr=2,1\r\n");
  printFun();




  mySerial.print("at+httpinit\r\n");
  printFun();



  mySerial.print("AT+HTTPPARA=\"URL\",");
  printFun();
  mySerial.print("\"https://senzmate-test-hub.azure-devices.net/devices/");
  printFun();
  mySerial.print("test-1-device-1/messages/events?api-version=2016-11-14\"\r\n");
  printFun();


  mySerial.print("AT+HTTPPARA=\"CID\",1\r\n");
  printFun();


  mySerial.print("AT+HTTPPARA=\"REDIR\",1\r\n");
  printFun();


   mySerial.print("AT+HTTPPARA=\"USERDATA\",");
   printFun();
   mySerial.print("\"Authorization :SharedAccessSignature sr=");
   printFun();
   mySerial.print("senzmate-test-hub.azure-devices.net%");
   printFun();
   mySerial.print("2Fdevices%2Ftest-1-device-1");
   printFun();
   mySerial.print("&sig=");
   printFun();
   mySerial.print("wlW3vekFym1aicxjc0zdhgGd75FsLCGLEn%2Bk92eyF%2Bc%3D&");
   printFun();
   mySerial.print("se=1530605287\"\r\n");
   printFun();



  mySerial.print("at+httpssl=1\r\n");
  delay(2000);
  printFun();


  
    mySerial.print("at+httppara=\"CONTENT\",\"application/json\"\r\n");
    delay(1000);
    printFun();
   
  
  mySerial.print("AT+HTTPDATA=" + String(reading.length()) + ",100000\r\n");
  delay(100);
  printFun(); 
  
  mySerial.println(reading);
  delay(100);
  printFun(); 
  
  

  mySerial.print("at+httpaction=1\r\n");
  delay(5000);
  printFun();

  mySerial.print("at+httpread\r\n");
  printFun();

  Serial.println("DONE");

}

void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}

String printFun() {

  delay(2000);

  String response = "";
  while (mySerial.available() != 0) {
    response += char(mySerial.read());
  }
  Serial.println(response);
  Serial.println(String(response.length()));
  return response;
}


void safeAT(String AT, String Check, int start, int stopNum){
  out= "";
  while(!out.equals(Check)){
    mySerial.print(AT);
    out = printFun();
    out = out.substring(start,stopNum);
  }
  
}

