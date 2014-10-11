//
//  Hidra.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 08/09/14.
//
//

#include "Hidra.h"

#include <SimpleAudioEngine.h>
#include "GameScene.h"
#include "AppMacros.h"
#include "SpriteAnimation.h"
#include "Utils.h"
#include "Wind.h"
#include "User.h"

USING_NS_CC;
using namespace CocosDenshion;

const float Hidra_SPEED = 30.0f;
const float RADIUS = 40.0f;
const uint8_t TOUCHABLE_OPACITY = 255;
const uint8_t UNTOUCHABLE_OPACITY = 128;

const float PICK_RADIUS = 50.0f;

const uint32_t TOUCHABLE_ZONE = 100;

Hidra* Hidra::create(GameScene* game)
{
    Hidra *pRet = new Hidra();
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

Hidra::Hidra():
_game(NULL), _animation(NULL), _leftUpAnimation(NULL), _leftDownAnimation(NULL),
_leftAnimation(NULL), _upAnimation(NULL), _downAnimation(NULL),_victory(NULL), _angle(0.0f),
_touchable(true),_removeSmoke(NULL),_crashAnimation(NULL)
{
	
}

Hidra::~Hidra()
{
	if (_game)
    {
        //        _game->playInGameSound("Hidra_exit");
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

bool Hidra::init(GameScene* game)
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
    
    mTutorialHidra = false;
    _touchable = false;
    
    if(User::getInstance()->_tutorial == 0){
        mTutorialHidra = true;
    }
    
    mDrawLineID = -1;
    
    mHidraStartY = 0;
    
    _waitForReturn = 3;
    
    mRunToExit = false;
    mCrystalCatch = NULL;
    
    mExit_X = 0;
    mExit_Y = 0;
    
    mTimesTaped = 0;//3 taps disables it
    
    mMoveByNodes = false;
    mMoveIndex = 0;
    mMoveClock = false;
    
    mAnimationsPaused = false;
    mMoveCurrentDir = 0;
    
    mTutorfix = false;
    
    blitzHit = false;
    
    _canMove = true;//wait a bit !!!s
    _isDisabled = false;
    _forceRemove = false;
	
	_game = game;
	_game->retain();
    
    mCatchRadar = NULL;
    
    mCatchRadar = CCSprite::create("troll_sensor.png");
    mCatchRadar->setAnchorPoint(ccp(1,0.5));
    mCatchRadar->setRotation(270);
    mCatchRadar->setOpacity(64);
    addChild(mCatchRadar,0);
    
    
    
    //The overlay
    //    ccBlendFunc* someBlend = new ccBlendFunc();
    //    someBlend->src = GL_SRC_ALPHA;//OGLES.GL_ONE;
    //    someBlend->dst = GL_ONE_MINUS_SRC_ALPHA;//OGLES.GL_ONE;
    //    _leftUpAnimation->setBlendFunc(*someBlend);
	
	_leftUpAnimation = SpriteAnimation::create("Characters/troll/troll_diagonal_up.plist");
	_leftUpAnimation->retain();
    _leftUpAnimation->setColor(ccc3(153, 0, 153));
	
	_leftDownAnimation = SpriteAnimation::create("Characters/troll/troll_diagonal_down.plist");
	_leftDownAnimation->retain();
    _leftDownAnimation->setColor(ccc3(153, 0, 153));
	
	_leftAnimation = SpriteAnimation::create("Characters/troll/troll_profile.plist");
	_leftAnimation->retain();
    _leftAnimation->setColor(ccc3(153, 0, 153));
    //    ccBlendFunc* someBlend = new ccBlendFunc();
    //    someBlend->src = GL_SRC_ALPHA;//OGLES.GL_ONE;
    //    someBlend->dst = GL_ONE_MINUS_SRC_ALPHA;//OGLES.GL_ONE;
    //    _leftAnimation->setBlendFunc(*someBlend);
	
	_upAnimation = SpriteAnimation::create("Characters/troll/troll_up_back.plist");
	_upAnimation->retain();
    _upAnimation->setColor(ccc3(153, 0, 153));
	
	_downAnimation = SpriteAnimation::create("Characters/troll/troll_down_front.plist");
	_downAnimation->retain();
    _downAnimation->setColor(ccc3(153, 0, 153));
    
    mSpecialHidra = false;
    if(_game->_hidras->count()>0){
        mSpecialHidra = true;
    }
    
    _victory = SpriteAnimation::create("Characters/troll/troll_victory_dance.plist");
    _victory->retain();
    _victory->setColor(ccc3(153, 0, 153));
    
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
	_speed = Hidra_SPEED;
    
    mHidraScale = 1.1;
    
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
        _leftUpAnimation->setScale(mHidraScale);
        _leftDownAnimation->setScale(mHidraScale);
        _leftAnimation->setScale(mHidraScale);
        _upAnimation->setScale(mHidraScale);
        _downAnimation->setScale(mHidraScale);
        _victory->setScale(mHidraScale);
        
        _removeSmoke->setScale(mHidraScale);
    }
	
    _game->playInGameSound("Troll_intro");
	
	return true;
}

void Hidra::setAnimationVisibility(bool theValue)
{
    if (_animation)
        _animation->setVisible(theValue);
}

void Hidra::setEffect(Effect* value)
{
    
}

void Hidra::removeEffect()
{
    
}

void Hidra::HitHidra(bool theRemove)
{
    return;
}

void Hidra::update(float delta)
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
    
    //pause all anims
    if(!mAnimationsPaused){
//        _animation->pause();
        mAnimationsPaused = true;
        
        CCRotateTo* aRotate1 = CCRotateTo::create(4.0f,190);
        CCRotateTo* aRotate2 = CCRotateTo::create(4.0f,350);
        CCSequence* aSeq1 = CCSequence::create(aRotate1,aRotate2,NULL);
        CCRepeatForever* aRepeat1 = CCRepeatForever::create(aSeq1);
        
        mCatchRadar->runAction(aRepeat1);
    }
    
    //Hide the crash stuff
    if(_crashAnimation->isVisible())
    {
        _crashAnimation->pause();
        _crashAnimation->setVisible(false);
    }
    
    //This one does not move but rotates his radar around it !!!
    
    mTimeToMove+=delta;
    if(mTimeToMove>30){
        mTimeToMove = 0;
        //move hidra somewhere
        //Create some 5 sec
        CCSprite* aMoveToSpt = CCSprite::create("DebugDot.png");
        aMoveToSpt->setTag(89921112);
        
        //Where will we go?
        if(mSpecialHidra)
        {
            if(getPositionX()<_game->_screenSize.width/2){
                mMoveToSpot = CCPoint(_game->_screenSize.width/2+200,200);
            }
            else{
                mMoveToSpot = CCPoint(300,400);
            }
        }
        else
        {
            if(getPositionX()<_game->_screenSize.width/2){
                mMoveToSpot = CCPoint(_game->_screenSize.width/2+200,450);
            }
            else{
                mMoveToSpot = CCPoint(300,240);
            }
        }
        
        aMoveToSpt->setPosition(mMoveToSpot);
        
        CCDelayTime* aDelay = CCDelayTime::create(5.0f);
        CCFadeIn* aFadeIn = CCFadeIn::create(0.25f);
        CCFadeOut* aFadeOut = CCFadeOut::create(0.5);
        CCSequence* aFadeSeq = CCSequence::create(aFadeIn,aFadeOut,NULL);
        CCRepeatForever* aFadeRepeat = CCRepeatForever::create(aFadeSeq);
        
        CCCallFuncN* aFuncCall = CCCallFuncN::create(this, callfuncN_selector(Hidra::TimeToMoveHidra));
        CCSequence* aSeq2 = CCSequence::create(aDelay,aFuncCall,NULL);
        CCSpawn* aSpawnSeq = CCSpawn::create(aSeq2,aFadeRepeat,NULL);
        aMoveToSpt->runAction(aSpawnSeq);
        
        _game->addChild(aMoveToSpt);
    }
    
    //----------------------------------------------------------
}

