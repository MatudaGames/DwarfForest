//
//  GameStats.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 10/13/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "GameStats.h"


using namespace DF::Stats;


GameStats::GameStats()
: points(0)
, avoidedTrolls(0)
, avoidedMetereoits(0)
, gatheredDiamonds(0)
, dailyChallengeCompleted(false)
{
    gatheredCrystals[CRYSTAL_COLOR_GREEN] = 0;
    gatheredCrystals[CRYSTAL_COLOR_BLUE] = 0;
    gatheredCrystals[CRYSTAL_COLOR_RED] = 0;
    gatheredCrystals[CRYSTAL_COLOR_YELLOW] = 0;
    
    rescuedDwarfs[DWARF_TYPE_FAT] = 0;
    rescuedDwarfs[DWARF_TYPE_TALL] = 0;
}
