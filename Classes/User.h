#pragma once

#include <stdint.h>
#include "DailyChallenges.h"
#include "WeeklyChallenges.h"
#include "UserStats.h"
#include "MissionManager.h"

#include "ItemDataManager.h"

class User
{
public:
    
    ~User();
    
    static User* getInstance();
    
    uint32_t getLevel()         { return _level; }
    void increaseLevel();
    
    uint32_t getExperience()    { return _experience; }
    void addExperience(uint32_t exp);
    
    uint32_t getDiamonds()      { return _diamonds; }
    bool addDiamonds(int diamonds);
    void setDiamonds(int theAmount);
    
    uint32_t getCrystals()      { return _crystals; }
    bool addCrystals(int crystals);
    void setCrystals(int theAmount);
    int canUseCrystals(int theAmountToUse);
    
    uint32_t getDwarfs()    { return _dwarfs; }
    void addDwarfs(uint32_t dwarf);
    
    bool mSpecialMissionBuild;
    int mSpecialMissionProgress;
    bool mShowNewPart;
    
    // The new mission stuff with sand bags
    bool mNewMissionBuild;
    
    int mNewMissionProgress;
    
    bool DWARF_FOREST_2;
    
    //--------------------------
    
    void finishTutorial();
    
    void ResetData();
    
    void finishTutorial_Small_1();
    void finishTutorial_Small_2();
    void finishTutorial_Small_3();
    
    void finishTutorial_Small_4();
    void finishTutorial_Small_5();
    void finishTutorial_Small_6();
    void finishTutorial_Small_7();
    void finishTutorial_Small_8();
    void finishTutorial_Small_9();
    
    void ChangeDebugButtons();
    
    uint32_t getScore_Multi()    { return score_multi_lvl; }
    void addScore_Multi(uint32_t score_lvl);
    void setScore_Multi(uint32_t score_lvl);
    
    /*
    uint32_t getMachine_StampLvl()    { return machine_stamp_lvl; }
    void addMachine_StampLvl(uint32_t machine_stamp_lvl);
    void setMachine_StampLvl(uint32_t level);
    
    uint32_t getMachine_EnemiesLvl()    { return machine_enemies_lvl; }
    void addMachine_EnemiesLvl(uint32_t machine_enemies_lvl);
    void setMachine_EnemiesLvl(uint32_t level);
    
    uint32_t getMachine_GhostLvl()    { return machine_ghost_lvl; }
    void addMachine_GhostLvl(uint32_t machine_ghost_lvl);
    void setMachine_GhostLvl(uint32_t level);
    
    uint32_t getMachine_PlantLvl()    { return machine_plants_lvl; }
    void addMachine_PlantLvl(uint32_t machine_plants_lvl);
    void setMachine_PlantLvl(uint32_t level);
    */
    
    uint32_t getSpeedMode()    { return speedUp_Enabled; }
    void setSpeedMode(uint32_t speedUp_Enabled);
    
    uint32_t getSpeedStartMin_1()    { return speedUp_StartMin_1; }
    void setSpeedStartMin_1(uint32_t speedUp_StartMin_1);
    
    uint32_t getSpeedExtraMin_1_extra()    { return speedUp_StartMin_1_extra; }
    void setSpeedExtraMin_1_extra(uint32_t speedUp_StartMin_1_extra);
    
    uint32_t getDebugButtonsState(){ return _debugButtonsState;}
    
    uint32_t getSpeedStartMin_2()    { return speedUp_StartMin_2; }
    void setSpeedStartMin_2(uint32_t speedUp_StartMin_2);
    
    uint32_t getSpeedExtraMin_2_extra()    { return speedUp_StartMin_2_extra; }
    void setSpeedExtraMin_2_extra(uint32_t speedUp_StartMin_2_extra);
    
    uint32_t getSpeedStartMin_3()    { return speedUp_StartMin_3; }
    void setSpeedStartMin_3(uint32_t speedUp_StartMin_3);
    
    uint32_t getSpeedExtraMin_3_extra()    { return speedUp_StartMin_3_extra; }
    void setSpeedExtraMin_3_extra(uint32_t speedUp_StartMin_3_extra);
    
    bool showFreeStuffPopup() { return _showFreeStuffPopup; }
    void setShowFreeStuffPopup(bool value);
    
    bool showRateGamePopup() { return _showRateGamePopup; }
    void setShowRateGamePopup(bool value);
    
    uint32_t getChallengesCount();
    
