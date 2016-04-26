#include "Arduino.h"
#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>

#define BUTTON 		2
#define RELAY 		4
#define BLUE 		5 
#define GREEN 		6
#define RED 10
#ifdef SURFBOARDBETA  
 #define RED 5
#endif

#define SPEAKER 	16 //A2 Analog 2
#define DHTPIN 		8     
#define DHTTYPE 	DHT11   
#define ALCOHOL 	0
#define POTENCIOMETER 	1
#define LDR 		3
#define TRIGGER_PIN  	13 //sonar 
#define ECHO_PIN     	12 //sonar 
#define MAX_DISTANCE 	200 
#define SERVO		9
#define IR_RECEIVER1	7


//WRAPER FUNCTIONS
char* servo1(char* );
char* temperature(char*);
char* humidity(char* args);
char* distance(char* args);
char* rtc(char* args);



class IoTSurfboard {

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
  float temperatureC();
  float temperatureF();
  float humidity();
  void relay(boolean);
  void speaker(boolean);
  void red(int);
  void green(int);
  void blue(int);
  void rgb(int, int, int);
  void servo(int);  
  int distance();
  void printJSON(char*, char*);
  void printJSON(char*, int);
};

extern IoTSurfboard board;

