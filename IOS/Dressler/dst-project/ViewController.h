//
//  ViewController.h
//  dst
//
//  Created by Haleeq Usman on 11/26/2013.
//  Copyright (c) 2013 Dressler llc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SwipeView.h"
#import "AccordionView.h"
#import "TTTAttributedLabel.h"

@interface CALayer(XibConfiguration)

// This assigns a CGColor to borderColor.
@property(nonatomic, assign) UIColor* borderUIColor;

// This assigns a CGColor to backgroundColor.
@property(nonatomic, assign) UIColor* backgroundUIColor;

@end

@interface ViewController : UIViewController <SwipeViewDataSource, SwipeViewDelegate, UIScrollViewDelegate, TTTAttributedLabelDelegate>

@property (nonatomic, weak) IBOutlet IBOutlet UILabel *lbl1;

@property (nonatomic, weak) IBOutlet SwipeView *swipeView;
@property (nonatomic, weak) IBOutlet SwipeView *swipeView2;
@property (nonatomic, strong) NSMutableArray *items;

// page views
@property (nonatomic, retain) IBOutlet UIView *page1;
@property (nonatomic, retain) IBOutlet UIView *page2;

// Study Highlights
@property (nonatomic, retain) IBOutlet UIView *page3;
@property (nonatomic, retain) IBOutlet UIView *page4;
@property (nonatomic, retain) IBOutlet UIView *page5;
@property (nonatomic, retain) IBOutlet UIView *page6;
@property (nonatomic, retain) IBOutlet UIView *page7;

// Efficacy
@property (nonatomic, retain) IBOutlet UIView *page8;
@property (nonatomic) NSInteger eff1ActiveChart;
@property (nonatomic, retain) IBOutlet UIView *page9;
@property (nonatomic, retain) IBOutlet UIView *page10;
@property (nonatomic, retain) IBOutlet UIView *page11;

// Adverse Events
@property (nonatomic, retain) IBOutlet UIView *page12;
@property (nonatomic, retain) IBOutlet UIView *page13;
@property (nonatomic, retain) IBOutlet UIView *page14;

- (IBAction)dosing1ChartTapped:(id)sender;
- (IBAction)dosing3ChartTapped:(id)sender;

// Dosing
@property (nonatomic, retain) IBOutlet UIView *page15;
@property (nonatomic, retain) IBOutlet UIView *page16;
@property (nonatomic, retain) IBOutlet UIView *page17;
@property (nonatomic, retain) IBOutlet UIView *page18;
@property (nonatomic, retain) IBOutlet UIView *page19;
@property (nonatomic) NSInteger dosing4Chart;

- (IBAction)switchDosing4Animation:(id)sender;
- (IBAction)showDosingItem:(id)sender;

// View: Overview #2 properties/methods
@property (nonatomic, retain) IBOutlet UIImageView *molecule;

// View: Study-Highlight #1 properties/methods
- (IBAction)tapToExpand:(id)sender;
@property (nonatomic, strong) AccordionView *accordion;

// View: Study-Highlight #3 properties/methods
@property(nonatomic, assign) Boolean enableAnimation;
@property(nonatomic) UITapGestureRecognizer *studyHighlight3AnimBlockTap;
@property(nonatomic) NSInteger eff1label1Num;
@property(nonatomic) NSInteger eff1label1Num2;
@property(nonatomic) NSTimer *eff1AnimationTimer;
- (IBAction)switchEfficacy1Animation:(id)sender;
- (IBAction)showEfficacy1Popup:(id)sender;
- (IBAction)switchEfficacy1Chart:(id)sender;


// Revlimid Information
@property (nonatomic, retain) IBOutlet UIView *revlimidInfoPage1;
@property (nonatomic, retain) IBOutlet UIView *revlimidInfoPage2;
@property (nonatomic, retain) IBOutlet UIView *revlimidInfoPage3;

- (IBAction)revInfoExpandContent:(id)sender;
- (IBAction)revInfoswitchContent:(id)sender;

// Other views
@property (nonatomic, retain) IBOutlet UIView *revlimidMenuPage;
@property (nonatomic, retain) IBOutlet UIView *revlimidReferencePage;
@property (nonatomic, retain) IBOutlet UIView *celegeneSupportPage;
@property (nonatomic, retain) IBOutlet UIView *safetyInformationPage;
@property (nonatomic, retain) IBOutlet UIView *launchPage;
@property (nonatomic, retain) IBOutlet UIView *SIAcceptView;
@property (nonatomic, retain) IBOutlet UIView *ISIInfoView;
@property (nonatomic, retain) IBOutlet UIView *PIViewPage;
@property (nonatomic, retain) IBOutlet UIView *footerNav;

// Important Safety page properties/methods
@property (nonatomic, weak) IBOutlet UIScrollView *safetyInfoScroller;
@property (nonatomic, weak) IBOutlet UIScrollView *safetyAcceptScroller;

- (IBAction)switchCelgeneSupportTab:(id)sender;

// Header/sub-header properties/methods
- (IBAction)showPage:(id)sender;

// Core properties/methods
@property(nonatomic) NSInteger activePageViewIndex;
@property(nonatomic, strong) UIView *activePageView;

- (IBAction)showLaunchPopup:(id)sender ;
- (IBAction)displaySutdyPage:(id)sender;
- (IBAction)closeISIInfoPopup:(id)sender;
- (IBAction)closePIPopup:(id)sender;
- (IBAction)toggleFooterNav:(id)sender;

- (void)setPageToSubviewTag:(NSInteger)viewTag;
- (void)makeLabelIntoLink:(UILabel *)lbl parentView:(UIView *)parent;

@property (nonatomic, strong) NSArray *tapToExpandViews;

@property (nonatomic, retain) IBOutlet UIView *testv;

- (IBAction)testButtonTapped:(id)sender;

@end
