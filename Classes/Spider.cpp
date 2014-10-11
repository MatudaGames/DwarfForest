//
//  Spider.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 14/09/14.
//
//

#include "Spider.h"

#include <SimpleAudioEngine.h>
#include "GameScene.h"
#include "AppMacros.h"
#include "SpriteAnimation.h"
#include "Utils.h"
#include "Wind.h"
#include "User.h"

USING_NS_CC;
using namespace CocosDenshion;

const float Spider_SPEED = 30.0f;
const float RADIUS = 40.0f;
const uint8_t TOUCHABLE_OPACITY = 255;
const uint8_t UNTOUCHABLE_OPACITY = 128;

const float PICK_RADIUS = 50.0f;

const uint32_t TOUCHABLE_ZONE = 100;

Spider* Spider::create(GameScene* game)
{
    Spider *pRet = new Spider();
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

Spider::Spider():
_game(NULL), _animation(NULL), _leftUpAnimation(NULL), _leftDownAnimation(NULL),
_leftAnimation(NULL), _upAnimation(NULL), _downAnimation(NULL),_victory(NULL), _angle(0.0f),
_touchable(true),_removeSmoke(NULL),_crashAnimation(NULL)
{
	
}

Spider::~Spider()
{
	if (_game)
    {
        //        _game->playInGameSound("Spider_exit");
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

bool Spider::init(GameScene* game)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    _HideTime = 0;
    
    //    mStartCatchDwarf = 0.0;
    
    mDwarfToCatch = NULL;
    //    mCatchingDwarf = false;
    
    mPatrolPoints = false;
    mMoveToControl_1 = false;
    
    mTutorialSpider = false;
    _touchable = false;
    
    if(User::getInstance()->_tutorial == 0){
        mTutorialSpider = true;
    }
    
    mDrawLineID = -1;
    
    mSpiderStartY = 0;
    
    _waitForReturn = 3;
    
    mRunToExit = false;
    mCrystalCatch = NULL;
    
    mExit_X = 0;
    mExit_Y = 0;
    
    _timeInGame = 0;
    
    mTimesTaped = 0;//3 taps disables it
    
    mMoveByNodes = false;
    mMoveIndex = 0;
    mMoveClock = false;
    
    mAnimationsPaused = false;
    mMoveCurrentDir = 0;
    
    mTutorfix = false;
    
    blitzHit = false;
    
    mCaveID = 0;
    
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
//    addChild(mCatchRadar,0);
    
    
    
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
    
    mSpecialSpider = false;
    if(_game->_spiders->count()>0){
        mSpecialSpider = true;
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
    
    _dwarfCatch = NULL;
    
    mHeadIcon = CCSprite::create("Crystals/blue_crystal.png");
    mHeadIcon->setPosition(ccp(0,50));
    mHeadIcon->setScale(0.75);
    mHeadIcon->setVisible(false);
    mHeadIcon->setOpacity(128);
    addChild(mHeadIcon);
	
	_angle = 6.0f * M_PI / 8.0f;
	_speed = Spider_SPEED;
    
    mSpiderScale = 0.7;
    
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
        _leftUpAnimation->setScale(mSpiderScale);
        _leftDownAnimation->setScale(mSpiderScale);
        _leftAnimation->setScale(mSpiderScale);
        _upAnimation->setScale(mSpiderScale);
        _downAnimation->setScale(mSpiderScale);
        _victory->setScale(mSpiderScale);
        
        _removeSmoke->setScale(mSpiderScale);
    }
	
    _game->playInGameSound("Troll_intro");
	
	return true;
}

void Spider::setAnimationVisibility(bool theValue)
{
    if (_animation)
        _animation->setVisible(theValue);
}

void Spider::setEffect(Effect* value)
{
    
}

void Spider::removeEffect()
{
    
}

void Spider::HitSpider(bool theRemove)
{
//    setVisible(false);
    
    mTimeDown = 5;
    _crashAnimation->resume();
    _crashAnimation->setVisible(true);
    
    if(_dwarfCatch){
        _dwarfCatch->_disabled = false;
        _dwarfCatch = NULL;
    }
    
    return;
}

void Spider::CheckDwarfs()
{
    //Catch if can !!!
	for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
	{
		Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
        
        if (dwarf && dwarf->isVisible())
        {
            //Check if catch
            if (ccpDistanceSQ(dwarf->getPosition(), getPosition()) <= 800)
            {
                dwarf->_disabled=true;
                _dwarfCatch = dwarf;
                break;
            }
        }
    }
    
    //Now let the fun begin
}

void Spider::update(float delta)
{
    if(mTimeDown>0){
        mTimeDown-=delta;
        if(mTimeDown<=0){
            //Allow to move again !!!
            _canMove = true;
            
            //Hide the crash stuff
            if(_crashAnimation->isVisible())
            {
                _crashAnimation->pause();
                _crashAnimation->setVisible(false);
            }
            
            _timeInGame = 11;
        }
        
        return;
    }
    
    if(!_canMove){
        return;
    }
    
    //Walks around it's cave
    if(isVisible())
    {
        float x = getPositionX();
        float y = getPositionY();
        CCPoint point;
        
        //Check if catched something !!!
        if(_dwarfCatch!=NULL){
            //Try to get him to cave !!!
            if(mCaveID == 0){
                point = _game->_SpiderCave_1->getPosition();
            }
            else if(mCaveID == 1){
                point = _game->_SpiderCave_2->getPosition();
            }
            else if(mCaveID == 2){
                point = _game->_SpiderCave_3->getPosition();
            }
            
            
            if (ccpDistanceSQ(point, getPosition()) <= 800)
            {
                setVisible(false);
                _timeInGame = 0;
//                _game->lose();//GameOver
                return;
            }
            else
            {
                setAngle(atan2f(point.y - y, point.x - x));
            }
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed())*0.5f,
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed())*0.5f);
            
            cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
            
            
            _dwarfCatch->setPosition(ccp(getPositionX(),getPositionY()));
            
            return;
        }
        
        _timeInGame+=delta;
        if(_timeInGame>=10){
            //Time to hide
            
            //Move back to cave !!!
            
            if(mCaveID == 0){
                point = _game->_SpiderCave_1->getPosition();
            }
            else if(mCaveID == 1){
                point = _game->_SpiderCave_2->getPosition();
            }
            else if(mCaveID == 2){
                point = _game->_SpiderCave_3->getPosition();
            }
            
            if (ccpDistanceSQ(point, getPosition()) <= 800)
            {
                setVisible(false);
                _timeInGame = 0;
                return;
            }
            else
            {
                setAngle(atan2f(point.y - y, point.x - x));
            }
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()),
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed()));
            
            cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
            
            return;
        }
        
        CheckDwarfs();
        
        //Walk around points !!!
        if(mCaveID==0){
            point = _game->_movePointsSpiderCave_1->getControlPointAtIndex(mMoveIndex);
        }
        else if(mCaveID == 1){
            point = _game->_movePointsSpiderCave_2->getControlPointAtIndex(mMoveIndex);
        }
        else if(mCaveID == 2){
            point = _game->_movePointsSpiderCave_3->getControlPointAtIndex(mMoveIndex);
        }
        
