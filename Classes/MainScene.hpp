//
//  MainScene.hpp
//  DontTap
//
//  Created by Syuuhi on 4/30/16.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include <stdio.h>
#include "cocos2d.h"

class MainScene:public cocos2d::Layer{

public:
    MainScene();
    virtual ~MainScene();
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void onEnter() override;
    CREATE_FUNC(MainScene);
private:
    void updateUI();
private:
    cocos2d::Node* _uiNode;
};
#endif /* MainScene_hpp */
