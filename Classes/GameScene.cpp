//
//  GameScene.cpp
//  DontTap
//
//  Created by Syuuhi on 4/30/16.
//
//
#include<stdio.h>
#include<math.h>

#include "MainScene.hpp"
#include "GameScene.hpp"
#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "time.h"
#include "GameController.hpp"
#include "hy_function.h"
#include "ad_function.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocostudio;
using namespace ui;

#define BUFF_COUNT 3

GameScene::GameScene():
_uiNode(nullptr),
_gameTime(0.0f),
_isReallyStart(false),
_canTabBlockCount(0),
_tabedBlockCount(0),
_gameResult(0),
_speedBuf(0),
_specialIndex(10),
_timeBlock(3),
_speedChange(0),
_timeIndex(0),
_cellIndex(0),
_result(GAME_NONE),
_continueToken(100){
    
}

GameScene::~GameScene(){
    _groupData.Clear();
    _gameOverUINode->release();
}
bool GameScene::init(){
    if (!LayerColor::initWithColor(Color4B(133,213,98,255))) {
        return false;
    }
    _groupData.SetArray();
    GameController::getInstance()->getCurrentGroup(_groupData);
    //ui
    _uiNode = CSLoader::createNode("ui/GameScene.csb");
    //game over ui
    _gameOverUINode = CSLoader::createNode("ui/GameOver.csb");
    _gameOverUINode->retain();
    _gameOverAction = CSLoader::getInstance()->createTimeline("ui/GameOver.csb");

    _gameOverUINode->runAction(_gameOverAction);

    //tap error ui
    _gameOverDialogUINode = CSLoader::createNode("ui/GameOverDialog.csb");
    _gameOverDialogUINode->retain();
    _gameOverDialogAction = CSLoader::getInstance()->createTimeline("ui/GameOverDialog.csb");
    _gameOverDialogUINode->runAction(_gameOverDialogAction);

//    _gameOverAction->retain();
    //start label
    _startLabel = dynamic_cast<Node*>(_uiNode->getChildByName("start_node"));
    
    //progress
    _progress = dynamic_cast<LoadingBar*>(_uiNode->getChildByName("progress"));
    _targetLabel = dynamic_cast<TextBMFont*>(_uiNode->getChildByName("target_label"));

    //list
    auto listPanel = dynamic_cast<Layout*>(_uiNode->getChildByName("list_panel"));
    TableView* tableView = TableView::create(this,listPanel->getContentSize());
    tableView->setDelegate(this);
    tableView->setDirection(ScrollView::Direction::HORIZONTAL);
    tableView->setBounceable(false);
    tableView->setTag(1);
    tableView->setPosition(Vec2::ZERO);
    listPanel->addChild(tableView);
    
    //button
    auto shareButton = dynamic_cast<Button*>(_gameOverUINode->getChildByName("Panel")->getChildByName("share_btn"));
    auto backButton = dynamic_cast<Button*>(_gameOverUINode->getChildByName("Panel")->getChildByName("back_btn"));
    auto startAgainButton = dynamic_cast<Button*>(_gameOverUINode->getChildByName("Panel")->getChildByName("start_again_btn"));
    shareButton->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEvent, this));
    backButton->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEvent, this));
    startAgainButton->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEvent, this));
    
    
    auto overButton = dynamic_cast<Button*>(_gameOverDialogUINode->getChildByName("Panel")->getChildByName("over_btn"));
    auto continueButton = dynamic_cast<Button*>(_gameOverDialogUINode->getChildByName("Panel")->getChildByName("continue_btn"));
    overButton->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEvent, this));
    continueButton->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEvent, this));

    //game content
    _bgColor = Color4F::WHITE;
    _blockColor = GameController::getInstance()->randomColor();
    _blockLayer = Layer::create();
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    return true;
}



void GameScene::onEnter(){
    
    LayerColor::onEnter();
    this->addChild(_blockLayer);
    this->addChild(_uiNode);
    resetGame();
    setGameModel();
    resetTargetUI();

    addBlock();
    
    GameController::getInstance()->updateLanguage(_uiNode);
    scheduleUpdate();
    
    ad_function::instance()->showBanner();
}

void GameScene::showModelList(bool isShow){
    auto listPanel = dynamic_cast<Layout*>(_uiNode->getChildByName("list_panel"));
    if (listPanel) {
        listPanel->setVisible(isShow);
    }

}

void GameScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
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
            int n = (rand()%20)+1;
            GameController::getInstance()->playSoundEffect(StringUtils::format("%d.mp3",n),true);
            if (name == "continue_btn") {
                gameContinue();
                _gameOverDialogUINode->removeFromParentAndCleanup(false);
            }else if(name == "over_btn"){
//                GameController::getInstance()->toScene(MAIN_SCENE);
                showGameOverUI(GAME_FAIL);
            }else if(name == "share_btn"){
                
            }else if(name == "back_btn"){
                GameController::getInstance()->gameOver(_result);
                GameController::getInstance()->toScene(MAIN_SCENE);
            }else if(name == "start_again_btn"){
                GameController::getInstance()->startAgain();
                
            }
        }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event){
    
    if(GameController::getInstance()->isGameOver() != GAME_NONE) return false;
    
    Vec2 locT = touch->getLocation();
    Vec2 loc = _blockLayer->convertToNodeSpace(touch->getLocation());
    std::vector<BlockSprite*> row = _blocks.front();
    bool isHit = true;
    for (auto block:row) {
        Rect rectSuccess(block->getPosition()+Vec2(0,-_blockSize.height/3), _blockSize +Size(0,_blockSize.height*2/3));
        Rect rectError(block->getPosition(), _blockSize);

        if(block->canTap()){
            if (rectSuccess.containsPoint(loc)) {
                tap(block);
                showModelList(false);

            }
        }else{
            if (rectError.containsPoint(loc)) {
                GameController::getInstance()->playSoundEffect("error_piano.m4a", false);
                block->beTaped(true,Color4F::RED);
                showModelList(false);
                gameOver(GAME_TAP_MISTAKE);
                
            }
        }
        if (!block->isTaped() && block->canTap()) {
            isHit = false;
        }
    }
    
    if (isHit) {
        if (_startLabel->isVisible()) {
            _startLabel->setVisible(false);
        }
        _blocks.erase(_blocks.begin());
        _unUsingBlocks.insert(_unUsingBlocks.begin(), row);
        
        if (GameController::getInstance()->getSubType() == GAME_SUBTYPE_Mist) {
            VECTOR_BLOCK newRow = _blocks.front();
            bool isShow = true;
            for (auto block:newRow) {
                if (block->canTap()) {
                    isShow = false;
                    break;
                }
            }
            if (isShow) {
                int canTapIndex = getNewTapIndex(false);
                
                for (int i = 0;i<newRow.size();i++) {
                    auto block = newRow[i];
                    if(canTapIndex == i){
                        block->reset(true, true, _blockColor,1);
                        
                    }else{
                        block->reset(false, true, _bgColor,1);
                    }
                }
            }
            
        }
    }else{
        if (GameController::getInstance()->getSubType() == GAME_SUBTYPE_Bomb) {
            bool isNext = true;
            for (auto block:row) {
                if (block->canTap()) {
                    isNext = false;
                    break;
                }
            }
            if (isNext) {
                isHit = false;
                std::vector<BlockSprite*> row1 = _blocks[1];
                for (auto block:row1) {
                    Rect rectSuccess(block->getPosition()+Vec2(0,-_blockSize.height/3), _blockSize +Size(0,_blockSize.height*2/3));
                    Rect rectError(block->getPosition(), _blockSize);
                    
                    if(block->canTap()){
                        if (rectSuccess.containsPoint(loc)) {
                            tap(block);
                        }
                    }else{
                        if (rectError.containsPoint(loc)) {
                            GameController::getInstance()->playSoundEffect("error_piano.m4a", false);
                            block->beTaped(true,Color4F::RED);
                            gameOver(GAME_TAP_MISTAKE);
                            return true;
                        }
                    }
                    if (block->isTaped()) {
                        showModelList(false);
                        isHit = true;
                        break;
                    }
                }
                if (isHit) {
                    _blocks.erase(_blocks.begin());
                    _blocks.erase(_blocks.begin());
                    
                    _unUsingBlocks.insert(_unUsingBlocks.begin(), row);
                    _unUsingBlocks.insert(_unUsingBlocks.begin(), row1);
                    
                }

            }
            
        }
    }
    return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event){
    
}
void GameScene::onTouchEnded(Touch *touch, Event *unused_event){
    
}
void GameScene::onTouchCancelled(Touch *touch, Event *unused_event){
    
}

//tablevie delegate
void GameScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
    rapidjson::Value& data = _groupData[(int)cell->getIdx()];
    if (data["id"].GetString() != GameController::getInstance()->getGameId()) {
        GameController::getInstance()->startGame(data["id"].GetString(),false);
        
        auto label = dynamic_cast<Label*>(cell->getChildByTag(1));
        auto block = dynamic_cast<BlockSprite*>(cell->getChildByTag(2));
        label->setTextColor(Color4B::WHITE);
        block->changeColor(Color4F::BLACK);
        
        int old = _cellIndex;

        _cellIndex = (int)cell->getIdx();
        table->updateCellAtIndex(old);
        GameController::getInstance()->playSoundForClick();
    }
}

Size GameScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(260, 260);
}

TableViewCell* GameScene::tableCellAtIndex(TableView *table, ssize_t idx)
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

void GameScene::setTableCell(ssize_t idx,TableViewCell& cell,bool isAdd){
    if (isAdd) {
        auto bg = BlockSprite::createWithColor(false, Color4F(Color4B(240,196,46,255)), Size(260,260));
        bg->setAnchorPoint(Vec2::ZERO);
        bg->setPosition(Vec2(0, 0));
        bg->setTag(2);
        cell.addChild(bg);
        
        auto label = Label::createWithSystemFont("", "Helvetica", 40.0);
        label->setTextColor(Color4B::BLACK);
        label->setPosition(Vec2(130,200));
        label->setAnchorPoint(Vec2(0.5f,0.5f));
        label->setTag(1);
        cell.addChild(label);
    }
    
    auto label = dynamic_cast<Label*>(cell.getChildByTag(1));
    auto block = dynamic_cast<BlockSprite*>(cell.getChildByTag(2));
    if (_cellIndex == idx) {
        
        label->setTextColor(Color4B::WHITE);
        block->changeColor(Color4F::BLACK);
    }else{
        label->setTextColor(Color4B::BLACK);
        block->changeColor(Color4F(Color4B(240,196,46,255)));
    }
    if (_groupData.IsArray()) {
        rapidjson::Value& arrayValue = _groupData[(int)idx];
        std::string s = arrayValue["tid"].GetString();
        label->setString(GameController::getInstance()->getTidForKey(s.c_str()));
    }
}

