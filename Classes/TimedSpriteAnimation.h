//
//  LogoAnimation.h
//  DwarfForest
//
//  Created by Agris Dumins on 7/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

class TimedSpriteAnimation: public cocos2d::CCSprite
{
public:
	static TimedSpriteAnimation* create(const char* plist, float delay, int limit = 0);
	
	TimedSpriteAnimation(float delay,int limit);
    virtual ~TimedSpriteAnimation();
	
	virtual bool init(const char* plist);
	
	virtual void onEnter();
	virtual void onExit();
	
private:
	void onAnimFinsh(CCObject* selector);
	
	cocos2d::CCAction* _action;
    float _delay;
    
    int _frameLimit;
};

