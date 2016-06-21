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
    
    
    Button* videoBtn = dynamic_cast<Button*>(_uiNode->getChildByName("Panel")->getChildByName("ok_btn"));
    Button* closeBtn = dynamic_cast<Button*>(_uiNode->getChildByName("Panel")->getChildByName("close_btn"));
    
    videoBtn->addTouchEventListener(CC_CALLBACK_2(UI_Dialog::touchEvent, this));
    closeBtn->addTouchEventListener(CC_CALLBACK_2(UI_Dialog::touchEvent, this));
    
    this->addChild(_uiNode);
    
    
    return true;
}

void UI_Dialog::touchEvent(Ref *pSender, Widget::TouchEventType type){
    Button* btn = dynamic_cast<Button*>(pSender);
    const std::string name = btn->getName();
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
            break;
            
        case Widget::TouchEventType::MOVED:
            break;
            
        case Widget::TouchEventType::ENDED:
        {
            if (name == "ok_btn") {
                if(_type == DIALOG_AD_WATCH_){
                    ad_function::instance()->incentivized();
                }
                UIManage::getInstance()->closeUI(UI_DIALOG_);
            }else if(name == "close_btn"){
                UIManage::getInstance()->closeUI(UI_DIALOG_);
            }
            GameController::getInstance()->playSoundForClick();
        }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

void UI_Dialog::onEnter(){
    Layer::onEnter();  
    
//    GameController::getInstance()->updateLanguage(_uiNode);

//    updateUI();

//    ad_function::instance()->hideBanner();
}

void UI_Dialog::setStr(const std::string& str,DIALOG_TYPE type ){
    _str = str;
    
    
};
void UI_Dialog::updateUI(){
    auto text = dynamic_cast<Text*>(_uiNode->getChildByName("Panel")->getChildByName("text"));
    text->setTextAreaSize(Size(350, 90));
    text->setString(_str);
    
}

void UI_Dialog::showUI(DIALOG_TYPE type){
    _type = type;
    _action->setLastFrameCallFunc(CC_CALLBACK_0(UI_Dialog::showCallback, this));
    if (_type == DIALOG_AD_REWARD_) {
        _action->play("show_reward", false);
    }else if (_type == DIALOG_AD_WATCH_){
        _action->play("show_dialog", false);
    }
    
}
void UI_Dialog::closeUI(){
    _action->setLastFrameCallFunc(CC_CALLBACK_0(UI_Dialog::closeCallback, this));
    
    if (_type == DIALOG_AD_REWARD_) {
        _action->play("close_reward", false);
    }else if (_type == DIALOG_AD_WATCH_){
        _action->play("close_dialog", false);
    }
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