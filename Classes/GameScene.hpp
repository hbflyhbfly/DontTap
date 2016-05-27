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
#include "extensions/cocos-ext.h"
#include "ui/UILoadingBar.h"
#include "UITextBMFont.h"
USING_NS_CC;
USING_NS_CC_EXT;

typedef std::vector<BlockSprite*> VECTOR_BLOCK;
typedef std::list<VECTOR_BLOCK> LIST_VECTOR_BLOCK;


typedef enum{
    ACTION_BLOCK_MOVE_DONE,
    ACTION_BLOCK_
} ACTION_TYPE;

class GameScene:public cocos2d::LayerColor,public TableViewDataSource, public TableViewDelegate{
public:
    GameScene();
    ~GameScene();
    virtual bool init() override;
    virtual void onEnter() override;
    static GameScene* createScene();
    
    CREATE_FUNC(GameScene);
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    bool onTouchBegan(Touch *touch, Event *unused_event) override;
    void onTouchMoved(Touch *touch, Event *unused_event) override;
    void onTouchEnded(Touch *touch, Event *unused_event) override;
    void onTouchCancelled(Touch *touch, Event *unused_event) override;
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
    void gameOver();
    void restartGame();
public:
    Scene* _scene;
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
    void setGameModel();
    void resetGame();
    void setTableCell(ssize_t idx,TableViewCell& cell,bool isAdd = false);
    void showModelList(bool isShow);
protected:
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    
private:
    CustomCommand _customCommand;
    cocos2d::Size _blockSize;
    Color4F _bgColor;
    Color4F _blockColor;
    cocos2d::Size _mapSize;
    cocos2d::Node* _uiNode;
    Node* _startLabel;
    cocos2d::ui::LoadingBar* _progress;
    cocos2d::ui::TextBMFont* _targetLabel;
    cocos2d::Layer* _blockLayer;

    LIST_VECTOR_BLOCK _blocks;
    LIST_VECTOR_BLOCK _unUsingBlocks;
    float _curOffset;//移动偏移
    float _gameTime;//当前时间
    int _tabedBlockCount;//当前击中多少块
    bool _isReallyStart;

    rapidjson::Value _groupData;
    
};
#endif /* GameScene_hpp */
