//
//  Mushroom.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 24.08.2013.
//  Copyright (c) 2013 Kristaps Auzins All rights reserved.
//

#include "Mushroom.h"
#include "GameScene.h"
#include "Dwarf.h"
#include "User.h"

USING_NS_CC;

Mushroom* Mushroom::create(GameScene* gameScene,int theTimeOnMap)
{
	Mushroom *pRet = new Mushroom();
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

Mushroom::Mushroom():
    _mushroomSpriteOn(NULL),_mushroomSpriteOff(NULL)
{
}

Mushroom::~Mushroom()
{
}

bool Mushroom::init(GameScene* gameScene,int theTimeOnMap)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    _gameScene = gameScene;
    
    //Show up the mushroom
    _mushroomSpriteOff = CCSprite::create("powerup_mushroom/shroom_base.png");
    _mushroomSpriteOn = CCSprite::create("powerup_mushroom/shroom_over.png");
    
    _mushroomSpriteOff->setOpacity(0);
    _mushroomSpriteOn->setOpacity(0);
    
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
        _mushroomSpriteOff->setScale(GLOBAL_SCALE);
        _mushroomSpriteOn->setScale(GLOBAL_SCALE);
    }
    
    addChild(_mushroomSpriteOff);
	addChild(_mushroomSpriteOn);
    
    _mushroomSpriteOn->setAnchorPoint(ccp(0.5,0.0f));
    
    _mushroomSpriteOff->setAnchorPoint(ccp(0.5,0.0f));
    _mushroomSpriteOff->setScaleX(0);
    _mushroomSpriteOff->setScaleY(0);
    
    CCScaleTo* aScaleIn = CCScaleTo::create(0.5f, GLOBAL_SCALE, GLOBAL_SCALE);
    CCActionInterval* aScaleAct = CCEaseElasticOut::create(aScaleIn,0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Mushroom::onFinishedShowUp));
    
    CCFadeIn* aFadeIn = CCFadeIn::create(0.2f);
    CCSpawn* aSpawn = CCSpawn::create(aFadeIn,aScaleAct,NULL);
    
    CCSequence* aTotal = CCSequence::create(aSpawn,func,NULL);
    _mushroomSpriteOff->runAction(aTotal);
    
    //10 sec to collect this bad boy
    schedule(schedule_selector(Mushroom::createRemove), 0.0f, 0, theTimeOnMap);
    
    _needToRemove = false;
    
    return true;
}

void Mushroom::createRemove()
{
    unschedule(schedule_selector(Mushroom::createRemove));
    _needToRemove = true;
}

void Mushroom::onRemove()
{
//    _gameScene->_mushrooms->removeObject(this);
    _gameScene->removeNode(this);
}

void Mushroom::onFinishedShowUp()
{
    if (_needToRemove)
    {
        CCScaleTo* aScale = CCScaleTo::create(0.5f, 0);
        CCActionInterval* aScaleAct = CCEaseElasticIn::create(aScale,0.5f);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Mushroom::onRemove));
        CCSequence* seqfin = CCSequence::create(aScaleAct,func,NULL);
        _mushroomSpriteOff->runAction(seqfin);
        return;
    }
    
    CCFadeIn* a1_FadeIn = CCFadeIn::create(0.5f);
    _mushroomSpriteOn->runAction(a1_FadeIn);
    
    CCFadeOut* a2_FadeOut = CCFadeOut::create(0.5f);
    CCDelayTime* a2_Delay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Mushroom::onFinishedTrans));
    CCSequence* a2_Seq = CCSequence::create(a2_Delay,a2_FadeOut,func,NULL);
    _mushroomSpriteOff->runAction(a2_Seq);
    
    //1ST TIME STUFF
    if(User::getInstance()->_tutorial_small_4==0)
    {
        //1st time pause the game and show some stuff !!!
        _gameScene->stopInGameSound("Footsteps",true);
        _gameScene->stopInGameSound("troll_walk",true);
        _gameScene->stopInGameSound("dwarf_web_stuck",true);
        
        //Get the mid point !!!
        int aShowX = getPositionX();
        int aShowY = getPositionY();
        
        _gameScene->CreateSmallTutorial(4,aShowX,aShowY);
    }
}

void Mushroom::onFinishedTrans()
{
    CCFadeIn* a1_FadeIn = CCFadeIn::create(0.5f);
    _mushroomSpriteOff->runAction(a1_FadeIn);
    
    CCFadeOut* a2_FadeOut = CCFadeOut::create(0.5f);
    CCDelayTime* a2_Delay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Mushroom::onFinishedShowUp));
    CCSequence* a2_Seq = CCSequence::create(a2_Delay,a2_FadeOut,func,NULL);
    _mushroomSpriteOn->runAction(a2_Seq);
}


