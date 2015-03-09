//
//  Machine_Stamp.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 30/10/13.
//
//

#include "Machine_Stamp.h"

#include "GameScene.h"
#include "Dwarf.h"
#include "SpriteAnimation.h"
#include "TimedSpriteAnimation.h"

#include "User.h"
#include "CCShake.h"

USING_NS_CC;

Machine_Stamp* Machine_Stamp::create(GameScene* gameScene)
{
	Machine_Stamp *pRet = new Machine_Stamp();
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

Machine_Stamp::Machine_Stamp():
_LeafRight(NULL),_LeafLeft(NULL),_LeafCenter(NULL),_LeafBottom(NULL)
{
}

Machine_Stamp::~Machine_Stamp()
{
}

bool Machine_Stamp::init(GameScene* gameScene)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    _gameScene = gameScene;
    
    //Lets get the animations in order
    mBlast = CCSprite::create("powerup/Machine_Stamp/blast_impact.png");
    mBlast->setScale(0.2f);
    mBlast->setPosition(ccp(0,-20));
    mBlast->setOpacity(0);
    addChild(mBlast);
    mBroken = NULL;
    
    mCanBeActivated = false;
    
    //Check in what state is this machine now !!!
//    mUpgradeLevel = User::getInstance()->getMachine_StampLvl();
    mUpgradeLevel = 0;
    
    if(mUpgradeLevel<1)
    {
        //This item is broken
        mBroken = CCSprite::create("powerup/Machine_Stamp/stampatajs_broken.png");
        addChild(mBroken);
    }
    else
    {
        switch (mUpgradeLevel) {
            case 1:mGrowTime = 140;break;
            case 2:mGrowTime = 120;break;
            case 3:mGrowTime = 100;break;
            case 4:mGrowTime = 80;break;
            case 5:mGrowTime = 60;break;
                
            default:
                break;
        }
        
        mSmoke = SpriteAnimation::create("powerup/Machine_Stamp/steaming.plist",false);
        mSmoke->retain();
        mSmoke->setPosition(ccp(40,70));
        mSmoke->setTag(111);
        
        CreateMachineByLevel(mUpgradeLevel);
    }
    
    return true;
}

void Machine_Stamp::ClearOldParts()
{
    //Maybe we had active broken state
    if(mBroken)
    {
        removeChild(mBroken,true);
        mBroken = NULL;
    }
    
    if(mBase)
    {
        removeChild(mBase,true);
        mBase = NULL;
    }
    
    if(mBaseBlock)
    {
        removeChild(mBaseBlock,true);
        mBaseBlock = NULL;
    }
    
    if(mLegRight)
    {
        removeChild(mLegRight,true);
        mLegRight = NULL;
    }
    
    if(mLegLeft)
    {
        removeChild(mLegLeft,true);
        mLegLeft = NULL;
    }
    
    if(mHandRight)
    {
        removeChild(mHandRight,true);
        mHandRight = NULL;
    }
    
    if(mHandLeft)
    {
        removeChild(mHandLeft,true);
        mHandLeft = NULL;
    }
}

