//
//  HidraIntro.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 08/09/14.
//
//

#pragma once

#include "IntroAnimation.h"

class Hidra;

class HidraIntro: public IntroAnimation
{
public:
	static HidraIntro* create(GameScene* game, Hidra* hidra);
	
	HidraIntro();
    virtual ~HidraIntro();
	
	virtual bool init(GameScene* game, Hidra* hidra);
	
	Hidra* getHidra() const;
	
private:
	void startAnimation();
    void startTrollFade();
	
	SpriteAnimation* _animation;
	Hidra* _hidra;
    
    cocos2d::CCSprite* _introFog;
    cocos2d::CCSprite* _introFog_Dot;
};
