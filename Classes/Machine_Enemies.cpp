//
//  Machine_Enemies.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 02/11/13.
//
//

#include "Machine_Enemies.h"

#include "GameScene.h"
#include "Dwarf.h"
#include "SpriteAnimation.h"
#include "TimedSpriteAnimation.h"

#include "User.h"

USING_NS_CC;

Machine_Enemies* Machine_Enemies::create(GameScene* gameScene)
{
	Machine_Enemies *pRet = new Machine_Enemies();
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

Machine_Enemies::Machine_Enemies():
_LeafRight(NULL),_LeafLeft(NULL),_LeafCenter(NULL),_LeafBottom(NULL)
{
}

Machine_Enemies::~Machine_Enemies()
{
}

bool Machine_Enemies::init(GameScene* gameScene)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    //Lets get the animations in order
    mBlast = CCSprite::create("powerup/Machine_Stamp/blast_impact.png");
    mBroken = NULL;
    
    mCanBeActivated = false;
    
    _gameScene = gameScene;
    
    setTag(121212122);
    
    //Check in what state is this machine now !!!
//    mUpgradeLevel = User::getInstance()->getMachine_EnemiesLvl();
//    mUpgradeLevel = 1;
    
    mUpgradeLevel = 0;
    
    if(User::getInstance()->mSpecial_12_Mission && gameScene)
    {
        //Get from gamescreen current dwarf amount transfere
        mUpgradeLevel = 0;
        if(gameScene->_DwarfsEnteredCave<5){
            mUpgradeLevel = 0;
        }
        else if(gameScene->_DwarfsEnteredCave<10){
            mUpgradeLevel = 1;
        }
        else if(gameScene->_DwarfsEnteredCave<15){
            mUpgradeLevel = 2;
        }
        else if(gameScene->_DwarfsEnteredCave<20){
            mUpgradeLevel = 3;
        }
        else if(gameScene->_DwarfsEnteredCave<25){
            mUpgradeLevel = 4;
        }
        else{
            mUpgradeLevel = 5;
        }
    }
    
    if(mUpgradeLevel<1)
    {
        //This item is broken
        mBroken = CCSprite::create("powerup/Machine_Enemies/elektrobuuda_broken.png");
        addChild(mBroken);
    }
    else
    {
        mProgress = CCSprite::create("powerup/Machine_Enemies/progress_bar.png");
//        mProgress->setPosition(ccp(5,-4));//-27
        mProgress->setPosition(ccp(12,-7));//-27
        mProgress->setTag(121212122);
        addChild(mProgress,7);
        
        mJelly_FakeMask = CCSprite::create("powerup/Machine_Ghost/Ghoust_Mask.png");
        mJelly_FakeMask->setPosition(ccp(6,17));//-27
        mJelly_FakeMask->setScaleX(0.5f);
        mJelly_FakeMask->setScaleY(1.1f);
        mJelly_FakeMask->setAnchorPoint(ccp(0.5,1));
        mJelly_FakeMask->setTag(121212122);
        addChild(mJelly_FakeMask,8);
        
        mSparkAnim = SpriteAnimation::create("powerup/Machine_Enemies/spark_ready.plist",-10);
		mSparkAnim->retain();
        mSparkAnim->setTag(111);
        mSparkAnim->setPosition(ccp(-1,65));
        
        mSparkActive = SpriteAnimation::create("powerup/Machine_Enemies/elektrospark_action.plist",-10);
		mSparkActive->retain();
        mSparkActive->setTag(222);
//        mSparkActive->setPosition(ccp(-3,55));//lvl1
        mSparkActive->setPosition(ccp(0,57));//lvl1
        
        CreateMachineByLevel(mUpgradeLevel);
    }
    
    return true;
}

void Machine_Enemies::ClearOldParts()
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

