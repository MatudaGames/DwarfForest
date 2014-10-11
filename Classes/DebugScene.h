//
//  DebugScene.h
//  DwarfForest
//
//  Created by Agris Dumins on 10/17/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

#include "User.h"
#include "PopUp.h"

class DebugScene : public cocos2d::CCLayer
{
public:
	CREATE_FUNC(DebugScene);
    static cocos2d::CCScene* scene();
    virtual bool init();
    
    void incLevel();
    void decLevel();
    void addDiamonds();
    void back();
    
    void upgradeMachineClear();
    void downgradeMachineClear();
    void upgradeMachineGhost();
    void downgradeMachineGhost();
    void upgradeMachineEnemies();
    void downgradeMachineEnemies();
    
    void AddStart_3();
    void RemoveStart_3();
    void AddStart_3_extra();
    void RemoveStart_3_extra();
    
    void AddStart_2();
    void RemoveStart_2();
    void AddStart_2_extra();
    void RemoveStart_2_extra();
    
    void AddStart_1();
    void RemoveStart_1();
    void AddStart_1_extra();
    void RemoveStart_1_extra();
    
    void EnableFormulas();
};
