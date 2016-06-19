//
//  DMAdView+AdsMogo.h
//   AdSDK
//
//  Copyright (c) All rights reserved.
//

@interface DMAdView (AdsMogo)

//开始请求Domob广告
- (void)dbAdLoad;

//聚合认为展示了Domob的广告则调用此方法
- (void)dbAdImpression;

//聚合认为Domob的广告结束了则调用此方法
- (void)dbAdDismiss;

@end
