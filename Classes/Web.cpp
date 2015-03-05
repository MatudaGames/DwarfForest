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
	_animation(NULL),_startAnimation(NULL),_endAnimation(NULL),_growAnimation(NULL)
{
}

Web::~Web()
{
    if (_animation) _animation->release();
    if (_growAnimation) _growAnimation->release();
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
	
	_growAnimation = SpriteAnimation::create("sticky_web/trap_sticky_grow.plist");
    _growAnimation->retain();
    _growAnimation->setTag(876);
	addChild(_growAnimation);
	    
    _growDelay = CCDelayTime::create(2.4f);//Maybe 2.4f will be good.
    //_growDelay->retain();
    CCCallFuncN* aFunc = CCCallFuncN::create(this, callfuncN_selector(Web::pauseGrow));
    CCSequence* aSeqFun = CCSequence::create(_growDelay,aFunc,NULL);
    runAction(aSeqFun);
    
    _startAnimation = SpriteAnimation::create("sticky_web/web_intro.plist");
    _startAnimation->retain();
    
    if (GLOBAL_SCALE!=1.0f)
    {
        _startAnimation->setScale(GLOBAL_SCALE);
        _growAnimation->setScale(GLOBAL_SCALE);
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

void Web::pauseGrow()
{
	//_growAnimation->pause();
	_growAnimation->stopAllActions();
}

void Web::onComplete()
{
    unschedule(schedule_selector(Web::onComplete));
    
    _needToRemove = true;
    _dwarf->setAnimationVisibility(true);
}

void Web::shotBulletAtDwarf()
{
	_game->CreateBulletByType(MASTER_ACTION_BULLET_STRAIGHT, _game->_MasterTrollBase->getPositionX(), _game->_MasterTrollBase->getPositionY());
}

void Web::touch(Dwarf* dwarf,Troll* troll)
{
	Effect::touch(dwarf,NULL);
    
    _dwarfType = dwarf->getType();
    _dwarf = dwarf;
    
    // New Troll catch stuff
    for(int trollIndex = _game->_trolls->count() - 1; trollIndex >= 0; --trollIndex)
    {
        Troll* troll = static_cast<Troll*>(_game->_trolls->objectAtIndex(trollIndex));
        if(troll->mEnemySpawnID == 3)
        {
            if(troll->mCatchingDwarf == false)
            {
                troll->CatchDwarf(dwarf);
            }else{
                break;
            }
        }
    }
    
    //Play the intro
    removeChild(_growAnimation);
    addChild(_startAnimation);

   	if(_game->mtSnipe>0)
    {
   	CCDelayTime* aSnipeDelay = CCDelayTime::create(_game->mtSnipeDelay);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Web::shotBulletAtDwarf));
    CCSequence* aSeq1 = CCSequence::create(aSnipeDelay,func,NULL);
	runAction(aSeq1);
	}
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