ssize_t GameScene::numberOfCellsInTableView(TableView *table)
{
    return _groupData.Size();
}

void GameScene::update(float dt){
    if (_isReallyStart) {
        _gameTime += dt;
    }else{
        return;
    }
    updateTargetUI(dt);
    move(dt);
    checkPosition(dt);
    
    checkOver(dt);
    
    
    
}

void GameScene::updateTargetUI(float dt){
    if(GameController::getInstance()->getType() == GAME_TYPE_ZEN){
        _progress->setPercent((float)(GameController::getInstance()->getTimeLimit()-_gameTime)/(float)GameController::getInstance()->getTimeLimit()*100);
        _targetLabel->setString(StringUtils::format("%d",_tabedBlockCount));

    }else if(GameController::getInstance()->getType() == GAME_TYPE_CLASSICS){
        _progress->setPercent((float)_tabedBlockCount/(float)GameController::getInstance()->getTargetCount()*100);
        _targetLabel->setString(StringUtils::format("%.3f\"",_gameTime));
    }else if(GameController::getInstance()->getType() == GAME_TYPE_ARCADE){
        _targetLabel->setString(StringUtils::format("%d",_tabedBlockCount));
    }else if(GameController::getInstance()->getType() == GAME_TYPE_RUSH){
        _targetLabel->setString(StringUtils::format("%.3f/s",_speedBuf));
    }else if(GameController::getInstance()->getType() == GAME_TYPE_RELAY){
        _progress->setPercent((float)_tabedBlockCount/(float)GameController::getInstance()->getTargetCount()*100);
        _targetLabel->setString(StringUtils::format("%.3f\"",GameController::getInstance()->getTimeLimit()-_gameTime));
    }else if(GameController::getInstance()->getType() == GAME_TYPE_ARCADE_2){
        _targetLabel->setString(StringUtils::format("%d",_tabedBlockCount));
    }
}

void GameScene::resetTargetUI(){
    if(GameController::getInstance()->getType() == GAME_TYPE_ZEN){
        _progress->setPercent((float)(GameController::getInstance()->getTimeLimit()-_gameTime)/(float)GameController::getInstance()->getTimeLimit()*100);
        _targetLabel->setString(StringUtils::format("%d",0));
        
    }else if(GameController::getInstance()->getType() == GAME_TYPE_CLASSICS){
        _progress->setPercent((float)_tabedBlockCount/(float)GameController::getInstance()->getTargetCount()*100);
        _targetLabel->setString(StringUtils::format("%.3f\"",0.f));
    }else if(GameController::getInstance()->getType() == GAME_TYPE_ARCADE){
        _targetLabel->setString(StringUtils::format("%d",0));
    }else if(GameController::getInstance()->getType() == GAME_TYPE_RUSH){
        _targetLabel->setString(StringUtils::format("%.3f/s",0.0f));
    }else if(GameController::getInstance()->getType() == GAME_TYPE_RELAY){
        _progress->setPercent((float)_tabedBlockCount/(float)GameController::getInstance()->getTargetCount()*100);
        _targetLabel->setString(StringUtils::format("%.3f\"",0.0f));
    }
}
void GameScene::checkPopRow(float dt){
    
}

void GameScene::setGameModel(){
    
    _mapSize = Size(GameController::getInstance()->getMapSize(),GameController::getInstance()->getMapSize());
    Size WIN_SIZE = Director::getInstance()->getWinSize();
    int w = WIN_SIZE.width/_mapSize.width;
    int h = WIN_SIZE.height/_mapSize.height;
    if(w%2!=0) w +=1;
    if(h%2!=0) h +=1;
    
    _blockSize = Size(w,h);
    auto cloud = _uiNode->getChildByName("cloud");
    cloud->stopAllActions();
    if (GameController::getInstance()->getSubType() == GAME_SUBTYPE_Cloudy) {
        cloud->setVisible(true);
        
        cloud->runAction(RepeatForever::create(Sequence::create(
                                                                MoveTo::create(1.0f, Vec2(cloud->getPositionX(),WIN_SIZE.height*0.5)),
                                                                MoveTo::create(1.0f, Vec2(cloud->getPositionX(),WIN_SIZE.height*1.0)),
                                                                MoveTo::create(1.0f, Vec2(cloud->getPositionX(),WIN_SIZE.height*0.2)),
                                                                MoveTo::create(1.0f, Vec2(cloud->getPositionX(),WIN_SIZE.height*0.9)),
                                                                MoveTo::create(1.0f, Vec2(cloud->getPositionX(),WIN_SIZE.height*0.3)),
                                                                MoveTo::create(1.0f, Vec2(cloud->getPositionX(),WIN_SIZE.height*1.0)),NULL)));
    }else{
        cloud->setVisible(false);
    }

}

