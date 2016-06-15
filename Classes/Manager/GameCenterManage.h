//
//  GameCenterManage.h
//  DontTap
//
//  Created by Syuuhi on 6/15/16.
//
//

#import <Foundation/Foundation.h>
#import <GameKit/GKLocalPlayer.h>

@interface GameCenterManage : NSObject

+ (GameCenterManage *) sharedInstance;
- (void) authenticateLocalPlayer;
- (BOOL) isGameCenterAvailable;

@end