//        CCPoint point = _game->_movePointsWind->getControlPointAtIndex(mMoveIndex);
        
        if (ccpDistanceSQ(point, getPosition()) <= 800)
        {
            if(mMoveClock)
            {
                mMoveIndex--;
                if(mMoveIndex<0){
                    if(mCaveID==0){
                        mMoveIndex = _game->_movePointsSpiderCave_1->count();
                    }
                    else if(mCaveID == 1){
                        mMoveIndex = _game->_movePointsSpiderCave_2->count();
                    }
                    else if(mCaveID == 2){
                        mMoveIndex = _game->_movePointsSpiderCave_3->count();
                    }
                }
            }
            else
            {
                mMoveIndex++;
                if(mCaveID==0){
                    if(mMoveIndex>_game->_movePointsSpiderCave_1->count())
                        mMoveIndex = 0;//Start from 0
                }
                else if(mCaveID == 1){
                    if(mMoveIndex>_game->_movePointsSpiderCave_2->count())
                        mMoveIndex = 0;//Start from 0
                }
                else if(mCaveID == 2){
                    if(mMoveIndex>_game->_movePointsSpiderCave_3->count())
                        mMoveIndex = 0;//Start from 0
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
    else
    {
        //Wait and choose from what cave will come !!!
        _HideTime+=delta;
        if(_HideTime>=2.0f){
            mTimesTaped = 0;
            _timeInGame = 0;
            
            mMoveIndex = 0;
            //Choose where will come out
            _HideTime = 0;
            mCaveID = rand()%3;
            
            //Transfer to that cave pos
            if(mCaveID == 0){
                setPosition(_game->_SpiderCave_1->getPosition());
            }
            else if(mCaveID == 1){
                setPosition(_game->_SpiderCave_2->getPosition());
            }
            else if(mCaveID == 2){
                setPosition(_game->_SpiderCave_3->getPosition());
            }
            
            setVisible(true);
        }
    }
    
    /*
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
        //move Spider somewhere
        //Create some 5 sec
        CCSprite* aMoveToSpt = CCSprite::create("DebugDot.png");
        aMoveToSpt->setTag(89921112);
        
        //Where will we go?
        if(mSpecialSpider)
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
        
        CCCallFuncN* aFuncCall = CCCallFuncN::create(this, callfuncN_selector(Spider::TimeToMoveSpider));
        CCSequence* aSeq2 = CCSequence::create(aDelay,aFuncCall,NULL);
        CCSpawn* aSpawnSeq = CCSpawn::create(aSeq2,aFadeRepeat,NULL);
        aMoveToSpt->runAction(aSpawnSeq);
        
        _game->addChild(aMoveToSpt);
    }
    */
    
    //----------------------------------------------------------
}

void Spider::TimeToMoveSpider(CCNode* sender)
{
    _game->removeChild(sender);
    
    setPosition(mMoveToSpot);
}

void Spider::CatchDwarf(Dwarf* theDwarf)
{
    //set 0.5sec wtf ???
    //    mStartCatchDwarf = 0.5f;
    //    mDwarfToCatch = theDwarf;
    //    mCatchingDwarf = true;
}

void Spider::CancelDwarfCatch(Dwarf* theCancelDwarf)
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

void Spider::setFadeIn()
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

void Spider::setVictory()
{
    //Remove it !!!
    mDwarfToCatch = NULL;
    
    setAnimation(_victory);
    _canMove = false;//Disable move for now
    
    _game->playInGameSound("Spider_win");
}

void Spider::SetDrawLine(int theLine)
{
    if(theLine == 1){
        mDrawLine = CCSprite::create("Spider_Line_1.png");
    }
    else if(theLine == 2){
        mDrawLine = CCSprite::create("Spider_Line_1.png");
    }
    else if(theLine == 3){
        mDrawLine = CCSprite::create("Spider_Line_2.png");
    }
    else if(theLine == 4){
        mDrawLine = CCSprite::create("Spider_Line_2.png");
    }
    else if(theLine == 5){
        mDrawLine = CCSprite::create("Spider_Line_3.png");
    }
    
    addChild(mDrawLine,0);
}

bool Spider::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
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

void Spider::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    //Do nothing for now !!!
    
}

void Spider::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(_touched){
        //Count taps !!!
        mTimesTaped+=1;
        if(mTimesTaped>=3){
            mTimesTaped = 0;
            
            HitSpider(false);
            //            _canMove = false;
            //            mTimeDown = 2.0f;
            
        }
        
        //Check if can blitz it !!!
        if(_game->mZipZapActive){
            //            _canMove = false;
            HitSpider(true);
            _game->CreateBlitz_Spider(getPositionX(),getPositionY()-80,this);
        }
    }
    _touched = false;
}

void Spider::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void Spider::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}


void Spider::removeFromSave()
{
    setAnimation(_removeSmoke);
    //Add timer to remove from game after 2sec
    CCActionInterval* aRemoveDelay = CCDelayTime::create(0.5f);
    CCCallFunc* aRemoveCall = CCCallFuncN::create(this, callfuncN_selector(Spider::setForceRemove));
    CCSequence* aRemoveSpiderSeq = CCSequence::create(aRemoveDelay,aRemoveCall,NULL);
    
    runAction(aRemoveSpiderSeq);
}

void Spider::setForceRemove()
{
    _forceRemove = true;
}

float Spider::GetAngle()
{
    return _angle;
}

void Spider::setAngle(float value)
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

void Spider::setAnimation(SpriteAnimation* animation)
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

bool Spider::getTouchable()
{
	return _touchable;
}