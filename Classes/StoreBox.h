//
//  StoreBox.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 02/03/14.
//
//

#pragma once

#include <cocos2d.h>
#include "Utils.h"

#define BUTTON_ACTION 10
#define BUTTON_ICON 30
#define ICON_LOCKED 20

#define POWER_DESC 40

#define UPGRADE_STAR 100

#define ITEM_PRICE_AMOUNT 101
#define ITEM_PRICE_TEXT 102
#define ITEM_PRICE_ICON 103

class StoreBox: public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
//    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
//    static cocos2d::CCScene* scene(int x);
    
    // implement the "static node()" method manually
//    CREATE_FUNC(StoreBox);
    cocos2d::CCRect mBoxCords;
    cocos2d::CCRect mZoneCords;
    
    cocos2d::CCSprite* mBase;
    cocos2d::CCSprite* mBaseShop;
    cocos2d::CCSprite* mBaseFreeStuff;
    
    bool mUpgradesCreated;
    bool mShopCreated;
    bool mFreeStuffCreated;
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(cocos2d::CCRect size,cocos2d::CCRect zone);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(cocos2d::CCRect size,cocos2d::CCRect zone);
    
    static StoreBox* create(cocos2d::CCRect size,cocos2d::CCRect zone);
	
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onEnter();
    virtual void onExit();
    
    void OnFreeStuff(cocos2d::CCObject* sender);
    
    void CreateUpgrades();
    void CreateShop();
    void CreateFreeStuff();
    
    void visit();
    
    void UpdateStats();
    
    int mLayer_2X;
    bool mMoveBackScroll;
    bool mDidMoveFinger;
    bool mSlidingFree;
    bool _moveStarted;
    cocos2d::CCPoint _dragStart;
    cocos2d::CCPoint _dragEnd;
    
    int mMinScrollY;
    int mMaxScrollY;
    
    int mMinScroll_Shop_Y;
    int mMaxScroll_Shop_Y;
    
    int mMinScroll_Free_Y;
    int mMaxScroll_Free_Y;
    
    
    int mPlantButtonState;
    int mGhostButtonState;
    int mStampButtonState;
    int mElectroButtonState;
    
    void CreateMachineByLevel(int theMachine,int theLevel,bool theCreate = false);
    void ChangeButtonState(int theButton,int theState);
    
    void UnlockNow_Plant(cocos2d::CCObject *sender);
    void Buy_Plant(cocos2d::CCObject *sender);
    
    void UnlockNow_Ghost(cocos2d::CCObject *sender);
    void Buy_Ghost(cocos2d::CCObject *sender);
    
    void UnlockNow_Electro(cocos2d::CCObject *sender);
    void Buy_Electro(cocos2d::CCObject *sender);
    
    void UnlockNow_Stamp(cocos2d::CCObject *sender);
    void Buy_Stamp(cocos2d::CCObject *sender);
    
    
    void OnSpellClick(CCObject* sender);
    void OnPowerUpClick(CCObject* sender);
    void CheckSpellButtons(bool OnlySpells, bool OnlyPowers);
    
    std::vector<std::string> SplitString_VecString(const std::string s,char delim);
    
    int mCurrentSelectedButtonIndex;
	
private:
    bool mDone;
};