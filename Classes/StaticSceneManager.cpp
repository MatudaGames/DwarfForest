//
//  StaticSceneManager.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 10/8/13.
//
//

#include "StaticSceneManager.h"
#include "MainMenuScene.h"
#include "DailyChallengesScene.h"
#include "WeeklyChallengesScene.h"
#include "OptionsScene.h"
#include "StatsScene.h"
#include "MissionScene.h"
#include "WorldMap.h"
#include <cocos2d.h>

USING_NS_CC;
using namespace DF;

static StaticSceneManager* instance = NULL;

StaticSceneManager::StaticSceneManager()
{
    scenesContainer = CCArray::create();
    
    scenes[MAIN_MENU] = MainMenuScene::scene();
    scenes[MAIN_MENU]->retain();
    
//    scenes[DAILY_CHALLENGES] = DailyChallengesScene::scene();
//    scenes[DAILY_CHALLENGES]->retain();
    
//    scenes[WEEKLY_CHALLENGES] = WeeklyChallengesScene::scene();
//    scenes[WEEKLY_CHALLENGES]->retain();
    
//    scenes[OPTIONS] = OptionsScene::scene();
//    scenes[OPTIONS]->retain();
    
//    scenes[STATS] = Stats::StatsScene::scene();
//    scenes[STATS]->retain();
    
//    scenes[MISSIONS] = MissionScene::scene();
    scenes[MISSIONS] = WorldMap::scene();
    scenes[MISSIONS]->retain();
}

StaticSceneManager::~StaticSceneManager()
{
    scenes[MAIN_MENU]->release();
    scenes[DAILY_CHALLENGES]->release();
    scenes[WEEKLY_CHALLENGES]->release();
    scenes[OPTIONS]->release();
    scenes[STATS]->release();
    scenes[MISSIONS]->release();
    
    instance = NULL;
}

StaticSceneManager* StaticSceneManager::getInstance()
{
    if (instance == NULL)
    {
        instance = new StaticSceneManager();
    }
    return instance;
}

CCScene* StaticSceneManager::getScene(DF::StaticSceneManager::Scene scene)
{
    return scenes[scene];
}
