#include <Arduino.h>
#include <Servo.h> 
#include <NewPing.h>
#include <DHT.h>
#include <EEPROM.h>
#include <DHT.h>
#include <NewPing.h>
#include <Servo.h> 
#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>
#include "IoTSurfboard.h"


IoTSurfboard board;
DS1307RTC clock;//define a object of DS1307 class

Servo servo_1; 
DHT dht(DHTPIN, DHTTYPE);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

void IoTSurfboard::blink(char led, int f, int i) {
  for(int x=0;x<f;x++) {
    if(led=='R') board.red(255);
    else if(led=='G') board.green(255);
    else if(led=='B') board.blue(255);
    delay(i);
    if(led=='R') board.red(0);
    else if(led=='G') board.green(0);
    else if(led=='B') board.blue(0);
    delay(i);
  }
}

void IoTSurfboard::setup(int baudRate) {
  bool parse=false;
  bool config=false;
  Serial.begin(baudRate);
  pinMode(RELAY, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  dht.begin();
}
void IoTSurfboard::initServo() {
  servo_1.attach(SERVO);  
}  
float IoTSurfboard::temperature() {
  return dht.readTemperature();
}
float IoTSurfboard::temperatureC() {
  return dht.readTemperature();
}
float IoTSurfboard::temperatureF() {
  return dht.readTemperature(true);
}
float IoTSurfboard::humidity() {
  return dht.readHumidity();
}
int IoTSurfboard::alcohol() {
  return analogRead(ALCOHOL);
}
int IoTSurfboard::potentiometer() {
  return analogRead(POTENCIOMETER);
}
int IoTSurfboard::light() {
  return analogRead(LDR);
}
void IoTSurfboard::relay(boolean b) {
  digitalWrite(RELAY, b);
}
void IoTSurfboard::speaker(boolean b) {
  digitalWrite(SPEAKER, b);
}

void IoTSurfboard::red(int b) {
  analogWrite(10, b);
}
void IoTSurfboard::green(int b) {
  analogWrite(6, b);
}
void IoTSurfboard::blue(int b) {
  analogWrite(5, b);
} 
void IoTSurfboard::rgb(int r, int g, int b) {
  red(r);
  green(g);
  blue(b);
}
void IoTSurfboard::printJSON(char* n, char* v) {
  Serial.print("{");
  Serial.print(n);
  Serial.print(":\"");
  Serial.print(v);
  Serial.println("\"}");
}
void IoTSurfboard::printJSON(char* n, int v1) {
  Serial.print("{");
  Serial.print(n);
  Serial.print(":\"");
  Serial.print(v1);
  Serial.println("\"}");
}

int IoTSurfboard::distance() {
  unsigned int uS = sonar.ping(); 
  return uS / US_ROUNDTRIP_CM;
}

void IoTSurfboard::servo(int pos) {
  servo_1.write(pos);
}

//WRAPER

char* humidity(char* args) {
  Serial.print(board.humidity());
  return "";
}
char* distance(char* args) {
  Serial.print(board.distance());
  return "";
}

char* temperature(char* args) {
  Serial.print(board.temperature());
  return "";
}

char* servo1(char* args) {
  int pos = atoi(args);
  board.servo(pos);
  return args;
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

char* rtc(char* args) {
  tmElements_t tm;
  if (RTC.read(tm)) {
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.print(' ');
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);

  } else {
    if (RTC.chipPresent()) {
      Serial.print("RTC-ERROR1");

    } else {
      Serial.print("RTC-ERROR2");

    }
  }
  return "";
}

//Date and Time adjustment

tmElements_t tm;
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}
bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}


void IoTSurfboard::clockAdjustment() {

    if (getDate(__DATE__) && getTime(__TIME__)) {
    //parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      //config = true;
    }
  }
}
