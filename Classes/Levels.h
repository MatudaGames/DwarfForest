//
//  Levels.h
//  DwarfForest
//
//  Created by Agris Dumins on 7/17/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once
#include <stdint.h>
#include <vector>

class Level
{
public:
    Level(uint32_t needed, float modifier): _pointsNeeded(needed), _pointsModifier(modifier)
    {}
	
	uint32_t getPointsNeeded() const { return _pointsNeeded; }
    float getPointsModifier() const { return _pointsModifier; }
	
private:
    uint32_t _pointsNeeded;
    float _pointsModifier;
};

class Levels
{
public:
    static Levels* getInstance();
    
    Level* getCurrentLevel();
    bool isNextLevelReached();
    uint32_t nextLevelPoints();
    uint32_t currentLevelPoints();
    uint32_t getPoints(uint32_t points);
    
    uint32_t getModifier();
    uint32_t getMaxLevel();
    
private:
	Levels();
    Levels(Levels const&);          // only prevents copies
    void operator=(Levels const&);  // only prevents copies
	
	std::vector<Level> _list;
};
