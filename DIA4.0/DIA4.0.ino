#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <string.h>
#include <EEPROM.h>
//#include <MD5.h>

#define PIN_TX    7
#define PIN_RX    8
#define BAUDRATE  4800
char phoneNum[]= "+94770528082";
char serverNum[]="Anywheresms";

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
bool DIA=true;
bool SENSOR=false;
bool AUTH= false;
bool SET=false;
bool SEVER_CMD=false;
bool SOL=false;
//setting parameters


GPRS gprsTest(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate

void setup() {
 
  Serial.begin(9600);
  delay(1000);

//  //error test
//  EEPROM_write(phoneNum,13);
//  delay(1000);
  
//  updatePhoneNumber();
//  delay(1000);
  
//  while(!gprsTest.init()) {
//      Serial.print("init error\r\n");
//      SIM900power();
//      delay(1000);
//  }  
  Serial.println("gprs init success");
  
//  enableGPRS();
//
//  sendSMS(phoneNum,"DIA powered up!");
  
}

void loop() {
  String cmd;
  String sendMsg;

  delay(3000);

  //debug
  updateSensor();
  Serial.println(temperature);
  
  sendData_http();

  
//  cmd=readSMS();
//  Serial.println(cmd);
//  filterMsg(cmd);
  

      if(DIA==true){
          DIA=false;
          if(AUTH==true){
              AUTH=false;
              Serial.println("Sending back to user");
              sendMsg="DIA\n";
              if(SENSOR==true){
                SENSOR=false;
                Serial.println("Sending temprature back to user " + String(temperature));
                sendMsg+= "Temperature - " +  String(temperature) +"  \n" + "Moisture - " +  String(moisture)+"  \n" + "pH Level - " +  String(ph)+"  \n";
              }
              else if(SET==true){
                SET=false;
                char newNum[20];
                (cmd.substring(8,20)).toCharArray(newNum,20);
                Serial.println(newNum);
                sendMsg+="Authorized user, Welcome to DIA.";
                EEPROM_write(newNum,13);
                delay(1000);
                updatePhoneNumber();
              }
              else if(SOL==true){
                //turn on solunoid valve
                sendMsg+="Watering turned on.";
              }
              else if(SOL==false){
                //turn off solunoid valve
                sendMsg+="Watering turned off.";
              }
              else{
                sendMsg+="Please give a valid command!";
              }
              SOL=false;
              sendSMS(phoneNum, sendMsg);
          }
          else{
              sendSMS(phoneNum, "UNAUTHORIZED ACCESS!");
          }
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
      if(!strcmp(phone,phoneNum)){
        Serial.println("AUTHORIZED MESSEGE!");
        AUTH=true;
      }  
      if(!strcmp(phone,serverNum)){
        Serial.println("AUTHORIZED MESSEGE FROM SERVER!");
        AUTH=true;
        SEVER_CMD=true;
      }
   }
   else{
       memset(message, 0, sizeof message);
   }
   return message;
}

void filterMsg(String msg){
  if(SEVER_CMD==true){
    msg=msg.substring(38,-1);
    Serial.println(msg);
    SEVER_CMD=false;
  }
  
  if(msg.substring(0, 3)=="DIA"){
        Serial.println("DIA initaiated");
        DIA=true;
        if(msg.substring(4, 7)=="ALL"){
            Serial.println("requesting all sensor reading");
            SENSOR=true;
        }
        else if(msg.substring(4,7)=="CPN"){
            SET=true;
        }
        else if(msg.substring(4,7)=="SOL" and msg.substring(8,9)=="1"){
            SOL=true;
        }
        else if(msg.substring(4,7)=="SOL" and msg.substring(8,9)=="0"){
            SOL=false;
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

   send_AT_commands("AT+HTTPINIT\r\n");
   delay(2000); 
   Serial.println(send_AT_commands("AT+HTTPPARA=\"URL\",\"http://dia.mybluemix.net/dia.php?number="+(String(phoneNum))+"&temperature="+(String(temperature))+"&moisture="+(String(moisture))+"&ph="+(String(ph))+"\"\r\n"));
   delay(2000);
   Serial.println(send_AT_commands("AT+HTTPACTION=0\r\n"));
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


int send_AT_commands(String at){
  char at_cmd[200]="";
  at.toCharArray(at_cmd, 200);
  return gprsTest.send_custom(at_cmd,sizeof(at_cmd)-1);
}


void updateSensor(){
  temperature= random(10,50);;
  moisture = random(1,10);
  ph = random(1,14);
}


void EEPROM_write(char * data, byte datasize) {
   int addr = 0;
   EEPROM.write(addr++, datasize);
   for (int i=0; i<datasize; i++) {
      EEPROM.write(addr++, data[i]);
   }
}

void updatePhoneNumber(){
      char stringToRead[20];          // allocate enough space for the string here!
      for (int i=0;i<20; i++) {
          stringToRead[i] = EEPROM.read(i);
      }
      (String(stringToRead).substring(1,13)).toCharArray(phoneNum, 20);
      Serial.println(phoneNum);
}

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(5000);
}

//String md5_hash(String str){
//
//   String tokenString = String(str)+phoneNum;
//   char tokenChar[20];
//   tokenString.toCharArray(tokenChar,20);
//   
//  unsigned char* hash=MD5::make_hash(tokenChar);
//  //generate the digest (hex encoding) of our hash
//  char *md5str = MD5::make_digest(hash, 16);
//  free(hash);
//  //print it on our serial monitor
//  Serial.println(md5str);
//  //Give the Memory back to the System if you run the md5 Hash generation in a loop
//  free(md5str);
//  return md5str;
//}

