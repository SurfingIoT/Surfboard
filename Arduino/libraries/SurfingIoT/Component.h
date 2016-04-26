#ifndef COMPONENT_H
#define COMPONENT_H
#include "Arduino.h"
#define DIGITAL 0
#define ANALOG 	1
#define PWM 	2

#define RELAY   3
#define LIGHT   4
#define TEMP    5
#define SERIAL 	6

#define PING    7

#define ALL     8
#define CUSTOM  9
class Component  {
 private:
   typedef char* (*customFunction)(char*);
   customFunction myCustomFunction;
   
  public:    
      char* name;
      int   type;
      int   port;
      int   state;
      boolean isSensor;
      Component(char*, int, int);
      Component(char*, int, customFunction);
      Component(char*, int, customFunction, boolean);
      Component();

      char* write(char *);
      char* read();
      char* getTypeName();
      char* getValue();
      void emptyReadValue();
};
#endif



