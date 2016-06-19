//
//  AdsMogoInterstitial.cpp
//  AdsMogoCocos2dxSample
//
//  Created by Castiel on 2015-3-26
//
//

#include "AdsMogoInterstitial.h"
#include "cocos2d.h"
#import "AdMoGoView.h"
#import "AppController.h"
//#import "EAGLView.h"
#import "AdMoGoInterstitial.h"
#import "AdMoGoLogCenter.h"
#import "AdMoGoInterstitialManager.h"

@interface InterstitialObj :NSObject<AdMoGoInterstitialDelegate,AdMoGoWebBrowserControllerUserDelegate>
{
    AdMoGoInterstitial* interstitial;
}
-(AdMoGoInterstitial*)createInterstitial:(NSString*)mogoid andInterType:(AdViewType)interType  isManualRefresh:(BOOL) ismanualrefresh isvideo:(BOOL)isVideo;
-(void)hideInterstitial;
@end
@implementation InterstitialObj

+ (InterstitialObj *)sharedInterstitialObj {
    static InterstitialObj *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [self new];
    });
    return instance;
}



-(AdMoGoInterstitial*)createInterstitial:(NSString*)mogoid andInterType:(AdViewType)interType  isManualRefresh:(BOOL) ismanualrefresh isvideo:(BOOL)isVideo{
    if (interstitial) {
        interstitial.delegate = nil;
      if (isVideo) {
          [[AdMoGoInterstitialManager shareInstance] removeVideoInterstitialInstanceByAppKey:mogoid];
      }else{
          [[AdMoGoInterstitialManager shareInstance] removeInterstitialInstanceByAppKey:mogoid];
      }

    }
    [self showMogoLog];
    //常规初始化
  if(isVideo){
   interstitial= [[AdMoGoInterstitialManager shareInstance] adMogoVideoInterstitialByAppKey:mogoid isManualRefresh:ismanualrefresh];
  }else{
    interstitial= [[AdMoGoInterstitialManager shareInstance] adMogoInterstitialByAppKey:mogoid isManualRefresh:ismanualrefresh];
  }
  
     interstitial.delegate = self;
    //手动刷新方式初始化
//    interstitial= [[AdMoGoInterstitialManager shareInstance] adMogoInterstitialByAppKey:mogoid isManualRefresh:YES];
    interstitial.adWebBrowswerDelegate = self;
    return nil;
}


-(void)showInterstitial{
    if (interstitial) {
        [interstitial interstitialShow:YES];
    }
}



-(void)showMogoLog{
    [[AdMoGoLogCenter shareInstance] setLogLeveFlag:AdMoGoLogTemp];
      [[AdMoGoLogCenter shareInstance] setLogLeveFlag:AdMoGoLogProduction];
      [[AdMoGoLogCenter shareInstance] setLogLeveFlag:AdMoGoLogDebug];
}

//手动刷新接口
-(void)refreshAd{
    
    [interstitial refreshAd];
}


-(void)hideInterstitial{
    if (interstitial) {
        [interstitial interstitialCancel];
    }
}

-(void)dealloc{
    [super dealloc];
}


#pragma mark  -AdMoGoInterstitialDelegate
/*
 Return to advertising(rootViewController)
 */
- (UIViewController *)viewControllerForPresentingInterstitialModalView{
    UIViewController *result = nil;
    
    UIWindow *topWindow = [[UIApplication sharedApplication] keyWindow];
    
    if (topWindow.windowLevel != UIWindowLevelNormal){
        NSArray *windows = [[UIApplication sharedApplication] windows];
        for(topWindow in windows){
            if (topWindow.windowLevel == UIWindowLevelNormal){
                break;
            }
        }
    }
    UIView *rootView = [[topWindow subviews] objectAtIndex:0];
    id nextResponder = [rootView nextResponder];
    if ([nextResponder isKindOfClass:[UIViewController class]]){
        
        result = nextResponder;
        
    }else if ([topWindow respondsToSelector:@selector(rootViewController)] && topWindow.rootViewController != nil){
        
        result = topWindow.rootViewController;
        
    }
    return  result;
}
/*
 Full screen advertising began to request
 */
- (void)adsMoGoInterstitialAdDidStart{
    
}
/*
 Full screen advertising ready
 */
- (void)adsMoGoInterstitialAdIsReady{
   
}

/*
Full screen AD receive success
 */
- (void)adsMoGoInterstitialAdReceivedRequest{
    
}
/*
Full screen will display advertising
 */
- (void)adsMoGoInterstitialAdWillPresent{
    
}

/*
Full screen AD receive failure
 */
- (void)adsMoGoInterstitialAdFailedWithError:(NSError *) error{

}

/*
Full screen advertising disappeared
 */
- (void)adsMoGoInterstitialAdDidDismiss{
    if (interstitial) {
        [interstitial interstitialCancel];
    }
}

/*
Full-screen browser display advertising
 */
- (void)adsMoGoWillPresentInterstitialAdModal{

}

/*
Full screen AD browser disappeared
 */
- (void)adsMoGoDidDismissInterstitialAdModal{
    
}

/*
  ads close
 */
- (void)adsMogoInterstitialAdClosed{

}




#pragma AdMoGoWebBrowserControllerUserDelegate

/*
 The browser will display
 */
- (void)webBrowserWillAppear{

}
/*
 Browser has shown
 */
- (void)webBrowserDidAppear{
    
}

/*
 The browser will be shut down
 */
- (void)webBrowserWillClosed{
    
}

/*
 The browser is closed
 */
- (void)webBrowserDidClosed{
    
}

/*
 The browser share
 url Browser to open the url
 */
- (void)webBrowserShare:(NSString *)url{
    
}


@end
AdsMogoInterstitial * AdsMogoInterstitial ::sharedInterstitial(){
    static AdsMogoInterstitial * banner=NULL;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken,^{
        banner= new AdsMogoInterstitial;
    });
    return banner;
}
void AdsMogoInterstitial::loadInterstitial(char* mogoid,AdsMogoInterstitialType type ,bool ismanualrefresh,bool isvideo){
    NSString *mogo_id = [[NSString alloc] initWithCString:mogoid
                                                 encoding:NSASCIIStringEncoding];
    // You can set the debug mode
//    [[AdMoGoLogCenter shareInstance] setLogLeveFlag:AdMoGoLogDebug];
    //Set the advertising types
    AdViewType mogotype=(AdViewType)type;
      //For advertising
    
    InterstitialObj  * inter_obj= [InterstitialObj sharedInterstitialObj];
    [inter_obj createInterstitial:mogo_id
                            andInterType:mogotype isManualRefresh:ismanualrefresh isvideo:isvideo];
}
void AdsMogoInterstitial::showInterstitial() {
    //You need to call this method display ads
     [[InterstitialObj sharedInterstitialObj]showInterstitial];
}
void AdsMogoInterstitial::hideInterstitial(){
    //You need to call this method to release advertisement
    [[InterstitialObj sharedInterstitialObj]hideInterstitial];
}
void AdsMogoInterstitial::refreshAd(){
    [[InterstitialObj sharedInterstitialObj]refreshAd];
}
