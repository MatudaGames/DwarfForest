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
	static Crystal* create(GameScene* gameScene,int theCrystalID,int theTimeOnMap);
	
	Crystal();
	virtual ~Crystal();
	
	virtual bool init(GameScene* gameScene,int theCrystalID,int theTimeOnMap);
	
	CrystalColor getColor() const;
    
    void onFinishedIntro();
    
    float _OnMapTime;
	
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
	// Not the best spot, but for now as time is money :D
    int SMALL_CRYSTAL_SCORE;
    int MEDIUM_CRYSTAL_SCORE;
    int BIG_CRYSTAL_SCORE;
    int XXL_CRYSTAL_SCORE;
	
	GameScene* _gameScene;
    
    SpriteAnimation* _animation;
	
	cocos2d::CCSprite* _shadowSprite;
	cocos2d::CCSprite* _crystalSprite;
    cocos2d::CCSprite* _crystalShadow;
	
	float _secondsActive;
};