void Machine_Enemies::CreateMachineByLevel(int theLevel)
{
    ClearOldParts();
    
    char theFileName[64];
    sprintf(theFileName, "powerup/Machine_Enemies/level%i/elektro_base_lvl%i.png", theLevel,theLevel);
    mBase = CCSprite::create(theFileName);
    addChild(mBase,10);
    
    sprintf(theFileName, "powerup/Machine_Enemies/level%i/zb_rats_lvl%d.png", theLevel,theLevel);
    mBaseBlock = CCSprite::create(theFileName);
//    mBaseBlock->setPosition(ccp(-40,5));//lvl1
    mBaseBlock->setPosition(ccp(-37,4));//lvl1
    addChild(mBaseBlock,9);
    
    //The legs
    sprintf(theFileName, "powerup/Machine_Enemies/level%i/antenas_lvl%d.png", theLevel,theLevel);
    mLegLeft = CCSprite::create(theFileName);
    mLegLeft->setPosition(ccp(0,50));
    addChild(mLegLeft,8);
    
//    sprintf(theFileName, "powerup/Machine_Enemies/level%i/kaats_bottom_lvl%d.png", theLevel,theLevel);
//    mLegRight = CCSprite::create(theFileName);
//    mLegRight->setFlipX(true);
//    addChild(mLegRight,8);
//    
//    //The arms
//    sprintf(theFileName, "powerup/Machine_Enemies/level%i/kaats_top_lvl%d.png", theLevel,theLevel);
//    mHandLeft = CCSprite::create(theFileName);
//    addChild(mHandLeft,7);
//    
//    sprintf(theFileName, "powerup/Machine_Enemies/level%i/kaats_top_lvl%d.png", theLevel,theLevel);
//    mHandRight = CCSprite::create(theFileName);
//    mHandRight->setFlipX(true);
//    addChild(mHandRight,7);
    
    if(_gameScene != NULL){
        SetAnimation_Grow();
    }
}


void Machine_Enemies::SetAnimation_Grow()
{
    mBaseBlock->stopAllActions();
    
    if(getChildByTag(222))
        removeChild(mSparkActive);
    
    _gameScene->removeRemovableEffects();
    
    mCanBeActivated = false;
    
    CCScaleTo* aScale1;
    if(_gameScene && _gameScene->mTutorialEnabled && _gameScene->mTutorialStep<33)
        aScale1 = CCScaleTo::create(30.0f, 0.5,0.0f);
    else
        aScale1 = CCScaleTo::create(30.0f, 0.5,0.0f);
    
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Enemies::SetAnimation_Ready));
    CCSequence* aSeq1 = CCSequence::create(aScale1,func,NULL);
    mJelly_FakeMask->runAction(aSeq1);
    
//    //Base for now !!!
//    CCMoveTo* aMove = CCMoveTo::create(5.0f,ccp(0,60));
//    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Enemies::SetAnimation_Ready));
//    CCSequence* aSeq = CCSequence::create(aMove,func,NULL);
//    mBaseBlock->runAction(aSeq);
}

void Machine_Enemies::ForceSetReady()
{
    mCanBeActivated = true;
    
    if(mJelly_FakeMask->numberOfRunningActions()>0)
        mJelly_FakeMask->stopAllActions();
    mJelly_FakeMask->setScaleY(0.0f);
    
    
    SetAnimation_Ready();
}

void Machine_Enemies::SetAnimation_Ready()
{
    addChild(mSparkAnim,11);
    
    _gameScene->playInGameSound("ET_Done");
    
//    CCFadeTo* aFade1 = CCFadeTo::create(1.0f, 128);
//    CCFadeTo* aFade2 = CCFadeTo::create(1.0f, 255);
//    CCSequence* aSeq1 = CCSequence::create(aFade1,aFade2,NULL);
//    CCRepeatForever* aRep = CCRepeatForever::create(aSeq1);
//    mBaseBlock->runAction(aRep);
    
    if(User::getInstance()->mSpecialMissionBuild || User::getInstance()->mNewMissionBuild)
    {
        if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission
           || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
           || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission)
        {
            //All chill
        }
        else
        {
            //Check if is not completed already
            if(User::getInstance()->_tutorial_small_6 == 0)
            {
                //1st time pause the game and show some stuff !!!
                _gameScene->stopInGameSound("Footsteps",true);
                _gameScene->stopInGameSound("troll_walk",true);
                _gameScene->stopInGameSound("dwarf_web_stuck",true);
                
                //Get the mid point !!!
                int aShowX = _gameScene->getChildByTag(1030)->getPositionX();
                int aShowY = _gameScene->getChildByTag(1030)->getPositionY();
                
                _gameScene->CreateSmallTutorial(6,aShowX,aShowY);
            }
        }
    }
    
    mCanBeActivated = true;
}

