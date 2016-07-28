//
//  ConfigViewController.m
//  IotSurfboardClient
//
//  Created by Jose Lino Neto on 7/27/16.
//  Copyright © 2016 Construtor. All rights reserved.
//

#import "ConfigViewController.h"
#import "DevicesExternos.h"

@interface ConfigViewController ()
@property (weak, nonatomic) IBOutlet UITextField *brokerMqttTextField;
@property (weak, nonatomic) IBOutlet UITextField *canalPublicacaoSensoresTextField;
@property (weak, nonatomic) IBOutlet UITextField *canalPublicacaoComandosTextField;

@property (strong, nonatomic) DevicesExternos *device;

@end

@implementation ConfigViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    _device = [DevicesExternos sharedClient];
    
    self.brokerMqttTextField.text = self.device.mqttBroker;
    self.canalPublicacaoComandosTextField.text = self.device.mqttPublichChannel;
    self.canalPublicacaoSensoresTextField.text = self.device.mqttSensorsChannel;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (IBAction)saveAction:(id)sender {
    
    self.device.mqttBroker = self.brokerMqttTextField.text;
    self.device.mqttSensorsChannel = self.canalPublicacaoSensoresTextField.text;
    self.device.mqttPublichChannel = self.canalPublicacaoComandosTextField.text;
    
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setValue:self.brokerMqttTextField.text forKey:@"mqttbroker"];
    [defaults setValue:self.canalPublicacaoSensoresTextField.text forKey:@"mqttSensorsChannel"];
    [defaults setValue:self.canalPublicacaoComandosTextField.text forKey:@"mqttPublichChannel"];
    [defaults synchronize];
    
    [self voltarViewAnterior];
}

/**
 *  Voltar para view Anterior
 */
- (void)voltarViewAnterior
{
    UIViewController *sourceViewController = self;
    [sourceViewController.presentingViewController dismissViewControllerAnimated:YES completion:nil];
}

@end
