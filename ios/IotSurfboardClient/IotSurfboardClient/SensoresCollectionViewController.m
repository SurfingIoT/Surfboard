//
//  SensoresCollectionViewController.m
//  joinIot
//
//  Created by Jose Lino Neto on 6/19/16.
//  Copyright © 2016 Construtor. All rights reserved.
//

#import "SensoresCollectionViewController.h"
#import "LightCellCollectionViewCell.h"
#import "TempCellCollectionViewCell.h"
#import "GenericCellCollectionViewCell.h"
#import "DevicesExternos.h"
#import <MQTTKit.h>

@interface SensoresCollectionViewController ()

@property (strong, nonatomic) DevicesExternos *casa;
@property (strong, nonatomic) NSMutableArray *localSensors;

@end

@implementation SensoresCollectionViewController

static NSString * const reuseIdentifier = @"Cell";
static NSString * const reuseIdentifierLight = @"CellLight";
static NSString * const reuseIdentifierTemp = @"CellTemp";

#pragma mark - LifeCicle

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self.collectionView setBackgroundColor:[UIColor whiteColor]];
    
    // Uncomment the following line to preserve selection between presentations
    // self.clearsSelectionOnViewWillAppear = NO;
    
    // Register cell classes
    [self.collectionView registerClass:[GenericCellCollectionViewCell class] forCellWithReuseIdentifier:reuseIdentifier];
    [self.collectionView registerClass:[LightCellCollectionViewCell class] forCellWithReuseIdentifier:reuseIdentifierLight];
    [self.collectionView registerClass:[TempCellCollectionViewCell class] forCellWithReuseIdentifier:reuseIdentifierTemp];
    
    // Do any additional setup after loading the view.
    // My Home Work Devices
    self.casa = [DevicesExternos sharedClient];
    
    // Sensors
    self.localSensors = [[NSMutableArray alloc] init];
}

-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    
    // Conectar
    [self.casa connect];
    
    // Ouvir Mensagem
    [self ouvirMensagem];
}

-(void)viewDidDisappear:(BOOL)animated{
    [super viewDidDisappear:animated];
    
    [self.casa disconnect];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark <UICollectionViewDataSource>

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}


- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return [self.localSensors count];
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    
    switch (indexPath.row) {
        case 0:{
            TempCellCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifierTemp forIndexPath:indexPath];
            float valorSensor = [[self.localSensors objectAtIndex:indexPath.row] floatValue];
            
            cell.tempValue = valorSensor;
            cell.cellValue = [NSString stringWithFormat:@"%.02f °c", valorSensor];
            cell.sensorName = @"Temperatura";
            
            [cell atualizar];
            
            return cell;
        }
            break;
        case 1: {
            LightCellCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifierLight forIndexPath:indexPath];
            float valorSensor = [[self.localSensors objectAtIndex:indexPath.row] floatValue];
            
            if (valorSensor && valorSensor > 0){
                cell.onOff = YES;
            }
            else{
                cell.onOff = NO;
            }
            
            cell.cellValue = [NSString stringWithFormat:@"%.02f", valorSensor];
            cell.sensorName = @"Luminosidade";
            
            [cell atualizar];
            
            return cell;
            
        }
            break;
        case 2:{
            GenericCellCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
            float valorSensor = [[self.localSensors objectAtIndex:indexPath.row] floatValue];
            
            cell.cellValue = [NSString stringWithFormat:@"%.02f", valorSensor];
            cell.sensorName = @"Humidade";
            cell.backGroundColorCustom = nil;
            
            [cell atualizar];
            
            return cell;
        }
            break;
        case 3:{
            GenericCellCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
            float valorSensor = [[self.localSensors objectAtIndex:indexPath.row] floatValue];
            
            cell.cellValue = [NSString stringWithFormat:@"%.02f", valorSensor];
            cell.sensorName = @"Álcool";
            
            if (valorSensor > 0 && valorSensor < 50){
                cell.backGroundColorCustom = [UIColor greenColor];
            }
            
            if (valorSensor > 50 && valorSensor < 400){
                cell.backGroundColorCustom = [UIColor yellowColor];
            }
            
            if (valorSensor > 400){
                cell.backGroundColorCustom = [UIColor redColor];
            }
            
            [cell atualizar];
            
            return cell;
        }
            break;
        case 4:{
            GenericCellCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
            float valorSensor = [[self.localSensors objectAtIndex:indexPath.row] floatValue];
            
            cell.cellValue = [NSString stringWithFormat:@"%.02f", valorSensor];
            cell.sensorName = @"Potenciômetro";
            cell.backGroundColorCustom = nil;
            
            [cell atualizar];
            
            return cell;
        }
            break;
        case 5:{
            GenericCellCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
            float valorSensor = [[self.localSensors objectAtIndex:indexPath.row] floatValue];
            
            cell.cellValue = [NSString stringWithFormat:@"%.02f", valorSensor];
            cell.sensorName = @"Distância";
            cell.backGroundColorCustom = nil;
            
            [cell atualizar];
            
            return cell;
        }
            break;
