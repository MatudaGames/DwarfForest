//
//  Diamond.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 13.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "Diamond.h"
#include "User.h"
#include "GameScene.h"

USING_NS_CC;

Diamond* Diamond::create(GameScene* gameScene,int theTimeOnMap)
{
	Diamond *pRet = new Diamond();
    if (pRet && pRet->init(gameScene,theTimeOnMap))
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

Diamond::Diamond():
	_diamondSprite(NULL), _shadowSprite(NULL)
{
}

Diamond::~Diamond()
{
}

bool Diamond::init(GameScene* gameScene,int theTimeOnMap)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    mTimeOnMap = theTimeOnMap;
    
    _gameScene = gameScene;
    
    _diamondTimer = 0.0f;
	
	_diamondSprite = CCSprite::create("diamond/diamond.png");
	_shadowSprite = CCSprite::create("diamond/diamond_shadow.png");
    
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
        _diamondSprite->setScale(GLOBAL_SCALE);
        _shadowSprite->setScale(GLOBAL_SCALE);
    }
    
    _diamondSprite->setOpacity(0);
    _shadowSprite->setOpacity(0);
    
	_diamondSprite->setPositionY(-_diamondSprite->getContentSize().height/2.0f);
	_shadowSprite->setPositionY(-_diamondSprite->getContentSize().height/4.0f);
	_diamondSprite->setAnchorPoint(ccp(0.5f, 0));
	
	addChild(_shadowSprite);
	addChild(_diamondSprite);
	
	return true;
}

void Diamond::createAnim()
{
    _diamondSprite->setOpacity(255);
    _shadowSprite->setOpacity(255);
    
    //Float in the air and finish if nobody collects it
    CCMoveBy* _move1 = CCMoveBy::create(1.0f,ccp(0,15));
    _diamondMoveUp = CCEaseSineInOut::create(_move1);
    
    CCMoveBy* _move2 = CCMoveBy::create(1.0f,ccp(0,-15));
    _diamondMoveDown = CCEaseSineInOut::create(_move2);
    
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Diamond::onFinishedFlow));
    
    CCSequence* _movement = CCSequence::create(_diamondMoveUp,_diamondMoveDown,func,NULL);
    CCRepeatForever* _repeat = CCRepeatForever::create(_movement);
    
    _diamondSprite->runAction(_repeat);
    
    //1ST TIME STUFF
    if(User::getInstance()->_tutorial_small_5==0)
    {
        //1st time pause the game and show some stuff !!!
//        _gameScene->stopInGameSound("dwarf_walk",true);
        _gameScene->stopInGameSound("Footsteps");
        
        _gameScene->stopInGameSound("troll_walk",true);
        _gameScene->stopInGameSound("dwarf_web_stuck",true);
        
        //Get the mid point !!!
        int aShowX = getPositionX();
        int aShowY = getPositionY();
        
        _gameScene->CreateSmallTutorial(5,aShowX,aShowY);
    }
    
    //Check timer
    schedule(schedule_selector(Diamond::updateDiamondTimer), 0.5f);
}

void Diamond::updateDiamondTimer()
{
    _diamondTimer += 0.5 * _gameScene->getGameSpeed();
//    CCLog("Dimond active, %f",mDiamondTimer);
}

void Diamond::onFinishedFlow()
{
    if (_diamondTimer>=mTimeOnMap)
    {
        //stop current actions
        _diamondSprite->stopAllActions();
        unschedule(schedule_selector(Diamond::updateDiamondTimer));
        
        //Just fade out !!!
        CCFadeOut* _fade = CCFadeOut::create(0.25f);
        _shadowSprite->runAction(_fade);
        
        CCScaleTo* _scale = CCScaleTo::create(0.25f, 0.0f);
        CCActionInterval* _tween = CCEaseSineInOut::create(_scale);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Diamond::expire));
        CCSequence* aSeq = CCSequence::create(_tween,func,NULL);
        
        _diamondSprite->runAction(aSeq);
    }
}

void Diamond::expire()
{
	_gameScene->removeDiamond(this);
}