void Machine_Enemies::SetAnimation_Use(int theTime)
{
    if(getChildByTag(111))
        removeChild(mSparkAnim);
    
    _gameScene->playInGameSound("ET_Activate");//plant_crystal_pop
    
    addChild(mSparkActive,11);
    
    CCRotateBy* a1 = CCRotateBy::create(0.5f, 100);
    CCRepeatForever* ar1 = CCRepeatForever::create(a1);
    mBaseBlock->runAction(ar1);
    
    CCScaleTo* aScale1 = CCScaleTo::create(theTime, 0.5,1.1f);
    
    //Add some other cool stuff - like timer when ends !!!
    mCurrentNumberID = 3;
    
    CCDelayTime* aDelayTotal = CCDelayTime::create(theTime-4);
    
    CCDelayTime* aDelayNum_3 = CCDelayTime::create(1.0f);
    CCCallFuncN* aPopNum_3 = CCCallFuncN::create(this, callfuncN_selector(Machine_Enemies::ShowNumber));
    CCSequence* aSeqNum_3 = CCSequence::create(aDelayNum_3,aPopNum_3,NULL);
    
    CCDelayTime* aDelayNum_2 = CCDelayTime::create(1.0f);
    CCCallFuncN* aPopNum_2 = CCCallFuncN::create(this, callfuncN_selector(Machine_Enemies::ShowNumber));
    CCSequence* aSeqNum_2 = CCSequence::create(aDelayNum_2,aPopNum_2,NULL);
    
    CCDelayTime* aDelayNum_1 = CCDelayTime::create(1.0f);
    CCCallFuncN* aPopNum_1 = CCCallFuncN::create(this, callfuncN_selector(Machine_Enemies::ShowNumber));
    CCSequence* aSeqNum_1 = CCSequence::create(aDelayNum_1,aPopNum_1,NULL);
    
    //The mega stuff
    CCSequence* aSeqNumTotal = CCSequence::create(aDelayTotal,aSeqNum_3,aSeqNum_2,aSeqNum_1,NULL);
    
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Enemies::SetAnimation_Grow));
    CCSequence* aSeq1 = CCSequence::create(aScale1,func,NULL);
    
    CCSpawn* aSpawnSpecial1 = CCSpawn::create(aSeq1,aSeqNumTotal,NULL);
    mJelly_FakeMask->runAction(aSpawnSpecial1);
    
//    //Clear the idle anim
//    mBaseBlock->stopAllActions();
//    mBaseBlock->setOpacity(255);
//    
//    CCMoveTo* aMove = CCMoveTo::create(1.0f,ccp(0,0));
//    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Machine_Enemies::SetAnimation_Grow));
//    CCSequence* aSeq = CCSequence::create(aMove,func,NULL);
//    mBaseBlock->runAction(aSeq);
    
    mCanBeActivated = false;
}

void Machine_Enemies::ShowNumber()
{
    CCSprite* frame_1;
    
    if(mCurrentNumberID == 3) frame_1 = CCSprite::create("Interfeiss/save_me/3.png");
    else if(mCurrentNumberID == 2) frame_1 = CCSprite::create("Interfeiss/save_me/2.png");
    else if(mCurrentNumberID == 1) frame_1 = CCSprite::create("Interfeiss/save_me/1.png");
    
    frame_1->setScale(0.5f);
    
    CCDelayTime* aDelay1 = CCDelayTime::create(0.8f);
    CCFadeOut* aFade1 = CCFadeOut::create(0.2f);
    CCCallFuncN* func1 = CCCallFuncN::create(this, callfuncN_selector(Machine_Enemies::OnRemove));
    CCSequence* aSeq1 = CCSequence::create(aDelay1,aFade1,func1,NULL);
    
    CCMoveBy* aMove1 = CCMoveBy::create(1.0f,ccp(0,-60));
    CCSpawn* aSpawn1 = CCSpawn::create(aSeq1,aMove1,NULL);
    frame_1->runAction(aSpawn1);
    
    addChild(frame_1,1000);
//    addChild(frame_1);
    
    mCurrentNumberID-=1;
}

void Machine_Enemies::OnRemove(CCNode* sender)
{
    removeChild(sender,true);
}

void Machine_Enemies::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void Machine_Enemies::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}

//The touch stuff - possible not needed !!!
bool Machine_Enemies::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
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

void Machine_Enemies::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(mTouched)
    {
        //Try to end it now !!!
    }
}