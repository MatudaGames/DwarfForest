//
//  LevelUpPopUp.h
//  DwarfForest
//
//  Created by Agris Dumins on 10/15/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

namespace DF
{
    namespace LevelEnd
    {
        class LevelUpPopUp :public cocos2d::CCLayer
        {
        public:
            static LevelUpPopUp* create(uint32_t level);
            
            virtual bool init(uint32_t level);
            
            void closeCallback();
            void shareCallback();
            void remove();
            
            void onEnter();
            void onExit();
            bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
        };

    }
}
