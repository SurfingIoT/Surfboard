//
//  HumidityCellCollectionViewCell.h
//  joinIot
//
//  Created by Jose Lino Neto on 6/20/16.
//  Copyright © 2016 Construtor. All rights reserved.
//

#import <UIKit/UIKit.h>

IB_DESIGNABLE
@interface GenericCellCollectionViewCell : UICollectionViewCell

@property (strong, nonatomic) IBInspectable NSString *sensorName;
@property (strong, nonatomic) IBInspectable NSString *cellValue;
@property (strong, nonatomic) UIColor *backGroundColorCustom;

-(void)atualizar;

@end
