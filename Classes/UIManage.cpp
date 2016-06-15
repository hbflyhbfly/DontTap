//
//  GameController.cpp
//  DontTap
//
//  Created by Syuuhi on 5/7/16.
//
//
#include "UIManage.hpp"
#include "ui/UIText.h"
#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"

#include "UI/UI_Songs.hpp"
#include "UI/UI_Main.hpp"
#include "UI/UI_Dialog.hpp"

#include "GameController.hpp"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocostudio;
using namespace ui;

UIManage* s_uiManager = nullptr;

UIManage::UIManage()
{
    
}

UIManage::~UIManage(){
    for (auto ite:_uiMap) {
        ite.second->release();
    }
    _uiMap.clear();
}

UIManage* UIManage::getInstance(){
    if (s_uiManager == nullptr) {
        s_uiManager = new UIManage;
        s_uiManager->init();
    }
    return s_uiManager;
}

bool UIManage::init(){
    bool ret = false;
    do {
        
        ret = true;
    } while (0);
    return true;
}

void UIManage::showDialog(DIALOG_TYPE type,const std::string& str){
    showUI(UI_DIALOG_, true);
    dynamic_cast<UI_Dialog*>(_uiMap.find(UI_DIALOG_)->second)->setStr(str,type);
    updateUI(UI_DIALOG_);
}

void UIManage::showUI(GAME_UI uiType,bool isAdd){
    auto scene = Director::getInstance()->getRunningScene();
    if(_uiMap.find(uiType) == _uiMap.end()){
        createUI(uiType);
    }
    if(!isAdd){
        
        for (int ui = 1; ui < UI_MAX;ui = ui + 1)
        {
            if(ui != uiType){
                scene->removeChildByTag(ui,false);
            }
            
        }
//        scene->runAction(Sequence::create(FadeOut::create(0.5f),CallFunc::create([scene,uiType,this](){
//            scene->addChild(_uiMap.find(uiType)->second);
//            updateUI(uiType);
//        }),FadeIn::create(0.5f),NULL));
    }else{
        
    }
    if(!_uiMap.find(uiType)->second->getParent()){
        scene->addChild(_uiMap.find(uiType)->second);
    }
    
    
    if (uiType == UI_SONGS_) {
        auto ui = dynamic_cast<UI_Songs*>(_uiMap.find(uiType)->second);
        ui->showUI();
    }else if(uiType == UI_MAIN_){
        auto ui = dynamic_cast<UI_Main*>(_uiMap.find(uiType)->second);
        ui->showUI();
    }else if(uiType == UI_DIALOG_){
        auto ui = dynamic_cast<UI_Dialog*>(_uiMap.find(uiType)->second);
        ui->showUI();
    }
    updateUI(uiType);
    
}

void UIManage::showUIToScene(GAME_UI uiType,Scene* scene,bool isAdd){
    if(_uiMap.find(uiType) == _uiMap.end()){
        createUI(uiType);
    }
    if(!isAdd){
        for (int ui = 1; ui < UI_MAX;ui = ui + 1)
        {
            if(ui != uiType){
                scene->removeChildByTag(ui,false);
            }
        }
    }
    if(!_uiMap.find(uiType)->second->getParent()){
        scene->addChild(_uiMap.find(uiType)->second);
    }
    
    if (uiType == UI_SONGS_) {
        auto ui = dynamic_cast<UI_Songs*>(_uiMap.find(uiType)->second);
        ui->showUI();
    }else if(uiType == UI_MAIN_){
        auto ui = dynamic_cast<UI_Main*>(_uiMap.find(uiType)->second);
        ui->showUI();
    }else if(uiType == UI_DIALOG_){
        auto ui = dynamic_cast<UI_Dialog*>(_uiMap.find(uiType)->second);
        ui->showUI();
    }
    
    updateUI(uiType);
}
void UIManage::closeUI(GAME_UI uiType){
    
    bool isNeedMain = true;
    for(auto ite: _uiMap){
        if (ite.first == uiType) {
            if (ite.second->getParent()) {
                
                if (uiType == UI_SONGS_) {
                    auto ui = dynamic_cast<UI_Songs*>(_uiMap.find(uiType)->second);
                    ui->closeUI();
                }else if(uiType == UI_MAIN_){
                    auto ui = dynamic_cast<UI_Main*>(_uiMap.find(uiType)->second);
                    ui->closeUI();
                }else if(uiType == UI_DIALOG_){
                    auto ui = dynamic_cast<UI_Dialog*>(_uiMap.find(uiType)->second);
                    ui->closeUI();
                }
//                ite.second->removeFromParent();
            }
        }
        if(ite.second->getParent()){
            updateUI(ite.first);
            isNeedMain = false;
        }
    }
    if (isNeedMain) {
        showUI(UI_MAIN_, false);
    }
}

bool UIManage::createUI(GAME_UI uiType){
    Node* ui = nullptr;
    switch (uiType) {
        case UI_SONGS_:
            ui = UI_Songs::create();
            break;
        case UI_ABOUT_:
            ui = CSLoader::getInstance()->createNode("ui/About.csb");
            break;
        case UI_MAIN_:
            ui = UI_Main::create();
            break;
        case UI_DIALOG_:
            ui = UI_Dialog::create();
            break;
        default:
            break;
    }
    ui->setTag(uiType);
    ui->retain();
    _uiMap.insert(std::make_pair(uiType, ui));
    return true;
}

void UIManage::updateUI(GAME_UI uiType){
    if (uiType == UI_SONGS_) {
        auto ui = dynamic_cast<UI_Songs*>(_uiMap.find(uiType)->second);
        ui->updateUI();
    }else if(uiType == UI_MAIN_){
        auto ui = dynamic_cast<UI_Main*>(_uiMap.find(uiType)->second);
        ui->updateUI();
    }else if(uiType == UI_DIALOG_){
        auto ui = dynamic_cast<UI_Dialog*>(_uiMap.find(uiType)->second);
        ui->updateUI();
    }
    
    GameController::getInstance()->updateLanguage(_uiMap.find(uiType)->second);

}