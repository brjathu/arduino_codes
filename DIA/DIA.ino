#include <GPRS_Shield_Arduino.h>
#include <UbidotsArduinoGPRS.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <string.h>

#define PIN_TX    10
#define PIN_RX    11
#define BAUDRATE  4800
String phoneNum= "+94770528082";
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


//MQTT parameters
#define APN "dialogbb" 
#define USER ""  // If your apn doesnt have username just put ""
#define PASS ""  // If your apn doesnt have password just put ""
#define TOKEN "QTZNC884yi5DEPbmpWtj3W44dmGdZn"  // Replace it with your Ubidots token
#define MQTT_TEMP "temperature" // Assign the variable label 
#define MQTT_MOIS "moisture" // Assign the variable label 



GPRS gprsTest(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate

Ubidots client(TOKEN);
SoftwareSerial gprs = SoftwareSerial(10, 11);
SoftwareSerial *GPRSSerial = &gprs;


void setup() {
  Serial.begin(9600);
  while(!gprsTest.init()) {
      delay(1000);
      Serial.print("init error\r\n");
  }  
  Serial.println("gprs init success");

  GPRSSerial->begin(4800);
  if (! client.init(*GPRSSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  while (!client.setApn(APN,USER,PASS));

  
  sendSMS(phoneNum,"DIA powered up!");
}

void loop() {
  //nothing to do
  String a;
  String sendMsg;
  
  a=readSMS();
  Serial.println(a);
  sendData_http();
  filterMsg(a);
  if(DIA==true){
    Serial.println("Sending back to user");
    sendMsg="DIA:";
    if(TEMP==true){
      Serial.println("Sending temprature back to user " + String(temperature));
      sendMsg+= "temperature:" +  String(temperature) +"  \n";
    }
    if(MOIS==true){
      Serial.println("Sending moisture back to user " + String(moisture));
      sendMsg +="moisture-" +  String(moisture)+"  \n";
    }
    sendSMS(phoneNum, sendMsg);
  }

  //mqttPublish();
  

}



String readSMS(){
   messageIndex = gprsTest.isSMSunread();
   if (messageIndex > 0) { //At least, there is one UNREAD SMS
      Serial.println("Reading");
      gprsTest.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);           
      //In order not to full SIM Memory, is better to delete it
      gprsTest.deleteSMS(messageIndex);
//      Serial.print("From number: ");
//      Serial.println(phone);  
//      Serial.print("Datetime: ");
//      Serial.println(datetime);        
//      Serial.print("Recieved Message: ");
//      Serial.println(message);    
   }
   else{
       memset(message, 0, sizeof message);
   }
   return message;
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


void sendSMS(String phoneNum,String sendMsg){
    
    sendMsg.toCharArray(sendMsgChar, 100);
    phoneNum.toCharArray(phoneNumberChar, 20);
    gprsTest.sendSMS(phoneNumberChar,sendMsgChar);
    Serial.println("Message sent to: "+ phoneNum + " , " + sendMsg);
}

//  gprsTest.sendSMS(PHONE_NUMBER,MESSAGE);

void mqttPublish(){
  client.add(MQTT_TEMP, temperature);
  client.add(MQTT_MOIS, moisture);
  client.sendAll(); 
}


void sendData_http(){



  // initialize http service
   gprs.println("AT+HTTPINIT");
   delay(2000); 


   // set http param value
   gprs.println("AT+HTTPPARA=\"URL\",\"http://dia.mybluemix.net/dia.php?temp=56\"");
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
}

