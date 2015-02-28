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
#include <math.h>

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
    
    _disabled = false;
    
    _pathSprites = NULL;
    
    mRadarSet = false;
    
    mCatchRadarAngle = 0;
    
    mEnemyID = -1;
    
    coneSwingAmp = 80; // kāda būs konusa svārstību amplitūda grādos
    coneSwingSpeed = 1; // cik ātri svārstīsies konuss
    loopPosition = 0;
    
    coneWidth = 60; // konusa platums grādos
    
    coneRadius = 100; // konusa garums pikseļos
    coneAngleOffset = 0;
    coneAngle = 0;
    
    _testAngle = 0;
    
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
    
    mGoblinFunctional = false;
    mEvilTreeFunctional = false;
    
    _canMove = false;//wait a bit !!!s
    _isDisabled = false;
    _forceRemove = false;
	
	_game = game;
	_game->retain();
	
	//TestTest();
    
    //--------------
    /*
    mCatchRadar = NULL;
    
    mCatchRadar = CCSprite::create("troll_sensor.png");
    mCatchRadar->setAnchorPoint(ccp(0,0.5));
    mCatchRadar->setFlipX(true);
    mCatchRadar->setOpacity(0);
    addChild(mCatchRadar,0);
    */
    //---------------
    
    
//    if(User::getInstance()->mDynamicTrolls)
//    {
//        mCatchRadar = CCSprite::create("troll_sensor.png");
//        if(User::getInstance()->mNewSplitCaves)
//        {
//            mCatchRadar->setAnchorPoint(ccp(0.5,0.5));
//            mCatchRadar->setRotation(90);
//            mCatchRadar->setOpacity(64);
//        }
//        else
//        {
//            mCatchRadar->setAnchorPoint(ccp(0,0.5));
//            mCatchRadar->setOpacity(64);
//        }
//        addChild(mCatchRadar,0);
        
    
    mWarnIcon = CCSprite::create("troll_warn.png");
    mWarnIcon->setAnchorPoint(ccp(0,0.5));
    mWarnIcon->setPosition(ccp(0,40));
    mWarnIcon->setVisible(false);
    addChild(mWarnIcon,100);
    
    
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
    
    //TestTest();
    
    // The radar stuff
    mCatchRadar = CCDrawNode::create();
    mCatchRadar->setVisible(false);
    this->addChild( mCatchRadar );//dnode
    
    // Draw the conus from settings
    /*
    std::vector<CCPoint> points;
    points.push_back(ccp(0,0));
    
    for (float ii = 0; ii < coneWidth; ii += 0.1)
    {
        points.push_back(ccp(0 + coneRadius * cos(ii * (M_PI / 180)), 0 + coneRadius * sin(ii * (M_PI / 180))));
    }
    
    points.push_back(ccp(0,0));
    
    dnode->drawPolygon_fromVector(points, points.size(), ccc4f(1, 0, 0, 0.4f), 2, ccc4f(0, 0, 0, 0.1) );
    */
    
//    CCRotateBy* aRotate1= CCRotateBy::create(1.0,10);
//    CCRepeatForever* rep = CCRepeatForever::create(aRotate1);
//    dnode->runAction(rep);
    
    
	
	return true;
}

void Troll::setRadar(int theRadius,int theWidth)
{
    // No radar for this troll
    if(theRadius == 0 || theWidth == 0) return;
    
    // Yes we will use radar
    mRadarSet = true;
    
    // Draw the conus from settings
    std::vector<CCPoint> points;
    points.push_back(ccp(0,0));
    
    mCatchRadar->clear();
    
    // ----------------------------
    coneWidth = theWidth;
    coneRadius = theRadius;
    
    for (float ii = 0; ii < coneWidth; ii += 0.1)
    {
        points.push_back(ccp(0 + coneRadius * cos(ii * (M_PI / 180)), 0 + coneRadius * sin(ii * (M_PI / 180))));
    }
    
    points.push_back(ccp(0,0));
    
    CCPoint* pPoints = new CCPoint[points.size()];
    
    for (unsigned int i = 0; i < points.size(); ++i)
    {
        pPoints[i].x = points[i].x;
        pPoints[i].y = points[i].y;
    }
    
    mCatchRadar->drawPolygon(pPoints, points.size(), ccc4f(1, 0, 0, 0.4f), 2, ccc4f(0, 0, 0, 0.1) );
    
    mCatchRadar->setRotation(0);
}

