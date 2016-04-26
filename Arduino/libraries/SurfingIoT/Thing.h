//#ifndef THING_H
//#define THING_H
#include "Component.h"
#include "Mode.h"
#include "Arduino.h"

#define MAX_COMPONENTS 20
#define MAX_MODES 10
#define I2C 99
#define DISCOVERY 0
#define BASIC 1
#define SENSORS 2

struct BoardID {
  long serial;
  int  id;
  long key;
};

class Thing {

private:
  bool found;
  int componentIndex;

public:

  Thing();
  BoardID boardID;
  void identification(boolean);
  int currentMode=0;
  int lastMode;
  int modeCounter=0;
  byte modeChanged1;
  Mode modes[MAX_MODES];
  int numberOfComponents=0;
  Component components[MAX_COMPONENTS];
  typedef void (*initFunc)();
  initFunc initFunction;
  int speakerPin = 16;
  void init(void(*initFunc)());
  void freeRam ();
  void discoverySerial();
  void start();
  void start(int noInt);
  void printConfig();
  void beep(int);
  void add(char*, int, int);
  void add(char*, int, char*(*componentFunction)(char *), boolean);
  void loop();
  void mode(int modeNumber, void(*modeFunction)());
  byte modeChanged();
  int firmware();
  void config();

  int findComponent(char*) ;
  void serialServer();
  char* read(char *);
  int   readInt(char *);
  void printComponents(int type);
  void data(int);
  void wait(long milis);
};

extern Thing system;
//#endif

