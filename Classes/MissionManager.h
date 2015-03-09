//
//  MissionsManager.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 08/01/14.
//
//

#pragma once

#include "cocos2d.h"

#include "Crystal.h"

//#include <stdint.h>
//#include <ctime>
//#include <sstream>

//#include "../extensions/ExtensionMacros.h"
//#include "../extensions/cocos-ext.h"
//#include "../cocos2d-x/extensions/cocos-ext.h"

#include "stdio.h"
#include "stdlib.h"
#include "curl/curl.h"


//More to come !!!
//Mission types & subtypes
#define SUB_EARN_POINTS 1

#define SUB_LEAD_DWARFS_ONE_GAME 2
#define SUB_LEAD_DWARF_FAT_ROW 3
#define SUB_LEAD_DWARFS_TALL 4

#define SUB_COLLECT_CRYSTAL_GREEN 5
#define SUB_COLLECT_CRYSTAL_BLUE 6
#define SUB_COLLECT_CRYSTAL_RED 7
#define SUB_COLLECT_CRYSTAL_YELLOW 8

#define SUB_COLLECT_MUSHROOM 10
#define SUB_COLLECT_DIAMOND 23

#define SUB_ACTIVATE_POWER_ANY 11
#define SUB_ACTIVATE_POWER_GHOST 12
#define SUB_ACTIVATE_POWER_BLITZ 13
#define SUB_ACTIVATE_POWER_BOOM 14

#define SUB_COLLECT_CRYSTALS_SAME_DWARF 15
#define SUB_COLLECT_CRYSTALS_FAT_DWARF 16
#define SUB_COLLECT_CRYSTALS_TALL_DWARF 17

#define SUB_USE_SAVE_ME 18

#define SUB_DWARF_AWAY_ANY 19
#define SUB_NO_CRYSTAL_COLLECT 20

#define SUB_ONE_DWARF_AWAY_FROM_CAVE 21
#define SUB_PLAY_ONE_GAME 22
#define SUB_LEAD_DWARFS_TO_CAVE_TOTAL 23

#define SUB_GET_COMBO 24



//The reward types
#define REWARD_DIAMONDS 1
#define REWARD_SCORE_MULTY 2
#define REWARD_CRYSTALS 3

#define UNLOCK_CRYSTAL_PLANT 1
#define UNLOCK_GHOST_MACHINE 2
#define UNLOCK_TESLA_MACHINE 3


struct SubMission
{
    int32_t type;
    int32_t amount;
    
    int32_t current_amount;
    
    int32_t best_amount;
    
    std::string name;
    
    std::string complete_name;
    
    int32_t id;
};

struct MissionTroll
{
    std::vector<cocos2d::CCPoint*> _paths;//What are the patrol points
    int32_t _circle;//on what circle to patrol
    int32_t _pathStartIndex;//At what point to start
    int32_t _startDirection;//To what direction start path or circle
    
    int32_t _circle_x;
    int32_t _circle_y;
    int32_t _circle_radius;
    int32_t _circle_precision;
    
    int32_t _radar_radius;
    int32_t _radar_width;
    int32_t _radar_wait;
    
    int32_t _circle_height;
    
    int32_t _timeOnMap;
    int32_t _enemySpawnID;// What functional will this enemy do
    int32_t _indexID;
    
    // Special bee stuff
    int32_t _beeBulletSpeed;
    
    float _speed;
};

struct MissionQuadInfo
{
    int32_t ID; // For what quad is this !!!
    
    int32_t type; // Is it a shield or etc
    
    // More like global
    int32_t active_time; // How long is active
    int32_t activate_time; // After how long time will activate
    
    // For shield - what visual shield will be used?
    int32_t event_type;
    
    // For deadzone radius
    int32_t deadzone_radius;
    
    // For bullets
    int32_t bullet_distance; // How far will fly till explodes
    int32_t bullet_amount; // How much bullets can fire?
    
    // Flame sub stuff
    int32_t flame_angle; // Flame angle?
    int32_t flame_radius; // Flame radius?
    int32_t flame_active_time; // How long flame throewr will be active?
    float flame_rotate_speed; // How fast rotates flame thrower?
    
    int32_t flame_start_angle;
    int32_t flame_end_angle;
};

struct MissionSet
{
    int32_t ID;// || ID
    
    //Old stuff
//	int32_t reward_type;
//	int32_t reward_amount;
//    int32_t completed;
//    
    int32_t unlock_type;//Not needed stuff
//
//    std::string reward_name;
//    
//    std::vector<SubMission> mSubMission_1;
    
    //New stuff
    //Point progress
    int32_t Star_1;// || Star_1
    int32_t Star_2;// || Star_2
    int32_t Star_3;// || Star_3
    
    //Point counter for task
    int32_t Task_type;//Desc under || Task_Type
    // = 0 then count crystal points,
    // = 1 then count dwarf for point,
    // = 2 activate some powerup,
    // = 3 collect green crstals?
    // = 4 hatch mission
    // = 5 dwarf survive
    
    
    
