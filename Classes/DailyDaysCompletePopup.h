//
//  DailyDaysCompletePopup.h
//  DwarfForest
//
//  Created by Agris Dumins on 24/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

class DailyDaysCompletePopup : public cocos2d::CCLayer
{
public:
    static DailyDaysCompletePopup* create(uint32_t days, uint32_t reward);
    bool init(uint32_t days, uint32_t reward);
    
private:
    void close();
    void share();
    void remove();
};
