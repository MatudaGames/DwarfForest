//
//  ChrystalIntro.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 10.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IntroAnimation.h"

class GameScene;
class Crystal;

class CrystalIntro: public IntroAnimation
{
public:
	static CrystalIntro* create(GameScene* game, Crystal* crystal);
	
	CrystalIntro();
    virtual ~CrystalIntro();
	
	virtual bool init(GameScene* game, Crystal* crystal);
	virtual void update(float delta);
	
	Crystal* getCrystal() const;
    
    void CreateRealIntro();
	
private:
	SpriteAnimation* _animation;
	Crystal* _crystal;
	
	cocos2d::CCSprite* _shadowSprite;
	cocos2d::CCSprite* _crystalSprite;
	
	cocos2d::CCActionInterval* _shadowFadeIn;
	cocos2d::CCActionInterval* _crystalFadeIn;
	
	//cocos2d::CCSequence* _tweenSequence;
};
