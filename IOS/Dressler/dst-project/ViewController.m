//
//  ViewController.m
//  dst
//
//  Created by Haleeq Usman on 11/26/2013.
//  Copyright (c) 2013 Dressler llc. All rights reserved.
//

#import "ViewController.h"

@implementation CALayer(XibConfiguration)

-(void)setBorderUIColor:(UIColor*)color
{
    self.borderColor = color.CGColor;
}

-(UIColor *)borderUIColor
{
    return [UIColor colorWithCGColor:self.borderColor];
}

-(void)setBackgroundUIColor:(UIColor*)color
{
    self.backgroundColor = color.CGColor;
}

-(UIColor *)backgroundUIColor
{
    return [UIColor colorWithCGColor:self.backgroundColor];
}

-(void)setBackgroundUIPattern:(NSString *)imagePath
{
    UIColor *color = [UIColor colorWithPatternImage:[UIImage imageNamed:imagePath]];
    [self setBackgroundColor:[color CGColor]];
}

-(NSString *)backgroundUIPattern
{
    return @"";
}

-(void)setBackgroundUIClear:(Boolean)isClear
{
    if(isClear) {
        UIColor *color = [UIColor redColor];
        [self setBackgroundColor:[color CGColor]];
    }
}

@end


@implementation ViewController

- (IBAction)tapToExpand:(id)sender {
    UIButton *tappedButton = (UIButton *)sender;
    UIView *tappedView = tappedButton.superview;
    UIImageView *topImageView = nil;
    
    // Get the top image views
    for(UIView *view in tappedView.subviews) {
        if(view.tag == 1) {
            topImageView = (UIImageView *)view;
            break;
        }
    }
    
    CGRect endFrame = tappedView.frame;
    CGFloat hDiff = tappedView.frame.size.height -
    (tappedView.superview.frame.size.height - tappedView.frame.origin.y);
    if(hDiff > 0.0) {
        endFrame.origin.y -= hDiff;
        if(topImageView) {
            [topImageView setImage:[UIImage imageNamed:@"Tap_Expand_Expanded.png"]];
        }
    } else {
        switch (tappedView.tag) {
            case 1500:
            case 1501:
            case 1502:
                endFrame.origin.y = 450;
                break;
                
            default:
                endFrame.origin.y = 478;
                break;
        }
        //tappedView.superview.frame.size.height - 180;
        if(topImageView) {
            [topImageView setImage:[UIImage imageNamed:@"Tap_Expand_Contracted.png"]];
        }
    }
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:1.3];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
    
    tappedView.frame = endFrame;
    [UIView commitAnimations];
    
}

- (void)resetTapToExpand {
    UIView *tappedView;
    UIImageView *topImageView = nil;
    
    for(UIView *pageView in self.tapToExpandViews) {
        
        CGRect endFrame;
        CGFloat hDiff;
        
        switch (pageView.tag) {
            case 1222:
                tappedView = [pageView viewWithTag:1500];
                // Get the top image views
                for(UIView *view in tappedView.subviews) {
                    if(view.tag == 1) {
                        topImageView = (UIImageView *)view;
                        break;
                    }
                }
                endFrame = tappedView.frame;
                endFrame.origin.y = 450;
                if(topImageView) {
                    [topImageView setImage:[UIImage imageNamed:@"Tap_Expand_Contracted.png"]];
                }
                break;
            case 1223:
                tappedView = [pageView viewWithTag:1501];
                // Get the top image views
                for(UIView *view in tappedView.subviews) {
                    if(view.tag == 1) {
                        topImageView = (UIImageView *)view;
                        break;
                    }
                }
                endFrame = tappedView.frame;
                endFrame.origin.y = 450;
                if(topImageView) {
                    [topImageView setImage:[UIImage imageNamed:@"Tap_Expand_Contracted.png"]];
                }
                break;
            case 1224:
                tappedView = [pageView viewWithTag:1502];
                // Get the top image views
                for(UIView *view in tappedView.subviews) {
                    if(view.tag == 1) {
                        topImageView = (UIImageView *)view;
                        break;
                    }
                }
                endFrame = tappedView.frame;
                endFrame.origin.y = 450;
                if(topImageView) {
                    [topImageView setImage:[UIImage imageNamed:@"Tap_Expand_Contracted.png"]];
                }
                break;
            default:
                tappedView = [pageView viewWithTag:2000];
                // Get the top image views
                for(UIView *view in tappedView.subviews) {
                    if(view.tag == 1) {
                        topImageView = (UIImageView *)view;
                        break;
                    }
                }
                endFrame = tappedView.frame;
                hDiff = tappedView.frame.size.height -
                (tappedView.superview.frame.size.height - tappedView.frame.origin.y);
                if(hDiff > 0.0) {
                    endFrame.origin.y -= hDiff;
                    if(topImageView) {
                        [topImageView setImage:[UIImage imageNamed:@"Tap_Expand_Expanded.png"]];
                    }
                }
                break;
        }
        
        tappedView.frame = endFrame;
    }
}

- (IBAction)testButtonTapped:(id)sender {
    UIView *targetView = (UIView *)sender;
    targetView = self.testv;
    
    if(targetView.frame.size.height < 151) {
        [UIView beginAnimations:@"animationOff" context:NULL];
        [UIView setAnimationDuration:1.3f];
        [targetView setFrame:CGRectMake(targetView.frame.origin.x, targetView.frame.origin.y,
                                        targetView.frame.size.width, 151)];
        [UIView commitAnimations];
    } else {
        [UIView beginAnimations:@"animationOff" context:NULL];
        [UIView setAnimationDuration:1.3f];
        [self.testv setFrame:CGRectMake(self.testv.frame.origin.x, self.testv.frame.origin.y, self.testv.frame.size.width, 0)];
        [UIView commitAnimations];
    }
}

- (void)setupViewOverview2 {
    // Setup molecule animation
    _molecule.animationImages = [NSArray arrayWithObjects:
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00000.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00001.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00002.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00003.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00004.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00005.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00006.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00007.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00008.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00009.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00010.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00011.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00012.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00013.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00014.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00015.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00016.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00017.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00018.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00019.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00020.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00021.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00022.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00023.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00024.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00025.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00026.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00027.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00028.png"],
                                 [UIImage imageNamed:@"CEREV_RenderRegion_00029.png"]
                                 , nil];
    _molecule.animationRepeatCount  = 0;
    _molecule.animationDuration = 2;
    [_molecule startAnimating];
    
    NSString *hvcn = @"HelveticaNeueLTStd-BdCn";
    
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    NSArray *subviews = [self.page2 subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 10:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
            }
                break;
            case 20:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
            }
                break;
        }
    }
    
}

-(void)eff1chart2labelTick {
    NSString *label1Value = [[NSString alloc] initWithFormat:@"%d%% (107/177)", self.eff1label1Num];
    NSString *label2Value = [[NSString alloc] initWithFormat:@"%d%% (34/176)", self.eff1label1Num2];
    
    UILabel *label1 = (UILabel *)[self.page8 viewWithTag:110];
    UILabel *label2 = (UILabel *)[self.page8 viewWithTag:111];
    
    [label1 setText:label1Value];
    [label2 setText:label2Value];
    
    if(self.eff1label1Num2 < 19) {
        ++self.eff1label1Num2;
    }
    if(self.eff1label1Num >= 61) {
        [self.eff1AnimationTimer invalidate];
        self.eff1AnimationTimer = nil;
    } else {
        ++self.eff1label1Num;
    }
}

-(void)eff1chart2labelTick2 {
    NSString *label1Value = [[NSString alloc] initWithFormat:@"%d%% (104/176)", self.eff1label1Num];
    NSString *label2Value = [[NSString alloc] initWithFormat:@"%d%% (41/175)", self.eff1label1Num2];
    
    UILabel *label1 = (UILabel *)[self.page8 viewWithTag:110];
    UILabel *label2 = (UILabel *)[self.page8 viewWithTag:111];
    
    [label1 setText:label1Value];
    [label2 setText:label2Value];
    
    if(self.eff1label1Num2 < 23) {
        ++self.eff1label1Num2;
    }
    if(self.eff1label1Num >= 59) {
        [self.eff1AnimationTimer invalidate];
        self.eff1AnimationTimer = nil;
    } else {
        ++self.eff1label1Num;
    }
}

- (void)playEfficacy1Animation {
    // Grab the Animation Blocks
    UIView *chartBlockTTP = [self.page8 viewWithTag:2];
    UIView *chartBlockOR = [self.page8 viewWithTag:10];
    UIView *chart1Description = [self.page8 viewWithTag:130];
    UIView *chart2Description = [self.page8 viewWithTag:135];
    if(!chartBlockTTP || !chartBlockOR || !chart1Description || !chart2Description) {
        return;
    }
    
    if(self.eff1ActiveChart >= 0 && self.eff1ActiveChart < 3) {
        [chartBlockOR setHidden:YES];
        [chart2Description setHidden:YES];
        [chartBlockTTP setHidden:NO];
        [chart1Description setHidden:NO];
    } else {
        [chart1Description setHidden:YES];
        [chartBlockTTP setHidden:YES];
        [chartBlockOR setHidden:NO];
        [chart2Description setHidden:NO];
    }
    
    switch(self.eff1ActiveChart) {
        case 0: {
            // Study plots
            UIView *studyPlot = (UIView *)[chartBlockTTP viewWithTag:3];
            
            // Hide other plots in this group
            [[chartBlockTTP viewWithTag:5] setHidden:YES];
            [[chartBlockTTP viewWithTag:6] setHidden:YES];
            
            // Set left-side description to study 1, hiding study 2
            [[chart1Description viewWithTag:131] setHidden:NO];
            [[chart1Description viewWithTag:132] setHidden:YES];
            [[chart1Description viewWithTag:133] setHidden:NO];
            [[chart1Description viewWithTag:134] setHidden:YES];
            
            // Create animation with layers
            UIImageView *studyChart1 = (UIImageView *)[studyPlot viewWithTag:4];
            [studyChart1 setHidden:NO];
            
            
            CGRect endFrame;
            
            endFrame = studyPlot.frame;
            endFrame.size.width = 0;
            studyPlot.frame = endFrame;
            endFrame.size.width = 509;
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:2.5];
            [UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
            [UIView setAnimationDelegate:self];
            
            studyPlot.frame = endFrame;
            [UIView commitAnimations];
        }
            break;
        case 1: {
            // Study plots
            UIView *studyPlot = (UIView *)[chartBlockTTP viewWithTag:3];
            
            // Hide other plots in this group
            [[studyPlot viewWithTag:4] setHidden:YES];
            [[studyPlot viewWithTag:6] setHidden:YES];
            
            // Re-position study 2 description labels
            CGRect tmpFrame = [[chart1Description viewWithTag:132] frame];
            tmpFrame.origin.y = 148;
            [[chart1Description viewWithTag:132] setFrame:tmpFrame];
            
            tmpFrame = [[chart1Description viewWithTag:134] frame];
            tmpFrame.origin.y = 380;
            [[chart1Description viewWithTag:134] setFrame:tmpFrame];
            
            // Set left-side description to study 1, hiding study 2
            [[chart1Description viewWithTag:131] setHidden:YES];
            [[chart1Description viewWithTag:132] setHidden:NO];
            [[chart1Description viewWithTag:133] setHidden:YES];
            [[chart1Description viewWithTag:134] setHidden:NO];
            
            // Create animation with layers
            UIImageView *studyChart2 = (UIImageView *)[studyPlot viewWithTag:5];
            [studyChart2 setHidden:NO];
            
            
            CGRect endFrame;
            
            endFrame = studyPlot.frame;
            endFrame.size.width = 0;
            studyPlot.frame = endFrame;
            endFrame.size.width = 509;
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:2.5];
            [UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
            [UIView setAnimationDelegate:self];
            
            studyPlot.frame = endFrame;
            [UIView commitAnimations];
        }
            break;
        case 2: {
            // Study plots
            UIView *studyPlot = (UIView *)[chartBlockTTP viewWithTag:3];
            
            // Hide other plots in this group
            [[studyPlot viewWithTag:4] setHidden:YES];
            [[studyPlot viewWithTag:5] setHidden:YES];
            
            // Re-position study 2 description labels
            CGRect tmpFrame = [[chart1Description viewWithTag:132] frame];
            tmpFrame.origin.y = 214;
            [[chart1Description viewWithTag:132] setFrame:tmpFrame];
            
            tmpFrame = [[chart1Description viewWithTag:134] frame];
            tmpFrame.origin.y = 446;
            [[chart1Description viewWithTag:134] setFrame:tmpFrame];
            
            // Set left-side description to study 1, hiding study 2
            [[chart1Description viewWithTag:131] setHidden:NO];
            [[chart1Description viewWithTag:132] setHidden:NO];
            [[chart1Description viewWithTag:133] setHidden:NO];
            [[chart1Description viewWithTag:134] setHidden:NO];
            
            // Create animation with layers
            UIImageView *studyChart3 = (UIImageView *)[studyPlot viewWithTag:6];
            [studyChart3 setHidden:NO];
            
            
            CGRect endFrame;
            
            endFrame = studyPlot.frame;
            endFrame.size.width = 0;
            studyPlot.frame = endFrame;
            endFrame.size.width = 509;
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:2.5];
            [UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
            [UIView setAnimationDelegate:self];
            
            studyPlot.frame = endFrame;
            [UIView commitAnimations];
        }
            break;
        case 3: {
            // Study plot
            UIView *barChart1 = [chartBlockOR viewWithTag:84];
            UIView *barChart2 = [chartBlockOR viewWithTag:85];
            UILabel *label1 = (UILabel *)[chartBlockOR viewWithTag:110];
            UILabel *label2 = (UILabel *)[chartBlockOR viewWithTag:111];
            
            // Hide other plots in this group
            [[chartBlockOR viewWithTag:86] setHidden:YES];
            [[chartBlockOR viewWithTag:87] setHidden:YES];
            [[chartBlockOR viewWithTag:110] setHidden:YES];
            [[chartBlockOR viewWithTag:111] setHidden:YES];
            
            // Set left-side description to study 1, hiding study 2
            [[chart2Description viewWithTag:137] setHidden:YES];
            [[chart2Description viewWithTag:136] setHidden:NO];
            [[chart2Description viewWithTag:139] setHidden:YES];
            [[chart2Description viewWithTag:138] setHidden:NO];
            
            CGRect barChart1Frame, barChart1Frame2;
            CGRect labelFrame, labelFrame2;
            
            // Label 1
            labelFrame = label1.frame;
            labelFrame.origin.y = 372;
            label1.frame = labelFrame;
            labelFrame.origin.y = 223;
            
            // Label 2
            labelFrame2 = label2.frame;
            labelFrame2.origin.y = 372;
            label2.frame = labelFrame2;
            labelFrame2.origin.y = 318;
            
            // Bar chart 1
            barChart1Frame = barChart1.frame;
            barChart1Frame.origin.y = 390;
            barChart1Frame.size.height = 0;
            barChart1.frame = barChart1Frame;
            barChart1Frame.size.height = -149;
            
            // Bar chart 2
            barChart1Frame2 = barChart2.frame;
            barChart1Frame2.origin.y = 390;
            barChart1Frame2.size.height = 0;
            barChart2.frame = barChart1Frame2;
            barChart1Frame2.size.height = -54;
            
            // Unhide plots for this group
            [[chartBlockOR viewWithTag:84] setHidden:NO];
            [[chartBlockOR viewWithTag:85] setHidden:NO];
            [[chartBlockOR viewWithTag:110] setHidden:NO];
            [[chartBlockOR viewWithTag:111] setHidden:NO];
            
            self.eff1label1Num = 0;
            self.eff1label1Num2 = 0;
            if(self.eff1AnimationTimer != nil) {
                [self.eff1AnimationTimer invalidate];
                self.eff1AnimationTimer = nil;
            }
            
            self.eff1AnimationTimer = [NSTimer scheduledTimerWithTimeInterval:0.03 target:self
                                           selector:@selector(eff1chart2labelTick)
                                           userInfo:nil repeats:YES];
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:2.0];
            [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
            [UIView setAnimationDelegate:self];
            
            label1.frame = labelFrame;
            label2.frame = labelFrame2;
            barChart1.frame = barChart1Frame;
            barChart2.frame = barChart1Frame2;
            [UIView commitAnimations];
        }
            break;
        case 4: {
            // Study plot
            UIView *barChart1 = [chartBlockOR viewWithTag:86];
            UIView *barChart2 = [chartBlockOR viewWithTag:87];
            UILabel *label1 = (UILabel *)[chartBlockOR viewWithTag:110];
            UILabel *label2 = (UILabel *)[chartBlockOR viewWithTag:111];
            
            // Hide other plots in this group
            [[chartBlockOR viewWithTag:84] setHidden:YES];
            [[chartBlockOR viewWithTag:85] setHidden:YES];
            [[chartBlockOR viewWithTag:110] setHidden:YES];
            [[chartBlockOR viewWithTag:111] setHidden:YES];
            
            // Re-position (OR) study 1 description labels
            CGRect tmpFrame = [[chart2Description viewWithTag:137] frame];
            tmpFrame.origin.y = 104;
            [[chart2Description viewWithTag:137] setFrame:tmpFrame];
            
            // Set left-side description to study 1, hiding study 2
            [[chart2Description viewWithTag:136] setHidden:YES];
            [[chart2Description viewWithTag:137] setHidden:NO];
            [[chart2Description viewWithTag:138] setHidden:YES];
            [[chart2Description viewWithTag:139] setHidden:NO];
            
            CGRect barChart1Frame, barChart1Frame2;
            CGRect labelFrame, labelFrame2;
            
            // Label 1
            labelFrame = label1.frame;
            labelFrame.origin.y = 372;
            label1.frame = labelFrame;
            labelFrame.origin.y = 223;
            
            // Label 2
            labelFrame2 = label2.frame;
            labelFrame2.origin.y = 372;
            label2.frame = labelFrame2;
            labelFrame2.origin.y = 318;
            
            // Bar chart 1
            barChart1Frame = barChart1.frame;
            barChart1Frame.origin.y = 390;
            barChart1Frame.size.height = 0;
            barChart1.frame = barChart1Frame;
            barChart1Frame.size.height = -140;
            
            // Bar chart 2
            barChart1Frame2 = barChart2.frame;
            barChart1Frame2.origin.y = 390;
            barChart1Frame2.size.height = 0;
            barChart2.frame = barChart1Frame2;
            barChart1Frame2.size.height = -62;
            
            // Unhide plots for this group
            [[chartBlockOR viewWithTag:86] setHidden:NO];
            [[chartBlockOR viewWithTag:87] setHidden:NO];
            [[chartBlockOR viewWithTag:110] setHidden:NO];
            [[chartBlockOR viewWithTag:111] setHidden:NO];
            
            self.eff1label1Num = 0;
            self.eff1label1Num2 = 0;
            if(self.eff1AnimationTimer != nil) {
                [self.eff1AnimationTimer invalidate];
                self.eff1AnimationTimer = nil;
            }
            
            self.eff1AnimationTimer = [NSTimer scheduledTimerWithTimeInterval:0.03 target:self
                                                                     selector:@selector(eff1chart2labelTick2)
                                                                     userInfo:nil repeats:YES];
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:2.0];
            [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
            [UIView setAnimationDelegate:self];
            
            label1.frame = labelFrame;
            label2.frame = labelFrame2;
            barChart1.frame = barChart1Frame;
            barChart2.frame = barChart1Frame2;
            [UIView commitAnimations];
        }
            break;
    }
}

