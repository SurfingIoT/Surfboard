void setup_wifi() {
  delay(100);
  if (ssid.length() > 0 && wifi_state == 0) {
    WiFi.begin(ssid.c_str(), password.c_str());
  }
  else {
    WiFi.begin(ssid_factory, password_factory);
  }
  delay(1500);
  //}
  Serial.print("red?0");
  Serial.print("blue?255");
  delay(500);
  //will try for 15 seconds...
  while (WiFi.status() != WL_CONNECTED && millis() - connection_timeout < 15000) {
    Serial.print("blue?0");
    
    //**********  adicionado por JL  **********
    server.handleClient();  
    //*****************************************
    
    delay(100);
    server.handleClient();  
    delay(100);
    server.handleClient();  
    delay(100);
    server.handleClient();  
    delay(100);
    server.handleClient();  
    delay(100);
    server.handleClient();  
    Serial.print("blue?255");
    delay(100);
    server.handleClient();  
    delay(100);
    server.handleClient();  
    delay(100);
    server.handleClient();  
    delay(100);
    server.handleClient();  
    delay(100);
  }

  
  //**********  adicionado por JL  **********
  if (WiFi.status() != WL_CONNECTED){
    ESP.reset();  //executa um Software reset no ESP
  }
  //*****************************************

  
  Serial.print("blue?0");
  if (WiFi.status() != WL_CONNECTED) return;


    //**********  comentado por JL  **********
//  createWebServer(1);
    //*****************************************

  delay(500);
  Serial.print("green?255");
  client.setServer(mqtt_server.c_str(), 1883);
  client.setCallback(callback);
  delay(1500);
  Serial.print("green?0");
  emptySerial();
  // Start the server
  server.begin();
}


