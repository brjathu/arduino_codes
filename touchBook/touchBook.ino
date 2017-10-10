#define next 23
#define back 22



/////////////////////////////////touch variables
int touchThreshold = 10000;
int numSamples = 4;

int touchNext = 0;
int sumArrayNext[6] = {0};

int touchBack = 0;
int sumArrayBack[6] = {0};
///////////////////////////////////////////////////


void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  delay(3000);
  //startBrochure();


  pinMode(10,INPUT);
  pinMode(11,INPUT);
  pinMode(12,INPUT);
}

void loop() {
//setUpBook();

Serial.print(digitalRead(10));
Serial.print("\t");
Serial.print(digitalRead(11));
Serial.print("\t");
Serial.print(digitalRead(12));
Serial.println("\t");


  

}



//void setUpBook(){
//  //getting inputs
//  touchNext = touchRead(next);
//  touchBack = touchRead(back);
//
//  leftShift(sumArrayNext, touchNext);
//  leftShift(sumArrayBack, touchBack);
//    Serial.print(touchNext);
//    Serial.print("     ");
//    Serial.print(sumArrayNext[0]);
//    Serial.print("     ");
//    Serial.print(sumArrayNext[1]);
//    Serial.print("     ");
//    Serial.print(sumArrayNext[2]);
//    Serial.print("     ");
//    Serial.print(sumArrayNext[3]);
//    Serial.print("     ");
//    Serial.print(sumArrayNext[4]);
//    Serial.print("     ");
//    Serial.println(sumArrayNext[5]);
//
//
//  //next
//  if (sumArrayNext[numSamples + 1] > touchThreshold) {
//    digitalWrite(13, HIGH);
//    nextKey();
//  }
//  else {
//    digitalWrite(13, LOW);
//  }
//
//
//  //back
//  if (sumArrayBack[numSamples + 1] > touchThreshold) {
//    digitalWrite(13, HIGH);
//    backKey();
//  }
//  else {
//    digitalWrite(13, LOW);
//  }
//
//
//
//  delay(10);
//}



//void startBrochure() {
//  digitalWrite(13, HIGH);
//  Keyboard.set_modifier(MODIFIERKEY_GUI);
//  Keyboard.send_now();
//  Keyboard.set_modifier(0);
//  Keyboard.set_key1(0);
//  Keyboard.send_now();
//  delay(100);
//  delay(100);
//  Keyboard.print("www.google.com");
//  delay(100);
//  Keyboard.set_key1(KEY_ENTER);
//  Keyboard.send_now();
//  Keyboard.set_key1(0);
//  Keyboard.send_now();
//
//
//
//  delay(1000);
//
//
//  Keyboard.set_modifier(MODIFIERKEY_CTRL);
//  Keyboard.set_key1(KEY_L);
//  Keyboard.send_now();
//  Keyboard.set_modifier(0);
//  Keyboard.set_key1(0);
//  Keyboard.send_now();
//  delay(1000);
//
//  Keyboard.print("http://online.fliphtml5.com/eyfe/rpnp/#p=1");
//  Keyboard.set_key1(KEY_ENTER);
//  Keyboard.send_now();
//  Keyboard.set_key1(0);
//  Keyboard.send_now();
//
//  digitalWrite(13, LOW); 
//  delay(5000);
//}
//
//void nextKey() {
//  Keyboard.set_key1(KEY_RIGHT);
//  Keyboard.send_now();
//  Keyboard.set_key1(0);
//  Keyboard.send_now();
//  delay(500);
//}
//
//void backKey() {
//  Keyboard.set_key1(KEY_LEFT);
//  Keyboard.send_now();
//  Keyboard.set_key1(0);
//  Keyboard.send_now();
//  delay(500);
//}
void leftShift(int *dir, int newNum) {
  *(dir + numSamples) = 0;
  for (int i = 0; i < numSamples - 1; i++) {
    *(dir + i) = *(dir + i + 1);
    *(dir + numSamples) += *(dir + i + 1);
  }
  *(dir + numSamples) += newNum;
  *(dir + numSamples - 1) = newNum;
  *(dir + numSamples + 1) = *(dir + numSamples) / numSamples;
}
