//
//  MissionsManager.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 08/01/14.
//
//

#include "MissionManager.h"

#include "User.h"
#include "Utils.h"

#include "DailyDaysCompletePopup.h"
#include "DailyChallenges.h"
#include "GameScene.h"

#include <algorithm>

//urllib2
//#include "stdio.h"
//#include "stdlib.h"
//#include "curl/curl.h"

//#include <stdio.h>
//#include "curl/curl.h"
//#include <types.h>
//#include "curl/easy.h"
//#include <string>




//#pragma  comment(lib,"libcurl_imp.lib")



//#include <tinyxml2.h>

//#include "FlurryX.h"

//static bool sortByID(const MissionSet* a, const MissionSet* b)
//{
//    return a->ID < a->ID;
//    
////	return (((CCString*)a)->m_sString < ((CCString*)b)->m_sString);
//}


static bool sortByID(MissionSet a, MissionSet b)
{
    return a.ID < b.ID;
}

//struct FtpFile {
//    const char *filename;
//    FILE *stream;
//};
//
//static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
//{
//    struct FtpFile *out=(struct FtpFile *)stream;
//    if(out && !out->stream) {
//        /* open file for writing */
//        out->stream=fopen(out->filename, "wb");
//        if(!out->stream)
//            return -1; /* failure, can't open file to write */
//    }
//    return fwrite(buffer, size, nmemb, out->stream);
//}

void MissionManager::downloadPackage()
{
    
}

size_t static pWriteCallback(void *pData, size_t n, size_t nDataSize, FILE *stream)
{
    size_t nWritten = fwrite(pData,n,nDataSize,(FILE *)stream);
    return nWritten;
}

int static DownProgresss(void* clientp,double fDownLoadTotal,double fDownLoaded,double fUpTotal,double fUpLoaded)
{
    if (fDownLoaded >= 0 && fDownLoadTotal != 0)
        CCLOG("%6.2f%%\n", 100*fDownLoaded/fDownLoadTotal);//Download percentage
    return 0;
}

void MissionManager::ReDownloadStuff()
{
    //Clear old stuff
    mAllMission.clear();
    
    CURL *pCurl;
    CURLcode nResCode;
    
    pCurl = curl_easy_init();//Initialize the CURL has initialized after the success of the CURL pointer
    if (pCurl != NULL)
    {
        std::string saveFileName;
        saveFileName = "DF_Missions2.plist";
        saveFileName = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + saveFileName;
        
        pFile = fopen(saveFileName.c_str(), "w+");
        curl_easy_setopt(pCurl,CURLOPT_URL,"https://www.dropbox.com/s/8kdt0m6kz3030v2/DF_Missions.xml?dl=1");
        if(pFile != NULL)
        {
            curl_easy_setopt(pCurl,CURLOPT_FILE,pFile);                   //The specified file write
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, pWriteCallback);//Callback function to write data
            curl_easy_setopt(pCurl, CURLOPT_VERBOSE, true);                //Let CURL report every suddenness
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 60);                  //Setting the timeout
            curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS,0L);
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, DownProgresss);//Specify a callback function
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER,false);
            curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, true);
            nResCode = curl_easy_perform(pCurl);//Executing the above a set operation and return a status code
            curl_easy_cleanup(pCurl);           //Release the related resources
            fputs ("fopen example",pFile);
            fclose(pFile);
            Donwloaded();
        }
    }
}


void MissionManager::Donwloaded()
{
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+"DF_Missions2.plist";
    
    cocos2d::CCDictionary* plistDictionary = cocos2d::CCDictionary::createWithContentsOfFile(path.c_str());
//    CCLOG("CCDictionary %u",dict->count());
    
    mCurrentActiveMission = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("ActiveMission", 0);
    mCurrentActiveMission = 0;
    
    mAllMission.clear();
    
    MissionSet* mission=new MissionSet();
    //    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement_dummy = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub_point = NULL;
    
    float aDummyVar = 0;
    
    cocos2d::CCDICT_FOREACH(plistDictionary, pElement)
    {
        cocos2d::CCDictionary* missionDict = (cocos2d::CCDictionary*)pElement->getObject();
        
        //Create new mission and set all the parametrs there !!!
        mission = new MissionSet();
        
        //The mission id in row
        mission->ID = missionDict->valueForKey("ID")->floatValue();
        CCLOG("Mission ID: %i",mission->ID);
        
        //Set the stars
        mission->Star_1 = missionDict->valueForKey("Star_1")->floatValue();
        mission->Star_2 = missionDict->valueForKey("Star_2")->floatValue();
        mission->Star_3 = missionDict->valueForKey("Star_3")->floatValue();
        
        mission->MapType = missionDict->valueForKey("MapVisualType")->floatValue();
        
        //Get the task type
        mission->Task_type = missionDict->valueForKey("Task_Type")->floatValue();
        
        //Do we need to show story
        mission->Story_show = missionDict->valueForKey("Story_Show")->floatValue();
        
        //How many caves do we allow to spawn
        mission->Dwarf_spawn_points = missionDict->valueForKey("DSP")->floatValue();
        
        //Get all the enemy paths and powas
        cocos2d::CCDictionary* enemyDict = (cocos2d::CCDictionary*)missionDict->objectForKey("Enemy_Paths");
        cocos2d::CCDICT_FOREACH(enemyDict,pElement_dummy)
        {
            MissionTroll theTroll = *new MissionTroll();
            theTroll._circle = -1;//No circle
            theTroll._pathStartIndex = -1;
            
            std::string aPathValue = pElement_dummy->getStrKey();
            
            //Check what is this for a troll !!!
            cocos2d::CCDictionary* enemySubDict = (cocos2d::CCDictionary*)enemyDict->objectForKey(aPathValue);
            int aPatrolCircle = enemySubDict->valueForKey("PatrolCircle")->intValue();
            if(aPatrolCircle==-1){
                //We have points
                cocos2d::CCDictionary* enemyPathPoint = (cocos2d::CCDictionary*)enemySubDict->objectForKey("PatrolPoints");
                cocos2d::CCDICT_FOREACH(enemyPathPoint,pElement_dummy_sub_point)
                {
                    //Starts to read from back
                    std::string aPathValue = pElement_dummy_sub_point->getStrKey();
                    cocos2d::CCString aPointValue = *enemyPathPoint->valueForKey(aPathValue);
                    
                    cocos2d::CCPoint ret = cocos2d::CCPointZero;
                    char *token = std::strtok(const_cast<char*>(aPointValue.getCString()), ",");
                    while (token != NULL) {
                        if(ret.x == 0) ret.x = std::atoi(token);
                        else ret.y = std::atoi(token);
                        token = std::strtok(NULL, " ");
                    }
                    
                    theTroll._paths.push_back(new cocos2d::CCPoint(ret.x, ret.y));
                }
            }
            else{
                theTroll._circle = aPatrolCircle;
                
                theTroll._circle_x = enemySubDict->valueForKey("PatrolCircle_CenterX")->intValue();
                theTroll._circle_y = enemySubDict->valueForKey("PatrolCircle_CenterY")->intValue();
                theTroll._circle_radius = enemySubDict->valueForKey("PatrolCircle_Radius")->intValue();
                theTroll._circle_precision = enemySubDict->valueForKey("PatrolCircle_Precision")->intValue();
                theTroll._circle_height = enemySubDict->valueForKey("PatrolCircle_Height")->intValue();
            }
            
            //Troll speed
            aDummyVar = enemySubDict->valueForKey("Speed")->floatValue();
            if(aDummyVar>0){
                theTroll._speed = aDummyVar;
            }
            else{
                theTroll._speed = 30.0;
            }
            
            theTroll._pathStartIndex = enemySubDict->valueForKey("PatrolStartPoint")->intValue();
            theTroll._startDirection = enemySubDict->valueForKey("PatrolStartDir")->intValue();
            
            //------------------------------------------------------------------------------------------
            // Check if has any radar stuff
            
            theTroll._radar_radius = 0;
            theTroll._radar_width = 0;
            
            aDummyVar = enemySubDict->valueForKey("Radar_Width")->floatValue();
            if(aDummyVar > 0){
                theTroll._radar_width = aDummyVar;
            }
            aDummyVar = enemySubDict->valueForKey("Radar_Radius")->floatValue();
            if(aDummyVar > 0){
                theTroll._radar_radius = aDummyVar;
            }
            
            //------------------------------------------------------------------------------------------
            
            mission->Enemy_info.push_back(theTroll);
        }
        
        //The dwarf spawn points forced
        cocos2d::CCDictionary* dwarfDict = (cocos2d::CCDictionary*)missionDict->objectForKey("DwarfSpawnPoints");
        cocos2d::CCDICT_FOREACH(dwarfDict,pElement_dummy)
        {
            std::string aPathValue = pElement_dummy->getStrKey();
            int aValue = dwarfDict->valueForKey(aPathValue)->intValue();
            mission->Dwarf_paths.push_back(aValue);
        }
        
        mission->StartDwarfCount = missionDict->valueForKey("StartDwarfCount")->floatValue();
        
//        mission->DwarfCount_Min = missionDict->valueForKey("Map_DwarfCoun_Min")->floatValue();
//        mission->DwarfCount_Max = missionDict->valueForKey("Map_DwarfCoun_Max")->floatValue();
        
        //The cave info!!!
        cocos2d::CCDictionary* caveBlueDict = (cocos2d::CCDictionary*)missionDict->objectForKey("BlueCave");
        mission->BlueCave_x = caveBlueDict->valueForKey("position_x")->intValue();
        mission->BlueCave_y = caveBlueDict->valueForKey("position_y")->intValue();
        
        cocos2d::CCDictionary* caveOrangeDict = (cocos2d::CCDictionary*)missionDict->objectForKey("OrangeCave");
        mission->OrangeCave_x = caveOrangeDict->valueForKey("position_x")->intValue();
        mission->OrangeCave_y = caveOrangeDict->valueForKey("position_y")->intValue();
        
        //---------------------------------------------------------------------------------------------------
        //New stuff || Dwarf Spawn Control ||
        
        mission->DSpawn_jump = missionDict->valueForKey("DSpawn_jump")->intValue();
        mission->DSpawn_change_jump_time = missionDict->valueForKey("DSpawn_change_jump_time")->intValue();
        mission->DSpawn_change_jump_value = missionDict->valueForKey("DSpawn_change_jump_value")->intValue();
        
        mission->DSpawn_max = missionDict->valueForKey("DSpawn_max")->intValue();
        mission->DSpawn_change_max_time = missionDict->valueForKey("DSpawn_change_max_time")->intValue();
        mission->DSpawn_change_max_value = missionDict->valueForKey("DSpawn_change_max_value")->intValue();
        
        mission->DSpawn_min = missionDict->valueForKey("DSpawn_min")->intValue();
        mission->DSpawn_change_min_time = missionDict->valueForKey("DSpawn_change_min_time")->intValue();
        mission->DSpawn_change_min_value = missionDict->valueForKey("DSpawn_change_min_value")->intValue();
        
        mission->DSpawn_zone = missionDict->valueForKey("DSpawn_zone")->intValue();
        mission->DSpawn_change_zone_time = missionDict->valueForKey("DSpawn_change_zone_time")->intValue();
        mission->DSpawn_change_zone_value = missionDict->valueForKey("DSpawn_change_zone_value")->intValue();
        
        // The limit of max dwarfs !!!
        mission->DSpawn_max_limit = 0;
        aDummyVar = missionDict->valueForKey("DSpawn_change_zone_value")->intValue();
        if(aDummyVar>0){
            mission->DSpawn_max_limit = aDummyVar;
        }
        
        
        mission->DSpawn_zone_step = missionDict->valueForKey("DSpawn_zone_step")->intValue();
        if(mission->DSpawn_zone_step == 0){
            mission->DSpawn_zone_step = 1;//Set to 1sec
        }
        
        //---------------------------------------------------------------------------------------------------
        // Crystal spawn stuff
        
//        const char* test = missionDict->valueForKey("ItemInterval")->getCString();
        std::vector<int> crystalMinMaxSpawn = SplitString(missionDict->valueForKey("ItemInterval")->getCString(),',');
        if(crystalMinMaxSpawn.size()>=1){
            mission->ItemInterval_Min = crystalMinMaxSpawn[0];
            mission->ItemInterval_Max = crystalMinMaxSpawn[1];
        }
        else{
            mission->ItemInterval_Min = 1;
            mission->ItemInterval_Max = 1;
        }
        
        mission->ItemProbMultiplier = missionDict->valueForKey("ItemProbMultiplier")->intValue();
        mission->ItemTimeOnMap = missionDict->valueForKey("ItemTimeOnMap")->intValue();
        
        
        mission->CrystalColProbs = SplitString(missionDict->valueForKey("CrystalColProbs")->getCString(),',');
        mission->ItemNumProbs = SplitString(missionDict->valueForKey("ItemNumProbs")->getCString(),',');
        mission->ItemTypeProbs = SplitString(missionDict->valueForKey("ItemTypeProbs")->getCString(),',');
        
        
        //---------------------------------------------------------------------------------------------------
        
        //Dwarf Speed on map
        mission->DwarfSpeed_Fat = 40.0;
        mission->DwarfSpeed_Tall = 40.0;
        
        float aDwarfSpeedDummy = missionDict->valueForKey("DwarfSpeed_Fat")->floatValue();
        if(aDwarfSpeedDummy>0){
            mission->DwarfSpeed_Fat = aDwarfSpeedDummy;
        }
        
        aDwarfSpeedDummy = missionDict->valueForKey("DwarfSpeed_Tall")->floatValue();
        if(aDwarfSpeedDummy>0){
            mission->DwarfSpeed_Tall = aDwarfSpeedDummy;
        }
        
        //------------------
        
        //How much possibility to spawn dwarf from one cave
        mission->DwarfSpawnOneCave = 100;//100% allow for now
        int aSpawnOnceValue = missionDict->valueForKey("DwarfSpawnOneCave")->intValue();
        if(aSpawnOnceValue>0){
            mission->DwarfSpawnOneCave = aSpawnOnceValue;
        }
        
        //The formula type
        mission->DSpawn_formula_type = 0;//Min to map
        aSpawnOnceValue = missionDict->valueForKey("DSpawn_formula_type")->intValue();
        if(aSpawnOnceValue>0){
            mission->DSpawn_formula_type = aSpawnOnceValue;
        }
        
        //Add to all missions parsed
        mAllMission.push_back(*mission);
        //Check if completed or whats the score?
        
    }
    
    std::sort(mAllMission.begin(), mAllMission.end(),sortByID);
    
    mAllFinished = true;
    
    //Create popup for wait !!!
    cocos2d::CCMessageBox("Mission file downloaded","Continue");
}

