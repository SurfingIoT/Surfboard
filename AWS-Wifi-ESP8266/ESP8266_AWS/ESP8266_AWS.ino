#include <ESP8266WiFi.h>

#include <AmazonDynamoDBClient.h>
#include <AmazonSNSClient.h>
#include <AWSClient.h>
#include <AWSClient2.h>
#include <DeviceIndependentInterfaces.h>
#include <jsmn.h>
#include <sha256.h>
#include <Utils.h>

#include "AmazonDynamoDBClient.h"
#include "Esp8266AWSImplementations.h"
#include "AWSFoundationalTypes.h"
#include <stdlib.h>
#include "keys.h"

extern "C" {
#include "user_interface.h"
}

/*
 *  This sample uses GetItem on a DynamoDB table to retrieve the RGB state
 *  and SetItem to set the RGB state.
 *
 * For this demo to work you must have keys.h/.ccp files that contain your AWS
 * access keys and define "awsSecKey" and "awsKeyID", a DynamoDB table with the
 * name defined by the constant TABLE_NAME with hash and range keys as defined
 * by constants HASH_KEY_NAME/RANGE_KEY_NAME, and and item in that table with
 * attributes as defined by HASH_KEY_VALUE/RANGE_KEY_VALUE and number
 * attributes R G and B.
 */

//GPIO pin assignments
#define LED_IND 2      // LED used for indicate when WiFi connected 

// Serial baud and server port (customize to your liking)
#define SERBAUD 19200
#define SVRPORT 9703

/* Contants describing DynamoDB table and values being used. */
static const char* HASH_KEY_NAME = "DeviceName";
static const char* HASH_KEY_VALUE = "Surfboard";
static const char* RANGE_KEY_NAME = "id";
static const char* RANGE_KEY_VALUE = "1";
static const char* TABLE_NAME = "Surfboard";
static const int KEY_SIZE = 2;
/* Constants for connecting to DynamoDB. */
static const char* AWS_REGION = "us-west-2";   //<<---CHANGE IF NECESSARY
static const char* AWS_ENDPOINT = "amazonaws.com";

/////////////////////////////////////////////////////////////////
//Network Parameters: You must customize for your network
/////////////////////////////////////////////////////////////////
const char* ssid = "ASSIST3";                //<<---CHANGE AS NECESSARY
const char* password = "**********";     //<<---CHANGE AS NECESSARY
const IPAddress ipadd(192,168,10,250);          //<<---CHANGE AS NECESSARY    
const IPAddress ipgat(192,168,10,1);            //<<---CHANGE AS NECESSARY      
const IPAddress ipsub(255,255,255,0);     

char szInfo[80];
char szT[40];

/* Device independent implementations required for AmazonDynamoDBClient to
 * function. */
Esp8266HttpClient httpClient;
Esp8266DateTimeProvider dateTimeProvider;

AmazonDynamoDBClient ddbClient;

PutItemInput putItemInput;    //Put Item
GetItemInput getItemInput;    //Get Item
AttributeValue hashKey;
AttributeValue rangeKey;
ActionError actionError;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(SVRPORT);
WiFiClient client;
WiFiClient client2;

char sensordata[200];                     //A 30 byte character array to hold incoming data from the sensors
byte sensor_bytes_received = 0;          //We need to know how many characters bytes have been received
String sensorstring = "";                //a string to hold the data from the Atlas Scientific product
boolean sensor_string_complete = false;               //have we received all sensors data 

void setup() {
    /* Begin serial communication. */
    Serial.begin(SERBAUD);
    Serial.println("Starting Setup");

    startWIFI();
 
    /* Initialize ddbClient. */
    ddbClient.setAWSRegion(AWS_REGION);
    ddbClient.setAWSEndpoint(AWS_ENDPOINT);
    ddbClient.setAWSSecretKey(awsSecKey);
    ddbClient.setAWSKeyID(awsKeyID);
    ddbClient.setHttpClient(&httpClient);
    ddbClient.setDateTimeProvider(&dateTimeProvider);

    /* Use GPIO2 for LED Indicator */
    pinMode(LED_IND , OUTPUT);
    digitalWrite(LED_IND, 0); //Turn LED ON (Setup completed)
}
/////////////////////////////////////////////////////////////////////////////////
//  yieldEspCPU() prevents ESP timeout resets
/////////////////////////////////////////////////////////////////////////////////
void loop() {
    if (WiFi.status() != WL_CONNECTED) {
      startWIFI();
    }
    static int TEMP,HUMI,POT,LIGHT,GAS,BUTTON,R,G,B,REL;
    yieldEspCPU();  
    awsGetTable(&TEMP,&HUMI,&POT,&LIGHT,&GAS,&BUTTON,&R,&G,&B,&REL);
    yieldEspCPU(); 
    if (Serial.available() > 0) {  
      sensor_bytes_received = Serial.readBytesUntil(10, sensordata, 200);  //le dados recebidos da SurfBoard ate o <LF>
      sensordata[sensor_bytes_received] = 0;  //adicionamos um 0 no final do array apos o ultimo caracter recebido assim sabemos que acabaram os dados
      sensor_string_complete = true;
      sensorstring = String(sensordata);                                
    }
    if (sensor_string_complete == true) {   //verifica se recebemos os dados completos
      publish_on_AWS(&TEMP,&HUMI,&POT,&LIGHT,&GAS,&BUTTON,&R,&G,&B,&REL);  //entao cham rotina que publica dados dos sensores na AWS 
      sensorstring = "";   //clear the string
      for (int i=0 ; i < 200 ; i++){  //limpa array sensordata
        sensordata[i]= 0x00;
      }
      sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    }
}
