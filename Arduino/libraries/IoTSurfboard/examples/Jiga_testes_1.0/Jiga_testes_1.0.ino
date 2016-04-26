  /*
Programa para teste do hardware da nova placa da Globalcode 
 */
#include "DHT.h"
#include <Wire.h>
#include "DS1307RTC.h"
#include "Time.h"
#include <Servo.h>


int RECV_PIN = 7;  //define porta recepcao IR




Servo servo11;




DS1307RTC clock;//define a object of DS1307 class


#define DHTPIN 8 // define porta onde esta conectado o DHT-11
#define DHTTYPE DHT11 // define sensor Temp/Humi como DHT 11
DHT dht(DHTPIN, DHTTYPE);
 
char inChar = 0;
int PotPin = 1 ;  //define porta onde esta conectado o potenciometro
int LDRPin = 3 ;  //define porta onde esta conectado o LDR
int relePin = 4;  //define porta onde esta conectado o Speaker/Rele
int speakerPin = 16;  //define porta onde esta conectado o Speaker/Rele
int chaveB2Pin = 2 ;  //define porta onde esta conectada a chave extra

int ledRGB_R_Pin = 5 ;  //define portas onde esta conectad o led RGB
int ledRGB_G_Pin = 6 ;
int ledRGB_B_Pin = 10 ;

int TR1_Pin = 11 ;  //define porta onde esta conectado o transistor TR1

int sensorAlcoolPin = 0 ;  // define porta onde esta conectado o sensor de Alcool

float h ;
float t ;  

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

tmElements_t tm;

void setup() {
  
  bool parse=false;
  bool config=false;

  
  // initialize serial:
  Serial.begin(9600);
  delay(1000);
  printMenu();  //chama rotina que imprime o menu de opcoes de teste
  pinMode (ledRGB_R_Pin, OUTPUT);
  pinMode (ledRGB_G_Pin, OUTPUT);
  pinMode (ledRGB_B_Pin, OUTPUT);
  pinMode (TR1_Pin, OUTPUT);

  digitalWrite (chaveB2Pin, HIGH); //habilita pullup na porta D2
  dht.begin();



  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
  }

  
  
}

void loop() {
   if (Serial.available()) {
     inChar = (char)Serial.read(); 
    
     if (inChar == '1') {
       testPotenciometro();
     }
     if (inChar == '2') {
       testLDR();
     }   
     if (inChar == '3') {
       testRele();
     }   
     if (inChar == '4') {
       teste_led_RGB();
     }   
     if (inChar == '5') {
       //teste_sensor_Alcool();
       teste_speaker();
     }
     if (inChar == '6') {
       teste_chave_B2();
     }
     if (inChar == '7') {
       testDHT11();
     }
     if (inChar == '8') {
       testRTC();
     }
     if (inChar == '9') {
       //testIR_RX();
     }
     
     
     if (inChar == 'A') {
       //ir_tx();
     }
     
     if (inChar == 'B') {
       test_servo();
     }
     
     if (inChar == 'C') {
       teste_sensor_Alcool();
     }
     
     
     
     
     
     
     
     
     
     
     
     
     
     
   }
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


