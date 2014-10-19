//
//  ChrystalIntro.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 10.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "CrystalIntro.h"
#include "GameScene.h"
#include "SpriteAnimation.h"
#include "Crystal.h"

USING_NS_CC;

CrystalIntro* CrystalIntro::create(GameScene* game, Crystal* crystal)
{
	CrystalIntro *pRet = new CrystalIntro();
    if (pRet && pRet->init(game, crystal))
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

CrystalIntro::CrystalIntro():
	_shadowSprite(NULL), _crystalSprite(NULL), _shadowFadeIn(NULL), _crystalFadeIn(NULL), _animation(NULL)
{
}

CrystalIntro::~CrystalIntro()
{
	if (_shadowFadeIn) _shadowFadeIn->release();
	if (_crystalFadeIn) _crystalFadeIn->release();
    if (_animation) _animation->release();
}

bool CrystalIntro::init(GameScene* game, Crystal* crystal)
{
	if (!IntroAnimation::init(INTRO_CRYSTAL, game))
	{
		return false;
	}
    
    _animation = NULL;
    
    //Check if need to create place show
    if (_game->getBoostFutureSee() > 0)
    {
        _animation = SpriteAnimation::create("Crystals/shiny_stuff.plist");
        _animation->retain();
        _animation->setTag(100);
        
        addChild(_animation);
    }
	
	_crystal = crystal;
	
//	switch (_crystal->getColor())
//	{
//		case CRYSTAL_COLOR_BLUE:
//			_shadowSprite = CCSprite::create("Crystals/blue_under.png");
//			_crystalSprite = CCSprite::create("Crystals/blue_crystal.png");
//			break;
//		case CRYSTAL_COLOR_GREEN:
//			_shadowSprite = CCSprite::create("Crystals/green_under.png");
//			_crystalSprite = CCSprite::create("Crystals/green_crystal.png");
//			break;
//		case CRYSTAL_COLOR_RED:
//			_shadowSprite = CCSprite::create("Crystals/red_under.png");
//			_crystalSprite = CCSprite::create("Crystals/red_crystal.png");
//			break;
//		case CRYSTAL_COLOR_YELLOW:
//		default:
//			_shadowSprite = CCSprite::create("Crystals/yellow_under.png");
//			_crystalSprite = CCSprite::create("Crystals/yellow_crystal.png");
//			break;
//	}
    
//    if (_animation!=NULL)
//    {
//        //Add delay 3 sec
//        float aRandomSpawn = (rand()%3)+3;
//        schedule(schedule_selector(CrystalIntro::CreateRealIntro),aRandomSpawn, 0, 0.0f);
//    }
//    else
//    {
//        CreateRealIntro();
//    }
    
    float aRandomSpawn = ((rand()%200)*0.01)+2;
    
	schedule(schedule_selector(CrystalIntro::CreateRealIntro),aRandomSpawn, 0, 0.0f);
    
	return true;
}

void CrystalIntro::CreateRealIntro()
{
    //remove the blink anim if had !!
    if (getChildByTag(100))
    {
        _animation->stopAllActions();
        removeChild(_animation);
    }
    
    switch (_crystal->getColor())
	{
        case CRYSTAL_EGG:
            _shadowSprite = CCSprite::create("trajectory_dot_white.png");
			_crystalSprite = CCSprite::create("Interfeiss/challenges/weekly/easter/egg_small.png");
            break;
		case CRYSTAL_COLOR_BLUE:
			_shadowSprite = CCSprite::create("Crystals/blue_under.png");
			_crystalSprite = CCSprite::create("Crystals/blue_crystal.png");
			break;
		case CRYSTAL_COLOR_GREEN:
			_shadowSprite = CCSprite::create("Crystals/green_under.png");
			_crystalSprite = CCSprite::create("Crystals/green_crystal.png");
			break;
		case CRYSTAL_COLOR_RED:
			_shadowSprite = CCSprite::create("Crystals/red_under.png");
			_crystalSprite = CCSprite::create("Crystals/red_crystal.png");
			break;
		case CRYSTAL_COLOR_YELLOW:
		default:
			_shadowSprite = CCSprite::create("Crystals/yellow_under.png");
			_crystalSprite = CCSprite::create("Crystals/yellow_crystal.png");
			break;
	}
    
    //For debug
//    if(GLOBAL_SCALE!=1.0f)
//    {
//        _shadowSprite->setScale(GLOBAL_SCALE);
//        _crystalSprite->setScale(GLOBAL_SCALE);
////        _animation->setScale(GLOBAL_SCALE);
//    }
    
    _shadowSprite->setScale(0.64f);
    _crystalSprite->setScale(0.64f);
    
    addChild(_shadowSprite);
	addChild(_crystalSprite);
	
    _shadowSprite->setOpacity(0);
	_crystalSprite->setScaleY(0.0f);
	
	_crystalSprite->setPositionY(-_crystalSprite->getContentSize().height/2.0f);
	_shadowSprite->setPositionY(-_crystalSprite->getContentSize().height/4.0f-6);
	_crystalSprite->setAnchorPoint(ccp(0.5f, 0.0f));
	
	_shadowFadeIn = CCFadeIn::create(1.5f);
	_shadowFadeIn->retain();
	_shadowSprite->runAction(_shadowFadeIn);
	
	CCScaleTo* scaleTween = CCScaleTo::create(1.5f, 0.64f);
	_crystalFadeIn = CCEaseElasticOut::create(scaleTween);
	_crystalFadeIn->retain();
}

void CrystalIntro::update(float delta)
{
	if (_shadowFadeIn && _shadowFadeIn->isDone())
	{
		_shadowFadeIn->release();
		_shadowFadeIn = NULL;
		
		_crystalSprite->runAction(_crystalFadeIn);
	}
	
	if (_crystalFadeIn && _crystalFadeIn->isDone())
	{
		_crystalFadeIn->release();
		_crystalFadeIn = NULL;
        
        //Had one crash here - don't get why?
        finished();
	}
}

Crystal* CrystalIntro::getCrystal() const
{
	return _crystal;
}
