//
//  DevicesExternos.h
//  joinIot
//
//  Created by Jose Lino Neto on 5/16/16.
//  Copyright © 2016 Construtor. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MQTTKit.h>

@interface DevicesExternos : NSObject

@property (strong, nonatomic) MQTTClient *client;

@property (nonatomic) float temperature;
@property (nonatomic) float light;
@property (nonatomic) float alcohol;
@property (nonatomic) float potenciometer;
@property (nonatomic) float humidity;
@property (nonatomic) float distance;
@property (nonatomic) float distanceSonar;

typedef void (^HandlerExecution)(BOOL sucess);

+(instancetype)sharedClient;

-(void)connect;
-(void)disconnect;
-(void)reconnect;
-(void)listenMessage:(HandlerExecution )mensagem;

-(void)makeNoise:(BOOL)action;
-(void)ledRed:(BOOL)action;
-(void)ledBlue:(BOOL)action;
-(void)ledGreen:(BOOL)action;
-(void)lights:(BOOL)action;

@end
