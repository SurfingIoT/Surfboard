#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
 
String commandString = "";         // a string to hold incoming data
boolean commandComplete = false;  // whether the string is complete
char ssid_factory[] = "surfing-iot";
char password_factory[] = "iotiotiot";
String mqtt_server_factory = "iot.eclipse.org";

String ssid = "iot-mobile";
String password = "iotiotiot";
String mqtt_server = "iot.eclipse.org";

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
    deviceName = "surfboard-hue";
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


  //**********************  adicionado por JL  **********************
  WiFi.hostname(deviceName);  //define um hostname para o ESP8266 usando o deviceName
  const String ssid_AP = deviceName;
  const char *password_AP = "surfboard";
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid_AP.c_str(), password_AP);  //cria a rede WiFi de configuracao do modulo WiFi
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

  if (!client.connected() && wifi_state == 0) {
    wifi_state = 1;
    reconnect();
  }
  else if (!client.connected() && wifi_state == 1) {
    connection_timeout = millis();
    setup_wifi();
    reconnect();
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

