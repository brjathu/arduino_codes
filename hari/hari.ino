
#include <Servo.h> 

int val_01, val_02, val_03, val_04;
int vall_01, vall_02, vall_03, vall_04;
int va, Dis_val, DDis_val, Box_val, BBox_val, First_OK, Stop;
Servo Servo_01;
Servo Servo_02;
int Servo_Speed = 15;
int Servo_01_Min = 45, Servo_01_Max = 179, Servo_02_Min = -1, Servo_02_Max = 179;
int Pikuped, Griped, UnGriped;

void setup()
{
  pinMode(14, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  Servo_01.attach(3);
  Servo_02.attach(4);
  Servo_01.write(Servo_01_Min);
  Servo_02.write(Servo_02_Min);
  Serial.begin(9600);
}

void loop()
{
  Line_Read();
  Va_Read();
  Dis_Sen();
  Box_Sen();
  Print();
  Inti();
  Access();
  Pik();
}

void Access()
{
  if(vall_02 > va && vall_03 > va && Stop == 0)
  {
    Robot_F();
  }
  else if(vall_02 > va && vall_03 < va && Stop == 0)
  {
    Robot_L_T();
  }
  else if(vall_02 < va && vall_03 > va && Stop == 0)
  {
    Robot_R_T();
  }
  else if(vall_02 < va && vall_03 < va && Stop == 0)
  {
    
  }
  if(DDis_val > 280 && DDis_val < 380 && Stop == 0)
  {
    delay(100);
    Robot_L_TT();
    delay(500);
    First_OK = 1;
  }
  if(DDis_val > 60 && DDis_val < 100 && First_OK == 1 && Stop == 0)
  {
    delay(100);
    Robot_R();
    delay(750);
    Robot_S();
    Stop = 1;
    UnGrip();
  }
}
void Pik()
{
  if(BBox_val > 50 && BBox_val < 95 && Pikuped == 0)
  {
    Robot_S();
    Grip();
    Pikuped = 1;
  }
}
void Grip()
{
  if(Griped == 0)
  {
    for(int i = Servo_01_Min; i < Servo_01_Max; i++)
    {
      Servo_01.write(i);
      delay(Servo_Speed);
    }
    for(int i = Servo_02_Min; i < Servo_02_Max; i++)
    {
      Servo_02.write(i);
      delay(Servo_Speed);
    }
    for(int i = Servo_01_Max; i > Servo_01_Min; i--)
    {
      Servo_01.write(i);
      delay(Servo_Speed);
    }
    for(int i = Servo_02_Max; i > Servo_02_Min; i--)
    {
      Servo_02.write(i);
      delay(Servo_Speed);
    }
    Griped = 1;
  }
}
void UnGrip()
{
  if(UnGriped == 0)
  {
    for(int i = Servo_02_Min; i < Servo_02_Max; i++)
    {
      Servo_02.write(i);
      delay(Servo_Speed);
    }
    for(int i = Servo_01_Min; i < Servo_01_Max; i++)
    {
      Servo_01.write(i);
      delay(Servo_Speed);
    }
    for(int i = Servo_02_Max; i > Servo_02_Min; i--)
    {
      Servo_02.write(i);
      delay(Servo_Speed);
    }
    for(int i = Servo_01_Max; i > Servo_01_Min; i--)
    {
      Servo_01.write(i);
      delay(Servo_Speed);
    }
    UnGriped = 1;
  }
}
void Print()
{
  Serial.print(vall_01);
  Serial.print("  ");
  Serial.print(vall_02);
  Serial.print("  ");
  Serial.print(vall_03);
  Serial.print("  ");
  Serial.print(vall_04);
  Serial.print("  ");
  Serial.print(va);
  Serial.print("  ");
  Serial.print(DDis_val);
  Serial.print("  ");
  Serial.println(BBox_val);
}
void Line_Read()
{
  val_01 = analogRead(A1);
  val_02 = analogRead(A2);
  val_03 = analogRead(A3);
  val_04 = analogRead(A4);
  
  vall_01 = map(val_01, 18, 192, 20, 250);
  vall_02 = map(val_02, 25, 222, 20, 250);
  vall_03 = map(val_03, 25, 273, 20, 250);
  vall_04 = map(val_04, 18, 231, 20, 250);
}
void Va_Read()
{
  va = analogRead(A7);
  va = map(va, 0, 1023, 20, 250);
}
void Dis_Sen()
{
  for(int i = 0; i < 25; i++)
  {
    Dis_val = analogRead(A8);
    DDis_val = DDis_val + Dis_val;
  }
  DDis_val = DDis_val / 25;
}
void Box_Sen()
{
  for(int i = 0; i < 25; i++)
  {
    Box_val = analogRead(A9);
    BBox_val = BBox_val + Box_val;
  }
  BBox_val = BBox_val / 25;
}
void Inti()
{
  if(vall_02 > va)
  {
    digitalWrite(14, HIGH);
  }
  else
  {
    digitalWrite(14, LOW);
  }
  if(vall_03 > va)
  {
    digitalWrite(18, HIGH);
  }
  else
  {
    digitalWrite(18, LOW);
  }
}
void Robot_F()
{
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
}
void Robot_R()
{
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
}
void Robot_S()
{
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}
void Robot_R_T()
{
  analogWrite(5, 96);
  digitalWrite(6, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
}
void Robot_R_TT()
{
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
}
void Robot_L_T()
{
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  analogWrite(11, 96);
  digitalWrite(12, LOW);
}
void Robot_L_TT()
{
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
}

