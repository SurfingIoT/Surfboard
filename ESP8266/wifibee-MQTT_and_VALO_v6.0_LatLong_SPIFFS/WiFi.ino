void setup_wifi() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  
  delay(100);
  if (ssid.length() > 0 && wifi_state == 0) {
    if(WiFi.status() != WL_CONNECTED)     WiFi.begin(ssid.c_str(), password.c_str());
  }
  else {
    if(WiFi.status() != WL_CONNECTED)     WiFi.begin(ssid.c_str(), password.c_str());
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

    delay(500);
    Serial.print("blue?255");
    delay(500);
  }


  //**********  adicionado por JL  **********
  if (WiFi.status() != WL_CONNECTED) {
    ESP.reset();  //executa um Software reset no ESP
  }
  //*****************************************


  if (WiFi.status() != WL_CONNECTED) {
    wifi_state = 0;
    return;
  }
  else {
    Serial.print("blue?0");

    //**********  comentado por JL  **********
    //  createWebServer(1);
    //*****************************************
    wifi_state = 1;
    delay(500);
    Serial.print("green?255");

    delay(1500);

    Serial.print("green?0");
    emptySerial();
    // Start the server
    //server.begin();
  }
}