void Machine_Stamp::CreateMachineByLevel(int theLevel)
{
    ClearOldParts();
    
    char theFileName[64];
    sprintf(theFileName, "powerup/Machine_Stamp/level%i/panel_front_lvl%i.png", theLevel,theLevel);
    mBase = CCSprite::create(theFileName);
    addChild(mBase,10);
    
    sprintf(theFileName, "powerup/Machine_Stamp/level%i/stampa_lvl%d.png", theLevel,theLevel);
    mBaseBlock = CCSprite::create(theFileName);
//    mBaseBlock->setPosition(ccp(0,60));//End Pos
//    mBaseBlock->setPosition(ccp(0,18));//start Pos
    mBaseBlock->setPosition(ccp(-5,18));//start Pos
    addChild(mBaseBlock,6);
    
    //The legs
    sprintf(theFileName, "powerup/Machine_Stamp/level%i/kaats_bottom_lvl%d.png", theLevel,theLevel);
    mLegLeft = CCSprite::create(theFileName);
//    mLegLeft->setPosition(ccp(-40,0));//End Pos
    mLegLeft->setPosition(ccp(-44,-15));//start Pos
    mLegLeft->setRotation(-40);
    
    addChild(mLegLeft,8);
    
    sprintf(theFileName, "powerup/Machine_Stamp/level%i/kaats_bottom_lvl%d.png", theLevel,theLevel);
    mLegRight = CCSprite::create(theFileName);
    mLegRight->setFlipX(true);
//    mLegRight->setPosition(ccp(36,0));//End Pos
//    mLegRight->setPosition(ccp(40,-15));//start Pos
    mLegRight->setPosition(ccp(35,-15));//start Pos
    mLegRight->setRotation(40);
    addChild(mLegRight,8);
    
    //The arms
    sprintf(theFileName, "powerup/Machine_Stamp/level%i/kaats_top_lvl%d.png", theLevel,theLevel);
    mHandLeft = CCSprite::create(theFileName);
//    mHandLeft->setPosition(ccp(-38,38));//End Pos
    mHandLeft->setPosition(ccp(-42,10));//start Pos
    mHandLeft->setRotation(10);
    addChild(mHandLeft,7);
    
    sprintf(theFileName, "powerup/Machine_Stamp/level%i/kaats_top_lvl%d.png", theLevel,theLevel);
    mHandRight = CCSprite::create(theFileName);
    mHandRight->setFlipX(true);
//    mHandRight->setPosition(ccp(35,38));//End Pos
//    mHandRight->setPosition(ccp(39,10));//start Pos
    mHandRight->setPosition(ccp(34,10));//start Pos
    mHandRight->setRotation(-10);
    addChild(mHandRight,7);
    
    if(_gameScene != NULL){
        SetAnimation_Grow();
    }
}


void Machine_Stamp::SetAnimation_Grow()
{
    mCanBeActivated = false;
    
//    CCMoveTo* moveA1 = CCMoveTo::create(mGrowTime,ccp(0,60));
    CCMoveTo* moveA1 = CCMoveTo::create(mGrowTime,ccp(-5,60));
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Stamp::SetAnimation_Ready));
    CCSequence* seqA1 = CCSequence::create(moveA1,func,NULL);
    mBaseBlock->runAction(seqA1);
    
    CCMoveTo* moveB1 = CCMoveTo::create(mGrowTime,ccp(-40,0));
    CCRotateTo* rotB1 = CCRotateTo::create(mGrowTime, 0);
    CCSpawn* spawnB1 = CCSpawn::create(moveB1,rotB1,NULL);
    mLegLeft->runAction(spawnB1);
    
//    CCMoveTo* moveC1 = CCMoveTo::create(mGrowTime,ccp(36,0));
    CCMoveTo* moveC1 = CCMoveTo::create(mGrowTime,ccp(31,0));
    CCRotateTo* rotC1 = CCRotateTo::create(mGrowTime, 0);
    CCSpawn* spawnC1 = CCSpawn::create(moveC1,rotC1,NULL);
    mLegRight->runAction(spawnC1);
    
    CCMoveTo* moveD1 = CCMoveTo::create(mGrowTime,ccp(-38,38));
    CCRotateTo* rotD1 = CCRotateTo::create(mGrowTime, 0);
    CCSpawn* spawnD1 = CCSpawn::create(moveD1,rotD1,NULL);
    mHandLeft->runAction(spawnD1);
    
//    CCMoveTo* moveE1 = CCMoveTo::create(mGrowTime,ccp(35,38));
    CCMoveTo* moveE1 = CCMoveTo::create(mGrowTime,ccp(30,38));
    CCRotateTo* rotE1 = CCRotateTo::create(mGrowTime, 0);
    CCSpawn* spawnE1 = CCSpawn::create(moveE1,rotE1,NULL);
    mHandRight->runAction(spawnE1);
}

void Machine_Stamp::CreateSmoke()
{
    if(getChildByTag(111))
        removeChild(mSmoke);
    
    addChild(mSmoke);
}

