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
_color(Color4F::WHITE){
    
}

BlockSprite::~BlockSprite(){
    
}

bool BlockSprite::initWithColor(cocos2d::Color4F color,Size size){
    if (!Node::init()) {
        return false;
    }
    _color = color;
    _size = size;
    
    return true;
}

BlockSprite* BlockSprite::createWithColor(Color4F color,Size size)
{
    BlockSprite *sprite = new (std::nothrow) BlockSprite();
    if (sprite && sprite->initWithColor(color,size))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
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
    for (int i =0; i<4; i++) {
        colors1[i] = Color4F::GRAY;
    }
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION,2, GL_FLOAT, GL_FALSE, 0, vertices1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors1);
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    glDrawArrays(GL_LINES, 0, 4);

}


void BlockSprite::onEnter(){
    Node::onEnter();
}

void BlockSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
    _customCommand.init(_globalZOrder,transform,flags);
    _customCommand.func = CC_CALLBACK_0(BlockSprite::onDraw, this,transform,flags);
    renderer->addCommand(&_customCommand);
}