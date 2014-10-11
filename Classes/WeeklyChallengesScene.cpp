//
//  ChallengesScene.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 7/29/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "WeeklyChallengesScene.h"
#include "AppMacros.h"
#include "StaticSceneManager.h"
#include "WeeklyChallenges.h"
#include "User.h"

USING_NS_CC;

CCScene* WeeklyChallengesScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    WeeklyChallengesScene *layer = WeeklyChallengesScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool WeeklyChallengesScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
    // enables back button on android
    this->setKeypadEnabled(true);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
    back->setAnchorPoint(ccp(0,0));
    addChild(back, 0);

	CCMenuItemImage* backButton = CCMenuItemImage::create(
														  "Interfeiss/challenges/back_arrow_v20001.png",
														  "Interfeiss/challenges/back_arrow_v20002.png",
														  this,
														  menu_selector(WeeklyChallengesScene::menuBackCallback));
	backButton->setAnchorPoint(ccp(0,0));
    
    CCMenu* backMenu = CCMenu::create(backButton, NULL);
    this->addChild(backMenu, 1);
    backMenu->setPosition(-10, -10);

    // title
    CCSprite* title = CCSprite::create("Interfeiss/challenges/challenges_title.png");
    title->setAnchorPoint(ccp(0,1));
    title->cocos2d::CCNode::setPosition(5, visibleSize.height-50);
    addChild(title, 0);

    CCMenuItemImage* dailyButton = CCMenuItemImage::create("Interfeiss/challenges/daily_inactive.png",
    		NULL, this,
    		menu_selector(WeeklyChallengesScene::menuDailyCallback));

    CCMenu* mainMenu = CCMenu::create(dailyButton, NULL);

    mainMenu->setAnchorPoint(ccp(0,0));
    mainMenu->alignItemsHorizontallyWithPadding(15);
    mainMenu->setPosition(visibleSize.width - 330, visibleSize.height - 100);
    this->addChild(mainMenu, 1);
    
    CCSprite* weeklyImage = CCSprite::create("Interfeiss/challenges/weekly_active.png");
    weeklyImage->setPosition(ccp(visibleSize.width - 140, visibleSize.height - 100));
    this->addChild(weeklyImage);

    // progress bars
    // TODO: Load weekly challenges and draw apropriate bars
    
//    const WeeklyTask* wt = User::getInstance()->getWeeklyChallenges().getTasks();
    
    /*
    bar1 = ChallengeProgressBar::create(wt[0].getTaskInfo(),
                                      wt[0].getRequirement(),
                                      User::getInstance()->getWeeklyChallenges().getTask1Progress(),
                                      wt[0].getReward(),
                                      wt[0].getRewardType());
	bar1->setPosition(150, 360);
	addChild(bar1);

	bar2 = ChallengeProgressBar::create(wt[1].getTaskInfo(),
                                      wt[1].getRequirement(),
                                      User::getInstance()->getWeeklyChallenges().getTask2Progress(),
                                      wt[1].getReward(),
                                      wt[1].getRewardType());
	bar2->setPosition(150, 240);
	addChild(bar2);

	bar3 = ChallengeProgressBar::create(wt[2].getTaskInfo(),
                                      wt[2].getRequirement(),
                                      User::getInstance()->getWeeklyChallenges().getTask3Progress(),
                                      wt[2].getReward(),
                                      wt[2].getRewardType());
	bar3->setPosition(150, 120);
	addChild(bar3);
    */
    update();
    return true;
}

void WeeklyChallengesScene::keyBackClicked()
{
    menuBackCallback(NULL);
}

void WeeklyChallengesScene::onEnter()
{
    CCLayer::onEnter();
    update();
}

void WeeklyChallengesScene::update()
{
//    const WeeklyTask* wt = User::getInstance()->getWeeklyChallenges().getTasks();
    
    /*
    bar1->update(wt[0].getTaskInfo(),
                                        wt[0].getRequirement(),
                                        User::getInstance()->getWeeklyChallenges().getTask1Progress(),
                                        wt[0].getReward(),
                                        wt[0].getRewardType());
    
	bar2->update(wt[1].getTaskInfo(),
                                        wt[1].getRequirement(),
                                        User::getInstance()->getWeeklyChallenges().getTask2Progress(),
                                        wt[1].getReward(),
                                        wt[1].getRewardType());
    
	bar3->update(wt[2].getTaskInfo(),
                                        wt[2].getRequirement(),
                                        User::getInstance()->getWeeklyChallenges().getTask3Progress(),
                                        wt[2].getReward(),
                                        wt[2].getRewardType());
    */

}

void WeeklyChallengesScene::menuBackCallback(cocos2d::CCObject *sender)
{
    CCScene* mainMenu = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
	CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, mainMenu);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void WeeklyChallengesScene::menuDailyCallback(cocos2d::CCObject *sender)
{
	// opens daily challenges
	CCScene* scene = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::DAILY_CHALLENGES);
	CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, scene);
	CCDirector::sharedDirector()->replaceScene(transition);
}

