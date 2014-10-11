//
//  StatsBox.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 09/03/14.
//
//

#pragma once

#include <cocos2d.h>
#include "Utils.h"

class StatsBox: public cocos2d::CCLayer
{
public:
    //    CREATE_FUNC(StoreBox);
    cocos2d::CCRect mBoxCords;
    cocos2d::CCRect mZoneCords;
    
    cocos2d::CCSprite* mBase;
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(cocos2d::CCRect size,cocos2d::CCRect zone);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(cocos2d::CCRect size,cocos2d::CCRect zone);
    
    static StatsBox* create(cocos2d::CCRect size,cocos2d::CCRect zone);
	
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onEnter();
    virtual void onExit();
    
    void visit();
    
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
	
private:
    bool mDone;
};