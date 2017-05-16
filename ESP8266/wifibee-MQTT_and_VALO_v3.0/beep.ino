void beep() {
  Serial.flush();
  Serial.print("speaker?1");
  Serial.flush();
  emptySerial();
  delay(700);
  Serial.print("speaker?0");
  Serial.flush();
  emptySerial();
  delay(100);
}

void beep(int interval) {
  Serial.print("speaker?1");
  Serial.flush();
  delay(interval);
  Serial.print("speaker?0");
  Serial.flush();
}
