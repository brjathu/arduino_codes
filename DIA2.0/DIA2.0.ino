#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <string.h>

#define PIN_TX    10
#define PIN_RX    11
#define BAUDRATE  4800
String phoneNum= "94770528082";
char phoneNumberChar[20];
char sendMsgChar[100];

#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;

char phone[16];
char datetime[24];

//Sensor readings
int temperature=27;
int moisture = 5;
int ph= 10;


//DIA parameters
bool DIA=false;
bool TEMP=false;
bool MOIS= false;

//setting parameters




GPRS gprsTest(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate

SoftwareSerial gprs = SoftwareSerial(10, 11);



void setup() {
  Serial.begin(9600);
//  gprs.begin(4800);

//  delay(1000);
  while(!gprsTest.init()) {
      delay(1000);
      Serial.print("init error\r\n");
  }  

  Serial.println("gprs init success");




char http_cmda[] = "AT+CGATT?\r\n";
Serial.println(gprsTest.send_custom(http_cmda,sizeof(http_cmda)-1));
  delay(100);


  // bearer settings
 char http_cmdaa[] = "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n";
 Serial.println(gprsTest.send_custom(http_cmdaa,sizeof(http_cmdaa)-1));
  delay(2000);
  
  // bearer settings
char http_cmdaaa[] = "AT+SAPBR=3,1,\"APN\",\"dialogbb\"\r\n";
Serial.println(gprsTest.send_custom(http_cmdaaa,sizeof(http_cmdaaa)-1));
  delay(2000);
  
  // bearer settings
char http_cmdaaaa[] = "AT+SAPBR=1,1\r\n";
Serial.println(gprsTest.send_custom(http_cmdaaaa,sizeof(http_cmdaaaa)-1));
  delay(2000);





  char http_cmd[] = "AT+HTTPINIT\r\n";
  Serial.println(gprsTest.send_custom(http_cmd,sizeof(http_cmd)-1));
  delay(1000);

    char http_cmd1[] = "AT+HTTPPARA=\"URL\",\"http://dia.mybluemix.net/dia.php?number=+94444\"\r\n";
  Serial.println(gprsTest.send_custom(http_cmd1,sizeof(http_cmd1)-1));
  delay(1000);

char http_cmd2[] = "AT+HTTPACTION=0\r\n";
  Serial.println(gprsTest.send_custom(http_cmd2,sizeof(http_cmd2)-1));
  delay(1000);
  
  Serial.println(":(");
//  readSMS();
//  enableGPRS();
//  
 // sendSMS(phoneNum,"DIA powered up!");

}

void loop() {
//  //nothing to do
//  String a;
//  String sendMsg;
//
//
//
//  //debug
//  temperature= random(10,50);;
//  moisture = random(1,10);
//  ph= random(1,14);
//  Serial.println(temperature);
//  
//  sendData_http();
//
//  
//  a=readSMS();
//  Serial.println(a);
//  filterMsg(a);
//  
//  if(DIA==true){
//    Serial.println("Sending back to user");
//    sendMsg="DIA:";
//    if(TEMP==true){
//      Serial.println("Sending temprature back to user " + String(temperature));
//      sendMsg+= "temperature:" +  String(temperature) +"  \n";
//    }
//    if(MOIS==true){
//      Serial.println("Sending moisture back to user " + String(moisture));
//      sendMsg +="moisture-" +  String(moisture)+"  \n";
//    }
//    sendSMS(phoneNum, sendMsg);
//  }
}




//String readSMS(){
//   messageIndex = gprsTest.isSMSunread();
//   if (messageIndex > 0) { //At least, there is one UNREAD SMS
//      Serial.println("Reading");
//      gprsTest.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);           
//      //In order not to full SIM Memory, is better to delete it
//      gprsTest.deleteSMS(messageIndex);
////      Serial.print("From number: ");
////      Serial.println(phone);  
////      Serial.print("Datetime: ");
////      Serial.println(datetime);        
////      Serial.print("Recieved Message: ");
////      Serial.println(message);    
//   }
//   else{
//       memset(message, 0, sizeof message);
//   }
//   return message;
//}


String readSMS(){
   gprs.println("AT+CMGL=\"REC UNREAD\",1");
   
   delay(1000);
   toSerial();
}


void toSerial()
{
  while(gprs.available()!=0)
  {
    Serial.write(gprs.read());
  }
}

void filterMsg(String msg){
  if(msg.substring(0, 3)=="DIA"){
        Serial.println("DIA initaiated");
        DIA=true;
        if(msg.substring(4, 7)=="TEM"){
            Serial.println("requesting Temprature");
            TEMP=true;
            MOIS=false;
        }
        else if(msg.substring(4, 7)=="MOS"){
            Serial.println("requesting moisture");
            MOIS=true;
            TEMP=false;
        }
        else if(msg.substring(4, 7)=="ALL"){
            Serial.println("requesting all sensor reading");
            MOIS=true;
            TEMP=true;
        }
        else{
            MOIS=false;
            TEMP=false;
        }
  }
  else{
    DIA=false;
  }
  
  
}


//void sendSMS(String phoneNum,String sendMsg){
//    
//    sendMsg.toCharArray(sendMsgChar, 100);
//    phoneNum.toCharArray(phoneNumberChar, 20);
//    gprsTest.sendSMS(phoneNumberChar,sendMsgChar);
//    Serial.println("Message sent to: "+ phoneNum + " , " + sendMsg);
//}

 void sendSMS(String phoneNum,String sendMsg)
{

  sendMsg.toCharArray(sendMsgChar, 100);
  phoneNum.toCharArray(phoneNumberChar, 20);
  gprs.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  gprs.println("AT+CMGS=\"+"+String(phoneNum)+"\"\r"); // Replace x with mobile number
  delay(1000);
  gprs.println(String(sendMsg));// The SMS text you want to send
  delay(100);
  gprs.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println("Message sent to: "+ phoneNum + " , " + sendMsg);
}



void sendData_http(){

  char buffer[5];         //the ASCII of the integer will be stored in this char array
  
  // initialize http service
   gprs.println("AT+HTTPINIT");
   delay(2000); 


   // set http param value
   gprs.println("AT+HTTPPARA=\"URL\",\"http://dia.mybluemix.net/dia.php?number="+phoneNum+"&temperature="+String(temperature)+"&moisture="+String(moisture)+"&ph="+String(ph)+"\"");
   delay(2000);


   // set http action type 0 = GET, 1 = POST, 2 = HEAD
   gprs.println("AT+HTTPACTION=0");
   delay(6000);

//
//   // read server response
//   gprsSerial.println("AT+HTTPREAD"); 
//   delay(1000);
//   toSerial();
//
//   gprsSerial.println("");
//   gprsSerial.println("AT+HTTPTERM");
//   toSerial();
//   delay(300);
//
//   gprsSerial.println("");
//   delay(10000);

  Serial.println("Data sent");
}



void enableGPRS(){

  Serial.println("Config SIM900...");

  delay(2000);
  Serial.println("Done!...");
//  gprs.flush();
//  Serial.flush();

  // attach or detach from GPRS service 
  gprs.println("AT+CGATT?");
  delay(100);


  // bearer settings
  gprs.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  
  // bearer settings
  gprs.println("AT+SAPBR=3,1,\"APN\",\"dialogbb\"");
  delay(2000);
  
  // bearer settings
  gprs.println("AT+SAPBR=1,1");
  delay(2000);


  Serial.println("GPRS configured!");
}


