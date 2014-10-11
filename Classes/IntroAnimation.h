//
//  IntroAnimation.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 06.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

class SpriteAnimation;
class GameScene;

enum IntroAnimationType
{
	INTRO_TROLL,
	INTRO_DWARF,
	INTRO_CRYSTAL,
	INTRO_EFFECT,
	INTRO_DIAMOND
};

class IntroAnimation: public cocos2d::CCNode
{
public:	
	IntroAnimation();
    virtual ~IntroAnimation();
	
	bool init(IntroAnimationType type, GameScene* game);
	virtual void update(float delta);
	
	IntroAnimationType getType() const;
	
	void finished(CCObject* sender = NULL);
	
protected:
	GameScene* _game;
	
	IntroAnimationType _type;
	
	bool _finished;
};
