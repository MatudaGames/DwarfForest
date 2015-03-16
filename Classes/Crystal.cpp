//
//  Chrystal.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 22.02.2013.
//
//

#include "Crystal.h"
#include "GameScene.h"
#include "Dwarf.h"
#include "SpriteAnimation.h"
#include "User.h"

USING_NS_CC;

//const unsigned int SMALL_CRYSTAL_SCORE = 10;
//const unsigned int MEDIUM_CRYSTAL_SCORE = 20;
//const unsigned int BIG_CRYSTAL_SCORE = 30;
//const unsigned int XXL_CRYSTAL_SCORE = 50;

#define SMALL_CRYSTAL_SCORE_DEFAULT 10
#define MEDIUM_CRYSTAL_SCORE_DEFAULT 20
#define BIG_CRYSTAL_SCORE_DEFAULT 30
#define XXL_CRYSTAL_SCORE_DEFAULT 50

Crystal* Crystal::create(GameScene* gameScene,int theCrystalID,int theTimeOnMap)
{
	Crystal *pRet = new Crystal();
    if (pRet && pRet->init(gameScene,theCrystalID,theTimeOnMap))
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

Crystal::Crystal():
	_crystalSprite(NULL),_animation(NULL),_crystalShadow(NULL)
{
	
}

Crystal::~Crystal()
{
	if (_animation) _animation->release();
}

bool Crystal::init(GameScene* gameScene,int theCrystalID,int theTimeOnMap)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    // Update crystal amount by game item stuff
    /* // OLD STUFF - NOW CRYSTAL VALUE CAN BE CHANGED - IGNORE THIS !!!
    if(gameScene->mPowerItem_CrystalRefiner>0)
    {
        // New values !!!
        SMALL_CRYSTAL_SCORE = SMALL_CRYSTAL_SCORE_DEFAULT+(SMALL_CRYSTAL_SCORE_DEFAULT*gameScene->mPowerItem_CrystalRefiner/100);
        MEDIUM_CRYSTAL_SCORE = MEDIUM_CRYSTAL_SCORE_DEFAULT+(MEDIUM_CRYSTAL_SCORE_DEFAULT*gameScene->mPowerItem_CrystalRefiner/100);
        BIG_CRYSTAL_SCORE = BIG_CRYSTAL_SCORE_DEFAULT+(BIG_CRYSTAL_SCORE_DEFAULT*gameScene->mPowerItem_CrystalRefiner/100);
        XXL_CRYSTAL_SCORE = XXL_CRYSTAL_SCORE_DEFAULT+(XXL_CRYSTAL_SCORE_DEFAULT*gameScene->mPowerItem_CrystalRefiner/100);
    }
    else
    {
        SMALL_CRYSTAL_SCORE = SMALL_CRYSTAL_SCORE_DEFAULT;
        MEDIUM_CRYSTAL_SCORE = MEDIUM_CRYSTAL_SCORE_DEFAULT;
        BIG_CRYSTAL_SCORE = BIG_CRYSTAL_SCORE_DEFAULT;
        XXL_CRYSTAL_SCORE = XXL_CRYSTAL_SCORE_DEFAULT;
    }
    */
	
	_color = static_cast<CrystalColor>(rand() % CRYSTAL_COLOR_COUNT);
    
    //Use some generate formula split !!!
    /*
    int aRandRes = rand()%10;
    
    //CHECK IF WANT TO SPAWN EGG !!!
    int aSpawnEgg = 0;
    if(User::getInstance()->isEasterActive() && gameScene->mTutorialEnabled==false)
    {
        aSpawnEgg = rand()%10;
    }
    
    mStealer = NULL;
    
    if(aSpawnEgg>8)
    {
        //Spawn the egg
        _color = CRYSTAL_EGG;
    }
    else
    {
        if (aRandRes<4)
            _color = CRYSTAL_COLOR_GREEN;
        else if (aRandRes<7)
            _color = CRYSTAL_COLOR_BLUE;
        else if (aRandRes<9)
            _color = CRYSTAL_COLOR_RED;
        else
            _color = CRYSTAL_COLOR_YELLOW;
    }
    */
    
    mStealer = NULL;
    
    if(theCrystalID == 0) _color = CRYSTAL_COLOR_GREEN;
    else if(theCrystalID == 1) _color = CRYSTAL_COLOR_BLUE;
    else if(theCrystalID == 2) _color = CRYSTAL_COLOR_RED;
    else if(theCrystalID == 3) _color = CRYSTAL_COLOR_YELLOW;
    
    _OnMapTime = theTimeOnMap;
    
	_gameScene = gameScene;
	
    if(_gameScene->mTutorialEnabled)
    {
        if(_gameScene->mTutorialStep == 11)
            _color = CRYSTAL_COLOR_BLUE;
    }
    
	switch (_color)
	{
        case CRYSTAL_EGG:
            _shadowSprite = CCSprite::create("trajectory_dot_white.png");
            _crystalSprite = CCSprite::create("Interfeiss/challenges/weekly/easter/egg_small.png");
            _crystalShadow = CCSprite::create("trajectory_dot_white.png");
            break;
		case CRYSTAL_COLOR_BLUE:
			_shadowSprite = CCSprite::create("Crystals/blue_under.png");
			_crystalSprite = CCSprite::create("Crystals/blue_crystal.png");
            _crystalShadow = CCSprite::create("Crystals/blue_glow.png");
			break;
		case CRYSTAL_COLOR_GREEN:
			_shadowSprite = CCSprite::create("Crystals/green_under.png");
			_crystalSprite = CCSprite::create("Crystals/green_crystal.png");
            _crystalShadow = CCSprite::create("Crystals/green_glow.png");
			break;
		case CRYSTAL_COLOR_RED:
			_shadowSprite = CCSprite::create("Crystals/red_under.png");
			_crystalSprite = CCSprite::create("Crystals/red_crystal.png");
            _crystalShadow = CCSprite::create("Crystals/red_glow.png");
			break;
		case CRYSTAL_COLOR_YELLOW:
		default:
			_shadowSprite = CCSprite::create("Crystals/yellow_under.png");
			_crystalSprite = CCSprite::create("Crystals/yellow_crystal.png");
            _crystalShadow = CCSprite::create("Crystals/yellow_glow.png");
			break;
	}
    
    //For debug
//    if(GLOBAL_SCALE!=1.0f)
//    {
//        _shadowSprite->setScale(GLOBAL_SCALE);
//        _crystalSprite->setScale(GLOBAL_SCALE);
//        _crystalShadow->setScale(GLOBAL_SCALE);
//    }
    
    mExitAnimSet = false;
    
    _shadowSprite->setScale(0.64f);
    if(_crystalSprite!=NULL)
        _crystalSprite->setScale(0.64f);
    
    if(_crystalShadow!=NULL)
    {
        _crystalShadow->setScale(0.64f);
        _crystalShadow->setOpacity(0);
    }
	
    addChild(_crystalShadow);
	addChild(_shadowSprite);
	addChild(_crystalSprite);
    
    _crystalShadow->setPosition(ccp(0,-7));
	
	_shadowSprite->setPositionY(-_crystalSprite->getContentSize().height/4.0f-6);
    
    _crystalSprite->setPositionY(-_crystalSprite->getContentSize().height/2.0f);
    _crystalSprite->setAnchorPoint(ccp(0.5f,0.0f));
    
//    //When we will create start???
//    schedule(schedule_selector(Crystal::createStar), 0.0f, 1, 2.0f);
//
//	
//    //First we need to reverse the fade out anim !!!
////    schedule(schedule_selector(Crystal::expireAnim), 0.0f, 1, 10.0f*_gameScene->gameSpeed);
//    schedule(schedule_selector(Crystal::UpdateCrystalTimer), 0.5f);
//    
//    //The glow anim
//    mSecondsActive = 0;
//    
//    CCFadeIn* aFadeIn = CCFadeIn::create(1.0f);
//    CCFadeOut* aFadeOut = CCFadeOut::create(1.0f);
//    CCSequence* aFadeIO = CCSequence::create(aFadeIn,aFadeOut,NULL);
//    CCRepeatForever* aRepeat = CCRepeatForever::create(aFadeIO);
//    _crystalShadow->runAction(aRepeat);
	
	return true;
}

