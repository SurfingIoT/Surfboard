void testPotenciometro(){
  while (!Serial.available()) {
    Serial.println (analogRead(PotPin));
    delay(100);
  } 
  inChar = '0';
    printMenu();
}

