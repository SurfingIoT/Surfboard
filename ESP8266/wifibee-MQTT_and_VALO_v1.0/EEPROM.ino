void read_memory() {
  if ((char)EEPROM.read(511) != 'I') {
    Serial.println("Memory not read Today...");
    emptySerial();
    return;
  }
  Serial.println("Start reading memory");
  emptySerial();
  ssid = "";
  for (int i = 0; i < 32; i++)
  {
    ssid += char(EEPROM.read(i));
  }
  password = "";
  for (int i = 32; i < 64; i++)
  {
    password += char(EEPROM.read(i));
  }
  mqtt_server = "";
  for (int i = 96; i < 160; i++)
  {
    mqtt_server += char(EEPROM.read(i));
  }
  queue_listener = "";
  for (int i = 160; i < 224; i++)
  {
    queue_listener += char(EEPROM.read(i));
  }
  queue_sensors = "";
  for (int i = 224; i < 288; i++)
  {
    queue_sensors += char(EEPROM.read(i));
  }
  queue_broadcast = "";
  for (int i = 288; i < 352; i++)
  {
    queue_broadcast += char(EEPROM.read(i));
  }
  queue_rendezvous = "";
  for (int i = 352; i < 416; i++)
  {
    queue_rendezvous += char(EEPROM.read(i));
  }

  opMode = "";
  for (int i = 416; i < 420; i++)
  {
    opMode += char(EEPROM.read(i));
  }

  valo_server = "";
  for (int i = 430; i < 494; i++)
  {
    valo_server += char(EEPROM.read(i));
    valo_server.toCharArray(VALO_HOST, valo_server.length());
  }


  
  /*String interval = "";
    for (int i = 416; i < 234; i++)
    {
    interval += char(EEPROM.read(i));
    }
    sensor_interval = atoi(interval.c_str());*/
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
  
  Serial.println("Finshed reading memory");
  Serial.println(ssid);
  Serial.println(password);
  Serial.println(mqtt_server);
  Serial.println(queue_sensors);
  Serial.println(queue_broadcast);
  Serial.println(queue_rendezvous);
  Serial.println(queue_listener);
  Serial.println(sensor_interval);
  
  emptySerial();

}

void save() {
  for (int x = 0; x < 512; x++) EEPROM.write(x, ' ');
  EEPROM.write(511, 'I');
  for (int x = 0; x < ssid.length(); x++) {
    EEPROM.write(x, ssid[x]);
  }
  for (int x = 0; x < password.length(); x++) {
    EEPROM.write(x + 32, password[x]);
  }
  for (int x = 0; x < mqtt_server.length(); x++) {
    EEPROM.write(x + 96, mqtt_server[x]);
  }
  for (int x = 0; x < queue_listener.length(); x++) {
    EEPROM.write(x + 160, queue_listener[x]);
  }
  for (int x = 0; x < queue_sensors.length(); x++) {
    EEPROM.write(x + 224, queue_sensors[x]);
  }
  for (int x = 0; x < queue_broadcast.length(); x++) {
    EEPROM.write(x + 288, queue_broadcast[x]);
  }
  for (int x = 0; x < queue_rendezvous.length(); x++) {
    EEPROM.write(x + 352, queue_rendezvous[x]);
  }

  for (int x = 0; x < opMode.length(); x++) {
    EEPROM.write(x + 416, opMode[x]);
  }

  for (int x = 0; x < valo_server.length(); x++) {
    EEPROM.write(x + 430, valo_server[x]);
  }




  
  /*String temp = String(sensor_interval);
    temp.replace("\n","");
    temp.replace("\0","");
    for (int x = 0; x < temp.length(); x++) {
    EEPROM.write(x + 416, temp[x]);
    }*/
  EEPROM.commit();
}


void restore() {
  EEPROM.write(511, 0);
  EEPROM.commit();
}

