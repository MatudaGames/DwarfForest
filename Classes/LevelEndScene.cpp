//
//  LevelEndScene.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 9/29/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "LevelEndScene.h"
#include "HighScore.h"
#include "ExperienceBar.h"
#include "WeeklyCompletePopUp.h"
#include "FreeStuffPopup.h"
#include "DailyDaysCompletePopup.h"
#include "User.h"
#include "Utils.h"
#include "SupportPopup.h"


USING_NS_CC;

using namespace DF::LevelEnd;

CCScene* DF::LevelEnd::LevelEndScene::scene(Stats::GameStats& stats)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LevelEndScene *layer = LevelEndScene::create(stats);
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

LevelEndScene* DF::LevelEnd::LevelEndScene::create(Stats::GameStats& stats)
{
    LevelEndScene *pRet = new LevelEndScene();
    if (pRet && pRet->init(stats))
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

void DF::LevelEnd::LevelEndScene::addWeeklyProgress(WeeklyTask::Task task_type, int target, uint32_t progress)
{
    WeeklyChallenges& w = User::getInstance()->getWeeklyChallenges();
    if (w.isTaskCompleted(task_type, target, progress))
    {
        w.finishTask(task_type, target);
        const WeeklyTask* task = w.getTask(task_type, target);
        
        WeeklyCompletePopUp* pop = WeeklyCompletePopUp::create(task->getTaskInfo(), task->getReward(), task->getRewardType());
        pop->retain();
        popups.push_back(pop);
    }
    else
    {
        w.addTaskProgress(task_type, target, progress);
    }
}

bool DF::LevelEnd::LevelEndScene::init(Stats::GameStats& stats)
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
    // Weekly challenges
    // add progresses for tasks
    
    addWeeklyProgress(WeeklyTask::T_AVOID_METEORITS, WeeklyTask::NO_TARGET, stats.avoidedMetereoits);
    addWeeklyProgress(WeeklyTask::T_AVOID_TROLLS,    WeeklyTask::NO_TARGET, stats.avoidedTrolls);
    addWeeklyProgress(WeeklyTask::T_COLLECT_CRYSTALS, CRYSTAL_COLOR_BLUE, stats.gatheredCrystals[CRYSTAL_COLOR_BLUE]);
    addWeeklyProgress(WeeklyTask::T_COLLECT_CRYSTALS, CRYSTAL_COLOR_GREEN, stats.gatheredCrystals[CRYSTAL_COLOR_GREEN]);
    addWeeklyProgress(WeeklyTask::T_COLLECT_CRYSTALS, CRYSTAL_COLOR_RED, stats.gatheredCrystals[CRYSTAL_COLOR_RED]);
    addWeeklyProgress(WeeklyTask::T_COLLECT_CRYSTALS, CRYSTAL_COLOR_YELLOW, stats.gatheredCrystals[CRYSTAL_COLOR_YELLOW]);
    addWeeklyProgress(WeeklyTask::T_COLLECT_DIAMONDS, WeeklyTask::NO_TARGET, stats.gatheredDiamonds);
    addWeeklyProgress(WeeklyTask::T_LEAD_DWARFS, WeeklyTask::NO_TARGET, stats.rescuedDwarfs[DWARF_TYPE_FAT] + stats.rescuedDwarfs[DWARF_TYPE_TALL]);
    addWeeklyProgress(WeeklyTask::T_LEAD_DWARFS, DWARF_TYPE_FAT, stats.rescuedDwarfs[DWARF_TYPE_FAT]);
    addWeeklyProgress(WeeklyTask::T_LEAD_DWARFS, DWARF_TYPE_TALL, stats.rescuedDwarfs[DWARF_TYPE_TALL]);
    addWeeklyProgress(WeeklyTask::T_SCORE_POINTS, WeeklyTask::NO_TARGET, stats.points);
    
    if (stats.dailyChallengeCompleted)
    {
        User::getInstance()->getDailyChallenges().finishChallenge();
        
        if (User::getInstance()->getDailyChallenges().isRewardDay())
        {
            DailyDaysCompletePopup* popup = DailyDaysCompletePopup::create(User::getInstance()->getDailyChallenges().getDaysCompleted(), User::getInstance()->getDailyChallenges().getCurrentDayReward());
            popup->retain();
            popups.push_back(popup);
        }
    }
    
////    // Free Diamonds popup
    if (User::getInstance()->getFreeDmdsWeek() != Time::getYWeek()
        && User::getInstance()->getSessionLength() > 300
        && !User::getInstance()->isFollowingSocialNetwork()
        && User::getInstance()->showFreeStuffPopup())
    {
        FreeStuffPopup* popup = FreeStuffPopup::create();
        popup->retain();
        popups.push_back(popup);
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
//    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
//    CCSprite* back = CCSprite::create("background_lvlend.png");
//    back->setAnchorPoint(ccp(0,0));
//    addChild(back, 0);
    
//    CCSprite* blur = CCSprite::create("Interfeiss/bg_2_blured.png");
    CCSprite* blur = CCSprite::create("background_lvlend.png");
    blur->setAnchorPoint(ccp(0,0));
    blur->setOpacity(0);
    blur->setTag(1234);
    addChild(blur, 0);
    
//    blur->runAction(CCFadeIn::create(1));
    
    exp = ExperienceBar::create(stats);
//    exp->setAnchorPoint(ccp(0, 0));
    exp->setPosition(ccp(0, -700));
    addChild(exp);
    
    score = HighScore::create(stats.points, User::getInstance()->getStats().getHighscore() < stats.points);
    
    User::getInstance()->getStats().saveGameStats(stats);
    
    score->setPosition(40, visibleSize.height - 40);
    score->setPosition(-380, visibleSize.height -40);
    addChild(score);
    
    menu = LevelEndMenu::create();
    menu->setPosition(ccp(visibleSize.width/2,-300));
//    menu->setPosition(visibleSize.width / 2 - 20 , 200);
//    menu->setPosition(visibleSize.width / 2 - 20 + 350 , 200);
    addChild(menu);
    
    //The missions
//    missions = LevelEndMissions::create();
//    missions->setPositionY(40);
//    addChild(missions);
    
    
//    container = CCSprite::create("Interfeiss/bg_2_blured.png");
    //Is this even needed
//    container = CCSprite::create("background_lvlend.png");
//    container->setAnchorPoint(ccp(0,0));
//    container->setOpacity(0);
//    addChild(container);
    
    screen_created = true;
    
    activePopup = NULL;
    showNextPopup();
    
    return true;
}

void DF::LevelEnd::LevelEndScene::showNextPopup()
{
    if(!screen_created)
        return;
    
    if (activePopup != NULL)
    {
        activePopup->release();
        removeChild(activePopup);
        activePopup = NULL;
    }
    
    if (popups.size() > 0)
    {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        
        activePopup = popups.back();
        popups.pop_back();
        
        activePopup->setAnchorPoint(ccp(0,0));
        activePopup->setPosition(ccp(0,-visibleSize.height));
        activePopup->runAction(CCMoveTo::create(0.5, ccp(0,0)));
        addChild(activePopup, 100);
    }
    else
    {
        CCCallFuncN* aF1_sound1 = CCCallFuncN::create(this, callfuncN_selector(LevelEndScene::ShowMissions));
        CCFadeIn* aFadeIn = CCFadeIn::create(0.5f);
        CCSequence* aSeq1 = CCSequence::create(aFadeIn,aF1_sound1,NULL);
        getChildByTag(1234)->runAction(aSeq1);
    }
}

void DF::LevelEnd::LevelEndScene::ShowMissions()
{
    showLevelEnd();
    
    missions = LevelEndMissions::create();
    missions->setPositionY(40);
    addChild(missions);
}

void DF::LevelEnd::LevelEndScene::fadeInLevelEnd()
{
    container->runAction(CCFadeTo::create(0.5, 0));
}

void DF::LevelEnd::LevelEndScene::fadeOutLevelEnd()
{
    container->runAction(CCFadeTo::create(0.5, 220));
}

void DF::LevelEnd::LevelEndScene::showLevelEnd()
{
    exp->runAction(CCSequence::create(
                                      CCEaseBackOut::create(CCMoveTo::create(0.5, ccp(0, 0))),
                                      CCCallFunc::create(exp, callfunc_selector(ExperienceBar::dropTroll)),
                                      CCDelayTime::create(0.3),
                                      CCCallFunc::create(this, callfunc_selector(LevelEndScene::showSides)),
                                      NULL));
}

void DF::LevelEnd::LevelEndScene::showSides()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    score->runAction(CCEaseBackOut::create(CCMoveTo::create(0.5, ccp(visibleSize.width/2, visibleSize.height-70))));
    menu->runAction(CCEaseBackOut::create(CCMoveTo::create(0.5, ccp(visibleSize.width/2, visibleSize.height/2))));
    
    if(User::getInstance()->mFeedBackGiven == 0 && User::getInstance()->mLooseTimes>=2)
    {
        User::getInstance()->mLooseTimes=0;
        SupportPopup::create(this);
    }
    
    User::getInstance()->mLooseTimes+=1;
    
//    missions->
}

