//
//  Troll.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 22.02.2013.
//
//

#include <SimpleAudioEngine.h>
#include "Troll.h"
#include "GameScene.h"
#include "AppMacros.h"
#include "SpriteAnimation.h"
#include "Utils.h"
#include "Wind.h"
#include "User.h"

USING_NS_CC;
using namespace CocosDenshion;

const float TROLL_SPEED = 30.0f;
const float RADIUS = 40.0f;
const uint8_t TOUCHABLE_OPACITY = 255;
const uint8_t UNTOUCHABLE_OPACITY = 128;

const float PICK_RADIUS = 50.0f;

const uint32_t TOUCHABLE_ZONE = 100;



Troll* Troll::create(GameScene* game)
{
    Troll *pRet = new Troll();
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

Troll::Troll():
	_game(NULL), _animation(NULL), _leftUpAnimation(NULL), _leftDownAnimation(NULL),
	_leftAnimation(NULL), _upAnimation(NULL), _downAnimation(NULL),_victory(NULL), _angle(0.0f),
	_touchable(true),_removeSmoke(NULL)
{
	
}

Troll::~Troll()
{
	if (_game)
    {
//        _game->playInGameSound("troll_exit");
        _game->release();
    }
	
	if (_leftUpAnimation) _leftUpAnimation->release();
	if (_leftDownAnimation) _leftDownAnimation->release();
	if (_leftAnimation) _leftAnimation->release();
	if (_upAnimation) _upAnimation->release();
	if (_downAnimation) _downAnimation->release();
    if (_victory) _victory->release();
    if (_removeSmoke) _removeSmoke->release();
}

bool Troll::init(GameScene* game)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    
    mFreezedTime = 0;
    
    mStartCatchDwarf = 0.0;
    
    mDwarfToCatch = NULL;
    mCatchingDwarf = false;
    
    mPatrolPoints = false;
    mMoveToControl_1 = false;
    
    mTutorialTroll = false;
    
    if(User::getInstance()->_tutorial == 0){
        mTutorialTroll = true;
    }
    
    mDrawLineID = -1;
    
    mTrollStartY = 0;
    
    _waitForReturn = 3;
    
    mMoveByNodes = false;
    mMoveIndex = 0;
    mMoveClock = false;
    mMoveCurrentDir = 0;
    
    mTutorfix = false;
    
    blitzHit = false;
    
    _canMove = false;//wait a bit !!!s
    _isDisabled = false;
    _forceRemove = false;
	
	_game = game;
	_game->retain();
    
    mCatchRadar = NULL;
    if(User::getInstance()->mDynamicTrolls)
    {
        mCatchRadar = CCSprite::create("troll_sensor.png");
        if(User::getInstance()->mNewSplitCaves)
        {
            mCatchRadar->setAnchorPoint(ccp(0.5,0.5));
            mCatchRadar->setRotation(90);
            mCatchRadar->setOpacity(64);
        }
        else
        {
            mCatchRadar->setAnchorPoint(ccp(0,0.5));
            mCatchRadar->setOpacity(64);
        }
        addChild(mCatchRadar,0);
        
        mWarnIcon = CCSprite::create("troll_warn.png");
        mWarnIcon->setAnchorPoint(ccp(0,0.5));
        mWarnIcon->setPosition(ccp(0,40));
        mWarnIcon->setVisible(false);
        addChild(mWarnIcon,100);
        
    }
	
	_leftUpAnimation = SpriteAnimation::create("Characters/troll/troll_diagonal_up.plist");
	_leftUpAnimation->retain();
	
	_leftDownAnimation = SpriteAnimation::create("Characters/troll/troll_diagonal_down.plist");
	_leftDownAnimation->retain();
	
	_leftAnimation = SpriteAnimation::create("Characters/troll/troll_profile.plist");
	_leftAnimation->retain();
	
	_upAnimation = SpriteAnimation::create("Characters/troll/troll_up_back.plist");
	_upAnimation->retain();
	
	_downAnimation = SpriteAnimation::create("Characters/troll/troll_down_front.plist");
	_downAnimation->retain();
    
    _victory = SpriteAnimation::create("Characters/troll/troll_victory_dance.plist");
    _victory->retain();
    
    _removeSmoke = SpriteAnimation::create("effects/virpulis.plist");
    _removeSmoke->retain();
	
	_angle = 6.0f * M_PI / 8.0f;
//	_speed = TROLL_SPEED;
    
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
        _leftUpAnimation->setScale(GLOBAL_SCALE);
        _leftDownAnimation->setScale(GLOBAL_SCALE);
        _leftAnimation->setScale(GLOBAL_SCALE);
        _upAnimation->setScale(GLOBAL_SCALE);
        _downAnimation->setScale(GLOBAL_SCALE);
        _victory->setScale(GLOBAL_SCALE);
        
        _removeSmoke->setScale(GLOBAL_SCALE);
    }
	
    _game->playInGameSound("troll_intro");
    
    //------------------
    _movePoints = CCPointArray::create(0);
    _movePoints->retain();
    
    _moveInCircle = false;
	
	return true;
}

