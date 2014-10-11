//
//  DiamondEntry.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 14.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IntroAnimation.h"

class GameScene;
class Diamond;

class DiamondIntro: public IntroAnimation
{
public:
	static DiamondIntro* create(GameScene* game, Diamond* diamond);
	
	DiamondIntro();
    virtual ~DiamondIntro();
	
	virtual bool init(GameScene* game, Diamond* diamond);
	virtual void update(float delta);
	
	Diamond* getDiamond() const;
	
private:
	Diamond* _diamond;
	
	cocos2d::CCSprite* _diamondSprite;
	cocos2d::CCSprite* _shadowSprite;
	
	cocos2d::CCActionInterval* _shadowFadeIn;
	cocos2d::CCActionInterval* _diamondFadeIn;
};