void Machine_Stamp::SetAnimation_Ready()
{
//    CCMoveTo* moveA0 = CCMoveTo::create(0.75f, ccp(0,55));
//    CCMoveTo* moveA1 = CCMoveTo::create(0.75f,ccp(0,60));
    CCMoveTo* moveA0 = CCMoveTo::create(0.75f, ccp(-5,55));
    CCMoveTo* moveA1 = CCMoveTo::create(0.75f,ccp(-5,60));
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Stamp::CreateSmoke));
    
    CCSequence* seqA1 = CCSequence::create(moveA0,func,moveA1,NULL);
    CCRepeatForever* repA1 = CCRepeatForever::create(seqA1);
    mBaseBlock->runAction(repA1);
    
    /////////
    CCMoveTo* moveB0 = CCMoveTo::create(0.75f,ccp(-40,0));
    CCRotateTo* rotB0 = CCRotateTo::create(0.75f, 0);
    CCSpawn* spawnB0 = CCSpawn::create(moveB0,rotB0,NULL);
    
    CCMoveTo* moveB1 = CCMoveTo::create(0.75f,ccp(-41,-2));
    CCRotateTo* rotB1 = CCRotateTo::create(0.75f, -5);
    CCSpawn* spawnB1 = CCSpawn::create(moveB1,rotB1,NULL);
    
    CCSequence* seqB1 = CCSequence::create(spawnB1,spawnB0,NULL);
    CCRepeatForever* repB1 = CCRepeatForever::create(seqB1);
    
    mLegLeft->runAction(repB1);
    
    /////////
//    CCMoveTo* moveC1 = CCMoveTo::create(0.75f,ccp(36,0));
    CCMoveTo* moveC1 = CCMoveTo::create(0.75f,ccp(31,0));
    CCRotateTo* rotC1 = CCRotateTo::create(0.75f, 0);
    CCSpawn* spawnC1 = CCSpawn::create(moveC1,rotC1,NULL);
    
//    CCMoveTo* moveC2 = CCMoveTo::create(0.75f,ccp(37,-2));
    CCMoveTo* moveC2 = CCMoveTo::create(0.75f,ccp(32,-2));
    CCRotateTo* rotC2 = CCRotateTo::create(0.75f, 5);
    CCSpawn* spawnC2 = CCSpawn::create(moveC2,rotC2,NULL);
    
    CCSequence* seqC1 = CCSequence::create(spawnC2,spawnC1,NULL);
    CCRepeatForever* repC1 = CCRepeatForever::create(seqC1);
    
    mLegRight->runAction(repC1);
    
    /////////
    CCMoveTo* moveD0 = CCMoveTo::create(0.75f,ccp(-38,38));
    CCRotateTo* rotD0 = CCRotateTo::create(0.75f, 0);
    CCSpawn* spawnD0 = CCSpawn::create(moveD0,rotD0,NULL);
    
    CCMoveTo* moveD1 = CCMoveTo::create(0.75f,ccp(-39,34));
    CCRotateTo* rotD1 = CCRotateTo::create(0.75f, 2);
    CCSpawn* spawnD1 = CCSpawn::create(moveD1,rotD1,NULL);
    
    CCSequence* seqD1 = CCSequence::create(spawnD1,spawnD0,NULL);
    CCRepeatForever* repD1 = CCRepeatForever::create(seqD1);
    
    mHandLeft->runAction(repD1);
    
    /////////
//    CCMoveTo* moveE0 = CCMoveTo::create(0.75f,ccp(35,38));
    CCMoveTo* moveE0 = CCMoveTo::create(0.75f,ccp(30,38));
    CCRotateTo* rotE0 = CCRotateTo::create(0.75f, 0);
    CCSpawn* spawnE0 = CCSpawn::create(moveE0,rotE0,NULL);
    
//    CCMoveTo* moveE1 = CCMoveTo::create(0.75f,ccp(36,34));
    CCMoveTo* moveE1 = CCMoveTo::create(0.75f,ccp(31,34));
    CCRotateTo* rotE1 = CCRotateTo::create(0.75f, -2);
    CCSpawn* spawnE1 = CCSpawn::create(moveE1,rotE1,NULL);
    
    CCSequence* seqE1 = CCSequence::create(spawnE1,spawnE0,NULL);
    CCRepeatForever* repE1 = CCRepeatForever::create(seqE1);
    
    mHandRight->runAction(repE1);
    
//    CCFadeTo* aFade1 = CCFadeTo::create(1.0f, 128);
//    CCFadeTo* aFade2 = CCFadeTo::create(1.0f, 255);
//    CCSequence* aSeq1 = CCSequence::create(aFade1,aFade2,NULL);
//    CCRepeatForever* aRep = CCRepeatForever::create(aSeq1);
//    mBaseBlock->runAction(aRep);
    
    _gameScene->playInGameSound("S_Done");
    
    mCanBeActivated = true;
}