-(void)eff2chartlabelTick {
    NSString *label1Value = [[NSString alloc] initWithFormat:@"%d%%", self.eff1label1Num];
    NSString *label2Value = [[NSString alloc] initWithFormat:@"%d%%", self.eff1label1Num2];
    
    UILabel *label1 = (UILabel *)[[self.page9 viewWithTag:130] viewWithTag:4];
    UILabel *label2 = (UILabel *)[[self.page9 viewWithTag:130] viewWithTag:6];
    
    [label1 setText:label1Value];
    [label2 setText:label2Value];
    
    if(self.eff1label1Num2 < 12) {
        ++self.eff1label1Num2;
    }
    if(self.eff1label1Num >= 45) {
        [self.eff1AnimationTimer invalidate];
        self.eff1AnimationTimer = nil;
    } else {
        ++self.eff1label1Num;
    }
}

- (void)playEfficacy2Animation {
    // Grab the Animation Blocks
    UIView *targetView = [self.page9 viewWithTag:130];
    
    // Study plot
    UIView *barChart1 = [targetView viewWithTag:3];
    UIView *barChart2 = [targetView viewWithTag:5];
    UILabel *label1 = (UILabel *)[targetView viewWithTag:4];
    UILabel *label2 = (UILabel *)[targetView viewWithTag:6];
    
    [barChart1 setHidden:YES];
    [barChart2 setHidden:YES];
    [label1 setHidden:YES];
    [label2 setHidden:YES];
    
    
    CGRect barChart1Frame, barChart1Frame2;
    CGRect labelFrame, labelFrame2;
    
    // Label 1
    labelFrame = label1.frame;
    labelFrame.origin.y = 497;
    label1.frame = labelFrame;
    labelFrame.origin.y = 390;
    
    // Label 2
    labelFrame2 = label2.frame;
    labelFrame2.origin.y = 497;
    label2.frame = labelFrame2;
    labelFrame2.origin.y = 468;
    
    // Bar chart 1
    barChart1Frame = barChart1.frame;
    barChart1Frame.origin.y = 514;
    barChart1Frame.size.height = 0;
    barChart1.frame = barChart1Frame;
    barChart1Frame.size.height = -107;
    
    // Bar chart 2
    barChart1Frame2 = barChart2.frame;
    barChart1Frame2.origin.y = 514;
    barChart1Frame2.size.height = 0;
    barChart2.frame = barChart1Frame2;
    barChart1Frame2.size.height = -32;
    
    // Unhide plots for this group
    [barChart1 setHidden:NO];
    [barChart2 setHidden:NO];
    [label1 setHidden:NO];
    [label2 setHidden:NO];
    
    self.eff1label1Num = 0;
    self.eff1label1Num2 = 0;
    if(self.eff1AnimationTimer != nil) {
        [self.eff1AnimationTimer invalidate];
        self.eff1AnimationTimer = nil;
    }
    
    self.eff1AnimationTimer = [NSTimer scheduledTimerWithTimeInterval:0.03 target:self
                                                             selector:@selector(eff2chartlabelTick)
                                                             userInfo:nil repeats:YES];
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:1.8];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
    [UIView setAnimationDelegate:self];
    
    label1.frame = labelFrame;
    label2.frame = labelFrame2;
    barChart1.frame = barChart1Frame;
    barChart2.frame = barChart1Frame2;
    [UIView commitAnimations];
}


- (void)changeEfficacy1PopupStudy1:(UITapGestureRecognizer *)recognizer {
    UIView *popupPanel = [self.view viewWithTag:100];
    UIImage *backgroundImage = [UIImage imageNamed:@"m13_ttp1_pop.png"];
    UIColor *backgroundColor = [UIColor colorWithPatternImage:backgroundImage];
    
    // Set the background image of the pop-up with a color pattern
    [[popupPanel viewWithTag:101] setBackgroundColor:backgroundColor];
}

- (void)changeEfficacy1PopupStudy2:(UITapGestureRecognizer *)recognizer {
    UIView *popupPanel = [self.view viewWithTag:100];
    UIImage *backgroundImage = [UIImage imageNamed:@"m13_ttp2_pop.png"];
    UIColor *backgroundColor = [UIColor colorWithPatternImage:backgroundImage];
    
    // Set the background image of the pop-up with a color pattern
    [[popupPanel viewWithTag:101] setBackgroundColor:backgroundColor];
}

- (void)changeEfficacy1PopupStudy3:(UITapGestureRecognizer *)recognizer {
    UIView *popupPanel = [self.view viewWithTag:100];
    UIImage *backgroundImage = [UIImage imageNamed:@"m13_ttp3_pop.png"];
    UIColor *backgroundColor = [UIColor colorWithPatternImage:backgroundImage];
    
    // Set the background image of the pop-up with a color pattern
    [[popupPanel viewWithTag:101] setBackgroundColor:backgroundColor];
}

- (void)changeEfficacy1PopupStudy1b:(UITapGestureRecognizer *)recognizer {
    UIView *popupPanel = [self.view viewWithTag:100];
    UIImage *backgroundImage = [UIImage imageNamed:@"m13_or1_pop.png"];
    UIColor *backgroundColor = [UIColor colorWithPatternImage:backgroundImage];
    
    // Set the background image of the pop-up with a color pattern
    [[popupPanel viewWithTag:101] setBackgroundColor:backgroundColor];
}

- (void)changeEfficacy1PopupStudy2b:(UITapGestureRecognizer *)recognizer {
    UIView *popupPanel = [self.view viewWithTag:100];
    UIImage *backgroundImage = [UIImage imageNamed:@"m13_or2_pop.png"];
    UIColor *backgroundColor = [UIColor colorWithPatternImage:backgroundImage];
    
    // Set the background image of the pop-up with a color pattern
    [[popupPanel viewWithTag:101] setBackgroundColor:backgroundColor];
}

- (void)changeEfficacy1PopupClose:(UITapGestureRecognizer *)recognizer {
    UIView *popupPanel = [self.view viewWithTag:100];
    
    [popupPanel removeFromSuperview];
}

- (IBAction)closeLaunchPopup:(id)sender {
    //UIButton *targetButton = (UIButton *)sender;
    UIView *popupPanel = [self.view viewWithTag:5000];
    
    [popupPanel removeFromSuperview];
    
    // Hide launch screen and go to home page
    [self.launchPage setHidden:YES];
    [self setPageToSubviewTag:800];
    
}

- (IBAction)showLaunchPopup:(id)sender {
    
    // References to views of pop-up
    UIView *popupPanel = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 1024, 768)];
    UIView *popupPanelContent = [[UIView alloc] initWithFrame:CGRectMake(10, 100, 909, 685)];
    UIColor *backgroundColor;
    UIImage *backgroundImage;
    
    popupPanel.tag = 5000;
    popupPanelContent.tag = 101;
    
    // Setup pop-up panel's modal aspect (background color, opacity, etc)
    [popupPanel setBackgroundColor:[UIColor colorWithRed:0.106 green:0.239 blue:0.345 alpha:0.6]];
    
    CGRect contentFrame = self.SIAcceptView.frame;
    contentFrame.origin.x = 0;
    contentFrame.origin.y = 0;
    self.SIAcceptView.frame = contentFrame;
    
    [popupPanelContent addSubview:self.SIAcceptView];
    
    [(UIButton *)[self.SIAcceptView viewWithTag:700] setSelected:NO];
    [(UIButton *)[self.SIAcceptView viewWithTag:700] setEnabled:NO];
    [(UIScrollView *)[self.SIAcceptView viewWithTag:1] setContentOffset:CGPointZero animated:NO];
    
    // Add pop-up panel's content area to modal container
    [popupPanel addSubview:popupPanelContent];
    
    
    // Set the background image of the pop-up with a color pattern
    backgroundColor = [UIColor colorWithPatternImage:backgroundImage];
    [popupPanelContent setBackgroundColor:backgroundColor];
    
    // Reset the position of the scroll view
    [(UIScrollView *)[self.SIAcceptView viewWithTag:1] setContentOffset:CGPointMake(0,0) animated:NO];
    
    // Finally show the pop-up
    [self.view addSubview:popupPanel];
    
}

- (IBAction)closeISIInfoPopup:(id)sender {
    UIView *popupPanel = [self.view viewWithTag:5010];
    
    popupPanel.hidden = YES;
}

- (IBAction)showISIInfoPopup:(id)sender {
    UIView *popupPanel = [self.view viewWithTag:5010];
    // Reset the position of the scroll view
    [(UIScrollView *)[self.ISIInfoView viewWithTag:1] setContentOffset:CGPointMake(0,0) animated:NO];
    popupPanel.hidden = NO;
    
}

- (IBAction)toggleFooterNav:(id)sender {
    UIView *container = self.view;
    UIView *footerNav = [container viewWithTag:999];
    CGRect endFrame = footerNav.frame;
    
    if(endFrame.origin.y >= 727) {
        endFrame.origin.y = 565;
    } else {
        endFrame.origin.y = 727;
    }
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:1.0];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
    
    footerNav.frame = endFrame;
    [UIView commitAnimations];
    
}

- (IBAction)closePIPopup:(id)sender {
    //UIButton *targetButton = (UIButton *)sender;
    UIView *popupPanel = [self.view viewWithTag:5020];
    
    popupPanel.hidden = YES;
}

- (IBAction)showPIPopup:(id)sender {
    
    UIView *popupPanel = [self.view viewWithTag:5020];
    // Reset the position of the scroll view
    [[(UIWebView *)[self.PIViewPage viewWithTag:200] scrollView] setContentOffset:CGPointMake(0,0) animated:NO];
    popupPanel.hidden = NO;
    
}

- (IBAction)showEfficacy1Popup:(id)sender {
    
    // References to views of pop-up
    UIView *popupPanel = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 1024, 768)];
    UIView *popupPanelContent = [[UIView alloc] initWithFrame:CGRectMake(57, 41, 909, 685)];
    UIColor *backgroundColor;
    UIImage *backgroundImage;
    
    popupPanel.tag = 100;
    popupPanelContent.tag = 101;
    
    // Setup pop-up panel's modal aspect (background color, opacity, etc)
    [popupPanel setBackgroundColor:[UIColor colorWithRed:0.106 green:0.239 blue:0.345 alpha:0.6]];
    
    // Add pop-up panel's content area to modal container
    [popupPanel addSubview:popupPanelContent];
        
    // Depending on active animation index (aka perspective, or tab)
    // Set the revelent background image for pop-up box.
    switch (self.eff1ActiveChart) {
        case 0:
        case 1:
        case 2: {
            // setup buttons/other UI related changes for TTP
            
            if(self.eff1ActiveChart == 0) {
                backgroundImage = [UIImage imageNamed:@"m13_ttp1_pop.png"];
            } else if(self.eff1ActiveChart == 1) {
                backgroundImage = [UIImage imageNamed:@"m13_ttp2_pop.png"];
            } else {
                backgroundImage = [UIImage imageNamed:@"m13_ttp3_pop.png"];
            }
            
            // Setup Buttons
            UIView *study1Btn = [[UIView alloc] initWithFrame:CGRectMake(55, 78, 70, 39)];
            UIView *study2Btn = [[UIView alloc] initWithFrame:CGRectMake(142, 78, 70, 39)];
            UIView *study3Btn = [[UIView alloc] initWithFrame:CGRectMake(229, 78, 50, 39)];
            UIView *closeBtn = [[UIView alloc] initWithFrame:CGRectMake(853, 31, 30, 30)];
            
            //[study1Btn setBackgroundColor:[UIColor colorWithRed:1.0 green:0.0 blue:0.0 alpha:0.5]];
            //[study2Btn setBackgroundColor:[UIColor colorWithRed:0.0 green:1.0 blue:0.0 alpha:0.5]];
            //[study3Btn setBackgroundColor:[UIColor colorWithRed:0.0 green:0.0 blue:1.0 alpha:0.5]];
            //[closeBtn setBackgroundColor:[UIColor colorWithRed:1.0 green:0.0 blue:1.0 alpha:0.5]];
            
            // Add delegates for the buttons
            UITapGestureRecognizer *study1BtnDelegate = [[UITapGestureRecognizer alloc] initWithTarget:self
                                                        action:@selector(changeEfficacy1PopupStudy1:)];
            UITapGestureRecognizer *study2BtnDelegate = [[UITapGestureRecognizer alloc] initWithTarget:self
                                                        action:@selector(changeEfficacy1PopupStudy2:)];
            UITapGestureRecognizer *study3BtnDelegate = [[UITapGestureRecognizer alloc] initWithTarget:self
                                                        action:@selector(changeEfficacy1PopupStudy3:)];
            UITapGestureRecognizer *closeBtnDelegate = [[UITapGestureRecognizer alloc] initWithTarget:self
                                                        action:@selector(changeEfficacy1PopupClose:)];
            
            [study1Btn addGestureRecognizer:study1BtnDelegate];
            [study2Btn addGestureRecognizer:study2BtnDelegate];
            [study3Btn addGestureRecognizer:study3BtnDelegate];
            [closeBtn addGestureRecognizer:closeBtnDelegate];
            
            [popupPanelContent addSubview:study1Btn];
            [popupPanelContent addSubview:study2Btn];
            [popupPanelContent addSubview:study3Btn];
            [popupPanelContent addSubview:closeBtn];
            
            
            
        }
            break;
        case 3:
        case 4: {
            // setup buttons/other UI related changes for TTP
            
            if(self.eff1ActiveChart == 3) {
                backgroundImage = [UIImage imageNamed:@"m13_or1_pop.png"];
            } else {
                backgroundImage = [UIImage imageNamed:@"m13_or2_pop.png"];
            }
            
            // Setup Buttons
            UIView *study1Btn = [[UIView alloc] initWithFrame:CGRectMake(55, 75, 70, 39)];
            UIView *study2Btn = [[UIView alloc] initWithFrame:CGRectMake(142, 75, 70, 39)];
            UIView *closeBtn = [[UIView alloc] initWithFrame:CGRectMake(853, 28, 30, 30)];
            
            //[study1Btn setBackgroundColor:[UIColor colorWithRed:1.0 green:0.0 blue:0.0 alpha:0.5]];
            //[study2Btn setBackgroundColor:[UIColor colorWithRed:0.0 green:1.0 blue:0.0 alpha:0.5]];
            //[closeBtn setBackgroundColor:[UIColor colorWithRed:1.0 green:0.0 blue:1.0 alpha:0.5]];
            
            // Add delegates for the buttons
            UITapGestureRecognizer *study1BtnDelegate = [[UITapGestureRecognizer alloc] initWithTarget:self
                                                                                                action:@selector(changeEfficacy1PopupStudy1b:)];
            UITapGestureRecognizer *study2BtnDelegate = [[UITapGestureRecognizer alloc] initWithTarget:self
                                                                                                action:@selector(changeEfficacy1PopupStudy2b:)];
            UITapGestureRecognizer *closeBtnDelegate = [[UITapGestureRecognizer alloc] initWithTarget:self
                                                                                               action:@selector(changeEfficacy1PopupClose:)];
            
            [study1Btn addGestureRecognizer:study1BtnDelegate];
            [study2Btn addGestureRecognizer:study2BtnDelegate];
            [closeBtn addGestureRecognizer:closeBtnDelegate];
            
            [popupPanelContent addSubview:study1Btn];
            [popupPanelContent addSubview:study2Btn];
            [popupPanelContent addSubview:closeBtn];
        }
            break;
    }
    
    // Set the background image of the pop-up with a color pattern
    backgroundColor = [UIColor colorWithPatternImage:backgroundImage];
    [popupPanelContent setBackgroundColor:backgroundColor];
    
    // Finally show the pop-up
    [self.view addSubview:popupPanel];
    
}

- (IBAction)showEfficacy1Popup2:(id)sender {
    
    // References to views of pop-up
    UIView *popupPanel = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 1024, 768)];
    UIView *popupPanelContent = [[UIView alloc] initWithFrame:CGRectMake(78, 153, 869, 462)];
    UIColor *backgroundColor;
    UIImage *backgroundImage;
    
    popupPanel.tag = 100;
    popupPanelContent.tag = 101;
    
    // Setup pop-up panel's modal aspect (background color, opacity, etc)
    [popupPanel setBackgroundColor:[UIColor colorWithRed:0.106 green:0.239 blue:0.345 alpha:0.6]];
    
    // Add pop-up panel's content area to modal container
    [popupPanel addSubview:popupPanelContent];
    
    // Set the revelent background image for pop-up box.
    // setup buttons/other UI related changes for TTP
    backgroundImage = [UIImage imageNamed:@"eff1detail2.png"];
    
    // Setup Buttons
    UIView *closeBtn = [[UIView alloc] initWithFrame:CGRectMake(833, 5, 30, 30)];
    
    //[study1Btn setBackgroundColor:[UIColor colorWithRed:1.0 green:0.0 blue:0.0 alpha:0.5]];
    //[study2Btn setBackgroundColor:[UIColor colorWithRed:0.0 green:1.0 blue:0.0 alpha:0.5]];
    //[study3Btn setBackgroundColor:[UIColor colorWithRed:0.0 green:0.0 blue:1.0 alpha:0.5]];
    //[closeBtn setBackgroundColor:[UIColor colorWithRed:1.0 green:0.0 blue:1.0 alpha:0.5]];
    
    // Add delegates for the buttons
    UITapGestureRecognizer *closeBtnDelegate = [[UITapGestureRecognizer alloc] initWithTarget:self
                                                                                       action:@selector(changeEfficacy1PopupClose:)];
    [closeBtn addGestureRecognizer:closeBtnDelegate];
    
    [popupPanelContent addSubview:closeBtn];
    
    // Set the background image of the pop-up with a color pattern
    backgroundColor = [UIColor colorWithPatternImage:backgroundImage];
    [popupPanelContent setBackgroundColor:backgroundColor];
    
    // Finally show the pop-up
    [self.view addSubview:popupPanel];
    
}