void Hidra::TimeToMoveHidra(CCNode* sender)
{
    _game->removeChild(sender);
    
    setPosition(mMoveToSpot);
}

void Hidra::CatchDwarf(Dwarf* theDwarf)
{
    //set 0.5sec wtf ???
    //    mStartCatchDwarf = 0.5f;
    //    mDwarfToCatch = theDwarf;
    //    mCatchingDwarf = true;
}

void Hidra::CancelDwarfCatch(Dwarf* theCancelDwarf)
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

void Hidra::setFadeIn()
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

void Hidra::setVictory()
{
    //Remove it !!!
    mDwarfToCatch = NULL;
    
    setAnimation(_victory);
    _canMove = false;//Disable move for now
    
    _game->playInGameSound("Hidra_win");
}

void Hidra::SetDrawLine(int theLine)
{
    if(theLine == 1){
        mDrawLine = CCSprite::create("Hidra_Line_1.png");
    }
    else if(theLine == 2){
        mDrawLine = CCSprite::create("Hidra_Line_1.png");
    }
    else if(theLine == 3){
        mDrawLine = CCSprite::create("Hidra_Line_2.png");
    }
    else if(theLine == 4){
        mDrawLine = CCSprite::create("Hidra_Line_2.png");
    }
    else if(theLine == 5){
        mDrawLine = CCSprite::create("Hidra_Line_3.png");
    }
    
    addChild(mDrawLine,0);
}

bool Hidra::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
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

void Hidra::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    //Do nothing for now !!!
    
}

void Hidra::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(_touched){
        //Count taps !!!
        mTimesTaped+=1;
        if(mTimesTaped>=3){
            mTimesTaped = 0;
            
            HitHidra(false);
            //            _canMove = false;
            //            mTimeDown = 2.0f;
            
        }
        
        //Check if can blitz it !!!
        if(_game->mZipZapActive){
            //            _canMove = false;
            HitHidra(true);
            _game->CreateBlitz_Hidra(getPositionX(),getPositionY()-80,this);
        }
    }
    _touched = false;
}

void Hidra::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void Hidra::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}


void Hidra::removeFromSave()
{
    setAnimation(_removeSmoke);
    //Add timer to remove from game after 2sec
    CCActionInterval* aRemoveDelay = CCDelayTime::create(0.5f);
    CCCallFunc* aRemoveCall = CCCallFuncN::create(this, callfuncN_selector(Hidra::setForceRemove));
    CCSequence* aRemoveHidraSeq = CCSequence::create(aRemoveDelay,aRemoveCall,NULL);
    
    runAction(aRemoveHidraSeq);
}

void Hidra::setForceRemove()
{
    _forceRemove = true;
}

float Hidra::GetAngle()
{
    return _angle;
}

void Hidra::setAngle(float value)
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

void Hidra::setAnimation(SpriteAnimation* animation)
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

bool Hidra::getTouchable()
{
	return _touchable;
}
