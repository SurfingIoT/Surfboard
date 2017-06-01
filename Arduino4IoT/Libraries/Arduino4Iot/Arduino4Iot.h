#include "Arduino.h"
#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>

#define BUTTON 		3
#define RGBLED		8
#define NUMPIXELS      11
#define RELAY 		4
#define SPEAKER 	5 
#define ALCOHOL 	0
#define POTENCIOMETER 	1
#define TRIGGER_PIN  	11 //sonar PING 
#define ECHO_PIN     	12 //sonar PING
#define MAX_DISTANCE 	200 
#define SERVO		9
#define IR_RECEIVER1	7


//WRAPPER FUNCTIONS for sensors
char* temperature(char*);
char* humidity(char* args);
char* distance(char* args);
char* rtc(char* args);
char* light(char* args);
char* accelerometer(char* args);
char* altitude(char* args);
char* pressure(char* args);

//WRAPPER FUNCTIONS for devices
char* servo1(char* );
char* red(char* );
char* green(char* );
char* blue(char* );
char* rgb(char* );
char* RGB(char* );
char* oled(char* );
char* buzzer(char* );


class Arduino4Iot {
private:
public:
  void clockAdjustment();
  void blink(char led, int f, int i);
  void setup(int baudRate);
  int alcohol();
  int light();
  int potentiometer();
  void initServo();
  float temperature();
//  float temperatureC();
//  float temperatureF();
  float humidity();
  float altitude();
  float pressure();
  int distance();
  String accelerometer();
  
  void relay(boolean);
  void speaker(boolean);
  void red(int);
  void green(int);
  void blue(int);
  void rgb(int, int, int);
  void RGB(int, int, int, char*, int, int);
  void oled(int, int, char*);
  void servo(int); 
  void buzzer(int,int);
  void printMode(int);
  
  void printJSON(char*, char*);
  void printJSON(char*, int);
};

extern Arduino4Iot board;
