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
    bool initWithColor(Color4F color,Size size);
    static BlockSprite* createWithColor(Color4F color,Size size);
protected:
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);

private:
    Color4F _color;
    Size _size;
    CustomCommand _customCommand;
    bool _isInUsing;
};

#endif /* BlockSprite_hpp */
