boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");  //configura ESP8266 no modo "Station"

  delay(1000);

  if (Serial.find("AT+CWMODE")){
    piscaLed(ledAzul,2);
  }else{
    piscaLed(ledVermelho,2);
  }

  
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
//  Serial1.println(cmd);
  Serial.println(cmd);
  delay(6000);
  
  if (Serial.find("OK")){
  //pisca led AZUL para indicar WiFi conectado
    for(int i = 0; i<15; i++){
    digitalWrite(ledAzul,HIGH);
    delay(50);
    digitalWrite(ledAzul,LOW);
    delay(50);
    }
    return true;
  } else{
  //pisca led VERMELHO para indicar falha conexao WiFi 
  for(int i = 0; i<15; i++){
   digitalWrite(ledVermelho,HIGH);
   delay(50);
   digitalWrite(ledVermelho,LOW);
   delay(50);
  }
    return false;
  }
}
