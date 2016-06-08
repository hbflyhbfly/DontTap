//
//  AdsMogoConstant.cpp
//  MyCppGame
//
//  Created by Castiel Chen on 15/5/13.
//
//

#import  "AdsMOGOContent.h"


#include "AdsMogoConstant.h"



void AdsMogoConstant::setTitle(char* title){
  NSString *title_str = [[NSString alloc] initWithCString:title
                                                    encoding:NSASCIIStringEncoding];
    [[AdsMOGOContent shareSingleton]setUrl:title_str];
     [title_str release];
}
void  AdsMogoConstant::setUrl(char* url){
  NSString *url_str = [[NSString alloc] initWithCString:url
                                                    encoding:NSASCIIStringEncoding];
    [[AdsMOGOContent shareSingleton]setUrl:url_str];
    [url_str release];
}
void  AdsMogoConstant::setCat(CatType2dx catType, ...){
    NSMutableString * catStr =[[NSMutableString alloc]init];
    va_list arguments;
    int eachObject;
    if (catType) {
        [catStr appendFormat:@"%d,",catType];
        va_start(arguments, catType);
        while ((eachObject = va_arg(arguments, int))) {
            [catStr appendFormat:@"%d,",eachObject];
        }
        va_end(arguments);
    }
    NSLog(@"%@",catStr);
  [[AdsMOGOContent shareSingleton] setCatTypeCocos2dx:[[catStr substringToIndex:([catStr length]-1)] copy]];
}
void  AdsMogoConstant::setKeywords(char * keyworkd, ...){
  NSString *keyworkd_str = [[NSString alloc] initWithCString:keyworkd
                                               encoding:NSASCIIStringEncoding];
    NSMutableString * catStr =[[NSMutableString alloc]init];
    va_list arguments;
    id eachObject;
    if (keyworkd_str) {
        [catStr appendFormat:@"%@,",keyworkd_str];
        [keyworkd_str release];
        va_start(arguments, keyworkd);
        while ((eachObject = va_arg(arguments, id))) {
            NSString *eachObject_str = [[NSString alloc] initWithCString:(char *)eachObject
                                                              encoding:NSASCIIStringEncoding];
            [catStr appendFormat:@"%@,",eachObject_str];
            [eachObject_str release];
        }
        va_end(arguments);
    }
    NSLog(@"%@",catStr);
    [[AdsMOGOContent shareSingleton]setkeyWordsCocos2dx:[[catStr substringToIndex:([catStr length]-1)] copy]];
}


