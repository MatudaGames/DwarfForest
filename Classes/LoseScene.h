//
//  LoseScene.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 13.03.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>
#include "Crystal.h"

class LoseScene: public cocos2d::CCLayer
{
public:
	
	static LoseScene* create(const std::map<CrystalColor, int>& crystals, bool dailyChallengeCompleted);
    
	LoseScene();
    virtual ~LoseScene();
	
	virtual bool init(const std::map<CrystalColor, int>& crystals, bool dailyChallengeCompleted);
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(unsigned int points, std::map<CrystalColor, int>& crystals, bool dailyChallengeCompleted);
    
private:
	void menuBackCallback(CCObject* sender);
	
    double _points;
    int _level;
    std::map<CrystalColor, int> _gatheredCrystals;
    bool _dailyChallengeCompleted;
    
};
