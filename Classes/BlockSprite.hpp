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
    void setBlockColor(Color4F color){_color = color;};
    void reset(bool canTap,bool inUsing,Color4F color);
    void blink();
    void playAction();
protected:
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);

private:
    Color4F _color;
    Size _size;
    CustomCommand _customCommand;
    bool _isInUsing;
    bool _isCanTap;
    Color4F _blinkColor;

    float _blinkDur;
};

#endif /* BlockSprite_hpp */
