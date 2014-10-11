//
//  StatsBox.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 09/03/14.
//
//

#include "StatsBox.h"
#include "JniCalls.h"
#include "AppMacros.h"

#include "User.h"
#include "StatsScreen.h"

USING_NS_CC;

CCScene* StatsBox::scene(CCRect size,CCRect zone)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StatsBox* layer = StatsBox::create(size,zone);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

StatsBox* StatsBox::create(CCRect size,CCRect zone)
{
    StatsBox *pRet = new StatsBox();
    if (pRet && pRet->init(size,zone))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

// on "init" you need to initialize your instance
bool StatsBox::init(CCRect size,CCRect zone)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    mLayer_2X = 0;
    
    mBoxCords = size;
    mZoneCords = zone;
    mZoneCords = CCRect(zone.origin.x+zone.size.width/2+100,zone.origin.y+zone.size.height/2+40,zone.size.width,zone.size.height);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    mBase = CCSprite::create("Interfeiss/store/FRESH/df_shop.png");
    mBase->setOpacity(0);
    mBase->setPosition(ccp(zone.origin.x+zone.size.width/2+10,zone.origin.y+zone.size.height/2));
    addChild(mBase);
    
    CCLayerColor* aLayer = CCLayerColor::create(ccc4(0,0,0,128),zone.size.width,zone.size.height);
    aLayer->setPosition(ccp(zone.origin.x,zone.origin.y));
    
//    CCLog("mBase->getPositionY():%f",mBase->getPositionY());
    
    mMinScrollY = mBase->getPositionY();
    
    CCLabelTTF* aStoreHeader;
    
    //Create all the stuff
    int aX = 20;
    int aY = 540;
    int aSpaceY = 10;
    
    CCSprite* aHeader = CCSprite::create("Interfeiss/stats/title_single_game.png");
    aHeader->setPosition(ccp(aX,aY));
    mBase->addChild(aHeader,1);
    aY-=aHeader->getContentSize().height+20;
    
    //=============================================================================
    
    CCSprite* aSlot = CCSprite::create("Interfeiss/stats/panel_stats.png");
    aSlot->setPosition(ccp(aX,aY));
    aY-=aSlot->getContentSize().height+aSpaceY;
    
    aStoreHeader = CCLabelTTF::create("High score", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(26,30));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    aSlot->addChild(aStoreHeader);
    
    aStoreHeader = CCLabelTTF::create(toString(User::getInstance()->getStats().getHighscore()).c_str(), "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(aSlot->getContentSize().width/2-30,30));
    aStoreHeader->setColor(ccc3(255, 255, 255));
    aSlot->addChild(aStoreHeader);
    
    mBase->addChild(aSlot);
    
    //=============================================================================
    
    aSlot = CCSprite::create("Interfeiss/stats/panel_stats.png");
    aSlot->setPosition(ccp(aX,aY));
    aY-=aSlot->getContentSize().height+aSpaceY;
    
    aStoreHeader = CCLabelTTF::create("Most Dwarfs rescued", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(26,30));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    aSlot->addChild(aStoreHeader);
    
    aStoreHeader = CCLabelTTF::create(toString(User::getInstance()->getStats().getMostDwarfsRescued()).c_str(), "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(aSlot->getContentSize().width/2-30,30));
    aStoreHeader->setColor(ccc3(255, 255, 255));
    aSlot->addChild(aStoreHeader);
    
    mBase->addChild(aSlot);
    
    //=============================================================================
    
    aHeader = CCSprite::create("Interfeiss/stats/title_lifetime.png");
    aHeader->setPosition(ccp(aX,aY));
    mBase->addChild(aHeader,1);
    aY-=aHeader->getContentSize().height+30;
    
    //=============================================================================
    
    aSlot = CCSprite::create("Interfeiss/stats/panel_stats.png");
    aSlot->setPosition(ccp(aX,aY));
    aY-=aSlot->getContentSize().height+aSpaceY;
    
    aStoreHeader = CCLabelTTF::create("Total Games", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(26,30));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    aSlot->addChild(aStoreHeader);
    
    aStoreHeader = CCLabelTTF::create(toString(User::getInstance()->getStats().getTotalGames()).c_str(), "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(aSlot->getContentSize().width/2-30,30));
    aStoreHeader->setColor(ccc3(255, 255, 255));
    aSlot->addChild(aStoreHeader);
    
    mBase->addChild(aSlot);
    
    //=============================================================================
    
    aSlot = CCSprite::create("Interfeiss/stats/panel_stats.png");
    aSlot->setPosition(ccp(aX,aY));
    aY-=aSlot->getContentSize().height+aSpaceY;
    
    aStoreHeader = CCLabelTTF::create("Total Green crystals collected", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(26,30));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    aSlot->addChild(aStoreHeader);
    
    aStoreHeader = CCLabelTTF::create(toString(User::getInstance()->getStats().getGreenCrystals()).c_str(), "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(aSlot->getContentSize().width/2-30,30));
    aStoreHeader->setColor(ccc3(255, 255, 255));
    aSlot->addChild(aStoreHeader);
    
    mBase->addChild(aSlot);
    
    //=============================================================================
    
    aSlot = CCSprite::create("Interfeiss/stats/panel_stats.png");
    aSlot->setPosition(ccp(aX,aY));
    aY-=aSlot->getContentSize().height+aSpaceY;
    
    aStoreHeader = CCLabelTTF::create("Total Blue crystals collected", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(26,30));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    aSlot->addChild(aStoreHeader);
    
    aStoreHeader = CCLabelTTF::create(toString(User::getInstance()->getStats().getBlueCrystals()).c_str(), "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(aSlot->getContentSize().width/2-30,30));
    aStoreHeader->setColor(ccc3(255, 255, 255));
    aSlot->addChild(aStoreHeader);
    
    mBase->addChild(aSlot);
    
    //=============================================================================
    
    aSlot = CCSprite::create("Interfeiss/stats/panel_stats.png");
    aSlot->setPosition(ccp(aX,aY));
    aY-=aSlot->getContentSize().height+aSpaceY;
    
    aStoreHeader = CCLabelTTF::create("Total Red crystals collected", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(26,30));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    aSlot->addChild(aStoreHeader);
    
    aStoreHeader = CCLabelTTF::create(toString(User::getInstance()->getStats().getRedCrystals()).c_str(), "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(aSlot->getContentSize().width/2-30,30));
    aStoreHeader->setColor(ccc3(255, 255, 255));
    aSlot->addChild(aStoreHeader);
    
    mBase->addChild(aSlot);
    
    //=============================================================================
    
    aSlot = CCSprite::create("Interfeiss/stats/panel_stats.png");
    aSlot->setPosition(ccp(aX,aY));
    aY-=aSlot->getContentSize().height+aSpaceY;
    
    aStoreHeader = CCLabelTTF::create("Total Yellow crystals collected", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(26,30));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    aSlot->addChild(aStoreHeader);
    
    aStoreHeader = CCLabelTTF::create(toString(User::getInstance()->getStats().getYellowCrystals()).c_str(), "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(aSlot->getContentSize().width/2-30,30));
    aStoreHeader->setColor(ccc3(255, 255, 255));
    aSlot->addChild(aStoreHeader);
    
    mBase->addChild(aSlot);
    
    //=============================================================================
    
    aSlot = CCSprite::create("Interfeiss/stats/panel_stats.png");
    aSlot->setPosition(ccp(aX,aY));
    aY-=aSlot->getContentSize().height+aSpaceY;
    
    aStoreHeader = CCLabelTTF::create("Total Diamonds collected", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(26,30));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    aSlot->addChild(aStoreHeader);
    
    aStoreHeader = CCLabelTTF::create(toString(User::getInstance()->getStats().getDiamondsCollected()).c_str(), "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(aSlot->getContentSize().width/2-30,30));
    aStoreHeader->setColor(ccc3(255, 255, 255));
    aSlot->addChild(aStoreHeader);
    
    mBase->addChild(aSlot);
    
    //=============================================================================
    
    aSlot = CCSprite::create("Interfeiss/stats/panel_stats.png");
    aSlot->setPosition(ccp(aX,aY));
    
    aStoreHeader = CCLabelTTF::create("Total Dwarfs rescued", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(26,30));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    aSlot->addChild(aStoreHeader);
    
    aStoreHeader = CCLabelTTF::create(toString(User::getInstance()->getStats().getTotalDwarfsrescued()).c_str(), "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(400, 75),
                                      kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setAnchorPoint(ccp(0,0.5f));
    aStoreHeader->setPosition(ccp(aSlot->getContentSize().width/2-30,30));
    aStoreHeader->setColor(ccc3(255, 255, 255));
    aSlot->addChild(aStoreHeader);
    
    mBase->addChild(aSlot);
    
    //=============================================================================
    
    mMaxScrollY = 123;
    
    return true;
}

void StatsBox::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void StatsBox::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool StatsBox::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    mDidMoveFinger = false;
    
    //    CCLOG("Did touch started");
    if(!mZoneCords.containsPoint(touch->getLocation()))
    {
        return false;
    }
    
    if (_moveStarted == false)
    {
        _moveStarted = true;
        _dragStart = touch->getLocation();
        
        if(mBase->isVisible())
        {
            if(mBase->numberOfRunningActions()>0)
                mBase->stopAllActions();
            
            mLayer_2X = mBase->getPositionY();
        }
        
        mSlidingFree = false;
    }
    
    return true;
}

void StatsBox::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    _moveStarted = false;
    _dragEnd = touch->getLocation();
    
    //check if slider is not too much off limits
    int aPosY = 0;
    bool aNeedToMove = false;
    
    //Disable all button clicks if was active
    if(mBase->isVisible())
    {
//        CCLog("mBase->getPositionY():%f",mBase->getPositionY());
        if(mBase->getPositionY()<mMinScrollY)
        {
            aPosY = mMinScrollY;
            aNeedToMove = true;
        }
        else if(mBase->getPositionY()>mMaxScrollY)
        {
            aPosY = mMaxScrollY;
            aNeedToMove = true;
        }
        
        if(aNeedToMove)
        {
            CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(mBase->getPositionX(),aPosY));
            CCEaseBackOut* aEase1 = CCEaseBackOut::create(aMove1);
            mBase->runAction(aEase1);
        }
    }
}

void StatsBox::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(mDidMoveFinger!=true)
        mDidMoveFinger = true;
    
    if(mBase->isVisible())
        mBase->setPositionY(mLayer_2X+(touch->getLocation().y - _dragStart.y));
}

void StatsBox::visit()
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(mBoxCords.origin.x-30,mBoxCords.origin.y,mBoxCords.size.width+10,mBoxCords.size.height);
    CCLayer::visit();
    glDisable(GL_SCISSOR_TEST);
}