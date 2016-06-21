//
//  MainScene.hpp
//  DontTap
//
//  Created by Syuuhi on 4/30/16.
//
//

#ifndef UI_Dialog_hpp
#define UI_Dialog_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "UIManage.hpp"

class UI_Dialog:public cocos2d::Layer{

public:
    UI_Dialog();
    virtual ~UI_Dialog();
    virtual bool init() override;
    virtual void onEnter() override;
    CREATE_FUNC(UI_Dialog);
    void updateUI();
    void closeUI();
    void showUI(DIALOG_TYPE type);
    void showCallback();
    void closeCallback();
    void updateUI(const std::string& str);
    void setStr(const std::string& str,DIALOG_TYPE type );
private:
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

private:
    DIALOG_TYPE _type;
    std::string _str;
    cocos2d::Node* _uiNode;
    cocostudio::timeline::ActionTimeline* _action;

    
};
#endif /* UI_Main_hpp */
