String reading="{  \" : 1,  \"latitude\" : 12.983027,  \"longitude\" : 80.2594,  \"timestamp\" : 100,  }";
String out="";
String ok = "OK";
int done = 0;

void setup() {
  power();
  delay(1000);
  Serial.begin(4800);

 

  safeAT("AT+CSQ\r\n","OK",24,26,100);
 
  safeAT("at+sapbr=3,1,\"CONTYPE\",\"GPRS\"\r\n","OK",33,35,100);

  safeAT("at+sapbr=3,1,\"APN\",\"dialogbb\"\r\n","OK",33,35,100);

  sendAT("at+sapbr=2,1\r\n","0.0.0.0",29,36,100);
//  Serial.println(done);
  if(done==1){
    safeAT("at+sapbr=1,1\r\n","OK",16,18,10000);
  }

 
  Serial.print("at+sapbr=2,1\r\n");
  printFun();

  Serial.print("at+httpinit\r\n");
  printFun();



  

}

void loop() {

  done = 0;
  while(done!=1){
    sendAT("AT+HTTPPARA=\"URL\"," , "AT" , 0 , 2 ,100);
    sendAT("\"https://senzmate-test-hub.azure-devices.net/devices/" , "\"h" , 0 , 2 ,100);
    sendAT("test-1-device-1/messages/events?api-version=2016-11-14\"\r\n" , "OK" , 59 , 61 ,100);
  }

  safeAT("AT+HTTPPARA=\"CID\",1\r\n","OK",23,25,100);

  safeAT("AT+HTTPPARA=\"REDIR\",1\r\n","OK",25,27,100);


  done = 0;
  while(done!=1){
    sendAT("AT+HTTPPARA=\"USERDATA\"," , "AT" , 0 , 2 ,100);
    sendAT("\"Authorization :SharedAccessSignature sr=" , "\"A" , 0 , 2 ,100);
    sendAT("senzmate-test-hub.azure-devices.net%" , "se" , 0 , 2 ,100);
    sendAT("2Fdevices%2Ftest-1-device-1" , "2F" , 0 , 2 ,100);
    sendAT("&sig=" , "&s" , 0 , 2 ,100);
    sendAT("wlW3vekFym1aicxjc0zdhgGd75FsLCGLEn%2Bk92eyF%2Bc%3D&" , "wl" , 0 , 2 ,100);
    sendAT("se=1530605287\"\r\n" , "OK" ,18 , 20 ,100);
  }

  safeAT("at+httpssl=1\r\n","OK",16,18,100);

  safeAT("at+httppara=\"CONTENT\",\"application/json\"\r\n","OK",44,46,100);

  done = 0;
  while(done!=1){
    sendAT("AT+HTTPDATA=" + String(reading.length()) + ",10000\r\n" , "AT" , 0 , 2 ,100);
    sendAT(reading, "OK" , 2 , 4 ,100);
  }

  safeAT("at+httpaction=1\r\n","204",39,42,10000);

  safeAT("at+httpread\r\n","OK",15,17,100);

//  Serial.println("DONE");

  delay(30000);
}

String printFun() {

  delay(2000);


  String response = "";
  while (Serial.available() != 0) {
    response += char(Serial.read());
  }
//  Serial.println(response);
//  Serial.println(String(response.length()));
  return response;
}


void safeAT(String AT, String Check, int start, int stopNum, int de){
  out= "";
  done = 0;
  while(!out.equals(Check)){
    Serial.print(AT);
    delay(de);
    out = printFun();
//    Serial.println(out.substring(start,stopNum));
    out = out.substring(start,stopNum);
    done = 1;
  }
  
}



void sendAT(String AT, String Check, int start, int stopNum, int de){
  out= "";
  done = 0;
    Serial.print(AT);
    delay(de);
    out = printFun();
//    Serial.println(out.substring(start,stopNum));
    out = out.substring(start,stopNum);
//    Serial.println(out.equals(Check));
    if(out.equals(Check)){
      done = 1;
    }
  
}



void power()
// software equivalent of pressing the GSM shield "power" button
{
  pinMode(15,OUTPUT);
  digitalWrite(15, HIGH);
  delay(1000);
  digitalWrite(15, LOW);
  delay(5000);
}



