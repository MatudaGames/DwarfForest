//
//  DailyChallenges.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 9/1/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "DailyChallenges.h"
#include "User.h"
#include "Utils.h"

DailyChallenges::DailyChallenges()
{
    bar1Color = (CrystalColor)cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("dailyChallenges.bar1Color", CRYSTAL_COLOR_COUNT);
    bar2Color = (CrystalColor)cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("dailyChallenges.bar2Color", CRYSTAL_COLOR_COUNT);
    bar3Color = (CrystalColor)cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("dailyChallenges.bar3Color", CRYSTAL_COLOR_COUNT);
    
    bar1Count = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("dailyChallenges.bar1Count", 0);
    bar2Count = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("dailyChallenges.bar2Count", 0);
    bar3Count = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("dailyChallenges.bar3Count", 0);
    
    reward = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("dailyChallenges.reward", 0);
    daysCompleted = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("dailyChallenges.daysCompleted", 0);
    lastCompletion = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("dailyChallenges.lastCompletion", 0);
    lastSpin = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("dailyChallenges.lastSpin", 0);
}

void DailyChallenges::setBar1Color(CrystalColor color)
{
    this->bar1Color = color;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.bar1Color", color);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void DailyChallenges::setBar2Color(CrystalColor color)
{
    this->bar2Color = color;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.bar2Color", color);
}

void DailyChallenges::setBar3Color(CrystalColor color)
{
    this->bar3Color = color;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.bar3Color", color);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void DailyChallenges::setBar1Count(uint32_t count)
{
    this->bar1Count = count;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.bar1Count", count);
}

void DailyChallenges::setBar2Count(uint32_t count)
{
    this->bar2Count = count;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.bar2Count", count);
}

void DailyChallenges::setBar3Count(uint32_t count)
{
    this->bar3Count = count;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.bar3Count", count);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void DailyChallenges::setReward(uint32_t reward)
{
    this->reward = reward;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.reward", reward);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void DailyChallenges::setDaysCompleted(uint32_t completed)
{
    this->daysCompleted = completed;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.daysCompleted", completed);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void DailyChallenges::setLastSpinTime(time_t t)
{
    lastSpin = t;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.lastSpin", t);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void DailyChallenges::setLastCompletionTime(time_t t)
{
    lastCompletion = t;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.lastCompletion", t);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void DailyChallenges::checkDayCounter()
{
    if(User::getInstance()->FirstSessionDone==1)
        return;
    
    // if completed more then one day away resets to 0
    if (Time::dayDifference(getLastCompletionTime()) > 1)
    {
        User::getInstance()->getDailyChallenges().setDaysCompleted(0);
    }
}

void DailyChallenges::getNextDayReward(uint32_t day, uint32_t& rewardDay, uint32_t& diamonds)
{
    if (day < 5){
        rewardDay = 5;
        diamonds = 5;
    } else if (day < 10) {
        rewardDay = 10;
        diamonds = 8;
    } else if (day < 15) {
        rewardDay = 15;
        diamonds = 12;
    } else if (day < 20) {
        rewardDay = 20;
        diamonds = 15;
    } else if (day < 25) {
        rewardDay = 25;
        diamonds = 20;
    }
}

uint32_t DailyChallenges::getCurrentDayReward()
{
    uint32_t day = getDaysCompleted();
    if (day <= 5){
        return 5;
    } else if (day <= 10) {
        return 8;
    } else if (day <= 15) {
        return 12;
    } else if (day <= 20) {
        return 15;
    } else if (day <= 25) {
        return 20;
    }
    return 0;
}

bool DailyChallenges::isTodaysCompleted()
{
    return Time::isToday(getLastCompletionTime());
}

bool DailyChallenges::isChallengeRequirementsMet(std::map<CrystalColor, int>& gathered)
{
    std::cout << "Check if daily compl ->\n";
    
    if (User::getInstance()->getDailyChallenges().isTodaysCompleted() == false
        && Time::isToday(User::getInstance()->getDailyChallenges().getLastSpinTime()))
    {
        std::cout << "checking >\n";
        std::map<CrystalColor, int> dailyChallenge;
        
        dailyChallenge[CRYSTAL_COLOR_BLUE] = 0;
        dailyChallenge[CRYSTAL_COLOR_GREEN] = 0;
        dailyChallenge[CRYSTAL_COLOR_RED] = 0;
        dailyChallenge[CRYSTAL_COLOR_YELLOW] = 0;
        
        dailyChallenge[User::getInstance()->getDailyChallenges().getBar1Color()] +=
			User::getInstance()->getDailyChallenges().getBar1Count();
        dailyChallenge[User::getInstance()->getDailyChallenges().getBar2Color()] +=
			User::getInstance()->getDailyChallenges().getBar2Count();
        dailyChallenge[User::getInstance()->getDailyChallenges().getBar3Color()] +=
			User::getInstance()->getDailyChallenges().getBar3Count();
        
        std::cout << "challenge blue(" << dailyChallenge[CRYSTAL_COLOR_BLUE] << ")"
            << "green(" << dailyChallenge[CRYSTAL_COLOR_GREEN] << ")"
            << "red(" << dailyChallenge[CRYSTAL_COLOR_RED] << ")"
            << "yellow(" << dailyChallenge[CRYSTAL_COLOR_YELLOW] << ")\n";
        std::cout << "gathered  blue(" << gathered[CRYSTAL_COLOR_BLUE] << ")"
            << "green(" << gathered[CRYSTAL_COLOR_GREEN] << ")"
            << "red(" << gathered[CRYSTAL_COLOR_RED] << ")"
            << "yellow(" << gathered[CRYSTAL_COLOR_YELLOW] << ")\n";
        
        if (gathered[CRYSTAL_COLOR_BLUE] >= dailyChallenge[CRYSTAL_COLOR_BLUE]
            && gathered[CRYSTAL_COLOR_GREEN] >= dailyChallenge[CRYSTAL_COLOR_GREEN]
            && gathered[CRYSTAL_COLOR_RED] >= dailyChallenge[CRYSTAL_COLOR_RED]
            && gathered[CRYSTAL_COLOR_YELLOW] >= dailyChallenge[CRYSTAL_COLOR_YELLOW])
        {
            std::cout << "return true;\n";
            return true;
        }
    }
    
    std::cout << "return false;\n";
    return false;
}

void DailyChallenges::ResetAllData()
{
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.reward", 0);
    reward = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.daysCompleted", 0);
    daysCompleted = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.lastCompletion", 0);
    lastCompletion = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.lastSpin", 0);
    lastSpin = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.bar1Count", 0);
    bar1Count = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.bar2Count", 0);
    bar2Count = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("dailyChallenges.bar3Count", 0);
    bar3Count = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

bool DailyChallenges::isRequiredCrystal(CrystalColor color)
{
    if (User::getInstance()->getDailyChallenges().getBar1Color() == color
        || User::getInstance()->getDailyChallenges().getBar2Color() == color
        || User::getInstance()->getDailyChallenges().getBar3Color() == color)
    {
        return true;
    }
    return false;
}

bool DailyChallenges::isRewardDay()
{
    if (daysCompleted == 5
        || daysCompleted == 10
        || daysCompleted == 15
        || daysCompleted == 20
        || daysCompleted == 25)
        return true;
    else
        return false;
}

void DailyChallenges::finishChallenge()
{
    // check days - vai nav jaaseto
    checkDayCounter();
    
    uint32_t day = 0;
    uint32_t dayReward = 0;
    getNextDayReward(getDaysCompleted(), day, dayReward);
    
    if (getDaysCompleted() >= 25)
    {
        setDaysCompleted(1);
    }
    else
    {
        setDaysCompleted(getDaysCompleted()+1);
    }
    setLastCompletionTime(time(0));
//    User::getInstance()->addDiamonds(getReward());
    
    // rewards par dienaam..
    if (getDaysCompleted() == day)
    {
        User::getInstance()->addDiamonds(dayReward);
    }
}
