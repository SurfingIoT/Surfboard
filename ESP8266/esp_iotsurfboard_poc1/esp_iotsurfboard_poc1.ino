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

String queue_listener = "/globalcode/thing";
String queue_broadcast = "globalcode/things";

String queue_sensors = "globalcode/things";
String queue_rendezvous = "/globalcode";
String queue_sensors_id;
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

  delay(5000);
  emptySerial();
  read_memory();
  Serial.print("name");
  delay(500);
  deviceName = Serial.readString();
  if (deviceName.length() == 0) {
    deviceName = "surfboard0";
  }
  Serial.print("Device name: ");
  Serial.println(deviceName);
  emptySerial();  
  delay(500);  
  queue_listener = queue_listener + "/" + deviceName;
  queue_sensors_id  = queue_sensors  + "/" + deviceName;
  read_memory();

  Serial.print("red?255");
  connection_timeout = millis();
  setup_wifi();
  while (WiFi.status() != WL_CONNECTED) {
    beep();
    wifi_state = 1;
    connection_timeout = millis();

    setup_wifi();
  }
}

void setup_wifi() {
  delay(100);
  if (ssid.length() > 0 && wifi_state == 0) {
    WiFi.begin(ssid.c_str(), password.c_str());
  }
  else {
    WiFi.begin(ssid_factory, password_factory);
  }
  delay(1500);
  //}
  Serial.print("red?0");
  Serial.print("blue?255");
  delay(500);
  //will try for 15 seconds...
  while (WiFi.status() != WL_CONNECTED && millis() - connection_timeout < 15000) {
    Serial.print("blue?0");
    delay(500);
    Serial.print("blue?255");
    delay(500);
  }
  Serial.print("blue?0");
  if (WiFi.status() != WL_CONNECTED) return;
  delay(500);
  Serial.print("green?255");
  client.setServer(mqtt_server_factory.c_str(), 1883);
  client.setCallback(callback);
  delay(1500);
  Serial.print("green?0");
  emptySerial();
  createWebServer(1);
  // Start the server
  server.begin();
}

