//
//  Menu.h
//  DwarfForest
//
//  Created by Agris Dumins on 10/2/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

class UpgradesRedDot2:public cocos2d::CCLayer
{
public:
    virtual bool init();
    CREATE_FUNC(UpgradesRedDot2);
    void update();
    
    cocos2d::CCLabelTTF* count;
};

namespace DF
{
    namespace LevelEnd
    {
        class LevelEndMenu: public cocos2d::CCLayer
        {
        public:
            CREATE_FUNC(LevelEndMenu);
            
            virtual bool init();
            
            void playCall();
            void menuCall();
            void upgradeCall();
            void OnRemoveUpgrade();
            
            UpgradesRedDot2* _upgradesPoint1;
            UpgradesRedDot2* _upgradesPoint2;
        };
    }
}
