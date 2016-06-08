//
//  StoreManager.m
//
//  Created by Jason Y Liu on 6/3/16.
//

#import "StoreManager.h"
#import "GameConst.h"

@implementation StoreManager

#pragma mark -
#pragma mark Public Method IAP

static StoreManager* sharedInstance = nil;

+ (StoreManager *) sharedInstance
{
    @synchronized(self){
        if (sharedInstance == nil){
            sharedInstance = [[self alloc] init];
        }
    }
    return sharedInstance;
    
}

- (void) startIapInfoProcess
{
    if(self.iapItems)
        return;
    NSMutableArray *iap_keys = [[NSMutableArray alloc] init];
    
    [iap_keys addObject:App_IAP];
    
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    
    
    NSSet *idSet = [[NSSet alloc] initWithArray:iap_keys];
    NSLog(@"Apple Store: %@", idSet);
    SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers: idSet];
    request.delegate = self;
    [request start];
    
    
    
    [iap_keys release];
    [idSet release];
    [request release];
}

- (void) startIAPRestoreProcess{
    self.inProgress = YES;
    self.inOrder = YES;
    
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

- (void) startIAPPaymentProcess{
    self.inProgress = YES;
    self.inOrder = YES;
    //    NSDictionary *params = [NSDictionary dictionaryWithObjectsAndKeys:
    //                            pName, @"pid",
    //                            getUUID(NO), PARAM_UDID,
    //                            nil];
    //    [[GameManager sharedGameManager].context.api  send:@"api_account_presale" params:params block:^(id response)
    //     {
    //
    //
    //         self.orderId = [response objectForKey:@"order_id"];
    //         if (self.orderId)
    //         {
    
    // 测试充值代码正常打包注释掉！
    //             NSString *jsonObjectString = @"aaa";
    //
    //             NSDictionary *params = [NSDictionary dictionaryWithObjectsAndKeys:
    //                                     _orderId, @"order_id",
    //                                     jsonObjectString, @"verification_data",
    //                                     nil];
    //             [[GameManager sharedGameManager].context.api  send:@"api_account_credit" params:params block:^(id response)
    //              {
    //                  [UserContext currentContext].usersave.credit_times +=1;
    //                  if ([UserContext currentContext].usersave.credit_times==1) {
    //                      [UserContext currentContext].usersave.first_purchase_time = [DataManager nowTime];
    //                  }
    //                  [GameManager sharedGameManager].context.usersave.cash = [[response objectForKey:@"cash"] intValue];
    //              }];
    //             return;
    // 测试充值代码正常打包注释掉！
    
    
    //             NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    //             [userDefaults setObject:self.orderId forKey:USERDEFAULT_KEY(ORDER_ID)];
    //
    //             if ([self.iapItems count] > 0) {
    for (SKProduct *pData in self.iapItems) {
        if ([pData.productIdentifier isEqualToString:App_IAP])
        {
            SKPayment *payment = [SKPayment paymentWithProduct:pData];
            [[SKPaymentQueue defaultQueue] addPayment:payment];
        }
    }
    
    //             }
    //             else{
    //
    //             }
    //         }
    //     }];
}

- (bool) iapReady{
    return self.iapItems != nil;
}

#pragma mark -
#pragma mark Private Method
- (void) final
{
    
    //    [[WLUIManager sharedWLUIManager] refresh];
    //    if([[UIContext currentContext].uiDictionary objectForKey:NSStringFromClass([ShopPopupView class])]){
    //        ShopPopupView *shop = [[UIContext currentContext].uiDictionary objectForKey:NSStringFromClass([ShopPopupView class])];
    //        [shop close];
    //    }
}

-(void) failedTransaction:(SKPaymentTransaction *)transaction
{
    self.inOrder = NO;
    self.inProgress = NO;
    //    if([GameManager sharedGameManager].pauseStart == 0){
    //
    //    }
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:App_IAP_Fail_Title
                                                    message:App_IAP_Fail_Msg
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}


-(void) completeTransaction:(SKPaymentTransaction *)transaction
{
    self.inProgress = NO;
    self.inOrder = NO;
    
    //    NSString *jsonObjectString = [Func encode:(uint8_t *)transaction.transactionReceipt.bytes
    //                                       length:transaction.transactionReceipt.length];
    // Process remove ads
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
    
    //    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:App_IAP_Success_Title
    //                                                    message:App_IAP_Success_Msg
    //                                                   delegate:self
    //                                          cancelButtonTitle:@"OK"
    //                                          otherButtonTitles:nil, nil];
    //    [alert show];
    //    [alert release];
}

- (void) restoreTransaction:(SKPaymentTransaction *)transaction
{
    self.inProgress = NO;
    self.inOrder = NO;
    
    if([transaction.payment.productIdentifier isEqualToString:App_IAP]){

    }
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}


#pragma mark -
#pragma mark SK delegate Method
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSArray *pList = [NSArray arrayWithArray:response.products];
    NSLog(@"SKRequestSuccess: %@", pList);
    self.iapItems = pList;
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    //self.inProgress = NO;
    NSLog(@"SKRequestError: %@", [error localizedDescription]);
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
            default:
                break;
        }
    }
}

- (void) dealloc
{
    self.iapItems = nil;
    [super dealloc];
}

@end