    uint32_t getPossibleUgrades();
    
    int canUseDiamonds(int theAmountToUse);
    
    bool mEasterActive;
    float mEasterTimer;
    bool isEasterActive();
    int mEasterEggCount;
    void addEgg();
    
    int mFeedBackGiven;
    int mLooseTimes;
    
    void setLevel(uint32_t level);
    void setExp(uint32_t exp);
    void setDwarfs(uint32_t dwarf);
    
    bool isFirstOpen();
    bool isFirstPlay();
    void setOpened();
    void setPlayed();
    
    void addTimesPlayed();
    
    uint32_t getActivationCount();
    void setActivationCount(uint32_t value);
    
    uint32_t getFreeDmdsWeek();
    void setFreeDmdsWeek();
    
    uint32_t getRateGameDay();
    void setRateGameDay();
    
    uint32_t getStoreOfferDay() { return _storeOfferDay; }
    void setStoreOfferDay();
    
	DF::Stats::UserStats& getStats() { return _stats; }
	
	DailyChallenges& getDailyChallenges() { return _dailyChallenges; }
    WeeklyChallenges& getWeeklyChallenges() { return _weeklyChallenges; }
    
    MissionManager& getMissionManager() { return _missionManager; }
    
    ItemDataManager& getItemDataManager() {return _itemDataManager; }
    
    float mDebugSpeed_troll;
    float mDebugSpeed_dwarf;
    
    void startSessionTime();
    time_t getSessionLength();
    
    bool isFollowingSocialNetwork();
    
    std::vector<float> _skewPositionFromMain;
    
    uint32_t _tutorial;
    
    uint32_t _tutorial_small_1;
    uint32_t _tutorial_small_2;
    uint32_t _tutorial_small_3;
    
    uint32_t _tutorial_small_4;
    uint32_t _tutorial_small_5;
    uint32_t _tutorial_small_6;
    uint32_t _tutorial_small_7;
    uint32_t _tutorial_small_8;
    uint32_t _tutorial_small_9;
    
    uint32_t _debugButtonsState;
    
    uint32_t FirstSessionDone;
    uint32_t getTimesPlayed()    { return _timesPlayed; }
    
    bool mSpecial_10_Mission;
    bool mSpecial_11_Mission;
    bool mSpecial_12_Mission;
    bool mSpecial_13_Mission;
    bool mSpecial_14_Mission;
    
    bool mSpecial_16_Mission;
    bool mSpecial_17_Mission;
    bool mSpecial_18_Mission;
    
    bool mSpecial_19_Mission;
    bool mSpecial_20_Mission;
    
    bool mSpecial_21_Mission;
    bool mSpecial_22_Mission;
    bool mSpecial_23_Mission;
    
    bool mNewSplitCaves;
    
    bool mDynamicTrolls;
    
    bool mNewMaps;
    
    // The new stuff for data management in game power stuff magic - wohooo :D
    std::string mActiveSpells;
    std::string mBoghtSpells;
    
    std::string mPowerInfo;
    std::string mBoghtPowers;
    
    // New struff for stats
    int mCurrentMissionLevel;
	
private:
	User();
    
    uint32_t _timesPlayed;
    
    uint32_t _level;
    uint32_t _experience;
    uint32_t _diamonds;
    uint32_t _crystals;
    
    /*
    uint32_t machine_stamp_lvl;
    uint32_t machine_ghost_lvl;
    uint32_t machine_enemies_lvl;
    uint32_t machine_plants_lvl;
    */
    
    uint32_t score_multi_lvl;
    
    uint32_t speedUp_Enabled;
    
    uint32_t speedUp_StartMin_1;
    uint32_t speedUp_StartMin_1_extra;
    
    uint32_t speedUp_StartMin_2;
    uint32_t speedUp_StartMin_2_extra;
    
    uint32_t speedUp_StartMin_3;
    uint32_t speedUp_StartMin_3_extra;
    
    uint32_t _dwarfs;
	
	DF::Stats::UserStats _stats;
	
	DailyChallenges _dailyChallenges;
    WeeklyChallenges _weeklyChallenges;
    
    MissionManager _missionManager;
    
    ItemDataManager _itemDataManager;
    
    bool _showFreeStuffPopup;
    bool _showRateGamePopup;
    
    uint32_t _activationCount;
    uint32_t _rateGameDay;
    uint32_t _storeOfferDay;
    uint32_t _freeDmdsWeek;
    
    time_t _sessionStart;
};
