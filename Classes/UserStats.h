//
//  UserStats.h
//  DwarfForest
//
//  Created by Agris Dumins on 10/13/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <stdint.h>
#include "GameStats.h"

namespace DF
{
    namespace Stats
    {
        class UserStats
        {
        public:
            UserStats();
            uint32_t getHighscore()         { return highscore; }
            uint32_t getMostDwarfsRescued() { return mostDwarfsRescued; }
            uint32_t getTotalGames()        { return totalGames; }
            uint32_t getTotalDwarfsrescued(){ return totalDwarfsRescued; }
            uint32_t getDiamondsCollected() { return diamondsCollected; }
            uint32_t getRedCrystals()       { return redCollected; }
            uint32_t getYellowCrystals()    { return yellowCollected; }
            uint32_t getBlueCrystals()      { return blueCollected; }
            uint32_t getGreenCrystals()     { return greenCollected; }
            
            void saveGameStats(Stats::GameStats& stats);
            
        private:
            uint32_t highscore;
            uint32_t mostDwarfsRescued;
            
            uint32_t totalGames;
            uint32_t totalDwarfsRescued;
            uint32_t diamondsCollected;
            
            uint32_t redCollected;
            uint32_t yellowCollected;
            uint32_t blueCollected;
            uint32_t greenCollected;
            
        };
    }
}
