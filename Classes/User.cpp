//
//  User.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 7/17/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "User.h"
#include "Utils.h"
#include "Crystal.h"
#include "Levels.h"
#include <cocos2d.h>
#include <stdlib.h>

#include "Levels.h"

static User* instance = NULL;

User* User::getInstance()
{
	if (instance == NULL)
    {
        instance = new User();
    }
    return instance;
}

User::User()
{
    //Just some global flag for futhure stuff
    DWARF_FOREST_2 = true;//Use the new stuff - and remove the old not needed parts
    
    mLooseTimes = 0;
    
//    mCurrentMissionLevel = 0;//No progress
    
    mDebugSpeed_dwarf = 1.0f;
    mDebugSpeed_troll = 1.2f;
    
    mNewMaps = false;
    
    mNewSplitCaves = false;
    
    mNewMissionBuild = true;
    mNewMissionProgress = 1;
    
    mDynamicTrolls = false;
    
    //For now disabled
    mSpecialMissionBuild = false;//was true when enabled
    mSpecialMissionProgress = 0;
    mShowNewPart = false;
    
    //......................................................................
    // The new stuff !!!
    
    mActiveSpells = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("Spells_Active","0,0");
    mBoghtSpells = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("Spells_Bought","");// Nothing bought at start !!!
    
    // The Powers
    mBoghtPowers = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("Powers_Bought","");
    mPowerInfo = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("Powers_Info","");//On what level is what power !!!
    
    //......................................................................
    
    
    _tutorial_small_4 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_small_4", 0);
    
    FirstSessionDone = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("userFirstSession", 0);
    
    mCurrentMissionLevel = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("MISSION_LVL", 1);
    
    _timesPlayed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("GamePlayedTimes", 0);
    
    _level = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("userLevel", 0);
    _experience = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("userExperience", 0);
    _diamonds = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("userDiamonds", 500);
    _dwarfs = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("userDwarfs", 0);
    
    _crystals = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("userCrystals", 500000);
    
    _showFreeStuffPopup = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("showFreeStuffPopup", 1);
    _showRateGamePopup = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("showRateGamePopup", 1);
    
    speedUp_Enabled = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("debug_speedOn", 1);
    speedUp_StartMin_1 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("debug_speedStartMin_1", 2);
    speedUp_StartMin_1_extra = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("debug_speedStartMin_1_extra", 1);
    
    speedUp_StartMin_2 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("debug_speedStartMin_2", 3);
    speedUp_StartMin_2_extra = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("debug_speedStartMin_2_extra", 2);
    
    speedUp_StartMin_3 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("debug_speedStartMin_3", 5);
    speedUp_StartMin_3_extra = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("debug_speedStartMin_3_extra", 4);
    
    _tutorial = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_completed", 0);
    
    _activationCount = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("activationCount", 0);
    _rateGameDay = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("rateGameDay", 0);
    _storeOfferDay = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("storeOfferDay", 0);
    _freeDmdsWeek = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("freeDmdsWeek", 0);
    _tutorial_small_1 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_small_1", 0);
    _tutorial_small_2 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_small_2", 0);
    _tutorial_small_3 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_small_3", 0);
    
    _tutorial_small_4 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_small_4", 0);
    _tutorial_small_5 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_small_5", 0);
    _tutorial_small_6 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_small_6", 0);
    _tutorial_small_7 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_small_7", 0);
    _tutorial_small_8 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_small_8", 0);
    _tutorial_small_9 = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_small_9", 0);
    
    _debugButtonsState = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("debug_buttons_state", 0);
    
    score_multi_lvl = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("scoreMulti",1);
    
    mEasterEggCount = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("event_easter_eggs",0);
    
    mFeedBackGiven = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("feedback_beta",0);
    
    mEasterActive = false;
    
    // The not needed stuff anymore !!!
//    machine_stamp_lvl = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("machineStampLevel", 0);
//    machine_enemies_lvl = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("machineEnemiesLevel", 0);
//    machine_ghost_lvl = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("machineGhostLevel", 0);
//    machine_plants_lvl = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("machinePlantLevel", 0);

    
//    if(mSpecialMissionBuild){
//        cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("ActiveMission", 9);
//        getMissionManager().mCurrentActiveMission = 9;
//    }
    
    //Special stuff - do not start tutorial if this is active
//    if(mNewMissionBuild){
//        _tutorial = 1;
//    }
    
    _tutorial = 1;//Always no tutorial
    
//    CCLOG("Diamonds %i:",_diamonds);
    
    mSpecial_10_Mission = false;
    mSpecial_11_Mission = false;
    mSpecial_12_Mission = false;
    mSpecial_13_Mission = false;
    mSpecial_14_Mission = false;
    
    mSpecial_16_Mission = false;
    mSpecial_17_Mission = false;
    mSpecial_18_Mission = false;
    
    mSpecial_19_Mission = false;
    mSpecial_20_Mission = false;
    
    mSpecial_21_Mission = false;
    mSpecial_22_Mission = false;
    mSpecial_23_Mission = false;
}