- (IBAction)switchEfficacy1Animation:(id)sender {
    UIButton *targetButton = (UIButton *)sender;
    NSInteger viewTag = targetButton.tag;
    UIView *parentView = targetButton.superview;
    
    
    switch (viewTag) {
        case 30:
        case 31:
        case 32: {
            // References to the current block's buttons
            UIButton *study1Btn = (UIButton *)[parentView viewWithTag:30];
            UIButton *study2Btn = (UIButton *)[parentView viewWithTag:31];
            UIButton *study3Btn = (UIButton *)[parentView viewWithTag:32];
            
            // Set all of the current block's images to inactive
            [study1Btn setImage:[UIImage imageNamed:@"m13_study1_btn_inactive.png"] forState:UIControlStateNormal];
            [study2Btn setImage:[UIImage imageNamed:@"m13_study2_btn_inactive.png"] forState:UIControlStateNormal];
            [study3Btn setImage:[UIImage imageNamed:@"m13_all_btn_inactive.png"] forState:UIControlStateNormal];
            
            // Turn off all chart hints
            [[parentView viewWithTag:34] setHidden:YES];
            [[parentView viewWithTag:35] setHidden:YES];
            [[parentView viewWithTag:36] setHidden:YES];
            
            // Turn off all Adverse Reactions
            [[self.page8 viewWithTag:50] setHidden:YES];
            [[self.page8 viewWithTag:51] setHidden:YES];
            [[self.page8 viewWithTag:52] setHidden:YES];
            
            if(viewTag == 30) {
                self.eff1ActiveChart = 0;
                
                // UI related changes for the current block's animation
                [study1Btn setImage:[UIImage imageNamed:@"m13_study1_btn_active.png"] forState:UIControlStateNormal];
                [[parentView viewWithTag:34] setHidden:NO];
                [[self.page8 viewWithTag:50] setHidden:NO];
                
            } else if(viewTag == 31) {
                self.eff1ActiveChart = 1;
                
                // UI related changes for the current block's animation
                [study2Btn setImage:[UIImage imageNamed:@"m13_study2_btn_active.png"] forState:UIControlStateNormal];
                [[parentView viewWithTag:35] setHidden:NO];
                [[self.page8 viewWithTag:51] setHidden:NO];
            } else {
                self.eff1ActiveChart = 2;
                
                // UI related changes for the current block's animation
                [study3Btn setImage:[UIImage imageNamed:@"m13_all_btn_active.png"] forState:UIControlStateNormal];
                [[parentView viewWithTag:36] setHidden:NO];
                [[self.page8 viewWithTag:52] setHidden:NO];
            }
            
            [self playEfficacy1Animation];
        }
            break;
        case 80:
        case 81: {
            // References to the current block's buttons
            UIButton *study1Btn = (UIButton *)[parentView viewWithTag:80];
            UIButton *study2Btn = (UIButton *)[parentView viewWithTag:81];
            
            // Set all of the current block's images to inactive
            [study1Btn setImage:[UIImage imageNamed:@"m13_study1_btn_inactive.png"] forState:UIControlStateNormal];
            [study2Btn setImage:[UIImage imageNamed:@"m13_study2_btn_inactive.png"] forState:UIControlStateNormal];
            
            // Turn off all chart hints
            [[parentView viewWithTag:82] setHidden:YES];
            [[parentView viewWithTag:83] setHidden:YES];
            
            // Turn off all Adverse Reactions
            //[[self.page8 viewWithTag:50] setHidden:YES];
            //[[self.page8 viewWithTag:51] setHidden:YES];
            
            if(viewTag == 80) {
                self.eff1ActiveChart = 3;
                
                // UI related changes for the current block's animation
                [study1Btn setImage:[UIImage imageNamed:@"m13_study1_btn_active.png"] forState:UIControlStateNormal];
                [[parentView viewWithTag:82] setHidden:NO];
                //[[self.page8 viewWithTag:50] setHidden:NO];// adverse
                
            } else {
                self.eff1ActiveChart = 4;
                
                // UI related changes for the current block's animation
                [study2Btn setImage:[UIImage imageNamed:@"m13_study2_btn_active.png"] forState:UIControlStateNormal];
                [[parentView viewWithTag:83] setHidden:NO];
                //[[self.page8 viewWithTag:52] setHidden:NO];
            }
            
            [self playEfficacy1Animation];
        }
            break;
    }
    
}

- (void)switchEfficacy1ChartAnimation:(NSInteger)chartIndex {
    UIView *parentView = self.page8;
    
    if(chartIndex == 0) { // TTP
        // @TODO: Retain previous study
        
        // References to the current block's buttons
        UIButton *study1Btn = (UIButton *)[parentView viewWithTag:30];
        UIButton *study2Btn = (UIButton *)[parentView viewWithTag:31];
        UIButton *study3Btn = (UIButton *)[parentView viewWithTag:32];
        
        // Set all of the current block's images to inactive
        [study1Btn setImage:[UIImage imageNamed:@"m13_study1_btn_inactive.png"] forState:UIControlStateNormal];
        [study2Btn setImage:[UIImage imageNamed:@"m13_study2_btn_inactive.png"] forState:UIControlStateNormal];
        [study3Btn setImage:[UIImage imageNamed:@"m13_all_btn_inactive.png"] forState:UIControlStateNormal];
        
        // Turn off all chart hints
        [[parentView viewWithTag:34] setHidden:YES];
        [[parentView viewWithTag:35] setHidden:YES];
        [[parentView viewWithTag:36] setHidden:YES];
        
        // Turn off all Adverse Reactions
        [[self.page8 viewWithTag:50] setHidden:YES];
        [[self.page8 viewWithTag:51] setHidden:YES];
        [[self.page8 viewWithTag:52] setHidden:YES];
        
        // Set study 1 as active
        self.eff1ActiveChart = 0;
            
        // UI related changes for the current block's animation
        [study1Btn setImage:[UIImage imageNamed:@"m13_study1_btn_active.png"] forState:UIControlStateNormal];
        [[parentView viewWithTag:34] setHidden:NO];
        [[self.page8 viewWithTag:50] setHidden:NO];
    } else {
        // @TODO: Retain previous study
        
        // References to the current block's buttons
        UIButton *study1Btn = (UIButton *)[parentView viewWithTag:80];
        UIButton *study2Btn = (UIButton *)[parentView viewWithTag:81];
        
        // Set all of the current block's images to inactive
        [study1Btn setImage:[UIImage imageNamed:@"m13_study1_btn_inactive.png"] forState:UIControlStateNormal];
        [study2Btn setImage:[UIImage imageNamed:@"m13_study2_btn_inactive.png"] forState:UIControlStateNormal];
        
        // Turn off all chart hints
        [[parentView viewWithTag:82] setHidden:YES];
        [[parentView viewWithTag:83] setHidden:YES];
        
        // Turn off all TTP Adverse Reactions
        [[self.page8 viewWithTag:50] setHidden:YES];
        [[self.page8 viewWithTag:51] setHidden:YES];
        [[self.page8 viewWithTag:52] setHidden:YES];
        
        // Set study 1 as active
        self.eff1ActiveChart = 3;
        
        // UI related changes for the current block's animation
        [study1Btn setImage:[UIImage imageNamed:@"m13_study1_btn_active.png"] forState:UIControlStateNormal];
        [[parentView viewWithTag:82] setHidden:NO];
    }
    [self playEfficacy1Animation];
}


- (IBAction)switchEfficacy1Chart:(id)sender {
    UIButton *targetButton = (UIButton *)sender;
    NSInteger viewTag = targetButton.tag;
    UIView *parentView = targetButton.superview;
    
    
    switch (viewTag) { //
        case 70: {
            // Change TTP/OR tab button colors
            [targetButton setTitleColor:[UIColor colorWithRed:1.0 green:0.77 blue:0.14 alpha:1.0] forState:UIControlStateNormal];
            [(UIButton *)[parentView viewWithTag:71] setTitleColor:[UIColor colorWithRed:0.004 green:0.659 blue:0.925 alpha:1.0] forState:UIControlStateNormal];
            
            // Flip to TTP chart and play animation
            [self switchEfficacy1ChartAnimation:0];
        }
            break;
        case 71: {
            // Change TTP/OR tab button colors
            [targetButton setTitleColor:[UIColor colorWithRed:1.0 green:0.77 blue:0.14 alpha:1.0] forState:UIControlStateNormal];
            [(UIButton *)[parentView viewWithTag:70] setTitleColor:[UIColor colorWithRed:0.004 green:0.659 blue:0.925 alpha:1.0] forState:UIControlStateNormal];
            
            // Flip to TTP chart and play animation
            [self switchEfficacy1ChartAnimation:1];
        }
            break;
    }
}
- (void)setupEfficacy1 {
    self.eff1ActiveChart = 0;
    
    //NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    UIButton *btn;
    UILabel *lbl;
    UIView *view;
    
    UIView *container = self.page8;
    
    for(UIView *subview in container.subviews) {
        switch (subview.tag) {
            case 70:
            case 71:
                // fix font for buttons
                btn = (UIButton *)subview;
                if(lbl.tag != 222) {
                    btn.titleLabel.font = [UIFont fontWithName:mcdn size:btn.titleLabel.font.pointSize];
                }
                break;
            case 2:
            {
                // Iterate through subviews
                for(UIView *chartBlockTTPElement in subview.subviews) {
                    switch (chartBlockTTPElement.tag) {
                        case 34:
                        case 35:
                        case 36:
                            // Iterate through view's children and set font
                            for(view in chartBlockTTPElement.subviews) {
                             // Check for label, if it is, then set the font
                                if([view class] == [UILabel class]) {
                                    // fix font for label
                                    lbl = (UILabel *)view;
                                    if(lbl.tag != 222) {
                                        lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                                    }
                                }
                            }
                            break;
                    }
                }
            }
                break;
            case 10:
            {
                // Iterate through subviews
                for(UIView *chartBlockTTPElement in subview.subviews) {
                    switch (chartBlockTTPElement.tag) {
                        case 82:
                        case 83:
                            // Iterate through view's children and set font
                            for(view in chartBlockTTPElement.subviews) {
                                // Check for label, if it is, then set the font
                                if([view class] == [UILabel class]) {
                                    // fix font for label
                                    lbl = (UILabel *)view;
                                    if(lbl.tag != 222) {
                                        lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                                    }
                                }
                            }
                            break;
                        case 110:
                        case 111:
                            lbl = (UILabel *)chartBlockTTPElement;
                            if(lbl.tag != 222) {
                                lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                            }
                            break;

                    }
                }
            }
                break;
            case 50:
            case 51:
            case 52:
            {
                // Iterate through view's children and set font
                for(view in subview.subviews) {
                    // Check for label, if it is, then set the font
                    if([view class] == [UILabel class]) {
                        // fix font for label
                        lbl = (UILabel *)view;
                        if(lbl.tag == 1) {
                            lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                        } else {
                            if(lbl.tag != 222) {
                                lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                            }
                        }
                    }
                }
            }
                break;
            case 130:
            {
                // Iterate through subviews
                for(UIView *chartBlockTTPElement in subview.subviews) {
                    if([chartBlockTTPElement class] == [UILabel class]) {
                        // fix font for label
                        lbl = (UILabel *)chartBlockTTPElement;
                        if(lbl.tag == 1) {
                            lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                        } else {
                            if(lbl.tag != 222) {
                                lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                            }
                        }
                    }
                    switch (chartBlockTTPElement.tag) {
                        case 131:
                        case 132:
                        case 133:
                        case 134:
                            // Iterate through view's children and set font
                            for(view in chartBlockTTPElement.subviews) {
                                // Check for label, if it is, then set the font
                                if([view class] == [UILabel class]) {
                                    // fix font for label
                                    lbl = (UILabel *)view;
                                    
                                    if(lbl.tag != 222) {
                                        lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                                    }
                                }
                            }
                            break;
                            
                    }
                }
            }
                break;
            case 135:
            {
                // Iterate through subviews
                for(UIView *chartBlockTTPElement in subview.subviews) {
                    if([chartBlockTTPElement class] == [UILabel class]) {
                        // fix font for label
                        lbl = (UILabel *)chartBlockTTPElement;
                        if(lbl.tag == 1) {
                            lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                        } else {
                            if(lbl.tag != 222) {
                                lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                            }
                        }
                    }
                    switch (chartBlockTTPElement.tag) {
                        case 136:
                        case 137:
                            // Iterate through view's children and set font
                            for(view in chartBlockTTPElement.subviews) {
                                // Check for label, if it is, then set the font
                                if([view class] == [UILabel class]) {
                                    // fix font for label
                                    lbl = (UILabel *)view;
                                    if(lbl.tag != 222) {
                                        lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                                    }
                                }
                            }
                            break;
                        case 138:
                        case 139:
                            // Iterate through view's children and set font
                            for(view in chartBlockTTPElement.subviews) {
                                // Check for label, if it is, then set the font
                                if([view class] == [UILabel class]) {
                                    // fix font for label
                                    lbl = (UILabel *)view;
                                    if(lbl.tag == 1) {
                                        lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                                    } else {
                                        if(lbl.tag != 222) {
                                            lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                                        }
                                    }
                                }
                            }
                            break;
                            
                    }
                }
            }
                break;
        }
    }
}

- (void)setupDosing1 {
    [self.page15 addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleDosing1Tap:)]];
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    
    NSArray *subviews = [self.page15 subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 1:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
                }
            }
                break;
            case 123:
            {
                for(UIView *view in [[subviews objectAtIndex:i] subviews]) {
                    if([view class] == [UILabel class]) {
                        lbl = (UILabel *)view;
                        if(lbl.tag == 1) {
                            lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
                        } else {
                            lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
                        }
                    }
                }
            }
                break;
            case 3:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
            }
            break;
            default:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
                }
            }
                break;
        }
    }
}

- (void)setupDosing2 {
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    
    NSArray *subviews = [[self.page16 viewWithTag:130] subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 5688:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
            }
                break;
            case 9815:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
            }
                break;
            case 3038:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
            }
                break;
        }
    }
    
}

- (void)setupDosing3 {
    [self.page17 addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleDosing3Tap:)]];
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    //NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    NSArray *subviews = [self.page17 subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 1:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
                }
            }
                break;
            case 2:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
                }
            }
            break;
            default:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                }
            }
                break;
        }
    }
}

- (void)setupDosing5 {
    [self.page19 addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleDosing3Tap:)]];
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    NSArray *subviews = [[self.page19 viewWithTag:2] subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 1:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
                }
            }
                break;
            default:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                }
            }
                break;
        }
    }
}

- (void)setupDosing4 {
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    NSString *mdcn = @"HelveticaNeueLTStd-HvCn";
    
    NSArray *subviews = [self.page18 subviews];
    NSArray *subviews2;
    NSInteger len = 0, len2 = 0;
    NSInteger i = 0, i2 = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 50: case 51:
            {
                subviews2 = [[subviews objectAtIndex:i] subviews];
                len2 = [subviews2 count];
                for(i2=0; i2<len2; ++i2) {
                    if([[subviews2 objectAtIndex:i2] class] == [UILabel class]) {
                        lbl = (UILabel *)[subviews2 objectAtIndex:i2];
                        lbl.font = [UIFont fontWithName:mdcn size:lbl.font.pointSize];
                    }
                }
            }
                break;
            case 357:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
            }
                break;
            case 754:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:mdcn size:lbl.font.pointSize];
            }
                break;
            case 2356:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
            }
                break;
        }
    }
    
}

- (void)dosing4Animation1Done {
    UIView *animBlock1 = [self.page18 viewWithTag:50];
    UIView *item1 = [animBlock1 viewWithTag:40];
    UIView *item2 = [animBlock1 viewWithTag:41];
    UIView *item3 = [animBlock1 viewWithTag:42];
    UIView *item4 = [animBlock1 viewWithTag:43];
    
    // Fade out all the items in current animation block
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:2.0];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
    [UIView setAnimationDelegate:self];
    
    [item1 setAlpha:0.0];
    [item2 setAlpha:0.0];
    [item3 setAlpha:0.0];
    [item4 setAlpha:0.0];
    
    [UIView commitAnimations];
}

- (void)dosing4Animation2Done {
    UIView *animBlock2 = [self.page18 viewWithTag:51];
    UIView *item1 = [animBlock2 viewWithTag:40];
    UIView *item2 = [animBlock2 viewWithTag:41];
    UIView *item3 = [animBlock2 viewWithTag:42];
    UIView *item4 = [animBlock2 viewWithTag:43];
    
    // Fade out all the items in current animation block
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:2.0];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
    [UIView setAnimationDelegate:self];
    
    [item1 setAlpha:0.0];
    [item2 setAlpha:0.0];
    [item3 setAlpha:0.0];
    [item4 setAlpha:0.0];
    
    [UIView commitAnimations];
}

