//
//  DwarfIntro.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 10.05.2013.
//
//

#include "DwarfIntro.h"
#include "GameScene.h"
#include "SpriteAnimation.h"
#include "Dwarf.h"
#include "User.h"

USING_NS_CC;

DwarfIntro* DwarfIntro::create(GameScene* game, Dwarf* dwarf)
{
	DwarfIntro *pRet = new DwarfIntro();
    if (pRet && pRet->init(game, dwarf))
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

DwarfIntro::DwarfIntro()
{
}

DwarfIntro::~DwarfIntro()
{
}

bool DwarfIntro::init(GameScene* game, Dwarf* dwarf)
{
	if (!IntroAnimation::init(INTRO_DWARF, game))
	{
		return false;
	}
    
    _game = game;
	
	_time = 0.0f;
	_dwarf = dwarf;
	
	_introLight = CCSprite::create("effects/dwarf_intro_light.png");
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
        _introLight->setScale(GLOBAL_SCALE);
    }
	addChild(_introLight);
	_introLight->setOpacity(0.0f);
	
    /*
//	schedule(schedule_selector(IntroAnimation::finished), 3.0f, 1, 0.0f);
    if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission
       || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
       || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission){
        if(game->_dwarves->count()-1<3){
            schedule(schedule_selector(DwarfIntro::onFinshed), 0.0f, 1, 0.0f);
        }
        else{
            schedule(schedule_selector(DwarfIntro::onFinshed), 3.0f, 1, 0.0f);
        }
    }
    else
    {
        if(game->_gameTime<60 && game->_gameTime>3 && game->_dwarves->count()-1<=0)
        {
            //Instatnt spawn
            schedule(schedule_selector(DwarfIntro::onFinshed), 0.0f, 1, 0.0f);
        }
        else
        {
            schedule(schedule_selector(DwarfIntro::onFinshed), 3.0f, 1, 0.0f);
        }
    }
    */
    
    if(game->_gameTime<60 && game->_gameTime>3 && game->_dwarves->count()-1<=0)
    {
        //Instatnt spawn
        schedule(schedule_selector(DwarfIntro::onFinshed), 0.0f, 1, 0.0f);
    }
    else
    {
        schedule(schedule_selector(DwarfIntro::onFinshed), 3.0f, 1, 0.0f);
    }
	
	return true;
}

void DwarfIntro::onFinshed()
{
    getDwarf()->setDisabled(false);//He is active now !!!
    IntroAnimation::finished();
}

void DwarfIntro::update(float delta)
{
	_time += delta;
	
	int phase = (int)_time;
	float timeInPhase = _time - phase;
	
	float maxOpacity = 0;
	
	switch (phase)
	{
		case 0:
			maxOpacity = 0.3f;
			break;
		case 1:
			maxOpacity = 0.5f;
			break;
		case 2:
			maxOpacity = 0.7f;
			break;
	}
	
	_introLight->setOpacity(255.0f * maxOpacity * (1.0f - 2.0f * fabsf(timeInPhase - 0.5f)));
    
    //Check if in map only 1 dwarf left - then instant spawn !!!
    if(_game->_dwarves->count()<=1){
        unschedule(schedule_selector(DwarfIntro::onFinshed));
        onFinshed();
    }
}

Dwarf* DwarfIntro::getDwarf() const
{
	return _dwarf;
}
