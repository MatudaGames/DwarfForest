//
//  StatsScreen.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 09/03/14.
//
//

#pragma once

#include <cocos2d.h>
#include "StatsBox.h"

class StatsScreen: public cocos2d::CCLayer
{
public:
	// implement the "static node()" method manually
    CREATE_FUNC(StatsScreen);
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
//    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
//    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
//    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    virtual void onEnter();
    virtual void onExit();
    
    void RemovePopup();
    void RealRemove();
    
    bool mRemoving;
    
    cocos2d::CCSprite* aBase;
    
    cocos2d::CCSprite* mBase;
    
    cocos2d::CCSprite* aBaseBase;
    
    StatsBox* mBox;
    
//    void loginFB(CCObject* sender);
//    void logoutFB(CCObject* sender);
    
//    void onEnterTransitionDidFinish();
//	void keyBackClicked();
    
//    cocos2d::CCLayer* slide;
//    cocos2d::CCGLProgram* boxShader1;
//    cocos2d::CCGLProgram* boxShader2;
    
//    virtual void update(float delta);
    
//    int sh1_minyLoc;
//    int sh1_maxyLoc;
//    int sh2_minyLoc;
//    int sh2_maxyLoc;
//    
//    float sh1_miny;
//    float sh1_maxy;
//    float sh2_miny;
//    float sh2_maxy;
    
private:
};
