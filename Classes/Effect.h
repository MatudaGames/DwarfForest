//
//  Effect.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 17.03.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

enum EffectType
{
	EFFECT_TYPE_ICE_BARRAGE,
	EFFECT_TYPE_WEB,
    EFFECT_TYPE_RAIN,
    EFFECT_TYPE_WIND,
	EFFECT_TYPE_COUNT
};

class GameScene;
class Dwarf;
class Troll;

class Effect: public cocos2d::CCNode
{
public:
	Effect();
    virtual ~Effect();
	
	virtual bool init(GameScene* gameScene);
	
	EffectType getType() const { return _effectType; }
	
	void expire(float delta);
    
    void onFinishedIntro();
	
	virtual void touch(Dwarf* dwarf,Troll* troll);
	
	virtual void tap() { _tapCount++; }
	uint32_t getTapCount() const { return _tapCount; }
	
	bool getNeedToRemove() const { return _needToRemove; }
    
    bool _needToRemove;
    bool _disabled;
	
protected:
	EffectType _effectType;
	GameScene* _gameScene;
	
	cocos2d::CCSprite* _sprite;
	
	uint32_t _tapCount;
	
};
