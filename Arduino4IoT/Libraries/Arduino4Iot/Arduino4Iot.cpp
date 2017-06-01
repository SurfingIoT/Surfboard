#include "Arduino4Iot.h"
#include <Arduino.h>
#include <Servo.h> 
#include <NewPing.h>
#include <EEPROM.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"
#include <SeeedOLED.h>
#include <Adafruit_NeoPixel.h>
#include "SparkFunMPL3115A2.h"
#include <SeeedOLED.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "SparkFunMPL3115A2.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

Arduino4Iot board;
DS1307RTC clock;  //define a object of DS1307 class
ClosedCube_HDC1080 hdc1080;
Servo servo_1; 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(11, 8, NEO_GRB + NEO_KHZ800);
#define Addr 0x39 // I2C address of the TSL2571 0x39(57)
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
MPL3115A2 myPressure;

int lastRed=0;
int lastGreen=0;
int lastBlue=0;



String ADXL345_read(void) {
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
  double acceleration_X = event.acceleration.x ;
  double acceleration_Y = event.acceleration.y ;
  double acceleration_Z = event.acceleration.z ;
  SeeedOled.setTextXY(4,0); //Set the cursor to row 4, Colum 0  
  SeeedOled.putString("                ");  //clear line
  SeeedOled.setTextXY(4,0); //Set the cursor to row 4, Colum 0  
  SeeedOled.putString("X:");  
  SeeedOled.putNumber(acceleration_X);  //Print value of X
  SeeedOled.setTextXY(4,5); //Set the cursor to row 4, Colum 5  
  SeeedOled.putString("Y:");
  SeeedOled.putNumber(acceleration_Y);  //Print value of Y
  SeeedOled.setTextXY(4,10);  //Set the cursor to row 4, Colum 10  
  SeeedOled.putString("Z:"); 
  SeeedOled.putNumber(acceleration_Z);  //Print value of Z
  Serial.print(acceleration_X);
  Serial.print(",") ;
  Serial.print(acceleration_Y);
  Serial.print(",") ;
  Serial.print(acceleration_Z);
  return "" ;
}  

void init_TSL2571(){
    Wire.beginTransmission(Addr);   // Start I2C Transmission
    Wire.write(0x00 | 0x80);        // Select enable register
    Wire.write(0x01);               // Set power on, ALS enabled
    Wire.endTransmission();         // Stop I2C Transmission
    delay(500);
    Wire.beginTransmission(Addr);   // Start I2C Transmission
    Wire.write(0x00 | 0x80);        // Select enable register
    Wire.write(0x0B);               // Set power on, ALS enabled
    Wire.endTransmission();         // Stop I2C Transmission
    Wire.beginTransmission(Addr);   // Start I2C Transmission
    Wire.write(0x01 | 0x80);        // Select ALS time register
    Wire.write(0xFF);               // Atime = 2.72 ms, max count = 1023
    Wire.endTransmission();         // Stop I2C Transmission
    Wire.beginTransmission(Addr);   // Start I2C Transmission
    Wire.write(0x03 | 0x80);        // Select Wait time register
    Wire.write(0xFF);               // Wtime = 2.72 ms
    Wire.endTransmission();         // Stop I2C Transmission
    Wire.beginTransmission(Addr);   // Start I2C Transmission
    Wire.write(0x0F | 0x80);        // Select control register
    Wire.write(0x20);               // Gain = 1x
    Wire.endTransmission();         // Stop I2C Transmission
    delay(300);
}

