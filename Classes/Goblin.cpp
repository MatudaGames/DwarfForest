//
//  Goblin.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 01/09/14.
//
//

#include "Goblin.h"

#include <SimpleAudioEngine.h>
#include "GameScene.h"
#include "AppMacros.h"
#include "SpriteAnimation.h"
#include "Utils.h"
#include "Wind.h"
#include "User.h"

USING_NS_CC;
using namespace CocosDenshion;

const float Goblin_SPEED = 30.0f;
const float RADIUS = 40.0f;
const uint8_t TOUCHABLE_OPACITY = 255;
const uint8_t UNTOUCHABLE_OPACITY = 128;

const float PICK_RADIUS = 50.0f;

const uint32_t TOUCHABLE_ZONE = 100;

Goblin* Goblin::create(GameScene* game)
{
    Goblin *pRet = new Goblin();
    if (pRet && pRet->init(game))
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

Goblin::Goblin():
_game(NULL), _animation(NULL), _leftUpAnimation(NULL), _leftDownAnimation(NULL),
_leftAnimation(NULL), _upAnimation(NULL), _downAnimation(NULL),_victory(NULL), _angle(0.0f),
_touchable(true),_removeSmoke(NULL),_crashAnimation(NULL)
{
	
}

Goblin::~Goblin()
{
	if (_game)
    {
        //        _game->playInGameSound("Goblin_exit");
        _game->release();
    }
	
	if (_leftUpAnimation) _leftUpAnimation->release();
	if (_leftDownAnimation) _leftDownAnimation->release();
	if (_leftAnimation) _leftAnimation->release();
	if (_upAnimation) _upAnimation->release();
	if (_downAnimation) _downAnimation->release();
    if (_victory) _victory->release();
    if (_removeSmoke) _removeSmoke->release();
    if (_crashAnimation) _crashAnimation->release();
    
}

bool Goblin::init(GameScene* game)
{
	if (!CCNode::init())
	{
		return false;
	}
    
//    mStartCatchDwarf = 0.0;
    
    mDwarfToCatch = NULL;
//    mCatchingDwarf = false;
    
    mPatrolPoints = false;
    mMoveToControl_1 = false;
    
    mTutorialGoblin = false;
    _touchable = false;
    
    if(User::getInstance()->_tutorial == 0){
        mTutorialGoblin = true;
    }
    
    mDrawLineID = -1;
    
    mGoblinStartY = 0;
    
    _waitForReturn = 3;
    
    mRunToExit = false;
    mCrystalCatch = NULL;
    
    mExit_X = 0;
    mExit_Y = 0;
    
    mTimesTaped = 0;//3 taps disables it
    
    mMoveByNodes = false;
    mMoveIndex = 0;
    mMoveClock = false;
    mMoveCurrentDir = 0;
    
    mTutorfix = false;
    
    blitzHit = false;
    
    _canMove = true;//wait a bit !!!s
    _isDisabled = false;
    _forceRemove = false;
	
	_game = game;
	_game->retain();
    
    mCatchRadar = NULL;
    
    //The overlay
//    ccBlendFunc* someBlend = new ccBlendFunc();
//    someBlend->src = GL_SRC_ALPHA;//OGLES.GL_ONE;
//    someBlend->dst = GL_ONE_MINUS_SRC_ALPHA;//OGLES.GL_ONE;
//    _leftUpAnimation->setBlendFunc(*someBlend);
	
	_leftUpAnimation = SpriteAnimation::create("Characters/troll/troll_diagonal_up.plist");
	_leftUpAnimation->retain();
    _leftUpAnimation->setColor(ccc3(64, 255, 0));
	
	_leftDownAnimation = SpriteAnimation::create("Characters/troll/troll_diagonal_down.plist");
	_leftDownAnimation->retain();
    _leftDownAnimation->setColor(ccc3(64, 255, 0));
	
	_leftAnimation = SpriteAnimation::create("Characters/troll/troll_profile.plist");
	_leftAnimation->retain();
    _leftAnimation->setColor(ccc3(64, 255, 0));
//    ccBlendFunc* someBlend = new ccBlendFunc();
//    someBlend->src = GL_SRC_ALPHA;//OGLES.GL_ONE;
//    someBlend->dst = GL_ONE_MINUS_SRC_ALPHA;//OGLES.GL_ONE;
//    _leftAnimation->setBlendFunc(*someBlend);
	
	_upAnimation = SpriteAnimation::create("Characters/troll/troll_up_back.plist");
	_upAnimation->retain();
    _upAnimation->setColor(ccc3(64, 255, 0));
	
	_downAnimation = SpriteAnimation::create("Characters/troll/troll_down_front.plist");
	_downAnimation->retain();
    _downAnimation->setColor(ccc3(64, 255, 0));
    
    _victory = SpriteAnimation::create("Characters/troll/troll_victory_dance.plist");
    _victory->retain();
    _victory->setColor(ccc3(64, 255, 0));
    
    _removeSmoke = SpriteAnimation::create("effects/virpulis.plist");
    _removeSmoke->retain();
    
    _crashAnimation = SpriteAnimation::create("effects/crash_stars.plist");
    _crashAnimation->retain();
//    _crashAnimation->setTag(111);
    _crashAnimation->pause();
    _crashAnimation->setVisible(false);
//    _crashAnimation->resume();
    addChild(_crashAnimation);
    
    mHeadIcon = CCSprite::create("Crystals/blue_crystal.png");
    mHeadIcon->setPosition(ccp(0,50));
    mHeadIcon->setScale(0.75);
    mHeadIcon->setVisible(false);
    mHeadIcon->setOpacity(128);
    addChild(mHeadIcon);
	
	_angle = 6.0f * M_PI / 8.0f;
	_speed = Goblin_SPEED;
    
    mGoblinScale = 0.5;
    
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
        _leftUpAnimation->setScale(mGoblinScale);
        _leftDownAnimation->setScale(mGoblinScale);
        _leftAnimation->setScale(mGoblinScale);
        _upAnimation->setScale(mGoblinScale);
        _downAnimation->setScale(mGoblinScale);
        _victory->setScale(mGoblinScale);
        
        _removeSmoke->setScale(mGoblinScale);
    }
	
    _game->playInGameSound("Troll_intro");
	
	return true;
}

