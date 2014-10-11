//
//  OptionsScene.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 26.02.2013.
//
//
#pragma once

#include <cocos2d.h>

class OptionsScene: public cocos2d::CCLayer
{
public:
	// implement the "static node()" method manually
    CREATE_FUNC(OptionsScene);
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    virtual void onEnter();
    virtual void onExit();
    
    void RemovePopup();
    void RealRemove();
    
    cocos2d::CCSprite* aBase;
    
    void OnExitTutorial();
	
    void menuEnableDebugButtons(CCObject* sender);
    void menuResetCallback(CCObject* sender);
	void menuBackCallback(CCObject* sender);
    void menuIntroCallback(CCObject* sender);
    void menuTutorialCallback(CCObject* sender);
    void menuCreditsCallback(CCObject* sender);
    void menuStatsCallback(CCObject* sender);
    void menuSoundCallback(CCObject* sender);
    void menuMusicCallback(CCObject* sender);
    void menuVibrationCallback(cocos2d::CCObject *sender);
    
    void menuSingOutClick();
    
    void OnRemoveStats();
    
    void loginFB(CCObject* sender);
    void logoutFB(CCObject* sender);
    
    void onEnterTransitionDidFinish();
	void keyBackClicked();
    
private:
	//FB STUFF - FOR NOW
    cocos2d::CCMenuItemImage* _fbButtonLogIn;
    cocos2d::CCMenuItemImage* _fbButtonLogOut;
};
