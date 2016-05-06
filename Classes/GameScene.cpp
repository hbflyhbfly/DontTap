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

USING_NS_CC;
using namespace cocostudio;
using namespace ui;


bool GameScene::init(){
    if (!Layer::init()) {
        return false;
    }
    setBackgroung();
    addBlock();
    
    _uiNode = CSLoader::getInstance()->createNode("ui/GameScene.csb");
    this->addChild(_uiNode);
//    timeline::ActionTimeline* timeline = CSLoader::getInstance()->createTimeline("ui/GameScene.csb");
//    timeline->play("idel", true);
//    _uiNode->runAction(timeline);
    
    Button* backButton = dynamic_cast<Button*>(_uiNode->getChildByName("back_button"));
    auto callback = [this](Ref* ref){
        auto mainScene = MainScene::createScene();
        Director::getInstance()->replaceScene(mainScene);
    };
    backButton->addClickEventListener(callback);
    
    //
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    
    scheduleUpdate();
    return true;
}



void GameScene::onEnter(){
    
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event){
    auto posInMap = this->convertToNodeSpace(touch->getLocation());
    auto tilePos = tileCoordForPosition(posInMap);
    
    if (tilePos == _blockPos[0]) {
        for (int i = 0; i < _blockPos.size(); i++) {
            _blockPos[i] = _blockPos[i] - Vec2(0,1);
            auto block = _blocks[i];
            auto moveDown = MoveTo::create(0.2f, Vec2(_blockPos[i].x*_blockSize.width,_blockPos[i].y*_blockSize.height));
            block->runAction(moveDown);
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

void GameScene::update(float dt){
    auto block =_blocks[0];
    Vec2 pos = tileCoordForPosition(block->getPosition());
    if (pos.y < -1) {
        
    }
}

Vec2 GameScene::tileCoordForPosition(Vec2 pos){
    int x = pos.x/_blockSize.width;
    int y = pos.y/_blockSize.height;
    return Vec2(x,y);
}
void GameScene::addBlock(){
    _blockPos.clear();
    _blocks.clear();
    
    Size WIN_SIZE = Director::getInstance()->getWinSize();
    
    _blockSize = Size(WIN_SIZE.width/4,WIN_SIZE.height/4);
    
//    _heigh = WIN_SIZE.height/_blockSize.height + 1;
//    _heigh = 1;
    srand( (unsigned)time( NULL ) );
    for (int i = 0; i<9; i++) {
        
        auto block = BlockSprite::createWithColor(Color4F::ORANGE,_blockSize);
//        block->setAnchorPoint(Vec2(0,0));
        int x = rand()%4;
//        x = 1;
        Vec2 pos(Vec2(x*_blockSize.width,_blockSize.height*i));
        block->setPosition(pos);
        _blocks.push_back(block);
        _blockPos.push_back(pos);
        
        this->addChild(block);
    }
    
}

Scene* GameScene::createScene(){
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

void GameScene::setBackgroung(){
    _backgroundColor = randomBrightColor();
    
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


void GameScene::onDraw(const cocos2d::Mat4 &transform, uint32_t flags){
    this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    CC_NODE_DRAW_SETUP();
    Size winSize = Director::getInstance()->getWinSize();
    int nVertices = 0;
    Vec2 vertices1[4];
    Color4F colors1[4];
    vertices1[nVertices] = Vec2(0, 0);
    colors1[nVertices++] = _backgroundColor;
    
    vertices1[nVertices] = Vec2(0, winSize.height);
    colors1[nVertices++] = _backgroundColor;
    
    vertices1[nVertices] = Vec2(winSize.width, 0);
    colors1[nVertices++] = _backgroundColor;
    
    vertices1[nVertices] = Vec2(winSize.width, winSize.height);
    colors1[nVertices++] = _backgroundColor;
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors1);
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    //网格
    Vec2 vertices2[12];
    Color4F colors2[12];
    nVertices = 0;
    for (int i = 1; i<=3; i++) {
        colors2[nVertices] = Color4F::GRAY;
        vertices2[nVertices++] = Vec2(_blockSize.width*i,0);
        
        colors2[nVertices] = Color4F::GRAY;
        vertices2[nVertices++] = Vec2(_blockSize.width*i,winSize.height);
//        
        colors2[nVertices] = Color4F::GRAY;
        vertices2[nVertices++] = Vec2(0,_blockSize.height*i);
        
        colors2[nVertices] = Color4F::GRAY;
        vertices2[nVertices++] = Vec2(winSize.width,_blockSize.height*i);
        

    }
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices2);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors2);
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    glDrawArrays(GL_LINES, 0, 12);
    
    
}
void GameScene::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags){
    _customCommand.init(getGlobalZOrder(),transform,flags);
    _customCommand.func = CC_CALLBACK_0(GameScene::onDraw, this,transform,flags);
    renderer->addCommand(&_customCommand);
}
