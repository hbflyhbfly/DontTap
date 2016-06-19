//
//  DMSplashAdController+AdsMogo.h
//   AdSDK
//
//  Copyright (c) All rights reserved.
//

#import "DMSplashAdController.h"

@interface DMSplashAdController (AdsMogo)

//开始请求Domob广告
- (void)dbSplashAdLoad;

//聚合认为展示了Domob的广告则调用此方法
- (void)dbSplashAdImpression;

//聚合认为Domob的广告结束了则调用此方法
- (void)dbSplashAdDismiss;

@end
