String content;
String st;
int statusCode;

void createWebServer(int webtype) {
  server.on("/", []() {
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    content = "<!DOCTYPE HTML>\r\n<html> IoT Surfboard IP ";
    content += ipStr;
    content += "<p>";
    content += st;
    content += "</p><form method='get' action='setup'><label>SSID: </label>";
    content += "<input name='ssid' length=32 value='";
    content += ssid + "'>";
    content += "<input name='pass' type='password' length=16 value='";
    content += password + "'><br/>";

    content += "<label>MQTT Broker: </label><input name='mqtt_server' length=32 value='";
    content += mqtt_server + "'><br/>";
    content += "<label>Sensor Publishing Interval (MS): </label><input name='sensor_interval' length=16 value='";
    String s_t = String(sensor_interval);
    content += s_t + "'><br/>";

    content += "<label>MQTT Broadcast Queue: </label><input name='queue_broadcast' length=64 value='";
    content += queue_broadcast + "'><br/>";

    content += "<label>MQTT Listener Queue: </label><input name='queue_listener' length=64 value='";
    content += queue_listener + "'><br/>";

    content += "<label>MQTT Sensors Queue: </label><input name='queue_sensors' length=64 value='";
    content += queue_sensors + "'><br/>";

    content += "<label>MQTT Rendezvous Queue: </label><input name='queue_rendezvous' length=64 value='";
    content += queue_rendezvous + "'><br/>";

    // add by Jose Luiz for VALO treatment
    //uncoment next lines for debug
    /*content += "<label>OP Mode: </label>";
      content += opMode + "<br/>";
      content += "<label>VALO HOST: </label>";
      content += String(VALO_HOST) + "<br/>";

      content += "<label>VALO HOST SIZE: </label>";
      content += String(sizeof(VALO_HOST)) + "<br/>";



      content += "<label>LATITUDE: </label>";
      content += String(LATITUDE) + "<br/>";
      content += "<label>LONGITUDE: </label>";
      content += String(LONGITUDE) + "<br/>";
    */
    content += "<tr> <td>Operation Mode:</td> <td>MQTT";
    if (opMode == "MQTT") {
      content += "<input name='opMode' type='radio' value='MQTT' checked></td>"; //MQTT mode
      content += "<td>VALO";
      content += "<input name='opMode' type='radio' value='VALO' ></td></tr><br/>";  //VALO mode
    } else {
      content += "<input name='opMode' type='radio' value='MQTT' ></td>"; //MQTT mode
      content += "<td>VALO";
      content += "<input name='opMode' type='radio' value='VALO' checked></td></tr><br/>";  //VALO mode
    }

    content += "<label>VALO Server: </label><input name='Valo_Server' length=32 value='"; //Valo Server config
    content += valo_server + "'><br/>";

    content += "<label>Latitude: </label><input name='latitude' length=16 value='"; //Valo latitude
    content += latitude + "'><br/>";

    content += "<label>Longitude: </label><input name='longitude' length=16 value='"; //Valo longitude
    content += longitude + "'><br/>";

    content += "<br/><input type='submit'></form>";
    content += "</html>";
    server.send(200, "text/html", content);
  });
  server.on("/sensors", []() {
    Serial.print("sensors");
    Serial.flush();
    String sensors = Serial.readString();
    toChar(sensors);
    server.send(200, "application/json", buffer);
  });
  server.on("/debug", []() {
    DEEP_DEBUG= DEEP_DEBUG==0 ? 1 : 0;

    if(DEEP_DEBUG==1) {
      server.send(200, "application/json", "{success: deep debug on}" );
    }
    else {
      server.send(200, "application/json", "{success: deep debug off}" );
    }
    
  });
  server.on("/control", []() {
    String command = server.arg("thing");
    command.replace("=", "?");
    Serial.print(command);
    Serial.flush();
    String resp = Serial.readString();
    toChar(resp);
    server.send(200, "application/json", buffer);
  });
  server.on("/restore", []() {
    restore();
    beep();
    server.send(200, "application/json", buffer);
  });
  server.on("/setup", []() {
    ssid = server.arg("ssid");
    password = server.arg("pass");
    mqtt_server = server.arg("mqtt_server");
    queue_listener = server.arg("queue_listener");
    queue_broadcast = server.arg("queue_broadcast");
    queue_sensors = server.arg("queue_sensors");
    queue_rendezvous = server.arg("queue_rendezvous");
    String t_interval = server.arg("sensor_interval");
    sensor_interval = atoi(t_interval.c_str());
    valo_server = server.arg("Valo_Server");
    latitude = server.arg("latitude");
    latitude.toCharArray(LATITUDE, latitude.length());
    longitude = server.arg("longitude");
    longitude.toCharArray(LONGITUDE, longitude.length());

    //valo_server.trim();
    //valo_server += '\0';
    //valo_server.replace("\0","");

    opMode = server.arg("opMode");
    //valo_server += '\0';
    valo_server.toCharArray(VALO_HOST, valo_server.length() + 1);
    //VALO_HOST = (valo_server.c_str());
    //VALO_HOST[13]='\0';

    save();
    beep();
    server.send(200, "application/json", "{success: restart your iot-surfboard}" );
  });
}
