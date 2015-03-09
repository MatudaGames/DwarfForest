//
//  SpriteAnimation.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 30.03.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

class SpriteAnimation: public cocos2d::CCSprite
{
public:
	static SpriteAnimation* create(const char* plist, bool repeat = true);
	
	SpriteAnimation();
    virtual ~SpriteAnimation();
	
	virtual bool init(const char* plist, bool repeat);
	
	virtual void onEnter();
	virtual void onExit();
    
    cocos2d::CCAnimation* animation;
	
    void pause();
    void resume();
    
    bool _paused;
    
    cocos2d::CCAction* _action;
    cocos2d::CCAction* _action_reverse;
    
private:

    int nothing;
    
};