void Goblin::setAnimationVisibility(bool theValue)
{
    if (_animation)
        _animation->setVisible(theValue);
}

void Goblin::setEffect(Effect* value)
{
    
}

void Goblin::removeEffect()
{
    
}

void Goblin::HitGoblin(bool theRemove)
{
    _canMove = false;
    
    mHeadIcon->setVisible(false);
    
    if(mCrystalCatch!=NULL){
        //Drop
        if(mRunToExit)
        {
            mCrystalCatch->setPosition(getPosition());
            mCrystalCatch->setVisible(true);//Hide it for now !!!
            
            CCPoint aDropPlace;
            std::vector<int> _dropSpace;
            
            //Move crystal to other space !!!
            if(mCrystalCatch->getPositionX()<_game->_screenSize.width/2){
                if(mCrystalCatch->getPositionY()<_game->_screenSize.height/2){
                    _dropSpace.push_back(0);
                    _dropSpace.push_back(1);
                    _dropSpace.push_back(3);
                }
                else{
                    _dropSpace.push_back(2);
                    _dropSpace.push_back(1);
                    _dropSpace.push_back(3);
                }
            }
            else{
                if(mCrystalCatch->getPositionY()<_game->_screenSize.height/2){
                    _dropSpace.push_back(0);
                    _dropSpace.push_back(1);
                    _dropSpace.push_back(2);
                }
                else{
                    _dropSpace.push_back(0);
                    _dropSpace.push_back(2);
                    _dropSpace.push_back(3);
                }
            }
            
            //Choose some random stuff
            int aRandomID = rand()%_dropSpace.size();
            int aFinalSpot = _dropSpace[aRandomID];
            _dropSpace.clear();
            
            if(aFinalSpot == 0){
                aDropPlace = ccp(_game->_screenSize.width/4,_game->_screenSize.height - _game->_screenSize.height/4);
            }
            else if(aFinalSpot == 1){
                aDropPlace = ccp(_game->_screenSize.width - _game->_screenSize.width/4,_game->_screenSize.height - _game->_screenSize.height/4);
            }
            else if(aFinalSpot == 2){
                aDropPlace = ccp(_game->_screenSize.width/4,_game->_screenSize.height/4);
            }
            else if(aFinalSpot == 3){
                aDropPlace = ccp(_game->_screenSize.width - _game->_screenSize.width/4,_game->_screenSize.height/4);
            }
            
            CCMoveTo* aMove1 = CCMoveTo::create(0.5,aDropPlace);
            CCEaseOut* aEase1 = CCEaseOut::create(aMove1,0.5);
            mCrystalCatch->runAction(aEase1);
        }
        else
        {
            mCrystalCatch->setVisible(true);//Hide it for now !!!
        }
        
        mCrystalCatch->mStealer = NULL;
        mCrystalCatch = NULL;
    }
    
    mRunToExit = false;
    
    if(theRemove){
        setVisible(false);
    }
    else{
        mTimeDown = 3;
        _crashAnimation->resume();
        _crashAnimation->setVisible(true);
    }
}

