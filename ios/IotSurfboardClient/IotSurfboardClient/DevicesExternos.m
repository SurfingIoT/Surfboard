//
//  DevicesExternos.m
//  joinIot
//
//  Created by Jose Lino Neto on 5/16/16.
//  Copyright © 2016 Construtor. All rights reserved.
//

#import "DevicesExternos.h"

@implementation DevicesExternos

+(instancetype)sharedClient{
    static DevicesExternos *sharedClient = nil;
    static dispatch_once_t pred;
    
    dispatch_once(&pred, ^{
        sharedClient = [[self alloc] init];
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        sharedClient.mqttBroker = [defaults objectForKey:@"mqttbroker"];
        sharedClient.mqttSensorsChannel = [defaults objectForKey:@"mqttSensorsChannel"];
        sharedClient.mqttPublichChannel = [defaults objectForKey:@"mqttPublichChannel"];
    });
    
    return sharedClient;
}

-(float)humidity{
    return _humidity;
}

-(float)temperature{
    return _temperature;
}

-(float)light{
    return _light;
}

-(float)potenciometer{
    return _potenciometer;
}

-(float)alcohol{
    return _alcohol;
}

-(float)distance{
    return _distance;
}

-(float)distanceSonar{
    return _distanceSonar;
}

-(NSString *)mqttBroker{
    if (!_mqttBroker || [_mqttBroker isEqualToString:@""]){
        _mqttBroker = @"iot.eclipse.org";
    }
    
    return _mqttBroker;
}

-(NSString *)mqttPublichChannel{
    if (!_mqttPublichChannel || [_mqttPublichChannel isEqualToString:@""]){
        _mqttPublichChannel = @"globalcode/things/surfboard177/publish";
    }
    
    return _mqttPublichChannel;
}

-(NSString *)mqttSensorsChannel{
    if (!_mqttSensorsChannel || [_mqttSensorsChannel isEqualToString:@""]){
        _mqttSensorsChannel = @"globalcode/things/surfboard177";
    }
    
    return _mqttSensorsChannel;
}

-(void)connect{
    // create the client with a unique client ID
    NSString *clientID = [[NSUUID UUID] UUIDString];
    
    //MQTTClient *client = [[MQTTClient alloc] initWithClientId:clientID];
    _client = [[MQTTClient alloc] initWithClientId:clientID];
    
    // connect to the MQTT server
    [_client connectToHost:_mqttBroker completionHandler:^(NSUInteger code) {
        if (code == ConnectionAccepted) {
            // when the client is connected, subscribe to the topic to receive message.
            [_client subscribe:_mqttSensorsChannel withCompletionHandler:^(NSArray *grantedQos) {
                NSLog(@"%@", grantedQos);
            }];
        }
    }];
}

-(void)disconnect{
    [_client disconnectWithCompletionHandler:^(NSUInteger code) {
        
    }];
}

-(void)reconnect{
    [_client reconnect];
}

-(void)makeNoise:(BOOL)action{
    // when the client is connected, send a MQTT message
    NSString *command = [@"speaker?" stringByAppendingString:action ? @"1" : @"0"];
    [_client publishString:command
                   toTopic:_mqttPublichChannel
                   withQos:AtMostOnce
                    retain:NO
         completionHandler:^(int mid) {
             NSLog(@"message has been delivered");
         }];
}

-(void)ledRed:(BOOL)action{
    NSString *command = [@"red?" stringByAppendingString:action ? @"255" : @"0"];
    [_client publishString:command
                   toTopic:_mqttPublichChannel
                   withQos:AtMostOnce
                    retain:NO
         completionHandler:^(int mid) {
             NSLog(@"message has been delivered");
         }];
}

-(void)ledBlue:(BOOL)action{
    NSString *command = [@"blue?" stringByAppendingString:action ? @"255" : @"0"];
    [_client publishString:command
                   toTopic:_mqttPublichChannel
                   withQos:AtMostOnce
                    retain:NO
         completionHandler:^(int mid) {
             NSLog(@"message has been delivered");
         }];
}

-(void)ledGreen:(BOOL)action{
    NSString *command = [@"green?" stringByAppendingString:action ? @"255" : @"0"];
    [_client publishString:command
                   toTopic:_mqttPublichChannel
                   withQos:AtMostOnce
                    retain:NO
         completionHandler:^(int mid) {
             NSLog(@"message has been delivered");
         }];
}

-(void)lights:(BOOL)action{
    NSString *command = [@"relay?" stringByAppendingString:action ? @"1" : @"0"];
    [_client publishString:command
                   toTopic:_mqttPublichChannel
                   withQos:AtMostOnce
                    retain:NO
         completionHandler:^(int mid) {
             NSLog(@"message has been delivered");
         }];
}

-(void)listenMessage:(HandlerExecution)sucess{
    // define the handler that will be called when MQTT messages are received by the client
    [_client setMessageHandler:^(MQTTMessage *message) {
        NSError *error;
        NSDictionary* json = [NSJSONSerialization JSONObjectWithData:message.payload
                                                             options:kNilOptions
                                                               error:&error];
        
        NSArray *components = [json objectForKey:@"components"];
        for (NSDictionary *item in components) {
            NSString *name = [item objectForKey:@"name"];
            float value = [[item objectForKey:@"value"] floatValue];
            
            if ([name isEqualToString:@"alcohol"]){
                _alcohol = value;
            }
            
            if ([name isEqualToString:@"pot"]){
                _potenciometer = value;
            }
            
            if ([name isEqualToString:@"light"]){
                _light = value;
            }
            
            if ([name isEqualToString:@"distance"]){
                _distance = value;
            }
            
            if ([name isEqualToString:@"temp"]){
                _temperature = value;
            }
            
            if ([name isEqualToString:@"humidity"]){
                _humidity = value;
            }
            
            if ([name isEqualToString:@"distanceSonar"]){
                _distanceSonar = value;
            }
        }
        NSString *text = message.payloadString;
        NSLog(@"received message %@", text);
        sucess(YES);
    }];
}

@end