CrystalColor Crystal::getColor() const
{
	return _color;
}

void Crystal::onFinishedIntro()
{
    if(_gameScene==NULL)
        return;//No need to do this
    //When we will create start???
    schedule(schedule_selector(Crystal::createStar), 0.0f, 1, 2.0f);
    
    //First we need to reverse the fade out anim !!!
    if(_gameScene->mTutorialEnabled)
    {
        //Do nothing for now !!!
    }
    else
    {
        schedule(schedule_selector(Crystal::updateCrystalTimer), 0.5f);
    }
    
    //The glow anim
    _secondsActive = 0;
    
    CCFadeIn* aFadeIn = CCFadeIn::create(1.0f);
    CCFadeOut* aFadeOut = CCFadeOut::create(1.0f);
    CCSequence* aFadeIO = CCSequence::create(aFadeIn,aFadeOut,NULL);
    CCRepeatForever* aRepeat = CCRepeatForever::create(aFadeIO);
    _crystalShadow->runAction(aRepeat);
    
    if(User::getInstance()->mSpecial_21_Mission){
        //Add this crytal for catch !!!
        _gameScene->GiveGoblinSnatch(this);
    }
}

void Crystal::updateCrystalTimer()
{
    if(isVisible()==false)
    {
        return;
    }
    
    _secondsActive += 0.5f*_gameScene->getGameSpeed();
    
//    CCLog("mSecondsActive %i",mSecondsActive);
    //If below 1 sec skip this step as it's not important
    if (_secondsActive>=_OnMapTime)
    {
        unschedule(schedule_selector(Crystal::updateCrystalTimer));
        expireAnim();//Finish it
    }
    
    if(_secondsActive>=(_OnMapTime-4) && !mExitAnimSet)
    {
        _crystalShadow->stopAllActions();
        
        //Move to base
//        CCFadeOut* aFadeOut = CCFadeOut::create(1.0f);
//        _crystalShadow->runAction(aFadeOut);
        
        mExitAnimSet = true;
        
        CCFadeTo* aFade1 = CCFadeTo::create(0.4f,128);
        CCFadeTo* aFade2 = CCFadeTo::create(0.4f,255);
        CCSequence* aFadeSeq1 = CCSequence::create(aFade1,aFade2,NULL);
        CCRepeatForever* aRep1 = CCRepeatForever::create(aFadeSeq1);
        _crystalSprite->runAction(aRep1);
    }
}

