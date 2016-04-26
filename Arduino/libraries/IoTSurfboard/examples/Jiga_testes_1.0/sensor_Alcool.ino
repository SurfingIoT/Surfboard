void teste_sensor_Alcool(){
  while (!Serial.available()) {
    Serial.println (analogRead(sensorAlcoolPin));
    delay(100);
  } 
  inChar = '0';
  printMenu();
}