void Troll::setAnimationVisibility(bool theValue)
{
    if (_animation)
        _animation->setVisible(theValue);
}

void Troll::setEvilTreeAnimation()
{	
		_leftUpAnimation = SpriteAnimation::create("Characters/evil_tree/evil_tree_diagonal_up.plist");
		_leftUpAnimation->retain();
		
		_leftDownAnimation = SpriteAnimation::create("Characters/evil_tree/evil_tree_diagonal_down.plist");
		_leftDownAnimation->retain();
	
		_leftAnimation = SpriteAnimation::create("Characters/evil_tree/evil_tree_profile.plist");
		_leftAnimation->retain();
		
		_upAnimation = SpriteAnimation::create("Characters/evil_tree/evil_tree_up.plist");
		_upAnimation->retain();
		
		_downAnimation = SpriteAnimation::create("Characters/evil_tree/evil_tree_down.plist");
		_downAnimation->retain();
		
		_leftUpAnimation->setScale(GLOBAL_SCALE);
        _leftDownAnimation->setScale(GLOBAL_SCALE);
        _leftAnimation->setScale(GLOBAL_SCALE);
        _upAnimation->setScale(GLOBAL_SCALE);
        _downAnimation->setScale(GLOBAL_SCALE);
}

void Troll::setGoblinAnimation()
{	
		_leftUpAnimation = SpriteAnimation::create("Characters/goblin/goblin_walk_diagonal_up.plist");
		_leftUpAnimation->retain();
		
		_leftDownAnimation = SpriteAnimation::create("Characters/goblin/goblin_walk_diagonal_down.plist");
		_leftDownAnimation->retain();
	
		_leftAnimation = SpriteAnimation::create("Characters/goblin/goblin_walk_profile.plist");
		_leftAnimation->retain();
	
		_upAnimation = SpriteAnimation::create("Characters/goblin/goblin_walk_up.plist");
		_upAnimation->retain();
	
		_downAnimation = SpriteAnimation::create("Characters/goblin/goblin_walk_down.plist");
		_downAnimation->retain();
		
		_leftUpAnimation->setScale(GLOBAL_SCALE);
        _leftDownAnimation->setScale(GLOBAL_SCALE);
        _leftAnimation->setScale(GLOBAL_SCALE);
        _upAnimation->setScale(GLOBAL_SCALE);
        _downAnimation->setScale(GLOBAL_SCALE);
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

void Troll::UpdateRadar(float delta)
{
    // Will add later smooth rotate to angle :)
    if(!mCatchRadar->isVisible()){
        mCatchRadar->setVisible(true);
    }
    
    /*
    float aRotation = mCatchRadar->getRotation()-((-_angle * 180.0 / M_PI)+coneWidth/2)*0.1;
    float aRota2 = (-_angle * 180.0 / M_PI)+coneWidth/2;
//    CCLog("aRota2 %f",aRota2);
//    CCLog("aRotation %f",aRotation);
    
    float aCurrentRadar = mCatchRadar->getRotation();
    CCLog("aCurrentRadar %f",aCurrentRadar);
    float aNeedRadar = (-_angle * 180.0 / M_PI)+coneWidth/2;
    CCLog("aNeedRadar %f",aNeedRadar);
    
    float aChange = aCurrentRadar + aNeedRadar;
    CCLog("aChange %f",aChange);
    
    float aFinalValue = 0;//mCatchRadar->getRotation() + aChange;
    
    if(aNeedRadar<=0){
        aFinalValue = mCatchRadar->getRotation()+delta*abs(aChange);
    }
    else{
        aFinalValue = mCatchRadar->getRotation()-delta*abs(aChange);
    }
    
    CCLog("aFinalValue %f",aFinalValue);
     
      mCatchRadar->setRotation(aFinalValue);
    */
    
    
    
   
    
    
//    mCatchRadar->setRotation((-_angle * 180.0 / M_PI)+coneWidth/2);
    
    
    int aCurrentAngle = (-_angle * 180.0 / M_PI)+coneWidth/2;
    if(mCatchRadarAngle != aCurrentAngle)
    {
        int aChange = abs(aCurrentAngle-mCatchRadarAngle);
        
//        CCLog("aChange:%i",aChange);
        
        mCatchRadarAngle = aCurrentAngle;
        
//        CCRotateTo* aRotate;
//        if(_changedAnimation){
//            _changedAnimation = false;
//            aRotate = CCRotateTo::create(0.1f, mCatchRadarAngle);
//        }
//        else{
//            aRotate = CCRotateTo::create(0.3f, mCatchRadarAngle);
//        }
        
        //Check how big is the change
        float aSpeedToRotate = 20 - abs(aChange);
        if(aSpeedToRotate<=0)aSpeedToRotate = 0.2;
        else aSpeedToRotate *= 0.025f;
        
        //The limit
        if(aSpeedToRotate<0.1){
            aSpeedToRotate = 0.2f;
        }
        
//        CCLog("Rotate speed %f",aSpeedToRotate);
        
        CCRotateTo* aRotate = CCRotateTo::create(aSpeedToRotate, mCatchRadarAngle);
        
//        mCatchRadar->stopAllActions();
        mCatchRadar->runAction(aRotate);
//        mCatchRadar->setRotation((-_angle * 180.0 / M_PI)+coneWidth/2);
    }
    
    
    
    
//    _testAngle+=delta*10;
//    if(_testAngle>=360)_testAngle = 0;
//    
//    mDebugPoint1->setRotation(_testAngle);
    
    
//    coneAngleOffset = ((coneSwingAmp / 2) * sin(0.01 * loopPosition * coneSwingSpeed * M_PI)) - coneWidth / 2;
    
//    coneAngleOffset = ((1 / 2) * sin(0.01 * 1 * 1 * M_PI)) - coneWidth / 2;
//    coneAngle = coneWidth + coneAngleOffset;
    
    // Draw the cone !!!
    
    /*
    ctx.beginPath();
    ctx.arc(trollPosX, trollPosY, coneRadius, offset * (Math.PI / 180), coneAngle * (Math.PI / 180), false);
    ctx.moveTo(trollPosX + Math.cos(coneAngle * (Math.PI / 180)) * coneRadius, trollPosY + Math.sin(coneAngle * (Math.PI / 180)) * coneRadius);
    ctx.lineTo(trollPosX, trollPosY);
    ctx.lineTo(trollPosX + Math.cos(offset * (Math.PI / 180)) * coneRadius, trollPosY + Math.sin(offset * (Math.PI / 180)) * coneRadius);
    ctx.fillStyle = 'pink';
    ctx.fill();
    */
    
//    ccDrawInit();
//    ccDrawCircle(getPosition(), coneRadius, coneAngleOffset * (M_PI/180), 5, true);
//    ccdr
    
    //------------------
    
//    loopPosition += 1;
//    if (loopPosition >= 200) loopPosition = 0;
    
//    mCatchRadar->setRotation(-_angle * 180.0 / M_PI);
    
    
    
//    CCLog("Angle: %f",_angle);
    
//    float x = getPositionX();
//    float y = getPositionY();
    
//    CCPoint newPosition = ccp(x + cosf(_angle),
//                              y + sinf(_angle));
    
//    CCPoint newPosition = ccp(100*cosf(_angle),
//                              100*sinf(_angle));
    
//    p'x = cos(theta) * (px-ox) - sin(theta) * (py-oy) + ox
//    p'y = sin(theta) * (px-ox) + cos(theta) * (py-oy) + oy
    
//    int aX = cos(_angle) * (mDebugPoint1->getPositionX()-100) - sin(_angle) * (mDebugPoint1->getPositionY()-100) + 100;
//    int aY = sin(_angle) * (mDebugPoint1->getPositionX()-100) + cos(_angle) * (mDebugPoint1->getPositionY()-100) + 100;
    
//    float rotateBy = 90 * delta;
//    CCLog("rotateBy: %f",rotateBy);
//    
//    CCPoint center = ccp(100,100);
//    
//    int angle = (rotateBy ) * (M_PI/180); // Convert to radians
//    CCLog("angle %i",angle);
    
//    int rotatedX = cos(angle) * (mDebugPoint1->getPositionX() - center.x) - sin(angle) * (mDebugPoint1->getPositionY() - center.y) + center.x;
//    int rotatedY = sin(angle) * (mDebugPoint1->getPositionX() - center.x) + cos(angle) * (mDebugPoint1->getPositionY() - center.y) + center.y;
    
//    CCPoint Pivot = ccp(0,0);
    
//    _Angle+=0.1;
//    if(_Angle>360)_Angle = 0;
//    CCLog("Angle:%f",_Angle);
    
    /*
    // Looks that this works
    
    float Angle = _angle;
    
    float x = 100;
    float y = 50;
    
    float s = sin(Angle);
    float c = cos(Angle);
    
    x -= Pivot.x;
    y -= Pivot.y;
    
    double nx = (x * c) - (y * s);
    double ny = (x * s) + (y * c);
    
    x = nx + Pivot.x;
    y = ny + Pivot.y;
    
    
    mDebugPoint1->setPosition(ccp(x,y));
    */
    
//    mDebugPoint1->setPosition(ccp());
//    mDebugPoint1 = getPositionX()+100;
}

bool Troll::collideAtPoint(cocos2d::CCPoint point) {
    
    // No need to go futher - no radar on troll
    if(mRadarSet == false) return false;
    
    coneAngleOffset = -fmod(mCatchRadar->getRotation(), 360);
    coneAngle = coneWidth + coneAngleOffset;
    
    // Viesturs troll radar
    int dx = point.x - getPositionX();
    int dy = point.y - getPositionY();
    
    int distanceFromTroll = sqrt(pow(dx, 2) + pow(dy, 2));
    int angle = atan2(dy, dx) * (180 / M_PI);
    
    if(angle>0 && fmod(mCatchRadar->getRotation(), 360)>=coneWidth){
        angle-=360;
    }
    
    bool result = false;
    
    if(distanceFromTroll <= coneRadius && (angle >= coneAngleOffset && angle <= coneAngle)) result = true;
    
    return result;
}

void Troll::update(float delta)
{
    //Order the troll to sit cool in map
    if (_game->getSpriteOrderZ(getPositionY())!=getZOrder()){
        _game->reorderChild(this,_game->getSpriteOrderZ(getPositionY()));
    }
    
    if(_disabled){
        return;
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
            if(mGoblinFunctional || mEvilTreeFunctional){
                
            }
            else{
                _animation->setColor(ccc3(255,255,255));
            }
        }
        
        return;
    }
    
    if(_timeOnMap>0){
        // Remove from map when can !!!
        _timeOnMap-=delta;
        if(_timeOnMap<=0){
            // Game over for this bee
            CCLog("Remove troll from map");
//            _game->_trolls->removeObject(this);
//            _game->removeNode(this);
//            _forceRemove = true;
            removeFromSave();
            
            return;
        }
    }
    
    //Walk by the movepoints
    float x = getPositionX();
    float y = getPositionY();
    
    //Do we want to catch some dwarf
    
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
        
        //This ignores it
        if(mEnemySpawnID == 3)
        {
            setAngle(atan2f(point.y - y, point.x - x));
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()),
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed()));
            
            cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
        }
        else
        {
            // Catch till catch him
            setAngle(atan2f(point.y - y, point.x - x));
            
            CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()),
                                      y + sinf(_angle) * delta * (_speed * _game->getGameSpeed()));
            
            cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
            
            /*
            float theDistance2 = sqrtf((getPositionX()-point.x)*(getPositionX()-point.x) + (getPositionY()-point.y)*(getPositionY()-point.y));
            
            if (theDistance2<coneRadius){
                setAngle(atan2f(point.y - y, point.x - x));
                
                CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()),
                                          y + sinf(_angle) * delta * (_speed * _game->getGameSpeed()));
                
                cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
            }
            else
            {
                //He lost it
                CancelDwarfCatch(mDwarfToCatch);
            }
            */
        }
        
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
    
    if(mRadarSet) UpdateRadar(delta);
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
//    CCLog("mStartCatchDwarf set to 1.0f");
    if(mFromMission_CatchDwarfWait>0){
        mStartCatchDwarf = mFromMission_CatchDwarfWait;
    }
    else{
        mStartCatchDwarf = 1.0f;
    }
    
    mDwarfToCatch = theDwarf;
    mCatchingDwarf = true;
}