std::vector<int> static &split(const std::string &s, char delim, std::vector<int> &elems){
    std::stringstream ss(s);
    std::string item;
    int resultInt;
    
    while (std::getline(ss, item, delim)) {
        resultInt = atoi(item.c_str());
        CCLOG("resultInt:%i",resultInt);
        elems.push_back(resultInt);
    }
    return elems;
}

std::vector<int> MissionManager::SplitString(const std::string s,char delim){
    std::vector<int> elems;
    split(s, delim, elems);
    return elems;
}



//Some global stuff for spliting litering stuff around the world
//std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
//    std::stringstream ss(s);
//    std::string item;
//    while (std::getline(ss, item, delim)) {
//        elems.push_back(item);
//    }
//    return elems;
//}


//std::vector<std::string> split(const std::string &s, char delim) {
//    std::vector<std::string> elems;
//    split(s, delim, elems);
//    return elems;
//}

MissionManager::MissionManager()
{
    ///Do the magic here !!!
    
    
    CURL *pCurl;
    CURLcode nResCode;
    
//    std::ofstream fileTest;
    
    
    
//    FILE *pFile;
    pCurl = curl_easy_init();//Initialize the CURL has initialized after the success of the CURL pointer
    if (pCurl != NULL)
    {
        std::string saveFileName;
        saveFileName = "DF_Missions2.plist";
        saveFileName = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + saveFileName;
        
        pFile = fopen(saveFileName.c_str(), "w+");
//        curl_easy_setopt(pCurl,CURLOPT_URL,"https://www.dropbox.com/s/m8usklijiq1c2u5/DF_Missions.plist?dl=1");
        curl_easy_setopt(pCurl,CURLOPT_URL,"https://www.dropbox.com/s/8kdt0m6kz3030v2/DF_Missions.xml?dl=1");
                         if(pFile != NULL)
                         {
                             curl_easy_setopt(pCurl,CURLOPT_FILE,pFile);                   //The specified file write
                             curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, pWriteCallback);//Callback function to write data
                             curl_easy_setopt(pCurl, CURLOPT_VERBOSE, true);                //Let CURL report every suddenness
                             curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 60);                  //Setting the timeout
                             curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS,0L);
                             curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, DownProgresss);//Specify a callback function
                             curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER,false);
                             curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, true);
                             nResCode = curl_easy_perform(pCurl);//Executing the above a set operation and return a status code
                             curl_easy_cleanup(pCurl);           //Release the related resources
                             fputs ("fopen example",pFile);
                             fclose(pFile);
                             Donwloaded();
//                             nResCode == CURLE_OK ? Donwloaded() : CCLOG("CODE: %d",nResCode);
                         }
        
                         }
    return;
    
    //=========================================================
    
    mCurrentActiveMission = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("ActiveMission", 0);
    mCurrentActiveMission = 0;
    CCLOG("CCDictionary 1");
    

    
    downloadPackage();
    
    cocos2d::CCDictionary* plistDictionary=cocos2d::CCDictionary::createWithContentsOfFile("DF_Missions2.plist");
//    cocos2d::CCDictionary *myKeyValuePairDictionary = cocos2d::CCDictionary::createWithContentsOfFile("DF_Missions.plist");
    CCLOG("CCDictionary 2");
    
    CCLOG("CCDictionary %u",plistDictionary->count());
    
    MissionSet* mission=new MissionSet();
//    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement_dummy = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub_point = NULL;
    
    cocos2d::CCDICT_FOREACH(plistDictionary, pElement)
    {
        cocos2d::CCDictionary* missionDict = (cocos2d::CCDictionary*)pElement->getObject();
        
        //Create new mission and set all the parametrs there !!!
        mission = new MissionSet();
        
        //The mission id in row
        mission->ID = missionDict->valueForKey("ID")->floatValue();
        
        //Set the stars
        mission->Star_1 = missionDict->valueForKey("Star_1")->floatValue();
        mission->Star_2 = missionDict->valueForKey("Star_2")->floatValue();
        mission->Star_3 = missionDict->valueForKey("Star_3")->floatValue();
        
        mission->MapType = missionDict->valueForKey("MapVisualType")->floatValue();
        
        //Get the task type
        mission->Task_type = missionDict->valueForKey("Task_Type")->floatValue();
        
        //Do we need to show story
        mission->Story_show = missionDict->valueForKey("Story_Show")->floatValue();
        
        //How many caves do we allow to spawn
        mission->Dwarf_spawn_points = missionDict->valueForKey("DSP")->floatValue();
        
        //Get all the enemy paths and powas
        cocos2d::CCDictionary* enemyDict = (cocos2d::CCDictionary*)missionDict->objectForKey("Enemy_Paths");
        cocos2d::CCDICT_FOREACH(enemyDict,pElement_dummy)
        {
            MissionTroll theTroll = *new MissionTroll();
            theTroll._circle = -1;//No circle
            theTroll._pathStartIndex = -1;
            
            std::string aPathValue = pElement_dummy->getStrKey();
            
            //Check what is this for a troll !!!
            cocos2d::CCDictionary* enemySubDict = (cocos2d::CCDictionary*)enemyDict->objectForKey(aPathValue);
            int aPatrolCircle = enemySubDict->valueForKey("PatrolCircle")->intValue();
            if(aPatrolCircle==-1){
                //We have points
                cocos2d::CCDictionary* enemyPathPoint = (cocos2d::CCDictionary*)enemySubDict->objectForKey("PatrolPoints");
                cocos2d::CCDICT_FOREACH(enemyPathPoint,pElement_dummy_sub_point)
                {
                    //Starts to read from back
                    std::string aPathValue = pElement_dummy_sub_point->getStrKey();
                    cocos2d::CCString aPointValue = *enemyPathPoint->valueForKey(aPathValue);
                    
                    cocos2d::CCPoint ret = cocos2d::CCPointZero;
                    char *token = std::strtok(const_cast<char*>(aPointValue.getCString()), ",");
                    while (token != NULL) {
                        if(ret.x == 0) ret.x = std::atoi(token);
                        else ret.y = std::atoi(token);
                        token = std::strtok(NULL, " ");
                    }
                    
                    theTroll._paths.push_back(new cocos2d::CCPoint(ret.x, ret.y));
                }
            }
            else{
                theTroll._circle = aPatrolCircle;
                //Get the new values
                theTroll._circle_x = enemySubDict->valueForKey("PatrolCircle_CenterX")->intValue();
                theTroll._circle_y = enemySubDict->valueForKey("PatrolCircle_CenterY")->intValue();
                theTroll._circle_radius = enemySubDict->valueForKey("PatrolCircle_Radius")->intValue();
                theTroll._circle_precision = enemySubDict->valueForKey("PatrolCircle_Precision")->intValue();
                theTroll._circle_height = enemySubDict->valueForKey("PatrolCircle_Height")->intValue();
            }
            
            int aPatrolStartPoint = enemySubDict->valueForKey("PatrolStartPoint")->intValue();
            theTroll._pathStartIndex = aPatrolStartPoint;
            theTroll._startDirection = enemySubDict->valueForKey("PatrolStartDir")->intValue();
            
            
            
            /*
            cocos2d::CCDictionary* enemySubDict = (cocos2d::CCDictionary*)enemyDict->objectForKey(aPathValue);
            cocos2d::CCDICT_FOREACH(enemySubDict,pElement_dummy_sub)
            {
                std::string aPathValue2 = pElement_dummy_sub->getStrKey();
                //Runs trough all the control points or circles and other stuff
                int aPatrolCircle = enemySubDict->valueForKey("PatrolCircle")->intValue();
                if(aPatrolCircle>0){
                    //We need to patrol a circle
                    theTroll._circle = aPatrolCircle;
                }
                else{
                    //We have some patrol points
                    cocos2d::CCDictionary* enemyPathPoint = (cocos2d::CCDictionary*)enemySubDict->objectForKey("PatrolPoints");
                    cocos2d::CCDICT_FOREACH(enemyPathPoint,pElement_dummy_sub_point)
                    {
                        //Starts to read from back
                        std::string aPathValue = pElement_dummy_sub_point->getStrKey();
                        cocos2d::CCString aPointValue = *enemyPathPoint->valueForKey(aPathValue);
                        
                        cocos2d::CCPoint ret = cocos2d::CCPointZero;
                        char *token = std::strtok(const_cast<char*>(aPointValue.getCString()), ",");
                        while (token != NULL) {
                            if(ret.x == 0) ret.x = std::atoi(token);
                            else ret.y = std::atoi(token);
                            token = std::strtok(NULL, " ");
                        }
                        
                        theTroll._paths.push_back(new cocos2d::CCPoint(ret.x, ret.y));
                    }
                    
                    int aPatrolStartPoint = enemySubDict->valueForKey("PatrolStartPoint")->intValue();
                    theTroll._pathStartIndex = aPatrolStartPoint;
                }
            }
            */
            
            mission->Enemy_info.push_back(theTroll);
        }
        
        //The dwarf spawn points forced
        cocos2d::CCDictionary* dwarfDict = (cocos2d::CCDictionary*)missionDict->objectForKey("DwarfSpawnPoints");
        cocos2d::CCDICT_FOREACH(dwarfDict,pElement_dummy)
        {
            std::string aPathValue = pElement_dummy->getStrKey();
            int aValue = dwarfDict->valueForKey(aPathValue)->intValue();
            mission->Dwarf_paths.push_back(aValue);
        }
        
        mission->StartDwarfCount = missionDict->valueForKey("StartDwarfCount")->floatValue();
        
//        mission->DwarfCount_Min = missionDict->valueForKey("Map_DwarfCoun_Min")->floatValue();
//        mission->DwarfCount_Max = missionDict->valueForKey("Map_DwarfCoun_Max")->floatValue();
        
        //The cave info!!!
        cocos2d::CCDictionary* caveBlueDict = (cocos2d::CCDictionary*)missionDict->objectForKey("BlueCave");
        mission->BlueCave_x = caveBlueDict->valueForKey("position_x")->intValue();
        mission->BlueCave_y = caveBlueDict->valueForKey("position_y")->intValue();
        
        cocos2d::CCDictionary* caveOrangeDict = (cocos2d::CCDictionary*)missionDict->objectForKey("OrangeCave");
        mission->OrangeCave_x = caveOrangeDict->valueForKey("position_x")->intValue();
        mission->OrangeCave_y = caveOrangeDict->valueForKey("position_y")->intValue();
        
        //Add to all missions parsed
        mAllMission.push_back(*mission);
//        _missionsArr[mission->ID] = mission;
//        _missionsArr->addObject((cocos2d::CCObject*)mission);
        
        //Check if completed or whats the score?
        
    }
    
    //Check on what mission are we now
    
    
//    std::sort(mAllMission.begin(), mAllMission.end(), [](const std::shared_ptr<MissionSet> &left, const std::shared_ptr<MissionSet> &right){
//        return left->ID < right->ID;
//    });
    
//    std::sort(mAllMission.begin(), mAllMission.end(), [](const static_cast<MissionSet*> left, const static_cast<MissionSet*> right){
//        return left->ID < right->ID;
//    });
    
    std::sort(mAllMission.begin(), mAllMission.end(),sortByID);
    
//    std::shared_p
    
//    (MissionSet std::left, MissionSet std::right)
    
//    std::sort(frameNames->data->arr, frameNames->data->arr + frameNames->data->num, compare);
//    sort(mAllMission.begin(), mAllMission.end(), sortByID);
    
