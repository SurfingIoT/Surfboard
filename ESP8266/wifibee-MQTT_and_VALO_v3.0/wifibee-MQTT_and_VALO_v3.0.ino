#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include <ESP8266HTTPClient.h>

char CONTRIBUTOR_ID[] = "SurfBoardXX";
char VALO_HOST[] = "192.168.0.110";
int  VALO_PORT = 8888;
char LATITUDE[] = "36.7585406465564";
char LONGITUDE[] = "-4.3971722687";

String commandString = "";         // a string to hold incoming data
boolean commandComplete = false;  // whether the string is complete
char ssid_factory[] = "surfing-iot";
char password_factory[] = "iotiotiot";
String mqtt_server_factory = "iot.eclipse.org";

String ssid = "iot-mobile";
String password = "iotiotiot";
String mqtt_server = "iot.eclipse.org";
String valo_server = "";
String opMode = "MQTT";

String queue_listener;
String queue_sensors;
String queue_broadcast = "/iot-surfboard";
String queue_rendezvous;
String deviceName = "surfboard";

long sensor_interval = 15000;
long connection_timeout = 0;
WiFiClient espClient;
PubSubClient client(espClient);
ESP8266WebServer server(80);

long lastMsg = 0;
char buffer[1000];
int wifi_state = 0;
HTTPClient http;

void setup() {
  EEPROM.begin(512);
  Serial.begin(9600);
  Serial.setTimeout(1000);
  Serial.print("start");
  emptySerial();
  emptySerial();
  delay(5000);
  emptySerial();
  Serial.print("name");   //request Surfboard Id
  //delay(500);
  Serial.flush();
  deviceName = Serial.readString();
  if (deviceName.length() == 0 || deviceName.startsWith(" ")) {
    deviceName = "surfboard-unnamed";
  }
  else {
    //taking out the line termination to avoid problem with MQTT QUEUE names
    deviceName.replace("\n", "");
    deviceName = deviceName.substring(0, deviceName.length() - 1);
  }
  Serial.print("Device name: ");
  Serial.println(deviceName.length());
  Serial.println(deviceName);
  emptySerial();
  delay(500);
  queue_rendezvous = "/iot-surfboard/" + deviceName;
  queue_listener = "/iot-surfboard/control/" + deviceName;
  queue_sensors  = "/iot-surfboard/sensors/" + deviceName;

  read_memory();  //request config parameters from EEPROM

  Serial.print("red?255");
  connection_timeout = millis();


  //**********************  add by Jose luiz  **********************
  WiFi.hostname(deviceName);  //define a hostname for  ESP8266 config LAN
  const String ssid_AP = deviceName;
  const char *password_AP = "surfboard";
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid_AP.c_str(), password_AP);  //start a managment WiFi network "SurfBoardXX"
  createWebServer(1);
  server.begin();
  //*********************************************************

  setup_wifi();
  while (WiFi.status() != WL_CONNECTED) {
    beep();
    wifi_state = 1;
    connection_timeout = millis();
    setup_wifi();
  }

}


void loop() {
  server.handleClient();

  if (opMode == "MQTT") { //publishes data to MQTT

    if (!client.connected() && wifi_state == 0) { //if MQTT disconected and WiFi state conected
      wifi_state = 1;
      reconnect();  //try to reconnect MQTT
    }
    else if (!client.connected() && wifi_state == 1) {  //if MQTT disconected and WiFi state disconected
      connection_timeout = millis();
      setup_wifi(); //reconect WiFi
      reconnect();  //reconect to MQTT
    }

    client.loop();
    long now = millis();
    if (sensor_interval > 0 && now - lastMsg > sensor_interval) {
      lastMsg = now;
      Serial.print("sensors");
      Serial.flush();
      String sensors = Serial.readString();
      toChar(sensors);
      client.publish(queue_sensors.c_str(), buffer);
      //debug(buffer);
      emptySerial();
      //client.publish(queue_sensors_id.c_str(), buffer);
    }
    while (Serial.available()) {
      char inChar = (char)Serial.read();
      commandString += inChar;
      if (inChar == '\n') {
        commandComplete = true;
      }
    }
    if (commandComplete && commandString.length() > 0 && !commandString.equals("")) {
      debug("execute command serial " + commandString);
      executeCommand();
      commandComplete = false;
    }
  }


  if (opMode == "VALO") { //publishes data to Valo

    if (WiFi.status() != WL_CONNECTED) { //if MQTT disconected and WiFi state conected
      wifi_state = 0;
      connection_timeout = millis();
      setup_wifi(); //reconect WiFi
    }

    long now = millis();
    if (sensor_interval > 0 && now - lastMsg > sensor_interval) {
      lastMsg = now;
      conectClient();
      Serial.print("clock");
      Serial.flush();
      String clock = Serial.readString();

      //Read Sensors and send to Valo
      //Temperature
      Serial.print("temp");
      Serial.flush();
      String sensor = Serial.readString();
      sensor.replace("\n", "");
      httpPOST("temperature", sensor, "celsius", clock);

      Serial.print("alcohol");
      Serial.flush();
      sensor = Serial.readString();
      sensor.replace("\n", "");
      httpPOST("alcohol", sensor, "analog", clock);

      Serial.print("light");
      Serial.flush();
      sensor = Serial.readString();
      sensor.replace("\n", "");
      httpPOST("light", sensor, "analog", clock);

      Serial.print("humidity");
      Serial.flush();
      sensor = Serial.readString();
      sensor.replace("\n", "");
      httpPOST("humidity", sensor, "porcentage", clock);

      Serial.print("distance");
      Serial.flush();
      sensor = Serial.readString();
      sensor.replace("\n", "");
      httpPOST("distance", sensor, "cm", clock);

      disconectClient();

    }
  }
}

int httpPOST(String sensorName, String sensorValue, String unit, String clock) {
  String json = "{\"contributor\" :\"" + deviceName + "\", " +
                "\"position\" :{\"latitude\":\"" + LATITUDE +
                "\", \"longitude\": \"" + LONGITUDE + "\"}, " +
                "\"time_stamp\" :\"" + clock + "\"," +
                "\"" + sensorName + "\" :" + sensorValue + ", " +
                "\"units\" :\"" + unit + "\"}";
  String url = "http://" + valo_server + ":" + VALO_PORT + "/streams/demo/iot_board/" + sensorName;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(json);
  return httpCode;
}


/*espClient.println("POST /streams/demo/iot_board/temperature HTTP/1.1");
  espClient.print("HOST ");
  espClient.print(valo_server);
  espClient.print(":");
  espClient.println(VALO_PORT);
  espClient.println("Content-Type: application/json");*/                         // JSON content type
/*espClient.print("{\"contributor\" :\""); espClient.print(deviceName);  espClient.print("\", ");
  espClient.print("\"position\" :{\"latitude\":"); espClient.print(LATITUDE);
  espClient.print("\", \"longitude\":"); espClient.print(LONGITUDE); espClient.print("}, ");
  espClient.print("\"time_stamp\" :\"bla\",");
  espClient.print("\"temperature\" :\""); espClient.print(tempRead);espClient.print(", ");
  espClient.print("\"units\" :\"celsius\"}");*/
/*espClient.print("Content-Length: ");
  espClient.println(stringao.length()); // Content length
  espClient.println(stringao);*/

