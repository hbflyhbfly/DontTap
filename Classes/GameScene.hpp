//
//  GameScene.hpp
//  DontTap
//
//  Created by Syuuhi on 4/30/16.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "BlockSprite.hpp"

USING_NS_CC;
typedef std::vector<BlockSprite*> VECTOR_BLOCK;
typedef std::list<VECTOR_BLOCK> LIST_VECTOR_BLOCK;

typedef enum{
    ACTION_BLOCK_MOVE_DONE,
    ACTION_BLOCK_
} ACTION_TYPE;

class GameScene:public cocos2d::LayerColor{
public:
    virtual bool init() override;
    virtual void onEnter() override;
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(GameScene);
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    bool onTouchBegan(Touch *touch, Event *unused_event) override;
    void onTouchMoved(Touch *touch, Event *unused_event) override;
    void onTouchEnded(Touch *touch, Event *unused_event) override;
    void onTouchCancelled(Touch *touch, Event *unused_event) override;
    void gameOver();
private:
    void addBlock();
    void setBackgroung();
    void checkPopRow(float dt = .0f);
    void checkPosition(float dt = .0f);
    void checkOver(float dt = .0f);
    void checkAction(ACTION_TYPE action);
    cocos2d::Color4F randomBrightColor();
    Vec2 tileCoordForPosition(Vec2 pos);
    void resetOneRowWithPos(const VECTOR_BLOCK& row,bool isMovePos);
    void update(float dt) override;
    void move(float offset);
    void tap(BlockSprite* block);
    void change();
protected:
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    
private:
    CustomCommand _customCommand;
    cocos2d::Size _blockSize;
    Color4F _bgColor;
    Color4F _blockColor;
    cocos2d::Size _mapSize;
    cocos2d::Node* _uiNode;
    cocos2d::Layer* _blockLayer;

    LIST_VECTOR_BLOCK _blocks;
    LIST_VECTOR_BLOCK _unUsingBlocks;
    float _curOffset;//移动偏移
    float _gameTime;//当前时间
    int _tabedBlockCount;//当前击中多少块
    
};
#endif /* GameScene_hpp */
