void read_memory() {
  if(!SPIFFS.exists("/config/ssid")){
    //Serial.println("Memory not read Today...");
    emptySerial();
    return;
  }
  //Serial.println("Start reading memory");

  ssid = readFile("/config/ssid");
  //Serial.println(ssid);
  password = readFile("/config/passwd");
  //Serial.println(password);
  mqtt_server = readFile("/config/mqtt_serv");
  //Serial.println(mqtt_server);
  sensor_interval = atoi(readFile("/config/sensor_int").c_str());
  //Serial.println(sensor_interval);
  queue_listener = readFile("/config/listener");
  //Serial.println(queue_listener);
  queue_sensors = readFile("/config/sensors");
  //Serial.println(queue_sensors);
  queue_broadcast = readFile("/config/broadcast");
  //Serial.println(queue_broadcast);
  queue_rendezvous = readFile("/config/rendezvous");
  //Serial.println(queue_rendezvous);
  emptySerial();
    
  ssid.trim();        //Get a version of the String with any leading and trailing whitespace removed
  password.trim();
  mqtt_server.trim();
  queue_sensors.trim();
  queue_rendezvous.trim();
  queue_broadcast.trim();
  queue_listener.trim();
  
  queue_sensors.replace("\0","");   //replace all instances of a given character with another character
  queue_broadcast.replace("\0","");
  queue_rendezvous.replace("\0","");
  queue_listener.replace("\0","");
  
  /*Serial.println("Finshed reading memory");
  Serial.println(ssid);
  Serial.println(password);
  Serial.println(mqtt_server);
  Serial.println(queue_sensors);
  Serial.println(queue_broadcast);
  Serial.println(queue_rendezvous);
  Serial.println(queue_listener);
  Serial.println(sensor_interval);
  Serial.println(valo_server);
  Serial.println(latitude);
  Serial.println(longitude);
  Serial.println(String(VALO_HOST));
*/  
  emptySerial();

}

void save() {

  if(!SPIFFS.exists("/config/ssid")){
    createFile("/config/ssid");
  }
  writeFile("/config/ssid",ssid);

  if(!SPIFFS.exists("/config/passwd")){
    createFile("/config/passwd");
  }
  writeFile("/config/passwd",password);

  if(!SPIFFS.exists("/config/mqtt_serv")){
    createFile("/config/mqtt_serv");
  }
  writeFile("/config/mqtt_serv",mqtt_server);

  if(!SPIFFS.exists("/config/sensor_int")){
    createFile("/config/sensor_int");
  }
  writeFile("/config/sensor_int",String(sensor_interval));

  if(!SPIFFS.exists("/config/listener")){
    createFile("/config/listener");
  }
  writeFile("/config/listener",queue_listener);
  
  if(!SPIFFS.exists("/config/sensors")){
    createFile("/config/sensors");
  }
  writeFile("/config/sensors",queue_sensors);

  if(!SPIFFS.exists("/config/broadcast")){
    createFile("/config/broadcast");
  }
  writeFile("/config/broadcast",queue_broadcast);

  if(!SPIFFS.exists("/config/rendezvous")){
    createFile("/config/rendezvous");
  }
  writeFile("/config/rendezvous",queue_rendezvous);


}

void restore() {
  formatFS();
}

