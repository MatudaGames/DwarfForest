//
//  ChallengesScene.h
//  DwarfForest
//
//  Created by Agris Dumins on 7/29/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>
#include <string.h>
#include "WeeklyChallenges.h"

/*
class ChallengeProgressBar: public cocos2d::CCLayer
{
public:
    static ChallengeProgressBar* create(std::string text, unsigned int max, unsigned int value, int rewardAmount, WeeklyTask::RewardType rewardType);

    virtual bool init(std::string text, unsigned int max, unsigned int value, int rewardAmount, WeeklyTask::RewardType rewardType);
    
    void update(std::string text, unsigned int max, unsigned int value, int rewardAmount, WeeklyTask::RewardType rewardType);

private:

    cocos2d::CCSprite* progress;
    cocos2d::CCLabelTTF* titleSh;
    cocos2d::CCLabelTTF* title;
    cocos2d::CCLabelTTF* pointsSh;
    cocos2d::CCLabelTTF* points;
    cocos2d::CCLabelTTF* rewardSh;
    cocos2d::CCLabelTTF* reward;
    cocos2d::CCSprite* icon;
};
*/

class WeeklyChallengesScene: public cocos2d::CCLayer
{
public:
	
    CREATE_FUNC(WeeklyChallengesScene);
    
    virtual bool init();
	
    static cocos2d::CCScene* scene();
	
	void menuBackCallback(CCObject* sender);
    void menuDailyCallback(CCObject* sender);
    
    void onEnter();
    void update();
    void keyBackClicked();
    
    /*
    ChallengeProgressBar* bar1;
    ChallengeProgressBar* bar2;
    ChallengeProgressBar* bar3;
    */
};
