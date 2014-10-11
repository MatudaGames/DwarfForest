#pragma once

#include <cocos2d.h>
#include "GameScene.h"

class FreeStuffPopup : public cocos2d::CCLayer
{
public:
	CREATE_FUNC(FreeStuffPopup);
    
    virtual bool init();
    
//    static cocos2d::CCScene* scene();
    GameScene* _gameScene;
    
private:
    cocos2d::CCMenuItemToggle* cbButton;
    
    void notNowCallback();
    void facebookCallback();
    void twitterCallback();
    
    
    
    void remove();
    
    void onEnter();
    void onExit();
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
};