void Crystal::createStar()
{
    unschedule(schedule_selector(Crystal::createStar));
    
    if (_animation!=NULL)
        return;
    
    _animation = SpriteAnimation::create("Crystals/shiny_stuff.plist");
    _animation->retain();
    _animation->setTag(100);
    
    //For debug
//    if(GLOBAL_SCALE!=1.0f)
//    {
//        _animation->setScale(0.64f);
//    }
    
    _animation->setScale(0.64f);
    
    addChild(_animation);
    
//    schedule(schedule_selector(Crystal::removeStar), 0.0f, 1, 1.0f);
    
    this->schedule(schedule_selector(Crystal::removeStar), 1.15f);
//    schedule(schedule_selector(Crystal::removeStar), 0.0f, 1, 1.0f);
//    [self schedule:@selector(Crystal::removeStar) interval:1];
}

void Crystal::removeStar()
{
    float aX = rand()%int(_crystalSprite->getContentSize().width)-_crystalSprite->getContentSize().width/2;
    float aY = rand()%int(_crystalSprite->getContentSize().height)-_crystalSprite->getContentSize().height/2;
//    CCLog("Create Random");
    //Assign some random place again !!!
    _animation->setPosition(ccp(aX,aY));
    
//    _animation->stopAllActions();
//    removeChild(_animation);
//    _animation = NULL;
}

void Crystal::expireAnim()
{
    unschedule(schedule_selector(Crystal::expireAnim));
    
//    _animation->stopAllActions();
    //Quick remove the blink
    _crystalShadow->stopAllActions();
    
    //Remove the blink !!!
    
    CCFadeOut* aShadowFadeOut = CCFadeOut::create(0.25f);
    CCDelayTime* aShadowFadeDelay = CCDelayTime::create(0.8f);
    CCSequence* aShadowSeq = CCSequence::create(aShadowFadeDelay,aShadowFadeOut,NULL);
    
    _shadowSprite->runAction(aShadowSeq);
    
    CCScaleTo* scaleOut = CCScaleTo::create(1.0f, 0.0f);
    CCActionInterval* scaleCombo = CCEaseElasticIn::create(scaleOut, 0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Crystal::expire));
    
    CCSequence* aSeq = CCSequence::create(scaleCombo,func,NULL);
    
    _crystalSprite->runAction(aSeq);
}

//void Crystal::expire(float delta)
void Crystal::expire()
{
    if(mStealer!=NULL){
        if(mStealer->mCrystalCatch!=NULL){
            mStealer->mCrystalCatch = NULL;
        }
        mStealer = NULL;
    }
	_gameScene->removeCrystal(this);
}