//        case 6:{
//            GenericCellCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
//            float valorSensor = [[self.localSensors objectAtIndex:indexPath.row] floatValue];
//            
//            cell.cellValue = [NSString stringWithFormat:@"%.02f", valorSensor];
//            cell.sensorName = @"Distância Sonar";
//            cell.backGroundColorCustom = nil;
//            
//            [cell atualizar];
//            
//            return cell;
//        }
//            break;
        default:{
            GenericCellCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
            return cell;
        }
            break;
    }
}

#pragma mark <UICollectionViewDelegate>

/*
 // Uncomment this method to specify if the specified item should be highlighted during tracking
 - (BOOL)collectionView:(UICollectionView *)collectionView shouldHighlightItemAtIndexPath:(NSIndexPath *)indexPath {
	return YES;
 }
 */
/*
 // Uncomment this method to specify if the specified item should be selected
 - (BOOL)collectionView:(UICollectionView *)collectionView shouldSelectItemAtIndexPath:(NSIndexPath *)indexPath {
 return YES;
 }
 */

/*
 // Uncomment these methods to specify if an action menu should be displayed for the specified item, and react to actions performed on the item
 - (BOOL)collectionView:(UICollectionView *)collectionView shouldShowMenuForItemAtIndexPath:(NSIndexPath *)indexPath {
	return NO;
 }
 
 - (BOOL)collectionView:(UICollectionView *)collectionView canPerformAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender {
	return NO;
 }
 
 - (void)collectionView:(UICollectionView *)collectionView performAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender {
	
 }
 */

#pragma mark - Local Methods

-(void)ouvirMensagem{
    [self.casa listenMessage:^(BOOL sucess) {
        if (sucess){
            dispatch_async(dispatch_get_main_queue(), ^{
                // Clear All Objects Values
                [self.localSensors removeAllObjects];
                
                // Add updates Values
                [self.localSensors addObject:[NSNumber numberWithFloat:self.casa.temperature]];
                [self.localSensors addObject:[NSNumber numberWithFloat:self.casa.light]];
                [self.localSensors addObject:[NSNumber numberWithFloat:self.casa.humidity]];
                [self.localSensors addObject:[NSNumber numberWithFloat:self.casa.alcohol]];
                [self.localSensors addObject:[NSNumber numberWithFloat:self.casa.potenciometer]];
                [self.localSensors addObject:[NSNumber numberWithFloat:self.casa.distance]];
                //[self.localSensors addObject:[NSNumber numberWithFloat:self.casa.distanceSonar]];
                
                // Reload Data
                [self.collectionView reloadData];
            });
        }
    }];
}

#pragma mark - Actions

- (IBAction)connectAction:(id)sender {
    [self.casa connect];
}

@end
