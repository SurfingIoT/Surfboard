void teste_pir(){
  while (!Serial.available()) {
  //  Serial.println (digitalRead());
    delay(100);
  } 
  inChar = '0';
  printMenu();
}
