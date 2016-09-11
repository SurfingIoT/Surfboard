void startWIFI(void) {
  IPAddress ip = WiFi.localIP();  //set IP if not correct
  if( ip!= ipadd) {   //if( (ip[0]!=ipadd[0]) || (ip[1]!=ipadd[1]) || (ip[2]!=ipadd[2]) || (ip[3]!=ipadd[3]) ) {
      WiFi.config(ipadd, ipgat, ipsub);  
      Serial.println();
      Serial.print("ESP8266 IP:");
      Serial.println(ip);
      Serial.print("Fixed   IP:");
      Serial.println(ipadd);
      Serial.print("IP now set to: ");
      Serial.println(WiFi.localIP());
  }
  Serial.println();   // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin(); // Start the server
  Serial.println("Server started");
  Serial.print("ESP8266 IP: "); // Print the IP address
  Serial.println(WiFi.localIP());
  Serial.print("ESP8266 WebServer Port: ");
  Serial.println(SVRPORT);
  yieldEspCPU(); 
  delay(300);
}