void reconnect() {
  Serial.print("red?255");
  while (!client.connected()) {
    toChar(deviceName);
    if (client.connect(deviceName.c_str())) {
      toChar("new-device:" + deviceName);
      client.publish(queue_rendezvous.c_str(), buffer);
      //discovery();
      client.subscribe(queue_broadcast.c_str());
      client.subscribe(queue_listener.c_str());
      Serial.print("red?0");
    } else {
      Serial.print("red?255");
      delay(1000);
      Serial.print("red?0");
      delay(1000);
      Serial.print("red?255");
      delay(1000);
      Serial.print("red?0");
      delay(1000);
    }
  }
  emptySerial();
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  server.handleClient();
  long now = millis();
  if (sensor_interval > 0 && now - lastMsg > sensor_interval) {
    lastMsg = now;
    Serial.print("sensors");
    Serial.flush();
    String sensors = Serial.readString();
    toChar(sensors);
    debug(buffer);
    emptySerial();
    client.publish(queue_sensors.c_str(), buffer);
    client.publish(queue_sensors_id.c_str(), buffer);
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


void executeCommand() {
  debug("execute command " + commandString);
  if (commandString.indexOf("config-ssid") > -1) {
    String param = commandString.substring(commandString.indexOf("=") + 1, commandString.length());
    ssid = param;
    beep();
  }
  else if (commandString.indexOf("config-pwd") > -1) {
    String param = commandString.substring(commandString.indexOf("=") + 1, commandString.length());
    password = param;
    beep();
  }
  else if (commandString.indexOf("config-interval") > -1) {
    String param = commandString.substring(commandString.indexOf("=") + 1, commandString.length());
    sensor_interval = param.toInt();
    beep();
  }
  else if (commandString.indexOf("config-reconnect") > -1) {
    setup_wifi();
    beep();
  }
  else if (commandString.indexOf("config-discovery") > -1) {
    discovery();
    beep();
  }
  else if (commandString.indexOf("config-save") > -1) {
    save();
    beep();
  }
  else if (commandString.indexOf("config-restore-factory") > -1) {
    restore();
    beep();
  }
  else if (commandString.indexOf("*/")>-1) {
    String t = commandString.substring(2,commandString.length());
    Serial.print(t);
  }
  else {
    Serial.print(commandString);
    delay(100);
    emptySerial();
  }
  commandString = "";
  commandComplete = false;
}

void restore() {
  EEPROM.write(511, 0);
  EEPROM.commit();
}
void discovery() {
  Serial.print("?");
  Serial.flush();
  String r = Serial.readString();
  toChar(r);
  client.publish(queue_rendezvous.c_str(), buffer);
}


void save() {
  for (int x = 0; x < 512; x++) EEPROM.write(x, 0);
  EEPROM.write(511, 'I');
  for (int x = 0; x < ssid.length(); x++) {
    EEPROM.write(x, ssid[x]);
  }
  for (int x = 0; x < password.length(); x++) {
    EEPROM.write(x + 32, password[x]);
  }
  for (int x = 0; x < mqtt_server.length(); x++) {
    EEPROM.write(x + 64, mqtt_server[x]);
  }
  for (int x = 0; x < queue_listener.length(); x++) {
    EEPROM.write(x + 96, queue_listener[x]);
  }
  for (int x = 0; x < queue_sensors.length(); x++) {
    EEPROM.write(x + 128, queue_sensors[x]);
  }
  for (int x = 0; x < queue_broadcast.length(); x++) {
    EEPROM.write(x + 160, queue_broadcast[x]);
  }
  for (int x = 0; x < queue_rendezvous.length(); x++) {
    EEPROM.write(x + 192, queue_rendezvous[x]);
  }
  String temp = String(sensor_interval);
  for (int x = 0; x < temp.length(); x++) {
    EEPROM.write(x + 224, temp[x]);
  }
  EEPROM.commit();
}

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
  for (int i = 64; i < 96; i++)
  {
    mqtt_server += char(EEPROM.read(i));
  }
  queue_listener = "";
  for (int i = 96; i < 128; i++)
  {
    queue_listener += char(EEPROM.read(i));
  }
  queue_sensors = "";
  for (int i = 128; i < 160; i++)
  {
    queue_sensors += char(EEPROM.read(i));
  }
  queue_broadcast = "";
  for (int i = 160; i < 192; i++)
  {
    queue_broadcast += char(EEPROM.read(i));
  }
  queue_rendezvous = "";
  for (int i = 192; i < 224; i++)
  {
    queue_rendezvous += char(EEPROM.read(i));
  }
  String interval = "";
  for (int i = 224; i < 234; i++)
  {
    interval += char(EEPROM.read(i));
  }
  sensor_interval = atoi(interval.c_str());
  Serial.println("Finshed reading memory");
  Serial.println(ssid);
  Serial.println(password);
  Serial.println(queue_sensors);
  Serial.println(queue_broadcast);
  Serial.println(queue_rendezvous);
  Serial.println(sensor_interval);
  
  emptySerial();

}

void toChar(String s) {
  s.toCharArray(buffer, s.length() + 2);
}

void emptySerial() {
  //descartando a resposta...
  while (Serial.available()) {
    Serial.read();
  }
}

void debug(String msg) {
  toChar(msg);
  client.publish(queue_rendezvous.c_str(), buffer);
}

void beep() {
  Serial.print("speaker?1");
  Serial.flush();
  emptySerial();
  delay(700);
  Serial.print("speaker?0");
  Serial.flush();
  emptySerial();
  delay(100);
}
void beep(int interval) {
  Serial.print("speaker?1");
  Serial.flush();
  delay(interval);
  Serial.print("speaker?0");
  Serial.flush();
}