void Machine_Stamp::ShakeGround()
{
    mBlast->setScale(0.5f);
    mBlast->setOpacity(255);
    
    //Create circl
    CCScaleTo* scale1 = CCScaleTo::create(0.5f, 3.0f);
    CCFadeOut* fade1 = CCFadeOut::create(0.25f);
    CCDelayTime* delay1 = CCDelayTime::create(0.25f);
    CCSequence* seq1 = CCSequence::create(delay1,fade1,NULL);
    CCSpawn* spawn1 = CCSpawn::create(scale1,seq1,NULL);
    mBlast->runAction(spawn1);
    
    _gameScene->runAction(CCShake::actionWithDuration( 0.5f, 5.0f ));
    _gameScene->clearAllMap();
}

void Machine_Stamp::SetAnimation_Use()
{
    mHandRight->stopAllActions();
    mHandLeft->stopAllActions();
    mLegRight->stopAllActions();
    mLegLeft->stopAllActions();
    mBaseBlock->stopAllActions();
    
    //The finish
//    CCMoveTo* moveA1 = CCMoveTo::create(0.15f,ccp(0,18));
    CCMoveTo* moveA1 = CCMoveTo::create(0.15f,ccp(-5,18));
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Stamp::SetAnimation_Grow));
    CCSequence* seqA1 = CCSequence::create(moveA1,func,NULL);
    mBaseBlock->runAction(seqA1);
    
    CCDelayTime* aAction = CCDelayTime::create(0.22f);
    CCCallFuncN* func3 = CCCallFuncN::create(this, callfuncN_selector(Machine_Stamp::ShakeGround));
    CCSequence* aSeqM1 = CCSequence::create(aAction,func3,NULL);
    runAction(aSeqM1);
    
    
    CCMoveTo* moveB1 = CCMoveTo::create(0.15f,ccp(-44,-15));
    CCRotateTo* rotB1 = CCRotateTo::create(0.25f, -40);
    CCSpawn* spawnB1 = CCSpawn::create(moveB1,rotB1,NULL);
    mLegLeft->runAction(spawnB1);
    
//    CCMoveTo* moveC1 = CCMoveTo::create(0.15f,ccp(40,-15));
    CCMoveTo* moveC1 = CCMoveTo::create(0.15f,ccp(35,-15));
    CCRotateTo* rotC1 = CCRotateTo::create(0.15f, 40);
    CCSpawn* spawnC1 = CCSpawn::create(moveC1,rotC1,NULL);
    mLegRight->runAction(spawnC1);
    
    CCMoveTo* moveD1 = CCMoveTo::create(0.15f,ccp(-42,10));
    CCRotateTo* rotD1 = CCRotateTo::create(0.15f, 10);
    CCSpawn* spawnD1 = CCSpawn::create(moveD1,rotD1,NULL);
    mHandLeft->runAction(spawnD1);
    
//    CCMoveTo* moveE1 = CCMoveTo::create(0.15f,ccp(39,10));
    CCMoveTo* moveE1 = CCMoveTo::create(0.15f,ccp(34,10));
    CCRotateTo* rotE1 = CCRotateTo::create(0.15f, -10);
    CCSpawn* spawnE1 = CCSpawn::create(moveE1,rotE1,NULL);
    mHandRight->runAction(spawnE1);
    
    //Clear the idle anim
//    mBaseBlock->stopAllActions();
//    mBaseBlock->setOpacity(255);
//    
//    CCMoveTo* aMove = CCMoveTo::create(1.0f,ccp(0,0));
//    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Stamp::SetAnimation_Grow));
//    CCSequence* aSeq = CCSequence::create(aMove,func,NULL);
//    mBaseBlock->runAction(aSeq);
    
    _gameScene->playInGameSound("S_Activate");
    
    mCanBeActivated = false;
}

void Machine_Stamp::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void Machine_Stamp::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}

//The touch stuff - possible not needed !!!
bool Machine_Stamp::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    bool result = false;
	mTouched = false;
    
	if (ccpDistanceSQ(touch->getLocation(), getPosition()) <= 200)
	{
        //Try to get money from player !!!
        mTouched = true;
	}
	
	return result;
}

void Machine_Stamp::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(mTouched)
    {
        //Try to end it now !!!
    }
}