- (void)playDosing4Animation {
    
    // Get references to the two animation blocks
    UIView *animBlock1 = [self.page18 viewWithTag:50];
    UIView *animBlock2 = [self.page18 viewWithTag:51];
    
    switch(self.dosing4Chart) {
        case 0:
        {
            [animBlock2 setHidden:YES];
            // Get The wave of animation block
            UIView *wave = [animBlock1 viewWithTag:3];
            UIView *item1 = [animBlock1 viewWithTag:40];
            UIView *item2 = [animBlock1 viewWithTag:41];
            UIView *item3 = [animBlock1 viewWithTag:42];
            UIView *item4 = [animBlock1 viewWithTag:43];
            
            [item1 setAlpha:1.0];
            [item2 setAlpha:1.0];
            [item3 setAlpha:1.0];
            [item4 setAlpha:1.0];
            
            CGRect waveFrame = wave.frame;
            waveFrame.size.width = 0;
            wave.frame = waveFrame;
            waveFrame.size.width = 779;
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:2.0];
            [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
            [UIView setAnimationDidStopSelector:@selector(dosing4Animation1Done)];
            [UIView setAnimationDelegate:self];
            
            wave.frame = waveFrame;
            
            [wave setHidden:NO];
            [animBlock1 setHidden:NO];
            
            [UIView commitAnimations];
        }
            break;
        case 1: {
            [animBlock1 setHidden:YES];
            // Get The wave of animation block
            UIView *wave = [animBlock2 viewWithTag:3];
            UIView *item1 = [animBlock2 viewWithTag:40];
            UIView *item2 = [animBlock2 viewWithTag:41];
            UIView *item3 = [animBlock2 viewWithTag:42];
            UIView *item4 = [animBlock2 viewWithTag:43];
            
            [item1 setAlpha:1.0];
            [item2 setAlpha:1.0];
            [item3 setAlpha:1.0];
            [item4 setAlpha:1.0];
            
            CGRect waveFrame = wave.frame;
            waveFrame.size.width = 0;
            wave.frame = waveFrame;
            waveFrame.size.width = 779;
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:2.0];
            [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
            [UIView setAnimationDidStopSelector:@selector(dosing4Animation2Done)];
            [UIView setAnimationDelegate:self];
            
            wave.frame = waveFrame;
            
            [wave setHidden:NO];
            [animBlock2 setHidden:NO];
            
            [UIView commitAnimations];
        }
            break;
    }
}

- (IBAction)showDosingItem:(id)sender {
    UIButton *targetButton = (UIButton *)sender;
    UIView *animBlock = targetButton.superview;
    
    UIView *item1 = [animBlock viewWithTag:40];
    UIView *item2 = [animBlock viewWithTag:41];
    UIView *item3 = [animBlock viewWithTag:42];
    UIView *item4 = [animBlock viewWithTag:43];
    
    
    
    switch(targetButton.tag) {
        case 44:
        {
            [item1 setAlpha:1.0];
            [item2 setAlpha:0.0];
            [item3 setAlpha:0.0];
            [item4 setAlpha:0.0];
        }
            break;
        case 45:
        {
            [item1 setAlpha:0.0];
            [item2 setAlpha:1.0];
            [item3 setAlpha:0.0];
            [item4 setAlpha:0.0];
        }
            break;
        case 46:
        {
            [item1 setAlpha:0.0];
            [item2 setAlpha:0.0];
            [item3 setAlpha:1.0];
            [item4 setAlpha:0.0];
        }
            break;
        case 47:
        {
            [item1 setAlpha:0.0];
            [item2 setAlpha:0.0];
            [item3 setAlpha:0.0];
            [item4 setAlpha:1.0];
        }
            break;
        case 48:
        {
            [item1 setAlpha:0.0];
            [item2 setAlpha:0.0];
            [item3 setAlpha:0.0];
            [item4 setAlpha:1.0];
        }
            break;
    }
}

- (IBAction)switchDosing4Animation:(id)sender {
    UIButton *targetbutton = (UIButton *)sender;
    
    switch(targetbutton.tag) {
        case 20:
        {
            self.dosing4Chart = 0;
            
            UIButton *button2 = (UIButton *)[self.page18 viewWithTag:21];
            [button2 setImage:[UIImage imageNamed:@"M1.5C_v2_dosing_for_grade34_neu_btn_inactive.png"] forState:UIControlStateNormal];
            [targetbutton setImage:[UIImage imageNamed:@"M1.5C_v2_dosing_for_grade34_throm_btn_active.png"] forState:UIControlStateNormal];
            [self playDosing4Animation];
        }
            break;
        case 21:
        {
            self.dosing4Chart = 1;
            
            UIButton *button2 = (UIButton *)[self.page18 viewWithTag:20];
            [button2 setImage:[UIImage imageNamed:@"M1.5C_v2_dosing_for_grade34_throm_btn_inactive.png"] forState:UIControlStateNormal];
            [targetbutton setImage:[UIImage imageNamed:@"M1.5C_v2_dosing_for_grade34_neu_btn_active.png"] forState:UIControlStateNormal];
            [self playDosing4Animation];
        }
            break;
    }
}

- (void)addToChartAccordion:(AccordionView *)accordion panelTitle:(NSString *)title
             chartImageName:(NSString *)imageName charHeight:(CGFloat)height {
    // Temp variables
    UIImageView *chartView;
    NSString *chartFilepath;
    UIImage *chart;
    
    // Only height is taken into account, so other parameters are just dummy
    UIButton *btnPanelTitle = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, 0, 35)];
    [btnPanelTitle setBackgroundImage:[UIImage imageNamed:@"M1.2A_Dk_Blue_row.png"]
                                         forState:UIControlStateNormal];
    [[btnPanelTitle titleLabel] setFont:[UIFont fontWithName:@"HelveticaNeue-CondensedBold" size:19]];
    
    [btnPanelTitle setTitleColor:[UIColor colorWithRed:1.0 green:0.768 blue:0.137 alpha:1.0] forState:UIControlStateNormal];
    [btnPanelTitle setTitleColor:[UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:1.0] forState:UIControlStateSelected];
    
    [btnPanelTitle setTitle:title forState:UIControlStateNormal];
    [btnPanelTitle setTitleEdgeInsets:UIEdgeInsetsMake(0.0, 15.0, 0.0, 0.0)];
    
    btnPanelTitle.contentHorizontalAlignment = UIControlContentHorizontalAlignmentLeft;
    
    UIView *panelChart = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, height)];
    chartView = [[UIImageView alloc] initWithFrame:CGRectMake(2, 0, 931, height)];
    chartFilepath = [[NSBundle mainBundle] pathForResource:imageName ofType:@"jpg"];
    chart = [[UIImage alloc] initWithContentsOfFile:chartFilepath];
    [chartView setImage:chart];
    [panelChart addSubview:chartView];
    
    [accordion addHeader:btnPanelTitle withView:panelChart];
}

- (void)setupStudyHighlight1 {
    
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    
    NSArray *subviews = [self.page3 subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 10:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
            }
                break;
            case 20:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
            }
                break;
            case 30:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
            }
                break;
        }
    }
    
}

- (void)setupStudyHighlight2 {
    
    self.accordion = [[AccordionView alloc] initWithFrame:CGRectMake(0, 0, 931, 600)];
    self.accordion.startsClosed = YES;
    self.accordion.tag = 1;
    [[self.page4 viewWithTag:1] addSubview:self.accordion];
    
    // Add panels to accordion
    [self addToChartAccordion:self.accordion panelTitle:@"Patient Characteristics"
          chartImageName:@"M1.2A_age_sex_race_ECOG_all4rows" charHeight:214];
    
    [self addToChartAccordion:self.accordion panelTitle:@"Disease Characteristics"
               chartImageName:@"M1.2A_multiple_B2_all2rows" charHeight:160];
    
    [self addToChartAccordion:self.accordion panelTitle:@"Number of Prior Therapies"
               chartImageName:@"M1.2A_1_2_all2rows" charHeight:70];
    
    [self addToChartAccordion:self.accordion panelTitle:@"Types of Prior Therapies"
               chartImageName:@"M1.2A_stem_thalid_dex_bort_Mel_dox_all6rows" charHeight:205];
    
    // Direct the accordion view to tell it's implicit layer to redraw
    [self.accordion setNeedsLayout];
    
    // Set this to 'YES' if you want to allow multiple accordions to be opened at once
    [self.accordion setAllowsMultipleSelection:NO];
    
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    
    NSArray *subviews = [[[self.page4 viewWithTag:200010] viewWithTag:200020] subviews];

    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    lbl = (UILabel *)[self.page4 viewWithTag:1234];
    lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
    
    
    for(i=0; i<len; ++i) {
        if([[subviews objectAtIndex:i] class] == [UILabel class]) {
            lbl = (UILabel *)[subviews objectAtIndex:i];
            lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
        }
    }
    
    
}

- (void)setupStudyHighlight3 {
    self.studyHighlight3AnimBlockTap =
    [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleStudyHighlight3AnimTap:)];
    [[self.page5 viewWithTag:10] setHidden:YES];
    
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    NSArray *subviews = [self.page5  subviews];
    
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    lbl = (UILabel *)[self.page4 viewWithTag:30];
    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
    
    
    for(i=0; i<len; ++i) {
        if([[subviews objectAtIndex:i] class] == [UILabel class] && [[subviews objectAtIndex:i] tag] != 30) {
            lbl = (UILabel *)[subviews objectAtIndex:i];
            lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
        }
    }
}

- (void)setupStudyHighlight4 {
    
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    NSArray *subviews = [[self.page6 viewWithTag:4321] subviews];
    
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        lbl = (UILabel *)[subviews objectAtIndex:i];
        lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
    }
}

- (void)setupStudyHighlight5 {
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    
    NSArray *subviews = [[self.page7 viewWithTag:2] subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    [self makeLabelIntoLink:(UILabel *)[[self.page7 viewWithTag:2] viewWithTag:3] parentView:self.page7];
    
    lbl = (UILabel *)[self.page7 viewWithTag:1];
    
    // Set font of title
    lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
    
    for(i=0; i<len; ++i) {
        if([[subviews objectAtIndex:i] class] == [UILabel class]) {
            lbl = (UILabel *)[subviews objectAtIndex:i];
            lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
        }
    }
}

- (void)setupEfficacy2 {
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    NSArray *subviews = [[self.page9 viewWithTag:130] subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 1:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
            }
                break;
            case 10:
            {
                for(UIView *view in [[subviews objectAtIndex:i] subviews]) {
                    if([view class] == [UILabel class]) {
                        lbl = (UILabel *)view;
                        lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
                    }
                }
            }
                break;
            case 51:
            {
                for(UIView *view in [[subviews objectAtIndex:i] subviews]) {
                    if([view class] == [UILabel class]) {
                        lbl = (UILabel *)view;
                        if(lbl.tag == 1) {
                            lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                        } else {
                         lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                        }
                    }
                }
            }
                break;
            default:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
                }
            }
                break;
        }
    }
}

- (void)setupEfficacy3 {
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    NSArray *subviews = [self.page10 subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        if([[subviews objectAtIndex:i] class] == [UILabel class]) {
            lbl = (UILabel *)[subviews objectAtIndex:i];
            if(lbl.tag == 1) {
                lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
            } else {
                lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
            }
        }
    }
}

- (void)setupEfficacy4 {
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    NSArray *subviews = [[self.page11 viewWithTag:5] subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        if([[subviews objectAtIndex:i] class] == [UILabel class]) {
            lbl = (UILabel *)[subviews objectAtIndex:i];
            if(lbl.tag == 1) {
                lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
            } else {
                lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
            }
        }
    }
}

- (void)setupRevlimidPIPDF {
    UIWebView *webView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 110, 1024, 605)];
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"PI2" ofType:@"pdf"];
    NSURL *targetURL = [NSURL fileURLWithPath:path];
    NSURLRequest *request = [NSURLRequest requestWithURL:targetURL];
    [webView loadRequest:request];
    
    webView.scrollView.bounces = NO;
    [webView.scrollView setBouncesZoom:NO];
    
    [webView setBackgroundColor:[UIColor clearColor]];
    webView.tag = 802;
    [webView setHidden:YES];
    
    [self.view addSubview:webView];
}

- (void)setupLaunchPage {
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    
    NSArray *subviews = [self.launchPage subviews];
    NSArray *subviews2;
    NSInteger len = 0, len2 = 0;
    NSInteger i = 0, i2 = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 150:
            {
                // Transverse through children and set font
                subviews2 = [[subviews objectAtIndex:i] subviews];
                len2 = [subviews2 count];
                
                for(i2=0; i2<len2; ++i2) {
                    if([[subviews2 objectAtIndex:i2] class] == [UILabel class]) {
                        lbl = (UILabel *)[subviews2 objectAtIndex:i2];
                        lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
                    }
                }
            }
                break;
            case 151:
            {
                // Transverse through children and set font
                subviews2 = [[subviews objectAtIndex:i] subviews];
                len2 = [subviews2 count];
                
                for(i2=0; i2<len2; ++i2) {
                    if([[subviews2 objectAtIndex:i2] tag] != 342350) {
                        if([[subviews2 objectAtIndex:i2] class] == [UILabel class]) {
                            lbl = (UILabel *)[subviews2 objectAtIndex:i2];
                            lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                        }
                    }
                }
            }
                break;
        }
    }
    
}

- (void)setupPIPopupView {
    // References to views of pop-up
    UIView *popupPanel = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 1024, 768)];
    UIView *popupPanelContent = [[UIView alloc] initWithFrame:CGRectMake(10, 100, 1010, 685)];
    
    popupPanel.tag = 5020;
    popupPanelContent.tag = 101;
    
    // Setup pop-up panel's modal aspect (background color, opacity, etc)
    [popupPanel setBackgroundColor:[UIColor colorWithRed:0.106 green:0.239 blue:0.345 alpha:0.6]];
    
    CGRect contentFrame = self.PIViewPage.frame;
    contentFrame.origin.x = 0;
    contentFrame.origin.y = 0;
    self.PIViewPage.frame = contentFrame;
    
    UIWebView *webView = (UIWebView *)[self.PIViewPage viewWithTag:200];
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"PI" ofType:@"pdf"];
    NSURL *targetURL = [NSURL fileURLWithPath:path];
    NSURLRequest *request = [NSURLRequest requestWithURL:targetURL];
    [webView loadRequest:request];
    
    webView.scrollView.bounces = NO;
    [webView.scrollView setBouncesZoom:NO];
    
    [webView setBackgroundColor:[UIColor clearColor]];

    
    [popupPanelContent addSubview:self.PIViewPage];
    // Add pop-up panel's content area to modal container
    [popupPanel addSubview:popupPanelContent];
    
    popupPanel.hidden = YES;
    
    // Finally show the pop-up
    [self.view addSubview:popupPanel];
    
}

- (void)setupRevlimidOtherViews {
    CGRect rect;
    
    // Launch Page
    rect = self.launchPage.frame;
    rect.origin.x = 0;
    rect.origin.y = 0;
    
    self.launchPage.frame = rect;
    self.launchPage.tag = 807;
    [self.view addSubview:self.launchPage];
    
    // Menu Page
    rect = self.revlimidMenuPage.frame;
    rect.origin.x = 0;
    rect.origin.y = 126;
    
    self.revlimidMenuPage.frame = rect;
    self.revlimidMenuPage.tag = 803;
    [self.view addSubview:self.revlimidMenuPage];
    
    // Refernces Page
    rect = self.revlimidReferencePage.frame;
    rect.origin.x = 0;
    rect.origin.y = 126;
    
    self.revlimidReferencePage.frame = rect;
    self.revlimidReferencePage.tag = 804;
    [self.view addSubview:self.revlimidReferencePage];
    
    // Celegene Support Page
    rect = self.celegeneSupportPage.frame;
    rect.origin.x = 0;
    rect.origin.y = 40;
    
    self.celegeneSupportPage.frame = rect;
    self.celegeneSupportPage.tag = 805;
    [self.view insertSubview:self.celegeneSupportPage atIndex:1];
    
    // Important Safety Information Page
    rect = self.safetyInformationPage.frame;
    rect.origin.x = 0;
    rect.origin.y = 100;
    
    self.safetyInformationPage.frame = rect;
    self.safetyInformationPage.tag = 806;
    self.safetyInformationPage.hidden = YES;
    [self.view insertSubview:self.safetyInformationPage atIndex:1];
}

- (void)handleDosing1Tap:(UITapGestureRecognizer *)recognizer {
    // Reset highlight/transparent state of all charts
    [[self.page15 viewWithTag:16] setHidden:YES];
    [[self.page15 viewWithTag:17] setHidden:YES];
    
    [[self.page15 viewWithTag:10] setBackgroundColor:[UIColor clearColor]];
    [[self.page15 viewWithTag:11] setBackgroundColor:[UIColor clearColor]];
}

- (void)handleDosing3Tap:(UITapGestureRecognizer *)recognizer {
    // Reset highlight/transparent state of all charts
    [[self.page17 viewWithTag:20] setHidden:YES];
    [[self.page17 viewWithTag:21] setHidden:YES];
    [[self.page17 viewWithTag:22] setHidden:YES];
    
    [[self.page17 viewWithTag:5] setBackgroundColor:[UIColor clearColor]];
    [[self.page17 viewWithTag:6] setBackgroundColor:[UIColor clearColor]];
    [[self.page17 viewWithTag:7] setBackgroundColor:[UIColor clearColor]];
}

- (void)awakeFromNib
{
    //set up data
    //your swipeView should always be driven by an array of
    //data of some kind - don't store data in your item views
    //or the recycling mechanism will destroy your data once
    //your item views move off-screen
    self.items = [NSMutableArray array];
    for (int i = 0; i < 19; i++)
    {
        [_items addObject:@(i)];
    }
}

- (void)dealloc
{
    //it's a good idea to set these to nil here to avoid
    //sending messages to a deallocated viewcontroller
    //this is true even if your project is using ARC, unless
    //you are targeting iOS 5 as a minimum deployment target
    _swipeView.delegate = nil;
    _swipeView.dataSource = nil;
    _swipeView2.delegate = nil;
    _swipeView2.dataSource = nil;
}

