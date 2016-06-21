//
//  GameController.hpp
//  DontTap
//
//  Created by Syuuhi on 5/7/16.
//
//

#ifndef UIManage_hpp
#define UIManage_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

typedef enum {
    UI_NONE_ = -1,
    UI_SONGS_,
    UI_ABOUT_,
    UI_MAIN_,
    UI_DIALOG_,
    UI_MAX,
} GAME_UI;

typedef enum {
    DIALOG_AD_REWARD_,
    DIALOG_SIGN_REWARD_,
    DIALOG_AD_WATCH_
} DIALOG_TYPE;

class UIManage:public cocos2d::Ref{
public:
    UIManage();
    virtual ~UIManage();
    static UIManage* getInstance();
    void showDialog(DIALOG_TYPE type,const std::string& str);
    void showUI(GAME_UI uiType,bool isAdd);
    void showUIToScene(GAME_UI uiType,Scene* scene,bool isAdd);

    void closeUI(GAME_UI uiType);
    virtual bool init();
    
    bool createUI(GAME_UI uiType);
    void updateUI(GAME_UI uiType);
    
private:
    std::map<GAME_UI,cocos2d::Node*> _uiMap;
};
#endif /* UIManage_hpp */


