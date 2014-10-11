//
//  DailyChallenges.h
//  DwarfForest
//
//  Created by Agris Dumins on 9/1/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Crystal.h"

#include <stdint.h>
#include <ctime>

class DailyChallenges
{
public:
    DailyChallenges();
    
    CrystalColor getBar1Color() { return bar1Color; }
    CrystalColor getBar2Color() { return bar2Color; }
    CrystalColor getBar3Color() { return bar3Color; }
    uint32_t getBar1Count() { return bar1Count; }
    uint32_t getBar2Count() { return bar2Count; }
    uint32_t getBar3Count() { return bar3Count; }
    
    uint32_t getReward() { return reward; }
    uint32_t getDaysCompleted() { return daysCompleted; }
    time_t getLastSpinTime() { return lastSpin; }
    time_t getLastCompletionTime() { return lastCompletion; }
    
    void ResetAllData();
    
    bool isChallengeRequirementsMet(std::map<CrystalColor, int>& gathered);
    bool isRequiredCrystal(CrystalColor color);
    
    void setBar1Color(CrystalColor color);
    void setBar2Color(CrystalColor color);
    void setBar3Color(CrystalColor color);
    void setBar1Count(uint32_t count);
    void setBar2Count(uint32_t count);
    void setBar3Count(uint32_t count);
    
    void setReward(uint32_t reward);
    void setDaysCompleted(uint32_t completed);
    void setLastSpinTime(time_t t);
    void setLastCompletionTime(time_t t);
    
    void checkDayCounter();
    void finishChallenge();
    void getNextDayReward(uint32_t day, uint32_t& rewardDay, uint32_t& diamonds);
    uint32_t getCurrentDayReward();
    bool isRewardDay();
    bool isTodaysCompleted();
    
private:
    CrystalColor bar1Color;
    uint32_t bar1Count;
    CrystalColor bar2Color;
    uint32_t bar2Count;
    CrystalColor bar3Color;
    uint32_t bar3Count;
    
    uint32_t reward;
    
    // date when spinned
    time_t lastSpin;
    
    // date when last completed
    time_t lastCompletion;
    
    uint32_t daysCompleted;
    
};
