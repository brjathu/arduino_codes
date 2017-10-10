

#define r1 13
#define r2 12
#define r3 11
#define r4 10
#define r5 9
//
//
#define g1 8
#define g2 7
#define g3 6
#define g4 5
#define g5 4


#define b1 3
#define b2 2
#define b3 46
#define b4 45
#define b5 47

void setup() {
  


pinMode(r1,OUTPUT);
pinMode(r2,OUTPUT);
pinMode(r3,OUTPUT);
pinMode(r4,OUTPUT);
pinMode(r5,OUTPUT);
//
pinMode(g1,OUTPUT);
pinMode(g2,OUTPUT);
pinMode(g3,OUTPUT);
pinMode(g4,OUTPUT);
pinMode(g5,OUTPUT);

//
pinMode(b1,OUTPUT);
pinMode(b2,OUTPUT);
pinMode(b3,OUTPUT);
pinMode(b4,OUTPUT);
pinMode(b5,OUTPUT);
}

void loop() {


for(int i=0;i<255;i++){
  color(i,0,0);
  delay(3);
}

for(int i=0;i<255;i++){
  color(0,i,0);
  delay(3);
}
for(int i=0;i<255;i++){
  color(0,0,i);
  delay(3);
}
for(int i=0;i<255;i++){
  color(0,255-i,i);
  delay(3);
}
for(int i=0;i<255;i++){
  color(i,0,i);
  delay(3);
}
for(int i=0;i<255;i++){
  color(255,i,0);
  delay(3);
}



}



void color(int r,int b,int g){


  analogWrite(r1,r);
  analogWrite(r2,r);
  analogWrite(r3,r);
  analogWrite(r4,r);
 
  analogWrite(r5,r);
//
//
    analogWrite(g1,g);
  analogWrite(g2,g);
  analogWrite(g3,g);
  analogWrite(g4,g);
  analogWrite(g5,g);

//
    analogWrite(b1,b);
  analogWrite(b2,b);
  analogWrite(b3,b);
  analogWrite(b4,b);
  analogWrite(b5,b);

}

