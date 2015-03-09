//
//  Plant_Crystal_Weed.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 04/11/13.
//
//

#include "Plant_Crystal_Weed.h"
#include "GameScene.h"
#include "Dwarf.h"
#include "SpriteAnimation.h"
#include "TimedSpriteAnimation.h"

#include "User.h"

USING_NS_CC;

Plant_Crystal_Weed* Plant_Crystal_Weed::create(GameScene* gameScene)
{
	Plant_Crystal_Weed *pRet = new Plant_Crystal_Weed();
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

Plant_Crystal_Weed::Plant_Crystal_Weed():
_LeafRight(NULL),_LeafLeft(NULL),_LeafCenter(NULL),_LeafBottom(NULL)
{
}

Plant_Crystal_Weed::~Plant_Crystal_Weed()
{
}

bool Plant_Crystal_Weed::init(GameScene* gameScene)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    mTouched = false;
    
    _gameScene = gameScene;
    gameScene->retain();
    
    mSpawnCrystals = 1;
    mPlantsFallenDown = 0;
    
    //Do the magic of grow !!!
    //Check the level of this plant !!!
//    mLevel = 5;
    
    int aDelay = 3;
    int aAnimFrameLimit = 20;
    int aPlants = 1;
    int aPakstID = 1;
    
    float aRealDelay = 4.5f;
    
//    mLevel = User::getInstance()->getMachine_PlantLvl();
    
    if(User::getInstance()->mNewMissionBuild){
        mLevel = 3;
    }
    
    if(mLevel>1 && mLevel<4)
        aPakstID = 2;
    else if(mLevel>=4)
        aPakstID = 3;
    
    if(mLevel == 3 || mLevel == 4)
    {
        aRealDelay = 5.0f;
        aPlants = 2;
        aDelay = 4;
        aAnimFrameLimit = 32;
    }
    else if(mLevel>4)
    {
        aRealDelay = 5.0f;
        aPlants = 3;
        aDelay = 5;
        aAnimFrameLimit = 0;
    }
    
    mSpawnCrystals = aPlants;
    
    char theFileName[64];
    sprintf(theFileName, "powerup/Crystal_weed/paaksts%i.png", aPakstID);
    
    TimedSpriteAnimation* _animation = TimedSpriteAnimation::create("powerup/Crystal_weed/crystalweed_augshana.plist",20,aAnimFrameLimit);
    addChild(_animation);
    
    //Play grow sound
    _gameScene->playInGameSound("CP_Kats");
    
    //When will it be finished
//    schedule(schedule_selector(Plant_Crystal_Weed::onGrowCompleted), aDelay, 0, 0.0f);
    
    //When the crystal bombs show up?
    CCSprite* aBox1 = CCSprite::create(theFileName);
    aBox1->setTag(11);
    aBox1->setRotation(-150);
    aBox1->setAnchorPoint(ccp(0.5,0));
//    aBox1->setOpacity(160);
    aBox1->setScale(0.0f);
    aBox1->setPosition(ccp(-22,28));
    addChild(aBox1,1);
    
    CCDelayTime* aDelay_1a = CCDelayTime::create(1.4f);
    
    CCScaleTo* aScale_1a = CCScaleTo::create(0.75f,1.0f);
    CCActionInterval* aScale_1 = CCEaseElasticOut::create(aScale_1a,0.5f);
    
    CCDelayTime* aDelay_1b = CCDelayTime::create(2.0f);
    CCMoveTo* aMove_1 = CCMoveTo::create(0.1f,ccp(-20,-10));
    
    CCCallFuncN* aFunc1 = CCCallFuncN::create(this, callfuncN_selector(Plant_Crystal_Weed::onPlantFallDown));
    
    //Some extra sound on grow complete - paksts scqueze out
    CCDelayTime* aDelaySoundS = CCDelayTime::create(1.4f);
    CCCallFuncN* aFuncSoundS = CCCallFuncN::create(this, callfuncN_selector(Plant_Crystal_Weed::onGrowCompleted));
    CCSequence* aSeqSoundS = CCSequence::create(aDelaySoundS,aFuncSoundS,NULL);
    
    CCSequence* aSeq_1 = CCSequence::create(aDelay_1a,aScale_1,aDelay_1b,aMove_1,aFunc1,NULL);
    CCSpawn* aSpawn_1 = CCSpawn::create(aSeqSoundS,aSeq_1,NULL);
    
//    aBox1->runAction(aSeq_1);
    aBox1->runAction(aSpawn_1);
    
    //Do we need to
    if(aPlants>=2)
    {
        //Create the 2nd plant
        CCSprite* aBox2 = CCSprite::create(theFileName);
        aBox2->setTag(11);
        aBox2->setRotation(150);
        aBox2->setAnchorPoint(ccp(0.5,0));
//        aBox2->setOpacity(160);
        aBox2->setScale(0.0f);
        aBox2->setPosition(ccp(29,22));
        addChild(aBox2,1);
        
        //The actions!!!
        CCDelayTime* aDelay_2a = CCDelayTime::create(1.9f);
        
        CCScaleTo* aScale_2a = CCScaleTo::create(0.75f,1.0f);
        CCActionInterval* aScale_2 = CCEaseElasticOut::create(aScale_2a,0.5f);
        
        CCDelayTime* aDelay_2b = CCDelayTime::create(2.0f);
        CCMoveTo* aMove_2 = CCMoveTo::create(0.1f,ccp(28,-10));
        
        CCCallFuncN* aFunc2 = CCCallFuncN::create(this, callfuncN_selector(Plant_Crystal_Weed::onPlantFallDown));
        
        //Some extra sound on grow complete - paksts scqueze out
        aDelaySoundS = CCDelayTime::create(1.9f);
        aFuncSoundS = CCCallFuncN::create(this, callfuncN_selector(Plant_Crystal_Weed::onGrowCompleted));
        aSeqSoundS = CCSequence::create(aDelaySoundS,aFuncSoundS,NULL);
        
        CCSequence* aSeq_2 = CCSequence::create(aDelay_2a,aScale_2,aDelay_2b,aMove_2,aFunc2,NULL);
        aSpawn_1 = CCSpawn::create(aSeqSoundS,aSeq_2,NULL);
        
//        aBox2->runAction(aSeq_2);
        aBox2->runAction(aSpawn_1);
    }
    
    if(aPlants>=3)
    {
        //Create the 3rd plant
        CCSprite* aBox3 = CCSprite::create(theFileName);
        aBox3->setTag(11);
        aBox3->setRotation(20);
        aBox3->setAnchorPoint(ccp(0.5,0));
//        aBox3->setOpacity(160);
        aBox3->setScale(0.0f);
        aBox3->setPosition(ccp(12,29));
        addChild(aBox3,1);
        
        //The actions!!!
        CCDelayTime* aDelay_3a = CCDelayTime::create(2.1f);
        
        CCScaleTo* aScale_3a = CCScaleTo::create(0.75f,1.0f);
        CCActionInterval* aScale_3 = CCEaseElasticOut::create(aScale_3a,0.5f);
        
        CCDelayTime* aDelay_3b = CCDelayTime::create(2.0f);
        CCMoveTo* aMove_3 = CCMoveTo::create(0.1f,ccp(11,-18));
        
        CCCallFuncN* aFunc3 = CCCallFuncN::create(this, callfuncN_selector(Plant_Crystal_Weed::onPlantFallDown));
        
        //Some extra sound on grow complete - paksts scqueze out
        aDelaySoundS = CCDelayTime::create(2.1f);
        aFuncSoundS = CCCallFuncN::create(this, callfuncN_selector(Plant_Crystal_Weed::onGrowCompleted));
        aSeqSoundS = CCSequence::create(aDelaySoundS,aFuncSoundS,NULL);
        
        CCSequence* aSeq_3 = CCSequence::create(aDelay_3a,aScale_3,aDelay_3b,aMove_3,aFunc3,NULL);
        aSpawn_1 = CCSpawn::create(aSeqSoundS,aSeq_3,NULL);
        
//        aBox3->runAction(aSeq_3);
        aBox3->runAction(aSpawn_1);
    }
    
    //Set fade out for the weed !!!
    CCDelayTime* aWeedDelay = CCDelayTime::create(aRealDelay);
    CCFadeOut* aFadeOut = CCFadeOut::create(0.5f);
    CCCallFuncN* aFunc3 = CCCallFuncN::create(this, callfuncN_selector(Plant_Crystal_Weed::onRemoveThisPlant));
    CCSequence* aSeqAll = CCSequence::create(aWeedDelay,aFadeOut,aFunc3,NULL);
    _animation->runAction(aSeqAll);
    
    return true;
}

