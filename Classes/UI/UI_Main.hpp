//
//  MainScene.hpp
//  DontTap
//
//  Created by Syuuhi on 4/30/16.
//
//

#ifndef UI_Main_hpp
#define UI_Main_hpp

#include <stdio.h>
#include "cocos2d.h"

class UI_Main:public cocos2d::Layer{

public:
    UI_Main();
    virtual ~UI_Main();
    virtual bool init() override;
    virtual void onEnter() override;
    CREATE_FUNC(UI_Main);
    void updateUI();
    void closeUI();
    void showUI();
private:
    
private:
    cocos2d::Node* _uiNode;
};
#endif /* UI_Main_hpp */
