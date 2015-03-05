//
//  UniversalPowerItem.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 02/03/15.
//
//

#include "Universal_PowerItem.h"

#include "GameScene.h"
#include "Dwarf.h"
#include "SpriteAnimation.h"
#include "TimedSpriteAnimation.h"

#include "User.h"

USING_NS_CC;

Universal_PowerItem* Universal_PowerItem::create(GameScene* gameScene,int theID)
{
    Universal_PowerItem *pRet = new Universal_PowerItem();
    if (pRet && pRet->init(gameScene,theID))
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

Universal_PowerItem::Universal_PowerItem():
_LeafRight(NULL),_LeafLeft(NULL),_LeafCenter(NULL),_LeafBottom(NULL)
{
}

Universal_PowerItem::~Universal_PowerItem()
{
}

bool Universal_PowerItem::init(GameScene* gameScene,int theID)
{
    if (!CCNode::init())
    {
        return false;
    }
    
    mTouched = false;
    
    _gameScene = gameScene;
    gameScene->retain();
    CCLog("Want to spawn power item with ID:%i",theID);
    // Check what to do with this power item !!!
    mItemInfo = User::getInstance()->getItemDataManager().getPowerByID(theID);
    
    // What level is this item
    mLevel = User::getInstance()->getItemDataManager().getPowerItemLevel(theID);
    
    // Now do the magic if it's a plant or etc
    if(mItemInfo.id == ITEM_CRYSTAL_PLANT)
    {
        // Prepeare crystal plant stuff
        mCrystalPlant_SpawnCrystals = 1;
        mCrystalPlant_PlantsFallenDown = 0;
        
        // The function that will create all the needed stuff
        CreateCrystalPlant();
    }
    else if(mItemInfo.id == ITEM_GHOST)
    {
        CreateGhostMushroom();
    }
    else if(mItemInfo.id == ITEM_CRYSTAL_DOUBLER)
    {
        CreateCrystalDoublerMushroom();
    }
    
    return true;
}

//......................................................................
// The Crystal Point Dobler

void Universal_PowerItem::CreateCrystalDoublerMushroom()
{
    // Take time from mission current
    mMushroom_CrystalDuble_Base = CCSprite::create("powerup_mushroom/shroom_base.png");
    mMushroom_CrystalDuble_Over = CCSprite::create("InGameIcons/Crystal_Doubler_Shroom.png");
    
    mMushroom_CrystalDuble_Base->setOpacity(0);
    mMushroom_CrystalDuble_Over->setOpacity(0);
    
    addChild(mMushroom_CrystalDuble_Base);
    addChild(mMushroom_CrystalDuble_Over);
    
    if (GLOBAL_SCALE!=1.0f)
    {
        mMushroom_CrystalDuble_Base->setScale(GLOBAL_SCALE);
        mMushroom_CrystalDuble_Over->setScale(GLOBAL_SCALE);
    }
    
    mMushroom_CrystalDuble_Over->setAnchorPoint(ccp(0.5,0.0f));
    
    mMushroom_CrystalDuble_Base->setAnchorPoint(ccp(0.5,0.0f));
    mMushroom_CrystalDuble_Base->setScaleX(0);
    mMushroom_CrystalDuble_Base->setScaleY(0);
    
    CCScaleTo* aScaleIn = CCScaleTo::create(0.5f, GLOBAL_SCALE, GLOBAL_SCALE);
    CCActionInterval* aScaleAct = CCEaseElasticOut::create(aScaleIn,0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::MushroomCrystalDuble_onFinishedShowUp));
    
    CCFadeIn* aFadeIn = CCFadeIn::create(0.2f);
    CCSpawn* aSpawn = CCSpawn::create(aFadeIn,aScaleAct,NULL);
    
    CCSequence* aTotal = CCSequence::create(aSpawn,func,NULL);
    mMushroom_CrystalDuble_Base->runAction(aTotal);
    
    //10 sec to collect this bad boy
    schedule(schedule_selector(Universal_PowerItem::MushroomCrystalDuble_createRemove), 0.0f, 0, _gameScene->mCurrentMission.ItemTimeOnMap);
    
    mMushroomCrystalDuble_NeedToRemove = false;
}

void Universal_PowerItem::MushroomCrystalDuble_createRemove()
{
    unschedule(schedule_selector(Universal_PowerItem::MushroomCrystalDuble_createRemove));
    mMushroomCrystalDuble_NeedToRemove = true;
}

