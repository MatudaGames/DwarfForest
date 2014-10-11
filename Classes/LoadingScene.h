//
//  LoadingScene.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 24/11/13.
//
//

#pragma once

#include <cocos2d.h>
#include <string.h>

class LoadingScene: public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    static LoadingScene* create();
	
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
//    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onEnter();
    virtual void onExit();
    
//    virtual void visit();
    
    void exitScene();
    
    void playSound1();
    void playSound2();
    void playSound3();
    void playSound4();
    
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