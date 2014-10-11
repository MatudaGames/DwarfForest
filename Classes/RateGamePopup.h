#pragma once

#include <cocos2d.h>

class RateGamePopup: public cocos2d::CCLayer
{
public:
	
    CREATE_FUNC(RateGamePopup);
    
    virtual bool init();
    
private:
    cocos2d::CCLayerColor* background;
    cocos2d::CCLayer* container;
    cocos2d::CCMenuItemToggle* cbButton;
    
    void notNowCallback();
    void rateCallback();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onEnter();
    virtual void onExit();
    
    void remove();
};