int TSL2571_read(){
    unsigned int data[4];
    Wire.beginTransmission(Addr); // Start I2C Transmission
    Wire.write(0x14 | 0x80);  // Select data register
    Wire.endTransmission(); // Stop I2C Transmission
    Wire.requestFrom(Addr, 4);  // Request 4 bytes of data
    if(Wire.available() == 4){  // Read 4 bytes of data
      data[0] = Wire.read();    // c0Data lsb, c0Data msb, c1Data lsb, c1Data msb
      data[1] = Wire.read();
      data[2] = Wire.read();
      data[3] = Wire.read();
    }
    // Convert the data
    int c0Data = (data[1] * 256) + data[0];
    int c1Data = (data[3] * 256) + data[2];
    float CPL = (2.72 * 1.0) / 53.0;
    float luminance1 = (1 * c0Data - 2.0 * c1Data) / CPL;
    float luminance2 = (0.6 * c0Data - 1.00 * c1Data) / CPL;
    float luminance = 0.0;
    if((luminance1 > 0) && (luminance1 > luminance2)){
      luminance = luminance1;
    }
    else if((luminance2 > 0) && (luminance2 > luminance1)){
      luminance = luminance2;
    }
    SeeedOled.setTextXY(0,9);       //Set the cursor to row 0 , Colum 9  
    SeeedOled.putString("       "); //clear field
    SeeedOled.setTextXY(0,9);       //Set the cursor to row 0 , Colum 9  
    SeeedOled.putNumber(luminance);  //Print luminosity value
    SeeedOled.setTextXY(1,9);       //Set the cursor to row 1 , Colum 9  
    SeeedOled.putString("Lux");         //Print Lux label
    return int(luminance);
}

//********** metodos Arduino4Iot **********

