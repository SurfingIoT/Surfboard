#include <EEPROM.h>
#include <DHT.h>
#include <NewPing.h>
#include <Servo.h> 
#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>

#include <SurfThing.h>
#include <IoTSurfboard.h>

void setup() {
  board.setup(9600);  
  board.clockadj();
  system.add("transistor",DIGITAL,      11);
  system.add("red",       DIGITAL,      10);
  system.add("green",     PWM,      6);
  system.add("blue",      PWM,      5);
  system.add("relay",     DIGITAL,  4);
  system.add("speaker",   DIGITAL,  16);//A2
  system.add("alcohol",   ANALOG,   0);
  system.add("pot",       ANALOG,   1);
  system.add("light",     ANALOG,   3);
  system.add("servo",     CUSTOM,   servo1, false);
  system.add("distance",  CUSTOM,   distance, true);
  system.add("clock",     CUSTOM,   rtc, true);
  system.add("temp",      CUSTOM,   temperature, true);
  system.add("humidity",  CUSTOM,   humidity, true);
  
  system.mode(1, myBlink);
  system.mode(2, light_test);
  system.mode(3, temperature_test);
  system.mode(4, humidity_test);
  system.mode(5, alcohol_test);
  system.start();


}


void loop() {
    system.loop();
}