void Goblin::update(float delta)
{
    if(mTimeDown>0){
        mTimeDown-=delta;
        if(mTimeDown<=0){
            //Allow to move again !!!
            _canMove = true;
        }
        
        return;
    }
    
    if(!_canMove){
        return;
    }
    

    
    //Hide the crash stuff
    if(_crashAnimation->isVisible())
    {
        _crashAnimation->pause();
        _crashAnimation->setVisible(false);
    }
    
    //Lets move the goblin
    //Try to get it
    float x = getPositionX();
    float y = getPositionY();
    
    _touchable = (x > TOUCHABLE_ZONE &&
				  y > TOUCHABLE_ZONE &&
				  x < designResolutionSize.width - TOUCHABLE_ZONE &&
				  y < designResolutionSize.height - TOUCHABLE_ZONE);
    
    if(mRunToExit)
    {
        //Totaly different logic here !!!
        CCPoint point = ccp(mExit_X,mExit_Y);
        
        float theDistance = ccpDistanceSQ(point, getPosition());
        
        if (theDistance>100){
            setAngle(atan2f(point.y - y, point.x - x));
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()) * User::getInstance()->mDebugSpeed_troll,
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed())  * User::getInstance()->mDebugSpeed_troll);
            
            cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
        }
        else if(theDistance<=100)
        {
            //He got it !!!
            //            CancelDwarfCatch(mDwarfToCatch);
//            mCrystalCatch->setVisible(false);//Hide it for now !!!
            //Remove goblin and crystal !!!
            
            setVisible(false);
            
//            _goblins
        }

        
        return;
    }
    
    if(mCrystalCatch!=NULL)
    {
        //Totaly different logic here !!!
        CCPoint point = mCrystalCatch->getPosition();
        
        float theDistance = ccpDistanceSQ(point, getPosition());
        
        if (theDistance>2000){
            setAngle(atan2f(point.y - y, point.x - x));
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()) * User::getInstance()->mDebugSpeed_troll,
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed())  * User::getInstance()->mDebugSpeed_troll);
            
            cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
        }
        else if(theDistance<=2000)
        {
            //He got it !!!
            mHeadIcon->setVisible(true);
//            CancelDwarfCatch(mDwarfToCatch);
            mCrystalCatch->setVisible(false);//Hide it for now !!!
            
            mRunToExit = true;
            
            //Find some exit spot !!!
            //Choose some random exit spot !!!
//            int aRandomSpot = rand()%_game->_genearetPoints.size();
//            GeneratePoint generatePoint = _game->_genearetPoints[aRandomSpot];
            
            GeneratePoint generatePoint;
            int aRandomSpotN = rand()%8;
            if(aRandomSpotN == 0){
                generatePoint.x = 0;
                generatePoint.y = _game->_screenSize.height/4;
            }
            else if(aRandomSpotN == 1){
                generatePoint.x = 0;
                generatePoint.y = _game->_screenSize.height - _game->_screenSize.height/4;
            }
            else if(aRandomSpotN == 2){
                generatePoint.x = _game->_screenSize.width/4;
                generatePoint.y = _game->_screenSize.height;
            }
            else if(aRandomSpotN == 3){
                generatePoint.x = _game->_screenSize.width-_game->_screenSize.width/4;
                generatePoint.y = _game->_screenSize.height;
            }
            else if(aRandomSpotN == 4){
                generatePoint.x = _game->_screenSize.width;
                generatePoint.y = _game->_screenSize.height - _game->_screenSize.height/4;
            }
            else if(aRandomSpotN == 5){
                generatePoint.x = _game->_screenSize.width;
                generatePoint.y = _game->_screenSize.height/4;
            }
            else if(aRandomSpotN == 6){
                generatePoint.x = _game->_screenSize.width-_game->_screenSize.width/4;
                generatePoint.y = 0;
            }
            else {
                generatePoint.x = _game->_screenSize.width/4;
                generatePoint.y = 0;
            }
            
            mExit_Y = generatePoint.x;
            mExit_X = generatePoint.y;
//            CCLog("mExit_X:%i mExit_Y:%i",mExit_X,mExit_Y);
        }

    }
    
    return;
    
    
    
    /*
     if (!_canMove)
     {
     if(User::getInstance()->mNewMissionBuild)
     {
     if(getTag() == 999)
     {
     _waitForReturn-=delta*_game->getGameSpeed();
     if(_waitForReturn<=0){
     _waitForReturn = 3;
     
     setTag(0);
     _canMove = true;//Disable move for now
     
     //Set normal animation back
     setAngle(_lastSetAngle);
     }
     }
     }
     return;
     }
     
     if(_game->_boostNoEnemyTimer>0)
     return;
     
    if(mCatchingCrystal)
    {
        if(mStartCatchDwarf>0.0f){
     
            if(mCatchRadar->isVisible()){
                mCatchRadar->setVisible(false);
            }
            
            mStartCatchDwarf-=delta;
            
            if(mWarnIcon->isVisible()==false){
                mWarnIcon->setVisible(true);
            }
            return;//Goblin waits and then attacks
        }
        
        if(mWarnIcon->isVisible()==true){
            mWarnIcon->setVisible(false);
        }
        
        if(mDwarfToCatch == NULL){
            mCatchingCrystal = false;
            return;
        }
        
        if(mCatchRadar->isVisible()){
            mCatchRadar->setVisible(false);
        }
        
        float x = getPositionX();
        float y = getPositionY();
        
        //Totaly different logic here !!!
        CCPoint point = mDwarfToCatch->getPosition();
        
        float theDistance = ccpDistanceSQ(point, getPosition());
        
        if (theDistance<20000){
            setAngle(atan2f(point.y - y, point.x - x));
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()) * User::getInstance()->mDebugSpeed_Goblin,
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed())  * User::getInstance()->mDebugSpeed_Goblin);
            
            cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
        }
        else if(theDistance>=20000)
        {
            //He lost it
            CancelDwarfCatch(mDwarfToCatch);
        }
        
        return;
    }
    
    if(User::getInstance()->mDynamicGoblins){
        
        if(!mCatchRadar->isVisible()){
            if(getPositionY()>=mGoblinStartY-15 && getPositionY()<=mGoblinStartY+15){
                mCatchRadar->setVisible(true);
            }
        }
    }
    
    if(_game->mTutorialEnabled)
    {
        if(mTutorfix==false && _game->_gamePause==false)
        {
            mTutorfix = true;
            //            _game->pauseSchedulerAndActionsRecursive(this,true);
            _game->resumeSchedulerAndActionsRecursive(this);
        }
    }
    
    if(User::getInstance()->mSpecial_17_Mission){
        _timeInGame+=delta;
        if(_timeInGame>60){
            removeFromSave();
            _canMove = false;
            return;
        }
    }
    
    
    if(User::getInstance()->mSpecial_12_Mission && mMoveByNodes)
    {
        if (_game->getSpriteOrderZ(getPositionY())!=getZOrder())
        {
            //        CCLog("Changed Z order of wind");
            _game->reorderChild(this,_game->getSpriteOrderZ(getPositionY()));
        }
        
        if(mMoveByNodes)
        {
            float x = getPositionX();
            float y = getPositionY();
            
            CCPoint point = _game->_movePointsWind->getControlPointAtIndex(mMoveIndex);
            
            if (ccpDistanceSQ(point, getPosition()) <= 1000)
            {
                if(mMoveClock)
                {
                    mMoveIndex--;
                    if(mMoveIndex<0)
                        mMoveIndex = _game->_movePointsWind->count();//Start from 0
                }
                else
                {
                    mMoveIndex++;
                    if(mMoveIndex>_game->_movePointsWind->count())
                        mMoveIndex = 0;//Start from 0
                }
            }
            else
            {
                //                _angle = wrapTwoPI(atan2f(point.y - y, point.x - x));
                setAngle(atan2f(point.y - y, point.x - x));
            }
            
            //            setAngle(atan2f(point.y - y, point.x - x));
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()),
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed()));
            
            //            setPosition(newPosition);
            cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
        }
        
        return;
    }
    
    if(User::getInstance()->mSpecial_13_Mission || User::getInstance()->mSpecial_14_Mission)
    {
        if (_game->getSpriteOrderZ(getPositionY())!=getZOrder())
        {
            _game->reorderChild(this,_game->getSpriteOrderZ(getPositionY()));
        }
        
        //Just up and down movement !!!
        
        float x = getPositionX();
        float y = getPositionY();
        
        x += cosf(_angle) * delta * (_speed * _game->getGameSpeed());
        y += sinf(_angle) * delta * (_speed * _game->getGameSpeed());
        
        cocos2d::CCNode::setPosition(x, y);
        
        _touchable = (x > TOUCHABLE_ZONE &&
                      y > TOUCHABLE_ZONE &&
                      x < designResolutionSize.width - TOUCHABLE_ZONE &&
                      y < designResolutionSize.height - TOUCHABLE_ZONE);
        
        if(!_touchable)
        {
            if(_angle <= 2){
                setAngle(M_PI);
                //                    _angle = 4.7;
            }
            else{
                //                    _angle = 1.6;
                setAngle(0);
            }
        }
        
        return;
    }
    
    if((User::getInstance()->mSpecial_10_Mission || User::getInstance()->mNewMissionBuild) && !mTutorialGoblin)
    {
        if (_game->getSpriteOrderZ(getPositionY())!=getZOrder())
        {
            //        CCLog("Changed Z order of wind");
            _game->reorderChild(this,_game->getSpriteOrderZ(getPositionY()));
        }
        
        if(mMoveByNodes)
        {
            float x = getPositionX();
            float y = getPositionY();
            
            CCPoint point = _game->_movePointsWind->getControlPointAtIndex(mMoveIndex);
            
            if (ccpDistanceSQ(point, getPosition()) <= 1000)
            {
                if(mMoveClock)
                {
                    mMoveIndex--;
                    if(mMoveIndex<0)
                        mMoveIndex = _game->_movePointsWind->count();//Start from 0
                }
                else
                {
                    mMoveIndex++;
                    if(mMoveIndex>_game->_movePointsWind->count())
                        mMoveIndex = 0;//Start from 0
                }
            }
            else
            {
                //                _angle = wrapTwoPI(atan2f(point.y - y, point.x - x));
                setAngle(atan2f(point.y - y, point.x - x));
            }
            
            //            setAngle(atan2f(point.y - y, point.x - x));
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()),
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed()));
            
            //            setPosition(newPosition);
            cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
        }
        else if(mPatrolPoints)
        {
            float x = getPositionX();
            float y = getPositionY();
            
            CCPoint point = mGoblinPatrolPoint_1;
            if(mMoveToControl_1){
                point = mGoblinPatrolPoint_1;
            }
            else{
                point = mGoblinPatrolPoint_2;
            }
            
            if (ccpDistanceSQ(point, getPosition()) <= 1000)
            {
                mMoveToControl_1 = !mMoveToControl_1;
            }
            else
            {
                setAngle(atan2f(point.y - y, point.x - x));
            }
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()) * User::getInstance()->mDebugSpeed_Goblin,
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed()) * User::getInstance()->mDebugSpeed_Goblin);
            cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
        }
        else
        {
            //Just up and down movement !!!
            
            float x = getPositionX();
            float y = getPositionY();
            
            x += cosf(_angle) * delta * (_speed * _game->getGameSpeed());
            y += sinf(_angle) * delta * (_speed * _game->getGameSpeed());
            
            cocos2d::CCNode::setPosition(x, y);
            
            _touchable = (x > TOUCHABLE_ZONE &&
                          y > TOUCHABLE_ZONE &&
                          x < designResolutionSize.width - TOUCHABLE_ZONE &&
                          y < designResolutionSize.height - TOUCHABLE_ZONE);
            
            if(!_touchable)
            {
                if(_angle <= 2){
                    setAngle(4.7);
                    //                    _angle = 4.7;
                }
                else{
                    //                    _angle = 1.6;
                    setAngle(1.6);
                }
            }
        }
        
        return;
    }
    
	float x = getPositionX();
	float y = getPositionY();
	
	x += cosf(_angle) * delta * (_speed * _game->getGameSpeed());
	y += sinf(_angle) * delta * (_speed * _game->getGameSpeed());
	
	cocos2d::CCNode::setPosition(x, y);
	
	_touchable = (x > TOUCHABLE_ZONE &&
				  y > TOUCHABLE_ZONE &&
				  x < designResolutionSize.width - TOUCHABLE_ZONE &&
				  y < designResolutionSize.height - TOUCHABLE_ZONE);
    
    if (!_touchable && !_isDisabled)
    {
        _isDisabled = true;
        _animation->runAction(cocos2d::CCFadeTo::create(0.25, UNTOUCHABLE_OPACITY));
    }
    */
    
    //For now disabled as it's rare to get in to Goblin action
    //    //The magic Goblin check tornado :)
    //    CCObject* entry;
    //    CCARRAY_FOREACH(_game->getActiveTornado(), entry)
    //    {
    //        Wind* otherDwarf = (Wind*)entry;
    //
    //        //The danger !!!
    //        //Check for crash now !!!
    //        if (ccpDistanceSQ(otherDwarf->getPosition(), getPosition())<= powf(30, 2)*GLOBAL_SCALE)
    //        {
    //            setVictory();
    //
    //            _game->stopInGameSound("dwarf_walk");
    //            _game->stopInGameSound("Goblin_walk");
    //
    //            _game->stopInGameSound("dwarf_web_stuck",true);
    //
    ////            dwarf->createGoblinCrash();
    //
    //            //------------------------
    //
    //            setTag(999);
    ////            dwarf->setTag(999);
    //
    //            _game->menuSaveMeCallBack(NULL,NULL,this);
    //
    //            //------------------------
    //            break;
    //        }
    //    }
    
    
    //	if (_animation)
    //        _animation->setOpacity(_touchable?TOUCHABLE_OPACITY:UNTOUCHABLE_OPACITY);
}

