//
//  LevelUpPopUp.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 10/15/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "LevelUpPopUp.h"
#include "Utils.h"
#include "AppMacros.h"
#include "TimedSpriteAnimation.h"
#include "LevelEndScene.h"

USING_NS_CC;
using namespace DF::LevelEnd;

LevelUpPopUp* LevelUpPopUp::create(uint32_t level)
{
    LevelUpPopUp *pRet = new LevelUpPopUp();
    if (pRet && pRet->init(level))
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

bool LevelUpPopUp::init(uint32_t level)
{
    if (!CCLayer::init()) return false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/level_end/level_up_popup/level_up_main.png");
    back->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
    addChild(back);
    
    TimedSpriteAnimation* t1 = TimedSpriteAnimation::create("Interfeiss/level_end/level_up_popup/shiny_stuff.plist", 3);
    t1->setPosition(ccp(visibleSize.width / 2 + 100, visibleSize.height / 2));
    addChild(t1);
    
    TimedSpriteAnimation* t2 = TimedSpriteAnimation::create("Interfeiss/level_end/level_up_popup/shiny_stuff.plist", 3);
    t2->setPosition(ccp(visibleSize.width / 2 - 100, visibleSize.height / 2));
    addChild(t2);
    
    TimedSpriteAnimation* t3 = TimedSpriteAnimation::create("Interfeiss/level_end/level_up_popup/shiny_stuff.plist", 3);
    t3->setPosition(ccp(visibleSize.width / 2 + 20, visibleSize.height / 2 + 80));
    addChild(t3);
    
    CCMenuItemImage* closeBt = CCMenuItemImage::create("Interfeiss/level_end/level_up_popup/x_btn0001.png",
                                                       "Interfeiss/level_end/level_up_popup/x_btn0002.png",
                                                       this, menu_selector(LevelUpPopUp::closeCallback));
    
    CCMenu* closeMenu = CCMenu::create(closeBt, NULL);
    closeMenu->setPosition(ccp(640, 420));
    addChild(closeMenu);
    
    CCMenuItemImage* shareBt = CCMenuItemImage::create("Interfeiss/level_end/level_up_popup/share_btn20001.png",
                                                       "Interfeiss/level_end/level_up_popup/share_btn20002.png",
                                                       this, menu_selector(LevelUpPopUp::shareCallback));
    CCMenu* shareMenu = CCMenu::create(shareBt, NULL);
    shareMenu->setPositionY(170);
    addChild(shareMenu);
    
    
    CCLabelTTF* levelTxt = CCLabelTTF::create(toString(level).c_str(), FONT_SKRANJI,
                                              TITLE_FONT_SIZE*1.6, CCSize(145, 145),
                                              kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    levelTxt->setColor(ccc3(0xFF, 0xFC, 0x00));
    levelTxt->setPosition(ccp(visibleSize.width / 2+5, visibleSize.height / 2 - 28));
    addChild(levelTxt);
    
    return true;
}

void LevelUpPopUp::closeCallback()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    this->runAction(CCSequence::create(CCMoveTo::create(0.5, ccp(0, visibleSize.height)),
                                       CCCallFunc::create(this, callfunc_selector(LevelUpPopUp::remove)),
                                       NULL));
    
    ((LevelEnd::LevelEndScene*)this->getParent())->fadeInLevelEnd();
    ((LevelEnd::LevelEndScene*)this->getParent())->exp->barFill->continueFill();
}

void LevelUpPopUp::remove()
{
    this->getParent()->removeChild(this);
}

void LevelUpPopUp::shareCallback()
{
    
}

void LevelUpPopUp::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void LevelUpPopUp::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool LevelUpPopUp::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    return true;
}