void Troll::CancelDwarfCatch(Dwarf* theCancelDwarf)
{
    if(mCatchingDwarf){
        if(mDwarfToCatch == theCancelDwarf){
            
            //Check if did not complete some mission with escape from trolls
            _game->_mission_escaped_trolls+=1;
            _game->CheckMissionByValue(MissionType_TrollEscape, _game->_mission_escaped_trolls);
            
            mCatchingDwarf = false;
            mDwarfToCatch = NULL;
            
            if(!mCatchRadar->isVisible()){
                mCatchRadar->setVisible(true);
            }
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
    
//    CCLog("touch");
    
    CCLog("Touched X:%f | Y:%f",touch->getLocation().x,touch->getLocation().y);
//    CCLog("Touched X:%f | Y:%f",touch->getLocationInView().x,touch->getLocationInView().y);
    
    touch->getLocation();
    
//    collideAtPoint(ccp(touch->getLocation().x,touch->getLocation().y));
    
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
        /*
        _game->mPowerMenu->OnPlayerClickTroll(this);// Do the damage to him
        */
        
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
    
    if(_pathSprites)
    {
        for(int i = 0;i<_pathSprites->count();i++)
        {
            _game->removeChild(static_cast<CCSprite*>(_pathSprites->objectAtIndex(i)));
        }
        
        _pathSprites->release();
        _pathSprites = NULL;
    }
	
	CCNode::onExit();
}

void Troll::removeFromSave()
{
    _disabled = true;
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
    
    /*
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
    */
	
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
        
        // Forced radar update
        _changedAnimation = true;
	}
    
    // The color set for goblin
    if(mGoblinFunctional)
    {
        // Set some blue to all
        /*
        ccColor3B currentColor = _animation->getColor();
        
        if(currentColor.r != 201 || currentColor.g != 0 || currentColor.b != 255){
            _animation->setColor(ccc3(222, 0, 255));
        }
        */
    }
    
    else if(mEvilTreeFunctional)
    {
        //Set some blue to all
        /*
        ccColor3B currentColor = _animation->getColor();
		
        if(currentColor.r != 255 || currentColor.g != 154 || currentColor.b != 0){
            _animation->setColor(ccc3(255, 154, 0));
        }
        */    
	}

}

bool Troll::getTouchable()
{
	return _touchable;
}

// The new stuff
void Troll::SetMissionStuff(MissionTroll theMission)
{
    mFromMission_CatchDwarfWait = 0;
    
    // Check if this is not some same stuff troll stuff
    if(theMission._enemySpawnID == 2){
        // Smaller blue troll
        mGoblinFunctional = true;
    }
    else if(theMission._enemySpawnID == 3){
        mEvilTreeFunctional = true;
    }
    
    
    mEnemySpawnID = 0;
    if(theMission._enemySpawnID>0){
        mEnemySpawnID = theMission._enemySpawnID;
    }
    
    if(theMission._radar_wait>0){
        mFromMission_CatchDwarfWait = theMission._radar_wait;
    }
    
//    mGoblinFunctional = true;
//    mEvilTreeFunctional = true;
//    mEnemySpawnID = 3;
    
    //Check if circle then use the circle stuff
    setPosition(ccp(200,200));//Some def value for now !!!
    setAngle(0);
    
    _moveValue = theMission._pathStartIndex;//Start to forward?
    if(_moveValue == 0){
        _moveValue = 1;//Start random
    }
    
    // This one will expier on map
    _timeOnMap = -1;// Stays on map for ever
    
    if(theMission._timeOnMap>0){
        _timeOnMap = theMission._timeOnMap;
    }
    
    mEnemyID = theMission._indexID;
    
    // Do wee need radar?
    setRadar(theMission._radar_radius,theMission._radar_width);
    
    _speed = theMission._speed;
    
    if(theMission._circle>0){
        
        //The params of circles
        float precision = 0;
        float cir = 0;
        int mRadius = 0;
        
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
        
        _pathSprites = CCArray::create();
        _pathSprites->retain();
        
        for (float a = 0.0f; a < cir; a += precision)
        {
        	if(_game->EnemyLineCount >0){
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
            
            _pathSprites->addObject(pointsBack);
            
            _game->addChild(pointsBack,1);
            
            last_x = x;
            last_y = y;
        	}
        }
        
        //Set it to the point?
        mMoveIndex = theMission._pathStartIndex;
        setPosition(_movePoints->getControlPointAtIndex(theMission._pathStartIndex));
    }
    else{
        
        _pathSprites = CCArray::create();
        _pathSprites->retain();
        
        //Create control paths !!!
        _moveValue = 1;//Start to forward?
        
        for (int a = 0; a < theMission._paths.size(); a += 1)
        {
            float x = theMission._paths[a]->x;
            float y = theMission._paths[a]->y;
            
            //Lets do magic
            if(a>0)
            {
            	if(_game->EnemyLineCount >0){
                // Get the draw cords
                int P1_y = y;
                int P1_x = x;
                
                int P2_y = theMission._paths[a-1]->y;
                int P2_x = theMission._paths[a-1]->x;
                
                float theDistance2 = sqrtf((x-theMission._paths[a-1]->x)*(x-theMission._paths[a-1]->x) +
                                           (y-theMission._paths[a-1]->y)*(y-theMission._paths[a-1]->y));
                // Get the angle between points?
                float deltaY = P2_y - P1_y;
                float deltaX = P2_x - P1_x;
                
                float angleInDegrees = atan2(deltaY, deltaX) * 180 / M_PI;
                
                //Lets check if this is true
                int aRealFinalX = x + theDistance2*cos(angleInDegrees*(M_PI / 180));
                int aRealFinalY = y + theDistance2*sin(angleInDegrees*(M_PI / 180));
                
                CCSprite* pointsBack;
                int aTimesToDraw = theDistance2/20;
                for(int i = 0;i<aTimesToDraw;i++)
                {
                    pointsBack = CCSprite::create("troll_line.png");
                    
                    aRealFinalX = x + (20*i)*cos(angleInDegrees*(M_PI / 180));
                    aRealFinalY = y + (20*i)*sin(angleInDegrees*(M_PI / 180));
                    
                    pointsBack->setPosition(ccp(aRealFinalX,aRealFinalY));
                    pointsBack->setRotation(angleInDegrees);
                    pointsBack->setOpacity(120);
                    _game->addChild(pointsBack,1);
                    
                    _pathSprites->addObject(pointsBack);
                	}
				}
            }
            
            /*
            CCSprite* pointsBack = CCSprite::create("troll_line.png");
            pointsBack->setPosition(ccp(x,y));
            pointsBack->setOpacity(120);
            _game->addChild(pointsBack,1);
            
            CCLog("What:%i",a);
            _movePoints->addControlPoint(ccp(x,y));
            CCLog("_movePoints.size():%i",_movePoints->count());
            
            _pathSprites->addObject(pointsBack);
            */
            
            _movePoints->addControlPoint(ccp(x,y));
        }
        
        //Set to the start point
        mMoveIndex = theMission._pathStartIndex;
        setPosition(ccp(theMission._paths[theMission._pathStartIndex]->x,theMission._paths[theMission._pathStartIndex]->y));
        
        
        CCLog("end");
    }
    
    
    
}
