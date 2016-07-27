//
//  TempCellCollectionViewCell.h
//  joinIot
//
//  Created by Jose Lino Neto on 6/20/16.
//  Copyright © 2016 Construtor. All rights reserved.
//

#import <UIKit/UIKit.h>

IB_DESIGNABLE
@interface TempCellCollectionViewCell : UICollectionViewCell

@property (strong, nonatomic) IBInspectable NSString *sensorName;
@property (strong, nonatomic) IBInspectable NSString *cellValue;
@property (assign, nonatomic) IBInspectable float tempValue;

-(void)atualizar;


@end