void Universal_PowerItem::MushroomCrystalDuble_onRemove()
{
    _gameScene->mUniversalItems->removeObject(this);
    _gameScene->removeNode(this);
}

void Universal_PowerItem::MushroomCrystalDuble_onFinishedShowUp()
{
    if (mMushroomCrystalDuble_NeedToRemove)
    {
        CCScaleTo* aScale = CCScaleTo::create(0.5f, 0);
        CCActionInterval* aScaleAct = CCEaseElasticIn::create(aScale,0.5f);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::MushroomCrystalDuble_onRemove));
        CCSequence* seqfin = CCSequence::create(aScaleAct,func,NULL);
        mMushroom_CrystalDuble_Base->runAction(seqfin);
        return;
    }
    
    CCFadeIn* a1_FadeIn = CCFadeIn::create(0.5f);
    mMushroom_CrystalDuble_Over->runAction(a1_FadeIn);
    
    CCFadeOut* a2_FadeOut = CCFadeOut::create(0.5f);
    CCDelayTime* a2_Delay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::MushroomCrystalDuble_onFinishedTrans));
    CCSequence* a2_Seq = CCSequence::create(a2_Delay,a2_FadeOut,func,NULL);
    mMushroom_CrystalDuble_Base->runAction(a2_Seq);
}

void Universal_PowerItem::MushroomCrystalDuble_onFinishedTrans()
{
    CCFadeIn* a1_FadeIn = CCFadeIn::create(0.5f);
    mMushroom_CrystalDuble_Base->runAction(a1_FadeIn);
    
    CCFadeOut* a2_FadeOut = CCFadeOut::create(0.5f);
    CCDelayTime* a2_Delay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::MushroomCrystalDuble_onFinishedShowUp));
    CCSequence* a2_Seq = CCSequence::create(a2_Delay,a2_FadeOut,func,NULL);
    mMushroom_CrystalDuble_Over->runAction(a2_Seq);
}


//......................................................................
// The Ghost Mushroom

void Universal_PowerItem::CreateGhostMushroom()
{
    // Take time from mission current
    mMushroom_Ghost_Base = CCSprite::create("powerup_mushroom/shroom_base.png");
    mMushroom_Ghost_Over = CCSprite::create("powerup_mushroom/shroom_over.png");
    
    mMushroom_Ghost_Base->setOpacity(0);
    mMushroom_Ghost_Over->setOpacity(0);
    
    addChild(mMushroom_Ghost_Base);
    addChild(mMushroom_Ghost_Over);
    
    if (GLOBAL_SCALE!=1.0f)
    {
        mMushroom_Ghost_Base->setScale(GLOBAL_SCALE);
        mMushroom_Ghost_Over->setScale(GLOBAL_SCALE);
    }
    
    mMushroom_Ghost_Over->setAnchorPoint(ccp(0.5,0.0f));
    
    mMushroom_Ghost_Base->setAnchorPoint(ccp(0.5,0.0f));
    mMushroom_Ghost_Base->setScaleX(0);
    mMushroom_Ghost_Base->setScaleY(0);
    
    CCScaleTo* aScaleIn = CCScaleTo::create(0.5f, GLOBAL_SCALE, GLOBAL_SCALE);
    CCActionInterval* aScaleAct = CCEaseElasticOut::create(aScaleIn,0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::MushroomGhost_onFinishedShowUp));
    
    CCFadeIn* aFadeIn = CCFadeIn::create(0.2f);
    CCSpawn* aSpawn = CCSpawn::create(aFadeIn,aScaleAct,NULL);
    
    CCSequence* aTotal = CCSequence::create(aSpawn,func,NULL);
    mMushroom_Ghost_Base->runAction(aTotal);
    
    //10 sec to collect this bad boy
    schedule(schedule_selector(Universal_PowerItem::MushroomGhost_createRemove), 0.0f, 0, _gameScene->mCurrentMission.ItemTimeOnMap);
    
    mMushroomGhost_NeedToRemove = false;
}

void Universal_PowerItem::MushroomGhost_createRemove()
{
    unschedule(schedule_selector(Universal_PowerItem::MushroomGhost_createRemove));
    mMushroomGhost_NeedToRemove = true;
}

void Universal_PowerItem::MushroomGhost_onRemove()
{
    _gameScene->mUniversalItems->removeObject(this);
    _gameScene->removeNode(this);
}