    // 0 = crystal point count
    // 1 = dwarf count
    // 2 = combo level reach
    
    // 10 = collect green crystals
    // 11 = collect red crystals
    // 12 = collect blue crystals
    
    //Is this mission completed
    int32_t Completed;//1 Yes, 0 Nope || Comes from save
    
    //Contains story step
    int32_t Story_show;//>=1 Yes and what chapter probably || Story_Show
    
    //What map background will be
    int32_t MapType;
    
    int32_t _EnemyLines;
    
    //As they will be randomly placed - just give the count
    int32_t Dwarf_spawn_points;//How much cave enterances  || DSP
    
//    std::vector<int> Enemy_paths;//What and where will trolls spawn, give the ID's of the coded paths || Enemy_Paths
    std::vector<MissionTroll> Enemy_info;//What mission trolls will do here ???
    
    std::vector<int> Dwarf_paths;//What and where will forced dwarfs spawn, give the ID's of the coded paths || DwarfSpawnPoints
    
    int32_t StartDwarfCount;
    
//    int32_t DwarfCount_Max;
//    int32_t DwarfCount_Min;
    
    int32_t BlueCave_x;
    int32_t BlueCave_y;
    
    int32_t OrangeCave_x;
    int32_t OrangeCave_y;
    
    int32_t MT_Event_CaveBlock_duration;
    
    //Dwarf spawn
    int32_t DSpawn_zone;//Whats the spawn zone control
    int32_t DSpawn_min;//Whats the min spawn dwarfs in one time
    int32_t DSpawn_max;//Whats the max spawn dwafrs in one time
    int32_t DSpawn_jump;//Whats the max random jump in dwarf generate
    
    //The change to spawn contorl bases
    int32_t DSpawn_change_zone_time;//After what time should change spawn zone
    int32_t DSpawn_change_zone_value;//Whats the change value
    
    int32_t DSpawn_max_limit;//Limit at what max stops to increase
    
    int32_t DSpawn_zone_step;//The step in zone change
    
    int32_t DSpawn_change_min_time;
    int32_t DSpawn_change_min_value;
    
    int32_t DSpawn_change_max_time;
    int32_t DSpawn_change_max_value;
    
    int32_t DSpawn_change_jump_time;
    int32_t DSpawn_change_jump_value;
    
    //The crystal spawn stuff
    int32_t ItemInterval_Min;
    int32_t ItemInterval_Max;
    float ItemProbMultiplier;
    float ItemTimeOnMap;
    float PowerTimeOnMap;
    
    float DwarfSpeed_Fat;
    float DwarfSpeed_Tall;
    
    int32_t DwarfSpawnOneCave;
    
    int32_t DSpawn_formula_type;
    
    
    std::vector<int> ItemNumProbs;
    std::vector<int> CrystalColProbs;
    std::vector<int> ItemTypeProbs;
    std::vector<int> PowerTypeProbs;
    std::vector<int> PowerupItemsProbs;
    
    // The test stuff
    float SpawnBee;
    int32_t Enemy_Bee_StartX;
    int32_t Enemy_Bee_StartY;
    
    int32_t Enemy_Bee_FinishX;
    int32_t Enemy_Bee_FinishY;
    
    int32_t Enemy_Bee_Speed;
    int32_t Enemy_Bee_Bullet_Speed;
    
    int32_t Forced_Bullets;
    int32_t MT_Bullet_Speed_Max;
    int32_t MT_Bullet_Speed_Min;
    
    std::vector<int> MT_Bullet_Types;//What bullets can fire Master Troll
    bool MT_Bullet_Instant;
    
    std::vector<int> MT_Event_Types;// What events can spawn MT
    // The settings for events
    int32_t MT_Event_Percent_Bullet; //What is the procent for this to spawn
    int32_t MT_Event_Percent_Trap;
    int32_t MT_Event_Percent_Troll;
    int32_t MT_Event_Percent_Mass;
    int32_t MT_Event_Percent_Caveblock;
    int32_t MT_Event_Percent_Iceblitz;
    
    int32_t MT_Event_IceBlitz_count;
    
    int32_t MT_Event_Timer_Min;
    int32_t MT_Event_Timer_Max;
    int32_t MT_Event_Value_Timer;
    int32_t MT_Event_Value_Max;
    int32_t MT_Event_Value_Add;
    int32_t MT_Event_Value_Start;
    
    bool DifferentEnemySpawn;
    int32_t MaxEnemy_OnMap;
    
    int32_t MT_Event_Bullets_Max;
    int32_t MT_Event_Bullets_Min;
    int32_t MT_Event_Bullets_Percent;
    
    int32_t MT_Event_Traps_Max;
    int32_t MT_Event_Traps_Min;
    int32_t MT_Event_Traps_Percent;
    
    int32_t MT_Event_Confusion_Time;
    
