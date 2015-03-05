#pragma once

#include <cocos2d.h>

class GameScene;

class SaveMeScene: public cocos2d::CCLayer
{
public:
	// implement the "static node()" method manually
    CREATE_FUNC(SaveMeScene);
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
	
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onEnter();
    virtual void onExit();
    
    void endGameScreen();
    
private:
	void menuResumeCallback(CCObject* sender);
    void menuMainMenuCallback(CCObject* sender);
    void menuTutorialCallback(CCObject* sender);
    
    void menuSoundCallback(CCObject* sender);
    void menuMusicCallback(CCObject* sender);
	
    void onCreatePrice();
    void playAgain(CCObject* sender);
    void menuSkipCallback(CCObject* sender);
    void menuSaveCallback(CCObject* sender);
    
    cocos2d::CCSize mScreenSize;
    cocos2d::CCLayerColor* mKautkasScreen;
    cocos2d::CCLabelTTF* _pointLabel;
    
    GameScene *_game;
    
};
