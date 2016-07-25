//
//  AtoresTableViewController.m
//  joinIot
//
//  Created by Jose Lino Neto on 6/20/16.
//  Copyright © 2016 Construtor. All rights reserved.
//

#import "AtoresTableViewController.h"
#import "DevicesExternos.h"

@interface AtoresTableViewController ()

@property (strong, nonatomic) DevicesExternos *casa;

@property (nonatomic) BOOL releOn;
@property (nonatomic) BOOL redOn;
@property (nonatomic) BOOL blueOn;
@property (nonatomic) BOOL greenOn;
@property (nonatomic) BOOL speakerOn;

@end

@implementation AtoresTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
    
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
    
    self.casa = [DevicesExternos sharedClient];
}

-(void)viewWillAppear:(BOOL)animated{
    [self.casa connect];
}

-(void)viewWillDisappear:(BOOL)animated{
    [self.casa disconnect];
}
- (IBAction)conectarAction:(id)sender {
    [self.casa connect];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 5;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Cell" forIndexPath:indexPath];
    
    UISwitch *switchValue = [cell viewWithTag:100];
    
    [switchValue addTarget:self action:@selector(switchValueChange:) forControlEvents:UIControlEventValueChanged];
    [switchValue setOn:NO]; //TODO: VALUE FROM THE IOT SURFBOARD
    [switchValue setTag:indexPath.row];
    
    UILabel *labelSensor = [cell viewWithTag:200];
    //[labelSensor setText:@"Relay"];
    
    switch (indexPath.row) {
        case 0:
            [labelSensor setText:@"Relé"];
            break;
        case 1:
            [labelSensor setText:@"Led Vermelho"];
            break;
        case 2:
            [labelSensor setText:@"Led Azul"];
            break;
        case 3:
            [labelSensor setText:@"Led Verde"];
            break;
        case 4:
            [labelSensor setText:@"Buzina"];
            break;
    }
    
    return cell;
}

- (void)switchValueChange:(id)sender
{
    BOOL state = [sender isOn];
    //NSString *rez = state == YES ? @"YES" : @"NO";
    //NSLog(@"%@", rez);
    //NSLog(@"%ld", (long)[sender tag]);
    
    NSInteger tag = (long)[sender tag];
    
    switch (tag) {
        case 0:
            [self.casa lights:state];
            break;
        case 1:
            [self.casa ledRed:state];
            break;
        case 2:
            [self.casa ledBlue:state];
            break;
        case 3:
            [self.casa ledGreen:state];
            break;
        case 4:
            [self.casa makeNoise:state];
            break;
    }
    
}

/*
 // Override to support conditional editing of the table view.
 - (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
 // Return NO if you do not want the specified item to be editable.
 return YES;
 }
 */

/*
 // Override to support editing the table view.
 - (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
 if (editingStyle == UITableViewCellEditingStyleDelete) {
 // Delete the row from the data source
 [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
 } else if (editingStyle == UITableViewCellEditingStyleInsert) {
 // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
 }
 }
 */

/*
 // Override to support rearranging the table view.
 - (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
 }
 */

/*
 // Override to support conditional rearranging of the table view.
 - (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
 // Return NO if you do not want the item to be re-orderable.
 return YES;
 }
 */

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

@end