//    std::sort(0,2,compare);
//    std::sort(mAllMission.begin(), mAllMission.end(), compare);
    
    mAllFinished = true;
    
    return;
    
    
    /*
    
    //Create the missions?
    mAllFinished = false;
    
    //Create the mission - maybe in future load from xml?
//    MissionSet *mission=new MissionSet();
    
    //-------------------------------------------------------
    //the 1st mission set
    //parse saved data
    std::string saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_1","0|0|0|0");
    std::string delimiter = "|";
    std::string token;
    
    size_t pos = 0;
//    pos = saved_data.find(delimiter);
//    token = saved_data.substr(0, pos);
//    saved_data.erase(0, pos + delimiter.length());
    
    //Check if this mission is completed
    pos = saved_data.find(delimiter);
    mission->completed = std::atoi(saved_data.substr(0, pos).c_str());//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    mission->reward_name = "2 DIAMONDS";
    mission->unlock_type = 0;
    
    //Lets create the submission
    SubMission *sub=new SubMission();
    sub->id = 1;
    sub->type = SUB_LEAD_DWARFS_ONE_GAME;
    sub->amount = 10;
    sub->name = "Lead 10 dwarfs to cave in one game.";
    
    //Get the saved data
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    //Add to finish
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->id = 2;
    sub->type = SUB_LEAD_DWARFS_ONE_GAME;
    sub->amount = 5;
    sub->best_amount = 0;
    sub->name = "Lead 5 dwarfs to cave in one game.";
    
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());
    saved_data.erase(0, pos + delimiter.length());
    
//    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->id = 3;
    sub->type = SUB_COLLECT_CRYSTAL_GREEN;
    sub->amount = 1;
    sub->best_amount = 0;
    sub->name = "Collect 1 Green.";
    
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());
    saved_data.erase(0, pos + delimiter.length());
    
//    mission->mSubMission_1.push_back(*sub);
    
    //Whats the reward
    mission->reward_type = REWARD_DIAMONDS;
    mission->reward_amount = 3;
//    mission->completed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("mission_1", 0);
    mission->id = 1;
    mAllMission.push_back(*mission);
    //-------------------------------------------------------
    
    //-------------------------------------------------------
    //the 2nd mission set
    saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_2","0|0|0|0");
    mission = new MissionSet();
    mission->reward_name = "3 DIAMONDS";
    mission->unlock_type = 0;//UNLOCK_TESLA_MACHINE;
    
    pos = saved_data.find(delimiter);
    mission->completed = std::atoi(saved_data.substr(0, pos).c_str());//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    sub=new SubMission();
    sub->type = SUB_EARN_POINTS;
    sub->amount = 500;
    sub->best_amount = 0;
    
    sub->name = "Earn 500 points in total";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    sub->best_amount = 0;
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_COLLECT_CRYSTAL_GREEN;
    sub->amount = 10;
    sub->best_amount = 0;
    
    sub->name = "Collect 10 Green.";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_COLLECT_MUSHROOM;
    sub->amount = 3;
    sub->best_amount = 0;
    
    sub->name = "Collect 3 Power-Up mushrooms.";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
//    mission->mSubMission_1.push_back(*sub);
    
    //Whats the reward
    mission->reward_type = REWARD_DIAMONDS;
    mission->reward_amount = 3;
//    mission->completed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("mission_2", 0);
    mission->id = 2;
    mAllMission.push_back(*mission);
    //-------------------------------------------------------
    
    //-------------------------------------------------------
    //the 3rd mission set
    saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_3","0|0|0|0");
    mission = new MissionSet();
    mission->reward_name = "Score multiplier x 2";
    mission->unlock_type = 0;
    
    pos = saved_data.find(delimiter);
    mission->completed = std::atoi(saved_data.substr(0, pos).c_str());//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    sub=new SubMission();
    sub->type = SUB_LEAD_DWARF_FAT_ROW;
    sub->amount = 4;
    
    sub->name = "Lead 4 fat dwarfs to cave in row.";
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_COLLECT_CRYSTAL_BLUE;
    sub->amount = 10;
    sub->best_amount = 0;
    
    sub->name = "Collect 10 Blue crystals.";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    sub->best_amount = 0;
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_COLLECT_MUSHROOM;
    sub->amount = 3;
    sub->best_amount = 0;
    
    sub->name = "Collect 3 Power-Up mushrooms.";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    //Whats the reward
    mission->reward_type = REWARD_SCORE_MULTY;
    mission->reward_amount = 2;
//    mission->completed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("mission_3", 0);
    mission->id = 3;
    mAllMission.push_back(*mission);
    //-------------------------------------------------------
    
    //-------------------------------------------------------
    //the 4th mission set
    saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_4","0|0|0|0");
    mission = new MissionSet();
    mission->reward_name = "Score multiplier x 2";
    mission->unlock_type = 0;
    
    pos = saved_data.find(delimiter);
    mission->completed = std::atoi(saved_data.substr(0, pos).c_str());//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    sub=new SubMission();
    sub->type = SUB_ACTIVATE_POWER_ANY;
    sub->amount = 1;
    sub->best_amount = 0;
    
    sub->name = "Activate any power up machine by tapping on it";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    sub->best_amount = 0;
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_COLLECT_CRYSTALS_SAME_DWARF;
    sub->amount = 3;
    sub->best_amount = 0;
    
    sub->name = "Collect 3 crystals with same dwarf";
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_COLLECT_CRYSTAL_RED;
    sub->amount = 15;
    sub->best_amount = 0;
    
    sub->name = "Collect 15 Red Crystals";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    //Whats the reward
    mission->reward_type = REWARD_SCORE_MULTY;
    mission->reward_amount = 3;
    //    mission->completed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("mission_3", 0);
    mission->id = 4;
    mAllMission.push_back(*mission);
    //-------------------------------------------------------
    
    //-------------------------------------------------------
    //the 5th mission set
    saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_5","0|0|0|0");
    mission = new MissionSet();
    mission->reward_name = "400 Crystals";
    mission->unlock_type = 0;//UNLOCK_CRYSTAL_PLANT;
    
    pos = saved_data.find(delimiter);
    mission->completed = std::atoi(saved_data.substr(0, pos).c_str());//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    //Sub mission 1
    sub=new SubMission();
    sub->type = SUB_DWARF_AWAY_ANY;
    sub->amount = 45;
    sub->best_amount = 0;
    
    sub->name = "Keep dwarf on field for 45 sec.";
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    //Sub mission 2
    sub=new SubMission();
    sub->type = SUB_NO_CRYSTAL_COLLECT;
    sub->amount = 60;
    sub->best_amount = 0;
    
    sub->name = "Collect no crystals for 1 min.";
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_USE_SAVE_ME;
    sub->amount = 3;
    sub->best_amount = 0;//std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    
    sub->name = "Use 'Save Me' 3 times";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    //Whats the reward
    mission->reward_type = REWARD_CRYSTALS;
    mission->reward_amount = 400;
    //    mission->completed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("mission_3", 0);
    mission->id = 5;
    mAllMission.push_back(*mission);
    //-------------------------------------------------------
    
    //-------------------------------------------------------
    //the 6th mission set
    saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_6","0|0|0|0");
    mission = new MissionSet();
    mission->reward_name = "REWARD – 5 Diamonds";
    mission->unlock_type = 0;
    
    pos = saved_data.find(delimiter);
    mission->completed = std::atoi(saved_data.substr(0, pos).c_str());//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    sub=new SubMission();
    sub->type = SUB_GET_COMBO;
    sub->amount = 6;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());
    
    sub->name = "Get combo x 3";
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;//std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_COLLECT_DIAMOND;
    sub->amount = 3;
    sub->best_amount = 0;
    
    sub->name = "Collect 3 Diamonds.";
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_LEAD_DWARFS_TO_CAVE_TOTAL;
    sub->amount = 250;
    sub->best_amount = 0;
    
    sub->name = "Lead 250 dwarfs to cave in total";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    //Whats the reward
    mission->reward_type = REWARD_DIAMONDS;
    mission->reward_amount = 5;
    //    mission->completed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("mission_3", 0);
    mission->id = 6;
    mAllMission.push_back(*mission);
    //-------------------------------------------------------

    //-------------------------------------------------------
    //the 7th mission set
    saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_7","0|0|0|0");
    mission = new MissionSet();
    mission->reward_name = "Score multiplier x 4";
    mission->unlock_type = 0;
    
    pos = saved_data.find(delimiter);
    mission->completed = std::atoi(saved_data.substr(0, pos).c_str());//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    sub=new SubMission();
    sub->type = SUB_EARN_POINTS;
    sub->amount = 3000;
    sub->best_amount = 0;
    
    sub->name = "Collect ";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_DWARF_AWAY_ANY;
    sub->amount = 3;
    sub->best_amount = 0;
    
    sub->name = "Walk around cave with 3 dwarfs in one game";
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_USE_SAVE_ME;
    sub->amount = 1;
    sub->best_amount = 0;
    
    sub->name = "Get dwarf trapped 20 times.";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    //Whats the reward
    mission->reward_type = REWARD_SCORE_MULTY;
    mission->reward_amount = 4;
    //    mission->completed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("mission_3", 0);
    mission->id = 7;
    mAllMission.push_back(*mission);
    //-------------------------------------------------------

    //-------------------------------------------------------
    //the 8th mission set
    saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_8","0|0|0|0");
    mission = new MissionSet();
    mission->reward_name = "REWARD – 7 Diamonds";
    mission->unlock_type = UNLOCK_GHOST_MACHINE;
    
    pos = saved_data.find(delimiter);
    mission->completed = std::atoi(saved_data.substr(0, pos).c_str());//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    sub=new SubMission();
    sub->type = SUB_ACTIVATE_POWER_ANY;
    sub->amount = 1;
    sub->best_amount = 0;
    
    sub->name = "Activate any power up machine by tapping on it.";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_COLLECT_CRYSTALS_SAME_DWARF;
    sub->amount = 3;
    sub->best_amount = 0;
    
    sub->name = "Collect 3 crystals with same dwarf.";
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_USE_SAVE_ME;
    sub->amount = 1;
    sub->best_amount = 0;
    
    sub->name = "Use Save me.";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    //Whats the reward
    mission->reward_type = REWARD_DIAMONDS;
    mission->reward_amount = 7;
    //    mission->completed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("mission_3", 0);
    mission->id = 8;
    mAllMission.push_back(*mission);
    //-------------------------------------------------------

    //-------------------------------------------------------
    //the 9th mission set
    saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_9","0|0|0|0");
    mission = new MissionSet();
    mission->reward_name = "REWARD – 500 Crystals";
    mission->unlock_type = 0;
    
    pos = saved_data.find(delimiter);
    mission->completed = std::atoi(saved_data.substr(0, pos).c_str());//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    sub=new SubMission();
    sub->type = SUB_ACTIVATE_POWER_ANY;
    sub->amount = 1;
    sub->best_amount = 0;
    
    sub->name = "Activate any power up machine by tapping on it.";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_COLLECT_CRYSTALS_SAME_DWARF;
    sub->amount = 3;
    sub->best_amount = 0;
    
    sub->name = "Collect 3 crystals with same dwarf.";
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_USE_SAVE_ME;
    sub->amount = 1;
    sub->best_amount = 0;
    
    sub->name = "Use Save me.";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    //Whats the reward
    mission->reward_type = REWARD_CRYSTALS;
    mission->reward_amount = 500;
    //    mission->completed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("mission_3", 0);
    mission->id = 9;
    mAllMission.push_back(*mission);
    //-------------------------------------------------------

    //-------------------------------------------------------
    //the 10th mission set
    saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_10","0|0|0|0");
    mission = new MissionSet();
    mission->reward_name = "REWARD – 500 Crystals";
    mission->unlock_type = 0;
    
    pos = saved_data.find(delimiter);
    mission->completed = std::atoi(saved_data.substr(0, pos).c_str());//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    sub=new SubMission();
    sub->type = SUB_ACTIVATE_POWER_ANY;
    sub->amount = 1;
    sub->best_amount = 0;
    
    sub->name = "Activate any power up machine by tapping on it.";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_COLLECT_CRYSTALS_SAME_DWARF;
    sub->amount = 3;
    sub->best_amount = 0;
    
    sub->name = "Collect 3 crystals with same dwarf.";
    pos = saved_data.find(delimiter);
    sub->current_amount = 0;
    sub->best_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    sub=new SubMission();
    sub->type = SUB_USE_SAVE_ME;
    sub->amount = 1;
    sub->best_amount = 0;
    
    sub->name = "Use Save me.";
    pos = saved_data.find(delimiter);
    sub->current_amount = std::atoi(saved_data.substr(0, pos).c_str());//The 2nd 0 is the sub mission amount saved and so on
    saved_data.erase(0, pos + delimiter.length());
    
    mission->mSubMission_1.push_back(*sub);
    
    //Whats the reward
    mission->reward_type = REWARD_CRYSTALS;
    mission->reward_amount = 500;
    //    mission->completed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("mission_3", 0);
    mission->id = 10;
    mAllMission.push_back(*mission);
    //-------------------------------------------------------

    
    mCompletedStuff.clear();
    */
    
//    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

std::string MissionManager::GetSubMissionText(int theMissionID,int theSubMissionID,bool thePause,bool theDone)
{
    std::stringstream theResult;
    
    /*
    MissionSet *aMission = &mAllMission[theMissionID];
    SubMission aSubMission = aMission->mSubMission_1[theSubMissionID];
    
    if(theDone)
    {
        return aSubMission.name;
    }
    else
    {
        //The base
//        theResult<<aSubMission.name;
//        return aSubMission.name;
    }
    
    if(aSubMission.current_amount<aSubMission.amount && aSubMission.best_amount<aSubMission.amount)
    {
        //Add some progress for this text field
        
        //Check by type what to write here !!!
        switch (aSubMission.type)
        {
            case SUB_LEAD_DWARFS_ONE_GAME:
                if(thePause)
                    theResult<<"Dwarfs left: "<<(aSubMission.amount-aSubMission.current_amount);
                else
                   theResult<<"Current best "<<aSubMission.best_amount<<" in one game";
                break;
            case SUB_EARN_POINTS:
            case SUB_COLLECT_MUSHROOM:
            case SUB_COLLECT_CRYSTAL_RED:
            case SUB_COLLECT_CRYSTAL_GREEN:
            case SUB_COLLECT_CRYSTAL_YELLOW:
            case SUB_COLLECT_CRYSTAL_BLUE:
                theResult<<(aSubMission.amount-aSubMission.current_amount)<<" left";
                break;
            case SUB_LEAD_DWARF_FAT_ROW:
                if(thePause)
                    theResult<<(aSubMission.amount-aSubMission.current_amount)<<" left";
                else
                    theResult<<"Current best "<<aSubMission.best_amount<<" in row";
                break;
            case SUB_ACTIVATE_POWER_ANY:
                theResult<<" Not activated!";
                break;
            case SUB_COLLECT_CRYSTALS_SAME_DWARF:
                if(thePause)
                    theResult<<(aSubMission.amount-aSubMission.current_amount)<<" crystals left";
                else
                    theResult<<"Current best "<<aSubMission.best_amount<<" with same dwarf";
                break;
            case SUB_USE_SAVE_ME:
                theResult<<"Not used!";
                break;
            
            case SUB_GET_COMBO:
                if(thePause)
                    theResult<<"Best combo x"<<aSubMission.current_amount;
                else
                    theResult<<"Current best x"<<aSubMission.best_amount<<" combo";
                break;
                
            case SUB_NO_CRYSTAL_COLLECT:
                if(thePause)
                    theResult<<"Time left: "<<(aSubMission.amount-aSubMission.current_amount)<<" sec";
                else
                    theResult<<"Current best time: "<<aSubMission.best_amount<<" sec";
                break;
            case SUB_DWARF_AWAY_ANY:
                if(thePause)
                    theResult<<"Time left: "<<(aSubMission.amount-aSubMission.current_amount)<<" sec";
                else
                    theResult<<"Current best time: "<<aSubMission.best_amount<<" sec";
                break;
                
            default:
                theResult<<"missing string";
                break;
        }
        
    }
    else
    {
        //We have completed this subtask!!!
        theResult<<"Submission completed !!!";
    }
    */
    
    return theResult.str();
}

