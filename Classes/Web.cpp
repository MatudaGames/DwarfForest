//
//  Web.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 17.03.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "Web.h"
#include "GameScene.h"
#include "Dwarf.h"
#include "SpriteAnimation.h"

USING_NS_CC;
/////////////////////////////////////////
Web* Web::create(GameScene* gameScene)
{
	Web *pRet = new Web();
    if (pRet && pRet->init(gameScene))
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

Web::Web():
	_animation(NULL),_startAnimation(NULL),_endAnimation(NULL)
{
}

Web::~Web()
{
    if (_animation) _animation->release();
    if (_startAnimation) _startAnimation->release();
    if (_endAnimation) _endAnimation->release();
}

bool Web::init(GameScene* gameScene)
{
	if (!Effect::init(gameScene))
	{
		return false;
	}
    
    _needToRemove = false;
	
	_effectType = EFFECT_TYPE_WEB;
	
	_sprite = CCSprite::create("meteorits/effect_area.png");
	addChild(_sprite);
    
    _startAnimation = SpriteAnimation::create("sticky_web/web_intro.plist");
    _startAnimation->retain();
    
    if (GLOBAL_SCALE!=1.0f)
    {
        _startAnimation->setScale(GLOBAL_SCALE);
        _sprite->setScale(GLOBAL_SCALE);
    }
    
    _startAnimation->setPosition(ccp(12,14));
    
    _dwarfType = 0;
    
    _game = gameScene;
	
	return  true;
}

void Web::startStuckAnim()
{
    unschedule(schedule_selector(Web::startStuckAnim));
    _dwarf->setAnimationVisibility(false);
    
    if (_dwarfType == DWARF_TYPE_FAT)
    {
        _animation = SpriteAnimation::create("sticky_web/fatass_stuck.plist");
        _animation->retain();
    }
    else if (_dwarfType == DWARF_TYPE_TALL)
    {
        _animation = SpriteAnimation::create("sticky_web/tallass_stuck.plist");
        _animation->retain();
    }
    
    _game->playInGameSound("dwarf_web_stuck");
    
    if (GLOBAL_SCALE!=1.0f)
    {
        _animation->setScale(GLOBAL_SCALE);
    }
    
    removeChild(_startAnimation);
    addChild(_animation);
    schedule(schedule_selector(Web::finishStuckAnim), 0.0f, 0, 5.0f);
}

void Web::finishStuckAnim()
{
    unschedule(schedule_selector(Web::finishStuckAnim));
    
    if (_dwarfType == DWARF_TYPE_FAT)
    {
        _endAnimation = SpriteAnimation::create("sticky_web/fatass_free.plist");
        _endAnimation->retain();
    }
    else if (_dwarfType == DWARF_TYPE_TALL)
    {
        _endAnimation = SpriteAnimation::create("sticky_web/tallass_free.plist");
        _endAnimation->retain();
    }
    
    if (GLOBAL_SCALE!=1.0f)
    {
        _endAnimation->setScale(GLOBAL_SCALE);
    }
    
    _game->stopInGameSound("dwarf_web_stuck");
    _game->playInGameSound("dwarf_web_exit");
    
    removeChild(_animation);
    addChild(_endAnimation);
    schedule(schedule_selector(Web::onComplete), 0.0f, 1, 0.5f);
}

void Web::onComplete()
{
    unschedule(schedule_selector(Web::onComplete));
    
    _needToRemove = true;
    _dwarf->setAnimationVisibility(true);
}

void Web::touch(Dwarf* dwarf,Troll* troll)
{
	Effect::touch(dwarf,NULL);
	
	removeChild(_sprite);
    
    _dwarfType = dwarf->getType();
    _dwarf = dwarf;
    
    //Play the intro
    addChild(_startAnimation);
    schedule(schedule_selector(Web::startStuckAnim), 0, 0, 0.6f);
    
//    CreateAnims();
	
//	if (dwarf->getType() == DWARF_TYPE_TALL)
//	{
//		_animation = SpriteAnimation::create("sticky_web/tallass_stuck.plist");
//	}
//	else if (dwarf->getType() == DWARF_TYPE_FAT)
//	{		
//		_animation = SpriteAnimation::create("sticky_web/fatass_stuck.plist");
//	}
//	
//	CCLOG("Adding animation");
//	
//	if (_animation)
//	{
//		CCLOG("Added animation");
//		addChild(_animation);
//	}
}
