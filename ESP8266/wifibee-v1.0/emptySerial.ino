void emptySerial() {
  //descartando a resposta...
  Serial.flush();
  while (Serial.available()) {
    Serial.read();
  }
}
