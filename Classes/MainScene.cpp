//
//  MainScene.cpp
//  DontTap
//
//  Created by Syuuhi on 4/30/16.
//
//
#include "MainScene.hpp"
#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "GameScene.hpp"
#include "GameController.hpp"
USING_NS_CC;
using namespace cocostudio;
using namespace ui;

MainScene::MainScene():
_uiNode(nullptr)
{
    
}

MainScene::~MainScene()
{
    
}

bool MainScene::init(){
    if (!Layer::init()) {
        return false;
    }
    _uiNode = CSLoader::getInstance()->createNode("ui/MainScene.csb");
    
    timeline::ActionTimeline* timeline = CSLoader::getInstance()->createTimeline("ui/MainScene.csb");
    timeline->play("idel", true);
    _uiNode->runAction(timeline);
//    GameController::getInstance()->getGameData("Classic_25");

    for (int i = 1; i<=9; i++) {
        Layout* startButton = dynamic_cast<Layout*>(_uiNode->getChildByTag(i));
        GameController* gameController = GameController::getInstance();
        auto callback = [this,gameController](Ref* ref){
            auto lay = dynamic_cast<Layout*>(ref);
            int tag = lay->getTag();
            if (tag == 8) {//more
                
            }else if(tag == 9){//songs
                
            }else{
                GAME_TYPE gameType = (GAME_TYPE)tag;
                rapidjson::Value group(rapidjson::kArrayType);
                gameController->getGroupWithType(gameType, group);
                rapidjson::Value& data = group[0];
                
                gameController->startGame(data["id"].GetString(),true);
            }
            int n = (rand()%20)+1;
            GameController::getInstance()->playSoundEffect(StringUtils::format("%d.mp3",n),true);
        };
        startButton->addClickEventListener(callback);

    }
    return true;
}

Scene* MainScene::createScene(){
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}

void MainScene::onEnter(){
    Layer::onEnter();
//    Sprite* s =Sprite::create("res/button_startCN1.png");
//    s->setPosition(300,500);
//    
//    auto action = MoveTo::create(2, Vec2(300,800));
//    s->runAction(EaseBackOut::create(action));
//    this->addChild(s);
    this->addChild(_uiNode);
    
    updateUI();

}

void MainScene::updateUI(){
    auto n = _uiNode->getChildByName("Panel_5");
    auto token = dynamic_cast<Text*>(n->getChildByName("token_text"));
    int count = GameController::getInstance()->getUserData("token", DATA_INT).GetInt();
    token->setString(StringUtils::format("%d",count));
}