//
//  MainScene.cpp
//  DontTap
//
//  Created by Syuuhi on 4/30/16.
//
//
#include "UI_Songs.hpp"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"
#include "ad_function.h"
#include "UIManage.hpp"
#include "GameController.hpp"
#include "json/document.h"


UI_Songs::UI_Songs():
_uiNode(nullptr)
{
    
}

UI_Songs::~UI_Songs()
{
    
}

bool UI_Songs::init(){
    if (!Layer::init()) {
        return false;
    }
    
    _uiNode = CSLoader::getInstance()->createNode("ui/Songs.csb");
    this->addChild(_uiNode);
    Button* videoBtn = dynamic_cast<Button*>(_uiNode->getChildByName("watch_video_btn"));
    Button* closeBtn = dynamic_cast<Button*>(_uiNode->getChildByName("close_btn"));
    Button* playModeBtn = dynamic_cast<Button*>(_uiNode->getChildByName("play_mode_btn"));
    
    videoBtn->addTouchEventListener(CC_CALLBACK_2(UI_Songs::touchEvent, this));
    closeBtn->addTouchEventListener(CC_CALLBACK_2(UI_Songs::touchEvent, this));
    playModeBtn->addTouchEventListener(CC_CALLBACK_2(UI_Songs::touchEvent, this));
    
    auto listPanel = dynamic_cast<cocos2d::ui::Layout*>(_uiNode->getChildByName("list_panel"));
    auto tableView = dynamic_cast<TableView*>(listPanel->getChildByTag(1));
    if (!tableView) {
        tableView = TableView::create(this,listPanel->getContentSize());
        tableView->setDelegate(this);
        tableView->setDirection(ScrollView::Direction::VERTICAL);
        tableView->setBounceable(false);
        tableView->setTag(1);
        tableView->setPosition(Vec2::ZERO);
        listPanel->addChild(tableView);
    }
    tableView->reloadData();
    
    return true;
}

void UI_Songs::touchEvent(Ref *pSender, Widget::TouchEventType type){
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
            if (name == "watch_video_btn") {
                ad_function::instance()->incentivized();
            }else if(name == "close_btn"){
                UIManage::getInstance()->closeUI(UI_SONGS_);
            }else if(name == "play_mode_btn"){
                GameController::getInstance()->setPlayMode(!GameController::getInstance()->getPlayMode());
                updateUI();
            }
        }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

void UI_Songs::onEnter(){
    Layer::onEnter();
//    Sprite* s =Sprite::create("res/button_startCN1.png");
//    s->setPosition(300,500);
//    
//    auto action = MoveTo::create(2, Vec2(300,800));
//    s->runAction(EaseBackOut::create(action));
//    this->addChild(s);
    

//    updateUI();

}

void UI_Songs::updateUI(){
    
    Button* playMode = dynamic_cast<Button*>(_uiNode->getChildByName("play_mode_btn"));
    
    if (GameController::getInstance()->getPlayMode()) {
        playMode->loadTextures("res/textures/repeat.png", "res/textures/repeat.png","",cocos2d::ui::Button::TextureResType::PLIST);
    }else{
        playMode->loadTextures("res/textures/shuffle.png", "res/textures/shuffle.png","",cocos2d::ui::Button::TextureResType::PLIST);
    }
//    auto token = dynamic_cast<Text*>(n->getChildByName("token_text"));
//    int count = GameController::getInstance()->getUserData("token", DATA_INT).GetInt();
//    token->setString(StringUtils::format("%d",count));
}
void UI_Songs::closeUI(){
    this->removeFromParentAndCleanup(false);
}

void UI_Songs::showUI(){
    
}
//tablevie delegate
void UI_Songs::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
    rapidjson::Value music;
    GameController::getInstance()->getMusicData((int)cell->getIdx(), music);
    if(GameController::getInstance()->haveBuy(music["id"].GetString())){
        GameController::getInstance()->selectMusic(music["id"].GetString());
    }else{
        GameController::getInstance()->buyMusic(music["id"].GetString());
    }
    table->updateCellAtIndex(cell->getIdx());
}

Size UI_Songs::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(750, 100);
}

TableViewCell* UI_Songs::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto string = StringUtils::format("%ld", idx);
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        
        cell = TableViewCell::create();
        setTableCell(idx,*cell,true);
    }
    else
    {
        setTableCell(idx,*cell);
    }
    
    return cell;
}

void UI_Songs::setTableCell(ssize_t idx,TableViewCell& cell,bool isAdd){
    if (isAdd) {
        auto content = CSLoader::getInstance()->createNode("ui/SongCell.csb");
        
        content->setAnchorPoint(Vec2::ZERO);
        content->setPosition(Vec2(0, 0));
        content->setTag(1);
        cell.addChild(content);
    }
    ImageView* checkImage = dynamic_cast<ImageView*>(cell.getChildByTag(1)->getChildByName("check_image"));

    Text* name = dynamic_cast<Text*>(cell.getChildByTag(1)->getChildByName("song_name"));
    ImageView* costBtn =dynamic_cast<ImageView*>(cell.getChildByTag(1)->getChildByName("buy_song_btn"));
    Text* cost = dynamic_cast<Text*>(costBtn->getChildByName("cost_text"));

    rapidjson::Value music;
    GameController::getInstance()->getMusicData((int)idx, music);
    name->setString(music["tid"].GetString());

    int costNum = music["price"].GetInt();
    if(costNum>0){
        if(GameController::getInstance()->haveBuy(music["id"].GetString())){
            costBtn->setVisible(false);
            checkImage->setVisible(true);
            checkImage->loadTexture("res/textures/unlock_song.png",cocos2d::ui::Button::TextureResType::PLIST);
        }else{
            costBtn->setVisible(true);
            checkImage->setVisible(false);
        }
    }else{
        costBtn->setVisible(false);
        checkImage->setVisible(true);
        checkImage->loadTexture("res/textures/check.png",cocos2d::ui::Button::TextureResType::PLIST);
    }
    if(checkImage->isVisible()){
        checkImage->setVisible(GameController::getInstance()->isSelectedMusic(music["id"].GetString()));
    }
    
    cost->setString(StringUtils::format("%d",costNum));
    //    auto label = dynamic_cast<Label*>(cell.getChildByTag(1));
    
}

ssize_t UI_Songs::numberOfCellsInTableView(TableView *table)
{
    ;
    return GameController::getInstance()->getMusicCount();
}