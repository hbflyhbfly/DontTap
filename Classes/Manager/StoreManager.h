//
//  StoreManager.h
//  MiniWar
//
//  Created by Tianlin Yuan on 7/21/13.
//  Copyright (c) 2013 Wiselinc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@interface StoreManager : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>

@property(nonatomic,retain)NSArray *iapItems;
@property(nonatomic,retain)NSString *orderId;
// Patch added 2014-09-22 ensure game will not restart after purchase with iTunes
@property(nonatomic, assign)BOOL inProgress, inOrder, ignorePause;


+ (StoreManager *) sharedInstance;

- (void) startIAPPaymentProcess;

- (void) startIAPRestoreProcess;

- (void) startIapInfoProcess;

- (bool) iapReady;

@end