void Crystal::pickUp(Dwarf* dwarf,int aMulti)
{
    CCParticleSystemQuad* p;
    
    //TODO add mPowerItem_CrystalDoublerValue ???
    // This is one old whacky functional :(
    int aScoreToAdd = 0;
    
    switch (_color)
	{
        case CRYSTAL_EGG:
            User::getInstance()->addEgg();
            //should show that picked up egg ??? some baner
//            _gameScene->createPoints(1, 0, dwarf->getPosition(),ccc3(0, 232, 225));
            
            p = CCParticleSystemQuad::create("Particles/KaboomFx.plist");
            p->setPosition(ccp(dwarf->getPositionX(), dwarf->getPositionY()+20));
            p->setAutoRemoveOnFinish(true);
            _gameScene->addChild(p,dwarf->getZOrder()+1);
            
            break;
		case CRYSTAL_COLOR_BLUE:
            aScoreToAdd = (CHARGE_CRYSTAL_BLUE+(CHARGE_CRYSTAL_BLUE*_gameScene->mPowerItem_CrystalRefiner/100))*_gameScene->mPowerItem_CrystalDoublerValue;
//            _gameScene->addDebugCrystalPoints(MEDIUM_CRYSTAL_SCORE+_gameScene->getBoostExtraPoints());
            _gameScene->addDebugCrystalPoints(aScoreToAdd);
            
//            _gameScene->createPoints(MEDIUM_CRYSTAL_SCORE*aMulti, _gameScene->getBoostExtraPoints(), dwarf->getPosition(),ccc3(0, 232, 225));
            _gameScene->createPoints(aScoreToAdd*aMulti, _gameScene->getBoostExtraPoints(), dwarf->getPosition(),ccc3(0, 232, 225));
			_gameScene->addPickedUpCrystall(_color); // avoiding strang bug, when _color out of bounds
            
//            if(User::getInstance()->mNewMissionBuild){
//                dwarf->mCrystalPoints+=MEDIUM_CRYSTAL_SCORE*aMulti;
//            }
			break;
		case CRYSTAL_COLOR_GREEN:
            aScoreToAdd = (CHARGE_CRYSTAL_GREEN+(CHARGE_CRYSTAL_GREEN*_gameScene->mPowerItem_CrystalRefiner/100))*_gameScene->mPowerItem_CrystalDoublerValue;
//            _gameScene->addDebugCrystalPoints(SMALL_CRYSTAL_SCORE+_gameScene->getBoostExtraPoints());
            
//            _gameScene->createPoints(SMALL_CRYSTAL_SCORE*aMulti, _gameScene->getBoostExtraPoints(), dwarf->getPosition(),ccc3(0, 232, 225));
            _gameScene->createPoints(aScoreToAdd, _gameScene->getBoostExtraPoints(), dwarf->getPosition(),ccc3(0, 232, 225));
			_gameScene->addPickedUpCrystall(_color); // avoiding strang bug, when _color out of bounds
            
//            if(User::getInstance()->mNewMissionBuild){
//                dwarf->mCrystalPoints+=SMALL_CRYSTAL_SCORE*aMulti;
//            }
			break;
		case CRYSTAL_COLOR_RED:
            aScoreToAdd = (CHARGE_CRYSTAL_RED+(CHARGE_CRYSTAL_RED*_gameScene->mPowerItem_CrystalRefiner/100))*_gameScene->mPowerItem_CrystalDoublerValue;
//            _gameScene->addDebugCrystalPoints(BIG_CRYSTAL_SCORE+_gameScene->getBoostExtraPoints());
            
//            _gameScene->createPoints(BIG_CRYSTAL_SCORE*aMulti, _gameScene->getBoostExtraPoints(), dwarf->getPosition(),ccc3(0, 232, 225));
            _gameScene->createPoints(aScoreToAdd, _gameScene->getBoostExtraPoints(), dwarf->getPosition(),ccc3(0, 232, 225));
			_gameScene->addPickedUpCrystall(_color); // avoiding strang bug, when _color out of bounds
            
//            if(User::getInstance()->mNewMissionBuild){
//                dwarf->mCrystalPoints+=BIG_CRYSTAL_SCORE*aMulti;
//            }
			break;
		case CRYSTAL_COLOR_YELLOW:
            aScoreToAdd = (CHARGE_CRYSTAL_YELLOW+(CHARGE_CRYSTAL_YELLOW*_gameScene->mPowerItem_CrystalRefiner/100))*_gameScene->mPowerItem_CrystalDoublerValue;
//            _gameScene->addDebugCrystalPoints(XXL_CRYSTAL_SCORE+_gameScene->getBoostExtraPoints());
            
//            _gameScene->createPoints(XXL_CRYSTAL_SCORE*aMulti, _gameScene->getBoostExtraPoints(), dwarf->getPosition(),ccc3(0, 232, 225));
            _gameScene->createPoints(aScoreToAdd, _gameScene->getBoostExtraPoints(), dwarf->getPosition(),ccc3(0, 232, 225));
			_gameScene->addPickedUpCrystall(_color); // avoiding strang bug, when _color out of bounds
            
//            if(User::getInstance()->mNewMissionBuild){
//                dwarf->mCrystalPoints+=XXL_CRYSTAL_SCORE*aMulti;
//            }
			break;
		default:
			break;
	}
}
