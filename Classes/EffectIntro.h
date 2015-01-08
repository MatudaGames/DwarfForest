//
//  EffectIntro.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 10.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IntroAnimation.h"

class GameScene;
class Effect;
class SpriteAnimation;

class EffectIntro: public IntroAnimation
{
public:
	static EffectIntro* create(GameScene* game, Effect* effect);
	
	EffectIntro();
    virtual ~EffectIntro();
	
	virtual bool init(GameScene* game, Effect* effect);
	virtual void update(float delta);
	
	Effect* getEffect() const;
    
    void OnRainCompleted();
    void OnWebCompleted();
    void WebIntro();
    
private:
	SpriteAnimation* _animation;
	Effect* _effect;
    
    cocos2d::CCSprite* _placeSprite;
	
	cocos2d::CCSprite* _shadowSprite;
	cocos2d::CCSprite* _meteoriteSprite;
	SpriteAnimation* _collisionAnimation;//Not needed anymore
    SpriteAnimation* stickyIntroAnimation;
	SpriteAnimation* stickyGrowAnimation;
    
    cocos2d::CCSprite* _boomSprite;
	cocos2d::CCSprite* _areaSprite;
    
	cocos2d::CCActionInterval* _shadowFadeIn;
	cocos2d::CCActionInterval* _meteoriteFadeIn;
	cocos2d::CCActionInterval* _introDelay;
    
    cocos2d::CCSequence* _fallIntro;
};
