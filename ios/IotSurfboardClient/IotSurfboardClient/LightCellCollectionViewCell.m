//
//  LightCellCollectionViewCell.m
//  joinIot
//
//  Created by Jose Lino Neto on 6/19/16.
//  Copyright © 2016 Construtor. All rights reserved.
//

#import "LightCellCollectionViewCell.h"

@implementation LightCellCollectionViewCell

-(void)drawRect:(CGRect)rect{
    //// General Declarations
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    //// Color Declarations
    UIColor* backGroudRed = [UIColor colorWithRed: 0.8 green: 0.32 blue: 0.32 alpha: 1];
    UIColor* textColorWhite = [UIColor colorWithRed: 1 green: 1 blue: 1 alpha: 1];
    UIColor* sunColorYellow = [UIColor colorWithRed: 0.889 green: 0.876 blue: 0.235 alpha: 1];
    UIColor* sunColorBlue = [UIColor colorWithRed: 0.746 green: 0.748 blue: 0.776 alpha: 1];
    
    //// Variable Declarations
    UIColor* expression = _onOff ? sunColorYellow : sunColorBlue;
    
    //// Rectangle Drawing
    UIBezierPath* rectanglePath = [UIBezierPath bezierPathWithRect: CGRectMake(0, 0, 355, 160)];
    [backGroudRed setFill];
    [rectanglePath fill];
    
    
    //// Group
    {
        //// Oval Drawing
        UIBezierPath* ovalPath = [UIBezierPath bezierPathWithOvalInRect: CGRectMake(27, 32, 108, 108)];
        [expression setFill];
        [ovalPath fill];
        
        
        //// Rectangle 2 Drawing
        CGContextSaveGState(context);
        CGContextTranslateCTM(context, 14.94, 33.25);
        CGContextRotateCTM(context, 32.66 * M_PI / 180);
        
        UIBezierPath* rectangle2Path = [UIBezierPath bezierPathWithRect: CGRectMake(0, 0, 25, 11)];
        [expression setFill];
        [rectangle2Path fill];
        
        CGContextRestoreGState(context);
        
        
        //// Rectangle 3 Drawing
        CGContextSaveGState(context);
        CGContextTranslateCTM(context, 42.51, 11.69);
        CGContextRotateCTM(context, 50.71 * M_PI / 180);
        
        UIBezierPath* rectangle3Path = [UIBezierPath bezierPathWithRect: CGRectMake(0, 0, 25, 11)];
        [expression setFill];
        [rectangle3Path fill];
        
        CGContextRestoreGState(context);
        
        
        //// Rectangle 4 Drawing
        CGContextSaveGState(context);
        CGContextTranslateCTM(context, 7.15, 72.48);
        CGContextRotateCTM(context, -0.29 * M_PI / 180);
        
        UIBezierPath* rectangle4Path = [UIBezierPath bezierPathWithRect: CGRectMake(0, 0, 17.64, 9.54)];
        [expression setFill];
        [rectangle4Path fill];
        
        CGContextRestoreGState(context);
        
        
        //// Rectangle 5 Drawing
        CGContextSaveGState(context);
        CGContextTranslateCTM(context, 7.15, 102.48);
        CGContextRotateCTM(context, -16.59 * M_PI / 180);
        
        UIBezierPath* rectangle5Path = [UIBezierPath bezierPathWithRect: CGRectMake(0, 0, 17.64, 9.54)];
        [expression setFill];
        [rectangle5Path fill];
        
        CGContextRestoreGState(context);
        
        
        //// Rectangle 6 Drawing
        CGContextSaveGState(context);
        CGContextTranslateCTM(context, 75.8, 8.09);
        CGContextRotateCTM(context, 83.91 * M_PI / 180);
        
        UIBezierPath* rectangle6Path = [UIBezierPath bezierPathWithRect: CGRectMake(0, 0, 19.27, 8.79)];
        [expression setFill];
        [rectangle6Path fill];
        
        CGContextRestoreGState(context);
    }
    
    
    //// Text Drawing
    CGRect textRect = CGRectMake(57, 48, 242, 63);
    NSMutableParagraphStyle* textStyle = [NSMutableParagraphStyle new];
    textStyle.alignment = NSTextAlignmentCenter;
    
    NSDictionary* textFontAttributes = @{NSFontAttributeName: [UIFont fontWithName: @"ArialRoundedMTBold" size: 49], NSForegroundColorAttributeName: textColorWhite, NSParagraphStyleAttributeName: textStyle};
    
    CGFloat textTextHeight = [_cellValue boundingRectWithSize: CGSizeMake(textRect.size.width, INFINITY)  options: NSStringDrawingUsesLineFragmentOrigin attributes: textFontAttributes context: nil].size.height;
    CGContextSaveGState(context);
    CGContextClipToRect(context, textRect);
    [_cellValue drawInRect: CGRectMake(CGRectGetMinX(textRect), CGRectGetMinY(textRect) + (CGRectGetHeight(textRect) - textTextHeight) / 2, CGRectGetWidth(textRect), textTextHeight) withAttributes: textFontAttributes];
    CGContextRestoreGState(context);
    
    
    //// Text 2 Drawing
    CGRect text2Rect = CGRectMake(118, 4, 232, 21);
    NSMutableParagraphStyle* text2Style = [NSMutableParagraphStyle new];
    text2Style.alignment = NSTextAlignmentRight;
    
    NSDictionary* text2FontAttributes = @{NSFontAttributeName: [UIFont fontWithName: @"ArialRoundedMTBold" size: UIFont.labelFontSize], NSForegroundColorAttributeName: textColorWhite, NSParagraphStyleAttributeName: text2Style};
    
    CGFloat text2TextHeight = [_sensorName boundingRectWithSize: CGSizeMake(text2Rect.size.width, INFINITY)  options: NSStringDrawingUsesLineFragmentOrigin attributes: text2FontAttributes context: nil].size.height;
    CGContextSaveGState(context);
    CGContextClipToRect(context, text2Rect);
    [_sensorName drawInRect: CGRectMake(CGRectGetMinX(text2Rect), CGRectGetMinY(text2Rect) + (CGRectGetHeight(text2Rect) - text2TextHeight) / 2, CGRectGetWidth(text2Rect), text2TextHeight) withAttributes: text2FontAttributes];
    CGContextRestoreGState(context);
}

-(void)atualizar{
    [self setNeedsDisplay];
}

@end
