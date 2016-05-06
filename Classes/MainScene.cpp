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
    Node* n = CSLoader::getInstance()->createNode("ui/eff_gates_0002.csb");
    
    timeline::ActionTimeline* timeline1 = CSLoader::getInstance()->createTimeline("ui/eff_gates_0002.csb");
    timeline1->play("eff_gates_0002_idle", true);
    n->runAction(timeline1);
    
    n->setPosition(Vec2(300,500));
    _uiNode->addChild(n);
//    Armature* arm = dynamic_cast<Armature*>(_uiNode->getChildByName("ArmatureNode_1"));
//    arm->getAnimation()->play("eff_gates_0002_idle");
    timeline::ActionTimeline* timeline = CSLoader::getInstance()->createTimeline("ui/MainScene.csb");
    timeline->play("idel", true);
    _uiNode->runAction(timeline);
    
    Button* startButton = dynamic_cast<Button*>(_uiNode->getChildByName("start_button"));
    auto callback = [this](Ref* ref){
        auto gameScene = GameScene::createScene();
        Director::getInstance()->replaceScene(gameScene);
    };
    startButton->addClickEventListener(callback);
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
    this->addChild(_uiNode);
}