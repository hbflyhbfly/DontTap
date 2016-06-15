//
//  AdMoGoAdapterDMBFullAd.m
//  TestMOGOSDKAPP
//
//  Created by 靳磊 on 12-11-16.
//
//

#import "AdMoGoAdapterDMbFullAd.h"
#import "AdMoGoAdNetworkRegistry.h"
#import "AdMoGoAdNetworkAdapter+Helpers.h"
#import "AdMoGoAdNetworkConfig.h"
#import "AdMoGoConfigDataCenter.h"
#import "AdMoGoConfigData.h"
#import "AdMoGoDeviceInfoHelper.h"

#import "AdMoGoAdSDKInterstitialNetworkRegistry.h"
#import "DMInterstitialAdController+AdsMogo.h"
@implementation AdMoGoAdapterDMbFullAd

+ (AdMoGoAdNetworkType)networkType{
    return AdMoGoAdNetworkTypeDMb;
}

+ (void)load{
    [[AdMoGoAdSDKInterstitialNetworkRegistry sharedRegistry] registerClass:self];
}

- (void)getAd{
    isStop = NO;
    isRequest = NO;
    isStopTimer = NO;
    //获取用于展示插屏的UIViewController

    UIViewController *uiViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    if(!uiViewController){
        uiViewController = [self rootViewControllerForPresent];
    }

    if(uiViewController){
        NSString *publishId = nil;
        NSString *placementId = nil;
        id key = [self.ration objectForKey:@"key"];
        if ([key isKindOfClass:[NSDictionary class]]) {
            publishId  = [key objectForKey:@"PublisherId"];
            placementId = [key objectForKey:@"PlacementId"];
        }
        else{
            [self adapter:self didFailAd:nil];
        }
        if (_dmInterstitial == nil) {
            _dmInterstitial = [[DMInterstitialAdController alloc]
                               initWithPublisherId:publishId
                               placementId:placementId
                               rootViewController:uiViewController
                               ];
            // 设置插屏广告的Delegate
            _dmInterstitial.delegate = self;
        }
        
        //开始加载广告
        [_dmInterstitial loadAd];
        //芒果调用多盟请求
        [_dmInterstitial dbInterstitialAdLoad];
        
        [self adapterDidStartRequestAd:self];
        
        id _timeInterval = [self.ration objectForKey:@"to"];
        if ([_timeInterval isKindOfClass:[NSNumber class]]) {
            timer = [[NSTimer scheduledTimerWithTimeInterval:[_timeInterval doubleValue] target:self selector:@selector(loadAdTimeOut:) userInfo:nil repeats:NO] retain];
        }
        else{
            timer = [[NSTimer scheduledTimerWithTimeInterval:AdapterTimeOut15 target:self selector:@selector(loadAdTimeOut:) userInfo:nil repeats:NO] retain];
        }
    }
    else{
        [self adapter:self didFailAd:nil];
    }
}

-(void)stopBeingDelegate{
    if(_dmInterstitial){
        // 芒果调用多盟取消展示
        [_dmInterstitial dbInterstitialAdDismiss];
        _dmInterstitial.delegate = nil;
        [_dmInterstitial release],_dmInterstitial = nil;
    }
}

- (void)stopAd{
    [self stopBeingDelegate];
    isStop = YES;
}

- (void)dealloc {
    [self stopBeingDelegate];
    [super dealloc];
}

- (BOOL)isReadyPresentInterstitial{
    return _dmInterstitial.isReady;
}

- (void)presentInterstitial{
    // 呈现插屏广告
    [_dmInterstitial present];
    
    //芒果调用多盟展示
    [_dmInterstitial dbInterstitialAdImpression];
}
#pragma mark DMB Delegate
// Sent when an ad request success to loaded an ad
- (void)dmInterstitialSuccessToLoadAd:(DMInterstitialAdController *)dmInterstitial{
    if (isStop) {
        return;
    }
    [self stopTimer];
    
    [self adapter:self didReceiveInterstitialScreenAd:dmInterstitial];
}
// Sent when an ad request fail to loaded an ad
- (void)dmInterstitialFailToLoadAd:(DMInterstitialAdController *)dmInterstitial withError:(NSError *)err{
    if(isStop){
        return;
    }
    
    [self stopTimer];
    if (!isRequest) {
        isRequest = YES;
    }else{
        return;
    }
    MGLog(MGE,@"dm interstitial error-->%@",err);
    [self adapter:self didFailAd:nil];
}
// Sent when the ad is clicked
- (void)dmInterstitialDidClicked:(DMInterstitialAdController *)dmInterstitial{
     [self specialSendRecordNum];
}

// Sent just before presenting an interstitial
- (void)dmInterstitialWillPresentScreen:(DMInterstitialAdController *)dmInterstitial{
    [self adapter:self willPresent:dmInterstitial];
    [self adapter:self didShowAd:dmInterstitial];
}
// Sent just after dismissing an interstitial
- (void)dmInterstitialDidDismissScreen:(DMInterstitialAdController *)dmInterstitial{
    [self adapter:self didDismissScreen:dmInterstitial];
}
// Sent just fail to present an interstitial because of error orientation
- (void)dmInterstitialFailToPresentScreenForErrorOrientation:(DMInterstitialAdController *)dmInterstitial{

}

// Sent just before presenting the user a modal view
- (void)dmInterstitialWillPresentModalView:(DMInterstitialAdController *)dmInterstitial{
    [self adapterAdModal:self willPresent:dmInterstitial];
}
// Sent just after dismissing the modal view.
- (void)dmInterstitialDidDismissModalView:(DMInterstitialAdController *)dmInterstitial{
    [self adapterAdModal:self didDismissScreen:dmInterstitial];
}
// Sent just before the application will background or terminate because the user's action
// (Such as the user clicked on an ad that will launch App Store).
- (void)dmInterstitialApplicationWillEnterBackground:(DMInterstitialAdController *)dmInterstitial{

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
    [self adapter:self didFailAd:nil];
}
@end
