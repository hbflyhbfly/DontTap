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

//#include <AdsMogoInterstitial.h>
#include <AdsmogoBanner.h>

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
    AdsmogoBanner::sharedBanner()->createBanner(Mogo_Key,AdsmogoBannerTypeNormalBanner,
                                                AdMoGoPointTypeDown_middle,false);
    
}

void ad_function::showBanner(){
    AdsmogoBanner::sharedBanner()->refreshBanner();
    AdsmogoBanner::sharedBanner()->showBanner();
}

void ad_function::hideBanner(){
    AdsmogoBanner::sharedBanner()->hidenBanner();
}

void ad_function::showVideo(){
    [HZVideoAd fetch];
    [HZVideoAd show];
}


void ad_function::showInterstitial(){
    [HZInterstitialAd fetch];
    [HZInterstitialAd show];
}
