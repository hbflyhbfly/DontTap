//
//  AdMoGoAdapterDoMob.m
//  TestMOGOSDKAPP
//
//  Created by 孟令之 on 12-11-16.
//
//

#import "AdMoGoAdapterDMb.h"
#import "AdMoGoAdNetworkRegistry.h"
#import "AdMoGoAdNetworkAdapter+Helpers.h"
#import "AdMoGoAdNetworkConfig.h"
#import "AdMoGoConfigDataCenter.h"
#import "AdMoGoDeviceInfoHelper.h"

#import "AdMoGoAdSDKBannerNetworkRegistry.h"
#import "DMAdView+AdsMogo.h"
@implementation AdMoGoAdapterDMb

+ (AdMoGoAdNetworkType)networkType{
    return AdMoGoAdNetworkTypeDMb;
}

+ (void)load{
    [[AdMoGoAdSDKBannerNetworkRegistry sharedRegistry] registerClass:self];
}


- (void)getAd{
    isStop = NO;
    isStopTimer = NO;
    [adMoGoCore adapter:self didGetAd:@"dmb"];
    [adMoGoCore adDidStartRequestAd];
    AdMoGoConfigDataCenter *configDataCenter = [AdMoGoConfigDataCenter singleton];
    configData = [configDataCenter.config_dict objectForKey:adMoGoCore.config_key];
    AdViewType type = [configData.ad_type intValue];
    CGSize size = CGSizeZero;
    switch (type) {
        case AdViewTypeNormalBanner:
        case AdViewTypeiPadNormalBanner:
            size = DB_AD_SIZE_320x50;
            break;
        case AdViewTypeiPhoneRectangle:
            size = DB_AD_SIZE_300x250;
            break;
        case AdViewTypeMediumBanner:
            size = DB_AD_SIZE_488x80;
            break;
        case AdViewTypeRectangle:
            size = DB_AD_SIZE_600x500;
            break;
        case AdViewTypeLargeBanner:
            size = DB_AD_SIZE_728x90;
            break;
        default:
            [adMoGoCore adapter:self didFailAd:nil];
            return;
            break;
    }

    NSString *publishId = [[self.ration objectForKey:@"key"] objectForKey:@"PublisherId"];
    NSString *placementId = [[self.ration objectForKey:@"key"] objectForKey:@"PlacementId"];

    
    dmAdView = [[DMAdView alloc] initWithPublisherId:publishId
                                         placementId:placementId
                                         autorefresh:NO];
    [dmAdView setAdSize:size];
    [dmAdView setFrame:CGRectMake(0.0, 0.0, size.width, size.height)];

    dmAdView.rootViewController = [adMoGoDelegate viewControllerForPresentingModalView];
    dmAdView.delegate = self;
    self.adNetworkView = dmAdView;
    [dmAdView release];
    [dmAdView loadAd];
    /*
        芒果调用多盟请求
     */
    [dmAdView dbAdLoad];
    id _timeInterval = [self.ration objectForKey:@"to"];
    if ([_timeInterval isKindOfClass:[NSNumber class]]) {
        timer = [[NSTimer scheduledTimerWithTimeInterval:[_timeInterval doubleValue] target:self selector:@selector(loadAdTimeOut:) userInfo:nil repeats:NO] retain];
    }
    else{
        timer = [[NSTimer scheduledTimerWithTimeInterval:AdapterTimeOut8 target:self selector:@selector(loadAdTimeOut:) userInfo:nil repeats:NO] retain];
    }
    
}

- (void)stopBeingDelegate{
    DMAdView *aDMAdView = (DMAdView *)self.adNetworkView;
    if (aDMAdView) {
        if (aDMAdView) {
            /*
                芒果调用多盟结束
             */
            [aDMAdView dbAdDismiss];
        }
        aDMAdView.delegate = nil;
        aDMAdView.rootViewController = nil;
        
    }
}

- (void)stopAd{
    isStop = YES;
    [self stopTimer];
    [self stopBeingDelegate];
}

- (void)dealloc{
    
    [super dealloc];
}


- (void)stopTimer {
    if (!isStopTimer) {
        isStopTimer = YES;
    }else{
        return;
    }
    if (timer) {
        [timer invalidate];
        [timer release];
        timer = nil;
    }
}





/*2013*/
- (void)loadAdTimeOut:(NSTimer*)theTimer {
    if (isStop) {
        return;
    }
    [super loadAdTimeOut:theTimer];
    
    [self stopTimer];
    [self stopBeingDelegate];
    
    [adMoGoCore adapter:self didFailAd:nil];
}

- (BOOL)isSDKSupportClickDelegate{
    return YES;
}



// Sent when an ad request success to loaded an ad
- (void)dmAdViewSuccessToLoadAd:(DMAdView *)adView{
    if (isStop) {
        return;
    }
    [self stopTimer];
    /*
     芒果调用多盟展示
     */
    [dmAdView dbAdImpression];
    [adMoGoCore adapter:self didReceiveAdView:adView];

}


// Sent when an ad request fail to loaded an ad
- (void)dmAdViewFailToLoadAd:(DMAdView *)adView withError:(NSError *)error{
    if (isStop) {
        return;
    }
    [self stopTimer];
    MGLog(MGE, @"dm error %@",error);
    [adMoGoCore adapter:self didFailAd:nil];
}


// Sent when the ad view is clicked
- (void)dmAdViewDidClicked:(DMAdView *)adView{
    if (isStop) {
        return;
    }
    [adMoGoCore sdkplatformSendCLK:self];
}
// Sent just before presenting the user a modal view
- (void)dmWillPresentModalViewFromAd:(DMAdView *)adView{

}
// Sent just after dismissing the modal view
- (void)dmDidDismissModalViewFromAd:(DMAdView *)adView{

}
// Sent just before the application will background or terminate because the user's action
// (Such as the user clicked on an ad that will launch App Store).
- (void)dmApplicationWillEnterBackgroundFromAd:(DMAdView *)adView{

}


@end