- (void)resizeLabelToFitWidth:(CGFloat)constrainedWidth Label:(UILabel *)lbl {
    CGSize sizeOfText;
    CGRect lblFrame;
    NSString *fontName;
    CGFloat fontSize;
    
    fontName = lbl.font.fontName;
    fontSize = lbl.font.pointSize;
    
    sizeOfText = [lbl.text sizeWithFont:[UIFont fontWithName:fontName size:fontSize] constrainedToSize:CGSizeMake(constrainedWidth, CGFLOAT_MAX) lineBreakMode:UILineBreakModeWordWrap];
    sizeOfText.height += 2;
    
    lblFrame = lbl.frame;
    lblFrame.size = sizeOfText;
    lbl.frame = lblFrame;
    
}
- (void)setupCelgeneSupport {
    UIView *container = self.celegeneSupportPage;
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    UILabel *lbl;
    NSInteger i = 0, len = 0;
    
    // Dedicated, Central...
    lbl = (UILabel *)[container viewWithTag:50];
    lbl.font = [UIFont fontWithName:mcdn size:23];
    [self resizeLabelToFitWidth:424.0 Label:lbl];
    
    // Dedicated, Central... - sub
    lbl = (UILabel *)[container viewWithTag:51];
    lbl.font = [UIFont fontWithName:mcdn size:17];
    [self resizeLabelToFitWidth:470 Label:lbl];
    
    // Dedicated, Central... - sub2
    lbl = (UILabel *)[container viewWithTag:52];
    lbl.font = [UIFont fontWithName:mcdn size:17];
    [self resizeLabelToFitWidth:470 Label:lbl];
    
    // Celgene Support Title...
    lbl = (UILabel *)[container viewWithTag:53];
    lbl.font = [UIFont fontWithName:mcdn size:18];
    [self resizeLabelToFitWidth:230 Label:lbl];
    
    // Celgene Support (right text)...
    for(i=54; i<=61; ++i) {
        lbl = (UILabel *)[container viewWithTag:i];
        lbl.font = [UIFont fontWithName:mcdn size:18];
        [self resizeLabelToFitWidth:230 Label:lbl];
    }
    
    // @TODO: Refactor this! Tabbed content...
    for(i=62; i<=65; ++i) {
        lbl = (UILabel *)[container viewWithTag:i];
        lbl.font = [UIFont fontWithName:mcdn size:18];
        [self resizeLabelToFitWidth:380 Label:lbl];
    }
    
    // Tabbed content new grouping mech...
    NSArray *subviews = [container subviews];
    NSArray *subviews2;
    NSInteger len2 = 0;
    NSInteger i2 = 0;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 66:
            case 67:
            case 68:
            case 69:
            {
                // Transverse through children and set font
                subviews2 = [[subviews objectAtIndex:i] subviews];
                len2 = [subviews2 count];
                
                for(i2=0; i2<len2; ++i2) {
                    lbl = (UILabel *)[subviews2 objectAtIndex:i2];
                    lbl.font = [UIFont fontWithName:mcdn size:18];
                    [self resizeLabelToFitWidth:450 Label:lbl];
                }
            }
                break;
            case 70:
            {
                // Transverse through children and set font
                subviews2 = [[subviews objectAtIndex:i] subviews];
                len2 = [subviews2 count];
                
                for(i2=0; i2<len2; ++i2) {
                    lbl = (UILabel *)[subviews2 objectAtIndex:i2];
                    lbl.font = [UIFont fontWithName:mcdn size:26];
                    [self resizeLabelToFitWidth:450 Label:lbl];
                }
            }
                break;
            case 400:
            {
                // Transverse through children and set font
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:mcdn size:12];
                [self resizeLabelToFitWidth:10 Label:lbl];
            }
                break;
        }
    }
    
}

- (void)setupFontForTapToExpand:(UIView *)pageView {
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    NSArray *subviews;
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    
    // Transverse through children and set font
    if(pageView == self.page12) {
        subviews = [[[pageView viewWithTag:1500] viewWithTag:2001] subviews];
    } else if(pageView == self.page13) {
        subviews = [[[pageView viewWithTag:1501] viewWithTag:2001] subviews];
    } else if(pageView == self.page14) {
        subviews = [[[pageView viewWithTag:1502] viewWithTag:2001] subviews];
    } else {
        subviews = [[[pageView viewWithTag:2000] viewWithTag:2001] subviews];
    }
    
    len = [subviews count];
    for(i=0; i<len; ++i) {
        if([[subviews objectAtIndex:i] class] == [UILabel class]) {
            lbl = (UILabel *)[subviews objectAtIndex:i];
            if(lbl.tag == 10) {
                lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
            } else {
                lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
            }
        }
    }
}

- (void)setupTapToExpand {
    // Study Highlight Views
    [self setupFontForTapToExpand:self.page3];
    [self setupFontForTapToExpand:self.page4];
    [self setupFontForTapToExpand:self.page5];
    [self setupFontForTapToExpand:self.page6];
    
    // Adverse Effects
    [self setupFontForTapToExpand:self.page12];
    [self setupFontForTapToExpand:self.page13];
    [self setupFontForTapToExpand:self.page14];
    
    [self setupFontForTapToExpand:self.revlimidInfoPage3];
}

- (void)setupImportantSafetyInfoPage {
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    NSInteger i, i2, len = 0, len2 = 0;
    UIView *container = self.safetyInformationPage;
    NSArray *subviews = [[container viewWithTag:1] subviews], *subviews2;
    UILabel *lbl;
    
    // Setup scroll view
    [self.safetyInfoScroller setScrollEnabled:YES];
    [self.safetyInfoScroller setContentSize:CGSizeMake(995, 5600)];
    
    len = [subviews count];
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 1:
            {
                // Transverse through children and set font
                subviews2 = [[subviews objectAtIndex:i] subviews];
                len2 = [subviews2 count];
                
                for(i2=0; i2<len2; ++i2) {
                    if([[subviews2 objectAtIndex:i2] class] == [UILabel class]) {
                        lbl = (UILabel *)[subviews2 objectAtIndex:i2];
                        lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                    }
                    if([[subviews2 objectAtIndex:i2] tag] == 5632) {
                        UIView *bodyView = [subviews objectAtIndex:i];
                        UILabel *linkLabel = (UILabel *)[subviews2 objectAtIndex:i2];
                        [self makeLabelIntoLink:linkLabel parentView:bodyView];
                    }
                }
            }
                break;
            case 2:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                }
            }
                break;
            case 3:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    [self makeLabelIntoLink:lbl parentView:[container viewWithTag:1]];
                }
            }
                break;
            default:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                }
            }
                break;
        }
    }
}

-(void)scrollViewDidEndScrollingAnimation:(UIScrollView *)newScrollView
{
    CGPoint p = newScrollView.contentOffset;
    
    if(p.y >= 2500) {
        [(UIButton *)[self.SIAcceptView viewWithTag:700] setEnabled:YES];
        [(UIButton *)[self.SIAcceptView viewWithTag:700] setSelected:YES];
    }
    
}
- (void)scrollViewDidEndDecelerating:(UIScrollView *)newScrollView
{
    
    [self scrollViewDidEndScrollingAnimation:newScrollView];
    
}

- (void)setupISIAccept {
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    NSInteger i, i2, len = 0, len2 = 0;
    UIView *container = self.SIAcceptView;
    NSArray *subviews = [[container viewWithTag:1] subviews], *subviews2;
    UILabel *lbl;
    
    // Setup scroll view
    [self.safetyAcceptScroller setScrollEnabled:YES];
    [self.safetyAcceptScroller setContentSize:CGSizeMake(870, 3100)];
    // use this for working in the xib
//    [self.safetyAcceptScroller setContentSize:CGSizeMake(870, 7100)];
    
    self.safetyAcceptScroller.delegate = self;
    
    len = [subviews count];
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 1:
            {
                // Transverse through children and set font
                subviews2 = [[subviews objectAtIndex:i] subviews];
                len2 = [subviews2 count];
                
                for(i2=0; i2<len2; ++i2) {
                    if([[subviews2 objectAtIndex:i2] class] == [UILabel class]) {
                        lbl = (UILabel *)[subviews2 objectAtIndex:i2];
                        lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                    }
                    if([[subviews2 objectAtIndex:i2] tag] == 3) {
                        [self makeLabelIntoLink:(UILabel *)[subviews2 objectAtIndex:i2] parentView:[subviews objectAtIndex:i]];
                    }
                }
            }
                break;
            case 2:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                }
            }
                break;
            case 3:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    [self makeLabelIntoLink:lbl parentView:[container viewWithTag:1]];
                }
            }
                break;
            default:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                }
            }
                break;
        }
    }
}

- (void)setupISIInfoPopup {
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    NSInteger i, i2, len = 0, len2 = 0;
    UIView *container = self.ISIInfoView;
    NSArray *subviews = [[container viewWithTag:1] subviews], *subviews2;
    UILabel *lbl;
    
    // Setup scroll view
    [(UIScrollView *)[self.ISIInfoView viewWithTag:1] setScrollEnabled:YES];
    [(UIScrollView *)[self.ISIInfoView viewWithTag:1] setContentSize:CGSizeMake(870, 3100)];
    
    len = [subviews count];
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 1:
            {
                // Transverse through children and set font
                subviews2 = [[subviews objectAtIndex:i] subviews];
                len2 = [subviews2 count];
                
                for(i2=0; i2<len2; ++i2) {
                    if([[subviews2 objectAtIndex:i2] class] == [UILabel class]) {
                        lbl = (UILabel *)[subviews2 objectAtIndex:i2];
                        lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                    }
                    if([[subviews2 objectAtIndex:i2] tag] == 3) {
                        [self makeLabelIntoLink:(UILabel *)[subviews2 objectAtIndex:i2] parentView:[subviews objectAtIndex:i]];
                    }
                }
            }
                break;
            case 2:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
                }
            }
                break;
            case 3:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    [self makeLabelIntoLink:lbl parentView:[container viewWithTag:1]];
                }
            }
                break;
            default:
            {
                if([[subviews objectAtIndex:i] class] == [UILabel class]) {
                    lbl = (UILabel *)[subviews objectAtIndex:i];
                    lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
                }
            }
                break;
        }
    }
    
    //NSInteger viewTag = targetButton.tag;
    
    // References to views of pop-up
    UIView *popupPanel = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 1024, 768)];
    UIView *popupPanelContent = [[UIView alloc] initWithFrame:CGRectMake(10, 100, 1010, 685)];
    popupPanelContent.clipsToBounds = YES;
    
    popupPanel.tag = 5010;
    popupPanelContent.tag = 101;
    
    // Setup pop-up panel's modal aspect (background color, opacity, etc)
    [popupPanel setBackgroundColor:[UIColor colorWithRed:0.106 green:0.239 blue:0.345 alpha:0.6]];
    
    CGRect contentFrame = self.ISIInfoView.frame;
    contentFrame.origin.x = 0;
    contentFrame.origin.y = 0;
    self.ISIInfoView.frame = contentFrame;
    
    [popupPanelContent addSubview:self.ISIInfoView];
    [(UIScrollView *)[self.ISIInfoView viewWithTag:1] setContentOffset:CGPointZero animated:NO];
    
    // Add pop-up panel's content area to modal container
    [popupPanel addSubview:popupPanelContent];
    
    // Hide popup initially
    popupPanel.hidden = YES;
    
    // Finally show the pop-up
    [self.view addSubview:popupPanel];
}


- (void)attributedLabel:(TTTAttributedLabel *)label didSelectLinkWithURL:(NSURL *)url {
    [[UIApplication sharedApplication]openURL:url];
}

- (void)attributedLabel:(TTTAttributedLabel *)label didSelectLinkWithTextCheckingResult:(NSTextCheckingResult *)result {

}

- (void)makeLabelIntoLink:(UILabel *)lbl parentView:(UIView *)parent {
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    
    CGRect lblFrame = lbl.frame;
    UIColor *color = [UIColor colorWithRed:0.475 green:0.831 blue:1.0 alpha:1.0];
    
    TTTAttributedLabel *label = [[TTTAttributedLabel alloc] initWithFrame:lblFrame];
    label.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
    label.textColor = [UIColor darkGrayColor];
    label.lineBreakMode = UILineBreakModeWordWrap;
    label.numberOfLines = 0;
    
    NSArray *keys = [[NSArray alloc] initWithObjects:(id)kCTForegroundColorAttributeName,(id)kCTUnderlineStyleAttributeName, nil];
    NSArray *objects = [[NSArray alloc] initWithObjects:color,[NSNumber numberWithInt:kCTUnderlineStyleSingle], nil];
    NSDictionary *linkAttributes = [[NSDictionary alloc] initWithObjects:objects forKeys:keys];
    
    label.linkAttributes = linkAttributes;
    
    
    NSString *text = lbl.text;
    [label setText:text afterInheritingLabelAttributesAndConfiguringWithBlock:^ NSMutableAttributedString *(NSMutableAttributedString *mutableAttributedString) {
        NSRange boldRange = [[mutableAttributedString string] rangeOfString:@"manage" options:NSCaseInsensitiveSearch];
        //NSRange strikeRange = [[mutableAttributedString string] rangeOfString:@"sit amet" options:NSCaseInsensitiveSearch];
        
        // Core Text APIs use C functions without a direct bridge to UIFont. See Apple's "Core Text Programming Guide" to learn how to configure string attributes.
        UIFont *boldSystemFont = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
        CTFontRef font = CTFontCreateWithName((__bridge CFStringRef)boldSystemFont.fontName, boldSystemFont.pointSize, NULL);
        if (font) {
            [mutableAttributedString addAttribute:(NSString *)kCTFontAttributeName value:(__bridge id)font range:boldRange];
            //[mutableAttributedString addAttribute:kTTTStrikeOutAttributeName value:[NSNumber numberWithBool:YES] range:strikeRange];
            CFRelease(font);
        }
        
        return mutableAttributedString;
    }];
    
    label.enabledTextCheckingTypes = NSTextCheckingTypeLink; // Automatically detect links when the label text is subsequently changed
    label.delegate = self; // Delegate methods are called when the user taps on a link (see `TTTAttributedLabelDelegate` protocol)
    
    NSRange range = [label.text rangeOfString:@"www.celgeneriskmanagement.com"];
    [label addLinkToURL:[NSURL URLWithString:@"http://www.celgeneriskmanagement.com"] withRange:range];
    
    [lbl removeFromSuperview];
    [parent addSubview:label];
}


- (void)setupRevlimidInfoPage1 {

    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    
    NSArray *subviews = [self.revlimidInfoPage1 subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 10:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
            }
                break;
            case 20:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
            }
                break;
            case 30:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
            }
                break;
            case 100:
            {
                UIView *bodyView = self.revlimidInfoPage1;
                UILabel *linkLabel = (UILabel *)[subviews objectAtIndex:i];
                [self makeLabelIntoLink:linkLabel parentView:bodyView];
            }
                break;
        }
    }

}

- (void)setupRevlimidInfoPage2 {
    
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *hvcn = @"HelveticaNeueLTStd-HvCn";
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    
    NSArray *subviews = [self.revlimidInfoPage2 subviews];
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    len = [subviews count];
    
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 10:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
            }
                break;
            case 20:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:hvcn size:lbl.font.pointSize];
            }
                break;
            case 30:
            {
                lbl = (UILabel *)[subviews objectAtIndex:i];
                lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
            }
                break;
            case 100:
            {
                UIView *bodyView = self.revlimidInfoPage2;
                UILabel *linkLabel = (UILabel *)[subviews objectAtIndex:i];
                [self makeLabelIntoLink:linkLabel parentView:bodyView];
            }
                break;
        }
    }
    
}



- (void)setupRevlimidInfoPage3 {
    UIView *container = self.revlimidInfoPage3;
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    UIButton *btn;
    UIView *view;
    NSInteger i = 0, len = 0;
    
    // Tabbed content new grouping mech...
    NSArray *subviews = [container subviews];
    NSArray *subviews2;
    NSInteger len2 = 0;
    NSInteger i2 = 0;
    len = [subviews count];
    
//    for(i=70; i<=75; ++i) {
//        lbl = (UILabel *)[[[container viewWithTag:i] subviews] objectAtIndex:0];
//        lbl.font = [UIFont fontWithName:cn size:17];
//    }
    for(i=0; i<len; ++i) {
        switch([[subviews objectAtIndex:i] tag]) {
            case 50:
            case 51:
            {
                // Transverse through children and set font
                subviews2 = [[subviews objectAtIndex:i] subviews];
                len2 = [subviews2 count];
                
                for(i2=0; i2<len2; ++i2) {
                    view = (UIView *)[subviews2 objectAtIndex:i2];
                    if(view.tag == 2) {// the pill button text items
                        btn = (UIButton *)[[view subviews] objectAtIndex:1];
                        btn.titleLabel.font = [UIFont fontWithName:cn size:18];
                    } else if(view.tag > 69 && view.tag < 76) { // the content within each accordion
                        UILabel *lbl;
                        lbl = (UILabel *)[[view subviews] objectAtIndex:0];
                        lbl.font = [UIFont fontWithName:cn size:17];
                    }
                }
            }
                break;
            case 3:
            case 4:
            {
                btn = (UIButton *)[subviews objectAtIndex:i];
                btn.titleLabel.font = [UIFont fontWithName:cn size:21];
            }
                break;
            case 2000:
            {
                
                // Transverse through children and set font
                UIView *bodyView = [[[subviews objectAtIndex:i] subviews] objectAtIndex:1];
                UILabel *linkLabel = (UILabel *)[bodyView viewWithTag:5555];
                [self makeLabelIntoLink:linkLabel parentView:bodyView];
            }
                break;
        }
    }
    
    //HelveticaNeueLTStd-Cn
    
}

- (void)setupAdverseEventsPage1 {
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    
    lbl = (UILabel *)[self.page12 viewWithTag:10];
    lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
    
    lbl = (UILabel *)[self.page12 viewWithTag:11];
    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
    
    lbl = (UILabel *)[self.page12 viewWithTag:12];
    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
    
    // Setup scroll view
    [(UIScrollView *)[self.page12 viewWithTag:111] setScrollEnabled:YES];
    [(UIScrollView *)[self.page12 viewWithTag:111] setBounces:NO];
    [(UIScrollView *)[self.page12 viewWithTag:111] setContentSize:CGSizeMake(694, 2106)];
    [(UIScrollView *)[self.page12 viewWithTag:111] setContentOffset:CGPointZero animated:NO];
    
    // Panel Header
    NSArray *subviews = [[[self.page12 viewWithTag:25] viewWithTag:35] subviews];
    len = [subviews count];
    for(i=0; i<len; ++i) {
        if([[subviews objectAtIndex:i] class] == [UILabel class]) {
            lbl = (UILabel *)[subviews objectAtIndex:i];
            lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
        }
    }
    
    // Panel Body
    subviews = [[[[self.page12 viewWithTag:25] viewWithTag:111] viewWithTag:45] subviews];
    len = [subviews count];
    for(i=0; i<len; ++i) {
        if([[[subviews objectAtIndex:i] subviews] count] > 0) {
            for(UIView *panelRow in [[subviews objectAtIndex:i] subviews]) {
                if([panelRow class] == [UILabel class]) {
                    lbl = (UILabel *)panelRow;
                    lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
                }
            }
        }
    }
    
}

