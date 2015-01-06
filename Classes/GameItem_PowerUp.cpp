//
//  GameItem_PowerUp.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 05/01/15.
//
//

#include "GameItem_PowerUp.h"

#include "GameScene.h"
#include "Dwarf.h"
#include "User.h"

USING_NS_CC;

GameItem_PowerUp* GameItem_PowerUp::create(GameScene* gameScene,int theTimeOnMap)
{
    GameItem_PowerUp *pRet = new GameItem_PowerUp();
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

GameItem_PowerUp::GameItem_PowerUp():
_mushroomSpriteOn(NULL),_mushroomSpriteOff(NULL)
{
}

GameItem_PowerUp::~GameItem_PowerUp()
{
}

bool GameItem_PowerUp::init(GameScene* gameScene,int theTimeOnMap)
{
    if (!CCNode::init())
    {
        return false;
    }
    
    mPowerID = theTimeOnMap;
    
    _gameScene = gameScene;
    
    if(mPowerID == 0)
    {
        _mushroomSpriteOff = CCSprite::create("button_electro.png");
    }
    else
    {
        _mushroomSpriteOff = CCSprite::create("button_freez.png");
    }
    
    _mushroomSpriteOff->setOpacity(0);
    
    addChild(_mushroomSpriteOff);
    
    _mushroomSpriteOff->setAnchorPoint(ccp(0.5,0.0f));
    _mushroomSpriteOff->setScaleX(0);
    _mushroomSpriteOff->setScaleY(0);
    
    /*
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
    */
    
    CCScaleTo* aScaleIn = CCScaleTo::create(0.5f, GLOBAL_SCALE, GLOBAL_SCALE);
    CCActionInterval* aScaleAct = CCEaseElasticOut::create(aScaleIn,0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameItem_PowerUp::onFinishedShowUp));
    
    CCFadeIn* aFadeIn = CCFadeIn::create(0.2f);
    CCSpawn* aSpawn = CCSpawn::create(aFadeIn,aScaleAct,NULL);
    
    CCSequence* aTotal = CCSequence::create(aSpawn,func,NULL);
    _mushroomSpriteOff->runAction(aTotal);
    
    //10 sec to collect this bad boy
    schedule(schedule_selector(GameItem_PowerUp::createRemove), 0.0f, 0, 5.0f);
    
    _needToRemove = false;
    
    return true;
}

void GameItem_PowerUp::createRemove()
{
    unschedule(schedule_selector(GameItem_PowerUp::createRemove));
    _needToRemove = true;
    
    // The remove part
    CCScaleTo* aScale = CCScaleTo::create(0.5f, 0);
    CCActionInterval* aScaleAct = CCEaseElasticIn::create(aScale,0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameItem_PowerUp::onRemove));
    CCSequence* seqfin = CCSequence::create(aScaleAct,func,NULL);
    _mushroomSpriteOff->runAction(seqfin);
}

void GameItem_PowerUp::onRemove()
{
    _gameScene->_powersOnMap->removeObject(this);
    _gameScene->removeNode(this);
}

void GameItem_PowerUp::onFinishedShowUp()
{
    
    return;
    
    /*
    if (_needToRemove)
    {
        CCScaleTo* aScale = CCScaleTo::create(0.5f, 0);
        CCActionInterval* aScaleAct = CCEaseElasticIn::create(aScale,0.5f);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameItem_PowerUp::onRemove));
        CCSequence* seqfin = CCSequence::create(aScaleAct,func,NULL);
        _mushroomSpriteOff->runAction(seqfin);
        return;
    }
    
    CCFadeIn* a1_FadeIn = CCFadeIn::create(0.5f);
    _mushroomSpriteOn->runAction(a1_FadeIn);
    
    CCFadeOut* a2_FadeOut = CCFadeOut::create(0.5f);
    CCDelayTime* a2_Delay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameItem_PowerUp::onFinishedTrans));
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
    */
}

void GameItem_PowerUp::onFinishedTrans()
{
    return;
    
    /*
    CCFadeIn* a1_FadeIn = CCFadeIn::create(0.5f);
    _mushroomSpriteOff->runAction(a1_FadeIn);
    
    CCFadeOut* a2_FadeOut = CCFadeOut::create(0.5f);
    CCDelayTime* a2_Delay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameItem_PowerUp::onFinishedShowUp));
    CCSequence* a2_Seq = CCSequence::create(a2_Delay,a2_FadeOut,func,NULL);
    _mushroomSpriteOn->runAction(a2_Seq);
    */
}