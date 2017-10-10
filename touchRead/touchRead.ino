int t22 = 0;
int numAvg = 4;
int sumArray[4] = {0};
int sum = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  t22 = touchRead(22);
  leftShift(sumArray, t22);
  sum = 0;
  for (int i = 0; i < numAvg; i++) {
    sum += sumArray[i];
  }

  Serial.print(touchRead(22));
  Serial.print("     ");
  Serial.println(sum / numAvg);
  delay(10);
  if (sum > 10000) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }
}



void leftShift(int *dir, int newNum) {

  for (int i = 0; i < numAvg - 1; i++) {
    *(dir + i) = *(dir + i + 1);
  }
  *(dir + numAvg - 1) = newNum;
}