void Troll::setAnimationVisibility(bool theValue)
{
    if (_animation)
        _animation->setVisible(theValue);
}

void Troll::setEffect(Effect* value)
{
//	if (_effect)
//	{
//		_effect->release();
//		removeChild(_effect);
//	}
//
//	_effect = value;
//
//	if (_effect)
//	{
//		removeMovePoints();
		
//		_effect->retain();
    
//        _game->stopInGameSound("dwarf_walk");
//		
//		switch (_effect->getType())
//		{
//			case EFFECT_TYPE_ICE_BARRAGE:
//                _game->playInGameSound("dwarf_freeze");
//				break;
//			case EFFECT_TYPE_WEB:
//                _game->playInGameSound("dwarf_web_enter");
//				break;
//			default:
//				break;
//		}
		
//		_effect->setPosition(0.0f, 0.0f);
//		addChild(_effect);
//	}
}

void Troll::removeEffect()
{
    //	unschedule(schedule_selector(Dwarf::removeWeb));
	
//	if (_effect)
//	{
//        bool aNeedToResume = true;
//        if (_effect->getType()==EFFECT_TYPE_w)
//            aNeedToResume = false;
//        
//		_effect->release();
//		removeChild(_effect);
//		_effect = NULL;
    
		//set beck the animation
//        if (aNeedToResume)
//		{
//            resumeAnimation();
//		}
//        else
//		{
//            setAngle(_angle);
//		}
        
//        if(!_game->mTutorialEnabled)
//            _game->playInGameSound("dwarf_walk");
//	}
}

void Troll::update(float delta)
{
    //Order the troll to sit cool in map
    if (_game->getSpriteOrderZ(getPositionY())!=getZOrder()){
        _game->reorderChild(this,_game->getSpriteOrderZ(getPositionY()));
    }
    
    // New checks if did not happen to our hereo
    
    // Can't move and do stuff - dwarfs only hit back
    if(mFreezedTime>0){
        
        if(_animation->numberOfRunningActions()>0){
            _animation->pauseSchedulerAndActions();
        }
        
        // Runs timer while ice melts down
        mFreezedTime -= delta;
        
        if(mFreezedTime<1){
            //We are unfreezing
            mFreezedTime = 0;
            _animation->resumeSchedulerAndActions();
            
            //Remove freeze FX
            _animation->setColor(ccc3(255,255,255));
        }
        
        return;
    }
    
    
    //Walk by the movepoints
    float x = getPositionX();
    float y = getPositionY();
    
    //Do we want to catch some dwarf
    if(mCatchingDwarf)
    {
        //Catch that boy
        return;
    }
    
    CCPoint point = _movePoints->getControlPointAtIndex(mMoveIndex);
    
    if (ccpDistanceSQ(point, getPosition()) <= 800)
    {
        if(_moveInCircle){
            if(mMoveClock){
                mMoveIndex--;
                if(mMoveIndex<0)
                    mMoveIndex = _movePoints->count();//Start from 0
            }
            else{
                mMoveIndex++;
                if(mMoveIndex>_movePoints->count())
                    mMoveIndex = 0;//Start from 0
            }
        }
        else{
            mMoveIndex+=_moveValue;
            if(mMoveIndex<0){
                mMoveIndex = 1;
                _moveValue = 1;
            }
            else if(mMoveIndex>_movePoints->count()){
                mMoveIndex = _movePoints->count()-1;
                _moveValue = -1;
            }
        }
    }
    else
    {
        setAngle(atan2f(point.y - y, point.x - x));
    }
    
    CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()),
                              y + sinf(_angle) * delta * (_speed * _game->getGameSpeed()));
    
    cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
}

