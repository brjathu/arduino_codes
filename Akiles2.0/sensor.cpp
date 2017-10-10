/////////////////////////////////////////////////////////////////////////////////////////////sensor
int senVal[8] = {0};
int pin[8] = {62, 63, 64, 65, 66, 67, 68, 69};
int senCalWhite[8] = {0};
int senCalBlack[8] = {0};
//**********************************************************************************************







void sensorRead() {
  lineCount = 0;
  lineSum = 0;
  for (int i = 0; i < 8; i++) {
    senVal[i] = map(getSensorVal(3, pin[i]) , senCalBlack[i], senCalWhite[i], 0, 1024);
    Serial.print(senVal[i]);
    Serial.print("\t");
    if (senVal[i] > 800) {
      lineArr[i] = 1;
      lineSum += 1000 * i;
      lineCount++;
    }
    else {
      lineArr[i] = 0;
    }



  }
  Serial.print(lineCount);
  Serial.print("\t");
  if (lineCount != 0) {
    lineError = 3500 - lineSum / lineCount;
  }
  else {
    lineError = 0;

  }
  Serial.print(lineError);
  Serial.print("\t");
  Serial.println();
}


int getSensorVal(int x, int pin) {
  int sum = 0;
  for (int i = 0; i < x; i++) {
    sum += analogRead(pin);
  }
  return (sum / x);
}

void calibirate() {
  for (int i = 0; i < 8; i++) {
    senCalWhite[i] = getSensorVal(10, pin[i]);
    Serial.print(senCalWhite[i]);
    Serial.print("\t");
  }
  Serial.println();
  delay(5000);


  for (int i = 0; i < 8; i++) {
    senCalBlack[i] = getSensorVal(10, pin[i]);
    Serial.print(senCalBlack[i]);
    Serial.print("\t");
  }
  Serial.println();
  delay(3000);

}

