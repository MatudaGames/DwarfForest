//
//  ExperienceBar.h
//  DwarfForest
//
//  Created by Agris Dumins on 9/30/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>
#include "SpriteAnimation.h"
#include "GameStats.h"

namespace DF
{
    namespace LevelEnd
    {
        class ExperienceFill: public cocos2d::CCLayer
        {
        public:
            ExperienceFill();
            
            CREATE_FUNC(ExperienceFill);
            
            virtual bool init();
            virtual void update(float delta);
            
            void fill(uint32_t points);
            
            void continueFill();
            
            cocos2d::CCGLProgram* boxShader;
            
            int xposLoc;
            float xpos;
            int brghtLoc;
            float brght;
            int contrLoc;
            float contr;
            
        private:
            
            uint32_t pointsToFill;
            uint32_t pps; // points per second
            bool _paused;
            int _contrastDirection;
            
        };
        
        class ExperienceBar: public cocos2d::CCLayer
        {
        public:
            static ExperienceBar* create(Stats::GameStats& stats);
            
            bool init(Stats::GameStats& stats);
            
            void dropTroll();
            void bumpDwarfs();
            
            void updateLevel();
            
            cocos2d::CCLabelTTF* levelTxt;
            
            SpriteAnimation* troll;
            cocos2d::CCSprite* trollContainer;
            ExperienceFill* barFill;
            
            cocos2d::CCSprite* fat;
            SpriteAnimation* fatStars;
            cocos2d::CCSprite* tall;
            SpriteAnimation* tallStars;
            
        private:
            
            uint32_t pointsToFill;
        };
        
        
    }
}
