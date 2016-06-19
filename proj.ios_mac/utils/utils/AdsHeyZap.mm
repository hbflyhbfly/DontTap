//
//  AdsHeyZap.m
//  DontTap
//
//  Created by Syuuhi on 6/14/16.
//
//

#import "AdsHeyZap.h"

#define LOG_METHOD_NAME_TO_CONSOLE_WITH_STRING(str) [self logToConsole:[NSString stringWithFormat:@"%@ %@", NSStringFromSelector(_cmd), str]]

@implementation AdsHeyZap

+ (AdsHeyZap *)sharedInstance {
    static AdsHeyZap *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [self new];
    });
    return instance;
}

-(void)showIncentivizedAd(){
    [HZIncentivizedAd show];
}


#pragma mark - Callbacks
- (void)didCompleteAdWithTag:(NSString *)tag {
    LOG_METHOD_NAME_TO_CONSOLE_WITH_STRING(tag);
}

-(void)dealloc{
    [super dealloc];
}
@end
