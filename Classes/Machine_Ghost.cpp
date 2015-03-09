//
//  Machine_Ghost.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 02/11/13.
//
//

#include "Machine_Ghost.h"

#include "GameScene.h"
#include "Dwarf.h"
#include "SpriteAnimation.h"
#include "TimedSpriteAnimation.h"

#include "User.h"

USING_NS_CC;

Machine_Ghost* Machine_Ghost::create(GameScene* gameScene)
{
	Machine_Ghost *pRet = new Machine_Ghost();
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

Machine_Ghost::Machine_Ghost():
_LeafRight(NULL),_LeafLeft(NULL),_LeafCenter(NULL),_LeafBottom(NULL)
{
}

Machine_Ghost::~Machine_Ghost()
{
}

bool Machine_Ghost::init(GameScene* gameScene)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    _gameScene = gameScene;
    
    //Lets get the animations in order
    mBlast = CCSprite::create("powerup/Machine_Stamp/blast_impact.png");
    mBroken = NULL;
    
    mCanBeActivated = false;
    
    //To do - offests for each level :/
    switch (mUpgradeLevel) {
        case 1:
            mChimney_1_IdleStart = (ElementInfo){252, 719 - 0, 5};
            mChimney_1_IdleEnd = (ElementInfo){252, 719 - 0, 5};
            
            mChimney_2_IdleStart = (ElementInfo){252, 719 - 0, 5};
            mChimney_2_IdleEnd = (ElementInfo){252, 719 - 0, 5};
            
            mChimney_1_UseStart = (ElementInfo){252, 719 - 0, 5};
            mChimney_1_UseEnd = (ElementInfo){252, 719 - 0, 5};
            
            mChimney_2_UseStart = (ElementInfo){252, 719 - 0, 5};
            mChimney_2_UseEnd = (ElementInfo){252, 719 - 0, 5};
            break;
            
        default:
            break;
    }
    
    //Check in what state is this machine now !!!
//    mUpgradeLevel = User::getInstance()->getMachine_GhostLvl();
    
    if(mUpgradeLevel<1)
    {
        //This item is broken
        mBroken = CCSprite::create("powerup/Machine_Ghost/ghost_dwarfs_broken.png");
        addChild(mBroken);
    }
    else
    {
        mJelly = SpriteAnimation::create("powerup/Machine_Ghost/ghost_jelly.plist",-10);
		mJelly->retain();
        mJelly->setPosition(ccp(-6,-8));
        addChild(mJelly,8);
        
        mJelly_FakeMask = CCSprite::create("powerup/Machine_Ghost/Ghoust_Mask.png");
        mJelly_FakeMask->setPosition(ccp(-5,12));//-27
        mJelly_FakeMask->setAnchorPoint(ccp(0.5,1));
        addChild(mJelly_FakeMask,9);
        
        mSiksna = SpriteAnimation::create("powerup/Machine_Ghost/siksna.plist",-10);
		mSiksna->retain();
        mSiksna->setPosition(ccp(26,11));
        addChild(mSiksna,7);
        mSiksna->pause();
        
        
        mSmokeBlue = SpriteAnimation::create("powerup/Machine_Ghost/smoke_blue.plist",false);
		mSmokeBlue->retain();
        mSmokeBlue->setTag(111);
        
//        mSmokeBlue->setPosition(ccp(16,70));//Labais
        
//        mSmokeBlue->setFlipX(true);
//        mSmokeBlue->setPosition(ccp(-65,60));//Kreisais
        
//        addChild(mSmokeBlue,7);
        
        CreateMachineByLevel(mUpgradeLevel);
    }
    
    return true;
}

void Machine_Ghost::SetLeftSmoke()
{
    if(getChildByTag(111))
        removeChild(mSmokeBlue);
    
    mSmokeBlue->setFlipX(false);
    mSmokeBlue->setPosition(ccp(16,70));//Labais
    addChild(mSmokeBlue,7);
}

void Machine_Ghost::SetRightSmoke()
{
    if(getChildByTag(111))
        removeChild(mSmokeBlue);
    
    mSmokeBlue->setFlipX(true);
    mSmokeBlue->setPosition(ccp(-65,60));//Kreisais
    addChild(mSmokeBlue,7);
}