void Plant_Crystal_Weed::onGrowCompleted()
{
    //Create the drop anim?
    _gameScene->playInGameSound("CP_Paksts");//plant_crystal_pop
}

void Plant_Crystal_Weed::onPlantFallDown(CCNode* sender)
{
//    explosion.plist;
    TimedSpriteAnimation* _animation = TimedSpriteAnimation::create("powerup/Crystal_weed/explosion.plist",10);
    _animation->setPosition(ccp(sender->getPositionX()-8,sender->getPositionY()-20));
    addChild(_animation);
    
    CCDelayTime* aRemoveMe = CCDelayTime::create(1.0f);
    CCCallFuncN* aFunc1 = CCCallFuncN::create(this,callfuncN_selector(Plant_Crystal_Weed::onRemoveSmoke));
    CCSequence* aSeq1 = CCSequence::create(aRemoveMe,aFunc1,NULL);
    _animation->runAction(aSeq1);
    
    
    CCFadeOut* aFadeOut = CCFadeOut::create(0.1f);
    sender->runAction(aFadeOut);
    
    //Spawn some crystals !!!
    if(mLevel==1)
    {
        _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()+5,this->getPositionY()+sender->getPositionY()+10);
    }
    else if(mLevel == 2)
    {
        _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()+10,this->getPositionY()+sender->getPositionY()+14);
        _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()-10,this->getPositionY()+sender->getPositionY()+8);
    }
    else if(mLevel==3)
    {
        if(mPlantsFallenDown==0)
        {
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX(),this->getPositionY()+sender->getPositionY()+10);
        }
        else
        {
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()+10,this->getPositionY()+sender->getPositionY()+14);
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()-10,this->getPositionY()+sender->getPositionY()+8);
        }
    }
    else if(mLevel==4)
    {
        _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()+10,this->getPositionY()+sender->getPositionY()+14);
        _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()-10,this->getPositionY()+sender->getPositionY()+8);
    }
    else
    {
        //The ultimate
        if(mPlantsFallenDown==0)
        {
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()+10,this->getPositionY()+sender->getPositionY()+14);
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()-10,this->getPositionY()+sender->getPositionY()+8);
        }
        else if(mPlantsFallenDown==1)
        {
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()+10,this->getPositionY()+sender->getPositionY()+14);
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()-10,this->getPositionY()+sender->getPositionY()+8);
        }
        else
        {
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX(),this->getPositionY()+sender->getPositionY()+10);
        }
    }
    
    mPlantsFallenDown++;
    
    //Play the sound
    _gameScene->playInGameSound("CP_Pop");//plant_crystal_pop
}

void Plant_Crystal_Weed::onRemoveSmoke(CCNode* sender)
{
    removeChild(sender);
}


void Plant_Crystal_Weed::onRemoveThisPlant()
{
    _gameScene->removeNode(this);//Remove after anim ended
}