void Goblin::CatchDwarf(Dwarf* theDwarf)
{
    //set 0.5sec wtf ???
//    mStartCatchDwarf = 0.5f;
//    mDwarfToCatch = theDwarf;
//    mCatchingDwarf = true;
}

void Goblin::CancelDwarfCatch(Dwarf* theCancelDwarf)
{
//    if(mCatchingDwarf){
//        if(mDwarfToCatch == theCancelDwarf){
//            mCatchingDwarf = false;
//            mDwarfToCatch = NULL;
//            
//            //            if(!mCatchRadar->isVisible()){
//            //                mCatchRadar->setVisible(true);
//            //            }
//        }
//    }
}

void Goblin::setFadeIn()
{
    if(_game->mTutorialEnabled)
    {
        _animation->setOpacity(255);
    }
    else
    {
        _animation->setOpacity(0);
        CCActionInterval* fade = CCFadeIn::create(0.25f);
        _animation->runAction(fade);
    }
}

void Goblin::setVictory()
{
    //Remove it !!!
    mDwarfToCatch = NULL;
    
    setAnimation(_victory);
    _canMove = false;//Disable move for now
    
    _game->playInGameSound("Goblin_win");
}

void Goblin::SetDrawLine(int theLine)
{
    if(theLine == 1){
        mDrawLine = CCSprite::create("Goblin_Line_1.png");
    }
    else if(theLine == 2){
        mDrawLine = CCSprite::create("Goblin_Line_1.png");
    }
    else if(theLine == 3){
        mDrawLine = CCSprite::create("Goblin_Line_2.png");
    }
    else if(theLine == 4){
        mDrawLine = CCSprite::create("Goblin_Line_2.png");
    }
    else if(theLine == 5){
        mDrawLine = CCSprite::create("Goblin_Line_3.png");
    }
    
    addChild(mDrawLine,0);
}

