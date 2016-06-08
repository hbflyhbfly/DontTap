//
//  Wrapper.m
//  Unity-iPhone
//
//  Created by Jason Y Liu on 5/23/16.
//
//

#import <Foundation/Foundation.h>
#import "AdManager.h"
#import "StoreManager.h"
#import "GameConst.h"

extern void wrapper_test (NSString *message){}


extern void wrapper_showBanner (){
    [[AdManager sharedInstance] showBanner];
}


extern void wrapper_hideBanner (){
    [[AdManager sharedInstance] hideBanner];
}


extern void wrapper_showInterstitial (){
    [[AdManager sharedInstance] showInterstitial];
}


extern bool wrapper_isVideoReady (){
//    return [[UnityAds sharedInstance] canShow];
    return true;

}


extern void wrapper_showVideo (){
    [[AdManager sharedInstance] showVideo];
}

extern void wrapper_rate(){
    SKStoreProductViewController *controller = [[SKStoreProductViewController alloc] init];
    
    NSDictionary *param = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:App_ID], SKStoreProductParameterITunesItemIdentifier, nil];
    
    [controller loadProductWithParameters:param completionBlock:nil];
    
    [controller release];
    
    [UnityGetGLViewController() presentViewController:controller animated:YES completion:nil];
}

extern bool wrapper_iapReady(){
    return [[StoreManager sharedInstance] iapReady];
}

extern void wrapper_buyNoAds(){
    [[StoreManager sharedInstance] startIapInfoProcess];
}