void Troll::update_old(float delta)
{
    
    
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
    
    if(mCatchingDwarf)
    {
        if(mStartCatchDwarf>0.0f){
            
            if(mCatchRadar->isVisible()){
                mCatchRadar->setVisible(false);
            }
            
            mStartCatchDwarf-=delta;
            
            if(mWarnIcon->isVisible()==false){
                mWarnIcon->setVisible(true);
            }
            return;//Troll waits and then attacks
        }
        
        if(mWarnIcon->isVisible()==true){
            mWarnIcon->setVisible(false);
        }
        
        if(mDwarfToCatch == NULL){
            mCatchingDwarf = false;
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
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()) * User::getInstance()->mDebugSpeed_troll,
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed())  * User::getInstance()->mDebugSpeed_troll);
            
            cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
        }
        else if(theDistance>=20000)
        {
            //He lost it
            CancelDwarfCatch(mDwarfToCatch);
        }
        
        return;
    }
    
    if(User::getInstance()->mDynamicTrolls){
        
        if(!mCatchRadar->isVisible()){
            if(getPositionY()>=mTrollStartY-15 && getPositionY()<=mTrollStartY+15){
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
    
    if((User::getInstance()->mSpecial_10_Mission || User::getInstance()->mNewMissionBuild) && !mTutorialTroll)
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
            
            CCPoint point = mTrollPatrolPoint_1;
            if(mMoveToControl_1){
                point = mTrollPatrolPoint_1;
            }
            else{
                point = mTrollPatrolPoint_2;
            }
            
            if (ccpDistanceSQ(point, getPosition()) <= 1000)
            {
                mMoveToControl_1 = !mMoveToControl_1;
            }
            else
            {
                setAngle(atan2f(point.y - y, point.x - x));
            }
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()) * User::getInstance()->mDebugSpeed_troll,
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed()) * User::getInstance()->mDebugSpeed_troll);
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

    //For now disabled as it's rare to get in to troll action
//    //The magic troll check tornado :)
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
//            _game->stopInGameSound("troll_walk");
//            
//            _game->stopInGameSound("dwarf_web_stuck",true);
//            
////            dwarf->createTrollCrash();
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

void Troll::CatchDwarf(Dwarf* theDwarf)
{
    //set 0.5sec wtf ???
    mStartCatchDwarf = 0.5f;
    mDwarfToCatch = theDwarf;
    mCatchingDwarf = true;
}

void Troll::CancelDwarfCatch(Dwarf* theCancelDwarf)
{
    if(mCatchingDwarf){
        if(mDwarfToCatch == theCancelDwarf){
            mCatchingDwarf = false;
            mDwarfToCatch = NULL;
            
//            if(!mCatchRadar->isVisible()){
//                mCatchRadar->setVisible(true);
//            }
        }
    }
}

void Troll::setFadeIn()
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

