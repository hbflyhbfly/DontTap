//
//  MainScene.cpp
//  DontTap
//
//  Created by Syuuhi on 4/30/16.
//
//
#include "UI_Main.hpp"
#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "GameScene.hpp"
#include "GameController.hpp"
#include "ad_function.h"

#include "UIManage.hpp"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

UI_Main::UI_Main():
_uiNode(nullptr)
{
    
}

UI_Main::~UI_Main()
{
    
}

bool UI_Main::init(){
    if (!Layer::init()) {
        return false;
    }
    
    
    _uiNode = CSLoader::getInstance()->createNode("ui/MainScene.csb");
    this->addChild(_uiNode);

    for (int i = 1; i<=9; i++) {
        Layout* startButton = dynamic_cast<Layout*>(_uiNode->getChildByTag(i));
        GameController* gameController = GameController::getInstance();
        auto callback = [this,gameController](Ref* ref){
            auto lay = dynamic_cast<Layout*>(ref);
            int tag = lay->getTag();
            if (tag == 8) {//more
                
            }else if(tag == 9){//songs
                UIManage::getInstance()->showUI(UI_SONGS_, true);

            }else if(tag == 7){
                gameController->startGame(GameController::getInstance()->getRandomGame(),true);
            }else{
                GAME_TYPE gameType = (GAME_TYPE)tag;
                rapidjson::Value group(rapidjson::kArrayType);
                gameController->getGroupWithType(gameType, group);
                rapidjson::Value& data = group[0];
                
                gameController->startGame(data["id"].GetString(),true);
            }
            GameController::getInstance()->playSoundForClick();
        };
        startButton->addClickEventListener(callback);

    }
    return true;
}

void UI_Main::onEnter(){
    Layer::onEnter();  
    
//    GameController::getInstance()->updateLanguage(_uiNode);

//    updateUI();

    ad_function::instance()->hideBanner();
}

void UI_Main::updateUI(){
    auto n = _uiNode->getChildByName("Panel_5");
    auto token = dynamic_cast<Text*>(n->getChildByName("token_text"));
    int count = GameController::getInstance()->getUserData("token", DATA_INT).GetInt();
    token->setString(StringUtils::format("%d",count));
}

void UI_Main::closeUI(){
    this->removeFromParentAndCleanup(false);
}

void UI_Main::showUI(){
    
}