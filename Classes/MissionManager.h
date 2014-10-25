//
//  MissionsManager.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 08/01/14.
//
//

#pragma once

//#include "cocos2d.h"

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
    
    int32_t _circle_height;
    
    float _speed;
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
    int32_t Task_type;//If 1 then count dwarf for point, if 0 then count crystal points, 2 activate some powerup, 3 collect green crstals? || Task_Type
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
    
    //Dwarf spawn
    int32_t DSpawn_zone;//Whats the spawn zone control
    int32_t DSpawn_min;//Whats the min spawn dwarfs in one time
    int32_t DSpawn_max;//Whats the max spawn dwafrs in one time
    int32_t DSpawn_jump;//Whats the max random jump in dwarf generate
    
    //The change to spawn contorl bases
    int32_t DSpawn_change_zone_time;//After what time should change spawn zone
    int32_t DSpawn_change_zone_value;//Whats the change value
    
    int32_t DSpawn_zone_step;//The step in zone change
    
    int32_t DSpawn_change_min_time;
    int32_t DSpawn_change_min_value;
    
    int32_t DSpawn_change_max_time;
    int32_t DSpawn_change_max_value;
    
    int32_t DSpawn_change_jump_time;
    int32_t DSpawn_change_jump_value;
    
    //The crystal spawn stuff
    int32_t CrystalInterval_Min;
    int32_t CrystalInterval_Max;
    float CrystalProbMultiplier;
    float CrystalTimeOnMap;
    
    float DwarfSpeed_Fat;
    float DwarfSpeed_Tall;
    
    int32_t DwarfSpawnOneCave;
    
    int32_t DSpawn_formula_type;
    
    
    std::vector<int> CrystalNumProbs;
    std::vector<int> CrystalColProbs;
    
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
    
    FILE *pFile;
    
    int mCurrentActiveMission;
    
    cocos2d::CCArray* _missionsArr;
    
    std::vector<MissionSet> mAllMission;
    
    std::vector<CompletedStuff> mCompletedStuff;
    
    std::vector<int> mCompletedMissions;
    
    std::vector<int> SplitString(const std::string s,char delim);
//    std::vector<std::string> split(const std::string &s, char delim);
//    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    
    GameScene* _gameScene;
    
    void downloadPackage();
    
    MissionSet GetActiveMission();
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
    
private:
    bool mAllFinished;
};