void Troll::setVictory()
{
    //Remove it !!!
    mDwarfToCatch = NULL;
    
    setAnimation(_victory);
    _canMove = false;//Disable move for now
    
    _game->playInGameSound("troll_win");
}

void Troll::SetDrawLine(int theLine)
{
    if(theLine == 1){
        mDrawLine = CCSprite::create("Troll_Line_1.png");
    }
    else if(theLine == 2){
        mDrawLine = CCSprite::create("Troll_Line_1.png");
    }
    else if(theLine == 3){
        mDrawLine = CCSprite::create("Troll_Line_2.png");
    }
    else if(theLine == 4){
        mDrawLine = CCSprite::create("Troll_Line_2.png");
    }
    else if(theLine == 5){
        mDrawLine = CCSprite::create("Troll_Line_3.png");
    }
    
    addChild(mDrawLine,0);
}

bool Troll::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    /*
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
    */
    
    //Check if want to do anything with him
    if(getChildByTag(TROLL_SELECT_INDICATOR)==NULL){
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

void Troll::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    //Do nothing for now !!!
    
}

void Troll::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(_touched){
        
        //Check if can blitz it !!!
        _game->mPowerMenu->OnPlayerClickTroll(this);// Do the damage to him
        
        
//        if(_game->CanZipTroll()){
//            _canMove = false;
//            _game->CreateBlitz(getPositionX(),getPositionY()-80,this);
//            if(User::getInstance()->mDynamicTrolls){
//                _game->_SpawnSpecialTrolls = 30;//Get it back after a while !!!
//            }
//        }
    }
    _touched = false;
}

void Troll::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void Troll::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}


void Troll::removeFromSave()
{
    setAnimation(_removeSmoke);
    //Add timer to remove from game after 2sec
    CCActionInterval* aRemoveDelay = CCDelayTime::create(0.5f);
    CCCallFunc* aRemoveCall = CCCallFuncN::create(this, callfuncN_selector(Troll::setForceRemove));
    CCSequence* aRemoveTrollSeq = CCSequence::create(aRemoveDelay,aRemoveCall,NULL);
    
    runAction(aRemoveTrollSeq);
}

void Troll::setForceRemove()
{
    _forceRemove = true;
}

float Troll::GetAngle()
{
    return _angle;
}