void GameScene::resetGame(){
    unscheduleUpdate();
    _result = GAME_NONE;
    _isReallyStart = false;
    _speedBuf = 0;
    _gameTime = 0.0f;
    _tabedBlockCount = 0;
    _gameResult = 0;
    _specialIndex = 10;
    _timeBlock = 3.0f;
    _speedChange = 1;
    _progress->setPercent(0);
    _targetLabel->setString(StringUtils::format("%.3f",_gameTime));
    _blockLayer->removeAllChildren();
    _blocks.clear();
    _unUsingBlocks.clear();
    _blockLayer->setPositionY(0);
    _curOffset = _blockLayer->getPositionY();
    _continueToken = 100;
    _gameOverUINode->removeFromParentAndCleanup(false);
    _gameOverDialogUINode->removeFromParentAndCleanup(false);
    showModelList(true);
    _startLabel->setVisible(true);
    
    
    updateTargetUI(0);
}

void GameScene::restartGame(){
    resetGame();
    setGameModel();
    addBlock();
    scheduleUpdate();
}
void GameScene::tap(BlockSprite* block){
    if(!_isReallyStart) _isReallyStart = true;
    block->beTaped(false,Color4F::GRAY);
    _tabedBlockCount ++;
    if (GameController::getInstance()->getType() == GAME_TYPE_RELAY) {
        if (_tabedBlockCount >= GameController::getInstance()->getTargetCount()&&GameController::getInstance()->getTimeLimit()-_gameTime >= 0) {
            _gameTime -= GameController::getInstance()->getTimeLimit();
            _tabedBlockCount = 0;
            auto labelFly = dynamic_cast<TextBMFont*>(_uiNode->getChildByName("label_fly"));
            labelFly->setString(StringUtils::format("%.3f",GameController::getInstance()->getTimeLimit()));
            labelFly->setPositionY(1050);
            labelFly->setOpacity(255);
            labelFly->runAction(Spawn::create(MoveTo::create(1.0f, Vec2(labelFly->getPosition()+Vec2(0,100))),FadeOut::create(1.0f), NULL));
            
        }
    }
    moveForTap(-_blockSize.height);
    GameController::getInstance()->playPianoSount();

}

void GameScene::checkPosition(float dt){
    if(checkAction(ACTION_BLOCK_MOVE)) return;
    std::vector<BlockSprite*> row_u = _blocks.front();

    for (int i = 0;i<row_u.size();i++) {
        auto block = row_u[i];
        if (block->canTap()) {
            if(!block->isTaped()){
                Vec2 p = _blockLayer->convertToWorldSpace(block->getPosition());
                if (p.y <= -_blockSize.height) {
                    gameOver(GAME_OVER);
                    GameController::getInstance()->playSoundEffect("error_piano.m4a", false);
                    return;
                }

            }
        }
    }
    
    if (_unUsingBlocks.size() == 0) {
        return;
    }
    
    std::vector<BlockSprite*> row = _unUsingBlocks.back();

    auto firstBlock = row[0];

    Vec2 p = _blockLayer->convertToWorldSpace(firstBlock->getPosition());
    if (p.y <= -_blockSize.height) {
        
        resetOneRowWithPos(row,true);
        _unUsingBlocks.pop_back();
        _blocks.push_back(row);
        
        _canTabBlockCount++;
    }
    
}

void GameScene::checkOver(float dt){
    GAME_TYPE type = GameController::getInstance()->getType();
    GAME_SUBTYPE subType = GameController::getInstance()->getSubType();
    
    switch (type) {
        case GAME_TYPE_CLASSICS:
            if (GameController::getInstance()->getTargetCount() <= _tabedBlockCount) {
                gameOver(GAME_SUCCESS);
            }
            break;
        case GAME_TYPE_ARCADE:
            
            break;
        case GAME_TYPE_ZEN:
            if (GameController::getInstance()->getTimeLimit() <= _gameTime) {
                gameOver(GAME_OVER);
            }
            
            break;
        case GAME_TYPE_RUSH:
            
            break;
        case GAME_TYPE_RELAY:
            if (GameController::getInstance()->getTimeLimit() <= _gameTime ) {
                gameOver(GAME_OVER);
            }
            break;
        case GAME_TYPE_ARCADE_2:
            
            break;
        default:
            break;
    }
}

