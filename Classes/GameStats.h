//
//  GameStats.h
//  DwarfForest
//
//  Created by Agris Dumins on 10/13/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <map>
#include "Crystal.h"
#include "Dwarf.h"

namespace DF
{
    namespace Stats
    {
        class GameStats
        {
        public:
            GameStats();
            
            uint32_t points;
            uint32_t gatheredDiamonds;
            
            std::map<CrystalColor, int> gatheredCrystals;
            std::map<DwarfType, int> rescuedDwarfs;
            uint32_t avoidedTrolls;
            uint32_t avoidedMetereoits;
            
            bool dailyChallengeCompleted;
        };
    }
}