bool Goblin::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(User::getInstance()->mSpecial_16_Mission==false && User::getInstance()->mSpecial_17_Mission==false
       && User::getInstance()->mSpecial_18_Mission==false && User::getInstance()->mSpecial_19_Mission==false
       && User::getInstance()->mSpecial_20_Mission==false && User::getInstance()->mSpecial_21_Mission==false
       && User::getInstance()->mSpecial_22_Mission==false && User::getInstance()->mSpecial_23_Mission==false){
        return false;
    }
    
    if(_canMove == false){
        _touched = false;
        return false;
    }
    
    if(ccpDistanceSQ(touch->getLocation(), getPosition()) <= PICK_RADIUS * PICK_RADIUS){
        _touched = true;
        return true;
    }
    
    _touched = false;
    return false;
}

void Goblin::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    //Do nothing for now !!!
    
}

void Goblin::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(_touched){
        //Count taps !!!
        mTimesTaped+=1;
        if(mTimesTaped>=3){
            mTimesTaped = 0;
            
            HitGoblin(false);
//            _canMove = false;
//            mTimeDown = 2.0f;
            
        }
        
        //Check if can blitz it !!!
        if(_game->mZipZapActive){
//            _canMove = false;
            HitGoblin(true);
            _game->CreateBlitz_Goblin(getPositionX(),getPositionY()-80,this);
        }
    }
    _touched = false;
}

