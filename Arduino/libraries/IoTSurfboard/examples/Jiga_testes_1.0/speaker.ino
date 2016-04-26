void teste_speaker(){
  pinMode(speakerPin, OUTPUT);
  while (!Serial.available()) {
    digitalWrite(speakerPin,HIGH);
    delay(100);
    digitalWrite(speakerPin,LOW);
    delay(100);
  } 
  inChar = '0';
  printMenu();
}
