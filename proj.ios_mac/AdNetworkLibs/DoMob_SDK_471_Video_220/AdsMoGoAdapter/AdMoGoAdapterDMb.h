//
//  AdMoGoAdapterDMb.h
//  TestMOGOSDKAPP
//
//  
//
//

#import "AdMoGoAdNetworkAdapter.h"
#import "DMAdView.h"
#import "AdMoGoConfigData.h"

@interface AdMoGoAdapterDMb : AdMoGoAdNetworkAdapter<DMAdViewDelegate>{
    DMAdView *dmAdView;
    AdMoGoConfigData *configData;
    BOOL isStop;
    NSTimer *timer;
    BOOL isStopTimer;
}
+ (AdMoGoAdNetworkType)networkType;
//+ (NSDictionary *)networkType;
@end
