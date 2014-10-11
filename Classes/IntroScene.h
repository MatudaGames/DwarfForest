//
//  IntroScene.h
//  DwarfForest
//
//  Created by Agris Dumins on 7/10/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>
#include <string.h>

class IntroScene: public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(bool firstTime);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(bool firstTime = false);
    
    static IntroScene* create(bool firstTime);
	
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onEnter();
    virtual void onExit();
    
private:
    void btNextCallback(CCObject* sender);
    void btPrevCallback(CCObject* sender);
    void btCloseCallback(CCObject* sender);
    
    const static int IMG_CNT = 5;
    std::string _images[IMG_CNT];
    int _activeImg;
    cocos2d::CCSprite* _activeImage;
    
    bool _moveStarted;
    cocos2d::CCPoint _dragStart;
	
	bool _firstTime;
};
