//
//  StatsScene.h
//  DwarfForest
//
//  Created by Agris Dumins on 10/6/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>
#include "LevelEndScene.h"

namespace DF
{
    namespace Stats
    {
        class StatsScene: public cocos2d::CCLayer
        {
        public:
            static cocos2d::CCScene* scene();
            
            CREATE_FUNC(StatsScene);
            
            virtual bool init();
            
            void menuBack();
            void updateInfo();
            void onEnter();
            void keyBackClicked();
            
        private:
            
            DF::LevelEnd::ExperienceFill* barFill;
            cocos2d::CCLabelTTF* levelTxt;
            
            cocos2d::CCLabelTTF* highScore;
            cocos2d::CCLabelTTF* mostDwarfs;
            cocos2d::CCLabelTTF* totalGames;
            cocos2d::CCLabelTTF* totalGreen;
            cocos2d::CCLabelTTF* totalBlue;
            cocos2d::CCLabelTTF* totalRed;
            cocos2d::CCLabelTTF* totalYellow;
            cocos2d::CCLabelTTF* totalDiamonds;
            cocos2d::CCLabelTTF* totalDwarfs;
        };
    }
}
