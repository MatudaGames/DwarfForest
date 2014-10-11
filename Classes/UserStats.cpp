//
//  UserStats.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 10/13/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "UserStats.h"
#include "Dwarf.h"
#include "Crystal.h"

#include <cocos2d.h>

using namespace DF::Stats;

UserStats::UserStats()
{
    highscore = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("userHighscore", 0);
    mostDwarfsRescued = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("stats.mostDwarfs", 0);
    totalGames = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("stats.totalGames", 0);
    totalDwarfsRescued = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("stats.totalDwarfs", 0);
    diamondsCollected = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("stats.diamondsCollected", 0);
    
    redCollected = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("stats.crystals.red", 0);
    yellowCollected = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("stats.crystals.yellow", 0);
    blueCollected = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("stats.crystals.blue", 0);
    greenCollected = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("stats.crystals.green", 0);
}

void UserStats::saveGameStats(Stats::GameStats& stats)
{
    highscore = stats.points > highscore ? stats.points : highscore;
    
    uint32_t dwarfsRescued = stats.rescuedDwarfs[DWARF_TYPE_FAT] + stats.rescuedDwarfs[DWARF_TYPE_TALL];
    mostDwarfsRescued = mostDwarfsRescued > dwarfsRescued ? mostDwarfsRescued : dwarfsRescued;
    totalDwarfsRescued += dwarfsRescued;
    totalGames += 1;
    diamondsCollected += stats.gatheredDiamonds;
    
    redCollected += stats.gatheredCrystals[CRYSTAL_COLOR_RED];
    yellowCollected += stats.gatheredCrystals[CRYSTAL_COLOR_YELLOW];
    blueCollected += stats.gatheredCrystals[CRYSTAL_COLOR_BLUE];
    greenCollected += stats.gatheredCrystals[CRYSTAL_COLOR_GREEN];
    

    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userHighscore", highscore);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("stats.mostDwarfs", mostDwarfsRescued);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("stats.totalGames", totalGames);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("stats.totalDwarfs", totalDwarfsRescued);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("stats.diamondsCollected", diamondsCollected);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("stats.crystals.red", redCollected);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("stats.crystals.yellow", yellowCollected);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("stats.crystals.blue", blueCollected);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("stats.crystals.green", greenCollected);
    
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}
