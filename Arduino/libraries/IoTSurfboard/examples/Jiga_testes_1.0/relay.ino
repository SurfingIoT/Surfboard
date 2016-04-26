void testRele(){
  pinMode(relePin, OUTPUT);
  while (!Serial.available()) {
    digitalWrite(relePin,HIGH);
    delay(500);
    digitalWrite(relePin,LOW);
    delay(500);
  } 
  inChar = '0';
  printMenu();
}
