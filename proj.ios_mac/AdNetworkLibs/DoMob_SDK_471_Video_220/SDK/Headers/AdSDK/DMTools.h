//
//  DMTools.h
//
//  Copyright (c) All rights reserved.
//

#import <Foundation/Foundation.h>

@interface DMTools : NSObject

// init user tool
- (id)initWithPublisherId:(NSString *)publisherId;

// check rate information
- (void)checkRateInfo;

@end