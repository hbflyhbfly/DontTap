//
//  MainScene.cpp
//  DontTap
//
//  Created by Syuuhi on 4/30/16.
//
//
#include "UI_Dialog.hpp"
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

UI_Dialog::UI_Dialog():
_uiNode(nullptr),
_str("")
{
    
}

UI_Dialog::~UI_Dialog()
{
//    _action->release();
}

bool UI_Dialog::init(){
    if (!Layer::init()) {
        return false;
    }
    
    
    _uiNode = CSLoader::getInstance()->createNode("ui/Dialog.csb");
    _action = CSLoader::getInstance()->createTimeline("ui/Dialog.csb");
//    _action->retain();
    _uiNode->runAction(_action);
    
    this->addChild(_uiNode);
    
    
    return true;
}

void UI_Dialog::onEnter(){
    Layer::onEnter();  
    
//    GameController::getInstance()->updateLanguage(_uiNode);

//    updateUI();

//    ad_function::instance()->hideBanner();
}

void UI_Dialog::updateUI(){
    auto text = dynamic_cast<Text*>(_uiNode->getChildByName("Panel")->getChildByName("text"));
    text->setString(_str);
}

void UI_Dialog::showUI(){
    _action->setLastFrameCallFunc(CC_CALLBACK_0(UI_Dialog::showCallback, this));
    _action->play("show", false);
}
void UI_Dialog::closeUI(){
    _action->setLastFrameCallFunc(CC_CALLBACK_0(UI_Dialog::closeCallback, this));
    _action->play("close", false);
}

void UI_Dialog::showCallback(){
    _action->clearLastFrameCallFunc();
    if (_type == DIALOG_AD_REWARD_) {
        closeUI();
    }
}

void UI_Dialog::closeCallback(){
    this->removeFromParentAndCleanup(false);
}