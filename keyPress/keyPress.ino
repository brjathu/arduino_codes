int count = 0;
void setup() { } // no setup needed
void loop() {
  //  Keyboard.print("Hello World ");
  //  Keyboard.println(count);
  //  count = count + 1;
  delay(3000);



  //  Keyboard.set_modifier(MODIFIERKEY_GUI);
  //  Keyboard.send_now();

  //  Keyboard.set_modifier(0);
  //  Keyboard.set_key1(0);
  //  Keyboard.send_now();


  //  Keyboard.print("browser");






  Keyboard.set_modifier(MODIFIERKEY_GUI);
  Keyboard.send_now();
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(100);
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
  delay(2000);

  Keyboard.set_key1(KEY_RIGHT);
  Keyboard.send_now();
    Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(2000);

  Keyboard.set_key1(KEY_RIGHT);
  Keyboard.send_now();
    Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(2000);

  Keyboard.set_key1(KEY_RIGHT);
  Keyboard.send_now();
    Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(2000);

  Keyboard.set_key1(KEY_RIGHT);
  Keyboard.send_now();
    Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(2000);






  //  Keyboard.press(KEY_TAB);
  //  Keyboard.release(KEY_TAB);

  //  Keyboard.set_key1(KEY_TAB);
  //  Keyboard.send_now();
  //  Keyboard.set_key1(KEY_ENTER);
  //  Keyboard.send_now();

  //  Keyboard.press(KEY_ENTER);
  //  Keyboard.release(KEY_ENTER);

  delay(100000);




}



































































































































