- (void)setupAdverseEventsPage2 {
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    
    lbl = (UILabel *)[self.page13 viewWithTag:10];
    lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
    
    lbl = (UILabel *)[self.page13 viewWithTag:11];
    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
    
    lbl = (UILabel *)[self.page13 viewWithTag:12];
    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
    
    // Setup scroll view
    [(UIScrollView *)[self.page13 viewWithTag:111] setScrollEnabled:YES];
    [(UIScrollView *)[self.page13 viewWithTag:111] setBounces:NO];
    [(UIScrollView *)[self.page13 viewWithTag:111] setContentSize:CGSizeMake(694, 1367)];
    [(UIScrollView *)[self.page13 viewWithTag:111] setContentOffset:CGPointZero animated:NO];
    
    // Panel Header
    NSArray *subviews = [[[self.page13 viewWithTag:25] viewWithTag:35] subviews];
    len = [subviews count];
    for(i=0; i<len; ++i) {
        if([[subviews objectAtIndex:i] class] == [UILabel class]) {
            lbl = (UILabel *)[subviews objectAtIndex:i];
            lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
        }
    }
    
    // Panel Body
    subviews = [[[[self.page13 viewWithTag:25] viewWithTag:111] viewWithTag:45] subviews];
    len = [subviews count];
    for(i=0; i<len; ++i) {
        if([[[subviews objectAtIndex:i] subviews] count] > 0) {
            for(UIView *panelRow in [[subviews objectAtIndex:i] subviews]) {
                if([panelRow class] == [UILabel class]) {
                    lbl = (UILabel *)panelRow;
                    lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
                }
            }
        }
    }
    
}

- (void)setupAdverseEventsPage3 {
    NSString *bdcn = @"HelveticaNeueLTStd-BdCn";
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    
    NSInteger len = 0;
    NSInteger i = 0;
    UILabel *lbl;
    
    lbl = (UILabel *)[self.page14 viewWithTag:10];
    lbl.font = [UIFont fontWithName:bdcn size:lbl.font.pointSize];
    
    lbl = (UILabel *)[self.page14 viewWithTag:11];
    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
    
    lbl = (UILabel *)[self.page14 viewWithTag:12];
    lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
    
    // Setup scroll view
    [(UIScrollView *)[self.page14 viewWithTag:111] setScrollEnabled:YES];
    [(UIScrollView *)[self.page14 viewWithTag:111] setBounces:NO];
    [(UIScrollView *)[self.page14 viewWithTag:111] setContentSize:CGSizeMake(694, 598)];
    [(UIScrollView *)[self.page14 viewWithTag:111] setContentOffset:CGPointZero animated:NO];
    
    // Panel Header
    NSArray *subviews = [[[self.page14 viewWithTag:25] viewWithTag:35] subviews];
    len = [subviews count];
    for(i=0; i<len; ++i) {
        if([[subviews objectAtIndex:i] class] == [UILabel class]) {
            lbl = (UILabel *)[subviews objectAtIndex:i];
            lbl.font = [UIFont fontWithName:cn size:lbl.font.pointSize];
        }
    }
    
    // Panel Body
    subviews = [[[[self.page14 viewWithTag:25] viewWithTag:111] viewWithTag:45] subviews];
    len = [subviews count];
    for(i=0; i<len; ++i) {
        if([[[subviews objectAtIndex:i] subviews] count] > 0) {
            for(UIView *panelRow in [[subviews objectAtIndex:i] subviews]) {
                if([panelRow class] == [UILabel class]) {
                    lbl = (UILabel *)panelRow;
                    lbl.font = [UIFont fontWithName:mcdn size:lbl.font.pointSize];
                }
            }
        }
    }
    
}

- (IBAction)revInfoswitchContent:(id)sender {
    UIButton *targetButton = (UIButton *)sender;
    UIView *container = self.revlimidInfoPage3;
    
    [targetButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    
    
    switch(targetButton.tag) {
        case 3:
        {
            [[container viewWithTag:51] setHidden:YES];
            [[container viewWithTag:50] setHidden:NO];
            [(UIButton *)[container viewWithTag:4] setTitleColor:[UIColor colorWithRed:0.549 green:0.859 blue:1.0 alpha:1.0] forState:UIControlStateNormal];
        }
            break;
        case 4:
        {
            [[container viewWithTag:50] setHidden:YES];
            [[container viewWithTag:51] setHidden:NO];
            [(UIButton *)[container viewWithTag:3] setTitleColor:[UIColor colorWithRed:0.549 green:0.859 blue:1.0 alpha:1.0] forState:UIControlStateNormal];
        }
            break;
    }
}

- (IBAction)revInfoExpandContent:(id)sender {
    UIButton *targetButton = (UIButton *)sender;
    UIView *container = targetButton.superview.superview;
    UIView *content1, *content2, *content3;
    UIView *content4, *content5, *content6;
    UIImageView *btn1Imageview, *btn2Imageview, *btn3Imageview;
    UIImageView *btn4Imageview, *btn5Imageview, *btn6Imageview;
    NSString *inactiveBlueButtonImage = @"R1A_blue_row.png";
    NSString *inactiveGreenButtonImage = @"R1A_green_row.png";
    NSString *activeButtonImage = @"R1A_yellow_row.png";
    CGRect endFrame, resizeFrame;
    NSInteger i;
    
    if(container.tag == 50) {
        content1 = [container viewWithTag:70];
        content2 = [container viewWithTag:71];
        content3 = [container viewWithTag:72];
        content4 = [container viewWithTag:73];
        content5 = [container viewWithTag:74];
        content6 = [container viewWithTag:75];
        
        btn1Imageview = (UIImageView *)[container viewWithTag:40];
        btn2Imageview = (UIImageView *)[container viewWithTag:41];
        btn3Imageview = (UIImageView *)[container viewWithTag:42];
        btn4Imageview = (UIImageView *)[container viewWithTag:43];
        btn5Imageview = (UIImageView *)[container viewWithTag:44];
        btn6Imageview = (UIImageView *)[container viewWithTag:45];
        
        [btn1Imageview setImage:[UIImage imageNamed:inactiveGreenButtonImage]];
        [btn3Imageview setImage:[UIImage imageNamed:inactiveGreenButtonImage]];
        
    } else {
        content2 = [container viewWithTag:71];
        content4 = [container viewWithTag:73];
        content5 = [container viewWithTag:74];
        content6 = [container viewWithTag:75];
        
        btn2Imageview = (UIImageView *)[container viewWithTag:41];
        btn4Imageview = (UIImageView *)[container viewWithTag:43];
        btn5Imageview = (UIImageView *)[container viewWithTag:44];
        btn6Imageview = (UIImageView *)[container viewWithTag:45];
    }
    
    [btn2Imageview setImage:[UIImage imageNamed:inactiveBlueButtonImage]];
    [btn4Imageview setImage:[UIImage imageNamed:inactiveBlueButtonImage]];
    [btn5Imageview setImage:[UIImage imageNamed:inactiveBlueButtonImage]];
    [btn6Imageview setImage:[UIImage imageNamed:inactiveBlueButtonImage]];
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:0.5];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
    [UIView setAnimationDelegate:self];
    
    // Collapse others
    for(i=70; i<=75; ++i) {
        if(targetButton.tag != i-60) {
            resizeFrame = [[container viewWithTag:i] frame];
            resizeFrame.size.height = 0;
            [[container viewWithTag:i] setFrame:resizeFrame];
        }
    }
    
    for(i=10; i<=15; ++i) {
        [(UIButton *)[container viewWithTag:i] setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    }
    [targetButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    
    
    
    switch(targetButton.tag) {
        case 10:
        {
            endFrame = content1.frame;
            if(endFrame.size.height > 0) {
                endFrame.size.height = 0;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:inactiveGreenButtonImage]];
                [targetButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
            } else {
                endFrame.size.height = 81;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:activeButtonImage]];
            }
            content1.frame = endFrame;
        }
            break;
        case 11:
        {
            endFrame = content2.frame;
            if(endFrame.size.height > 0) {
                endFrame.size.height = 0;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:inactiveBlueButtonImage]];
                [targetButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
            } else {
                endFrame.size.height = 81;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:activeButtonImage]];
            }
            content2.frame = endFrame;
        }
            break;
        case 12:
        {
            endFrame = content3.frame;
            if(endFrame.size.height > 0) {
                endFrame.size.height = 0;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:inactiveGreenButtonImage]];
                [targetButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
            } else {
                endFrame.size.height = 81;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:activeButtonImage]];
            }
            content3.frame = endFrame;
        }
            break;
        case 13:
        {
            endFrame = content4.frame;
            if(endFrame.size.height > 0) {
                endFrame.size.height = 0;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:inactiveBlueButtonImage]];
                [targetButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
            } else {
                endFrame.size.height = 122;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:activeButtonImage]];
            }
            content4.frame = endFrame;
        }
            break;
        case 14:
        {
            endFrame = content5.frame;
            if(endFrame.size.height > 0) {
                endFrame.size.height = 0;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:inactiveBlueButtonImage]];
                [targetButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
            } else {
                endFrame.size.height = 87;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:activeButtonImage]];
            }
            content5.frame = endFrame;
        }
            break;
        case 15:
        {
            endFrame = content6.frame;
            if(endFrame.size.height > 0) {
                endFrame.size.height = 0;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:inactiveBlueButtonImage]];
                [targetButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
            } else {
                endFrame.size.height = 87;
                
                // Set button to inactive state
                [(UIImageView *)[container viewWithTag:(targetButton.tag + 30)] setImage:[UIImage imageNamed:activeButtonImage]];
            }
            content6.frame = endFrame;
        }
            break;
    }
    [UIView commitAnimations];
    
}


- (IBAction)switchCelgeneSupportTab:(id)sender {
    UIButton *targetButton = (UIButton *)sender;
    
    UIButton *btn1 = (UIButton *)[self.celegeneSupportPage viewWithTag:20];
    UIButton *btn2 = (UIButton *)[self.celegeneSupportPage viewWithTag:21];
    UIButton *btn3 = (UIButton *)[self.celegeneSupportPage viewWithTag:22];
    UIButton *btn4 = (UIButton *)[self.celegeneSupportPage viewWithTag:23];
    
    UIView *tab1 = [self.celegeneSupportPage viewWithTag:66];
    UIView *tab2 = [self.celegeneSupportPage viewWithTag:67];
    UIView *tab3 = [self.celegeneSupportPage viewWithTag:68];
    UIView *tab4 = [self.celegeneSupportPage viewWithTag:69];
    
    // Set all the tab content to hidden and buttons to inactive
    [btn1 setSelected:NO];
    [btn2 setSelected:NO];
    [btn3 setSelected:NO];
    [btn4 setSelected:NO];
    [tab1 setHidden:YES];
    [tab2 setHidden:YES];
    [tab3 setHidden:YES];
    [tab4 setHidden:YES];
    
    // Set target button's state to active
    [targetButton setSelected:TRUE];
    
    switch(targetButton.tag) {
        case 20:
        {
            [tab1 setHidden:NO];
        }
            break;
        case 21:
        {
            [tab2 setHidden:NO];
        }
            break;
        case 22:
        {
            [tab3 setHidden:NO];
        }
            break;
        case 23:
        {
            [tab4 setHidden:NO];
        }
            break;
    }
}

#pragma mark -
#pragma mark View lifecycle


- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Hide Active view and load up the launch page
    [self.activePageView setHidden:YES];
    
    // Hide efficacy pop-up if it is active
    UIView *effPopupPanel = [self.view viewWithTag:100];
    
    if(effPopupPanel) {
        [effPopupPanel removeFromSuperview];
    }
    
    
    // Hide accept notice pop-up
    UIView *popupPanel = [self.view viewWithTag:5000];
    [popupPanel removeFromSuperview];
    
    [self.launchPage setHidden:NO];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
//    for (NSString* family in [UIFont familyNames])
//    {
//        NSLog(@"%@", family);
//        
//        for (NSString* name in [UIFont fontNamesForFamilyName: family])
//        {
//            NSLog(@"  %@", name);
//        }
//    }
    [[NSNotificationCenter defaultCenter] addObserver: self
                                             selector: @selector(applicationDidEnterBackground:)
                                                 name: UIApplicationDidEnterBackgroundNotification
                                               object: nil];
    
    NSString *mcdn = @"HelveticaNeueLTStd-MdCn";
    //NSString *cn = @"HelveticaNeueLTStd-Cn";
    
    // Setup font for main view
    [(UILabel *)[[self.view viewWithTag:1001] viewWithTag:1010] setFont:[UIFont fontWithName:mcdn size:15]];
    [(UILabel *)[[self.view viewWithTag:1001] viewWithTag:1011] setFont:[UIFont fontWithName:mcdn size:15]];
    
    // Set core properties
    self.activePageView = [self.view viewWithTag:800];
    self.activePageViewIndex = 0;
    
    //configure swipeView
    _swipeView.pagingEnabled = YES;
    _swipeView.bounces = NO;
    _swipeView.defersItemViewLoading = YES;
    
    _swipeView2.pagingEnabled = YES;
    _swipeView2.bounces = NO;
    _swipeView2.defersItemViewLoading = YES;
    
    // Setup tap to expand array list
    self.tapToExpandViews = [[NSArray alloc] initWithObjects:
                             (UIView *)self.page3,
                             self.page4,
                             self.page6,
                             self.page12,
                             self.page13,
                             self.page14,
                             self.revlimidInfoPage3,
                             nil];
    
    // Pages that need additional setup
    [self setupViewOverview2];
    [self setupStudyHighlight2];
    [self setupStudyHighlight3];
    [self setupStudyHighlight4];
    [self setupStudyHighlight5];
    [self setupEfficacy1];
    [self setupEfficacy2];
    [self setupEfficacy3];
    [self setupEfficacy4];
    [self setupDosing1];
    [self setupDosing2];
    [self setupDosing3];
    [self setupDosing4];
    [self setupDosing5];
    [self setupRevlimidPIPDF];
    [self setupRevlimidOtherViews];
    
    [self setupCelgeneSupport];
    [self setupRevlimidInfoPage1];
    [self setupRevlimidInfoPage2];
    [self setupRevlimidInfoPage3];
    [self setupImportantSafetyInfoPage];
    
    [self setupAdverseEventsPage1];
    [self setupAdverseEventsPage2];
    [self setupAdverseEventsPage3];
    
    [self setupTapToExpand];
    [self setupISIAccept];
    [self setupISIInfoPopup];
    [self setupPIPopupView];
    [self setupLaunchPage];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if (interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight) {
        return YES;
    }
        return NO;
    
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

#pragma mark -
#pragma mark iCarousel methods

- (NSInteger)numberOfItemsInSwipeView:(SwipeView *)swipeView
{
    //return the total number of items in the carousel
    // @TODO: deally, this should be  decoupled from current object
    if(swipeView.tag == 801) { // Revlimid 3-view slides
        return 3;
    }
    return [_items count];
}

- (void)studyHighlight3Anim1Finished {
    if(!self.enableAnimation) {
        return;
    }
    // Grab the Animation Block
    UIView *animationBlock = [self.page5 viewWithTag:2];
    if(!animationBlock) {
        return;
    }
    
    // Create animation (97)
    //UIView *image2View = [[UIView alloc] initWithFrame:CGRectMake(1024, 2, 779, 245)];
    //image2View.clipsToBounds = YES;
    //UIImageView *image2 = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 779, 245)];
    //image2.image = [UIImage imageNamed:@"M1.2B_cycle14_all_2.png"];
    //[image2View addSubview:image2];
    //[image2View setAlpha:0.4];
    //image2.tag = -1;
    //image2View.tag = 4;
    
    UIView *image2 = [animationBlock viewWithTag:4];
    //[image2 setAlpha:0.4];
    
    //[animationBlock addSubview:image2View];
    
    CGRect endFrame;
    
    endFrame = image2.frame;
    endFrame.origin.x = 1024;
    image2.frame = endFrame;
    endFrame.origin.x = 824;
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:0.8];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
    [UIView setAnimationDidStopSelector:@selector(studyHighlight3Anim2Finished)];
    [UIView setAnimationDelegate:self];
    
    image2.frame = endFrame;
    
    [image2 setHidden:NO];
    [UIView commitAnimations];

}

- (void)studyHighlight3Anim2Finished {
    if(!self.enableAnimation) {
        return;
    }
    // Grab the Animation Block
    UIView *animationBlock = [self.page5 viewWithTag:2];
    if(!animationBlock) {
        return;
    }
    
    // Create animation #3
    // First adjust the width of image #2
    //UIView *image2view = [self.page5 viewWithTag:4];
    //UIImageView *image3 = [[UIImageView alloc] initWithFrame:CGRectMake(1024, 40, 98, 207)];
    //image3.image = [UIImage imageNamed:@"M1.2B_continue_until_disease_progression2.png"];
    //image3.tag = 5;
    
    //[animationBlock addSubview:image3];
    
    UIImageView *image3 = (UIImageView *)[animationBlock viewWithTag:5];
    UIView *image2 = [animationBlock viewWithTag:4];
    
    CGRect endFrame, image2viewFrame;
    
    image2viewFrame = image2.frame;
    image2viewFrame.size.width = 92;
    image2.frame = image2viewFrame;
    
    endFrame = image3.frame;
    endFrame.origin.x = 1024;
    image3.frame = endFrame;
    endFrame.origin.x = 918;
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:0.8];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
    [UIView setAnimationDidStopSelector:@selector(studyHighlight3Anim3Finished)];
    [UIView setAnimationDelegate:self];
    
    [image2 setAlpha:0.4];
    
    image3.frame = endFrame;
    [image3 setHidden:NO];
    
    [UIView commitAnimations];
    
}

