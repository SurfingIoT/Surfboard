void myBlink() {
  board.printMode(1);
  board.red(255);
  things.wait(board.potentiometer());
  board.red(0);
  things.wait(board.potentiometer());
  board.green(255);
  things.wait(board.potentiometer());
  board.green(0);
  things.wait(board.potentiometer());
  board.blue(255);
  things.wait(board.potentiometer());
  board.blue(0);
  things.wait(board.potentiometer());
}

void light_test() {
  board.printMode(2);
  if (board.light() > board.potentiometer()){
    board.speaker(1);
  }else{
    board.speaker(0);
  }
  board.relay(board.light() > board.potentiometer());
  Serial.print("Light: ");
  Serial.print(board.light());
  Serial.print("   potentiometer: ");
  Serial.println(board.potentiometer());
  things.wait(200);
}

void temperature_test() {
  board.printMode(3);
  int t = map(board.potentiometer(),0, 1023, 5, 40);
  if (board.temperature()>t){
    board.buzzer(4000,1000);
  }
  board.relay(board.temperature()>t);
  Serial.print("Temperature: ");
  Serial.print(board.temperature());
  Serial.print("   potentiometer: ");
  Serial.print(board.potentiometer());
  Serial.print("   Proportional: ");
  Serial.println(t);
  things.wait(200);
}

void humidity_test() {
  board.printMode(4);
  int t = map(board.potentiometer(),0, 1023, 0, 100);
  if (board.humidity()>t){
    board.buzzer(4000,1000);
  }
  board.relay(board.humidity()>t);
  Serial.print("Humidity: ");
  Serial.print(board.humidity());
  Serial.print("   potentiometer: ");
  Serial.print(board.potentiometer());
  Serial.print("   Proportional: ");
  Serial.println(t);
  things.wait(200);
}

void alcohol_test() {
  board.printMode(5);
  if(board.alcohol()<=150) {
    board.printJSON("alcohol","alcohol not detected, you are clean!");
    board.rgb(0,255,0);
  }
  if(board.alcohol()>150 && board.alcohol()<300) {
   board.rgb(0,0,255);
  }
  else if(board.alcohol()>300 && board.alcohol()<500) {
    board.rgb(255,0,0);
  }
  else if(board.alcohol()>800) {
    board.buzzer(3000,1000);
    board.rgb(255,0,0);
  }
  board.printJSON("alcohol",board.alcohol());
  things.wait(500);
}

