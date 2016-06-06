//
//  BlockSprite.hpp
//  DontTap
//
//  Created by Syuuhi on 5/1/16.
//
//

#ifndef BlockSprite_hpp
#define BlockSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class BlockSprite:public cocos2d::Node {
    
public:
    BlockSprite();
    ~BlockSprite();
    virtual void onEnter() override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    bool initWithColor(bool canTap,Color4F color,Size size);
    static BlockSprite* createWithColor(bool canTap ,Color4F color,Size size);
    bool canTap(){return _isCanTap;};
    bool inUsing(){return _isInUsing;};
    void setUsing(bool inUsing){_isInUsing = inUsing;};
    void setCanTap(bool canTap){_isCanTap = canTap;};
    void beTaped(bool isError,Color4F color);
    bool isTaped(){return _beTaped;};
    void reset(bool canTap,bool inUsing,Color4F color);
    void reset(bool canTap, bool inUsing, cocos2d::Color4F color,const std::string& tatto,int tapCount);
    void reset(bool canTap, bool inUsing, cocos2d::Color4F color,int tapCount);
    void blink();
    void playAction();
private:
    void setBlockColor(Color4F color){_color = color;};
protected:
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);

private:
    Color4F _color;
    Size _size;
    CustomCommand _customCommand;
    bool _isInUsing;
    bool _isCanTap;
    int _tapCount;
    Color4F _blinkColor;
    bool _beTaped;
    float _blinkDur;
    int _beTapedCount;
    Sprite* _tattoo;
};

#endif /* BlockSprite_hpp */
