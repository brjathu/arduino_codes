#include <Servo.h>
Servo arm;
Servo gripper;


String inString = "";
int pos;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  arm.attach(46);
  gripper.attach(44);

  
  arm.write(20);
  gripper.write(35);
}

void loop() {//low 165   upper 20
  //gripper low 100........ high 35
  delay(1000);
 armDown();
 delay(2000);
 gripOn();
 delay(1000);
 armUp();
 delay(1000000);



}



void armDown() {

  for (pos = 25; pos <= 160; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    arm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

}


void armUp() {

  for (pos = 160; pos >= 25; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    arm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

}



void gripOn() {

  for (pos = 35; pos <= 95; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    gripper.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

}


void gripOff() {

  for (pos = 80; pos >= 35; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    gripper.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

}




