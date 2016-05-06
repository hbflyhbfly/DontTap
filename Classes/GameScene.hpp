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

class GameScene:public cocos2d::Layer{
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
private:
    void addBlock();
    void setBackgroung();
    cocos2d::Color4F randomBrightColor();
    Vec2 tileCoordForPosition(Vec2 pos);
    void update(float dt);
protected:
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    
private:
    CustomCommand _customCommand;
    cocos2d::Color4F _backgroundColor;
    cocos2d::Size _blockSize;
    int _heigh;
    cocos2d::Node* _uiNode;
    cocos2d::Layer* _blockLayer;
    std::vector<Vec2> _blockPos;
    std::vector<BlockSprite*> _blocks;
    
};
#endif /* GameScene_hpp */
