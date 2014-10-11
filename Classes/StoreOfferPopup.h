#pragma once

#include <cocos2d.h>

class StoreOfferPopup: public cocos2d::CCLayer
{
public:
	CREATE_FUNC(StoreOfferPopup);
    
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    
private:
    void updateCounter();
    
    void notNowCallback();
    void storeOfferCallback();
    
    void update(float delta);
    
    cocos2d::CCLabelTTF* h1;
    cocos2d::CCLabelTTF* h2;
    cocos2d::CCLabelTTF* m1;
    cocos2d::CCLabelTTF* m2;
    cocos2d::CCLabelTTF* delim;
};
