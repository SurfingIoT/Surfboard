void testLDR(){
  while (!Serial.available()) {
    Serial.println (analogRead(LDRPin));
    delay(100);
  } 
  inChar = '0';
  printMenu();
}