void Goblin::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void Goblin::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}


void Goblin::removeFromSave()
{
    setAnimation(_removeSmoke);
    //Add timer to remove from game after 2sec
    CCActionInterval* aRemoveDelay = CCDelayTime::create(0.5f);
    CCCallFunc* aRemoveCall = CCCallFuncN::create(this, callfuncN_selector(Goblin::setForceRemove));
    CCSequence* aRemoveGoblinSeq = CCSequence::create(aRemoveDelay,aRemoveCall,NULL);
    
    runAction(aRemoveGoblinSeq);
}

void Goblin::setForceRemove()
{
    _forceRemove = true;
}

float Goblin::GetAngle()
{
    return _angle;
}

void Goblin::setAngle(float value)
{
    _lastSetAngle = value;
    
	_angle = wrapTwoPI(value);
	
	if (_angle >= 15.0f * M_PI / 8.0f || _angle < M_PI / 8.0f)
	{
		//right
		_leftAnimation->setFlipX(true);
		_leftAnimation->setFlipY(false);
		setAnimation(_leftAnimation);
	}
	else if (_angle < 3.0f * M_PI / 8.0f)
	{
		//right up
		_leftUpAnimation->setFlipX(true);
		_leftUpAnimation->setFlipY(false);
		setAnimation(_leftUpAnimation);
	}
	else if (_angle < 5.0f * M_PI / 8.0f)
	{
		//up
		setAnimation(_upAnimation);
	}
	else if (_angle < 7.0f * M_PI / 8.0f)
	{
		//left up
		_leftUpAnimation->setFlipX(false);
		_leftUpAnimation->setFlipY(false);
		setAnimation(_leftUpAnimation);
	}
	else if (_angle < 9.0f * M_PI / 8.0f)
	{
		//left
		_leftAnimation->setFlipX(false);
		_leftAnimation->setFlipY(false);
		setAnimation(_leftAnimation);
	}
	else if (_angle < 11.0f * M_PI / 8.0f)
	{
		//left down
		_leftDownAnimation->setFlipX(false);
		_leftDownAnimation->setFlipY(false);
		setAnimation(_leftDownAnimation);
	}
	else if (_angle < 13.0f * M_PI / 8.0f)
	{
		//down
		setAnimation(_downAnimation);
	}
	else
	{
		//right down
		_leftDownAnimation->setFlipX(true);
		_leftDownAnimation->setFlipY(false);
		setAnimation(_leftDownAnimation);
	}
}

void Goblin::setAnimation(SpriteAnimation* animation)
{
	if (_animation != animation)
	{
		if (_animation)
		{
			removeChild(_animation);
		}
		
		_animation = animation;
		
		if (_animation)
		{
			addChild(_animation);
		}
	}
}

bool Goblin::getTouchable()
{
	return _touchable;
}
