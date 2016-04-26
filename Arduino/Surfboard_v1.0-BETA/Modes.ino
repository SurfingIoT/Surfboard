void colors() {
  for (int x = 0; x <= 255; x++) {
    analogWrite(5, x);
    system.wait(10);
  }
  for (int x = 255; x >= 0; x--) {
    analogWrite(5, x);
    system.wait(10);
  }
  for (int x = 0; x <= 255; x++) {
    analogWrite(6, x);
    system.wait(10);
  }
  for (int x = 255; x >= 0; x--) {
    analogWrite(6, x);
    system.wait(10);
  }
  for (int x = 0; x <= 255; x++) {
    analogWrite(10, x);
    system.wait(10);
  }
  for (int x = 255; x >= 0; x--) {
    analogWrite(10, x);
    system.wait(10);
  }
}
void light() {
  board.speaker(board.light()>board.potenciometer());
  board.relay(board.light()>board.potenciometer());
  Serial.print("Light: ");
  Serial.print(board.light());
  Serial.print("   Potenciometer: ");
  Serial.println(board.potenciometer());
  system.wait(200);
}
void temperature_test() {
  int t = map(board.potenciometer(),0, 1023, 5, 40);
  board.speaker(board.temperatureC()>t);
  board.relay(board.temperatureC()>t);
  Serial.print("Temperature: ");
  Serial.print(board.temperature());
  Serial.print("   Potenciometer: ");
  Serial.print(board.potenciometer());
  Serial.print("   Proportional: ");
  Serial.println(t);
  system.wait(200);
}
void humidity_test() {
  int t = map(board.potenciometer(),0, 1023, 0, 100);
  board.speaker(board.humidity()>t);
  board.relay(board.humidity()>t);
  Serial.print("Humidity: ");
  Serial.print(board.humidity());
  Serial.print("   Potenciometer: ");
  Serial.print(board.potenciometer());
  Serial.print("   Proportional: ");
  Serial.println(t);
  system.wait(200);
}

void alcohol_test() {
  board.rgb(0,255,0);
  if(board.alcohol()<=150) {
    board.printJSON("alcohol","alcohol not detected, you are clean!");
  }
  if(board.alcohol()>150 && board.alcohol()<300) {
   board.rgb(0,0,255);
  }
  else if(board.alcohol()>300 && board.alcohol()<500) {
    board.rgb(255,0,0);
  }
  else if(board.alcohol()>500) {
    board.speaker(1);
    board.rgb(255,0,0);
  }
  board.printJSON("alcohol",board.alcohol());
  system.wait(500);
  
}





