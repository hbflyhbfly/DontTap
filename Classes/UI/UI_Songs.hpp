//
//  MainScene.hpp
//  DontTap
//
//  Created by Syuuhi on 4/30/16.
//
//

#ifndef UI_Songs_hpp
#define UI_Songs_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;
using namespace ui;

class UI_Songs:public cocos2d::Layer,public TableViewDataSource, public TableViewDelegate{

public:
    UI_Songs();
    virtual ~UI_Songs();
    virtual bool init() override;
    virtual void onEnter() override;
    CREATE_FUNC(UI_Songs);
    
    void updateUI();
    void closeUI();
    void showUI();
private:
    
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    void setTableCell(ssize_t idx,TableViewCell& cell,bool isAdd = false);
    
private:
    cocos2d::Node* _uiNode;
};
#endif /* MainScene_hpp */