void Machine_Ghost::ClearOldParts()
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

void Machine_Ghost::CreateMachineByLevel(int theLevel)
{
    theLevel = 1;
    
    ClearOldParts();
    
    mLevel = theLevel;
    
    char theFileName[64];
    sprintf(theFileName, "powerup/Machine_Ghost/level%i/ghost_base_lvl%i.png", theLevel,theLevel);
    mBase = CCSprite::create(theFileName);
    mBase->setPosition(ccp(4,2));
    addChild(mBase,10);
    
    sprintf(theFileName, "powerup/Machine_Ghost/level%i/wheel_lvl%d.png", theLevel,theLevel);
    mBaseBlock = CCSprite::create(theFileName);
    mBaseBlock->setPosition(ccp(25,29));
    addChild(mBaseBlock,9);
    
    //The legs
    sprintf(theFileName, "powerup/Machine_Ghost/level%i/chimney_lvl%d.png", theLevel,theLevel);
    mLegLeft = CCSprite::create(theFileName);
    mLegLeft->setPosition(ccp(-12,44));
    mLegLeft->setRotation(-12);
    addChild(mLegLeft,8);
    
    sprintf(theFileName, "powerup/Machine_Ghost/level%i/chimney_lvl%d.png", theLevel,theLevel);
    mLegRight = CCSprite::create(theFileName);
    mLegRight->setFlipX(true);
    mLegRight->setPosition(ccp(-34,38));
    mLegRight->setRotation(-12);
    addChild(mLegRight,8);
    
    //The arms
//    sprintf(theFileName, "powerup/Machine_Ghost/level%i/kaats_top_lvl%d.png", theLevel,theLevel);
//    mHandLeft = CCSprite::create(theFileName);
//    addChild(mHandLeft,7);
//    
//    sprintf(theFileName, "powerup/Machine_Ghost/level%i/kaats_top_lvl%d.png", theLevel,theLevel);
//    mHandRight = CCSprite::create(theFileName);
//    mHandRight->setFlipX(true);
//    addChild(mHandRight,7);
    
    if(_gameScene != NULL){
        SetAnimation_Grow();
    }
}


void Machine_Ghost::SetAnimation_Grow()
{
    mSiksna->pause();
    
    if(getChildByTag(111))
        removeChild(mSmokeBlue);
    
    mLegLeft->stopAllActions();
    mLegRight->stopAllActions();
    mBaseBlock->stopAllActions();
    
//    CCMoveTo* aMoveA1 = CCMoveTo::create(0.5f, ccp(-12,44));
    CCMoveTo* aMoveA1 = CCMoveTo::create(0.5f, ccp(-10,46));
    mLegLeft->runAction(aMoveA1);
    
//    CCMoveTo* aMoveA2 = CCMoveTo::create(0.5f, ccp(-34,38));
    CCMoveTo* aMoveA2 = CCMoveTo::create(0.5f, ccp(-33,40));
    mLegRight->runAction(aMoveA2);
    
    
    //play the power up machine anim
    CCScaleTo* aScale1 = CCScaleTo::create(30.0f, 1.0f, 0);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Ghost::SetAnimation_Ready));
    CCSequence* aSeq = CCSequence::create(aScale1,func,NULL);
    mJelly_FakeMask->runAction(aSeq);
    
    mCanBeActivated = false;
}

void Machine_Ghost::SetAnimation_Ready()
{
    mLegLeft->stopAllActions();
    mLegRight->stopAllActions();
    
    CCDelayTime* aDelayA = CCDelayTime::create(0.3f);
//    CCMoveTo* aMoveA1 = CCMoveTo::create(0.12f, ccp(-11,43));
//    CCMoveTo* aMoveA2 = CCMoveTo::create(0.12f, ccp(-12,46));
    CCMoveTo* aMoveA1 = CCMoveTo::create(0.12f, ccp(-10,46));
    CCMoveTo* aMoveA2 = CCMoveTo::create(0.12f, ccp(-11,50));
    CCSequence* aSeqA = CCSequence::create(aMoveA1,aDelayA,aMoveA2,NULL);
    CCRepeatForever* aRepeatA = CCRepeatForever::create(aSeqA);
    mLegLeft->runAction(aRepeatA);
    
    
    CCDelayTime* aDelay1 = CCDelayTime::create(0.3f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Ghost::Create2ndAction));
    CCSequence* aSeq2 = CCSequence::create(aDelay1,func,NULL);
    runAction(aSeq2);
    
    _gameScene->playInGameSound("GS_Done");
    