/*
ChallengeProgressBar* ChallengeProgressBar::create(std::string text, unsigned int max, unsigned int value, int reward, WeeklyTask::RewardType rewardType)
{
    ChallengeProgressBar *pGOL = new ChallengeProgressBar();
    if (pGOL && pGOL->init(text, max, value, reward, rewardType)) {
        pGOL->autorelease();
        return pGOL;
    }
    CC_SAFE_DELETE(pGOL);
    return NULL;
}

bool ChallengeProgressBar::init(std::string text, unsigned int max, unsigned int value, int rewardAmount, WeeklyTask::RewardType rewardType)
{
    setAnchorPoint(ccp(0,0));

    CCSprite* back = CCSprite::create("Interfeiss/challenges/progress_bar.png");
    back->setAnchorPoint(ccp(0,0));
    addChild(back);

    progress = CCSprite::create("Interfeiss/challenges/progress_bar_fill.png");
    progress->setAnchorPoint(ccp(0,0));
    progress->setPosition(ccp(14,46));

    progress->setTextureRect(CCRect(0, 0,
                                    progress->getTexture()->getContentSize().width*((float)value / max),
                                    progress->getTexture()->getContentSize().height));

    addChild(progress);

    titleSh = CCLabelTTF::create(text.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
    		CCSize(700, 70), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    titleSh->setAnchorPoint(ccp(0,0));
    titleSh->cocos2d::CCNode::setPosition(ccp(25, 85));
    titleSh->setColor(ccc3(0, 0, 0));
    titleSh->enableShadow(CCSize(1,1), 0.5, 3, true);
    addChild(titleSh);

    title = CCLabelTTF::create(text.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
        		CCSize(700, 70), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
	title->setAnchorPoint(ccp(0,0));
	title->cocos2d::CCNode::setPosition(ccp(25, 85));
	title->setColor(ccc3(0xFF, 0xBA, 0x3D));
	addChild(title);

	char pointsStr[20];
	sprintf(pointsStr, "%d/%d", value, max);

    pointsSh = CCLabelTTF::create(pointsStr, FONT_SKRANJI, TITLE_FONT_SIZE*0.6,
    		CCSize(560, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
    pointsSh->setAnchorPoint(ccp(0,0));
    pointsSh->cocos2d::CCNode::setPosition(ccp(14, 39));
    pointsSh->setColor(ccc3(0, 0, 0));
    pointsSh->enableShadow(CCSize(1,1), 0.5, 3, true);
    addChild(pointsSh);

    points = CCLabelTTF::create(pointsStr, FONT_SKRANJI, TITLE_FONT_SIZE*0.6,
        		CCSize(560, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	points->setAnchorPoint(ccp(0,0));
	points->cocos2d::CCNode::setPosition(ccp(14, 39));
	addChild(points);

    char rewardStr[10];
    std::string iconStr;
    if (rewardType == WeeklyTask::RT_DIAMONDS) // diamonds
    {
    	sprintf(rewardStr, "x%d", rewardAmount);
    	iconStr = "Interfeiss/challenges/award_diamond.png";
    }
    else // xp
    {
    	sprintf(rewardStr, "+%d", rewardAmount);
    	iconStr = "Interfeiss/challenges/award_xp.png";
    }

    rewardSh = CCLabelTTF::create(rewardStr, FONT_SKRANJI, TITLE_FONT_SIZE*0.5,
    		CCSize(100, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
    rewardSh->setAnchorPoint(ccp(0,0));
    rewardSh->cocos2d::CCNode::setPosition(ccp(580, 2));
    rewardSh->setColor(ccc3(0, 0, 0));
    rewardSh->enableShadow(CCSize(1,1), 0.5, 3, true);
    addChild(rewardSh);

    reward = CCLabelTTF::create(rewardStr, FONT_SKRANJI, TITLE_FONT_SIZE*0.5,
        		CCSize(100, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	reward->setAnchorPoint(ccp(0,0));
	reward->setColor(ccc3(0xff, 0xba, 0x3d));
	reward->cocos2d::CCNode::setPosition(ccp(580, 2));
	addChild(reward);

	// icon
	icon = CCSprite::create(iconStr.c_str());
	icon->setAnchorPoint(ccp(0.5,0.5));
	icon->setPosition(ccp(635, 65));
	addChild(icon);

    update(text, max, value, rewardAmount, rewardType);
    
    return true;
}

void ChallengeProgressBar::update(std::string text,
                                  unsigned int max,
                                  unsigned int value,
                                  int rewardAmount,
                                  WeeklyTask::RewardType rewardType)
{
    
    progress->setTextureRect(CCRect(0, 0,
                                    progress->getTexture()->getContentSize().width*((float)value / max),
                                    progress->getTexture()->getContentSize().height));
    
    titleSh->setString(text.c_str());
    title->setString(text.c_str());
    
	char pointsStr[20];
	sprintf(pointsStr, "%d/%d", value, max);
    
    pointsSh->setString(pointsStr);
    points->setString(pointsStr);
    
    char rewardStr[10];
    std::string iconStr;
    if (rewardType == WeeklyTask::RT_DIAMONDS) // diamonds
    {
    	sprintf(rewardStr, "x%d", rewardAmount);
    	iconStr = "Interfeiss/challenges/award_diamond.png";
    }
    else // xp
    {
    	sprintf(rewardStr, "+%d", rewardAmount);
    	iconStr = "Interfeiss/challenges/award_xp.png";
    }
    
    rewardSh->setString(rewardStr);
    reward->setString(rewardStr);
    
	// icon
    if (icon != NULL)
    {
        removeChild(icon);
    }
	icon = CCSprite::create(iconStr.c_str());
	icon->setAnchorPoint(ccp(0.5,0.5));
	icon->setPosition(ccp(635, 65));
	addChild(icon);
}
 */

