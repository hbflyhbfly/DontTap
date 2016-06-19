//
//  BlockSprite.cpp
//  DontTap
//
//  Created by Syuuhi on 5/1/16.
//
//

#include "BlockSprite.hpp"

BlockSprite::BlockSprite():
_isInUsing(false),
_color(Color4F::WHITE),
_blinkColor(Color4F::RED),
_beTaped(false),
_tapCount(1),
_beTapedCount(0){
    
}

BlockSprite::~BlockSprite(){
    
}

bool BlockSprite::initWithColor(bool canTap,cocos2d::Color4F color,Size size){
    if (!Node::init()) {
        return false;
    }
    _color = color;
    _size = size;
    _isCanTap = canTap;
    _tattoo = Sprite::create();
    _tattoo->setPosition(Vec2(size.width/2,size.height/2));
    addChild(_tattoo);
    return true;
}

BlockSprite* BlockSprite::createWithColor(bool canTap,Color4F color,Size size)
{
    BlockSprite *sprite = new (std::nothrow) BlockSprite();
    if (sprite && sprite->initWithColor(canTap,color,size))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
void BlockSprite::changeColor(Color4F color){
    _color = color;
}
void BlockSprite::reset(bool canTap,bool inUsing,Color4F color){
    _color = color;
    _isCanTap = canTap;
    _isInUsing = inUsing;
    _beTaped = false;
    _beTapedCount = 0;
    _tapCount = 1;
    _tattoo->setVisible(false);
}

void BlockSprite::reset(bool canTap, bool inUsing, cocos2d::Color4F color,const std::string& tatto,int TapCount){
    reset(canTap,inUsing,color,TapCount);
    if (!tatto.empty()) {
        _tattoo->setSpriteFrame(tatto);
        _tattoo->setVisible(true);
    }else{
        _tattoo->setVisible(false);
    }
}

void BlockSprite::reset(bool canTap, bool inUsing, cocos2d::Color4F color,int tapCount){
    reset(canTap,_isInUsing,color);
    _tapCount = tapCount;
}

void resetBomb(cocos2d::Color4F color,const std::string& tatto){
    
}

void BlockSprite::beTaped(bool isError,Color4F color){
    if (isError) {
        _blinkDur = 1.0f;
        setBlockColor(_blinkColor);
        this->runAction(Blink::create(0.6f, 4));
    }else{
        if(_beTapedCount < _tapCount){
            _beTapedCount++;
        }
        if (_beTapedCount == _tapCount) {
            _beTaped = true;
        }else{
            setBlockColor(Color4F(_color.r - 0.2f,_color.g - 0.2f,_color.b - 0.2f,1));
        }
        if (_beTaped) {
            setBlockColor(color);
        }
//        this->runAction(Sequence::create(ScaleTo::create(0.1f, 0.9f),ScaleTo::create(0.1f, 1.0f),NULL));
    }
    
}

void BlockSprite::blink(){
    _color = _blinkColor;
}
void BlockSprite::onDraw(const cocos2d::Mat4 &transform, uint32_t flags){
    
    
    int nVertices = 0;
    Vec2 vertices1[4];
    Color4F colors1[4];
    vertices1[nVertices] = Vec2(0, 0);
    colors1[nVertices++] = _color;
    
    vertices1[nVertices] = Vec2(0, _size.height);
    colors1[nVertices++] = _color;
    
    vertices1[nVertices] = Vec2(_size.width, 0);
    colors1[nVertices++] = _color;
    
    vertices1[nVertices] = Vec2(_size.width, _size.height);
    colors1[nVertices++] = _color;
    


//    getGLProgram()->use();
//    getGLProgram()->setUniformsForBuiltins(transform);
//
//    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR );
    
    this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    CC_NODE_DRAW_SETUP();
    
    //
    // Attributes
    //
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //色块
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors1);
    
    glBlendFunc(CC_BLEND_SRC,CC_BLEND_DST);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    //网格
    nVertices = 0;
    vertices1[nVertices] = Vec2(0, 0);
    colors1[nVertices++] = Color4F::BLACK;
    
    vertices1[nVertices] = Vec2(_size.width, 0);
    colors1[nVertices++] = Color4F::BLACK;
    
    vertices1[nVertices] = Vec2(_size.width, _size.height);
    colors1[nVertices++] = Color4F::BLACK;
    
    vertices1[nVertices] = Vec2(0, _size.height);
    colors1[nVertices++] = Color4F::BLACK;

    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION,2, GL_FLOAT, GL_FALSE, 0, vertices1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors1);
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

}


void BlockSprite::onEnter(){
    Node::onEnter();
    _tattoo->setAnchorPoint(Vec2(0.5f,0.5f));
    
}

void BlockSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
    _customCommand.init(_globalZOrder,transform,flags);
    _customCommand.func = CC_CALLBACK_0(BlockSprite::onDraw, this,transform,flags);
    renderer->addCommand(&_customCommand);
}