MissionSet MissionManager::GetActiveMission()
{
    //Search for that mission !!! with correct ID
//    User::getInstance()->getMissionManager().mCurrentActiveMission
//    mLastClickedMission
    MissionSet *aMission = NULL;
    CCLOG("MissionID:%i",mCurrentActiveMission);
    
    for (int i = 0; i<mAllMission.size(); i++) {
        
        CCLOG("i = %i",i);
        CCLOG("mAllMission[i].ID = %i",mAllMission[i].ID);
        
        
        if(mAllMission[i].ID == (mCurrentActiveMission+1)){
            aMission = &mAllMission[i];
            CCLOG("Found mission");
            break;
        }
    }
    
//    MissionSet *aMission = &mAllMission[mCurrentActiveMission];
    return *aMission;
}

int MissionManager::GetActiveMissionID()
{
//    if(User::getInstance()->mNewMissionBuild)
//    {
//        return User::getInstance()->mNewMissionProgress;
//    }
//    
//    if(User::getInstance()->mSpecialMissionBuild)
//    {
//        return (mCurrentActiveMission+User::getInstance()->mSpecialMissionProgress);
//    }
    
    return mCurrentActiveMission;
}

MissionSet MissionManager::GetMissionByID(int theID)
{
    //Safe check
    if(mAllMission.size()<=theID)
    {
        //Return some dummy mission
        return *new MissionSet();
    }
    
    MissionSet *aMission = &mAllMission[theID];
    return *aMission;
}

void MissionManager::ResetSpecialMissions(int theType)
{
    /*
    for(int i=0;i<mAllMission.size();i++)
    {
        MissionSet *aMission = &mAllMission[i];
        
        if(aMission->completed == 1)
            continue;
        
        if(aMission->id>mCurrentActiveMission+1)
            continue;
        
        for(int s=0;s<aMission->mSubMission_1.size();s++)
        {
            if(aMission->mSubMission_1[s].type == theType)
            {
                if(aMission->mSubMission_1[s].best_amount<aMission->mSubMission_1[s].amount)
                {
                    aMission->mSubMission_1[s].current_amount = 0;
                    break;
                }
            }
        }
    }
    */
}

void MissionManager::ResetCurrentStates()
{
    /*
    //We know what tasks to reset
    MissionSet *aMission = &mAllMission[0];
    aMission->mSubMission_1[0].current_amount = 0;
    
    aMission = &mAllMission[2];
    aMission->mSubMission_1[0].current_amount = 0;
    
    aMission = &mAllMission[3];
    aMission->mSubMission_1[1].current_amount = 0;
    
    aMission = &mAllMission[4];
    aMission->mSubMission_1[0].current_amount = 0;
    
    aMission = &mAllMission[4];
    aMission->mSubMission_1[1].current_amount = 0;
    */
}

bool MissionManager::ShowMissionNumber(int theMissionID)
{
    MissionSet *aMission = &mAllMission[theMissionID];
    if(aMission->unlock_type > 0)
        return false;
    
    return true;
}

std::string MissionManager::GetMissionPad(int theMissionID)
{
    std::stringstream theMission;
    MissionSet *aMission = &mAllMission[theMissionID];
    
    if(aMission->unlock_type==0)
    {
        theMission<<"Interfeiss/missions/mission_pad.png";
    }
    else
    {
        if(aMission->unlock_type == UNLOCK_CRYSTAL_PLANT)
            theMission<<"Interfeiss/missions/unlock_mission5.png";
        else if(aMission->unlock_type == UNLOCK_TESLA_MACHINE)
            theMission<<"Interfeiss/missions/unlock_mission2.png";
        else if(aMission->unlock_type == UNLOCK_GHOST_MACHINE)
            theMission<<"Interfeiss/missions/unlock_mission8.png";
        else
            theMission<<"Interfeiss/missions/unlock_chest.png";
    }
    
    return theMission.str();
}

void MissionManager::CheckForStuckMission()
{
    /*
    MissionSet *aMission = &mAllMission[mCurrentActiveMission];
    SubMission *aSub;
    
    int aCompletedTotal = 0;
    int aSubID = 0;
    
    int aMissionCount = aMission->mSubMission_1.size();
    
    SubMission *aWin;
    
    bool aDidCompleteSmall = false;
    
    for(int i=0;i<aMissionCount;i++)
    {
        aSub = &aMission->mSubMission_1[i];
        if(aSub->current_amount>=aSub->amount || aSub->best_amount>=aSub->amount)
        {
            mCompletedStuff.push_back((CompletedStuff){i,mCurrentActiveMission});
//            CCLOG("Mission completed");//no more other stuff around
            aCompletedTotal++;
        }
    }
    
    if(aCompletedTotal==aMissionCount)
    {
        if(mCompletedMissions.size()>0)
        {
            
        }
        else
        {
//            ShowCompletePopup(mCurrentActiveMission);
            mCompletedMissions.push_back(mCurrentActiveMission);
        }
    }
    */
}

void MissionManager::CheckSubMission(int theType,int theAmount)
{
    /*
    //Get the current active mission and try to check if all is ok !!!
    
    MissionSet *aMission = &mAllMission[mCurrentActiveMission];
    SubMission *aSub;
    //Check if this mission contains submission with the needed stuff
    int aCompletedTotal = 0;
    int aSubID = 0;
    
    int aMissionCount = aMission->mSubMission_1.size();
    
    SubMission *aWin;
    
    bool aDidCompleteSmall = false;
    
    for(int i=0;i<aMissionCount;i++)
    {
        aSub = &aMission->mSubMission_1[i];
        if(aSub->type==theType && aSub->current_amount<aSub->amount && aSub->best_amount<aSub->amount)
        {
//            Dwarf* otherDwarf = (Dwarf*)entry;
            //Special cases for submissions
            if(aSub->type == SUB_LEAD_DWARFS_ONE_GAME || aSub->type == SUB_LEAD_DWARF_FAT_ROW || aSub->type == SUB_COLLECT_CRYSTALS_SAME_DWARF
               || aSub->type == SUB_GET_COMBO || aSub->type == SUB_NO_CRYSTAL_COLLECT)
            {
                aSub->current_amount = theAmount;
                if(aSub->best_amount<theAmount)
                    aSub->best_amount = theAmount;
            }
            else
            {
                //the default stuff
                 aSub->current_amount+=theAmount;
            }
            
           
//            aSub.current_amount+=theAmount;
            //Save this info !!!
//            std::string saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_1","mis_1|0|0|0|0");
            SaveProgress(mCurrentActiveMission,i,aSub->current_amount);
            
            if(aSub->current_amount>=aSub->amount)
            {
                //Completed mission
                mCompletedStuff.push_back((CompletedStuff){i,mCurrentActiveMission});
//                CCLOG("Mission completed");//no more other stuff around
                
                //Show some small popup that mission part completed !!!
//                ShowSubMissionCompleted();
//                break;
                aCompletedTotal++;
                aDidCompleteSmall = true;
                
                aSubID = i;
                
//                aWin = &aMission->mSubMission_1[i];
            }
        }
        else if(aSub->current_amount>=aSub->amount)
        {
            aCompletedTotal++;
        }
    }
    
    if(aCompletedTotal==aMissionCount)
    {
        if(mCompletedMissions.size()>0)
        {
            
        }
        else
        {
            ShowCompletePopup(mCurrentActiveMission);
            mCompletedMissions.push_back(mCurrentActiveMission);
            
//            std::stringstream aStream;
//            aStream<<mCurrentActiveMission;
//            FlurryX::logEvent("Completed Mission:","ID",aStream.str().c_str());
//            aStream.str("");aStream.clear();
            
//            FlurryX::logEvent("Completed Mission:",mCurrentActiveMission);
        }
        
        //We completed something in total !!!
        //aMission - show this mission completed !!!
        
        
        //Add to show this mission complete on level up!!!
        
        //Get the next mission to complete !!!
        
        //Do this only when level ended !!! or mission screen open [todo]
//        mCurrentActiveMission++;
//        cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("ActiveMission", mCurrentActiveMission);
//        cocos2d::CCUserDefault::sharedUserDefault()->flush();
    }
    else if(aDidCompleteSmall)
    {
        //aWin - use this to show the submission info !!!
        ShowSubMissionCompleted(mCurrentActiveMission,aSubID);
        
//        std::stringstream aStream;
//        aStream<<aSubID;
//        FlurryX::logEvent("Completed Sub Mission:","ID",aStream.str().c_str());
//        aStream.str("");aStream.clear();
        
//        FlurryX::logEvent("Completed Sub Mission:",aSubID);
    }
    */
}

void MissionManager::GetNewLevel()
{
    mCurrentActiveMission++;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("ActiveMission", mCurrentActiveMission);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();

}

void MissionManager::ResetAllStats()
{
    std::stringstream theMission;
    
    std::string aMissionFinal = theMission.str();
    
    for(int i=1;i<10;i++)
    {
        theMission.str("");
        theMission<<"mission_"<<i;
        aMissionFinal = theMission.str();
        
        cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey(aMissionFinal.c_str(),"0|0|0|0");
    }
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("ActiveMission", 0);
    mCurrentActiveMission = 0;
    
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void MissionManager::SaveProgress(int theMissionID,int theSubID,int theAmount)
{
    std::stringstream theMission;
    theMission<<"mission_"<<(theMissionID+1);
    std::string aMissionFinal = theMission.str();
    
    std::string saved_data = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey(aMissionFinal.c_str(),"0|0|0|0");
    std::string delimiter = "|";
    std::string token;
    
    //Remove the mission stuff
    size_t pos = 0;
    pos = saved_data.find(delimiter);
    token = saved_data.substr(0, pos);
    saved_data.erase(0, pos + delimiter.length());
    
    //Start to load prev values
    pos = saved_data.find(delimiter);
    std::string aValue_1 = saved_data.substr(0, pos);//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    if(theSubID==0)
        aValue_1 = toString(theAmount);
    
    //The 2nd value
    pos = saved_data.find(delimiter);
    std::string aValue_2 = saved_data.substr(0, pos);//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    if(theSubID==1)
        aValue_2 = toString(theAmount);
    
    //The 3rd value
    pos = saved_data.find(delimiter);
    std::string aValue_3 = saved_data.substr(0, pos);//The 1st 0 is the completed self mission status
    saved_data.erase(0, pos + delimiter.length());
    
    if(theSubID==2)
        aValue_3 = toString(theAmount);
    
    //Now move all together
    std::stringstream theSave;
    theSave<<token<<"|"<<aValue_1<<"|"<<aValue_2<<"|"<<aValue_3;
    
    std::string aFinal = theSave.str();
    
    cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey(aMissionFinal.c_str(), aFinal.c_str());
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
//    std::string saved_data2 = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("mission_1");
//    CCLOG("Mission saved");//no more other stuff around
    
//    ShowCompletePopup();
//    ShowSubMissionCompleted();
}

void MissionManager::ShowCompletePopup(int theMissionID)
{
    if(_gameScene)
        _gameScene->CreateMissionCompletedShow(theMissionID);
    
    return;
    
    
    //TODO - ADD NORMAL STUFF
    cocos2d::CCSize visibleSize = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
    
    //Test for now
    DailyDaysCompletePopup* pop = DailyDaysCompletePopup::create(User::getInstance()->getDailyChallenges().getDaysCompleted(), User::getInstance()->getDailyChallenges().getCurrentDayReward());
    pop->retain();
    
    pop->setAnchorPoint(ccp(0,0));
    pop->setPosition(ccp(0,-visibleSize.height));
    pop->runAction(cocos2d::CCMoveTo::create(0.5, ccp(0,0)));
    
    cocos2d::CCDirector::sharedDirector()->getRunningScene()->addChild(pop);
}

void MissionManager::ShowSubMissionCompleted(int theMissionID,int theSubMissionID)
{
    //For now show text that submission completed !!! :)
    if(_gameScene)
        _gameScene->CreateSubMissionCompletedShow(theMissionID, theSubMissionID);
    
    
    return;
    //TODO - ADD NORMAL STUFF
    cocos2d::CCSize _screenSize = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
    DF::Stats::GameStats _stats;
    
    //For now drop it from above?
    DailyChallengesInGameInfo* dailyChallengeInfo = DailyChallengesInGameInfo::create(_stats);
    dailyChallengeInfo->setAnchorPoint(ccp(0.5,0));
    dailyChallengeInfo->setPosition(ccp((_screenSize.width - dailyChallengeInfo->getContentSize().width) / 2 , 100));
    cocos2d::CCDirector::sharedDirector()->getRunningScene()->addChild(dailyChallengeInfo, 1000);
}

std::vector<CompletedStuff> MissionManager::GetCompletedStuff()
{
    return mCompletedStuff;
}

bool MissionManager::DidCompleteMission()
{
    if(mCompletedMissions.size()>0)
        return true;
    
    return false;
}

MissionSet MissionManager::GetCompletedMission()
{
    int aCompletedMissionID = mCompletedMissions[mCompletedMissions.size()-1];
    mCompletedMissions.clear();
    
    return mAllMission[aCompletedMissionID];
}

void MissionManager::UpdateSubMission(int theIndex,int theValue)
{
    
}

//----------------------------------------------------------------------------------------
//The extra 3 files !!!

void MissionManager::GetExtraMissions_1()
{
    CURL *pCurl;
    CURLcode nResCode;
    
    pCurl = curl_easy_init();//Initialize the CURL has initialized after the success of the CURL pointer
    if (pCurl != NULL)
    {
        std::string saveFileName;
        saveFileName = "DF_MissionsExtra_1.plist";
        saveFileName = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + saveFileName;
        
        pFile = fopen(saveFileName.c_str(), "w+");
        curl_easy_setopt(pCurl,CURLOPT_URL,"https://www.dropbox.com/s/avy1siu5hyotl2j/DF_Missions_E1.xml?dl=1");
        if(pFile != NULL)
        {
            curl_easy_setopt(pCurl,CURLOPT_FILE,pFile);                   //The specified file write
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, pWriteCallback);//Callback function to write data
            curl_easy_setopt(pCurl, CURLOPT_VERBOSE, true);                //Let CURL report every suddenness
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 60);                  //Setting the timeout
            curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS,0L);
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, DownProgresss);//Specify a callback function
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER,false);
            curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, true);
            nResCode = curl_easy_perform(pCurl);//Executing the above a set operation and return a status code
            curl_easy_cleanup(pCurl);           //Release the related resources
            fputs ("fopen example",pFile);
            fclose(pFile);
            AddExtraDownloadedMissions_1();
        }
    }
}

