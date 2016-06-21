///////////////////////////////////////////////////////////////////////////////
// Project: Hoyo Client Game
//
// hy_function.cpp
// Created: 2012-8-6
// Author: Like Cheng
///////////////////////////////////////////////////////////////////////////////
#include "ad_function.h"
//#include <iconv.h>
#include <fstream>
#include <stdio.h>
#include <iostream>

#import "GameConst.h"
#import <HeyzapAds/HeyzapAds.h>
//#include <AdsHeyZap.h>
//#include <AdsMogoInterstitial.h>
#include <AdsmogoBanner.h>

#include "GameController.hpp"

#include "cocos2d.h"

#include "UIManage.hpp"

#define LOG_METHOD_NAME_TO_CONSOLE_WITH_STRING(str) [self logToConsole:[NSString stringWithFormat:@"%@ %@", NSStringFromSelector(_cmd), str]]

@interface HZIncentivized: NSObject<HZIncentivizedAdDelegate>
{
    
}

@end

@implementation HZIncentivized

+ (HZIncentivized *)sharedIncentivized {
    static HZIncentivized *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [self new];
    });
    return instance;
}

/** Called when a user successfully completes viewing an ad */
- (void)didCompleteAdWithTag: (NSString *) tag{
    GameController::getInstance()->addTokenForAdReward(500);
    UIManage::getInstance()->updateUI(UI_SONGS_);
//    LOG_METHOD_NAME_TO_CONSOLE_WITH_STRING(tag);
}
/** Called when a user does not complete the viewing of an ad */
- (void)didFailToCompleteAdWithTag: (NSString *) tag{
//    LOG_METHOD_NAME_TO_CONSOLE_WITH_STRING(tag);
}

-(void)showIncentivizedAd{
    [HZIncentivizedAd fetchWithCompletion:^(BOOL result, NSError *error) {
        if (result) {
            [HZIncentivizedAd setDelegate:self];
            [HZIncentivizedAd show];
        }
    }];
}


-(void)dealloc{
    [super dealloc];
}

@end



static ad_function* ad_function_instance_p_ = NULL;
//////////////////////////////////////////////////////////////////////////////////
//hy_function::form_code
//////////////////////////////////////////////////////////////////////////////////

void  ad_function::init()
{
    
}

ad_function* ad_function::instance()
{
    if (ad_function_instance_p_ == NULL){
        ad_function_instance_p_ = new ad_function();
        ad_function_instance_p_->init();
    }
    return ad_function_instance_p_;
}

void ad_function::prepareBanner(){
    
    if ([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPhone) {
        AdsmogoBanner::sharedBanner()->createBanner(Mogo_Key_Iphone,AdsmogoBannerTypeNormalBanner,
                                                    AdMoGoPointTypeDown_middle,false);
    }else{
        AdsmogoBanner::sharedBanner()->createBanner(Mogo_Key_Ipad,AdsmogoBannerTypeLargeBanner,
                                                    AdMoGoPointTypeDown_middle,false);
    }
    
    
    hideBanner();
    
}

void ad_function::showBanner(){
    AdsmogoBanner::sharedBanner()->showBanner();
}

void ad_function::hideBanner(){
    AdsmogoBanner::sharedBanner()->hidenBanner();
}

void ad_function::releaseBanner(){
    AdsmogoBanner::sharedBanner()->releaseBanner();
}

void ad_function::showVideo(){
    [HZVideoAd fetchWithCompletion:^(BOOL result, NSError *error) {
        if (result) {
            if(!GameController::getInstance()->isIngame()){
                [HZVideoAd show];
            }
        }
    }];
    
}


void ad_function::showInterstitial(){
    [HZInterstitialAd fetchWithCompletion:^(BOOL result, NSError *error) {
        if (result) {
            if(!GameController::getInstance()->isIngame()){
                [HZInterstitialAd show];
            }
        }
    }];
}

void ad_function::incentivized(){
    
    
    [[HZIncentivized sharedIncentivized] showIncentivizedAd];
    //    [HZIncentivizedAd show];
}