//    CCFadeTo* aFade1 = CCFadeTo::create(1.0f, 128);
//    CCFadeTo* aFade2 = CCFadeTo::create(1.0f, 255);
//    CCSequence* aSeq1 = CCSequence::create(aFade1,aFade2,NULL);
//    CCRepeatForever* aRep = CCRepeatForever::create(aSeq1);
//    mBaseBlock->runAction(aRep);
    
    mCanBeActivated = true;
}

void Machine_Ghost::Create2ndAction()
{
    CCDelayTime* aDelayB = CCDelayTime::create(0.3f);
//    CCMoveTo* aMoveB1 = CCMoveTo::create(0.12f, ccp(-34,36));
//    CCMoveTo* aMoveB2 = CCMoveTo::create(0.12f, ccp(-36,40));
    CCMoveTo* aMoveB1 = CCMoveTo::create(0.12f, ccp(-33,40));
    CCMoveTo* aMoveB2 = CCMoveTo::create(0.12f, ccp(-34,44));
    CCSequence* aSeqB = CCSequence::create(aMoveB1,aDelayB,aMoveB2,NULL);
    CCRepeatForever* aRepeatB = CCRepeatForever::create(aSeqB);
    mLegRight->runAction(aRepeatB);
}

void Machine_Ghost::SetAnimation_Use(int theTime)
{
    mLegLeft->stopAllActions();
    mLegRight->stopAllActions();
    
    CCDelayTime* aDelayA1 = CCDelayTime::create(1.0f);
//    CCMoveTo* aMoveA1 = CCMoveTo::create(0.12f, ccp(-14,52));
    CCMoveTo* aMoveA1 = CCMoveTo::create(0.12f, ccp(-12,52));
    CCDelayTime* aDelayA2 = CCDelayTime::create(1.0f);
//    CCMoveTo* aMoveA2 = CCMoveTo::create(0.12f, ccp(-12,43));
    CCMoveTo* aMoveA2 = CCMoveTo::create(0.12f, ccp(-10,43));
    
    CCCallFuncN* funcA1 = CCCallFuncN::create(this, callfuncN_selector(Machine_Ghost::SetLeftSmoke));
    
    CCSequence* aSeqA = CCSequence::create(aMoveA1,funcA1,aDelayA1,aMoveA2,aDelayA2,NULL);
    CCRepeatForever* aRepeatA = CCRepeatForever::create(aSeqA);
    mLegLeft->runAction(aRepeatA);
    
    CCDelayTime* aDelayB1 = CCDelayTime::create(1.0f);
//    CCMoveTo* aMoveB1 = CCMoveTo::create(0.12f, ccp(-34,36));
    CCMoveTo* aMoveB1 = CCMoveTo::create(0.12f, ccp(-33,38));
    CCDelayTime* aDelayB2 = CCDelayTime::create(1.0f);
//    CCMoveTo* aMoveB2 = CCMoveTo::create(0.12f, ccp(-36,46));
    CCMoveTo* aMoveB2 = CCMoveTo::create(0.12f, ccp(-35,46));
    
    CCCallFuncN* funcB1 = CCCallFuncN::create(this, callfuncN_selector(Machine_Ghost::SetRightSmoke));
    
    CCSequence* aSeqB = CCSequence::create(aMoveB1,aDelayB2,aMoveB2,funcB1,aDelayB1,NULL);
    CCRepeatForever* aRepeatB = CCRepeatForever::create(aSeqB);
    mLegRight->runAction(aRepeatB);
    
    CCRotateBy* aRotate = CCRotateBy::create(0.1f, 50);
    CCRepeatForever* aRepeat1 = CCRepeatForever::create(aRotate);
    mBaseBlock->runAction(aRepeat1);
    
    //-----------------------------------------------------------
    
    //Add some other cool stuff - like timer when ends !!!
    mCurrentNumberID = 3;
    
    CCDelayTime* aDelayTotal = CCDelayTime::create(theTime-4);
    
    CCDelayTime* aDelayNum_3 = CCDelayTime::create(1.0f);
    CCCallFuncN* aPopNum_3 = CCCallFuncN::create(this, callfuncN_selector(Machine_Ghost::ShowNumber));
    CCSequence* aSeqNum_3 = CCSequence::create(aDelayNum_3,aPopNum_3,NULL);
    
    CCDelayTime* aDelayNum_2 = CCDelayTime::create(1.0f);
    CCCallFuncN* aPopNum_2 = CCCallFuncN::create(this, callfuncN_selector(Machine_Ghost::ShowNumber));
    CCSequence* aSeqNum_2 = CCSequence::create(aDelayNum_2,aPopNum_2,NULL);
    
    CCDelayTime* aDelayNum_1 = CCDelayTime::create(1.0f);
    CCCallFuncN* aPopNum_1 = CCCallFuncN::create(this, callfuncN_selector(Machine_Ghost::ShowNumber));
    CCSequence* aSeqNum_1 = CCSequence::create(aDelayNum_1,aPopNum_1,NULL);
    
    CCSequence* aSeqNumTotal = CCSequence::create(aDelayTotal,aSeqNum_3,aSeqNum_2,aSeqNum_1,NULL);
    
    CCScaleTo* aScale1 = CCScaleTo::create(theTime, 1.0f, 1.0f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Ghost::SetAnimation_Grow));
    CCSequence* aSeq = CCSequence::create(aScale1,func,NULL);
    
    CCSpawn* aSpawnSpecial1 = CCSpawn::create(aSeq,aSeqNumTotal,NULL);
    mJelly_FakeMask->runAction(aSpawnSpecial1);
    
    mSiksna->resume();
    