- (void)studyHighlight3AnimBlockToggleFinished {
    // Grab the Animation Block
    UIView *animationBlock = [self.page5 viewWithTag:2];
    if(!animationBlock) {
        return;
    }
    
    // Toggle animation
    UIView *image1 = [[animationBlock viewWithTag:2] viewWithTag:6];
    UIView *image2view = [animationBlock viewWithTag:4];
    UIImageView *image2 = (UIImageView *)[image2view viewWithTag:-1];
    
    if(image2view.frame.origin.x >= 138) {
        [image1 setAlpha:1.0];
        [image2view setAlpha:0.4];
        image2.image = [UIImage imageNamed:@"M1.2B_cycle14_all_2.png"];
    } else {
        [image1 setAlpha:0.4];
        [image2 setAlpha:1.0];
        image2.image = [UIImage imageNamed:@"M1.2B_cycle14_all_2b.png"];
    }
}

- (void)handleStudyHighlight3AnimTap:(UITapGestureRecognizer *)recognizer {
    //CGPoint location = [recognizer locationInView:[recognizer.view superview]];
    
    
    // Grab the Animation Block
    UIView *animationBlock = [self.page5 viewWithTag:2];
    if(!animationBlock) {
        return;
    }
    
    // Toggle animation
    UIView *image1 = [[animationBlock viewWithTag:3] viewWithTag:6];
    UIView *image2view = [animationBlock viewWithTag:4];
    //UIImageView *image2 = (UIImageView *)[image2view viewWithTag:-1];
    
    if(image2view.alpha < 1.0) {
        [image1 setAlpha:0.4];
        [image2view setAlpha:1.0];
    }
    
    
    CGRect endFrame;
    
    endFrame = image2view.frame;
    if(endFrame.origin.x >= 823) {
        endFrame.origin.x = 136;
        endFrame.size.width = 779;
    } else {
        endFrame.origin.x = 823;
        endFrame.size.width = 92;
    }
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:1.1];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
    [UIView setAnimationDidStopSelector:@selector(studyHighlight3AnimBlockToggleFinished)];
    [UIView setAnimationDelegate:self];
    
    image2view.frame = endFrame;
    [UIView commitAnimations];
    
}

- (void)studyHighlight3Anim3Finished {
    if(!self.enableAnimation) {
        return;
    }
    // Grab the Animation Block
    UIView *animationBlock = [self.page5 viewWithTag:2];
    if(!animationBlock) {
        return;
    }
    
    [animationBlock addGestureRecognizer:self.studyHighlight3AnimBlockTap];
    [[self.page5 viewWithTag:10] setHidden:NO];
    
}

- (IBAction)dosing1ChartTapped:(id)sender {
    UIButton *targetButton = (UIButton *)sender;
    if(!targetButton) {
        return;
    }
    
    UIView *container = self.page15;
    UIView *chartHilight1 = [container viewWithTag:16];
    UIView *chartHilight2 = [container viewWithTag:17];
    
    switch (targetButton.tag) {
        case 10:
            [chartHilight2 setHidden:YES];
            [chartHilight1 setHidden:NO];
            [targetButton setBackgroundColor:[UIColor clearColor]];
            [[container viewWithTag:11] setBackgroundColor:[UIColor colorWithRed:0.302 green:0.392 blue:0.478 alpha:0.5]];
            break;
        case 11:
            [chartHilight1 setHidden:YES];
            [chartHilight2 setHidden:NO];
            [targetButton setBackgroundColor:[UIColor clearColor]];
            [[container viewWithTag:10] setBackgroundColor:[UIColor colorWithRed:0.302 green:0.392 blue:0.478 alpha:0.5]];
            break;
    }
}

- (void)playDosing1ChartAnimation {
    UIView *container = self.page15;
    
    UIView *dosingChartTop1 = [container viewWithTag:12];
    UIView *dosingChartTop2 = [container viewWithTag:13];
    UIView *dosingChartBottom1 = [container viewWithTag:14];
    UIView *dosingChartBottom2 = [container viewWithTag:15];
    
    CGRect chart1Frame, chart2Frame, chart3Frame, chart4Frame;
    
    // Initiatize frames
    chart1Frame = dosingChartTop1.frame;
    chart2Frame = dosingChartTop2.frame;
    chart3Frame = dosingChartBottom1.frame;
    chart4Frame = dosingChartBottom2.frame;
    
    // Set all the chart's containing view's width to zero
    chart1Frame.size.width = 0;
    chart2Frame.size.width = 0;
    chart3Frame.size.width = 0;
    chart4Frame.size.width = 0;
    dosingChartTop1.frame = chart1Frame;
    dosingChartTop2.frame = chart2Frame;
    dosingChartBottom1.frame = chart3Frame;
    dosingChartBottom2.frame = chart4Frame;
    
    // Set final animation frame
    chart1Frame.size.width = 818;
    chart2Frame.size.width = 818;
    chart3Frame.size.width = 818;
    chart4Frame.size.width = 818;
    
    // Set charts to visible
    [dosingChartTop1 setHidden:NO];
    [dosingChartTop2 setHidden:NO];
    [dosingChartBottom1 setHidden:NO];
    [dosingChartBottom2 setHidden:NO];
    
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:3.5];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
    [UIView setAnimationDelegate:self];
    
    dosingChartTop1.frame = chart1Frame;
    dosingChartTop2.frame = chart2Frame;
    dosingChartBottom1.frame = chart3Frame;
    dosingChartBottom2.frame = chart4Frame;
    [UIView commitAnimations];
    
}

- (void)playDosing2ChartAnimation {
    UIView *container = self.page16;
    
    // Study plot
    UIView *barChart1 = [container viewWithTag:3];
    UIView *barChart2 = [container viewWithTag:5];
    
    [barChart1 setHidden:YES];
    [barChart2 setHidden:YES];
    
    
    CGRect barChart1Frame, barChart1Frame2;
    
    // Bar chart 1
    barChart1Frame = barChart1.frame;
    barChart1Frame.origin.y = 297;
    barChart1Frame.size.height = 0;
    barChart1.frame = barChart1Frame;
    barChart1Frame.size.height = -205;
    
    // Bar chart 2
    barChart1Frame2 = barChart2.frame;
    barChart1Frame2.origin.y = 297;
    barChart1Frame2.size.height = 0;
    barChart2.frame = barChart1Frame2;
    barChart1Frame2.size.height = -163;
    
    // Unhide plots for this group
    [barChart1 setHidden:NO];
    [barChart2 setHidden:NO];
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:1.5];
    [UIView setAnimationCurve:UIViewAnimationCurveLinear];
    [UIView setAnimationDelegate:self];
    
    barChart1.frame = barChart1Frame;
    barChart2.frame = barChart1Frame2;
    [UIView commitAnimations];
    
}

- (void)playDosing3ChartAnimation {
    UIView *container = self.page17;
    
    UIView *dosingChart1 = [container viewWithTag:10];
    UIView *dosingChart2 = [container viewWithTag:11];
    UIView *dosingChart3 = [container viewWithTag:12];
    
    CGRect chartFrame1, chartFrame2, chartFrame3;
    
    // Initiatize frames
    chartFrame1 = dosingChart1.frame;
    chartFrame2 = dosingChart2.frame;
    chartFrame3 = dosingChart3.frame;
    
    // Set all the chart's containing view's width to zero
    chartFrame1.size.width = 0;
    chartFrame2.size.width = 0;
    chartFrame3.size.width = 0;
    dosingChart1.frame = chartFrame1;
    dosingChart2.frame = chartFrame2;
    dosingChart3.frame = chartFrame3;
    
    // Set final animation frame
    chartFrame1.size.width = 660;
    chartFrame2.size.width = 660;
    chartFrame3.size.width = 660;
    
    // Set charts to visible
    [dosingChart1 setHidden:NO];
    [dosingChart2 setHidden:NO];
    [dosingChart3 setHidden:NO];
    
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:3.5];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
    [UIView setAnimationDelegate:self];
    
    dosingChart1.frame = chartFrame1;
    dosingChart2.frame = chartFrame2;
    dosingChart3.frame = chartFrame3;
    [UIView commitAnimations];
    
}

- (IBAction)dosing3ChartTapped:(id)sender {
    UIButton *targetButton = (UIButton *)sender;
    if(!targetButton) {
        return;
    }
    
    UIView *container = self.page17;
    UIView *chartHilight1 = [container viewWithTag:20];
    UIView *chartHilight2 = [container viewWithTag:21];
    UIView *chartHilight3 = [container viewWithTag:22];
    
    
    switch (targetButton.tag) {
        case 5:
            [chartHilight2 setHidden:YES];
            [chartHilight3 setHidden:YES];
            [chartHilight1 setHidden:NO];
            [targetButton setBackgroundColor:[UIColor clearColor]];
            [[container viewWithTag:6] setBackgroundColor:[UIColor colorWithRed:0.302 green:0.392 blue:0.478 alpha:0.5]];
            [[container viewWithTag:7] setBackgroundColor:[UIColor colorWithRed:0.302 green:0.392 blue:0.478 alpha:0.5]];
            break;
        case 6:
            [chartHilight1 setHidden:YES];
            [chartHilight3 setHidden:YES];
            [chartHilight2 setHidden:NO];
            [targetButton setBackgroundColor:[UIColor clearColor]];
            [[container viewWithTag:5] setBackgroundColor:[UIColor colorWithRed:0.302 green:0.392 blue:0.478 alpha:0.5]];
            [[container viewWithTag:7] setBackgroundColor:[UIColor colorWithRed:0.302 green:0.392 blue:0.478 alpha:0.5]];
            break;
        case 7:
            [chartHilight1 setHidden:YES];
            [chartHilight2 setHidden:YES];
            [chartHilight3 setHidden:NO];
            [targetButton setBackgroundColor:[UIColor clearColor]];
            [[container viewWithTag:5] setBackgroundColor:[UIColor colorWithRed:0.302 green:0.392 blue:0.478 alpha:0.5]];
            [[container viewWithTag:6] setBackgroundColor:[UIColor colorWithRed:0.302 green:0.392 blue:0.478 alpha:0.5]];
            break;
    }
}