void Arduino4Iot::setup(int baudRate) {
  bool parse=false;
  bool config=false;
  Serial.begin(baudRate);
  pinMode(RELAY, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
  hdc1080.begin(0x40);
  pixels.begin();     // This initializes the NeoPixel library.
  init_TSL2571();
  SeeedOled.init();   //initialze SEEED OLED display
  SeeedOled.clearDisplay();           //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();       //Set display to Normal mode
  SeeedOled.setPageMode();            //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0,0);           //Set the cursor row 0, Column 0  
  SeeedOled.putString("  Arduino 4Iot  ");
  accel.begin();
  delay(1000);
  SeeedOled.setTextXY(0,0);           //Set the cursor row 0, Column 0  
  SeeedOled.putString("                ");
  
  myPressure.begin(); // Get pressure sensor online
  
  myPressure.setModeAltimeter(); //Configure sensor to Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Configure sensor to Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
  
}

//sensors
float Arduino4Iot::altitude() {
  myPressure.setModeAltimeter();  // set sensor to altimeter mode (Pascals from 20 to 110 kPa)
  delay(400);
  myPressure.readAltitude();      //read altitude
  delay(400);
  SeeedOled.setTextXY(2,10);      //Set the cursor to row 2, Column 10  
  SeeedOled.putString("      ");  //clear field
  SeeedOled.setTextXY(2,10);      //Set the cursor to row 2, Column 10  
  SeeedOled.putNumber(myPressure.readAltitude());  //Print altitude
  SeeedOled.setTextXY(3,10);      //Set the cursor to row 3, Column 10  
  SeeedOled.putString("m");       
  return myPressure.readAltitude();
}

float Arduino4Iot::pressure() {
  myPressure.setModeBarometer(); // set sensor to pressure mode (above sea level in meters)
  delay(400);
  myPressure.readPressure();
  delay(400);
  SeeedOled.setTextXY(2,0);           //Set the cursor to row 2, Column 0  
  SeeedOled.putString("          ");  //clear field
  SeeedOled.setTextXY(2,0);           //Set the cursor to row 2, Column 0  
  SeeedOled.putNumber(myPressure.readPressure());      //Print pressure
  SeeedOled.setTextXY(3,0);           //Set the cursor to row 3, Column 0  
  SeeedOled.putString("Pa");          
  return myPressure.readPressure();
}

float Arduino4Iot::temperature() {
  SeeedOled.setTextXY(0,0);           //Set the cursor to row 0, Column 0  
  SeeedOled.putString("    ");        //clear field
  SeeedOled.setTextXY(0,0);           //Set the cursor to row 0, Column 0  
  SeeedOled.putNumber(hdc1080.readTemperature());   //Print pressure
  SeeedOled.setTextXY(1,0);           //Set the cursor to row 1, Column 0  
  SeeedOled.putString("oC");          
  return hdc1080.readTemperature();
}

float Arduino4Iot::humidity() {
  SeeedOled.setTextXY(0,5);           //Set the cursor to row 0, Column 5  
  SeeedOled.putString("   ");         //clear field
  SeeedOled.setTextXY(0,5);           //Set the cursor to row 0, Column 5  
  SeeedOled.putNumber(hdc1080.readHumidity());     //Print humidity
  SeeedOled.setTextXY(1,5);           //Set the cursor to row 0, Column 5  
  SeeedOled.putString("%");           
  return hdc1080.readHumidity();
}

int Arduino4Iot::alcohol() {
    SeeedOled.setTextXY(5,0);       //Set the cursor to row 5 , Colum 0  
    SeeedOled.putString("     "); //clear field
    SeeedOled.setTextXY(5,0);       //Set the cursor to row 5 , Colum 0  
    SeeedOled.putNumber(analogRead(ALCOHOL));  //Print luminosity value
    SeeedOled.setTextXY(6,0);       //Set the cursor to row 6 , Colum 0  
    SeeedOled.putString("ppm");     
  return analogRead(ALCOHOL);
}

int Arduino4Iot::potentiometer() {
  SeeedOled.setTextXY(7,0); //Set the cursor to row 7, Column 0  
  SeeedOled.putString("          ");           //clear row
  SeeedOled.setTextXY(7,0); //Set the cursor to row 7, Column 0  
  for (int i=0;i<(map(analogRead(POTENCIOMETER),0,1023,0,9));i++){
    SeeedOled.putString(">");    
  }
  return analogRead(POTENCIOMETER);
}

int Arduino4Iot::light() {
  return TSL2571_read();
}

String Arduino4Iot::accelerometer() {
  return ADXL345_read();
}

int Arduino4Iot::distance() {
  unsigned int uS = sonar.ping(); 
  return uS / US_ROUNDTRIP_CM;
}

//actuators
void Arduino4Iot::initServo() {
  servo_1.attach(SERVO);  
}

void Arduino4Iot::servo(int pos) {
  servo_1.write(pos);
}

void Arduino4Iot::relay(boolean b) {
  digitalWrite(RELAY, b);
}

void Arduino4Iot::speaker(boolean b) {
  digitalWrite(SPEAKER, b);
}

void Arduino4Iot::buzzer(int frequency,int duration) {
//tone(port,frequency,duration(ms));
  tone(5,frequency,duration);
}

void Arduino4Iot::red(int b) {
  lastRed=b;
//  for(int i=NUMPIXELS;i>=0;i--){  //all LEDs
    int i=0;  //just the first LED
    pixels.setPixelColor(i, pixels.Color(b,lastGreen,lastBlue)); // Bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
//   delay(50); // Delay for a period of time (in milliseconds).
//  }
}

void Arduino4Iot::green(int b) {
  lastGreen=b;
//  for(int i=NUMPIXELS;i>=0;i--){  //all LEDs
    int i=0;  //just the first LED
    pixels.setPixelColor(i, pixels.Color(lastRed,b,lastBlue)); // Bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
// delay(50); // Delay for a period of time (in milliseconds).
//  }
}

void Arduino4Iot::blue(int b) {
  lastBlue = b;
//  for(int i=NUMPIXELS;i>=0;i--){  //all LEDs
    int i=0;  //just the first LED
    pixels.setPixelColor(i, pixels.Color(lastRed,lastGreen,b)); // Bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
//    delay(50); // Delay for a period of time (in milliseconds).
//  }
}

void Arduino4Iot::rgb(int r, int g, int b) {
  lastRed=r;
  lastGreen=g;
  lastBlue=b;
  for(int i=NUMPIXELS;i>=0;i--){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(r,g,b)); // Bright all colors according with received command.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(50); // Delay for a period of time (in milliseconds).
  }
}