void MissionManager::AddExtraDownloadedMissions_1()
{
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+"DF_MissionsExtra_1.plist";
    
    cocos2d::CCDictionary* plistDictionary;
    
    plistDictionary = cocos2d::CCDictionary::createWithContentsOfFile(path.c_str());
    
    mAllMission.clear();
    
    MissionSet* mission=new MissionSet();
    //    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement_dummy = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub_point = NULL;
    
    float aDummyVar = 0;
    
    cocos2d::CCDICT_FOREACH(plistDictionary, pElement)
    {
        cocos2d::CCDictionary* missionDict = (cocos2d::CCDictionary*)pElement->getObject();
        
        //Create new mission and set all the parametrs there !!!
        mission = new MissionSet();
        
        //The mission id in row
        mission->ID = missionDict->valueForKey("ID")->floatValue();
        CCLOG("Mission ID: %i",mission->ID);
        
        //Set the stars
        mission->Star_1 = missionDict->valueForKey("Star_1")->floatValue();
        mission->Star_2 = missionDict->valueForKey("Star_2")->floatValue();
        mission->Star_3 = missionDict->valueForKey("Star_3")->floatValue();
        
        mission->MapType = missionDict->valueForKey("MapVisualType")->floatValue();
        
        //Get the task type
        mission->Task_type = missionDict->valueForKey("Task_Type")->floatValue();
        
        //Do we need to show story
        mission->Story_show = missionDict->valueForKey("Story_Show")->floatValue();
        
        //How many caves do we allow to spawn
        mission->Dwarf_spawn_points = missionDict->valueForKey("DSP")->floatValue();
        
        //Get all the enemy paths and powas
        cocos2d::CCDictionary* enemyDict = (cocos2d::CCDictionary*)missionDict->objectForKey("Enemy_Paths");
        cocos2d::CCDICT_FOREACH(enemyDict,pElement_dummy)
        {
            MissionTroll theTroll = *new MissionTroll();
            theTroll._circle = -1;//No circle
            theTroll._pathStartIndex = -1;
            
            std::string aPathValue = pElement_dummy->getStrKey();
            
            //Check what is this for a troll !!!
            cocos2d::CCDictionary* enemySubDict = (cocos2d::CCDictionary*)enemyDict->objectForKey(aPathValue);
            int aPatrolCircle = enemySubDict->valueForKey("PatrolCircle")->intValue();
            if(aPatrolCircle==-1){
                //We have points
                cocos2d::CCDictionary* enemyPathPoint = (cocos2d::CCDictionary*)enemySubDict->objectForKey("PatrolPoints");
                cocos2d::CCDICT_FOREACH(enemyPathPoint,pElement_dummy_sub_point)
                {
                    //Starts to read from back
                    std::string aPathValue = pElement_dummy_sub_point->getStrKey();
                    cocos2d::CCString aPointValue = *enemyPathPoint->valueForKey(aPathValue);
                    
                    cocos2d::CCPoint ret = cocos2d::CCPointZero;
                    char *token = std::strtok(const_cast<char*>(aPointValue.getCString()), ",");
                    while (token != NULL) {
                        if(ret.x == 0) ret.x = std::atoi(token);
                        else ret.y = std::atoi(token);
                        token = std::strtok(NULL, " ");
                    }
                    
                    theTroll._paths.push_back(new cocos2d::CCPoint(ret.x, ret.y));
                }
            }
            else{
                theTroll._circle = aPatrolCircle;
                
                theTroll._circle_x = enemySubDict->valueForKey("PatrolCircle_CenterX")->intValue();
                theTroll._circle_y = enemySubDict->valueForKey("PatrolCircle_CenterY")->intValue();
                theTroll._circle_radius = enemySubDict->valueForKey("PatrolCircle_Radius")->intValue();
                theTroll._circle_precision = enemySubDict->valueForKey("PatrolCircle_Precision")->intValue();
                theTroll._circle_height = enemySubDict->valueForKey("PatrolCircle_Height")->intValue();
            }
            
            //Troll speed
            aDummyVar = enemySubDict->valueForKey("Speed")->floatValue();
            if(aDummyVar>0){
                theTroll._speed = aDummyVar;
            }
            else{
                theTroll._speed = 30.0;
            }
            
            theTroll._pathStartIndex = enemySubDict->valueForKey("PatrolStartPoint")->intValue();
            theTroll._startDirection = enemySubDict->valueForKey("PatrolStartDir")->intValue();
            
            //------------------------------------------------------------------------------------------
            // Check if has any radar stuff
            
            theTroll._radar_radius = 0;
            theTroll._radar_width = 0;
            
            aDummyVar = enemySubDict->valueForKey("Radar_Width")->floatValue();
            if(aDummyVar > 0){
                theTroll._radar_width = aDummyVar;
            }
            aDummyVar = enemySubDict->valueForKey("Radar_Radius")->floatValue();
            if(aDummyVar > 0){
                theTroll._radar_radius = aDummyVar;
            }
            
            //------------------------------------------------------------------------------------------
            
            mission->Enemy_info.push_back(theTroll);
        }
        
        //The dwarf spawn points forced
        cocos2d::CCDictionary* dwarfDict = (cocos2d::CCDictionary*)missionDict->objectForKey("DwarfSpawnPoints");
        cocos2d::CCDICT_FOREACH(dwarfDict,pElement_dummy)
        {
            std::string aPathValue = pElement_dummy->getStrKey();
            int aValue = dwarfDict->valueForKey(aPathValue)->intValue();
            mission->Dwarf_paths.push_back(aValue);
        }
        
        mission->StartDwarfCount = missionDict->valueForKey("StartDwarfCount")->floatValue();
        
        //        mission->DwarfCount_Min = missionDict->valueForKey("Map_DwarfCoun_Min")->floatValue();
        //        mission->DwarfCount_Max = missionDict->valueForKey("Map_DwarfCoun_Max")->floatValue();
        
        //The cave info!!!
        cocos2d::CCDictionary* caveBlueDict = (cocos2d::CCDictionary*)missionDict->objectForKey("BlueCave");
        mission->BlueCave_x = caveBlueDict->valueForKey("position_x")->intValue();
        mission->BlueCave_y = caveBlueDict->valueForKey("position_y")->intValue();
        
        cocos2d::CCDictionary* caveOrangeDict = (cocos2d::CCDictionary*)missionDict->objectForKey("OrangeCave");
        mission->OrangeCave_x = caveOrangeDict->valueForKey("position_x")->intValue();
        mission->OrangeCave_y = caveOrangeDict->valueForKey("position_y")->intValue();
        
        //---------------------------------------------------------------------------------------------------
        //New stuff || Dwarf Spawn Control ||
        
        mission->DSpawn_jump = missionDict->valueForKey("DSpawn_jump")->intValue();
        mission->DSpawn_change_jump_time = missionDict->valueForKey("DSpawn_change_jump_time")->intValue();
        mission->DSpawn_change_jump_value = missionDict->valueForKey("DSpawn_change_jump_value")->intValue();
        
        mission->DSpawn_max = missionDict->valueForKey("DSpawn_max")->intValue();
        mission->DSpawn_change_max_time = missionDict->valueForKey("DSpawn_change_max_time")->intValue();
        mission->DSpawn_change_max_value = missionDict->valueForKey("DSpawn_change_max_value")->intValue();
        
        mission->DSpawn_min = missionDict->valueForKey("DSpawn_min")->intValue();
        mission->DSpawn_change_min_time = missionDict->valueForKey("DSpawn_change_min_time")->intValue();
        mission->DSpawn_change_min_value = missionDict->valueForKey("DSpawn_change_min_value")->intValue();
        
        mission->DSpawn_zone = missionDict->valueForKey("DSpawn_zone")->intValue();
        mission->DSpawn_change_zone_time = missionDict->valueForKey("DSpawn_change_zone_time")->intValue();
        mission->DSpawn_change_zone_value = missionDict->valueForKey("DSpawn_change_zone_value")->intValue();
        
        mission->DSpawn_zone_step = missionDict->valueForKey("DSpawn_zone_step")->intValue();
        if(mission->DSpawn_zone_step == 0){
            mission->DSpawn_zone_step = 1;//Set to 1sec
        }
        
        //---------------------------------------------------------------------------------------------------
        // Crystal spawn stuff
        
        //        const char* test = missionDict->valueForKey("ItemInterval")->getCString();
        std::vector<int> crystalMinMaxSpawn = SplitString(missionDict->valueForKey("ItemInterval")->getCString(),',');
        if(crystalMinMaxSpawn.size()>=1){
            mission->ItemInterval_Min = crystalMinMaxSpawn[0];
            mission->ItemInterval_Max = crystalMinMaxSpawn[1];
        }
        else{
            mission->ItemInterval_Min = 1;
            mission->ItemInterval_Max = 1;
        }
        
        mission->ItemProbMultiplier = missionDict->valueForKey("ItemProbMultiplier")->intValue();
        mission->ItemTimeOnMap = missionDict->valueForKey("ItemTimeOnMap")->intValue();
        
        mission->CrystalColProbs = SplitString(missionDict->valueForKey("CrystalColProbs")->getCString(),',');
        mission->ItemNumProbs = SplitString(missionDict->valueForKey("ItemNumProbs")->getCString(),',');
        mission->ItemTypeProbs = SplitString(missionDict->valueForKey("ItemTypeProbs")->getCString(),',');
        
        //---------------------------------------------------------------------------------------------------
        
        //Dwarf Speed on map
        mission->DwarfSpeed_Fat = 40.0;
        mission->DwarfSpeed_Tall = 40.0;
        
        float aDwarfSpeedDummy = missionDict->valueForKey("DwarfSpeed_Fat")->floatValue();
        if(aDwarfSpeedDummy>0){
            mission->DwarfSpeed_Fat = aDwarfSpeedDummy;
        }
        
        aDwarfSpeedDummy = missionDict->valueForKey("DwarfSpeed_Tall")->floatValue();
        if(aDwarfSpeedDummy>0){
            mission->DwarfSpeed_Tall = aDwarfSpeedDummy;
        }
        
        //------------------
        
        //How much possibility to spawn dwarf from one cave
        mission->DwarfSpawnOneCave = 100;//100% allow for now
        int aSpawnOnceValue = missionDict->valueForKey("DwarfSpawnOneCave")->intValue();
        if(aSpawnOnceValue>0){
            mission->DwarfSpawnOneCave = aSpawnOnceValue;
        }
        
        //The formula type
        mission->DSpawn_formula_type = 0;//Min to map
        aSpawnOnceValue = missionDict->valueForKey("DSpawn_formula_type")->intValue();
        if(aSpawnOnceValue>0){
            mission->DSpawn_formula_type = aSpawnOnceValue;
        }
        
        //Add to all missions parsed
        mAllMission.push_back(*mission);
        //Check if completed or whats the score?
        
    }
    
    std::sort(mAllMission.begin(), mAllMission.end(),sortByID);
    
    mAllFinished = true;
    
    //Create popup for wait !!!
    cocos2d::CCMessageBox("Mission file Extra 1 downloaded","Continue");
}

void MissionManager::GetExtraMissions_2()
{
    CURL *pCurl;
    CURLcode nResCode;
    
    pCurl = curl_easy_init();//Initialize the CURL has initialized after the success of the CURL pointer
    if (pCurl != NULL)
    {
        std::string saveFileName;
        saveFileName = "DF_MissionsExtra_2.plist";
        saveFileName = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + saveFileName;
        
        pFile = fopen(saveFileName.c_str(), "w+");
        curl_easy_setopt(pCurl,CURLOPT_URL,"https://www.dropbox.com/s/0w4zehdwa1m424i/DF_Missions_E2.xml?dl=1");
        if(pFile != NULL)
        {
            curl_easy_setopt(pCurl,CURLOPT_FILE,pFile);                   //The specified file write
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, pWriteCallback);//Callback function to write data
            curl_easy_setopt(pCurl, CURLOPT_VERBOSE, true);                //Let CURL report every suddenness
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 60);                  //Setting the timeout
            curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS,0L);
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, DownProgresss);//Specify a callback function
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER,false);
            curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, true);
            nResCode = curl_easy_perform(pCurl);//Executing the above a set operation and return a status code
            curl_easy_cleanup(pCurl);           //Release the related resources
            fputs ("fopen example",pFile);
            fclose(pFile);
            AddExtraDownloadedMissions_2();
        }
    }
}

