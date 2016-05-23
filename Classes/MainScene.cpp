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
#include "GameScene.hpp"
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
    
    for (int i = 1; i<=7; i++) {
        std::string btnName = StringUtils::format("Panel_%d",i);
        Layout* startButton = dynamic_cast<Layout*>(_uiNode->getChildByName(btnName));
        
        auto callback = [this](Ref* ref){
            auto gameScene = GameScene::createScene();
            Director::getInstance()->replaceScene(gameScene);
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
    
}