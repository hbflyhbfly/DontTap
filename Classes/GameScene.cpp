//
//  GameScene.cpp
//  DontTap
//
//  Created by Syuuhi on 4/30/16.
//
//
#include "MainScene.hpp"
#include "GameScene.hpp"
#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include "time.h"
#include "GameController.hpp"
USING_NS_CC;
using namespace cocostudio;
using namespace ui;

#define BUFF_COUNT 3

bool GameScene::init(){
    if (!LayerColor::init()) {
        return false;
    }
    _bgColor = Color4F::WHITE;
    _blockColor = randomBrightColor();
    
    _uiNode = CSLoader::getInstance()->createNode("ui/GameScene.csb");
    
    Button* backButton = dynamic_cast<Button*>(_uiNode->getChildByName("back_button"));
    auto callback = [this](Ref* ref){
        auto mainScene = MainScene::createScene();
        Director::getInstance()->replaceScene(mainScene);
    };
    backButton->addClickEventListener(callback);
    
    _blockLayer = Layer::create();
    
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    _mapSize = Size(5,5);
    Size WIN_SIZE = Director::getInstance()->getWinSize();
    int w = WIN_SIZE.width/_mapSize.width;
    int h = WIN_SIZE.height/_mapSize.height;
    if(w%2!=0) w +=1;
    if(h%2!=0) h +=1;

    _blockSize = Size(w,h);
    
    return true;
}



void GameScene::onEnter(){
    LayerColor::onEnter();
    this->addChild(_blockLayer);
    _curOffset = _blockLayer->getPositionY();
    addBlock();
    
    this->addChild(_uiNode);
    scheduleUpdate();
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event){
    
    if(GameController::getInstance()->isGameOver()) return false;
    
    Vec2 locT = touch->getLocation();
    Vec2 loc = _blockLayer->convertToNodeSpace(touch->getLocation());
    std::vector<BlockSprite*> row = _blocks.front();
    bool isHit = false;
    for (auto block:row) {
        Rect rect(block->getPosition()+Vec2(0,_blockSize.height/6), _blockSize +Size(0,_blockSize.height/3));
        if (rect.containsPoint(loc)) {
            if(block->canTap()){
                tap(block);
            }else{
                block->blink();
                gameOver();
                GameController::getInstance()->gameOver();
            }
            isHit = true;
            break;
        }
    }
    
    if (isHit) {
        _blocks.pop_front();
        _unUsingBlocks.push_front(row);
    }
    return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event){
    
}
void GameScene::onTouchEnded(Touch *touch, Event *unused_event){
    
}
void GameScene::onTouchCancelled(Touch *touch, Event *unused_event){
    
}

void GameScene::update(float dt){
    checkPosition(dt);
    
}

void GameScene::checkPopRow(float dt){
    
}

void GameScene::tap(BlockSprite* block){
    block->setBlockColor(Color4F::GRAY);
    move(-_blockSize.height);
    _tabedBlockCount ++;
}

void GameScene::checkPosition(float dt){
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
    }
}

void GameScene::checkOver(float dt){
    
}

void GameScene::resetOneRowWithPos(const VECTOR_BLOCK& row,bool isMovePos){
    int canTapIndex = rand()%(int)_mapSize.width;
    
    for (int i = 0;i<row.size();i++) {
        auto block = row[i];
        if(canTapIndex == i){
            block->reset(true, true, _blockColor);
        }else{
            block->reset(false, true, _bgColor);
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

    
    for (int i = 1; i<=_mapSize.height+BUFF_COUNT; i++) {
        
        std::vector<BlockSprite*> row;
        int canTapIndex = rand()%(int)_mapSize.width;
        
        for (int j = 0 ; j < _mapSize.width; j++) {
            BlockSprite* block;
            if (canTapIndex == j) {
                block = BlockSprite::createWithColor(true,_blockColor,_blockSize);
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
    for (int j = 0 ; j < _mapSize.width; j++) {
        BlockSprite* block = BlockSprite::createWithColor(true,Color4F::BLUE,_blockSize);
        Vec2 pos(Vec2(j*_blockSize.width,0));
        block->setPosition(pos);
        _blockLayer->addChild(block);
    }
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
Scene* GameScene::createScene(){
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

void GameScene::move(float offset){
    _curOffset += offset;
    auto action = MoveTo::create(.12f, Vec2(_blockLayer->getPositionX(),_curOffset));
    _blockLayer->runAction(action);
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

void GameScene::gameOver(){
    
    unscheduleUpdate();
}

void GameScene::onDraw(const cocos2d::Mat4 &transform, uint32_t flags){
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
    
    
}
void GameScene::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags){
//    _customCommand.init(getGlobalZOrder(),transform,flags);
//    _customCommand.func = CC_CALLBACK_0(GameScene::onDraw, this,transform,flags);
//    renderer->addCommand(&_customCommand);
}
