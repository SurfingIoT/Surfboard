#include "Thing.h"
#include "Arduino.h" // Arduino 1.0 
#include "Component.h"
#include <EEPROM.h>

//#1 = BETA - changed Feb/2016 (Catarse / Presale)
//#2 = BETA2 - Started at jFokus Feb/2016
//#3 = RC1 - Started before IoTDay Netherlands

#define firmwareNumber 3

volatile long lastDebounce=0;
volatile long debounceDelay=1;
volatile long ntime=0;
char command1[10];
char parameter1[17];
Thing system;


void blink(int pin, int nTimes, int delays){
  pinMode(pin, OUTPUT);
  for(int x=0;x<nTimes;x++) {
    digitalWrite(pin, HIGH);
    delay(delays);
    digitalWrite(pin, LOW);
    delay(delays);
  }
}


void initDefault() {
  blink(10, 1, 250);
  blink(5, 1, 250);
  pinMode(6, OUTPUT);
  for(int x=255;x>-1;x--) {
    analogWrite(6, x);
    delay(2);
  }
}

Thing::Thing() {
  Serial.begin(9600); //default
  initFunction = initDefault;
  componentIndex=0;
  found=false;
  int eeAddress = sizeof(int); //Endereco eh logo depois do no. do firmware

  EEPROM.get( eeAddress, boardID );
}
void Thing::init(void(*initf)()) {
  initFunction = initf;
}

void Thing::add(char* name, int type, int port) {
  Component c1 = Component(name, type,port);
  components[numberOfComponents++]= c1;
}

void Thing::add(char* name, int type, char*(*myF)(char *), boolean isSensor) {
  Component c1 = Component(name, type, myF, isSensor);
  components[numberOfComponents++]= c1;
}
void Thing::mode(int modeNumber, void(*mode1)()) {
  Mode newMode(mode1);
  modes[modeNumber]=newMode;
  modeCounter++;
}

void changeModeViaButton() {
  if(millis()-lastDebounce>debounceDelay && ntime++>=100) {
    system.lastMode=system.currentMode;
    system.currentMode = system.currentMode==system.modeCounter ? 0 : system.currentMode + 1;
    system.modeChanged1=1;	
    
    ntime=0;
    lastDebounce=millis();
  }
}



void Thing::loop() {

  if(lastMode!=currentMode) {
    lastMode = currentMode;
    if(currentMode>0) {
      beep(currentMode);
      //modoMudou=1;
      modes[currentMode].setup();
    }
    else { //Mode 0 - defualt
      initFunction();
    }
  }
  modeChanged1=0;
  if(currentMode==0) serialServer();
  else { 
    modes[currentMode].execute();
    if(Serial.available()) {
      serialServer();
    }
    //check IR Receiver
  }
}




int Thing::findComponent(char* name) {
  int n = 0;
  found=false;
  for(int x=0;x<numberOfComponents;x++) {
    //Serial.print(x);
    //Serial.print(" - Name: ");
    //Serial.println(name);

    if(strcmp(name,components[x].name)==0) {
      n = x;
      found=true;
      break;
    }
  }
  return n;
}



void split(char* command) {
  for(int x=0;x<16;x++) {
    command1[x]='\0';
    parameter1[x]='\0';
  }
  int cv=0;
  int cp=0;
  for(int x=0;x<32;x++) {
    if(command[x]!='?' && command[x]!='\0') {
      command1[cv++]=command[x];
    } else {
      cp = command[x]=='?' ? x : 0;
      break;
    }
  }

  if(cp>0) {
    for(int x=cp+1;x<32;x++) {
      parameter1[x-cp-1]=command[x];
      if(command[x]=='\0') break;
    }
  } 
}


char* Thing::read(char* command) {
  split(command);
  if(strcmp("mode",command1)==0) {
    system.lastMode=system.currentMode;
    int new_mode= atoi(parameter1);
    system.currentMode=new_mode;
    system.modeChanged1=1;	

  }

  Component &c = components[findComponent(command1)];
  if(!found) {
    return "\n";
  }
  if(parameter1[0]=='\0') {
    return c.read();
  }
  else {    
    return c.write(parameter1);
  }  
}
void Thing::discoverySerial() {
  Serial.print("surfboard");
  Serial.print(boardID.id);
  Serial.print("|");
  Serial.print(numberOfComponents);
  Serial.print("|");
  for(int x=0;x<numberOfComponents;x++) {
    Serial.print(components[x].name);
    Serial.print("|");
    Serial.print(components[x].getTypeName());
    Serial.print("|");
    Serial.print(components[x].port);
    Serial.print("|");
    Serial.print(0);
    Serial.print("|");
  }
}

void Thing::serialServer() {
  char command[32];
  int counter=0;
  char c;
  while(Serial.available()>0) 
  {
    command[counter++]=Serial.read(); 
    delay(2);
  }

  if(counter>0) {
     command[counter]='\0';
     if(strcmp("?",command)==0) {
        data(DISCOVERY);
        return;
     }
     else if(strcmp("??",command)==0) {
        data(BASIC);
        return;
     }
     else if(strcmp("discovery",command)==0) {
        discoverySerial();
        return;
     }
     else if(strcmp("sensors",command)==0) {
        data(SENSORS);
        return;
     }
     else if(strcmp("config",command)==0) {
        config();
        return;
     }
     else if(strcmp("fw",command)==0) {
        Serial.print("\n");
        Serial.println(firmware());
        return;
     }

     else if(strcmp("cls",command)==0) {
        for(int x=0;x<20;x++) Serial.print('\n');
        return;
     }
     else if(strcmp("mem",command)==0) {
        freeRam();
     }

     char* r = read(command);
     if(r[0]!='\0') {
       Serial.print(r);
       Serial.flush();
     }
  }  
}
int Thing::firmware() {
  return firmwareNumber;
}