bool GameScene::checkAction(ACTION_TYPE action){
    GAME_TYPE type = GameController::getInstance()->getType();

    switch (action) {
        case ACTION_BLOCK_MOVE:
            switch (type) {
                case GAME_TYPE_CLASSICS:
                    if (_canTabBlockCount >= GameController::getInstance()->getTargetCount()) {
                        return true;
                    }
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
    return false;
}

void GameScene::resetOneRowWithPos(const VECTOR_BLOCK& row,bool isMovePos){
    
    int canTapIndex = getNewTapIndex(false);
    int canTapIndex1 = -1;
    if (_tabedBlockCount > _specialIndex) {
        _specialIndex = _tabedBlockCount+hy_function::instance()->randomFrom(5, 20);
    }
    
    if(GameController::getInstance()->getSubType() == GAME_SUBTYPE_Double  && _specialIndex == _tabedBlockCount){
        canTapIndex1 = getNewTapIndex(false);
    }
    for (int i = 0;i<row.size();i++) {
        auto block = row[i];
        if(GameController::getInstance()->getSubType() == GAME_SUBTYPE_Mist && _specialIndex == _tabedBlockCount){
            block->reset(false, false, Color4F(0.2f+_blockColor.r, 0.2f+_blockColor.g, 0.2f+_blockColor.b, 1),1);

        }else if(canTapIndex == i){
            if(GameController::getInstance()->getSubType() == GAME_SUBTYPE_Bomb && _specialIndex == _tabedBlockCount){
                block->reset(false, true, _blockColor,"res/textures/skull.png",1);
            }else if(GameController::getInstance()->getSubType() == GAME_SUBTYPE_Bilayer && _specialIndex == _tabedBlockCount){
                block->reset(true, true, _blockColor,"res/textures/bilayer.png",2);
            }
            else{
                block->reset(true, true, _blockColor,1);
            }
        
        }else if(canTapIndex1 == i){
            block->reset(true, true, _blockColor,1);
        }else{
            block->reset(false, true, _bgColor,1);
        }
        if (isMovePos) {
            block->setPositionY(block->getPositionY()+_blockSize.height*(_mapSize.height+BUFF_COUNT));
        }
    }
}

Vec2 GameScene::tileCoordForPosition(Vec2 pos){
    int x = pos.x/_blockSize.width;
    int y = pos.y/_blockSize.height;
    return Vec2(x,y);
}
void GameScene::addBlock(){
    _blocks.clear();
    
    srand( (int)time( NULL ));

    std::vector<int> colors;

    for (int i = 1; i<=_mapSize.height+BUFF_COUNT; i++) {
        
        std::vector<BlockSprite*> row;
        int canTapIndex = getNewTapIndex(false);

        for (int j = 0 ; j < _mapSize.width; j++) {
            BlockSprite* block;
            if (canTapIndex == j) {
                block = BlockSprite::createWithColor(true,_blockColor,_blockSize);
                //start lable's position
                if (i == 1) {
                    _startLabel->setPosition(Vec2((j+0.5f)*_blockSize.width,_blockSize.height*(i+0.5f)));
                }
            }else{
                block = BlockSprite::createWithColor(false,_bgColor,_blockSize);
            }
            
            Vec2 pos(Vec2(j*_blockSize.width,_blockSize.height*i));
            block->setPosition(pos);
            _blockLayer->addChild(block);
            row.push_back(block);
        }
        
        _blocks.push_back(row);
        
    }
    _canTabBlockCount = _mapSize.height+BUFF_COUNT;
    
    for (int j = 0 ; j < _mapSize.width; j++) {
        BlockSprite* block = BlockSprite::createWithColor(true,_bgColor,_blockSize);
        Vec2 pos(Vec2(j*_blockSize.width,0));
        block->setPosition(pos);
        _blockLayer->addChild(block);
    }
}

int GameScene::getNewTapIndex(bool isFirst){
    if(!isFirst && GameController::getInstance()->getSubType() == GAME_SUBTYPE_Plus){
        std::vector<int> a;
        for (int i = 0 ; i < _mapSize.width; i++) {
            if (_lastCanTapIndex != i) {
                a.push_back(i);
            }
        }
        int n = rand()%(int)a.size();
        _lastCanTapIndex = a[n];
    }else{
        _lastCanTapIndex = rand()%(int)_mapSize.width;

    }
    
    return _lastCanTapIndex;
}
void GameScene::change(){
    LIST_VECTOR_BLOCK::iterator it;
    int i = 0;
    for (it = _blocks.begin();it != _blocks.end();it++) {
        if (i >= 2) {
            VECTOR_BLOCK blocks = *it;
            resetOneRowWithPos(blocks,false);
            break;
        }
        i++;
    }
}
GameScene* GameScene::createScene(){
    auto layer = GameScene::create();
    return layer;
}

void GameScene::moveForTap(float offset){
    if (GameController::getInstance()->getType() == GAME_TYPE_CLASSICS ||
        GameController::getInstance()->getType() == GAME_TYPE_ZEN ||
        GameController::getInstance()->getType() == GAME_TYPE_RELAY) {
        _curOffset += offset;
        float time = (std::fabs(_curOffset-_blockLayer->getPositionY()))/2400.0f;
        auto action = MoveTo::create(time, Vec2(_blockLayer->getPositionX(),_curOffset));
        _blockLayer->runAction(action);
        
    }
}

void GameScene::moveForBack(){
    _curOffset -= _blockSize.height;
    auto action = MoveTo::create(.12f, Vec2(_blockLayer->getPositionX(),_curOffset));
    _blockLayer->runAction(EaseIn::create(action, 1.0f));
}

void GameScene::move(float dt){
    if(GameController::getInstance()->getType() == GAME_TYPE_ARCADE ||
       GameController::getInstance()->getType() == GAME_TYPE_RUSH ||
       GameController::getInstance()->getType() == GAME_TYPE_ARCADE_2){
        _blockLayer->setPositionY(_blockLayer->getPositionY() - 25 -_speedBuf);
        if (GameController::getInstance()->getSubType() == GAME_SUBTYPE_Unstable) {
            if (_gameTime > _timeBlock) {
                int a[12] = {3,5,-2,3,7,-2,1,5,-4,3,7,-4};
                _speedChange = a[_timeIndex];
                
                _timeIndex++;
                if(_timeIndex == 11){
                    _timeIndex = 0;
                }
                _timeBlock = _gameTime + 3.0f;
                
                log("change----%f",_timeBlock);
            }else{
                _speedBuf += 0.006f*_speedChange;
                log("%d",_speedChange);
                
                if (_speedBuf<0) {
                    _speedBuf+=0.006f*GameController::getInstance()->getSpeed();
                }
            }
        }else{
            _speedBuf+=0.006f*GameController::getInstance()->getSpeed();
        }
        log("%f",_speedBuf);

        
    }
}

void GameScene::setBackgroung(){
    
}

Color4F GameScene::randomBrightColor(){
    srand( (unsigned)time( NULL ) );

    float requireBrightness = 90;
    while (true) {
        Color4B randomColor(rand()%255,
                            rand()%255,
                            rand()%255,
                            255);
        if (randomColor.r < requireBrightness ||
            randomColor.g < requireBrightness ||
            randomColor.b < requireBrightness) {
            return Color4F(randomColor);
        }
    }
}

void GameScene::gameOver(GAME_RESULT result){
    unscheduleUpdate();
    calculateResult();
    _result = result;
    showGameOverUI(result);
    if(result == GAME_TAP_MISTAKE){
        
    }else{
        GameController::getInstance()->gameOver(result);
    }
    
//    this->runAction(Sequence::create(CallFunc::create([this,result](){
//        
//    }),NULL));
    
}

void GameScene::gameContinue(){
    if (GameController::getInstance()->getToken() >= _continueToken) {
        GameController::getInstance()->addToken(-_continueToken);
        _isReallyStart = false;
        _result = GAME_NONE;
        scheduleUpdate();
        std::vector<BlockSprite*> row = _blocks.front();
        resetOneRowWithPos(row,false);
        for (auto block:row) {
            if (block->canTap()) {
                Vec2 p = block->convertToWorldSpace(Vec2::ZERO);
                _startLabel->setPosition(p + _blockSize*0.5f);
                _startLabel->setVisible(true);
                break;
            }
            
        }
        _continueToken*=1.5;
    }
}
void GameScene::calculateResult(){
    auto type = GameController::getInstance()->getType();
//    auto subType = GameController::getInstance()->getSubType();
    
    if(type == GAME_TYPE_ZEN){
        _gameResult = _tabedBlockCount;
        
    }else if(type == GAME_TYPE_CLASSICS){
        _gameResult = _gameTime;
        
    }else if(type == GAME_TYPE_ARCADE){
        _gameResult = _tabedBlockCount;
        
    }else if(type == GAME_TYPE_ARCADE_2){
        _gameResult = _tabedBlockCount;
        
    }else if(type == GAME_TYPE_RUSH){
        _gameResult = _speedBuf;
        
    }else if(type == GAME_TYPE_RELAY){
        _gameResult = _tabedBlockCount;
    }

}
void GameScene::showGameOverUI(GAME_RESULT result){
    _gameOverAction->setLastFrameCallFunc(CC_CALLBACK_0(GameScene::showDone, this));
    _gameOverDialogAction->setLastFrameCallFunc(CC_CALLBACK_0(GameScene::dialogShowDone, this));
    auto layout = dynamic_cast<Layout*>(_gameOverUINode->getChildByName("Panel"));
    switch (result) {
        case GAME_SUCCESS:
            if(!_gameOverUINode->getParent()){
                this->addChild(_gameOverUINode);
            }
            _gameOverAction->play("show", false);
            layout->setBackGroundColor(Color3B(133,213,98));
            //Color3B(237,79,79)
            break;
        case GAME_FAIL:
            if(!_gameOverUINode->getParent()){
                this->addChild(_gameOverUINode);
            }
            _gameOverAction->play("show", false);
            layout->setBackGroundColor(Color3B(207,81,80));
            //(97,236,79)
            break;
        case GAME_OVER:
            if(!_gameOverUINode->getParent()){
                this->addChild(_gameOverUINode);
            }
            _gameOverAction->play("show", false);
            layout->setBackGroundColor(Color3B(50,50,132));
            break;
        case GAME_TAP_MISTAKE:
            if(!_gameOverDialogUINode->getParent()){
                this->addChild(_gameOverDialogUINode);
            }
            _gameOverDialogAction->play("show", false);
            break;

        default:
            break;
    }
    updateResultUI();
    updateDialogUI();
}
void GameScene::showDone(){
    
    GAME_RESULT result = GameController::getInstance()->isGameOver();
    auto noteFly = _gameOverUINode->getChildByName("Panel")->getChildByName("note_fly");

    if(_tabedBlockCount > 0){
        
        auto tokenCount = dynamic_cast<TextBMFont*>(noteFly->getChildByName("token_count"));
        tokenCount->setString(StringUtils::format("%d",_tabedBlockCount));
        auto note = _gameOverUINode->getChildByName("Panel")->getChildByName("note");
        noteFly->setPosition(_gameOverUINode->getChildByName("Panel")->getContentSize()/2);
        noteFly->setVisible(true);
        noteFly->runAction(Sequence::create(MoveTo::create(1.0f, note->getPosition()),CallFunc::create([noteFly,this](){
            noteFly->setVisible(false);
            int tokenCount = GameController::getInstance()->getUserData("token", DATA_INT).GetInt();
            auto token = dynamic_cast<cocos2d::ui::Text*>(_gameOverUINode->getChildByName("Panel")->getChildByName("token_text"));
            
            token->setString(StringUtils::format("%d",tokenCount));
        }), NULL));
    }else{
        noteFly->setVisible(false);
    }
    
    
    _gameOverAction->clearLastFrameCallFunc();
    _gameOverDialogAction->clearLastFrameCallFunc();
    
    switch (result) {
        case GAME_SUCCESS:
            if(!_gameOverUINode->getParent()){
                this->addChild(_gameOverUINode);
            }
            _gameOverAction->play("game_success", true);
            break;
        case GAME_FAIL:
            if(!_gameOverUINode->getParent()){
                this->addChild(_gameOverUINode);
            }
            if(GameController::getInstance()->getType() != GAME_TYPE_CLASSICS){
                _gameOverAction->play("game_success", true);
            }else{
                _gameOverAction->play("game_fail", true);
            }
            

            break;
        case GAME_OVER:
            if(!_gameOverUINode->getParent()){
                this->addChild(_gameOverUINode);
            }
            _gameOverAction->play("game_success", true);
            break;
        case GAME_TAP_MISTAKE:
            if(!_gameOverUINode->getParent()){
                this->addChild(_gameOverUINode);
            }
            if(GameController::getInstance()->getType() != GAME_TYPE_CLASSICS){
                _gameOverAction->play("game_success", true);
            }else{
                _gameOverAction->play("game_fail", true);
            }
            
            break;

        default:
            break;
    }
}

void GameScene::dialogShowDone(){
    GAME_RESULT result = GameController::getInstance()->isGameOver();
    switch (result) {
//        case GAME_SUCCESS:
//            if(!_gameOverUINode->getParent()){
//                this->addChild(_gameOverUINode);
//            }
//            _gameOverAction->play("game_success", true);
//            break;
//        case GAME_FAIL:
//            if(!_gameOverUINode->getParent()){
//                this->addChild(_gameOverUINode);
//            }
//            _gameOverAction->play("game_fail", true);
//            
//            break;
//        case GAME_OVER:
//            if(!_gameOverUINode->getParent()){
//                this->addChild(_gameOverUINode);
//            }
//            _gameOverAction->play("game_success", true);
//            break;
        case GAME_TAP_MISTAKE:
            if(!_gameOverDialogUINode->getParent()){
                this->addChild(_gameOverDialogUINode);
            }
            if (GameController::getInstance()->getType() == GAME_TYPE_CLASSICS) {
                _gameOverDialogAction->play("game_fail", true);
            }else{
                _gameOverDialogAction->play("game_success", true);
            }
            
            
            break;
            
        default:
            break;
    }
}
void GameScene::updateResultUI(){
    auto token = dynamic_cast<cocos2d::ui::Text*>(_gameOverUINode->getChildByName("Panel")->getChildByName("token_text"));
    auto typeText = dynamic_cast<cocos2d::ui::Text*>(_gameOverUINode->getChildByName("Panel")->getChildByName("game_type"));
    auto subTypeText = dynamic_cast<cocos2d::ui::Text*>(_gameOverUINode->getChildByName("Panel")->getChildByName("game_subtype"));
//    auto resultText = dynamic_cast<cocos2d::ui::Text*>(_gameOverUINode->getChildByName("Panel")->getChildByName("result"));
    auto resultText_1 = dynamic_cast<cocos2d::ui::Text*>(_gameOverUINode->getChildByName("Panel")->getChildByName("result_1"));

    auto scoreText = dynamic_cast<TextBMFont*>(_gameOverUINode->getChildByName("Panel")->getChildByName("result_score"));
    auto scoreText_1 = dynamic_cast<TextBMFont*>(_gameOverUINode->getChildByName("Panel")->getChildByName("result_score_1"));
    
    
    int tokenCount = GameController::getInstance()->getUserData("token", DATA_INT).GetInt();
    token->setString(StringUtils::format("%d",tokenCount - _tabedBlockCount > 0?tokenCount - _tabedBlockCount:0));
    rapidjson::Value gameData;
    GameController::getInstance()->getGameData(GameController::getInstance()->getGameId(), gameData);
    
    typeText->setString(gameData["mode_tid"].GetString());
    subTypeText->setString(gameData["tid"].GetString());

    GAME_TYPE type = GameController::getInstance()->getType();
//    GAME_SUBTYPE subType = GameController::getInstance()->getSubType();
    float record = GameController::getInstance()->getUserResult(GameController::getInstance()->getGameId());
    
    resultText_1->setString("BEST");
    
    if(type == GAME_TYPE_CLASSICS){
        
        if (_result == GAME_SUCCESS) {
            if (_gameResult < record) {
                record = _gameResult;
                resultText_1->setString("NEW BEST");
            }else{
                resultText_1->setString("BEST");
            }
        }
    }else{
        if (_gameResult > record) {
            record = _gameResult;
            resultText_1->setString("NEW BEST");
        }else{
            resultText_1->setString("BEST");
        }
    }
    
    
    if(type == GAME_TYPE_ZEN || type == GAME_TYPE_ARCADE || type == GAME_TYPE_RELAY || type == GAME_TYPE_ARCADE_2){
        scoreText->setString(StringUtils::format("%d",(int)record));
        scoreText_1->setString(StringUtils::format("%d",(int)_gameResult));

    }else if(type == GAME_TYPE_CLASSICS){
        
        scoreText->setString(StringUtils::format("%.3f\"",record));
        scoreText_1->setString(StringUtils::format("%.3f\"",_gameResult));

    }else if(type == GAME_TYPE_RUSH){
        
        scoreText->setString(StringUtils::format("%.3f/s",record));
        scoreText_1->setString(StringUtils::format("%.3f/s",_gameResult));
}
    
    
    
    GameController::getInstance()->updateLanguage(_gameOverUINode);
}
void GameScene::updateDialogUI(){
    
//    auto resultText = dynamic_cast<Label*>(_gameOverDialogUINode->getChildByName("Panel")->getChildByName("result_text"));
//    auto tipsText = dynamic_cast<Label*>(_gameOverDialogUINode->getChildByName("Panel")->getChildByName("tips"));
    auto scoreTead = dynamic_cast<Text*>(_gameOverDialogUINode->getChildByName("Panel")->getChildByName("score_head"));
    auto scoreText = dynamic_cast<TextBMFont*>(_gameOverDialogUINode->getChildByName("Panel")->getChildByName("score_text"));
    auto scoreText_1 = dynamic_cast<TextBMFont*>(_gameOverDialogUINode->getChildByName("Panel")->getChildByName("score_text_1"));
    
    auto tokenCost = dynamic_cast<Text*>(_gameOverDialogUINode->getChildByName("Panel")->getChildByName("continue_game")->getChildByName("token_cost"));
    tokenCost->setString(StringUtils::format("%d",_continueToken));
    GAME_TYPE type = GameController::getInstance()->getType();
//    GAME_SUBTYPE subType = GameController::getInstance()->getSubType();
    float record = GameController::getInstance()->getUserResult(GameController::getInstance()->getGameId());
    
    scoreTead->setString("BEST");
    
    
    
    if(type == GAME_TYPE_CLASSICS){
        if (_result == GAME_SUCCESS) {
            if (_gameResult < record) {
                record = _gameResult;
                scoreTead->setString("NEW BEST");
            }else{
                scoreTead->setString("BEST");
            }
        }
        
    }else{
        if (_gameResult > record) {
            record = _gameResult;
            scoreTead->setString("NEW BEST");
        }else{
            scoreTead->setString("BEST");
        }
    }
    
    if(type == GAME_TYPE_ZEN || type == GAME_TYPE_ARCADE || type == GAME_TYPE_RELAY || type == GAME_TYPE_ARCADE_2){
        scoreText->setString(StringUtils::format("%d",(int)record));
        scoreText_1->setString(StringUtils::format("%d",(int)_gameResult));

    }else if(type == GAME_TYPE_CLASSICS){

        scoreText->setString(StringUtils::format("%.3f\"",record));
        scoreText_1->setString(StringUtils::format("%.3f\"",_gameResult));

    }else if(type == GAME_TYPE_RUSH){

        scoreText->setString(StringUtils::format("%.3f/s",record));
        scoreText_1->setString(StringUtils::format("%.3f/s",_gameResult));

    }
    GameController::getInstance()->updateLanguage(_gameOverDialogUINode);
}
//ui
//void GameScene::onDraw(const cocos2d::Mat4 &transform, uint32_t flags){
//    this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
//    CC_NODE_DRAW_SETUP();
//    Size winSize = Director::getInstance()->getWinSize();
//    int nVertices = 0;
//    Vec2 vertices1[4];
//    Color4F colors1[4];
//    vertices1[nVertices] = Vec2(0, 0);
//    colors1[nVertices++] = _backgroundColor;
//    
//    vertices1[nVertices] = Vec2(0, winSize.height);
//    colors1[nVertices++] = _backgroundColor;
//    
//    vertices1[nVertices] = Vec2(winSize.width, 0);
//    colors1[nVertices++] = _backgroundColor;
//    
//    vertices1[nVertices] = Vec2(winSize.width, winSize.height);
//    colors1[nVertices++] = _backgroundColor;
//    
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices1);
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors1);
//    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    //网格
//    Vec2 vertices2[12];
//    Color4F colors2[12];
//    nVertices = 0;
//    for (int i = 1; i<=3; i++) {
//        colors2[nVertices] = Color4F::GRAY;
//        vertices2[nVertices++] = Vec2(_blockSize.width*i,0);
//        
//        colors2[nVertices] = Color4F::GRAY;
//        vertices2[nVertices++] = Vec2(_blockSize.width*i,winSize.height);
////        
//        colors2[nVertices] = Color4F::GRAY;
//        vertices2[nVertices++] = Vec2(0,_blockSize.height*i);
//        
//        colors2[nVertices] = Color4F::GRAY;
//        vertices2[nVertices++] = Vec2(winSize.width,_blockSize.height*i);
//        
//
//    }
//    
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices2);
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors2);
//    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
//    glDrawArrays(GL_LINES, 0, 12);
    
    
//}
//void GameScene::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags){
//    _customCommand.init(getGlobalZOrder(),transform,flags);
//    _customCommand.func = CC_CALLBACK_0(GameScene::onDraw, this,transform,flags);
//    renderer->addCommand(&_customCommand);
//}
