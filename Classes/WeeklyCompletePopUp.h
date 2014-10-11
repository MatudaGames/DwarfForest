//
//  WeeklyCompletePopUp.h
//  DwarfForest
//
//  Created by Agris Dumins on 10/18/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

#include "WeeklyChallenges.h"

class WeeklyCompletePopUp : public cocos2d::CCLayer
{
public:
	static WeeklyCompletePopUp* create(std::string text, uint32_t reward, WeeklyTask::RewardType type);
    bool init(std::string text, uint32_t reward, WeeklyTask::RewardType type);
    
    void close();
    void share();
    void remove();
    
    void onEnter();
    void onExit();
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
};