//    //Clear the idle anim
//    mBaseBlock->stopAllActions();
//    mBaseBlock->setOpacity(255);
//    
//    CCMoveTo* aMove = CCMoveTo::create(1.0f,ccp(0,0));
//    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Ghost::SetAnimation_Grow));
//    CCSequence* aSeq = CCSequence::create(aMove,func,NULL);
//    mBaseBlock->runAction(aSeq);
    
    _gameScene->playInGameSound("GS_Activate");
    
    mCanBeActivated = false;
}

void Machine_Ghost::ShowNumber()
{
    CCSprite* frame_1;
    
    if(mCurrentNumberID == 3) frame_1 = CCSprite::create("Interfeiss/save_me/3.png");
    else if(mCurrentNumberID == 2) frame_1 = CCSprite::create("Interfeiss/save_me/2.png");
    else if(mCurrentNumberID == 1) frame_1 = CCSprite::create("Interfeiss/save_me/1.png");
    
    frame_1->setScale(0.5f);
    
    CCDelayTime* aDelay1 = CCDelayTime::create(0.8f);
    CCFadeOut* aFade1 = CCFadeOut::create(0.2f);
    CCCallFuncN* func1 = CCCallFuncN::create(this, callfuncN_selector(Machine_Ghost::OnRemove));
    CCSequence* aSeq1 = CCSequence::create(aDelay1,aFade1,func1,NULL);
    
    CCMoveBy* aMove1 = CCMoveBy::create(1.0f,ccp(0,-60));
    CCSpawn* aSpawn1 = CCSpawn::create(aSeq1,aMove1,NULL);
    frame_1->runAction(aSpawn1);
    
    addChild(frame_1,1000);
    //    addChild(frame_1);
    
    mCurrentNumberID-=1;
}

void Machine_Ghost::OnRemove(CCNode* sender)
{
    removeChild(sender,true);
}

void Machine_Ghost::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void Machine_Ghost::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}

//The touch stuff - possible not needed !!!
bool Machine_Ghost::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
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

void Machine_Ghost::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(mTouched)
    {
        //Try to end it now !!!
    }
}