void MissionManager::AddExtraDownloadedMissions_2()
{
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+"DF_MissionsExtra_2.plist";
    cocos2d::CCDictionary* plistDictionary = cocos2d::CCDictionary::createWithContentsOfFile(path.c_str());
    
    mAllMission.clear();
    
    MissionSet* mission=new MissionSet();
    //    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement_dummy = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub_point = NULL;
    
    float aDummyVar = 0;
    
    cocos2d::CCDICT_FOREACH(plistDictionary, pElement)
    {
        cocos2d::CCDictionary* missionDict = (cocos2d::CCDictionary*)pElement->getObject();
        
        //Create new mission and set all the parametrs there !!!
        mission = new MissionSet();
        
        //The mission id in row
        mission->ID = missionDict->valueForKey("ID")->floatValue();
        CCLOG("Mission ID: %i",mission->ID);
        
        //Set the stars
        mission->Star_1 = missionDict->valueForKey("Star_1")->floatValue();
        mission->Star_2 = missionDict->valueForKey("Star_2")->floatValue();
        mission->Star_3 = missionDict->valueForKey("Star_3")->floatValue();
        
        mission->MapType = missionDict->valueForKey("MapVisualType")->floatValue();
        
        //Get the task type
        mission->Task_type = missionDict->valueForKey("Task_Type")->floatValue();
        
        //Do we need to show story
        mission->Story_show = missionDict->valueForKey("Story_Show")->floatValue();
        
        //How many caves do we allow to spawn
        mission->Dwarf_spawn_points = missionDict->valueForKey("DSP")->floatValue();
        
        //Get all the enemy paths and powas
        cocos2d::CCDictionary* enemyDict = (cocos2d::CCDictionary*)missionDict->objectForKey("Enemy_Paths");
        cocos2d::CCDICT_FOREACH(enemyDict,pElement_dummy)
        {
            MissionTroll theTroll = *new MissionTroll();
            theTroll._circle = -1;//No circle
            theTroll._pathStartIndex = -1;
            
            std::string aPathValue = pElement_dummy->getStrKey();
            
            //Check what is this for a troll !!!
            cocos2d::CCDictionary* enemySubDict = (cocos2d::CCDictionary*)enemyDict->objectForKey(aPathValue);
            int aPatrolCircle = enemySubDict->valueForKey("PatrolCircle")->intValue();
            if(aPatrolCircle==-1){
                //We have points
                cocos2d::CCDictionary* enemyPathPoint = (cocos2d::CCDictionary*)enemySubDict->objectForKey("PatrolPoints");
                cocos2d::CCDICT_FOREACH(enemyPathPoint,pElement_dummy_sub_point)
                {
                    //Starts to read from back
                    std::string aPathValue = pElement_dummy_sub_point->getStrKey();
                    cocos2d::CCString aPointValue = *enemyPathPoint->valueForKey(aPathValue);
                    
                    cocos2d::CCPoint ret = cocos2d::CCPointZero;
                    char *token = std::strtok(const_cast<char*>(aPointValue.getCString()), ",");
                    while (token != NULL) {
                        if(ret.x == 0) ret.x = std::atoi(token);
                        else ret.y = std::atoi(token);
                        token = std::strtok(NULL, " ");
                    }
                    
                    theTroll._paths.push_back(new cocos2d::CCPoint(ret.x, ret.y));
                }
            }
            else{
                theTroll._circle = aPatrolCircle;
                
                theTroll._circle_x = enemySubDict->valueForKey("PatrolCircle_CenterX")->intValue();
                theTroll._circle_y = enemySubDict->valueForKey("PatrolCircle_CenterY")->intValue();
                theTroll._circle_radius = enemySubDict->valueForKey("PatrolCircle_Radius")->intValue();
                theTroll._circle_precision = enemySubDict->valueForKey("PatrolCircle_Precision")->intValue();
                theTroll._circle_height = enemySubDict->valueForKey("PatrolCircle_Height")->intValue();
            }
            
            //Troll speed
            aDummyVar = enemySubDict->valueForKey("Speed")->floatValue();
            if(aDummyVar>0){
                theTroll._speed = aDummyVar;
            }
            else{
                theTroll._speed = 30.0;
            }
            
            theTroll._pathStartIndex = enemySubDict->valueForKey("PatrolStartPoint")->intValue();
            theTroll._startDirection = enemySubDict->valueForKey("PatrolStartDir")->intValue();
            
            //------------------------------------------------------------------------------------------
            // Check if has any radar stuff
            
            theTroll._radar_radius = 0;
            theTroll._radar_width = 0;
            
            aDummyVar = enemySubDict->valueForKey("Radar_Width")->floatValue();
            if(aDummyVar > 0){
                theTroll._radar_width = aDummyVar;
            }
            aDummyVar = enemySubDict->valueForKey("Radar_Radius")->floatValue();
            if(aDummyVar > 0){
                theTroll._radar_radius = aDummyVar;
            }
            
            //------------------------------------------------------------------------------------------
            
            mission->Enemy_info.push_back(theTroll);
        }
        
        //The dwarf spawn points forced
        cocos2d::CCDictionary* dwarfDict = (cocos2d::CCDictionary*)missionDict->objectForKey("DwarfSpawnPoints");
        cocos2d::CCDICT_FOREACH(dwarfDict,pElement_dummy)
        {
            std::string aPathValue = pElement_dummy->getStrKey();
            int aValue = dwarfDict->valueForKey(aPathValue)->intValue();
            mission->Dwarf_paths.push_back(aValue);
        }
        
        mission->StartDwarfCount = missionDict->valueForKey("StartDwarfCount")->floatValue();
        
        //        mission->DwarfCount_Min = missionDict->valueForKey("Map_DwarfCoun_Min")->floatValue();
        //        mission->DwarfCount_Max = missionDict->valueForKey("Map_DwarfCoun_Max")->floatValue();
        
        //The cave info!!!
        cocos2d::CCDictionary* caveBlueDict = (cocos2d::CCDictionary*)missionDict->objectForKey("BlueCave");
        mission->BlueCave_x = caveBlueDict->valueForKey("position_x")->intValue();
        mission->BlueCave_y = caveBlueDict->valueForKey("position_y")->intValue();
        
        cocos2d::CCDictionary* caveOrangeDict = (cocos2d::CCDictionary*)missionDict->objectForKey("OrangeCave");
        mission->OrangeCave_x = caveOrangeDict->valueForKey("position_x")->intValue();
        mission->OrangeCave_y = caveOrangeDict->valueForKey("position_y")->intValue();
        
        //---------------------------------------------------------------------------------------------------
        //New stuff || Dwarf Spawn Control ||
        
        mission->DSpawn_jump = missionDict->valueForKey("DSpawn_jump")->intValue();
        mission->DSpawn_change_jump_time = missionDict->valueForKey("DSpawn_change_jump_time")->intValue();
        mission->DSpawn_change_jump_value = missionDict->valueForKey("DSpawn_change_jump_value")->intValue();
        
        mission->DSpawn_max = missionDict->valueForKey("DSpawn_max")->intValue();
        mission->DSpawn_change_max_time = missionDict->valueForKey("DSpawn_change_max_time")->intValue();
        mission->DSpawn_change_max_value = missionDict->valueForKey("DSpawn_change_max_value")->intValue();
        
        mission->DSpawn_min = missionDict->valueForKey("DSpawn_min")->intValue();
        mission->DSpawn_change_min_time = missionDict->valueForKey("DSpawn_change_min_time")->intValue();
        mission->DSpawn_change_min_value = missionDict->valueForKey("DSpawn_change_min_value")->intValue();
        
        mission->DSpawn_zone = missionDict->valueForKey("DSpawn_zone")->intValue();
        mission->DSpawn_change_zone_time = missionDict->valueForKey("DSpawn_change_zone_time")->intValue();
        mission->DSpawn_change_zone_value = missionDict->valueForKey("DSpawn_change_zone_value")->intValue();
        
        mission->DSpawn_zone_step = missionDict->valueForKey("DSpawn_zone_step")->intValue();
        if(mission->DSpawn_zone_step == 0){
            mission->DSpawn_zone_step = 1;//Set to 1sec
        }
        
        //---------------------------------------------------------------------------------------------------
        // Crystal spawn stuff
        
        //        const char* test = missionDict->valueForKey("ItemInterval")->getCString();
        std::vector<int> crystalMinMaxSpawn = SplitString(missionDict->valueForKey("ItemInterval")->getCString(),',');
        if(crystalMinMaxSpawn.size()>=1){
            mission->ItemInterval_Min = crystalMinMaxSpawn[0];
            mission->ItemInterval_Max = crystalMinMaxSpawn[1];
        }
        else{
            mission->ItemInterval_Min = 1;
            mission->ItemInterval_Max = 1;
        }
        
        mission->ItemProbMultiplier = missionDict->valueForKey("ItemProbMultiplier")->intValue();
        mission->ItemTimeOnMap = missionDict->valueForKey("ItemTimeOnMap")->intValue();
        
        mission->CrystalColProbs = SplitString(missionDict->valueForKey("CrystalColProbs")->getCString(),',');
        mission->ItemNumProbs = SplitString(missionDict->valueForKey("ItemNumProbs")->getCString(),',');
        mission->ItemTypeProbs = SplitString(missionDict->valueForKey("ItemTypeProbs")->getCString(),',');
        
        //---------------------------------------------------------------------------------------------------
        
        //Dwarf Speed on map
        mission->DwarfSpeed_Fat = 40.0;
        mission->DwarfSpeed_Tall = 40.0;
        
        float aDwarfSpeedDummy = missionDict->valueForKey("DwarfSpeed_Fat")->floatValue();
        if(aDwarfSpeedDummy>0){
            mission->DwarfSpeed_Fat = aDwarfSpeedDummy;
        }
        
        aDwarfSpeedDummy = missionDict->valueForKey("DwarfSpeed_Tall")->floatValue();
        if(aDwarfSpeedDummy>0){
            mission->DwarfSpeed_Tall = aDwarfSpeedDummy;
        }
        
        //------------------
        
        //How much possibility to spawn dwarf from one cave
        mission->DwarfSpawnOneCave = 100;//100% allow for now
        int aSpawnOnceValue = missionDict->valueForKey("DwarfSpawnOneCave")->intValue();
        if(aSpawnOnceValue>0){
            mission->DwarfSpawnOneCave = aSpawnOnceValue;
        }
        
        //The formula type
        mission->DSpawn_formula_type = 0;//Min to map
        aSpawnOnceValue = missionDict->valueForKey("DSpawn_formula_type")->intValue();
        if(aSpawnOnceValue>0){
            mission->DSpawn_formula_type = aSpawnOnceValue;
        }
        
        //Add to all missions parsed
        mAllMission.push_back(*mission);
        //Check if completed or whats the score?
        
    }
    
    std::sort(mAllMission.begin(), mAllMission.end(),sortByID);
    
    mAllFinished = true;
    
    //Create popup for wait !!!
    cocos2d::CCMessageBox("Mission file Extra 2 downloaded","Continue");
}

void MissionManager::GetExtraMissions_3()
{
    CURL *pCurl;
    CURLcode nResCode;
    
    pCurl = curl_easy_init();//Initialize the CURL has initialized after the success of the CURL pointer
    if (pCurl != NULL)
    {
        std::string saveFileName;
        saveFileName = "DF_MissionsExtra_3.plist";
        saveFileName = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + saveFileName;
        
        pFile = fopen(saveFileName.c_str(), "w+");
        curl_easy_setopt(pCurl,CURLOPT_URL,"https://www.dropbox.com/s/n65wo794dltehb8/DF_Missions_E3.xml?dl=1");
        if(pFile != NULL)
        {
            curl_easy_setopt(pCurl,CURLOPT_FILE,pFile);                   //The specified file write
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, pWriteCallback);//Callback function to write data
            curl_easy_setopt(pCurl, CURLOPT_VERBOSE, true);                //Let CURL report every suddenness
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 60);                  //Setting the timeout
            curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS,0L);
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, DownProgresss);//Specify a callback function
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER,false);
            curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, true);
            nResCode = curl_easy_perform(pCurl);//Executing the above a set operation and return a status code
            curl_easy_cleanup(pCurl);           //Release the related resources
            fputs ("fopen example",pFile);
            fclose(pFile);
            AddExtraDownloadedMissions_3();
        }
    }
}

