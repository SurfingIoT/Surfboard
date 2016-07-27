//
//  LightCellCollectionViewCell.h
//  joinIot
//
//  Created by Jose Lino Neto on 6/19/16.
//  Copyright © 2016 Construtor. All rights reserved.
//

#import <UIKit/UIKit.h>

IB_DESIGNABLE
@interface LightCellCollectionViewCell : UICollectionViewCell

@property (strong, nonatomic) IBInspectable NSString *sensorName;
@property (assign, nonatomic) IBInspectable BOOL onOff;
@property (strong, nonatomic) IBInspectable NSString *cellValue;

-(void)atualizar;


@end