    int32_t MT_Snipe;
    int32_t MT_Snipe_Delay;
    
    int32_t MT_Event_ForceSpawnEnemy_Time;
    
    int32_t Mission_SaveDwarfs;
    
    int32_t Mission_KillEnemys;
    
    int32_t OrginalDwarfCount;
    
    // The master troll attack stuff
    int32_t MT_Battle_HP;
    int32_t MT_Battle_Damage;
    int32_t MT_Battle_Attack;
    bool MT_Battle_WinOnKill;
    
    // The totem stuff
    bool Mission_Totem;
    int32_t TOTEM_HP;
    int32_t TOTEM_Event_Type1;
    int32_t TOTEM_Event_Type2;
    int32_t TOTEM_Event_Type3;
    int32_t TOTEM_PASSIVE_TYPE;
    int32_t TOTEM_RADIUS;
    int32_t TOTEM_x;
    int32_t TOTEM_y;
    
    std::vector<MissionQuadInfo> TOTEM_QUAD_1;// Whats in for totem quad 1
    std::vector<MissionQuadInfo> TOTEM_QUAD_2;// Whats in for totem quad 1
    std::vector<MissionQuadInfo> TOTEM_QUAD_3;// Whats in for totem quad 1
    std::vector<MissionQuadInfo> TOTEM_QUAD_4;// Whats in for totem quad 1
    
    /*
    int32_t TOTEM_Bullet_Event;
    int32_t TOTEM_Bullet_Freq;
    int32_t TOTEM_BubleShield_Event;
    int32_t TOTEM_BubleShield_Freq;
    int32_t TOTEM_BubleShield_ActiveTime;
    int32_t TOTEM_Flame_Freq;
    int32_t TOTEM_Flame_Radius;// If 0
    int32_t TOTEM_Flame_ActiveTime;
    int32_t TOTEM_x;
    int32_t TOTEM_y;
    */
    
    int32_t DEBUG_Electrify_range;// At what distance fires bullet to totem
    
    int32_t Task_DwarfWinCon;
    int32_t Task_CrystalsWinCon;
    int32_t Task_SurviveTime;
    int32_t Task_SurviveLives;
    
    std::vector<int> SpellSpawnPoints;// Correct forced places for spawn spells
    
    int32_t STORE_Booster_DwarfPrice;
    
};

struct CompletedStuff
{
	int32_t sub_id;
	int32_t mis_id;
};


class MissionManager
{
public:
    MissionManager();
    
//    size_t pWriteCallback(void *pData, size_t n, size_t nDataSize, FILE *stream);
//    int DownProgresss(void* clientp,double fDownLoadTotal,double fDownLoaded,double fUpTotal,double fUpLoaded);
    void Donwloaded();
    void ReDownloadStuff();
    
    void OnReDownload(CURLcode code);
    
    FILE *pFile;
    
    bool mReDownload;
    
    int mCurrentActiveMission;
    
    cocos2d::CCArray* _missionsArr;
    
    std::vector<MissionSet> mAllMission;
    
    std::vector<CompletedStuff> mCompletedStuff;
    
    std::vector<int> mCompletedMissions;
    
    std::vector<int> SplitString(const std::string s,char delim);
//    std::vector<std::string> split(const std::string &s, char delim);
//    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    
    GameScene* _gameScene;
    
    void OnLoadGloablValues();
    void OnFailToLoad(CURLcode code);
    
    void downloadPackage();
    
    void OnDownloadSpecialMissions();
    void OnDownloadedSpecial();
    
    MissionSet GetActiveMission();
    MissionSet AddDwarfs(int theID);
    MissionSet Reset(int theID);
    std::string GetSubMissionText(int theMissionID,int theSubMissionID,bool thePause,bool theDone);
    
    void CheckForStuckMission();
    void CheckSubMission(int theType,int theAmount);
    void UpdateSubMission(int theIndex,int theValue);
    
    void ResetSpecialMissions(int theType);
    void ResetCurrentStates();
    std::string GetMissionPad(int theMissionID);
    bool ShowMissionNumber(int theMissionID);
    
    int GetActiveMissionID();
    MissionSet GetMissionByID(int theID);
    
    void ResetAllStats();
    void GetNewLevel();
    void SaveProgress(int theMissionID,int theSubID,int theAmount);
    
    std::vector<CompletedStuff> GetCompletedStuff();
    
    void ShowCompletePopup(int theMissionID);
    void ShowSubMissionCompleted(int theMissionID,int theSubMissionID);
    
    bool DidCompleteMission();
    MissionSet GetCompletedMission();
    
    void GetExtraMissions_1();
    void AddExtraDownloadedMissions_1();
    
    void GetExtraMissions_2();
    void AddExtraDownloadedMissions_2();
    
    void GetExtraMissions_3();
    void AddExtraDownloadedMissions_3();
    
    void menuCloseCallback_mission();
    void CreateNoInternet();
    
private:
    bool mAllFinished;
};