void User::ResetData()
{
    //Some small stuff !!!
    getMissionManager().ResetAllStats();
    getDailyChallenges().ResetAllData();
    getWeeklyChallenges().ClearAllData();
    //========================================================
    
    mDynamicTrolls = false;
    
    mCurrentMissionLevel = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("MISSION_LVL",1);
    
    _debugButtonsState = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_buttons_state", 0);
    
    _timesPlayed = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("GamePlayedTimes", 0);
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userFirstSession", 0);
    FirstSessionDone = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userLevel", 0);
    _level = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userExperience", 0);
    _experience = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userDiamonds", 25);
    _diamonds = 25;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userDwarfs", 0);
    _dwarfs = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userCrystals", 0);
    _crystals = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("showFreeStuffPopup", 1);
    _showFreeStuffPopup = 1;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("showRateGamePopup", 1);
    _showRateGamePopup = 1;
    
    /*
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machineStampLevel", 0);
    machine_stamp_lvl = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machineEnemiesLevel", 0);
    machine_enemies_lvl = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machineGhostLevel", 0);
    machine_ghost_lvl = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machinePlantLevel", 0);
    machine_plants_lvl = 0;
    */
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedOn", 1);
    speedUp_Enabled = 1;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_1", 2);
    speedUp_StartMin_1 = 2;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_1_extra", 1);
    speedUp_StartMin_1_extra = 1;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_2", 3);
    speedUp_StartMin_2 = 3;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_2_extra", 2);
    speedUp_StartMin_2_extra = 2;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_3", 5);
    speedUp_StartMin_3 = 5;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_3_extra", 4);
    speedUp_StartMin_3_extra = 4;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_completed", 0);
    _tutorial = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("activationCount", 0);
    _activationCount = 0;
    
    _rateGameDay = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("rateGameDay", 0);
    
    _storeOfferDay = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("storeOfferDay", 0);
    
    _freeDmdsWeek = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("freeDmdsWeek", 0);
    
    _tutorial_small_1 = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_1", 0);
    
    _tutorial_small_2 = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_2", 0);
    
    _tutorial_small_3 = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_3", 0);
    
    _tutorial_small_4 = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_4", 0);
    _tutorial_small_5 = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_5", 0);
    _tutorial_small_6 = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_6", 0);
    _tutorial_small_7 = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_7", 0);
    _tutorial_small_8 = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_8", 0);
    _tutorial_small_9 = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_9", 0);
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("scoreMulti",1);
    
    mEasterEggCount = 0;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("event_easter_eggs",0);
    
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

bool User::isEasterActive()
{
    return mEasterActive;
    
    /*
    if(mEasterActive)
        return true;
    
    bool aFlag = false;
    
    //Try to check current data !!!
    mEasterActive = true;
    aFlag = true;
    mEasterTimer = 1000;
    
    return aFlag;
    */
}

User::~User()
{
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::addExperience(uint32_t exp)
{
    _experience += exp;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userExperience", _experience);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::addDwarfs(uint32_t dwarf)
{
    _dwarfs += dwarf;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userDwarfs", _dwarfs);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::finishTutorial()
{
    _tutorial = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_completed", _tutorial);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::finishTutorial_Small_1()
{
//    return;
    _tutorial_small_1 = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_1", _tutorial_small_1);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::finishTutorial_Small_2()
{
//    return;
    _tutorial_small_2 = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_2", _tutorial_small_2);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::finishTutorial_Small_3()
{
//    return;
    _tutorial_small_3 = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_3", _tutorial_small_3);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::finishTutorial_Small_4()
{
    //    return;
    _tutorial_small_4 = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_4", _tutorial_small_4);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}
void User::finishTutorial_Small_5()
{
    //    return;
    _tutorial_small_5 = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_5", _tutorial_small_5);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}
void User::finishTutorial_Small_6()
{
    //    return;
    _tutorial_small_6 = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_6", _tutorial_small_6);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}
void User::finishTutorial_Small_7()
{
    //    return;
    _tutorial_small_7 = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_7", _tutorial_small_7);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}
void User::finishTutorial_Small_8()
{
    //    return;
    _tutorial_small_8 = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_8", _tutorial_small_8);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}
void User::finishTutorial_Small_9()
{
    //    return;
    _tutorial_small_9 = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_small_9", _tutorial_small_9);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::ChangeDebugButtons()
{
    if(_debugButtonsState == 1)
        _debugButtonsState = 0;
    else
        _debugButtonsState = 1;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_buttons_state", _debugButtonsState);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::increaseLevel()
{
    if (getLevel() >= Levels::getInstance()->getMaxLevel()-1)
    {
        return;
    }
    
    _level++;
    
    /*
    if(_level>2 && machine_enemies_lvl==0)
    {
        //Unlock the tesla if not done already !!!
        machine_enemies_lvl+=1;
        setMachine_EnemiesLvl(machine_enemies_lvl);
    }
    if(_level>7 && machine_ghost_lvl==0)
    {
        machine_ghost_lvl+=1;
        setMachine_GhostLvl(machine_ghost_lvl);
    }
    if(_level>12 && machine_stamp_lvl==0)
    {
        machine_stamp_lvl+=1;
        setMachine_StampLvl(machine_stamp_lvl);
    }
    if(_level>4 && machine_plants_lvl==0)
    {
        machine_plants_lvl+=1;
        setMachine_PlantLvl(machine_plants_lvl);
    }
    */
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userLevel", _level);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

uint32_t User::getPossibleUgrades()
{
//    int aCrystals = getCrystals();
//    int aDiamonds = getDiamonds();
    
    //Now check if we can update anything or unlock???
    /*
    int aNoEnemieLvL = getMachine_EnemiesLvl();
    int aGhostLvl = getMachine_GhostLvl();
    int aPlantLvl = getMachine_PlantLvl();
    int aStampLvl = getMachine_StampLvl();
    */
    
    int aPriceToUpgrade = 0;
    int aPossibleUpgrades = 0;
    
    //Should check all possible upgrades !!!
    int aStartCrystals = getCrystals();
    int aStartDiamonds = getDiamonds();
    
    //Lets check the no enemies stuff
    /*
    if(aNoEnemieLvL>0 && aNoEnemieLvL<6)
    {
        for(int i=1;i<5;i++)
        {
            aPriceToUpgrade = 0;
            //
            if(aNoEnemieLvL<=i)
            {
                if(i==1) aPriceToUpgrade = 2000;
                else if(i==2) aPriceToUpgrade = 8000;
                else if(i==3) aPriceToUpgrade = 20000;
                else if(i==4) aPriceToUpgrade = 50000;
            }
            
            if(aPriceToUpgrade>0 && aPriceToUpgrade<=aStartCrystals)
            {
                aStartCrystals-=aPriceToUpgrade;
                aPossibleUpgrades++;
            }
        }
    }
    
    //Lets check the ghost stuff
    if(aGhostLvl>0 && aGhostLvl<6)
    {
        for(int i=1;i<5;i++)
        {
            aPriceToUpgrade = 0;
            //
            if(aGhostLvl<=i)
            {
                if(i==1) aPriceToUpgrade = 2000;
                else if(i==2) aPriceToUpgrade = 8000;
                else if(i==3) aPriceToUpgrade = 20000;
                else if(i==4) aPriceToUpgrade = 50000;
            }
            
            if(aPriceToUpgrade>0 && aPriceToUpgrade<=aStartCrystals)
            {
                aStartCrystals-=aPriceToUpgrade;
                aPossibleUpgrades++;
            }
        }
    }
    
    //Lets check the plant stuff
    if(aPlantLvl>0 && aPlantLvl<6)
    {
        for(int i=1;i<5;i++)
        {
            aPriceToUpgrade = 0;
            //
            if(aPlantLvl<=i)
            {
                if(i==1) aPriceToUpgrade = 2000;
                else if(i==2) aPriceToUpgrade = 8000;
                else if(i==3) aPriceToUpgrade = 20000;
                else if(i==4) aPriceToUpgrade = 50000;
            }
            
            if(aPriceToUpgrade>0 && aPriceToUpgrade<=aStartCrystals)
            {
                aStartCrystals-=aPriceToUpgrade;
                aPossibleUpgrades++;
            }
        }
    }
    
    //Lets check the plant stuff
    if(aStampLvl>0 && aStampLvl<6)
    {
        for(int i=1;i<5;i++)
        {
            aPriceToUpgrade = 0;
            //
            if(aStampLvl<=i)
            {
                if(i==1) aPriceToUpgrade = 2000;
                else if(i==2) aPriceToUpgrade = 8000;
                else if(i==3) aPriceToUpgrade = 20000;
                else if(i==4) aPriceToUpgrade = 50000;
            }
            
            if(aPriceToUpgrade>0 && aPriceToUpgrade<=aStartCrystals)
            {
                aStartCrystals-=aPriceToUpgrade;
                aPossibleUpgrades++;
            }
        }
    }
    */
    
    
    //The old functional for read all possible stuff
    /*
    if(aNoEnemieLvL>0 && aNoEnemieLvL<6)
    {
        if(aNoEnemieLvL==1)
            aPriceToUpgrade = 2000;
        else if(aNoEnemieLvL==2)
            aPriceToUpgrade = 8000;
        else if(aNoEnemieLvL==3)
            aPriceToUpgrade = 20000;
        else if(aNoEnemieLvL==4)
            aPriceToUpgrade = 50000;
        
        if(aPriceToUpgrade>0 && aPriceToUpgrade<=aCrystals)
        {
            aCrystals-=aPriceToUpgrade;
            aPossibleUpgrades++;
        }
    }
    
    if(aGhostLvl>0 && aGhostLvl<6)
    {
        if(aGhostLvl==1)
            aPriceToUpgrade = 2000;
        else if(aGhostLvl==2)
            aPriceToUpgrade = 8000;
        else if(aGhostLvl==3)
            aPriceToUpgrade = 20000;
        else if(aGhostLvl==4)
            aPriceToUpgrade = 50000;
        
        if(aPriceToUpgrade>0 && aPriceToUpgrade<=aCrystals)
        {
            aCrystals-=aPriceToUpgrade;
            aPossibleUpgrades++;
        }
    }
    
    if(aPlantLvl>0 && aPlantLvl<6)
    {
        if(aPlantLvl==1)
            aPriceToUpgrade = 2000;
        else if(aPlantLvl==2)
            aPriceToUpgrade = 8000;
        else if(aPlantLvl==3)
            aPriceToUpgrade = 20000;
        else if(aPlantLvl==4)
            aPriceToUpgrade = 50000;
        
        if(aPriceToUpgrade>0 && aPriceToUpgrade<=aCrystals)
        {
            aCrystals-=aPriceToUpgrade;
            aPossibleUpgrades++;
        }
    }
    
    if(aStampLvl>0 && aStampLvl<6)
    {
        if(aStampLvl==1)
            aPriceToUpgrade = 2000;
        else if(aStampLvl==2)
            aPriceToUpgrade = 8000;
        else if(aStampLvl==3)
            aPriceToUpgrade = 20000;
        else if(aStampLvl==4)
            aPriceToUpgrade = 50000;
        
        if(aPriceToUpgrade>0 && aPriceToUpgrade<=aCrystals)
        {
//            aCrystals-=aPriceToUpgrade;
            aPossibleUpgrades++;
        }
    }
    */
    
    return aPossibleUpgrades;
}

void User::addEgg()
{
    mEasterEggCount+=1;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("event_easter_eggs", mEasterEggCount);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

uint32_t User::getChallengesCount()
{
    return (_dailyChallenges.isTodaysCompleted() ? 0 : 1) + _weeklyChallenges.GetActiveTasks();
}

bool User::addCrystals(int theAmount)
{
    int aCurrentAmount = _crystals;
    
    if (aCurrentAmount + theAmount < 0)
        aCurrentAmount = 0;//We don't have any more diamonds
    else
        aCurrentAmount += theAmount;
    
    _crystals = aCurrentAmount;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userCrystals", aCurrentAmount);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
    return true;
}

void User::setCrystals(int theAmount)
{
    _crystals = theAmount;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userCrystals", _crystals);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

int User::canUseCrystals(int theAmountToUse)
{
    //If will return < 0 we need more diamonds - show screen how much diamonds are we short ???
    int aCurrentAmount = _crystals;
    if (aCurrentAmount-theAmountToUse>=0)
        addCrystals(-theAmountToUse);
    else
        aCurrentAmount-=theAmountToUse;
    
    return aCurrentAmount;
}

bool User::addDiamonds(int theAmount)
{
    int aCurrentAmount = _diamonds;
    
    if (aCurrentAmount + theAmount < 0)
        aCurrentAmount = 0;//We don't have any more diamonds
    else
        aCurrentAmount += theAmount;
    
    _diamonds = aCurrentAmount;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userDiamonds", aCurrentAmount);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
    return true;
}

void User::setDiamonds(int theAmount)
{
    _diamonds = theAmount;
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userDiamonds", _diamonds);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

int User::canUseDiamonds(int theAmountToUse)
{
    //If will return < 0 we need more diamonds - show screen how much diamonds are we short ???
    int aCurrentAmount = _diamonds;
    if (aCurrentAmount-theAmountToUse>=0)
        addDiamonds(-theAmountToUse);
    else
        aCurrentAmount-=theAmountToUse;
    
    return aCurrentAmount;
}

void User::setExp(uint32_t exp)
{
    _experience = exp;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userExperience", _experience);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setLevel(uint32_t level)
{
    _level = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userLevel", level);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setDwarfs(uint32_t dwarf)
{
    _dwarfs = dwarf;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userDwarfs", _dwarfs);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

bool User::isFirstOpen()
{
    return !cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("gameOpened", 0);
}

bool User::isFirstPlay()
{
    return !cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("gamePlayed", 0);
}

void User::setOpened()
{
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("gameOpened", 1);
}

void User::setPlayed()
{
    std::cout << "SetPlayed\n";
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("gamePlayed", 1);
}

void User::addTimesPlayed()
{
    _timesPlayed += 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("GamePlayedTimes", _timesPlayed);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

/*
void User::addMachine_StampLvl(uint32_t level)
{
    machine_stamp_lvl += level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machineStampLevel", machine_stamp_lvl);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setMachine_StampLvl(uint32_t level)
{
    machine_stamp_lvl = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machineStampLevel", machine_stamp_lvl);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}
*/

void User::addScore_Multi(uint32_t score_lvl)
{
    score_multi_lvl += score_multi_lvl;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("scoreMulti", score_multi_lvl);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setScore_Multi(uint32_t score_lvl)
{
    score_multi_lvl = score_lvl;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("scoreMulti", score_multi_lvl);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

//

/*
void User::addMachine_PlantLvl(uint32_t level)
{
    machine_plants_lvl += level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machinePlantLevel", machine_plants_lvl);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setMachine_PlantLvl(uint32_t level)
{
    machine_plants_lvl = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machinePlantLevel", machine_plants_lvl);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::addMachine_GhostLvl(uint32_t level)
{
    machine_ghost_lvl += level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machineGhostLevel", machine_ghost_lvl);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setMachine_GhostLvl(uint32_t level)
{
    machine_ghost_lvl = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machineGhostLevel", machine_ghost_lvl);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::addMachine_EnemiesLvl(uint32_t level)
{
    machine_enemies_lvl += level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machineEnemiesLevel", machine_enemies_lvl);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}


void User::setMachine_EnemiesLvl(uint32_t level)
{
    machine_enemies_lvl = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("machineEnemiesLevel", machine_enemies_lvl);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}
 */

//////----------

void User::setSpeedMode(uint32_t level)
{
    speedUp_Enabled = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedOn", speedUp_Enabled);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setSpeedStartMin_1(uint32_t level)
{
    speedUp_StartMin_1 = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_1", speedUp_StartMin_1);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setSpeedExtraMin_1_extra(uint32_t level)
{
    speedUp_StartMin_1_extra = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_1_extra", speedUp_StartMin_1_extra);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setSpeedStartMin_2(uint32_t level)
{
    speedUp_StartMin_2 = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_2", speedUp_StartMin_2);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setSpeedExtraMin_2_extra(uint32_t level)
{
    speedUp_StartMin_2_extra = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_2_extra", speedUp_StartMin_2_extra);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setSpeedStartMin_3(uint32_t level)
{
    speedUp_StartMin_3 = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_3", speedUp_StartMin_3);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setSpeedExtraMin_3_extra(uint32_t level)
{
    speedUp_StartMin_3_extra = level;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("debug_speedStartMin_3_extra", speedUp_StartMin_3_extra);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::setShowFreeStuffPopup(bool value)
{
    _showFreeStuffPopup = value;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("showFreeStuffPopup", value);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
}

void User::setShowRateGamePopup(bool value)
{
    _showRateGamePopup = value;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("showRateGamePopup", value);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

uint32_t User::getActivationCount()
{
    return _activationCount;
}

void User::setActivationCount(uint32_t value)
{
    _activationCount = value;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("activationCount", value);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void User::startSessionTime()
{
    _sessionStart = time(NULL);
}

time_t User::getSessionLength()
{
    return time(NULL) - _sessionStart;
}

uint32_t User::getRateGameDay()
{
    return _rateGameDay;
}

void User::setRateGameDay()
{
    _rateGameDay = Time::getYDay();
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("rateGameDay", _rateGameDay);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}


void User::setStoreOfferDay()
{
    _storeOfferDay = Time::getYDay();
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("storeOfferDay", _storeOfferDay);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

uint32_t User::getFreeDmdsWeek()
{
    return _freeDmdsWeek;
}

void User::setFreeDmdsWeek()
{
    _freeDmdsWeek = Time::getYWeek();
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("freeDmdsWeek", _freeDmdsWeek);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

bool User::isFollowingSocialNetwork()
{
    // TODO: implement check if user is following any social network
    return false;
}
