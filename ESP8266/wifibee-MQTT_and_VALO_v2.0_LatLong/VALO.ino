void conectClient(){
  if (!espClient.connect(VALO_HOST, VALO_PORT)) {  //stablish connection to Valo
    
    //Serial.println("connection failed");
    return;
  }else{
    //Serial.println("connection OK");
  }
}

void disconectClient(){
  //Serial.println("");
  //Serial.println("closing connection");
  espClient.stop();
}
