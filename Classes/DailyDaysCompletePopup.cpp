//
//  DailyDaysCompletePopup.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 24/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "DailyDaysCompletePopup.h"
#include "AppMacros.h"
#include "Utils.h"
#include "TimedSpriteAnimation.h"
#include "LevelEndScene.h"

USING_NS_CC;

DailyDaysCompletePopup* DailyDaysCompletePopup::create(uint32_t days, uint32_t reward)
{
    DailyDaysCompletePopup *pGOL = new DailyDaysCompletePopup();
    if (pGOL && pGOL->init(days, reward)) {
        pGOL->autorelease();
        return pGOL;
    }
    CC_SAFE_DELETE(pGOL);
    return NULL;
}

bool DailyDaysCompletePopup::init(uint32_t days, uint32_t reward)
{
    if (!CCLayer::init()) return false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/challenges/complete_popup/dont_leave.png");
    back->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
    addChild(back);
    
    
    CCMenuItemImage* closeBt = CCMenuItemImage::create("Interfeiss/level_end/level_up_popup/x_btn0001.png",
                                                       "Interfeiss/level_end/level_up_popup/x_btn0002.png",
                                                       this, menu_selector(DailyDaysCompletePopup::close));
    
    CCMenu* closeMenu = CCMenu::create(closeBt, NULL);
    closeMenu->setPosition(ccp(730, 530));
    addChild(closeMenu);
    
    CCMenuItemImage* shareBt =
    CCMenuItemImage::create("Interfeiss/challenges/complete_popup/share_btn_still0001.png",
                            "Interfeiss/challenges/complete_popup/share_btn_still0002.png",
                            this, menu_selector(DailyDaysCompletePopup::share));
    CCMenu* shareMenu = CCMenu::create(shareBt, NULL);
    shareMenu->setPositionY(200);
    shareMenu->setPositionX(visibleSize.width / 2 + 0);
    addChild(shareMenu);
    
    
    CCLabelTTF* titleTxt = CCLabelTTF::create(("Daily challenge completed " + toString(days) + " days in a row!").c_str(),
                                              FONT_SKRANJI_BOLD,
                                              TITLE_FONT_SIZE*0.6, CCSize(320, 180),
                                              kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
    titleTxt->setColor(ccc3(0x3E, 0x16, 0x04));
    titleTxt->setRotation(-2);
    titleTxt->setPosition(ccp(visibleSize.width / 2+80, visibleSize.height / 2 + 70));
    addChild(titleTxt);
    
    // title
    CCLabelTTF* levelTxt = CCLabelTTF::create("reward", FONT_SKRANJI_BOLD,
                                              TITLE_FONT_SIZE*0.6, CCSize(130, 120),
                                              kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
    levelTxt->setColor(ccc3(0x4F, 0x98, 0x8B));
    levelTxt->setRotation(-6);
    levelTxt->setPosition(ccp(visibleSize.width / 2 + 40, visibleSize.height / 2 - 70));
    addChild(levelTxt);
    
    // reward
    CCLabelTTF* rewardSh = CCLabelTTF::create(toString(reward).c_str(), FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1,
                                              CCSize(100, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    rewardSh->setAnchorPoint(ccp(0,0));
    rewardSh->cocos2d::CCNode::setPosition(ccp(580, 2));
    rewardSh->setColor(ccc3(0xff, 0xff, 0xff));
    rewardSh->enableShadow(CCSize(3,3), 0.5, 3, true);
    rewardSh->setRotation(-6);
    rewardSh->setPosition(ccp(visibleSize.width / 2 - 70, 245));
    addChild(rewardSh);
    
    CCLabelTTF* rewardT = CCLabelTTF::create(toString(reward).c_str(), FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1,
                                             CCSize(100, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
	rewardT->setAnchorPoint(ccp(0,0));
	rewardT->setColor(ccc3(0xff, 0xff, 0xff));
	rewardT->cocos2d::CCNode::setPosition(ccp(580, 2));
    rewardT->setRotation(-6);
	rewardT->setPosition(ccp(visibleSize.width / 2 - 70, 245));
    addChild(rewardT);
    
    // animation
    TimedSpriteAnimation* check = TimedSpriteAnimation::create("Interfeiss/challenges/ingame_pop_up/check.plist", 3);
    check->setAnchorPoint(ccp(0, 1));
    check->setPosition(ccp(visibleSize.width / 2 + 190, 460));
    addChild(check);
    
    // icon
    char rewardStr[10];
    std::string iconStr;
    sprintf(rewardStr, "+%d", reward);
    
    iconStr = "Interfeiss/challenges/award_diamond.png";
    
    rewardSh->setString(rewardStr);
    rewardT->setString(rewardStr);
    
    // icon
	CCSprite* icon = CCSprite::create(iconStr.c_str());
	icon->setAnchorPoint(ccp(0.5,0.5));
    icon->setScale(0.8);
	icon->setPosition(ccp(visibleSize.width / 2 + 80, 280));
	addChild(icon);
    
    return true;
}

void DailyDaysCompletePopup::close()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    this->runAction(CCSequence::create(CCMoveTo::create(0.5, ccp(0, visibleSize.height)),
                                       CCCallFunc::create(this, callfunc_selector(DailyDaysCompletePopup::remove)),
                                       NULL));
}

void DailyDaysCompletePopup::share()
{
    // TODO : add sharing functionality
}

void DailyDaysCompletePopup::remove()
{
    ((DF::LevelEnd::LevelEndScene*)this->getParent())->showNextPopup();
}
