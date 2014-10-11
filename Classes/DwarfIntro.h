//
//  DwarfIntro.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 10.05.2013.
//
//

#pragma once

#include "IntroAnimation.h"

class Dwarf;
class GameScene;

class DwarfIntro: public IntroAnimation
{
public:
	static DwarfIntro* create(GameScene* game, Dwarf* dwarf);
	
	DwarfIntro();
    virtual ~DwarfIntro();
	
	virtual bool init(GameScene* game, Dwarf* dwarf);
	
	virtual void update(float delta);
	
	Dwarf* getDwarf() const;
    
    void onFinshed();
	
private:
	GameScene* _game;
	Dwarf* _dwarf;
	
	cocos2d::CCSprite* _introLight;
	
	float _time;
};
