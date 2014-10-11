//
//  StoreScene.h
//  DwarfForest
//
//  Created by Agris Dumins on 7/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>
#include "StoreBox.h"

class StoreScene: public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(StoreScene);
    
    cocos2d::CCSprite* aBase;
    
    cocos2d::CCSprite* aBaseBase;
    
    StoreBox* mBox;
    
    bool mLevelEndCase;
    
    cocos2d::CCSprite* mTabUpgradesOn;
    cocos2d::CCSprite* mTabUpgradesOff;
    cocos2d::CCSprite* mTabShopOn;
    cocos2d::CCSprite* mTabShopOff;
    cocos2d::CCSprite* mTabFreeOn;
    cocos2d::CCSprite* mTabFreeOff;
    
    cocos2d::CCLabelTTF* _diamondsLabel;
    cocos2d::CCLabelTTF* _scoreLabel;
    
    void UpdateStats();
    
    void OnTab1();
    void OnTab2();
    void OnTab3();
    
    void SetActiveTab(int theType);
    int mActiveTab;
    
    static uint32_t getSpecialOfferCount();
    
    void RealRemove();
    void RemovePopup();
	
private:
    void menuBackCallback(CCObject* sender);
    void btNoAddsCallback(CCObject* sender);
    void bt10Callback(CCObject* sender);
    void bt100Callback(CCObject* sender);
    void bt300Callback(CCObject* sender);
    
};