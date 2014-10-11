//
//  LoadingLayer.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 24/11/13.
//
//

#pragma once

#include <cocos2d.h>
#include <string>

typedef void (cocos2d::CCObject::*CallBack)();

class LoadingLayer: public cocos2d::CCLayer
{
public:
    static LoadingLayer* create(cocos2d::CCLayer* parent);
    
    bool init(cocos2d::CCLayer* parent);
    
    virtual void visit();
    
    cocos2d::CCSize visibleSize;
    
private:
    int mValue;
};