void Universal_PowerItem::MushroomGhost_onFinishedShowUp()
{
    if (mMushroomGhost_NeedToRemove)
    {
        CCScaleTo* aScale = CCScaleTo::create(0.5f, 0);
        CCActionInterval* aScaleAct = CCEaseElasticIn::create(aScale,0.5f);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::MushroomGhost_onRemove));
        CCSequence* seqfin = CCSequence::create(aScaleAct,func,NULL);
        mMushroom_Ghost_Base->runAction(seqfin);
        return;
    }
    
    CCFadeIn* a1_FadeIn = CCFadeIn::create(0.5f);
    mMushroom_Ghost_Over->runAction(a1_FadeIn);
    
    CCFadeOut* a2_FadeOut = CCFadeOut::create(0.5f);
    CCDelayTime* a2_Delay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::MushroomGhost_onFinishedTrans));
    CCSequence* a2_Seq = CCSequence::create(a2_Delay,a2_FadeOut,func,NULL);
    mMushroom_Ghost_Base->runAction(a2_Seq);
}

void Universal_PowerItem::MushroomGhost_onFinishedTrans()
{
    CCFadeIn* a1_FadeIn = CCFadeIn::create(0.5f);
    mMushroom_Ghost_Base->runAction(a1_FadeIn);
    
    CCFadeOut* a2_FadeOut = CCFadeOut::create(0.5f);
    CCDelayTime* a2_Delay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::MushroomGhost_onFinishedShowUp));
    CCSequence* a2_Seq = CCSequence::create(a2_Delay,a2_FadeOut,func,NULL);
    mMushroom_Ghost_Over->runAction(a2_Seq);
}


//......................................................................
// The Crystal Plant Stuff

void Universal_PowerItem::CreateCrystalPlant()
{
    // Some quick stuff
    int aDelay = 3;
    int aAnimFrameLimit = 20;
    int aPlants = 1;
    int aPakstID = 1;
    float aRealDelay = 4.5f;
    
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
    
    mCrystalPlant_SpawnCrystals = aPlants;
    
    char theFileName[64];
    sprintf(theFileName, "powerup/Crystal_weed/paaksts%i.png", aPakstID);
    
    TimedSpriteAnimation* _animation = TimedSpriteAnimation::create("powerup/Crystal_weed/crystalweed_augshana.plist",20,aAnimFrameLimit);
    addChild(_animation);
    
    //Play grow sound
    _gameScene->playInGameSound("CP_Kats");
    
    //When the crystal bombs show up?
    CCSprite* aBox1 = CCSprite::create(theFileName);
    aBox1->setTag(11);
    aBox1->setRotation(-150);
    aBox1->setAnchorPoint(ccp(0.5,0));
    aBox1->setScale(0.0f);
    aBox1->setPosition(ccp(-22,28));
    addChild(aBox1,1);
    
    CCDelayTime* aDelay_1a = CCDelayTime::create(1.4f);
    
    CCScaleTo* aScale_1a = CCScaleTo::create(0.75f,1.0f);
    CCActionInterval* aScale_1 = CCEaseElasticOut::create(aScale_1a,0.5f);
    
    CCDelayTime* aDelay_1b = CCDelayTime::create(2.0f);
    CCMoveTo* aMove_1 = CCMoveTo::create(0.1f,ccp(-20,-10));
    
    CCCallFuncN* aFunc1 = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::onPlantFallDown));
    
    //Some extra sound on grow complete - paksts scqueze out
    CCDelayTime* aDelaySoundS = CCDelayTime::create(1.4f);
    CCCallFuncN* aFuncSoundS = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::onGrowCompleted));
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
        
        CCCallFuncN* aFunc2 = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::onPlantFallDown));
        
        //Some extra sound on grow complete - paksts scqueze out
        aDelaySoundS = CCDelayTime::create(1.9f);
        aFuncSoundS = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::onGrowCompleted));
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
        
        CCCallFuncN* aFunc3 = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::onPlantFallDown));
        
        //Some extra sound on grow complete - paksts scqueze out
        aDelaySoundS = CCDelayTime::create(2.1f);
        aFuncSoundS = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::onGrowCompleted));
        aSeqSoundS = CCSequence::create(aDelaySoundS,aFuncSoundS,NULL);
        
        CCSequence* aSeq_3 = CCSequence::create(aDelay_3a,aScale_3,aDelay_3b,aMove_3,aFunc3,NULL);
        aSpawn_1 = CCSpawn::create(aSeqSoundS,aSeq_3,NULL);
        
        //        aBox3->runAction(aSeq_3);
        aBox3->runAction(aSpawn_1);
    }
    
    //Set fade out for the weed !!!
    CCDelayTime* aWeedDelay = CCDelayTime::create(aRealDelay);
    CCFadeOut* aFadeOut = CCFadeOut::create(0.5f);
    CCCallFuncN* aFunc3 = CCCallFuncN::create(this, callfuncN_selector(Universal_PowerItem::onRemoveThisItem));
    CCSequence* aSeqAll = CCSequence::create(aWeedDelay,aFadeOut,aFunc3,NULL);
    _animation->runAction(aSeqAll);
}

