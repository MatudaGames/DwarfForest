//
//  Levels.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 7/17/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "Levels.h"
#include <cocos2d.h>

#include "User.h"

static const int MAX_LEVEL = 30;
static Levels* instance = NULL;

Levels* Levels::getInstance()
{
	if (instance == NULL)
    {
        instance = new Levels();
    }
    return instance;
}

Levels::Levels()
{
    _list.push_back(Level(0, 1.0));
    _list.push_back(Level(50, 1.0));//For now disabled level system
    _list.push_back(Level(100, 2.0));
    _list.push_back(Level(150, 3.0));
    _list.push_back(Level(200, 4.0));
    _list.push_back(Level(250, 5.0));
    _list.push_back(Level(300, 6.0));
    
    _list.push_back(Level(400, 7.0));
    _list.push_back(Level(500, 8.0));
    _list.push_back(Level(600, 9.0));
    _list.push_back(Level(700, 10.0));
    _list.push_back(Level(800, 11.0));
    _list.push_back(Level(900, 12.0));
    _list.push_back(Level(1000, 13.0));
    
    _list.push_back(Level(1200, 14.0));
    _list.push_back(Level(1400, 15.0));
    _list.push_back(Level(1600, 16.0));
    _list.push_back(Level(2000, 17.0));
    _list.push_back(Level(2200, 18.0));
    _list.push_back(Level(2400, 19.0));
    _list.push_back(Level(2600, 20.0));
    
    _list.push_back(Level(2800, 21.0));
    _list.push_back(Level(3000, 22.0));
    _list.push_back(Level(4000, 23.0));
    _list.push_back(Level(5000, 24.0));
    _list.push_back(Level(6000, 25.0));
    _list.push_back(Level(7000, 26.0));
    _list.push_back(Level(8000, 27.0));
    
    _list.push_back(Level(10000, 28.0));
    _list.push_back(Level(12000, 29.0));
    _list.push_back(Level(14000, 30.0));
    
//    list.push_back(Level(0, 1.0));
//    list.push_back(Level(2000, 1.1));
//    list.push_back(Level(5000, 1.2));
//    list.push_back(Level(9000, 1.3));
//    list.push_back(Level(14000, 1.4));
//    list.push_back(Level(20000, 1.5));
//    list.push_back(Level(27000, 1.6));
//    list.push_back(Level(35000, 1.7));
//    list.push_back(Level(44000, 1.8));
//    list.push_back(Level(54000, 1.9));
//    list.push_back(Level(65000, 2.0));
//    list.push_back(Level(77000, 2.1));
//    list.push_back(Level(90000, 2.2));
//    list.push_back(Level(104000, 2.3));
//    list.push_back(Level(119000, 2.4));
//    list.push_back(Level(135000, 2.5));
//    list.push_back(Level(152000, 2.6));
//    list.push_back(Level(170000, 2.7));
//    list.push_back(Level(189000, 2.8));
//    list.push_back(Level(209000, 2.9));
//    list.push_back(Level(230000, 3.0));
}

Level* Levels::getCurrentLevel()
{
    return &_list[User::getInstance()->getLevel()];
}

bool Levels::isNextLevelReached()
{
//    if (User::getInstance()->getLevel() != MAX_LEVEL &&
//        list[User::getInstance()->getLevel()+1].pointsNeeded <= User::getInstance()->getExperience())
    if (User::getInstance()->getLevel() != MAX_LEVEL &&
        _list[User::getInstance()->getLevel()+1].getPointsNeeded() <= User::getInstance()->getDwarfs())
    {
        return true;
    }
    return false;
}

uint32_t Levels::currentLevelPoints()
{
    return _list[User::getInstance()->getLevel()].getPointsNeeded();
}

uint32_t Levels::nextLevelPoints()
{
    if (User::getInstance()->getLevel() != MAX_LEVEL)
	{
    	return _list[User::getInstance()->getLevel()+1].getPointsNeeded();
	}
    return 0;
}

uint32_t Levels::getMaxLevel()
{
    return _list.size();
}

uint32_t Levels::getPoints(uint32_t points)
{
    return getCurrentLevel()->getPointsModifier() * points;
}

uint32_t Levels::getModifier()
{
    return getCurrentLevel()->getPointsModifier();
}
