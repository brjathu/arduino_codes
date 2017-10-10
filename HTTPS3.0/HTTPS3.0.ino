String reading="{  \" : 1,  \"latitude\" : 12.983027,  \"longitude\" : 80.2594,  \"timestamp\" : 100,  }";
String out="";
String ok = "OK";
int done = 0;

void setup() {

  Serial.begin(19200);
  delay(1000);
  mySerial.begin(19200);

 

  safeAT("AT+CSQ\r\n","OK",23,25,100);
 
  safeAT("at+sapbr=3,1,\"CONTYPE\",\"GPRS\"\r\n","OK",32,34,100);

  safeAT("at+sapbr=3,1,\"APN\",\"dialogbb\"\r\n","OK",32,34,100);

  sendAT("at+sapbr=2,1\r\n","0.0.0.0",28,35,100);
  Serial.println(done);
  if(done==1){
    safeAT("at+sapbr=1,1\r\n","OK",15,17,10000);
  }

 
  mySerial.print("at+sapbr=2,1\r\n");
  printFun();

  mySerial.print("at+httpinit\r\n");
  printFun();



  

}

void loop() {
//  if (mySerial.available()) {
//    Serial.write(mySerial.read());
//  }
//  if (Serial.available()) {
//    mySerial.write(Serial.read());
//  }

  done = 0;
  while(done!=1){
    sendAT("AT+HTTPPARA=\"URL\"," , "AT" , 0 , 2 ,100);
    sendAT("\"https://senzmate-test-hub.azure-devices.net/devices/" , "\"h" , 0 , 2 ,100);
    sendAT("test-1-device-1/messages/events?api-version=2016-11-14\"\r\n" , "OK" , 58 , 60 ,100);
  }

  safeAT("AT+HTTPPARA=\"CID\",1\r\n","OK",22,24,100);

  safeAT("AT+HTTPPARA=\"REDIR\",1\r\n","OK",24,26,100);


  done = 0;
  while(done!=1){
    sendAT("AT+HTTPPARA=\"USERDATA\"," , "AT" , 0 , 2 ,100);
    sendAT("\"Authorization :SharedAccessSignature sr=" , "\"A" , 0 , 2 ,100);
    sendAT("senzmate-test-hub.azure-devices.net%" , "se" , 0 , 2 ,100);
    sendAT("2Fdevices%2Ftest-1-device-1" , "2F" , 0 , 2 ,100);
    sendAT("&sig=" , "&s" , 0 , 2 ,100);
    sendAT("wlW3vekFym1aicxjc0zdhgGd75FsLCGLEn%2Bk92eyF%2Bc%3D&" , "wl" , 0 , 2 ,100);
    sendAT("se=1530605287\"\r\n" , "OK" ,17 , 19 ,100);
  }

  safeAT("at+httpssl=1\r\n","OK",15,17,100);

  safeAT("at+httppara=\"CONTENT\",\"application/json\"\r\n","OK",43,45,100);

  done = 0;
  while(done!=1){
    sendAT("AT+HTTPDATA=" + String(reading.length()) + ",10000\r\n" , "AT" , 0 , 2 ,100);
    sendAT(reading, "OK" , 2 , 4 ,100);
  }

  safeAT("at+httpaction=1\r\n","204",39,42,10000);

  safeAT("at+httpread\r\n","OK",14,16,100);

  Serial.println("DONE");

  delay(30000);
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


void safeAT(String AT, String Check, int start, int stopNum, int de){
  out= "";
  done = 0;
  while(!out.equals(Check)){
    mySerial.print(AT);
    delay(de);
    out = printFun();
    Serial.println(out.substring(start,stopNum));
    out = out.substring(start,stopNum);
    done = 1;
  }
  
}



void sendAT(String AT, String Check, int start, int stopNum, int de){
  out= "";
  done = 0;
    mySerial.print(AT);
    delay(de);
    out = printFun();
    Serial.println(out.substring(start,stopNum));
    out = out.substring(start,stopNum);
    Serial.println(out.equals(Check));
    if(out.equals(Check)){
      done = 1;
    }
  
}

