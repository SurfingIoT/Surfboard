#include "DHT.h"
#define DHTPIN 8  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//dados para acesso a rede WiFi
#define SSID "ASSIST"
#define PASS "akihabara@1969"

#define DST_IP "54.86.132.254"    //data.sparkfun.com
char myChar;

//dados para acesso ao banco de dador Sparkfun
const String publicKey = "G2q1b21w54FoDl5Q5GjY";
const String privateKey = "NW460W6J9RsMjqRkRWNl";
//char server[] = "data.sparkfun.com"; // Remote host site
const byte NUM_FIELDS = 6;
const String fieldNames[NUM_FIELDS] = {"botao2", "gas", "luminosidade", "potenciometro","temperatura","umidade"};
String fieldData[NUM_FIELDS];

//variaveis que armazenam dados do sensor DHT
float umidade = 50 ;
float temperatura =22  ;

//define portas associadas ao led RGB
int ledVermelho = 10 ;
int ledVerde = 6 ;
int ledAzul = 5 ;

void setup(){

  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  digitalWrite(2,HIGH); //ativa pull-up da chave B2
  
  dht.begin();
  
  // estabelece comunicacao serial com ESP8266:
  Serial.begin(19200);
  Serial.setTimeout(2000);

  Serial.println("AT+RST"); //reseta modulo ESP8266
  delay(500);

  do {
    Serial.println("AT+RST");
    piscaLed(ledVermelho,1);
    delay(500);
  } while (Serial.find("ready") == false);

    piscaLed(ledAzul,1);


  
  delay(1000);
  
  //estabelece conexao WiFi
  boolean connected = false;
  for (int i = 0; i < 5; i++){
    if (connectWiFi()){
      connected = true;
      break;
    }
  }
  
  if (!connected) { //se nao conectou na rede acende led VERMELHO
    digitalWrite(ledVermelho,HIGH);
    while (1);
  }

  
  delay(5000);
  
  //print the ip addr
//  Serial.println("ip address:");
  Serial.println("AT+CIFSR"); //verifica IP do modulo ESP8266
  delay(1000);

  
  if (Serial.find("+CIFSR")){
    piscaLed(ledVerde, 1);
  }else{
    piscaLed(ledVermelho, 1);
  }

  
  //set the single connection mode
  Serial.println("AT+CIPMUX=1");  //habilita conexoes multiplas no ESP8266
  delay(1000);

  if (Serial.find("OK")){
    piscaLed(ledVerde, 2);
  }else{
    piscaLed(ledVermelho, 2);
  }
}

void loop(){

  // atualiza array com leitura dos sensores:
  umidade = dht.readHumidity();
  temperatura = dht.readTemperature();

  if (digitalRead(2) == 0){
    fieldData[0] = "ON";  //botao B2
  }else{
    fieldData[0] = "OFF";  //botao B2
    }
  fieldData[1] = String(analogRead(0)); // gas
  fieldData[2] = String(analogRead(3)); // luminosidade
  fieldData[3] = String(analogRead(1)); //potenciometro
  fieldData[4] = String(temperatura); //temperatura
  fieldData[5] = String(umidade); //umidade
  
  String cmd = "AT+CIPSTART=4,\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  Serial.println(cmd);

  if (Serial.find("CONNECT")){
    piscaLed(ledVerde, 3);
  }else{
    piscaLed(ledVermelho, 3);
    Serial.println("AT+CIPCLOSE=4");
    //delay(1000);
    if (Serial.find("AT+CIPCLOSE")){
      piscaLed(ledVerde, 1);
    }else{
      piscaLed(ledVermelho, 1);
    }
    return ;
  }

 //   montagem comando GET em (cmd);

    cmd = "GET /input/";
    cmd += publicKey;
    cmd += "?private_key=";
    cmd += privateKey ;
  
    for (int i=0; i<NUM_FIELDS; i++){
      cmd +=  "&" ;
      cmd += (fieldNames[i]);
      cmd += "=";
      cmd += (fieldData[i]);
    }

  Serial.print("AT+CIPSEND=4,");
  
//  Serial.println(cmd.length()+1);
  Serial.println(cmd.length()+2);
  
  delay(100);
  
  if (Serial.find(">")){
  //  piscaLed(ledVerde, 4);
  }else{
    piscaLed(ledVermelho, 4);
    Serial.println("AT+CIPCLOSE=4");
  if (Serial.find("AT+CIPCLOSE")){
    piscaLed(ledVerde, 1);
  }else{
    piscaLed(ledVermelho, 1);
  }
    return;
  }
  
  Serial.println(cmd);  //envia comando GET
  
  //delay(1000);

  
  if (Serial.find("OK")){
    piscaLed(ledVerde, 5);
  }else{
    piscaLed(ledVermelho, 5);

    Serial.println("AT+CIPCLOSE=4");
    
    if (Serial.find("4,CLOSED")){
      piscaLed(ledVerde, 1);
    }else{
      piscaLed(ledVermelho, 1);
    }
  }

delay(1000);

  while (Serial.available()){
    Serial.read();
  }
      
}


