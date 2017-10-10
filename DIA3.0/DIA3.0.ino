#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <string.h>

#define PIN_TX    10
#define PIN_RX    11
#define BAUDRATE  4800
String phoneNum= "94770528082";

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
bool AUTH= true;
//setting parameters




GPRS gprsTest(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate

void setup() {
  Serial.begin(9600);

//  delay(1000);
  while(!gprsTest.init()) {
      delay(1000);
      Serial.print("init error\r\n");
  }  

  Serial.println("gprs init success");
  
  enableGPRS();

  //sendSMS(phoneNum,"DIA powered up!");

}

void loop() {
  //nothing to do
  String a;
  String sendMsg;



  //debug
  updateSensor();
  Serial.println(temperature);
  
  sendData_http();

  
  a=readSMS();
  Serial.println(a);
  filterMsg(a);
  if(AUTH==true){
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
  }
  else{
    
  }
}




String readSMS(){
   messageIndex = gprsTest.isSMSunread();
   if (messageIndex > 0) { //At least, there is one UNREAD SMS
      Serial.println("Reading");
      gprsTest.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);           
      //In order not to full SIM Memory, is better to delete it
      gprsTest.deleteSMS(messageIndex);
      Serial.print("From number: ");
      Serial.println(phone);  
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

    char sendMsgChar[100];
    char phoneNumberChar[20];
    sendMsg.toCharArray(sendMsgChar, 100);
    phoneNum.toCharArray(phoneNumberChar, 20);
    gprsTest.sendSMS(phoneNumberChar,sendMsgChar);
    Serial.println("Message sent to: "+ phoneNum + " , " + sendMsg);
}



void sendData_http(){
  char buffer[5];         //the ASCII of the integer will be stored in this char array
   send_AT_commands("AT+HTTPINIT\r\n");
   delay(2000); 
   send_AT_commands("AT+HTTPPARA=\"URL\",\"http://dia.mybluemix.net/dia.php?number="+String(phoneNum)+"&temperature="+String(temperature)+"&moisture="+String(moisture)+"&ph="+String(ph)+"\"\r\n");
   delay(2000);
   send_AT_commands("AT+HTTPACTION=0\r\n");
   delay(6000);

  Serial.println("Data sent");
}


void enableGPRS(){
  send_AT_commands("AT+CGATT?\r\n");
  delay(100);
  send_AT_commands("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
  delay(2000);
  send_AT_commands("AT+SAPBR=3,1,\"APN\",\"dialogbb\"\r\n");
  delay(2000);
  send_AT_commands("AT+SAPBR=1,1\r\n");
  delay(2000);
  Serial.println("GPRS configured!");
}


void send_AT_commands(String at){
  char at_cmd[200];
  at.toCharArray(at_cmd, 200);
  gprsTest.send_custom(at_cmd,sizeof(at_cmd)-1);
}


void updateSensor(){
  temperature= random(10,50);;
  moisture = random(1,10);
  ph= random(1,14);
}

