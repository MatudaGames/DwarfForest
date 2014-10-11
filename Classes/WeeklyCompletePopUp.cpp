//
//  WeeklyCompletePopUp.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 10/18/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "WeeklyCompletePopUp.h"
#include "AppMacros.h"
#include "Utils.h"
#include "TimedSpriteAnimation.h"
#include "LevelEndScene.h"

USING_NS_CC;

WeeklyCompletePopUp* WeeklyCompletePopUp::create(std::string text, uint32_t reward, WeeklyTask::RewardType type)
{
    WeeklyCompletePopUp *pGOL = new WeeklyCompletePopUp();
    if (pGOL && pGOL->init(text, reward, type)) {
        pGOL->autorelease();
        return pGOL;
    }
    CC_SAFE_DELETE(pGOL);
    return NULL;
}

bool WeeklyCompletePopUp::init(std::string text, uint32_t reward, WeeklyTask::RewardType type)
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/challenges/complete_popup/weekly_ch_complete.png");
    back->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
    addChild(back);
    
    
    CCMenuItemImage* closeBt = CCMenuItemImage::create("Interfeiss/level_end/level_up_popup/x_btn0001.png",
                                                       "Interfeiss/level_end/level_up_popup/x_btn0002.png",
                                                       this, menu_selector(WeeklyCompletePopUp::close));
    
    CCMenu* closeMenu = CCMenu::create(closeBt, NULL);
    closeMenu->setPosition(ccp(710, 410));
    addChild(closeMenu);
    
    CCMenuItemImage* shareBt =
            CCMenuItemImage::create("Interfeiss/challenges/complete_popup/share_btn_still0001.png",
                                    "Interfeiss/challenges/complete_popup/share_btn_still0002.png",
                                    this, menu_selector(WeeklyCompletePopUp::share));
    CCMenu* shareMenu = CCMenu::create(shareBt, NULL);
    shareMenu->setPositionY(150);
    shareMenu->setPositionX(visibleSize.width / 2 + 70);
    addChild(shareMenu);
    
    
    // title
    CCLabelTTF* levelTxt = CCLabelTTF::create(text.c_str(), FONT_SKRANJI_BOLD,
                                              TITLE_FONT_SIZE*0.6, CCSize(280, 120),
                                              kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
    levelTxt->setColor(ccc3(0x4F, 0x98, 0x8B));
    levelTxt->setRotation(-6);
    levelTxt->setPosition(ccp(visibleSize.width / 2+70, visibleSize.height / 2 - 32));
    addChild(levelTxt);
    
    // reward
    CCLabelTTF* rewardSh = CCLabelTTF::create(toString(reward).c_str(), FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1,
                                  CCSize(100, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    rewardSh->setAnchorPoint(ccp(0,0));
    rewardSh->cocos2d::CCNode::setPosition(ccp(580, 2));
    rewardSh->setColor(ccc3(0xff, 0xff, 0xff));
    rewardSh->enableShadow(CCSize(3,3), 0.5, 3, true);
    rewardSh->setRotation(-6);
    rewardSh->setPosition(ccp(visibleSize.width / 2 + 28, 215));
    addChild(rewardSh);
    
    CCLabelTTF* rewardT = CCLabelTTF::create(toString(reward).c_str(), FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1,
                                CCSize(100, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
	rewardT->setAnchorPoint(ccp(0,0));
	rewardT->setColor(ccc3(0xff, 0xff, 0xff));
	rewardT->cocos2d::CCNode::setPosition(ccp(580, 2));
    rewardT->setRotation(-6);
	rewardT->setPosition(ccp(visibleSize.width / 2 + 28, 215));
    addChild(rewardT);
    
    // animation
    TimedSpriteAnimation* check = TimedSpriteAnimation::create("Interfeiss/challenges/ingame_pop_up/check.plist", 3);
    check->setAnchorPoint(ccp(0, 1));
    check->setPosition(ccp(visibleSize.width / 2 + 140, 350));
    addChild(check);
    
    // icon
    char rewardStr[10];
    std::string iconStr;
    sprintf(rewardStr, "+%d", reward);
    
    if (type == WeeklyTask::RT_DIAMONDS) // diamonds
    {
    	iconStr = "Interfeiss/challenges/award_diamond.png";
    }
    else // xp
    {
    	iconStr = "Interfeiss/challenges/award_xp.png";
    }
    
    rewardSh->setString(rewardStr);
    rewardT->setString(rewardStr);
    
    
	// icon
	CCSprite* icon = CCSprite::create(iconStr.c_str());
	icon->setAnchorPoint(ccp(0.5,0.5));
    icon->setScale(0.8);
	icon->setPosition(ccp(visibleSize.width / 2 + 170, 250));
	addChild(icon);

    return true;
}

void WeeklyCompletePopUp::close()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    this->runAction(CCSequence::create(CCMoveTo::create(0.5, ccp(0, visibleSize.height)),
                                       CCCallFunc::create(this, callfunc_selector(WeeklyCompletePopUp::remove)),
                                       NULL));
}

void WeeklyCompletePopUp::remove()
{
    ((DF::LevelEnd::LevelEndScene*)this->getParent())->showNextPopup();
}

void WeeklyCompletePopUp::share()
{
    
}

void WeeklyCompletePopUp::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void WeeklyCompletePopUp::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool WeeklyCompletePopUp::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    return true;
}
