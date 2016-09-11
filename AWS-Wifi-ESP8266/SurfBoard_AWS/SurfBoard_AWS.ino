#include "DHT.h"
#define DHTPIN 8  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

char myChar;

//array para armazenar dados dos sensores da Surfboard
String fieldData[6];

//variaveis que armazenam dados do sensor DHT
float umidade = 0 ;
float temperatura = 0  ;

//define portas associadas ao led RGB
int portaLedVermelho = 5 ;
int portaLedVerde = 6 ;
int portaLedAzul = 10 ;

//define portas associadas ao rele
int portaRele = 4 ;

char atuadoresdata[300];                     //A 30 byte character array to hold incoming data from the sensors
byte atuadores_bytes_received = 0;          //We need to know how many characters bytes have been received
String atuadoresstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean atuadores_string_complete = false;               //have we received all the data from the Atlas Scientific product

String cmd = "";

void setup(){
  pinMode(portaLedVermelho, OUTPUT);
  pinMode(portaLedVerde, OUTPUT);
  pinMode(portaLedAzul, OUTPUT);
  pinMode(portaRele, OUTPUT);
  digitalWrite(2,HIGH); //ativa pull-up da chave B2
  
  dht.begin();
  
  // estabelece comunicacao serial com ESP8266:
  Serial.begin(19200);
}

void loop(){

  // atualiza array fieldData com leitura dos sensores:
  umidade = dht.readHumidity();
  temperatura = dht.readTemperature();
  fieldData[0] = String(temperatura); //temperatura
  fieldData[1] = String(umidade); //umidade
  fieldData[2] = String(analogRead(1)); //potenciometro
  fieldData[3] = String(analogRead(3)); // luminosidade
  fieldData[4] = String(analogRead(0)); // gas
  if (digitalRead(2) == 0){ //verifica estado do botao B2
    fieldData[5] = "1";  //botao B2
  }else{
    fieldData[5] = "0";  //botao B2
  }

  cmd = "";                 //monta string que sera enviado para ESP8266 com os dados dos sensores da SerfBoard 
  for (int i=0; i<6; i++){
    cmd += (fieldData[i]);
    cmd += ",";
  }

  if (Serial.available() > 0) {  //verifica se chegaram dados do ESP8266 (AWS)
    atuadores_bytes_received = Serial.readBytesUntil(10, atuadoresdata, 300);  //le dados recebidos da SurfBoard ate o <LF>
    atuadoresdata[atuadores_bytes_received] = 0;  //adicionamos um 0 no final do array apos o ultimo caracter recebido assim sabemos que acabaram os dados
    atuadores_string_complete = true;
    atuadoresstring = String(atuadoresdata);                                
  }
  if (atuadores_string_complete == true) {               //verifica se recebemos os dados completos
    atualizaAtuadores(atuadoresstring);                                //then call this function 

    atuadoresstring = "";                                //clear the string
    for (int i=0 ; i < 300 ; i++){ //limpa array sensordata
      atuadoresdata[i]= 0x00;
    }
    atuadores_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    }


//  delay(2000);
      
}


