//
//  DevicesExternos.h
//  joinIot
//
//  Created by Jose Lino Neto on 5/16/16.
//  Copyright © 2016 Construtor. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MQTTKit.h>

/*
 * ************************** README ***************************************
 * CHANGE THE FOLOWING LINES TO CONNECT WITH YOUR MQTT BROKER
 *
 * 1- URL OF THE BROKER
 * 2- WHERE THE SENSORS IS PUBLISHED FROM THE IOT SERVICES CHANNEL
 * 3- WHERE TO PUBLISH TO ACTORES CAN DO THINGS
 *
 * *************************************************************************
 */

//#define URL_MQTT_BROKER @"iot.eclipse.org"
//#define MQTT_SENSORS_CHANNEL @"myhomework/surfboard177/sensors"
//#define MQTT_PUBLISH_CHANNEL @"myhomework/surfboard177"

/**
 *  Class to work with the Iot Surfboard
 */
@interface DevicesExternos : NSObject

@property (strong, nonatomic) NSString *mqttBroker;
@property (strong, nonatomic) NSString *mqttSensorsChannel;
@property (strong, nonatomic) NSString *mqttPublichChannel;


/**
 *  MQtt Client
 */
@property (strong, nonatomic) MQTTClient *client;

/**
 *  Value from the temperature sensor
 */
@property (nonatomic) float temperature;

/**
 *  Value from the light sensor
 */
@property (nonatomic) float light;

/**
 *  Value from the alcohol sensor
 */
@property (nonatomic) float alcohol;

/**
 *  Value from the pot sensor
 */
@property (nonatomic) float potenciometer;

/**
 *  Value from the humidity sensor
 */
@property (nonatomic) float humidity;

/**
 *  *  Value from the distance sensor
 */
@property (nonatomic) float distance;

/**
 *  *  Value from the distance sonar sensor
 */
@property (nonatomic) float distanceSonar;

/**
 *  Sucess block
 *
 *  @param sucess Sucess
 */
typedef void (^HandlerExecution)(BOOL sucess);

/**
 *  Singleton Object
 *
 *  @return Singleton Object
 */
+(instancetype)sharedClient;

/**
 *  Connect to mqtt server
 */
-(void)connect;

/**
 *  Disconnect mqtt server
 */
-(void)disconnect;

/**
 *  Reconnect the mqtt server
 */
-(void)reconnect;

/**
 *  Listen a channel on mqtt broker
 *
 *  @param mensagem Recived Message
 */
-(void)listenMessage:(HandlerExecution )mensagem;

/**
 *  Make noise active the speaker
 *
 *  @param action YES Active and NO Desactive
 */
-(void)makeNoise:(BOOL)action;

/**
 *  Turn the red Led on or off
 *
 *  @param action YES Active and NO Desactive
 */
-(void)ledRed:(BOOL)action;

/**
 *  Turn the blue Led on or off
 *
 *  @param action YES Active and NO Desactive
 */
-(void)ledBlue:(BOOL)action;

/**
 *  Turn the green Led on or off
 *
 *  @param action YES Active and NO Desactive
 */
-(void)ledGreen:(BOOL)action;

/**
 *  Turn the all Led on or off
 *
 *  @param action YES Active and NO Desactive
 */
-(void)lights:(BOOL)action;

@end
