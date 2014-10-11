//
//  Plant_Crystal.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 10/6/13.
//
//

#include "PlantCrystal.h"
#include "GameScene.h"
#include "Dwarf.h"
#include "SpriteAnimation.h"
#include "TimedSpriteAnimation.h"

USING_NS_CC;

PlantCrystal* PlantCrystal::create(GameScene* gameScene)
{
	PlantCrystal *pRet = new PlantCrystal();
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

PlantCrystal::PlantCrystal():
_LeafRight(NULL),_LeafLeft(NULL),_LeafCenter(NULL),_LeafBottom(NULL)
{
}

PlantCrystal::~PlantCrystal()
{
}

bool PlantCrystal::init(GameScene* gameScene)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    mTouched = false;
    
    _gameScene = gameScene;
    gameScene->retain();
    
    //The magic
    _LeafRight = CCSprite::create("powerup/Crystal_plant/lapele1.png");
    _LeafRight->setFlipX(true);
    
    _LeafLeft = CCSprite::create("powerup/Crystal_plant/lapele1.png");
    
    _LeafCenter = CCSprite::create("powerup/Crystal_plant/plant_main.png");
    _LeafBottom = CCSprite::create("powerup/Crystal_plant/lapel2.png");
    
    //Grow up an pop
    _LeafRight->setPositionX(30);
    _LeafRight->setPositionY(-25);
    
    _LeafLeft->setPositionX(-30);
    _LeafLeft->setPositionY(-25);
    
    _LeafBottom->setPositionY(-36);
    
//    _LeafBottom->setScale(1.0);
//    _LeafLeft->setScale(1.0);
//    _LeafRight->setScale(0.30);
//    _LeafCenter->setScale(0.30);
    
    addChild(_LeafRight);
    addChild(_LeafLeft);
    addChild(_LeafCenter);
    addChild(_LeafBottom);
    
    this->setScale(0.0f);
    
    CCScaleTo* aScale_1 = CCScaleTo::create(1, 0.5);
    CCRotateTo* aRotate_1 = CCRotateTo::create(0.1, 20);
    CCRotateTo* aRotate_2 = CCRotateTo::create(0.1, -20);
    CCRotateTo* aRotete_3 = CCRotateTo::create(0.5, 0);
    
    //After this start the grow !!!
    CCSequence* aSeq1 = CCSequence::create(aRotate_1,aRotate_2,aRotete_3,NULL);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(PlantCrystal::onFinishShowUp));
    CCSequence* aSeq2 = CCSequence::create(aScale_1,func,NULL);
    
    CCSpawn* sSpawn = CCSpawn::create(aSeq1,aSeq2,NULL);
    this->runAction(sSpawn);
    
    //scale up
    /*
    CCScaleTo* aScaleUp_1 = CCScaleTo::create(10, 1.0);
    _LeafRight->runAction(aScaleUp_1);
    
    CCScaleTo* aScaleUp_2 = CCScaleTo::create(10, 1.0);
    _LeafLeft->runAction(aScaleUp_2);
    
    CCScaleTo* aScaleUp_3 = CCScaleTo::create(10, 1.0);
    _LeafCenter->runAction(aScaleUp_3);
    
    CCScaleTo* aScaleUp_4 = CCScaleTo::create(27, 1.0);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Plant_Crystal::OnFinishedGrow));
    CCDelayTime* aTime = CCDelayTime::create(3.0);
    CCSequence* seqfin = CCSequence::create(aScaleUp_4,aTime,func,NULL);
    _LeafBottom->runAction(seqfin);
    */
    
    return true;
}

void PlantCrystal::onFinishShowUp()
{
    //start normal growup
    CCScaleTo* aSacale_b0 = CCScaleTo::create(2.5f, 1.8f,2.0f);
    
    CCActionInterval* aScale_b1 = CCScaleTo::create(0.5f, 2.0f, 2.5f);
    CCFiniteTimeAction* action_b1 = CCEaseBackOut::create(aScale_b1);//::create(aScale_b1, 3);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(PlantCrystal::onStartToShake));
    CCDelayTime* aDelay = CCDelayTime::create(0.5f);
    CCSequence* aSeqb1 = CCSequence::create(aSacale_b0,action_b1,aDelay,func,NULL);
    
    CCMoveTo* aMove_b1 = CCMoveTo::create(3, ccp(0,40));
    CCSpawn* aSpawn_b1 = CCSpawn::create(aSeqb1,aMove_b1,NULL);
    _LeafCenter->runAction(aSpawn_b1);
    
    
    //------
    
    CCScaleTo* aScale_l1 = CCScaleTo::create(3, 2.0f);
    CCMoveTo* aMove_l1 = CCMoveTo::create(3, ccp(-50,-25));
    CCSpawn* aSpawn_l1 = CCSpawn::create(aScale_l1,aMove_l1,NULL);
    _LeafLeft->runAction(aSpawn_l1);
    
    CCScaleTo* aScale_r1 = CCScaleTo::create(3, 2.0f);
    CCMoveTo* aMove_r1 = CCMoveTo::create(3, ccp(50,-25));
    CCSpawn* aSpawn_r1 = CCSpawn::create(aScale_r1,aMove_r1,NULL);
    _LeafRight->runAction(aSpawn_r1);
    
    CCScaleTo* aScale_m1 = CCScaleTo::create(3, 2.0f);
    CCMoveTo* aMove_m1 = CCMoveTo::create(3, ccp(0,-40));
    CCSpawn* aSpawn_m1 = CCSpawn::create(aScale_m1,aMove_m1,NULL);
    _LeafBottom->runAction(aSpawn_m1);
    
//    OnFinishedGrow;
}

