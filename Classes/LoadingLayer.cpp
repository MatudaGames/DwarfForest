//
//  LoadingLayer.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 24/11/13.
//
//

#include "LoadingLayer.h"
//#include "AppMacros.h"
//#include "GameScene.h"

USING_NS_CC;

LoadingLayer* LoadingLayer::create(cocos2d::CCLayer* parent)
{
    LoadingLayer *layer = new LoadingLayer();
    
    if (layer && layer->init(parent))
    {
        // Set to autorelease
        layer->autorelease();
        
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool LoadingLayer::init(cocos2d::CCLayer* parent)
{
     visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
//    CCLayerColor* back = CCLayerColor::create(ccc4(255, 255, 255, 255), visibleSize.width, visibleSize.height);
//    back->setAnchorPoint(ccp(0,0));
//    back->setPosition(ccp(100,100));
//    addChild(back);
    
    return true;
}

void LoadingLayer::visit()
{
    kmGLPushMatrix();
    
    glEnable(GL_SCISSOR_TEST);
    
    CCEGLView::sharedOpenGLView()->setScissorInPoints(visibleSize.width/2-122, visibleSize.height/2-235+70, 238, 430);
    CCNode::visit();
    glDisable(GL_SCISSOR_TEST);
    
    kmGLPopMatrix();
}