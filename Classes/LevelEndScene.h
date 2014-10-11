//
//  LevelEndScene.h
//  DwarfForest
//
//  Created by Agris Dumins on 9/29/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>
#include <vector>
#include "ExperienceBar.h"
#include "HighScore.h"
#include "LevelEndMenu.h"
#include "WeeklyChallenges.h"
#include "GameStats.h"

#include "LevelEndMissions.h"

namespace DF
{
    namespace LevelEnd
    {
        class LevelEndScene: public cocos2d::CCLayer
        {
        public:
            static LevelEndScene* create(Stats::GameStats& stats);
            
            void addWeeklyProgress(WeeklyTask::Task task_type, int target, uint32_t progress);
            virtual bool init(Stats::GameStats& stats);
            
            static cocos2d::CCScene* scene(Stats::GameStats& stats);
            
            ExperienceBar* exp;
            HighScore* score;
            LevelEndMenu* menu;
            
            LevelEndMissions* missions;
            
            cocos2d::CCSprite* container;
            
            std::vector<cocos2d::CCLayer*> popups;
            
            bool screen_created;
            
            cocos2d::CCLayer* activePopup;
            void showNextPopup();
            void showLevelEnd();
            void fadeInLevelEnd();
            void fadeOutLevelEnd();
            void showSides();
            
            void ShowMissions();
        };
    }
}
