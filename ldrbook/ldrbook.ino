int pagePin[] = {6, 5, 4, 3, 2, 1, 0};
int currentPageNum = 0;
int lastPageNum=0;
int pageNum = 7;

void setup() {
  
  startBrochure();
  startBrochure();
}

void loop() {

  goToPage();


}

void findPage() {
  int max = 0;
  
  for (int i = 0; i < pageNum; i++) {
    int val = analogRead(pagePin[i]);
    if (val > max && val > 500) {
      max = val;
      currentPageNum = i;
    }
  }
  
}

void startBrochure() {
  Keyboard.set_modifier(MODIFIERKEY_GUI);
  Keyboard.send_now();
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(100);
  
  Keyboard.print("www.google.com");
  delay(100);
  Keyboard.set_key1(KEY_ENTER);
  Keyboard.send_now();
  Keyboard.set_key1(0);
  Keyboard.send_now();



  delay(1000);


  Keyboard.set_modifier(MODIFIERKEY_CTRL);
  Keyboard.set_key1(KEY_L);
  Keyboard.send_now();
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(1000);

  Keyboard.print("http://online.fliphtml5.com/eyfe/rpnp/#p=1");
  Keyboard.set_key1(KEY_ENTER);
  Keyboard.send_now();
  Keyboard.set_key1(0);
  Keyboard.send_now();

  delay(5000);
}




void goToPage() {
  lastPageNum=currentPageNum;
  findPage();
  int pageDiff=currentPageNum-lastPageNum;
  if(pageDiff>0){
    for(int i=0;i<pageDiff;i++){
      nextKey();
    }
  }
  else if(pageDiff<0){
    for(int i=0;i<abs(pageDiff);i++){
      backKey();
    }
  }
delay(2000);
  

  
}


void nextKey() {
  Keyboard.set_key1(KEY_RIGHT);
  Keyboard.send_now();
  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(500);
}

void backKey() {
  Keyboard.set_key1(KEY_LEFT);
  Keyboard.send_now();
  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(500);
}