void Troll::setAngle(float value)
{
    _lastSetAngle = value;
    
	_angle = wrapTwoPI(value);
    
    
    if(mCatchRadar){
        if(User::getInstance()->mNewSplitCaves)
        {
            if(_animation == _upAnimation)
            {
                if(mCatchRadar->isFlipX()){
                    mCatchRadar->setFlipX(false);
                    mCatchRadar->setAnchorPoint(ccp(1,0.5));
                }
            }
            else if(_animation == _downAnimation)
            {
                if(!mCatchRadar->isFlipX()){
                    mCatchRadar->setFlipX(true);
                    mCatchRadar->setAnchorPoint(ccp(0,0.5));
                }
            }
        }
        else
        {
            if(_animation == _leftAnimation){
                if(mCatchRadar->isFlipX() != _leftAnimation->isFlipX()){
                    mCatchRadar->setFlipX(_leftAnimation->isFlipX());
                    if(mCatchRadar->isFlipX()){
                        mCatchRadar->setAnchorPoint(ccp(0,0.5));
                    }
                    else{
                        mCatchRadar->setAnchorPoint(ccp(1,0.5));
                    }
                }
            }
        }
    }
	
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

void Troll::setAnimation(SpriteAnimation* animation)
{
    //Glitch Fix!!!
    if(animation->getOpacity()<128){
        animation->setOpacity(255);
    }
    
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

bool Troll::getTouchable()
{
	return _touchable;
}

// The new stuff
void Troll::SetMissionStuff(MissionTroll theMission)
{
    //Check if circle then use the circle stuff
    setPosition(ccp(200,200));//Some def value for now !!!
    setAngle(0);
    
    _moveValue = theMission._pathStartIndex;//Start to forward?
    if(_moveValue == 0){
        _moveValue = 1;//Start random
    }
    
    _speed = theMission._speed;
    
    if(theMission._circle>0){
        
        //The params of circles
        float precision = 0;
        float cir = 0;
        int mRadius = 0;
        
        //Check on what circle should set him !!!
//        if(theMission._circle == 1){
//            precision = 0.25f;
//            cir = 2 * M_PI;
//            mRadius = 280;
//        }
        
        _moveInCircle = true;
        mMoveCurrentDir = theMission._startDirection;
        if(mMoveCurrentDir>0)mMoveClock = false;
        else mMoveClock = true;
        
        precision = theMission._circle_precision*0.01;
        cir = 2 * M_PI;
        mRadius = theMission._circle_radius;
        
        int theCircleX = theMission._circle_x;
        int theCircleY = theMission._circle_y;
        float theCircleWidth = theMission._circle_height*0.1;
        
        float last_x = 0;
        float last_y = 0;
        
        float angleSize = 2*M_PI/theMission._circle_precision;
        
        //Create the circle stuff
        bool aDidSetAngle = false;
        
        for (float a = 0.0f; a < cir; a += precision)
        {
//            float x = _game->visibleSize.width/2 + mRadius * cos(a);
//            float y = _game->visibleSize.height/2 + mRadius/1.5f * sin(a);
            
            float x = theCircleX + mRadius * cos(a);
            float y = theCircleY + mRadius/theCircleWidth * sin(a);
            
            _movePoints->addControlPoint(ccp(x,y-50));
            
            CCSprite* pointsBack = CCSprite::create("troll_line.png");
            pointsBack->setPosition(ccp(x,y-50));
//            pointsBack->setOpacity(120);
            
            //Rotate to the next point
            
            if(aDidSetAngle){
                float angle = 360-(atan2(y - last_y, x - last_x) * 180 / M_PI) ;
                pointsBack->setRotation(angle);
            }
            else{
                aDidSetAngle = true;
                pointsBack->setRotation(91);
            }

            
            
//            float angleRadians = atanf((float)last_y / (float)last_x);
//            float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);
//            float cocosAngle = -1 * angleDegrees;
//            pointsBack->setRotation(cocosAngle);
//            pointsBack->seta
//            CCAffineTransformMakeIdentity(angleSize * i);
            
//            float aaaa = CC_RADIANS_TO_DEGREES(atan2f(last_y - y, last_x - x));
//            float cocosAngle = -1 * aaaa;
//            CCLOG("aaaa:%f",aaaa);
//            pointsBack->setRotation(aaaa);
//            setAngle(atan2f(point.y - y, point.x - x));
            
            _game->addChild(pointsBack,1);
            
            last_x = x;
            last_y = y;
        }
        
        //Set it to the point?
        mMoveIndex = theMission._pathStartIndex;
        setPosition(_movePoints->getControlPointAtIndex(theMission._pathStartIndex));
    }
    else{
        //Create control paths !!!
        _moveValue = 1;//Start to forward?
        
        for (int a = 0; a < theMission._paths.size(); a += 1)
        {
            float x = theMission._paths[a]->x;
            float y = theMission._paths[a]->y;
            
            CCSprite* pointsBack = CCSprite::create("troll_line.png");
            pointsBack->setPosition(ccp(x,y));
            pointsBack->setOpacity(120);
            _game->addChild(pointsBack,1);
            
            CCLog("What:%i",a);
            _movePoints->addControlPoint(ccp(x,y));
            CCLog("_movePoints.size():%i",_movePoints->count());
        }
        
        //Set to the start point
        mMoveIndex = theMission._pathStartIndex;
        setPosition(ccp(theMission._paths[theMission._pathStartIndex]->x,theMission._paths[theMission._pathStartIndex]->y));
        
        
        CCLog("end");
    }
    
    
    
}