void MissionManager::AddExtraDownloadedMissions_3()
{
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+"DF_MissionsExtra_3.plist";
    cocos2d::CCDictionary* plistDictionary = cocos2d::CCDictionary::createWithContentsOfFile(path.c_str());
    
    mAllMission.clear();
    
    MissionSet* mission=new MissionSet();
    //    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement_dummy = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub_point = NULL;
    
    float aDummyVar = 0;
    
    cocos2d::CCDICT_FOREACH(plistDictionary, pElement)
    {
        cocos2d::CCDictionary* missionDict = (cocos2d::CCDictionary*)pElement->getObject();
        
        //Create new mission and set all the parametrs there !!!
        mission = new MissionSet();
        
        //The mission id in row
        mission->ID = missionDict->valueForKey("ID")->floatValue();
        CCLOG("Mission ID: %i",mission->ID);
        
        //Set the stars
        mission->Star_1 = missionDict->valueForKey("Star_1")->floatValue();
        mission->Star_2 = missionDict->valueForKey("Star_2")->floatValue();
        mission->Star_3 = missionDict->valueForKey("Star_3")->floatValue();
        
        mission->MapType = missionDict->valueForKey("MapVisualType")->floatValue();
        
        //Get the task type
        mission->Task_type = missionDict->valueForKey("Task_Type")->floatValue();
        
        //Do we need to show story
        mission->Story_show = missionDict->valueForKey("Story_Show")->floatValue();
        
        //How many caves do we allow to spawn
        mission->Dwarf_spawn_points = missionDict->valueForKey("DSP")->floatValue();
        
        //Get all the enemy paths and powas
        cocos2d::CCDictionary* enemyDict = (cocos2d::CCDictionary*)missionDict->objectForKey("Enemy_Paths");
        cocos2d::CCDICT_FOREACH(enemyDict,pElement_dummy)
        {
            MissionTroll theTroll = *new MissionTroll();
            theTroll._circle = -1;//No circle
            theTroll._pathStartIndex = -1;
            
            std::string aPathValue = pElement_dummy->getStrKey();
            
            //Check what is this for a troll !!!
            cocos2d::CCDictionary* enemySubDict = (cocos2d::CCDictionary*)enemyDict->objectForKey(aPathValue);
            int aPatrolCircle = enemySubDict->valueForKey("PatrolCircle")->intValue();
            if(aPatrolCircle==-1){
                //We have points
                cocos2d::CCDictionary* enemyPathPoint = (cocos2d::CCDictionary*)enemySubDict->objectForKey("PatrolPoints");
                cocos2d::CCDICT_FOREACH(enemyPathPoint,pElement_dummy_sub_point)
                {
                    //Starts to read from back
                    std::string aPathValue = pElement_dummy_sub_point->getStrKey();
                    cocos2d::CCString aPointValue = *enemyPathPoint->valueForKey(aPathValue);
                    
                    cocos2d::CCPoint ret = cocos2d::CCPointZero;
                    char *token = std::strtok(const_cast<char*>(aPointValue.getCString()), ",");
                    while (token != NULL) {
                        if(ret.x == 0) ret.x = std::atoi(token);
                        else ret.y = std::atoi(token);
                        token = std::strtok(NULL, " ");
                    }
                    
                    theTroll._paths.push_back(new cocos2d::CCPoint(ret.x, ret.y));
                }
            }
            else{
                theTroll._circle = aPatrolCircle;
                
                theTroll._circle_x = enemySubDict->valueForKey("PatrolCircle_CenterX")->intValue();
                theTroll._circle_y = enemySubDict->valueForKey("PatrolCircle_CenterY")->intValue();
                theTroll._circle_radius = enemySubDict->valueForKey("PatrolCircle_Radius")->intValue();
                theTroll._circle_precision = enemySubDict->valueForKey("PatrolCircle_Precision")->intValue();
                theTroll._circle_height = enemySubDict->valueForKey("PatrolCircle_Height")->intValue();
            }
            
            //Troll speed
            aDummyVar = enemySubDict->valueForKey("Speed")->floatValue();
            if(aDummyVar>0){
                theTroll._speed = aDummyVar;
            }
            else{
                theTroll._speed = 30.0;
            }
            
            theTroll._pathStartIndex = enemySubDict->valueForKey("PatrolStartPoint")->intValue();
            theTroll._startDirection = enemySubDict->valueForKey("PatrolStartDir")->intValue();
            
            //------------------------------------------------------------------------------------------
            // Check if has any radar stuff
            
            theTroll._radar_radius = 0;
            theTroll._radar_width = 0;
            
            aDummyVar = enemySubDict->valueForKey("Radar_Width")->floatValue();
            if(aDummyVar > 0){
                theTroll._radar_width = aDummyVar;
            }
            aDummyVar = enemySubDict->valueForKey("Radar_Radius")->floatValue();
            if(aDummyVar > 0){
                theTroll._radar_radius = aDummyVar;
            }
            
            //------------------------------------------------------------------------------------------
            
            mission->Enemy_info.push_back(theTroll);
        }
        
        //The dwarf spawn points forced
        cocos2d::CCDictionary* dwarfDict = (cocos2d::CCDictionary*)missionDict->objectForKey("DwarfSpawnPoints");
        cocos2d::CCDICT_FOREACH(dwarfDict,pElement_dummy)
        {
            std::string aPathValue = pElement_dummy->getStrKey();
            int aValue = dwarfDict->valueForKey(aPathValue)->intValue();
            mission->Dwarf_paths.push_back(aValue);
        }
        
        mission->StartDwarfCount = missionDict->valueForKey("StartDwarfCount")->floatValue();
        
        //        mission->DwarfCount_Min = missionDict->valueForKey("Map_DwarfCoun_Min")->floatValue();
        //        mission->DwarfCount_Max = missionDict->valueForKey("Map_DwarfCoun_Max")->floatValue();
        
        //The cave info!!!
        cocos2d::CCDictionary* caveBlueDict = (cocos2d::CCDictionary*)missionDict->objectForKey("BlueCave");
        mission->BlueCave_x = caveBlueDict->valueForKey("position_x")->intValue();
        mission->BlueCave_y = caveBlueDict->valueForKey("position_y")->intValue();
        
        cocos2d::CCDictionary* caveOrangeDict = (cocos2d::CCDictionary*)missionDict->objectForKey("OrangeCave");
        mission->OrangeCave_x = caveOrangeDict->valueForKey("position_x")->intValue();
        mission->OrangeCave_y = caveOrangeDict->valueForKey("position_y")->intValue();
        
        //---------------------------------------------------------------------------------------------------
        //New stuff || Dwarf Spawn Control ||
        
        mission->DSpawn_jump = missionDict->valueForKey("DSpawn_jump")->intValue();
        mission->DSpawn_change_jump_time = missionDict->valueForKey("DSpawn_change_jump_time")->intValue();
        mission->DSpawn_change_jump_value = missionDict->valueForKey("DSpawn_change_jump_value")->intValue();
        
        mission->DSpawn_max = missionDict->valueForKey("DSpawn_max")->intValue();
        mission->DSpawn_change_max_time = missionDict->valueForKey("DSpawn_change_max_time")->intValue();
        mission->DSpawn_change_max_value = missionDict->valueForKey("DSpawn_change_max_value")->intValue();
        
        mission->DSpawn_min = missionDict->valueForKey("DSpawn_min")->intValue();
        mission->DSpawn_change_min_time = missionDict->valueForKey("DSpawn_change_min_time")->intValue();
        mission->DSpawn_change_min_value = missionDict->valueForKey("DSpawn_change_min_value")->intValue();
        
        mission->DSpawn_zone = missionDict->valueForKey("DSpawn_zone")->intValue();
        mission->DSpawn_change_zone_time = missionDict->valueForKey("DSpawn_change_zone_time")->intValue();
        mission->DSpawn_change_zone_value = missionDict->valueForKey("DSpawn_change_zone_value")->intValue();
        
        mission->DSpawn_zone_step = missionDict->valueForKey("DSpawn_zone_step")->intValue();
        if(mission->DSpawn_zone_step == 0){
            mission->DSpawn_zone_step = 1;//Set to 1sec
        }
        
        //---------------------------------------------------------------------------------------------------
        // Crystal spawn stuff
        
        //        const char* test = missionDict->valueForKey("ItemInterval")->getCString();
        std::vector<int> crystalMinMaxSpawn = SplitString(missionDict->valueForKey("ItemInterval")->getCString(),',');
        if(crystalMinMaxSpawn.size()>=1){
            mission->ItemInterval_Min = crystalMinMaxSpawn[0];
            mission->ItemInterval_Max = crystalMinMaxSpawn[1];
        }
        else{
            mission->ItemInterval_Min = 1;
            mission->ItemInterval_Max = 1;
        }
        
        mission->ItemProbMultiplier = missionDict->valueForKey("ItemProbMultiplier")->intValue();
        mission->ItemTimeOnMap = missionDict->valueForKey("ItemTimeOnMap")->intValue();
        
        mission->CrystalColProbs = SplitString(missionDict->valueForKey("CrystalColProbs")->getCString(),',');
        mission->ItemNumProbs = SplitString(missionDict->valueForKey("ItemNumProbs")->getCString(),',');
        mission->ItemTypeProbs = SplitString(missionDict->valueForKey("ItemTypeProbs")->getCString(),',');
        
        //---------------------------------------------------------------------------------------------------
        
        //Dwarf Speed on map
        mission->DwarfSpeed_Fat = 40.0;
        mission->DwarfSpeed_Tall = 40.0;
        
        float aDwarfSpeedDummy = missionDict->valueForKey("DwarfSpeed_Fat")->floatValue();
        if(aDwarfSpeedDummy>0){
            mission->DwarfSpeed_Fat = aDwarfSpeedDummy;
        }
        
        aDwarfSpeedDummy = missionDict->valueForKey("DwarfSpeed_Tall")->floatValue();
        if(aDwarfSpeedDummy>0){
            mission->DwarfSpeed_Tall = aDwarfSpeedDummy;
        }
        
        //------------------
        
        //How much possibility to spawn dwarf from one cave
        mission->DwarfSpawnOneCave = 100;//100% allow for now
        int aSpawnOnceValue = missionDict->valueForKey("DwarfSpawnOneCave")->intValue();
        if(aSpawnOnceValue>0){
            mission->DwarfSpawnOneCave = aSpawnOnceValue;
        }
        
        //The formula type
        mission->DSpawn_formula_type = 0;//Min to map
        aSpawnOnceValue = missionDict->valueForKey("DSpawn_formula_type")->intValue();
        if(aSpawnOnceValue>0){
            mission->DSpawn_formula_type = aSpawnOnceValue;
        }
        
        //Add to all missions parsed
        mAllMission.push_back(*mission);
        //Check if completed or whats the score?
        
    }
    
    std::sort(mAllMission.begin(), mAllMission.end(),sortByID);
    
    mAllFinished = true;
    
    //Create popup for wait !!!
    cocos2d::CCMessageBox("Mission file Extra 3 downloaded","Continue");
}

void MissionManager::OnDownloadSpecialMissions()
{
    CURL *pCurl;
    CURLcode nResCode;
    
    pCurl = curl_easy_init();//Initialize the CURL has initialized after the success of the CURL pointer
    if (pCurl != NULL)
    {
        std::string saveFileName;
        saveFileName = "DF_MissionsTest.plist";
        saveFileName = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + saveFileName;
        
        pFile = fopen(saveFileName.c_str(), "w+");
        
        curl_easy_setopt(pCurl,CURLOPT_URL,"https://www.dropbox.com/s/0t58p07139vxieb/DF_Missions_TEST.xml?dl=1");
        if(pFile != NULL)
        {
            curl_easy_setopt(pCurl,CURLOPT_FILE,pFile);                   //The specified file write
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, pWriteCallback);//Callback function to write data
            curl_easy_setopt(pCurl, CURLOPT_VERBOSE, true);                //Let CURL report every suddenness
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 60);                  //Setting the timeout
            curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS,0L);
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, DownProgresss);//Specify a callback function
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER,false);
            curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, true);
            nResCode = curl_easy_perform(pCurl);//Executing the above a set operation and return a status code
            curl_easy_cleanup(pCurl);           //Release the related resources
            fputs ("fopen example",pFile);
            fclose(pFile);
            OnDownloadedSpecial();
        }
        
    }
}

