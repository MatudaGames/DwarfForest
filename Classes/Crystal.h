//
//  Chrystal.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 22.02.2013.
//
//

#pragma once

#include <cocos2d.h>

class SpriteAnimation;
class GameScene;
class Dwarf;
class Goblin;

enum CrystalColor
{
	CRYSTAL_COLOR_BLUE = 0,
	CRYSTAL_COLOR_GREEN,
	CRYSTAL_COLOR_RED,
	CRYSTAL_COLOR_YELLOW,
	CRYSTAL_COLOR_COUNT,
    CRYSTAL_EGG
};

class Crystal: public cocos2d::CCNode
{
public:
	static Crystal* create(GameScene* gameScene);
	
	Crystal();
	virtual ~Crystal();
	
	virtual bool init(GameScene* gameScene);
	
	CrystalColor getColor() const;
    
    void onFinishedIntro();
	
//	void expire(float delta);
    void expire();
	
	void pickUp(Dwarf* dwarf,int aMulti=1);
    
    void createStar();
    void expireAnim();
    void removeStar();
    
    bool mExitAnimSet;
    
    Goblin* mStealer;
    
    void updateCrystalTimer();
    CrystalColor _color;
	
private:
	
	
	GameScene* _gameScene;
    
    SpriteAnimation* _animation;
	
	cocos2d::CCSprite* _shadowSprite;
	cocos2d::CCSprite* _crystalSprite;
    cocos2d::CCSprite* _crystalShadow;
	
	float _secondsActive;
};