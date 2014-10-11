//
//  StaticSceneManager.h
//  DwarfForest
//
//  Created by Agris Dumins on 10/8/13.
//
//

#pragma once

#include <stdio.h>
#include <cocos2d.h>
#include <map>

namespace DF
{
    class StaticSceneManager
    {
    public:
        
        enum Scene
        {
            MAIN_MENU,
            DAILY_CHALLENGES,
            WEEKLY_CHALLENGES,
            OPTIONS,
            MISSIONS,
            STATS
        };
        
        static StaticSceneManager* getInstance();
        cocos2d::CCScene* getScene(Scene scene);
        
    private:
        
        cocos2d::CCArray* scenesContainer;
        std::map<Scene, cocos2d::CCScene*> scenes;
        
        StaticSceneManager();
        ~StaticSceneManager();
    };
}