void Thing::identification(boolean complete) {
  Serial.print("{\"name\" : ");
  Serial.print("\"");
  Serial.print("surfboard");
  Serial.print(boardID.id);
  Serial.print("\"");
  Serial.print(",");

  Serial.print("\"firmware\" : ");
  Serial.print("\"");
  Serial.print(firmwareNumber);
  Serial.print("\"");
  Serial.print(",");


  Serial.print("\"serial\" : ");
  Serial.print("\"");
  Serial.print(boardID.serial);
  Serial.print("\"");
  Serial.print(",");

  Serial.print("\"key\" : ");
  Serial.print("\"");
  Serial.print(boardID.key);
  Serial.print("\"");
  if(complete) Serial.print("}");
}

void Thing::data(int w) {
  if(w==BASIC) { 
    identification(true);
    return;
  }
  identification(false); //não fechar json
  Serial.print(",");
  Serial.print("\"components\" : [");
  boolean r=false;
  for(int x=0;x<numberOfComponents;x++) {
   if(w==SENSORS && !components[x].isSensor && components[x].type!=ANALOG ) continue;
   if(w==SENSORS && components[x].type==DIGITAL) continue;
   if(w==SENSORS && components[x].type==PWM) continue;
   if(r) Serial.print(",");
   r=true;
    Serial.print("{\"name\" : ");
    Serial.print("\"");
    Serial.print(components[x].name);
    Serial.print("\"");
    Serial.print(",");
    if(w==DISCOVERY) {
      Serial.print("\"type\" : ");
      Serial.print("\"");
      Serial.print(components[x].getTypeName());
      Serial.print("\"");
      if(components[x].type==DIGITAL || components[x].type==ANALOG || components[x].type==PWM) { 
        Serial.print(",");
        Serial.print("\"port\" : ");
        Serial.print("\"");
        Serial.print(components[x].port);
        Serial.print("\"");	
      }
      Serial.print("}");

    }
    else if(w==SENSORS && (components[x].isSensor && components[x].type!=DIGITAL && components[x].type!=PWM) || components[x].type==ANALOG) {
      Serial.print("\"value\" : ");
      Serial.print("\"");
      Serial.print(components[x].read());
      Serial.print("\"");
      Serial.print("}");
    }
    //Serial.print(x+1<numberOfComponents ? "," : " ");
  }
  Serial.print("]}");
}
byte Thing::modeChanged() { return modeChanged1;}

void Thing::wait(long milis) { 
  modeChanged1=0;
  if(currentMode!=lastMode) {
    modeChanged1=1;
    return;
  }
  if(milis<10) delay(milis);
  else {
    for(long x=0;x<milis/10;x++) {
      delay(10);
      //checarMudancaModo();
      if(currentMode!=lastMode) {
        modeChanged1=1;
        return;
      }
    }
    delay(milis%10);
  }
}


void Thing::beep(int ntime) {
  pinMode(speakerPin, OUTPUT);
  for(int x=0;x<ntime;x++) {
    digitalWrite(speakerPin, HIGH);
    delay(100);
    digitalWrite(speakerPin, LOW);
    delay(100);
  }
}

void Thing::freeRam()
{
  extern int 	__heap_start, *__brkval;
  int v;
  int r = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  Serial.print(r);
}

void Thing::start() {
  
  componentIndex = 0;
  found = false;
  digitalWrite(2, HIGH); //PULL-UP
  attachInterrupt(0, changeModeViaButton, LOW);
  initFunction();
}

void Thing::start(int intNumber) {
  if(intNumber>-1) attachInterrupt(intNumber, changeModeViaButton, LOW);
  initFunction();

}

void Thing::config() {
  int counter=0;

  Serial.println("ID");
  while(!Serial.available());
  while(Serial.available()>0) 
  {
    command1[counter++]=Serial.read(); 
    delay(2);
  }
  boardID.id = atoi(command1);
  for(counter=0;counter<15;counter++) command1[counter]=' ';
  counter = 0;
  Serial.println("S.N.");
  while(!Serial.available());
  while(Serial.available()>0) 
  {
    command1[counter++]=Serial.read(); 
    delay(2);
  }
  boardID.serial = atol(command1);
  for(counter=0;counter<15;counter++) command1[counter]=' ';
  counter = 0;
  Serial.println("Key");
  while(!Serial.available());
  while(Serial.available()>0) 
  {
    command1[counter++]=Serial.read(); 
    delay(2);
  }
  boardID.key = atol(command1);
  data(BASIC);
  for(counter=0;counter<15;counter++) command1[counter]=' ';

  Serial.println("Confirm?");
  while(!Serial.available());
  if((char) Serial.read()=='1') {
    EEPROM.put( sizeof(int), boardID);
    beep(1);
    return;
  }
  beep(3);
}


