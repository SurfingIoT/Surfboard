void myBlink() {
  board.red(255);
  system.wait(board.potentiometer());
  board.red(0);
  system.wait(board.potentiometer());
  board.green(255);
  system.wait(board.potentiometer());
  board.green(0);
  system.wait(board.potentiometer());
  board.blue(255);
  system.wait(board.potentiometer());
  board.blue(0);
  system.wait(board.potentiometer());
  
  
}

void light_test() {
  board.speaker(board.light()>board.potentiometer());
  board.relay(board.light()>board.potentiometer());
  Serial.print("Light: ");
  Serial.print(board.light());
  Serial.print("   potentiometer: ");
  Serial.println(board.potentiometer());
  system.wait(200);
}
void temperature_test() {
  int t = map(board.potentiometer(),0, 1023, 5, 40);
  board.speaker(board.temperatureC()>t);
  board.relay(board.temperatureC()>t);
  Serial.print("Temperature: ");
  Serial.print(board.temperature());
  Serial.print("   potentiometer: ");
  Serial.print(board.potentiometer());
  Serial.print("   Proportional: ");
  Serial.println(t);
  system.wait(200);
}
void humidity_test() {
  int t = map(board.potentiometer(),0, 1023, 0, 100);
  board.speaker(board.humidity()>t);
  board.relay(board.humidity()>t);
  Serial.print("Humidity: ");
  Serial.print(board.humidity());
  Serial.print("   potentiometer: ");
  Serial.print(board.potentiometer());
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



