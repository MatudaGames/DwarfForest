//
//  TrollIntro.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 10.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IntroAnimation.h"

class Troll;

class TrollIntro: public IntroAnimation
{
public:
	static TrollIntro* create(GameScene* game, Troll* troll);
	
	TrollIntro();
    virtual ~TrollIntro();
	
	virtual bool init(GameScene* game, Troll* troll);
	
	Troll* getTroll() const;
	
private:
	void startAnimation();
    void startTrollFade();
	
	SpriteAnimation* _animation;
	Troll* _troll;
    
    cocos2d::CCSprite* _introFog;
    cocos2d::CCSprite* _introFog_Dot;
};
