void connectClient(){
  toChar(valo_server);
  Serial.print(">");
  Serial.println(valo_server);
  Serial.print(">");
  Serial.println(valo_server.c_str());
  Serial.print(">");
  Serial.println(buffer);
  const char* h="192.168.0.110";
  if (!espClient.connect(h, 8888)) {  //stablish connection to Valo
  //if (!espClient.connect("192.168.0.110", 8888)) {  //stablish connection to Valo
    Serial.print(">");
    Serial.println("connection failed");
    
    //Serial.println("connection failed");
    return;
  }else{
    //Serial.println("connection OK");
  }
}

void disconnectClient(){
  //Serial.println("");
  //Serial.println("closing connection");
  espClient.stop();
}