- (void)swipeViewCurrentItemIndexDidChange:(SwipeView *)swipeView {
    if(swipeView.tag != 800) {
        if([self.swipeView2 getLastIndex]  == 2) {
            // Collapse all accordions
            UIView *contentView;
            CGRect endFrame;
            NSInteger i;
            NSString *inactiveBlueButtonImage = @"R1A_blue_row.png";
            NSString *inactiveGreenButtonImage = @"R1A_green_row.png";
            
            for(i=70; i<=75; ++i) {
                // Reset button colors
                [(UIButton *)[[self.revlimidInfoPage3 viewWithTag:50] viewWithTag:i-60] setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
                
                // Reset button to inactive state
                if(i == 70 || i == 73) {
                    [(UIImageView *)[[self.revlimidInfoPage3 viewWithTag:50] viewWithTag:i-30] setImage:[UIImage imageNamed:inactiveGreenButtonImage]];
                } else {
                    [(UIImageView *)[[self.revlimidInfoPage3 viewWithTag:50] viewWithTag:i-30] setImage:[UIImage imageNamed:inactiveBlueButtonImage]];
                }
                
                contentView = [[self.revlimidInfoPage3 viewWithTag:50] viewWithTag:i];
                endFrame = contentView.frame;
                endFrame.size.height = 0;
                contentView.frame = endFrame;
                
                if(i != 70 && i != 72) {
                    // Reset button colors
                    [(UIButton *)[[self.revlimidInfoPage3 viewWithTag:51] viewWithTag:i-60] setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
                    
                    // Reset button to inactive state
                    [(UIImageView *)[[self.revlimidInfoPage3 viewWithTag:51] viewWithTag:i-30] setImage:[UIImage imageNamed:inactiveBlueButtonImage]];
                    
                    contentView = [[self.revlimidInfoPage3 viewWithTag:51] viewWithTag:i];
                    endFrame = contentView.frame;
                    endFrame.size.height = 0;
                    contentView.frame = endFrame;
                }
            }
            
            // Return tab back to Female of Reproductive Potential (tab #1)
            [[self.revlimidInfoPage3 viewWithTag:51] setHidden:YES];
            [[self.revlimidInfoPage3 viewWithTag:50] setHidden:NO];
            [(UIButton *)[self.revlimidInfoPage3 viewWithTag:3] setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
            [(UIButton *)[self.revlimidInfoPage3 viewWithTag:4] setTitleColor:[UIColor colorWithRed:0.549 green:0.859 blue:1.0 alpha:1.0] forState:UIControlStateNormal];
        }
        
        return;
    }
    
    // References need to UI elements that may need to be changed
    UIButton *homeBtn = (UIButton *)[self.view viewWithTag:500];
    
    // Reset previous view's chart state if necessary
    switch([self.swipeView getLastIndex]) {
            case 0:
        {
            [homeBtn setImage:[UIImage imageNamed:@"Nav_Home_btn_inactive.png"] forState:UIControlStateNormal];
        }
            break;
        case 3:
        {
            [self.accordion setSelectedIndex:-1];
        }
            break;
        case 4:
        {
            // Fix tap to expand
            UIView *tappedView;
            UIImageView *topImageView = nil;
            CGRect endFrame;
            
            tappedView = [self.page5 viewWithTag:2000];
            // Get the top image views
            for(UIView *view in tappedView.subviews) {
                if(view.tag == 1) {
                    topImageView = (UIImageView *)view;
                    break;
                }
            }
            endFrame = tappedView.frame;
            CGFloat hDiff = tappedView.frame.size.height -
            (tappedView.superview.frame.size.height - tappedView.frame.origin.y);
            if(hDiff > 0.0) {
                endFrame.origin.y -= hDiff;
                if(topImageView) {
                    [topImageView setImage:[UIImage imageNamed:@"Tap_Expand_Expanded.png"]];
                }
            }
            tappedView.frame = endFrame;
            
            self.enableAnimation = NO;
            // Grab the Animation Block
            UIView *animationBlock = [self.page5 viewWithTag:2];
            if(!animationBlock) {
                return;
            }
            [animationBlock removeGestureRecognizer:self.studyHighlight3AnimBlockTap];
            
            UIView *image1 = (UIView *)[animationBlock viewWithTag:6];
            UIView *image2 = (UIView *)[animationBlock viewWithTag:4];
            UIImageView *image3 = (UIImageView *)[animationBlock viewWithTag:5];
            
            image1.alpha = 1.0;
            
            endFrame = image1.frame;
            endFrame.origin.x = 1024;
            image1.frame = endFrame;
            
            endFrame = image2.frame;
            endFrame.origin.x = 1024;
            image2.frame = endFrame;
            
            endFrame = image3.frame;
            endFrame.origin.x = 1024;
            image3.frame = endFrame;
            
            
            [image1 setHidden:YES];
            [image2 setHidden:YES];
            [image3 setHidden:YES];
            
        }
            break;
        case 7:
        {
            // Grab the Animation Blocks
            UIView *chartBlockTTP = [self.page8 viewWithTag:2];
            UIView *chartBlockOR = [self.page8 viewWithTag:10];
            UIView *chart1Description = [self.page8 viewWithTag:130];
            UIView *chart2Description = [self.page8 viewWithTag:135];
            if(!chartBlockTTP || !chartBlockOR || !chart1Description || !chart2Description) {
                return;
            }
            
            if(self.eff1ActiveChart >= 0 && self.eff1ActiveChart < 3) {
                [[chartBlockTTP viewWithTag:4] setHidden:YES];
                [[chartBlockTTP viewWithTag:5] setHidden:YES];
                [[chartBlockTTP viewWithTag:6] setHidden:YES];
            } else {
                // Hide other plots in this group
                [[chartBlockOR viewWithTag:84] setHidden:YES];
                [[chartBlockOR viewWithTag:85] setHidden:YES];
                [[chartBlockOR viewWithTag:86] setHidden:YES];
                [[chartBlockOR viewWithTag:87] setHidden:YES];
                [[chartBlockOR viewWithTag:110] setHidden:YES];
                [[chartBlockOR viewWithTag:111] setHidden:YES];
            }

        }
            break;
        case 8:
        {
            // Grab the Animation Blocks
            UIView *targetView = [self.page9 viewWithTag:130];
            
            // Study plot
            UIView *barChart1 = [targetView viewWithTag:3];
            UIView *barChart2 = [targetView viewWithTag:5];
            UILabel *label1 = (UILabel *)[targetView viewWithTag:4];
            UILabel *label2 = (UILabel *)[targetView viewWithTag:6];
            
            [barChart1 setHidden:YES];
            [barChart2 setHidden:YES];
            [label1 setHidden:YES];
            [label2 setHidden:YES];
        }
            break;
        case 11:
        {
            // Reset the position of the adverse reaction panel
            UIScrollView *panelScrollView = (UIScrollView *)[[self.page12 viewWithTag:25] viewWithTag:111];
            [panelScrollView setContentOffset:CGPointMake(0,0) animated:NO];
        }
            break;
        case 12:
        {
            // Reset the position of the adverse reaction panel
            UIScrollView *panelScrollView = (UIScrollView *)[[self.page13 viewWithTag:25] viewWithTag:111];
            [panelScrollView setContentOffset:CGPointMake(0,0) animated:NO];
        }
            break;
        case 13:
        {
            // Reset the position of the adverse reaction panel
            UIScrollView *panelScrollView = (UIScrollView *)[[self.page14 viewWithTag:25] viewWithTag:111];
            [panelScrollView setContentOffset:CGPointMake(0,0) animated:NO];
        }
            break;
        case 14:
        {
            UIView *container = self.page15;
            
            UIView *dosingChartTop1 = [container viewWithTag:12];
            UIView *dosingChartTop2 = [container viewWithTag:13];
            UIView *dosingChartBottom1 = [container viewWithTag:14];
            UIView *dosingChartBottom2 = [container viewWithTag:15];
            
            // Set charts to visible
            [dosingChartTop1 setHidden:YES];
            [dosingChartTop2 setHidden:YES];
            [dosingChartBottom1 setHidden:YES];
            [dosingChartBottom2 setHidden:YES];
        }
            break;
        case 15:
        {
            UIView *container = self.page16;
            
            // Study plot
            UIView *barChart1 = [container viewWithTag:3];
            UIView *barChart2 = [container viewWithTag:5];
            
            [barChart1 setHidden:YES];
            [barChart2 setHidden:YES];
        }
            break;
        case 16:
        {
            UIView *container = self.page17;
                
            UIView *dosingChart1 = [container viewWithTag:10];
            UIView *dosingChart2 = [container viewWithTag:11];
            UIView *dosingChart3 = [container viewWithTag:12];
            
            // Set charts to visible
            [dosingChart1 setHidden:YES];
            [dosingChart2 setHidden:YES];
            [dosingChart3 setHidden:YES];

        }
            break;
        case 17:
        {
            UIView *animBlock1 = [self.page18 viewWithTag:50];
            UIView *animBlock2 = [self.page18 viewWithTag:51];
            
            [[animBlock1 viewWithTag:3] setHidden:YES];
            [[animBlock2 viewWithTag:3] setHidden:YES];
        }
            break;
    }
    
    
    // Make all footer nav buttons inactive
    UIImageView *btnImage1 = (UIImageView *)[self.view viewWithTag:3000];
    UIImageView *btnImage2 = (UIImageView *)[self.view viewWithTag:3001];
    UIImageView *btnImage3 = (UIImageView *)[self.view viewWithTag:3002];
    UIImageView *btnImage4 = (UIImageView *)[self.view viewWithTag:3003];
    UIImageView *btnImage5 = (UIImageView *)[self.view viewWithTag:3004];
    
    btnImage1.image = [UIImage imageNamed:@"footer_Overview_thumb.png"];
    btnImage2.image = [UIImage imageNamed:@"footer_Highlights_thumb.png"];
    btnImage3.image = [UIImage imageNamed:@"footer_Efficacy_thumb_inactive.png"];
    btnImage4.image = [UIImage imageNamed:@"footer_safety_thumb.png"];
    btnImage5.image = [UIImage imageNamed:@"footer_Dosing_thumb.png"];
    
    if(self.swipeView.currentItemIndex >= 0 && self.swipeView.currentItemIndex <= 1) {
        btnImage1.image = [UIImage imageNamed:@"footer_Overview_thumb_active.png"];
    } else if (self.swipeView.currentItemIndex >= 2 && self.swipeView.currentItemIndex <= 6) {
        btnImage2.image = [UIImage imageNamed:@"footer_Highlights_thumb_active.png"];
    } else if (self.swipeView.currentItemIndex >= 7 && self.swipeView.currentItemIndex <= 10) {
        btnImage3.image = [UIImage imageNamed:@"footer_Efficacy_thumb_active.png"];
    } else if (self.swipeView.currentItemIndex >= 11 && self.swipeView.currentItemIndex <= 13) {
        btnImage4.image = [UIImage imageNamed:@"footer_safety_thumb_active.png"];
    } else if (self.swipeView.currentItemIndex >= 14 && self.swipeView.currentItemIndex <= 18) {
        btnImage5.image = [UIImage imageNamed:@"footer_Dosing_thumb_active.png"];
    }
    
    switch(self.swipeView.currentItemIndex) {
        case 0: {
            // Set home button to active state
            [homeBtn setImage:[UIImage imageNamed:@"Nav_Home_btn_active.png"] forState:UIControlStateNormal];
        }
            break;
        case 4: {
            //self.swipeView.scrollEnabled = NO;
            self.enableAnimation = YES;
            // Grab the Animation Block
            UIView *animationBlock = [self.page5 viewWithTag:2];
            if(!animationBlock) {
                return;
            }
            
            [animationBlock removeGestureRecognizer:self.studyHighlight3AnimBlockTap];
            [[animationBlock viewWithTag:4] setHidden:YES];
            [[animationBlock viewWithTag:5] setHidden:YES];
            [[animationBlock viewWithTag:6] setHidden:YES];
            
            [(UIImageView *)[[animationBlock viewWithTag:4] viewWithTag:-1] setImage:[UIImage imageNamed:@"M1.2B_cycle14_all_2.png"]];
            [[animationBlock viewWithTag:4] setAlpha:1.0];
            
            CGRect anim2Frame = [[animationBlock viewWithTag:4] frame];
            anim2Frame.size.width = 779;
            [[animationBlock viewWithTag:4] setFrame:anim2Frame];
            [[animationBlock viewWithTag:4] setAlpha:1.0];
            
            // Create animation with layers
            //UIView *image1View = [[UIView alloc] initWithFrame:CGRectMake(1024, 2, 816, 246)];
            //UIImageView *randomizeButton = [[UIImageView alloc] initWithFrame:CGRectMake(0, 46, 39, 200)];
            //randomizeButton.image = [UIImage imageNamed:@"randomizationButton.png"];
            //UIImageView *image1 = [[UIImageView alloc] initWithFrame:CGRectMake(42, 0, 776, 245)];
            //image1.image = [UIImage imageNamed:@"M1.2B_cycle14_all_new.png"];
            //image1.tag = 6;
            //image1View.tag = 3;
            
            UIView *image1 = (UIView *)[animationBlock viewWithTag:6];
            
            //[image1View addSubview:randomizeButton];
            //[image1View addSubview:image1];
            
            //[animationBlock addSubview:image1View];
             
            CGRect endFrame;
             
            endFrame = image1.frame;
            endFrame.origin.x = 1024;
            image1.frame = endFrame;
            endFrame.origin.x = 9;
            
             
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.8];
            [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
            [UIView setAnimationDidStopSelector:@selector(studyHighlight3Anim1Finished)];
            [UIView setAnimationDelegate:self];
             
            image1.frame = endFrame;
            [image1 setHidden:NO];
            
            [UIView commitAnimations];
             
            //[self.page5 setAlpha:0.5];
             
        }
            break;
        case 7: // Efficacy - 1
        {
            
            [self playEfficacy1Animation];
        }
            break;
        case 8: // Efficacy - 2
        {
            [self playEfficacy2Animation];
        }
            break;
        case 14:
        {
            [self playDosing1ChartAnimation];
        }
            break;
        case 15:
        {
            [self playDosing2ChartAnimation];
        }
            break;
        case 16:
        {
            [self playDosing3ChartAnimation];
        }
            break;
        case 17:
        {
            [self playDosing4Animation];
        }
            break;
    }
}

- (void)turnOffActiveButtonState {
    // Turn off active button's 'active state'
    UIButton *activeButton = (UIButton *)[self.view viewWithTag:(500 + self.activePageViewIndex)];
    switch (activeButton.tag) {
        case 500:
            [activeButton setImage:[UIImage imageNamed:@"Nav_Home_btn_inactive.png"] forState:UIControlStateNormal];
            break;
        case 502:
            [activeButton setImage:[UIImage imageNamed:@"Nav_PI_btn_inactive.png"] forState:UIControlStateNormal];
            break;
        case 503:
            [activeButton setImage:[UIImage imageNamed:@"Nav_Index_btn_inactive.png"] forState:UIControlStateNormal];
            break;
        case 504:
            [activeButton setImage:[UIImage imageNamed:@"Nav_Ref_btn_inactive.png"] forState:UIControlStateNormal];
            break;
        case 505:
            [activeButton setImage:[UIImage imageNamed:@"Nav_CPS_btn_inactive.png"] forState:UIControlStateNormal];
            break;
        case 506:
            [activeButton setImage:[UIImage imageNamed:@"Nav_ISI_btn_inactive.png"] forState:UIControlStateNormal];
            break;
    }
    
    // Make all footer nav buttons inactive
    UIImageView *btnImage1 = (UIImageView *)[self.view viewWithTag:3000];
    UIImageView *btnImage2 = (UIImageView *)[self.view viewWithTag:3001];
    UIImageView *btnImage3 = (UIImageView *)[self.view viewWithTag:3002];
    UIImageView *btnImage4 = (UIImageView *)[self.view viewWithTag:3003];
    UIImageView *btnImage5 = (UIImageView *)[self.view viewWithTag:3004];
    
    btnImage1.image = [UIImage imageNamed:@"footer_Overview_thumb.png"];
    btnImage2.image = [UIImage imageNamed:@"footer_Highlights_thumb.png"];
    btnImage3.image = [UIImage imageNamed:@"footer_Efficacy_thumb_inactive.png"];
    btnImage4.image = [UIImage imageNamed:@"footer_safety_thumb.png"];
    btnImage5.image = [UIImage imageNamed:@"footer_Dosing_thumb.png"];
}

- (IBAction)displaySutdyPage:(id)sender {
    UIButton *targetButton = (UIButton *)sender;
    UIView *tmpView = self.activePageView;
    
    UIImageView *btnImage1 = (UIImageView *)[self.view viewWithTag:3000];
    UIImageView *btnImage2 = (UIImageView *)[self.view viewWithTag:3001];
    UIImageView *btnImage3 = (UIImageView *)[self.view viewWithTag:3002];
    UIImageView *btnImage4 = (UIImageView *)[self.view viewWithTag:3003];
    UIImageView *btnImage5 = (UIImageView *)[self.view viewWithTag:3004];
    
    switch(targetButton.tag) {
        case 10:
        {
            // Display requested view
            self.activePageView = [self.view viewWithTag:800];
            [self.swipeView scrollToItemAtIndex:1 duration:0];
            
            // Turn off active button's 'active state'
            [self turnOffActiveButtonState];
            
            // Update active page index
            self.activePageViewIndex = 0;
            
            // Switch 'Home' button to active state
            //[(UIButton *)[self.view viewWithTag:500] setImage:[UIImage imageNamed:@"Nav_Home_btn_active.png"] forState:UIControlStateNormal];
            
            
            // Hide current view
            [tmpView setHidden:YES];
            
            // Show requested view
            [self.activePageView setHidden:NO];
            
            btnImage1.image = [UIImage imageNamed:@"footer_Overview_thumb_active.png"];
        }
            break;
        case 11:
        {
            // Display requested view
            self.activePageView = [self.view viewWithTag:800];
            [self.swipeView scrollToItemAtIndex:2 duration:0];
            
            // Turn off active button's 'active state'
            [self turnOffActiveButtonState];
            
            // Update active page index
            self.activePageViewIndex = 0;
            
            
            // Hide current view
            [tmpView setHidden:YES];
            
            // Show requested view
            [self.activePageView setHidden:NO];
            
            btnImage2.image = [UIImage imageNamed:@"footer_Highlights_thumb_active.png"];
        }
            break;
        case 12:
        {
            // Display requested view
            self.activePageView = [self.view viewWithTag:800];
            [self.swipeView scrollToItemAtIndex:7 duration:0];
            
            // Turn off active button's 'active state'
            [self turnOffActiveButtonState];
            
            // Update active page index
            self.activePageViewIndex = 0;
            
            
            // Hide current view
            [tmpView setHidden:YES];
            
            // Show requested view
            [self.activePageView setHidden:NO];
            
            btnImage3.image = [UIImage imageNamed:@"footer_Efficacy_thumb_active.png"];
        }
            break;
        case 13:
        {
            // Display requested view
            self.activePageView = [self.view viewWithTag:800];
            [self.swipeView scrollToItemAtIndex:11 duration:0];
            
            // Turn off active button's 'active state'
            [self turnOffActiveButtonState];
            
            // Update active page index
            self.activePageViewIndex = 0;
            
            
            // Hide current view
            [tmpView setHidden:YES];
            
            // Show requested view
            [self.activePageView setHidden:NO];
            
            btnImage4.image = [UIImage imageNamed:@"footer_safety_thumb_active.png"];
        }
            break;
        case 14:
        {
            // Display requested view
            self.activePageView = [self.view viewWithTag:800];
            [self.swipeView scrollToItemAtIndex:14 duration:0];
            
            // Turn off active button's 'active state'
            [self turnOffActiveButtonState];
            
            // Update active page index
            self.activePageViewIndex = 0;
            
            
            // Hide current view
            [tmpView setHidden:YES];
            
            // Show requested view
            [self.activePageView setHidden:NO];
            
            btnImage5.image = [UIImage imageNamed:@"footer_Dosing_thumb_active.png"];
        }
            break;
    }
}

- (void)setPageToSubviewTag:(NSInteger)viewTag {
    // Hide current view
    [self.activePageView setHidden:YES];
    
    // Display requested view
    self.activePageView = [self.view viewWithTag:viewTag];
    
    switch(viewTag) {
        case 800:
        {
            [self.swipeView scrollToItemAtIndex:0 duration:0];
            break;
        }
        case 801:
        {
            [self.swipeView2 scrollToItemAtIndex:0 duration:0];
        }
            break;
    }
    [self.activePageView setHidden:NO];
}

- (IBAction)showPage:(id)sender {
    UIButton *targetButton = (UIButton *)sender;
    
    // Turn off active button's 'active state'
    [self turnOffActiveButtonState];
    
    switch(targetButton.tag) {
        case 500: // Revlimid Info Page
        {
            if(self.activePageViewIndex != 0 || [self.swipeView getLastIndex] != 0) {
                self.activePageViewIndex = 0;
                
                // Set current view
                [self setPageToSubviewTag:800];
                
                // Change linking button's image to active state
                [targetButton setImage:[UIImage imageNamed:@"Nav_Home_btn_active.png"] forState:UIControlStateNormal];
                
                // Footer nav
                [(UIImageView *)[self.view viewWithTag:3000] setImage:[UIImage imageNamed:@"footer_Overview_thumb.png"]];
            }
        }
            break;
        case 501: // Revlimid Info Page
        {
            if(self.activePageViewIndex != 1) {
                self.activePageViewIndex = 1;
                
                // Set current view
                [self setPageToSubviewTag:801];
            }
        }
            break;
        case 1001:
        case 502: // Revlimid PI page
        {
            if(self.activePageViewIndex != 2) {
                self.activePageViewIndex = 2;
                
                // Reset the position of the scroll view
                [[(UIWebView *)[self.view viewWithTag:802] scrollView] setContentOffset:CGPointMake(0,0) animated:NO];
                
                // Set current view
                [self setPageToSubviewTag:802];
                
                // Change linking button's image to active state
                [(UIButton *)[[self.view viewWithTag:900] viewWithTag:502] setImage:[UIImage imageNamed:@"Nav_PI_btn_active.png"] forState:UIControlStateNormal];
            }
        }
            break;
        case 503: // Revlimid Menu Page
        {
            if(self.activePageViewIndex != 3) {
                self.activePageViewIndex = 3;
                
                // Reset the state of all the buttons in Menu page
                [(UIButton *)[self.revlimidMenuPage viewWithTag:10] setImage:[UIImage imageNamed:@"M1_overview_inactive.png"] forState:UIControlStateNormal];
                [(UIButton *)[self.revlimidMenuPage viewWithTag:11] setImage:[UIImage imageNamed:@"M1_highlights_inactive.png"] forState:UIControlStateNormal];
                [(UIButton *)[self.revlimidMenuPage viewWithTag:12] setImage:[UIImage imageNamed:@"M1_efficacy_inactive.png"] forState:UIControlStateNormal];
                [(UIButton *)[self.revlimidMenuPage viewWithTag:13] setImage:[UIImage imageNamed:@"M1_adverse_events_inactive.png"] forState:UIControlStateNormal];
                [(UIButton *)[self.revlimidMenuPage viewWithTag:14] setImage:[UIImage imageNamed:@"M1_dosing_inactive.png"] forState:UIControlStateNormal];
                
                // Set current view
                [self setPageToSubviewTag:803];
                
                // Change linking button's image to active state
                [targetButton setImage:[UIImage imageNamed:@"Nav_Index_btn_active.png"] forState:UIControlStateNormal];
            }
        }
            break;
        case 504: // Revlimid Reference Page
        {
            if(self.activePageViewIndex != 4) {
                self.activePageViewIndex = 4;
                
                // Set current view
                [self setPageToSubviewTag:804];
                
                // Change linking button's image to active state
                [targetButton setImage:[UIImage imageNamed:@"Nav_Ref_btn_active.png"] forState:UIControlStateNormal];
            }
        }
            break;
        case 505: // Celegene Support Page
        {
            if(self.activePageViewIndex != 5) {
                self.activePageViewIndex = 5;
                
                // Set current view
                [self setPageToSubviewTag:805];
                
                // Change linking button's image to active state
                [targetButton setImage:[UIImage imageNamed:@"Nav_CPS_btn_active.png"] forState:UIControlStateNormal];
            }
        }
            break;
        case 1002:
        case 506: // Important Safety Information Page
        {
            if(self.activePageViewIndex != 6) {
                self.activePageViewIndex = 6;
                
                // Reset the position of the scroll view
                [self.safetyInfoScroller setContentOffset:CGPointMake(0,0) animated:NO];
                
                // Set current view
                [self setPageToSubviewTag:806];
                
                // Change linking button's image to active state
                [(UIButton *)[[self.view viewWithTag:900] viewWithTag:506] setImage:[UIImage imageNamed:@"Nav_ISI_btn_active.png"] forState:UIControlStateNormal];
            }
        }
            break;
    }
}

- (UIView *)swipeView:(SwipeView *)swipeView viewForItemAtIndex:(NSInteger)index reusingView:(UIView *)view
{
    [self resetTapToExpand];
    if(swipeView.tag == 800) {
        switch(index) {
            case 0:
                return self.page1;
                break;
            case 1:
                return self.page2;
                break;
            case 2:
                return self.page3;
                break;
            case 3:
                return self.page4;
                break;
            case 4:
                return self.page5;
                break;
            case 5:
                return self.page6;
                break;
            case 6:
                return self.page7;
                break;
            case 7:
                return self.page8;
                break;
            case 8:
                return self.page9;
                break;
            case 9:
                return self.page10;
                break;
            case 10:
                return self.page11;
                break;
            case 11:
                return self.page12;
                break;
            case 12:
                return self.page13;
                break;
            case 13:
                return self.page14;
                break;
            case 14:
                return self.page15;
                break;
            case 15:
                return self.page16;
                break;
            case 16:
                return self.page17;
                break;
            case 17:
                return self.page18;
                break;
            case 18:
                return self.page19;
                break;
        }
    } else {
        switch (index) {
            case 0:
                return self.revlimidInfoPage1;
                break;
            case 1:
                return self.revlimidInfoPage2;
                break;
            case 2:
                return self.revlimidInfoPage3;
                break;
        }
    }

    // Should never reach this point
    return view;
}

- (CGSize)swipeViewItemSize:(SwipeView *)swipeView
{
    return self.swipeView.bounds.size;
}

@end
