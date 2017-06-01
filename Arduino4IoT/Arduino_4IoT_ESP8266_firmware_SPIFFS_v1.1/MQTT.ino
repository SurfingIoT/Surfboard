void reconnect() {
  Serial.print("red?255");
  toChar(deviceName);
  Serial.print(">DEBUG Starting MQTT Connecting with ");
  Serial.print(deviceName);
  Serial.println(".");


  client.setServer(mqtt_server.c_str(), 1883);
  client.setCallback(callback);
  if (client.connect(deviceName.c_str())) {
    Serial.println(">DEBUG Connected to MQTT");

    toChar("new-device:" + deviceName);
    client.publish(queue_rendezvous.c_str(), buffer);
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    toChar(ipStr);
    client.publish(queue_rendezvous.c_str(), buffer);
    toChar(queue_listener);
    client.publish(queue_rendezvous.c_str(), buffer);
    toChar(queue_sensors);
    client.publish(queue_rendezvous.c_str(), buffer);
    client.subscribe(queue_broadcast.c_str());
    client.subscribe(queue_listener.c_str());
    Serial.print("red?0");
  }
  else {
    Serial.println(">DEBUG Couldn't connect to MQTT");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String strTopic = topic;
  //let's check if is the broadcast, device or system queue!
  commandString = "";
  for (int i = 0; i < length; i++) {
    commandString += (char)payload[i];
  }
  debug("execute command MQTT message " + commandString);
  executeCommand();
  commandComplete = false;
  commandString = "";
}

void debug(String msg) {
  toChar(msg);
  client.publish(queue_rendezvous.c_str(), buffer);
}

