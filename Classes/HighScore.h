//
//  HighScore.h
//  DwarfForest
//
//  Created by Agris Dumins on 9/29/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

namespace DF
{
    namespace LevelEnd
    {
        class HighScore: public cocos2d::CCLayer
        {
        public:
            static HighScore* create(uint32_t result, bool newRecord);
            
            bool init(uint32_t result, bool newRecord);
            
            void share();
            
            void sendFeedback();
            
            cocos2d::CCMenuItemImage* shareBt;
            int aScoreTotal;
        };
        
        class DropingCup: public cocos2d::CCLayer
        {
        public:
            CREATE_FUNC(DropingCup);
            virtual bool init();
        };
    }
}