void Arduino4Iot::RGB(int r, int g, int b, char* dir, int firstLed, int lastLed) {
  lastRed=r;
  lastGreen=g;
  lastBlue=b;
//  Serial.println(dir);
  if (dir[0] == 'r'){
    for(int i=firstLed ; i<= lastLed;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(r,g,b)); // Bright all colors on defined LEDs according with received command on right direction (CW)
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(50); // Delay for a period of time (in milliseconds).
    }
  }
  if (dir[0] == 'l'){
    for(int i=firstLed;i>=lastLed;i--){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(r,g,b)); // Bright all colors on defined LEDs according with received command on left direction (CCW)
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(50); // Delay for a period of time (in milliseconds).
    }
  }
}

void Arduino4Iot::blink(char led, int f, int i) {
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

void Arduino4Iot::oled(int row, int col, char* text) {
  SeeedOled.setTextXY(col,row);           //Set the cursor  
  SeeedOled.putString(text);           //Print number
}

void Arduino4Iot::printJSON(char* n, char* v) {
  Serial.print("{");
  Serial.print(n);
  Serial.print(":\"");
  Serial.print(v);
  Serial.println("\"}");
}
void Arduino4Iot::printJSON(char* n, int v1) {
  Serial.print("{");
  Serial.print(n);
  Serial.print(":\"");
  Serial.print(v1);
  Serial.println("\"}");
}


//WRAPPER Functions

//sensors
char* accelerometer(char* args) {
  Serial.print(board.accelerometer());
  return "";
}

char* light(char* args) {
  Serial.print(board.light());
  return "";
}

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

char* altitude(char* args) {
  Serial.print(board.altitude());
  return "";
}

char* pressure(char* args) {
  Serial.print(board.pressure());
  return "";
}

//actuators
char* red(char* args) {
  board.red(atoi(args));
  return "";
}

char* green(char* args) {
  board.green(atoi(args));
  return "";
}

char* blue(char* args) {
  board.blue(atoi(args));
  return "";
}

char* rgb(char* args) {
  char *r ;
  char *g ;
  char *b ;
  r = strtok(args, ",");               //let's pars the args at each comma
  g = strtok(NULL, ",");               //let's pars the args at each comma
  b = strtok(NULL, ",");               //let's pars the args at each comma
  board.rgb(atoi(r),atoi(g),atoi(b));
  return "";
}

char* RGB(char* args) {
  char *r ;
  char *g ;
  char *b ;
  char *dir ;
  char *first ;
  char *last ;
  
  r = strtok(args, ",");               //let's pars the args at each comma
  g = strtok(NULL, ",");               //let's pars the args at each comma
  b = strtok(NULL, ",");               //let's pars the args at each comma
  dir = strtok(NULL, ",");               //let's pars the args at each comma
  first = strtok(NULL, ",");               //let's pars the args at each comma
  last = strtok(NULL, ",");               //let's pars the args at each comma
  
//  board.RGB(atoi(r),atoi(g),atoi(b),atoi(d));
  board.RGB(atoi(r),atoi(g),atoi(b),dir, atoi(first), atoi(last));
  return "";
}

char* oled(char* args) {
  char *row ;
  char *col ;
  char *text ;
  row = strtok(args, ",");               //let's pars the args at each comma
  col = strtok(NULL, ",");               //let's pars the args at each comma
  text = strtok(NULL, ",");               //let's pars the args at each comma
  board.oled(atoi(col),atoi(row),text);
  return "";
}

char* buzzer(char* args) {
  char *freq ;
  char *time ;
  freq = strtok(args, ",");               //let's pars the args at each comma
  time = strtok(NULL, ",");               //let's pars the args at each comma
  board.buzzer(atoi(freq),atoi(time));
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

//Date and Time
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

void Arduino4Iot::clockAdjustment() {
    if (getDate(__DATE__) && getTime(__TIME__)) {
    //parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      //config = true;
    }
  }
}

void Arduino4Iot::printMode(int mode){
  SeeedOled.setTextXY(6,15);      //Set the cursor to row 6, Column 15  
  SeeedOled.putString("M");  //clear field
  SeeedOled.setTextXY(7,15);      //Set the cursor to row 7, Column 15  
  SeeedOled.putNumber(mode);  //Print mode
}