void PlantCrystal::onStartToShake()
{
    CCSkewTo* a1 = CCSkewTo::create(0.1f, 1, 0);
    CCSkewTo* a2 = CCSkewTo::create(0.2f, -5, 0);
    CCSkewTo* a3 = CCSkewTo::create(0.1f, 5, 0);
    CCSkewTo* a4 = CCSkewTo::create(0.1f, 0, 0);
    
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(PlantCrystal::onFinishedGrow));
    CCDelayTime* aDelay = CCDelayTime::create(0.7f);
    CCSequence* aSeq2 = CCSequence::create(aDelay,func,NULL);
    
    CCSequence* aSeq = CCSequence::create(a1,a2,a3,a4,NULL);
    CCSpawn* aSpawn = CCSpawn::create(aSeq,aSeq2,NULL);
    
    _LeafCenter->runAction(aSpawn);
}

void PlantCrystal::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void PlantCrystal::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}

void PlantCrystal::onFinishedGrow()
{
    //Create the anim !!!
    CCDelayTime* aDelay = CCDelayTime::create(0.2f);
    CCFadeOut* aFade = CCFadeOut::create(0.05f);
    CCSequence* aSeq = CCSequence::create(aDelay,aFade,NULL);
    _LeafCenter->runAction(aSeq);
    
    CCDelayTime* aDelay1 = CCDelayTime::create(0.2f);
    CCFadeOut* aFade1 = CCFadeOut::create(0.05f);
    CCSequence* aSeq1 = CCSequence::create(aDelay1,aFade1,NULL);
    _LeafLeft->runAction(aSeq1);
    
    CCDelayTime* aDelay2 = CCDelayTime::create(0.2f);
    CCFadeOut* aFade2 = CCFadeOut::create(0.05f);
    CCSequence* aSeq2 = CCSequence::create(aDelay2,aFade2,NULL);
    _LeafRight->runAction(aSeq2);
    
    CCDelayTime* aDelay3 = CCDelayTime::create(0.2f);
    CCFadeOut* aFade3 = CCFadeOut::create(0.05f);
    CCSequence* aSeq3 = CCSequence::create(aDelay3,aFade3,NULL);
    _LeafBottom->runAction(aSeq3);
    
//    _removeSmoke = SpriteAnimation::create("effects/crash_alarming.plist");
    TimedSpriteAnimation* _animation = TimedSpriteAnimation::create("powerup/Crystal_plant/explosion.plist",1);
    _animation->setScale(2.0f);
    _animation->retain();
    _animation->setPositionY(-10);
    addChild(_animation);
    
    _gameScene->playInGameSound("plant_explode");
    
    schedule(schedule_selector(PlantCrystal::onRemoveThisPlant), 1.0f, 0, 0.0f);
    
    schedule(schedule_selector(PlantCrystal::spawnCrystals), 0.2f, 0, 0.0f);
    
    
//    _gameScene->removeNode(this);//Remove after anim ended
}

void PlantCrystal::spawnCrystals()
{
    schedule(schedule_selector(PlantCrystal::spawnCrystals));
    //Pop some random crystals
    _gameScene->generateCrystalSpecial(this->getPositionX()-50,this->getPositionY()+50);
    _gameScene->generateCrystalSpecial(this->getPositionX()+50,this->getPositionY()+30);
    _gameScene->generateCrystalSpecial(this->getPositionX(),this->getPositionY()+10);
}

void PlantCrystal::onRemoveThisPlant()
{
    unschedule(schedule_selector(PlantCrystal::onRemoveThisPlant));
    _gameScene->removeNode(this);//Remove after anim ended
}

//The touch stuff
bool PlantCrystal::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
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

void PlantCrystal::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (mTouched)
    {
        //Try to end it now !!!
    }
}





