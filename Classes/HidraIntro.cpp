//
//  HidraIntro.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 08/09/14.
//
//

#include "HidraIntro.h"

#include "GameScene.h"
#include "SpriteAnimation.h"
#include "Hidra.h"

USING_NS_CC;

HidraIntro* HidraIntro::create(GameScene* game, Hidra* hidra)
{
	HidraIntro *pRet = new HidraIntro();
    if (pRet && pRet->init(game, hidra))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

HidraIntro::HidraIntro():
_animation(NULL)
{
}

HidraIntro::~HidraIntro()
{
	if (_animation) _animation->release();
}

bool HidraIntro::init(GameScene* game, Hidra* hidra)
{
	if (!IntroAnimation::init(INTRO_TROLL, game))
	{
		return false;
	}
	
	_hidra = hidra;
    
    _introFog = CCSprite::create("effects/fog.png");
    _introFog_Dot = CCSprite::create("effects/fog_dots.png");
    
    addChild(_introFog);
    addChild(_introFog_Dot);
    
    _introFog->setOpacity(0);
    _introFog_Dot->setOpacity(0);
    
    _introFog->setScale(0.75*GLOBAL_SCALE);
    _introFog_Dot->setScale(0.75*GLOBAL_SCALE);
    
    //Some cocos2d functions
    CCActionInterval* fog_in = CCFadeIn::create(1.5f);
    CCActionInterval* fog_out = CCFadeOut::create(1.5f);
    
    CCActionInterval* fog_scale_1 = CCScaleTo::create(1.5f, 1.0f*GLOBAL_SCALE);
    CCActionInterval* fog_scale_2 = CCScaleTo::create(1.5f, 1.1f*GLOBAL_SCALE);
    
    CCActionInterval* fog_delay_1 = CCDelayTime::create(1.5);
    CCActionInterval* fog_delay_2 = CCDelayTime::create(2.5);
    
    CCCallFunc* fog_fun = CCCallFuncN::create(this, callfuncN_selector(HidraIntro::startAnimation));
    
    CCSpawn* fog_spawn_2 = CCSpawn::create(fog_out,fog_scale_2,NULL);
    CCSequence* fog_seq_2 = CCSequence::create(fog_delay_1,fog_spawn_2,NULL);
    
    CCSequence* fog_seq_3 = CCSequence::create(fog_delay_2,fog_fun,NULL);
    
    CCSpawn* fog_spawn_1 = CCSpawn::create(fog_in,fog_scale_1,fog_seq_2,fog_seq_3,NULL);
    
    _introFog->runAction(fog_spawn_1);
    
    //Part II
    //StartTrollFade
    
    fog_in = CCFadeIn::create(1.5f);
    fog_out = CCFadeOut::create(1.5f);
    
    fog_scale_1 = CCScaleTo::create(1.5f, 1.0f*GLOBAL_SCALE);
    fog_scale_2 = CCScaleTo::create(1.5f, 1.4f*GLOBAL_SCALE);
    
    fog_delay_1 = CCDelayTime::create(1.5f);
    fog_delay_2 = CCDelayTime::create(3.0f);
    
    fog_fun = CCCallFunc::create(this, callfunc_selector(HidraIntro::startTrollFade));
    
    fog_spawn_2 = CCSpawn::create(fog_out,fog_scale_2,NULL);
    fog_seq_2 = CCSequence::create(fog_delay_1,fog_spawn_2,NULL);
    
    fog_seq_3 = CCSequence::create(fog_delay_2,fog_fun,NULL);
    
    fog_spawn_1 = CCSpawn::create(fog_in,fog_scale_1,fog_seq_2,fog_seq_3,NULL);
    
    _introFog_Dot->runAction(fog_spawn_1);
    
    
    //--------------------
	
    //	_animation = SpriteAnimation::create("effects/virpulis.plist");
    //	_animation->retain();
    //	addChild(_animation);
	
	schedule(schedule_selector(IntroAnimation::finished), 3.2f, 1, 0.0f);//A bit later !!! [0.7f, 1, 0.0f]
	
	return true;
}

void HidraIntro::startAnimation()
{
	_animation = SpriteAnimation::create("effects/virpulis.plist");
	_animation->retain();
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
        _animation->setScale(GLOBAL_SCALE);
    }
	addChild(_animation);
}

void HidraIntro::startTrollFade()
{
    _hidra->setVisible(true);
    _hidra->setFadeIn();
}

Hidra* HidraIntro::getHidra() const
{
	return _hidra;
}