void MissionManager::OnDownloadedSpecial()
{
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+"DF_MissionsTest.plist";
    
    cocos2d::CCDictionary* plistDictionary = cocos2d::CCDictionary::createWithContentsOfFile(path.c_str());
    
    mCurrentActiveMission = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("ActiveMission", 0);
    mCurrentActiveMission = 0;
    
    mAllMission.clear();
    
    MissionSet* mission=new MissionSet();
    //    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pElement_dummy = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub = NULL;
    cocos2d::CCDictElement* pElement_dummy_sub_point = NULL;
    
    float aDummyVar = 0;
    bool aSpawnDifferentEnemy = false;
    
    cocos2d::CCDICT_FOREACH(plistDictionary, pElement)
    {
        cocos2d::CCDictionary* missionDict = (cocos2d::CCDictionary*)pElement->getObject();
        
        aSpawnDifferentEnemy = false;
        
        //Create new mission and set all the parametrs there !!!
        mission = new MissionSet();
        
        //The mission id in row
        mission->ID = missionDict->valueForKey("ID")->floatValue();
        CCLOG("Mission ID: %i",mission->ID);
        
        //Set the stars
        mission->Star_1 = missionDict->valueForKey("Star_1")->floatValue();
        mission->Star_2 = missionDict->valueForKey("Star_2")->floatValue();
        mission->Star_3 = missionDict->valueForKey("Star_3")->floatValue();
        
        mission->MapType = missionDict->valueForKey("MapVisualType")->floatValue();
        
        //Get the task type
        mission->Task_type = missionDict->valueForKey("Task_Type")->floatValue();
        
        //Do we need to show story
        mission->Story_show = missionDict->valueForKey("Story_Show")->floatValue();
        
        //How many caves do we allow to spawn
        mission->Dwarf_spawn_points = missionDict->valueForKey("DSP")->floatValue();
        
        // Check what type of enemy is this?
        aDummyVar = missionDict->valueForKey("Enemy_Max_On_Map")->floatValue();
        
        if(aDummyVar>=1){
            aSpawnDifferentEnemy = true;// New stuff
            mission->MaxEnemy_OnMap = aDummyVar; // Whats the max enemy on map count?
        }
        
        mission->DifferentEnemySpawn = aSpawnDifferentEnemy;
        
        int aRealSpawnIndex = 0;
        
        //Get all the enemy paths and powas
        cocos2d::CCDictionary* enemyDict = (cocos2d::CCDictionary*)missionDict->objectForKey("Enemy_Paths");
        cocos2d::CCDICT_FOREACH(enemyDict,pElement_dummy)
        {
            MissionTroll theTroll = *new MissionTroll();
            theTroll._circle = -1;//No circle
            theTroll._pathStartIndex = -1;
            
            std::string aPathValue = pElement_dummy->getStrKey();
            
            //Check what is this for a troll !!!
            cocos2d::CCDictionary* enemySubDict = (cocos2d::CCDictionary*)enemyDict->objectForKey(aPathValue);
            int aPatrolCircle = enemySubDict->valueForKey("PatrolCircle")->intValue();
            if(aPatrolCircle==-1){
                //We have points
                cocos2d::CCDictionary* enemyPathPoint = (cocos2d::CCDictionary*)enemySubDict->objectForKey("PatrolPoints");
                cocos2d::CCDICT_FOREACH(enemyPathPoint,pElement_dummy_sub_point)
                {
                    //Starts to read from back
                    std::string aPathValue = pElement_dummy_sub_point->getStrKey();
                    cocos2d::CCString aPointValue = *enemyPathPoint->valueForKey(aPathValue);
                    
                    cocos2d::CCPoint ret = cocos2d::CCPointZero;
                    char *token = std::strtok(const_cast<char*>(aPointValue.getCString()), ",");
                    while (token != NULL) {
                        if(ret.x == 0) ret.x = std::atoi(token);
                        else ret.y = std::atoi(token);
                        token = std::strtok(NULL, " ");
                    }
                    
                    theTroll._paths.push_back(new cocos2d::CCPoint(ret.x, ret.y));
                }
            }
            else{
                theTroll._circle = aPatrolCircle;
                
                theTroll._circle_x = enemySubDict->valueForKey("PatrolCircle_CenterX")->intValue();
                theTroll._circle_y = enemySubDict->valueForKey("PatrolCircle_CenterY")->intValue();
                theTroll._circle_radius = enemySubDict->valueForKey("PatrolCircle_Radius")->intValue();
                theTroll._circle_precision = enemySubDict->valueForKey("PatrolCircle_Precision")->intValue();
                theTroll._circle_height = enemySubDict->valueForKey("PatrolCircle_Height")->intValue();
            }
            
            //Troll speed
            aDummyVar = enemySubDict->valueForKey("Speed")->floatValue();
            if(aDummyVar>0){
                theTroll._speed = aDummyVar;
            }
            else{
                theTroll._speed = 30.0;
            }
            
            // Split the name for the index !!!
            theTroll._indexID = aRealSpawnIndex;
            aRealSpawnIndex += 1;
            
            theTroll._pathStartIndex = enemySubDict->valueForKey("PatrolStartPoint")->intValue();
            theTroll._startDirection = enemySubDict->valueForKey("PatrolStartDir")->intValue();
            
            //------------------------------------------------------------------------------------------
            // Check if has any radar stuff
            
            theTroll._radar_radius = 0;
            theTroll._radar_width = 0;
            
            aDummyVar = enemySubDict->valueForKey("Radar_Width")->floatValue();
            if(aDummyVar > 0){
                theTroll._radar_width = aDummyVar;
            }
            aDummyVar = enemySubDict->valueForKey("Radar_Radius")->floatValue();
            if(aDummyVar > 0){
                theTroll._radar_radius = aDummyVar;
            }
            
            // New stuff
            aDummyVar = enemySubDict->valueForKey("EnemyID")->floatValue();
            theTroll._enemySpawnID = 0;// What type of enemy is this
            
            if(aDummyVar > 0){
                theTroll._enemySpawnID = aDummyVar;
            }
            
            aDummyVar = enemySubDict->valueForKey("TimeOnMap")->floatValue();
            theTroll._timeOnMap = -1;// What type of enemy is this
            
            if(aDummyVar > 0){
                theTroll._timeOnMap = aDummyVar;
            }
            
            // Bee stuff
            if(theTroll._enemySpawnID == 1){
                theTroll._beeBulletSpeed = 1;
                aDummyVar = enemySubDict->valueForKey("BulletSpeed")->floatValue();
                if(aDummyVar>0){
                    theTroll._beeBulletSpeed = aDummyVar;
                }
            }
            
            
            //------------------------------------------------------------------------------------------
            
            mission->Enemy_info.push_back(theTroll);
        }
        
        //The dwarf spawn points forced
        cocos2d::CCDictionary* dwarfDict = (cocos2d::CCDictionary*)missionDict->objectForKey("DwarfSpawnPoints");
        cocos2d::CCDICT_FOREACH(dwarfDict,pElement_dummy)
        {
            std::string aPathValue = pElement_dummy->getStrKey();
            int aValue = dwarfDict->valueForKey(aPathValue)->intValue();
            mission->Dwarf_paths.push_back(aValue);
        }
        
        mission->StartDwarfCount = missionDict->valueForKey("StartDwarfCount")->floatValue();
        
        //        mission->DwarfCount_Min = missionDict->valueForKey("Map_DwarfCoun_Min")->floatValue();
        //        mission->DwarfCount_Max = missionDict->valueForKey("Map_DwarfCoun_Max")->floatValue();
        
        //The cave info!!!
        cocos2d::CCDictionary* caveBlueDict = (cocos2d::CCDictionary*)missionDict->objectForKey("BlueCave");
        mission->BlueCave_x = caveBlueDict->valueForKey("position_x")->intValue();
        mission->BlueCave_y = caveBlueDict->valueForKey("position_y")->intValue();
        
        cocos2d::CCDictionary* caveOrangeDict = (cocos2d::CCDictionary*)missionDict->objectForKey("OrangeCave");
        mission->OrangeCave_x = caveOrangeDict->valueForKey("position_x")->intValue();
        mission->OrangeCave_y = caveOrangeDict->valueForKey("position_y")->intValue();
        
        //---------------------------------------------------------------------------------------------------
        //New stuff || Dwarf Spawn Control ||
        
        mission->DSpawn_jump = missionDict->valueForKey("DSpawn_jump")->intValue();
        mission->DSpawn_change_jump_time = missionDict->valueForKey("DSpawn_change_jump_time")->intValue();
        mission->DSpawn_change_jump_value = missionDict->valueForKey("DSpawn_change_jump_value")->intValue();
        
        mission->DSpawn_max = missionDict->valueForKey("DSpawn_max")->intValue();
        mission->DSpawn_change_max_time = missionDict->valueForKey("DSpawn_change_max_time")->intValue();
        mission->DSpawn_change_max_value = missionDict->valueForKey("DSpawn_change_max_value")->intValue();
        
        mission->DSpawn_min = missionDict->valueForKey("DSpawn_min")->intValue();
        mission->DSpawn_change_min_time = missionDict->valueForKey("DSpawn_change_min_time")->intValue();
        mission->DSpawn_change_min_value = missionDict->valueForKey("DSpawn_change_min_value")->intValue();
        
        mission->DSpawn_zone = missionDict->valueForKey("DSpawn_zone")->intValue();
        mission->DSpawn_change_zone_time = missionDict->valueForKey("DSpawn_change_zone_time")->intValue();
        mission->DSpawn_change_zone_value = missionDict->valueForKey("DSpawn_change_zone_value")->intValue();
        
        // The limit of max dwarfs !!!
        mission->DSpawn_max_limit = 0;
        aDummyVar = missionDict->valueForKey("DSpawn_change_zone_value")->intValue();
        if(aDummyVar>0){
            mission->DSpawn_max_limit = aDummyVar;
        }
        
        
        mission->DSpawn_zone_step = missionDict->valueForKey("DSpawn_zone_step")->intValue();
        if(mission->DSpawn_zone_step == 0){
            mission->DSpawn_zone_step = 1;//Set to 1sec
        }
        
        //---------------------------------------------------------------------------------------------------
        // Crystal spawn stuff
        
        //        const char* test = missionDict->valueForKey("ItemInterval")->getCString();
        std::vector<int> crystalMinMaxSpawn = SplitString(missionDict->valueForKey("ItemInterval")->getCString(),',');
        if(crystalMinMaxSpawn.size()>=1){
            mission->ItemInterval_Min = crystalMinMaxSpawn[0];
            mission->ItemInterval_Max = crystalMinMaxSpawn[1];
        }
        else{
            mission->ItemInterval_Min = 1;
            mission->ItemInterval_Max = 1;
        }
        
        mission->ItemProbMultiplier = missionDict->valueForKey("ItemProbMultiplier")->intValue();
        mission->ItemTimeOnMap = missionDict->valueForKey("ItemTimeOnMap")->intValue();
        
        
        mission->CrystalColProbs = SplitString(missionDict->valueForKey("CrystalColProbs")->getCString(),',');
        mission->ItemNumProbs = SplitString(missionDict->valueForKey("ItemNumProbs")->getCString(),',');
        mission->ItemTypeProbs = SplitString(missionDict->valueForKey("ItemTypeProbs")->getCString(),',');
        
        
        //---------------------------------------------------------------------------------------------------
        
        // TEST STUFF
        // The bee stuff
        
        aDummyVar = missionDict->valueForKey("Enable_Bee")->floatValue();
        if(aDummyVar>=1){
            // Yes please enable it
            mission->SpawnBee = missionDict->valueForKey("Enable_Bee")->floatValue();
            
            // Set some bee parametrs
            std::vector<int> BeeStartPoint = SplitString(missionDict->valueForKey("Enemy_Bee_Start")->getCString(),',');
            
            mission->Enemy_Bee_StartX = BeeStartPoint[0];
            mission->Enemy_Bee_StartY = BeeStartPoint[1];
            
            std::vector<int> BeeFinishPoint = SplitString(missionDict->valueForKey("Enemy_Bee_Finish")->getCString(),',');
            
            mission->Enemy_Bee_FinishX = BeeFinishPoint[0];
            mission->Enemy_Bee_FinishY = BeeFinishPoint[1];
            
            mission->Enemy_Bee_Speed = missionDict->valueForKey("Enemy_Bee_Speed")->floatValue();
            mission->Enemy_Bee_Bullet_Speed = missionDict->valueForKey("Enemy_Bee_Bullet_Speed")->floatValue();
        }
        
        aDummyVar = missionDict->valueForKey("MT_Force_Bullets")->floatValue();
        if(aDummyVar>=1){
            // We will spawn bullets
            mission->Forced_Bullets = missionDict->valueForKey("MT_Force_Bullets")->floatValue();
            
//            std::vector<int> Bullet_Speeds = SplitString(missionDict->valueForKey("MT_Bullet_Speed")->getCString(),',');
//            
//            mission->MT_Bullet_Speed_Min = Bullet_Speeds[0];
//            mission->MT_Bullet_Speed_Max = Bullet_Speeds[1];
        }
        
        std::vector<int> Bullet_Speeds = SplitString(missionDict->valueForKey("MT_Bullet_Speed")->getCString(),',');
        if(Bullet_Speeds.size()>0)
        {
            mission->MT_Bullet_Speed_Min = Bullet_Speeds[0];
            mission->MT_Bullet_Speed_Max = Bullet_Speeds[1];
        }
        
        // New bullets and etc stuff
        std::vector<int> BulletTypes = SplitString(missionDict->valueForKey("MT_Bullet_Types")->getCString(),',');
        if(BulletTypes.capacity()>0)
        {
            //We have some bullets
            mission->MT_Bullet_Types = SplitString(missionDict->valueForKey("MT_Bullet_Types")->getCString(),',');;
        }
        
        aDummyVar = missionDict->valueForKey("MT_Bullet_Instatn")->floatValue();
        mission->MT_Bullet_Instant = false;
        if(aDummyVar >= 1)
        {
            mission->MT_Bullet_Instant = true;
        }
        
        // The new stuff
        std::vector<int> MT_EventTypes = SplitString(missionDict->valueForKey("MT_Event_Types")->getCString(),',');
        if(MT_EventTypes.size()>0)
        {
            // We have some traps to spawn
            mission->MT_Event_Types = SplitString(missionDict->valueForKey("MT_Event_Types")->getCString(),',');
        }
        
        aDummyVar = missionDict->valueForKey("MT_EVENT_PROCENT_BULET")->floatValue();
        mission->MT_Event_Percent_Bullet = 0;
        if(aDummyVar>0){
            mission->MT_Event_Percent_Bullet = aDummyVar;
        }
        
        aDummyVar = missionDict->valueForKey("MT_EVENT_PROCENT_TRAP")->floatValue();
        mission->MT_Event_Percent_Trap = 0;
        if(aDummyVar>0){
            mission->MT_Event_Percent_Trap = aDummyVar;
        }
        
        aDummyVar = missionDict->valueForKey("MT_EVENT_PROCENT_TROLL")->floatValue();
        mission->MT_Event_Percent_Troll = 0;
        if(aDummyVar>0){
            mission->MT_Event_Percent_Troll = aDummyVar;
        }
        
        aDummyVar = missionDict->valueForKey("MT_EVENT_PROCENT_MASS")->floatValue();
        mission->MT_Event_Percent_Mass = 0;
        if(aDummyVar>0){
            mission->MT_Event_Percent_Mass = aDummyVar;
        }
        
        // The timers for Master Troll
        std::vector<int> MT_EventTimer = SplitString(missionDict->valueForKey("MT_EVENT_TIMER")->getCString(),'-');
        mission->MT_Event_Timer_Min = 0;
        mission->MT_Event_Timer_Max = 0;
        
        if(MT_EventTimer.size()>0){
            
            // The min
            mission->MT_Event_Timer_Min = MT_EventTimer[0];
            // The max
            if(MT_EventTimer.size()>1){
                mission->MT_Event_Timer_Max = MT_EventTimer[1];
            }
        }
        
        aDummyVar = missionDict->valueForKey("MT_EVENT_VALUE_LIMIT")->floatValue();
        if(aDummyVar>0){
            mission->MT_Event_Value_Start = aDummyVar;
        }
        
        aDummyVar = missionDict->valueForKey("MT_EVENT_VALUE_ADD")->floatValue();
        mission->MT_Event_Value_Add = 0;
        if(aDummyVar>0){
            mission->MT_Event_Value_Add = aDummyVar;
        }
        
        aDummyVar = missionDict->valueForKey("MT_EVENT_VALUE_TIMER")->floatValue();
        mission->MT_Event_Value_Timer = 0;
        if(aDummyVar>0){
            mission->MT_Event_Value_Timer = aDummyVar;
        }
        
        
        //Dwarf Speed on map
        mission->DwarfSpeed_Fat = 40.0;
        mission->DwarfSpeed_Tall = 40.0;
        
        float aDwarfSpeedDummy = missionDict->valueForKey("DwarfSpeed_Fat")->floatValue();
        if(aDwarfSpeedDummy>0){
            mission->DwarfSpeed_Fat = aDwarfSpeedDummy;
        }
        
        aDwarfSpeedDummy = missionDict->valueForKey("DwarfSpeed_Tall")->floatValue();
        if(aDwarfSpeedDummy>0){
            mission->DwarfSpeed_Tall = aDwarfSpeedDummy;
        }
        
        //------------------
        
        //How much possibility to spawn dwarf from one cave
        mission->DwarfSpawnOneCave = 100;//100% allow for now
        int aSpawnOnceValue = missionDict->valueForKey("DwarfSpawnOneCave")->intValue();
        if(aSpawnOnceValue>0){
            mission->DwarfSpawnOneCave = aSpawnOnceValue;
        }
        
        //The formula type
        mission->DSpawn_formula_type = 0;//Min to map
        aSpawnOnceValue = missionDict->valueForKey("DSpawn_formula_type")->intValue();
        if(aSpawnOnceValue>0){
            mission->DSpawn_formula_type = aSpawnOnceValue;
        }
        
        //Add to all missions parsed
        mAllMission.push_back(*mission);
    }
    
    std::sort(mAllMission.begin(), mAllMission.end(),sortByID);
    
    mAllFinished = true;
    
    //Create popup for wait !!!
    cocos2d::CCMessageBox("Test Mission file downloaded","Continue");
}


