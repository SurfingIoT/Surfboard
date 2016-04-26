void test_servo() {
  servo_init();
  while (!Serial.available()) {
    for(int x=0;x<179;x++) {
      servo11.write(x);
      delay(10);
      if(Serial.available()) break;
    }
  }
  inChar = '0';
  printMenu();

}

void servo_init() {
  servo11.attach(9);
}


