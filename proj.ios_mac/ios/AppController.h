#import <UIKit/UIKit.h>
#import <HeyzapAds/HeyzapAds.h>


//@import AVFoundation;
//@import AdSupport;
//@import AudioToolbox;
//@import CFNetwork;
//@import CoreData;
//@import CoreGraphics;
//@import CoreLocation;
//@import CoreMedia;
//@import CoreTelephony;
//@import EventKit;
//@import EventKitUI;
//@import Foundation;
//@import MediaPlayer;
//@import MessageUI;
//@import MobileCoreServices;
//@import QuartzCore;
//@import SafariServices;
//@import Security;
//@import Social;
//@import StoreKit;
//@import SystemConfiguration;
//@import UIKit;
//@import WebKit;
//@import iAd;

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;

@end