void Universal_PowerItem::onGrowCompleted()
{
    //Create the drop anim?
    _gameScene->playInGameSound("CP_Paksts");//plant_crystal_pop
}

void Universal_PowerItem::onPlantFallDown(CCNode* sender)
{
    //    explosion.plist;
    TimedSpriteAnimation* _animation = TimedSpriteAnimation::create("powerup/Crystal_weed/explosion.plist",10);
    _animation->setPosition(ccp(sender->getPositionX()-8,sender->getPositionY()-20));
    addChild(_animation);
    
    CCDelayTime* aRemoveMe = CCDelayTime::create(1.0f);
    CCCallFuncN* aFunc1 = CCCallFuncN::create(this,callfuncN_selector(Universal_PowerItem::onRemoveSmoke));
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
        if(mCrystalPlant_PlantsFallenDown==0)
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
        if(mCrystalPlant_PlantsFallenDown==0)
        {
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()+10,this->getPositionY()+sender->getPositionY()+14);
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()-10,this->getPositionY()+sender->getPositionY()+8);
        }
        else if(mCrystalPlant_PlantsFallenDown==1)
        {
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()+10,this->getPositionY()+sender->getPositionY()+14);
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX()-10,this->getPositionY()+sender->getPositionY()+8);
        }
        else
        {
            _gameScene->generateCrystalSpecial(this->getPositionX()+sender->getPositionX(),this->getPositionY()+sender->getPositionY()+10);
        }
    }
    
    mCrystalPlant_PlantsFallenDown++;
    
    //Play the sound
    _gameScene->playInGameSound("CP_Pop");//plant_crystal_pop
}

void Universal_PowerItem::onRemoveSmoke(CCNode* sender)
{
    removeChild(sender);
}

void Universal_PowerItem::onRemoveThisItem()
{
    _gameScene->removeNode(this);//Remove after anim ended
}

//......................................................................
// The universal stuff if needed

void Universal_PowerItem::onDwarfPickUp(Dwarf* theDwarWhoPicked)
{
    // Give some double crystals for limited time
//    CCLog("mItemInfo.upgrade_power[%i(mLevel)-1]:%i",mLevel,mItemInfo.upgrade_power[mLevel-1]);
    
    // Do the magic here for each power item [mushroom/ghost/doubler] you name it
    if(mItemInfo.id == ITEM_GHOST){
        
        _gameScene->playInGameSound("crystal_pick_up");
        
        User::getInstance()->getMissionManager().CheckSubMission(SUB_COLLECT_MUSHROOM,1);
        
        _gameScene->mTotalMushroom+=1;
        _gameScene->CheckMissionByValue(MissionType_Mushroom,_gameScene->mTotalMushroom);
        
        // Active the ghost stuff
        _gameScene->mPowerItem_GhostsActive = mItemInfo.upgrade_power[mLevel-1];//How long?
        
        // Some mission info update
        _gameScene->_mission_AP_GhostDwarfs += 1;
        _gameScene->CheckMissionByValue(MissionType_AP_GhostDwarfs,_gameScene->_mission_AP_GhostDwarfs);
    }
    else if(mItemInfo.id == ITEM_CRYSTAL_DOUBLER){
        
        _gameScene->playInGameSound("crystal_pick_up");
        
        _gameScene->mPowerItem_CrystalDoublerActive = mItemInfo.upgrade_power[mLevel-1];
        _gameScene->mPowerItem_CrystalDoublerValue = 2;
        
        // What more to add here?
    }
}



