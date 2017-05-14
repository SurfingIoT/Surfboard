#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>

// Internal dependencies
#include <SensorSample.h>
#include <Contributor.h>
using namespace io::valo::iot;

char CONTRIBUTOR_ID[] = "SurfBoardXX";
char VALO_HOST[] = "192.168.10.55";
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

// Contributor
//Contributor<Print> con(client, "3452352345", "localhost", 8888);
//Contributor<Print> con(Serial, CONTRIBUTOR_ID, VALO_HOST, VALO_PORT);
Contributor<Print> con(espClient, CONTRIBUTOR_ID, VALO_HOST, VALO_PORT);

// Sensors
SensorSample temp("/streams/demo/iot_board/temperature", "temperature", "celsius");
SensorSample humidity("/streams/demo/iot_board/humidity", "humidity", "percentage");
SensorSample distance("/streams/demo/iot_board/distance", "distance", "meter");
SensorSample luminance("/streams/demo/iot_board/luminance", "luminance", "lumex");
SensorSample alcohol("/streams/demo/iot_board/alcohol", "alcohol", "vol");

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

  // Set the sensor static data
  temp.setPosition(LATITUDE, LONGITUDE);
  humidity.setPosition(LATITUDE, LONGITUDE);
  distance.setPosition(LATITUDE, LONGITUDE);
  luminance.setPosition(LATITUDE, LONGITUDE);
  alcohol.setPosition(LATITUDE, LONGITUDE);
  
}


void loop() {
  server.handleClient();

  if (opMode == "MQTT"){  //publishes data to MQTT

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

  
  if (opMode == "VALO"){  //publishes data to Valo

    if (WiFi.status() != WL_CONNECTED) { //if MQTT disconected and WiFi state conected
      wifi_state = 0;
      connection_timeout = millis();
      setup_wifi(); //reconect WiFi
    }

    long now = millis();
    if (sensor_interval > 0 && now - lastMsg > sensor_interval) {
      lastMsg = now;

  
      //Read Sensors and send to Valo  
      //Temperature
      Serial.print("temp");
      Serial.flush();
      String tempRead = Serial.readString();
      toChar(tempRead);
      tempRead.toCharArray(buffer, sizeof(tempRead));
      temp.setValue(buffer);
      conectClient();
      con.feed(temp);
      disconectClient();

      //Humidity
      Serial.print("humidity");
      Serial.flush();
      String humidityRead = Serial.readString();
      toChar(humidityRead);
      humidityRead.toCharArray(buffer, sizeof(humidityRead));
      humidity.setValue(buffer);
      conectClient();
      con.feed(humidity);
      disconectClient();
  
      //Distance
      Serial.print("distance");
      Serial.flush();
      String distanceRead = Serial.readString();
      toChar(distanceRead);
      distanceRead.toCharArray(buffer, sizeof(distanceRead));
      distance.setValue(buffer);
      conectClient();
      con.feed(distance);
      disconectClient();

      //Luminance
      Serial.print("light");
      Serial.flush();
      String luminanceRead = Serial.readString();
      toChar(luminanceRead);
      luminanceRead.toCharArray(buffer, sizeof(luminanceRead));
      luminance.setValue(buffer);
      conectClient();
      con.feed(luminance);
      disconectClient();

      //Alcohol
      Serial.print("alcohol");
      Serial.flush();
      String alcoholRead = Serial.readString();
      toChar(alcoholRead);
      alcoholRead.toCharArray(buffer, sizeof(alcoholRead));
      alcohol.setValue(buffer);
      conectClient();
      con.feed(alcohol);
      disconectClient();

    }
  }
}

