#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance
#include <Thing.h>
#include <Arduino4Iot.h>

void setup() {
  board.setup(9600);  
  //myBlink();  //rgb leds test
  //board.clockAdjustment();  
  things.add("transistor",DIGITAL,  6);
  things.add("relay",     DIGITAL,  4);
  things.add("speaker",   DIGITAL,  5);
  things.add("alcohol",   ANALOG,   0);
  things.add("pot",       ANALOG,   1);
  things.add("distance",  CUSTOM,   distance, true);
  things.add("clock",     CUSTOM,   rtc, true);
  things.add("temp",      CUSTOM,   temperature, true);
  things.add("humidity",  CUSTOM,   humidity, true);
  things.add("current",   CUSTOM,   currentOnly, true);
  things.add("red",       CUSTOM,   red   , false);
  things.add("green",     CUSTOM,   green   ,false);
  things.add("blue",      CUSTOM,   blue   ,false);
  things.add("rgb",       CUSTOM,   rgb   ,false);
  things.add("RGB",       CUSTOM,   RGB   ,false);
  things.add("light",     CUSTOM,   light  ,true);
  things.add("servo",     CUSTOM,   servo1, false);
  things.add("display",   CUSTOM,   oled, false);
  things.add("buzzer",    CUSTOM,   buzzer, false);
  things.add("accel",     CUSTOM,   accelerometer  ,true);
  things.add("altitude",  CUSTOM,   altitude  ,true);
  things.add("pressure",  CUSTOM,   pressure ,true);
  

  emon1.current(7, 17.14);             // Current: input pin, calibration.
  board.initServo();  
  
  things.mode(1, myBlink);  
  things.mode(2, light_test);
  things.mode(3, temperature_test);
  things.mode(4, humidity_test);
  things.mode(5, alcohol_test);
  things.start(1);
}

void loop() {
  things.loop();
  if (things.currentMode == 0){
    board.printMode(0);
  }
}

