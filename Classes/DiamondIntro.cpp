//
//  DiamondEntry.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 14.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "DiamondIntro.h"
#include "GameScene.h"
#include "Diamond.h"

USING_NS_CC;

DiamondIntro* DiamondIntro::create(GameScene* game, Diamond* diamond)
{
	DiamondIntro *pRet = new DiamondIntro();
    if (pRet && pRet->init(game, diamond))
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

DiamondIntro::DiamondIntro()
{
}

DiamondIntro::~DiamondIntro()
{
}

bool DiamondIntro::init(GameScene* game, Diamond* crystal)
{
	if (!IntroAnimation::init(INTRO_DIAMOND, game))
	{
		return false;
	}
	
	_diamond = crystal;
	

	_diamondSprite = CCSprite::create("diamond/diamond.png");
	_shadowSprite = CCSprite::create("diamond/diamond_shadow.png");
	
	addChild(_shadowSprite);
    addChild(_diamondSprite);
	
	_diamondSprite->setScaleY(0.0f);
	
	_diamondSprite->setPositionY(-_diamondSprite->getContentSize().height/2.0f);
	_shadowSprite->setPositionY(-_diamondSprite->getContentSize().height/4.0f);
	_diamondSprite->setAnchorPoint(ccp(0.5f, 0));
	
	_shadowFadeIn = CCFadeIn::create(1.5f);
	_shadowFadeIn->retain();
	_shadowSprite->runAction(_shadowFadeIn);
	
	CCScaleTo* scaleTween = CCScaleTo::create(1.5f, GLOBAL_SCALE);
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
//        _diamondSprite->setScale(GLOBAL_SCALE);
        _shadowSprite->setScale(GLOBAL_SCALE);
    }
    
	_diamondFadeIn = CCEaseElasticOut::create(scaleTween);
	_diamondFadeIn->retain();
	_diamondSprite->runAction(_diamondFadeIn);
    

	
	return true;
}

void DiamondIntro::update(float delta)
{
	if (_shadowFadeIn->isDone() && _diamondFadeIn->isDone())
	{
		finished();
	}
}

Diamond* DiamondIntro::getDiamond() const
{
	return _diamond;
}
