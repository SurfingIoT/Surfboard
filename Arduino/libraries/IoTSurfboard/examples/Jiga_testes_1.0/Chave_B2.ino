void teste_chave_B2(){
  while (!Serial.available()) {
    digitalWrite (ledRGB_R_Pin, !digitalRead(chaveB2Pin)); 
    Serial.print("chave B2 = ");
    Serial.println(digitalRead(chaveB2Pin));
  }
  inChar = '0';
  printMenu();
}
