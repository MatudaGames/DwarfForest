//
//  Dwarf.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 22.02.2013.
//
//

#include <SimpleAudioEngine.h>
#include "Dwarf.h"
#include "GameScene.h"
#include "Effect.h"
#include "Wind.h"
#include "SpriteAnimation.h"
#include "Utils.h"
#include "User.h"

USING_NS_CC;
using namespace CocosDenshion;

const float DWARF_SPEED_TALL = 40.0f;//40.0f;//50
const float DWARF_SPEED_FAT = 40.f;//30.0f;//30

const float RADIUS = 30.0f;
const float PICK_RADIUS = 50.0f;
const float POINT_RADIUS = 20.0f;//20

const float FAT_SNAP_TO_CAVE = 2000.0f;//1500
const float TALL_SNAP_TO_CAVE = 2000.0f;//1600

#define kFreezeSprite 111

Dwarf* Dwarf::create(GameScene* game,int theType)
{
    Dwarf *pRet = new Dwarf();
    if (pRet && pRet->init(game,theType))
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

Dwarf::Dwarf():
	_game(NULL), _effect(NULL), _introAnimation(NULL), _animation(NULL),
	_leftUpAnimation(NULL), _leftDownAnimation(NULL),
	_leftAnimation(NULL), _upAnimation(NULL), _downAnimation(NULL), _line(NULL),
	_angle(0.0f),_crashAnimation(NULL),_alarmAnimation(NULL),_removeSmoke(NULL)
{
	
}

Dwarf::~Dwarf()
{
	unschedule(schedule_selector(Dwarf::removeWeb));
	if (_effect) _effect->release();
	
	if (_game) _game->release();
	
	if (_introAnimation) _introAnimation->release();
	
	if (_leftUpAnimation) _leftUpAnimation->release();
	if (_leftDownAnimation) _leftDownAnimation->release();
	if (_leftAnimation) _leftAnimation->release();
	if (_upAnimation) _upAnimation->release();
	if (_downAnimation) _downAnimation->release();
    
    if (_crashAnimation) _crashAnimation->release();
	if (_alarmAnimation) _alarmAnimation->release();
    
    if (_removeSmoke) _removeSmoke->release();
}

bool Dwarf::init(GameScene* game,int theType)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    mTotemSlow = 1;
    mTotemZoneIn = 0;
    
    // Qucik check for quad system - helper that says if dwarf is already removing
    _dwarfIsRemoving = false;
    
    mSnapedToTotem = false;
    
    mCanSearchForTrollsForSnap = false;
    mSnapedToMasterTroll = false;
    mSnapedToTotem = false;
    
    mContainsPowerUp = -1;// No power
    mSnapedTroll = NULL;
    mSnapedTroll_FallBack = NULL;
    
    _effect = NULL;
    
    _SpawnID = -1;
    
    mBulletActive = false;
    _knockOutTime = -1;
    
    mCrystalPoints = 0;
    
    mSpecialObjectPicked = false;
    
    _knockOut = false;
    
    mCollectedCrystals = 0;
    mCrystalSlowDown = 1.0f;
    
    mLineDotIndex = 1;
    
    //Move or not in diognal
    _diognalMove = true;
    _coolDown = 2.0f;
    
    _isConnectedToCave = false;
    
    _disabled = true;//Wait for real spawn and make the magic
    
    _alarmDwarf = false;
    _alarmTroll = false;
    
    _PoisonTime = 0;
    _PoisonSlowValue = 0;
	
	_game = game;
	game->retain();
    
    _canPlayAlarm = false;
    _alarmWasSet = false;
    
    _forceRemove = false;
	
//	_type = static_cast<DwarfType>(rand() % DWARF_TYPE_COUNT);
    if(theType==-1)
        _type = static_cast<DwarfType>(rand() % DWARF_TYPE_COUNT);
    else
        _type = static_cast<DwarfType>(theType);
	
	if (_type == DWARF_TYPE_TALL)
	{
		_leftUpAnimation = SpriteAnimation::create("Characters/tall_dwarf/tall_dwarf_diagonal_up.plist");
		_leftUpAnimation->retain();
		
		_leftDownAnimation = SpriteAnimation::create("Characters/tall_dwarf/tall_dwarf_diagonal_down.plist");
		_leftDownAnimation->retain();
		
		_leftAnimation = SpriteAnimation::create("Characters/tall_dwarf/tall_dwarf_profile.plist");
		_leftAnimation->retain();
		
		_upAnimation = SpriteAnimation::create("Characters/tall_dwarf/tall_dwarf_up.plist");
		_upAnimation->retain();
		
		_downAnimation = SpriteAnimation::create("Characters/tall_dwarf/tall_dwarf_down.plist");
		_downAnimation->retain();
        
//        _speed = DWARF_SPEED_TALL;
	}
	else if (_type == DWARF_TYPE_FAT)
	{
		_leftUpAnimation = SpriteAnimation::create("Characters/fat_dwarf/fat_dwarf_diagonal_up.plist");
		_leftUpAnimation->retain();
		
		_leftDownAnimation = SpriteAnimation::create("Characters/fat_dwarf/fat_dwarf_diagonal_down.plist");
		_leftDownAnimation->retain();
		
		_leftAnimation = SpriteAnimation::create("Characters/fat_dwarf/fat_dwarf_profile.plist");
		_leftAnimation->retain();
		
		_upAnimation = SpriteAnimation::create("Characters/fat_dwarf/fat_dwarf_up.plist");
		_upAnimation->retain();
		
		_downAnimation = SpriteAnimation::create("Characters/fat_dwarf/fat_dwarf_down.plist");
		_downAnimation->retain();
        
//        _speed = DWARF_SPEED_FAT;
	}
    
    //Add the ghoust backround
//    mGhoustBG = CCSprite::create("GhoustFat.png");
//    addChild(mGhoustBG,0);
    
    //Add the sand bag
    /*
     
    if(User::getInstance()->mNewMissionBuild)
    {
        int aX_Bag = 0;
        if (_type == DWARF_TYPE_TALL){
            aX_Bag = 5;
        }
        
        _BagIcon_1 = CCSprite::create("new_mission/maiss_empty_final.png");
        _BagIcon_1->setScale(0.7);
        _BagIcon_1->setOpacity(0);
        _BagIcon_1->setPosition(ccp(aX_Bag,80));
        addChild(_BagIcon_1);
        
        if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission
           || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
           || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission){
            _BagIcon_1->setVisible(false);
        }
        
        _BagIcon_2 = CCSprite::create("new_mission/maiss_half_final.png");
        _BagIcon_2->setPosition(ccp(aX_Bag,80));
        _BagIcon_2->setScale(0.7);
        _BagIcon_2->setVisible(false);
        addChild(_BagIcon_2);
        _BagIcon_3 = CCSprite::create("new_mission/maiss_full_final.png");
        _BagIcon_3->setPosition(ccp(aX_Bag,80));
        _BagIcon_3->setScale(0.7);
        addChild(_BagIcon_3);
        _BagIcon_3->setVisible(false);
        
        //Check by current mission whats the max bag amount
        if( User::getInstance()->getMissionManager().GetActiveMissionID() == 1 ){ mCollectedCrystalsMax = 1; }
        else if( User::getInstance()->getMissionManager().GetActiveMissionID() == 2 ){ mCollectedCrystalsMax = 1; }
        else if( User::getInstance()->getMissionManager().GetActiveMissionID() == 3 ){ mCollectedCrystalsMax = 1; }
        else if( User::getInstance()->getMissionManager().GetActiveMissionID() == 4 ){ mCollectedCrystalsMax = 2; }
        else if( User::getInstance()->getMissionManager().GetActiveMissionID() == 5 ){ mCollectedCrystalsMax = 2; }
        else{ mCollectedCrystalsMax = 3; }
        
        
        mNewMissionTimeout = 3;
        
    }
    */
    
    mNewMissionTimeout = 3;
	
	_movePoints = CCPointArray::create(0);
	_movePoints->retain();
	
//	_speed = DWARF_SPEED;
	
//	_line = CCSpriteBatchNode::create("trajectory_dot_white_white.png");
    _line = CCSpriteBatchNode::create("trajectory_dot_white.png");
	addChild(_line);
    
    //Add the kuku anim
    _crashAnimation = SpriteAnimation::create("effects/crash_stars.plist");
    _crashAnimation->retain();
    _crashAnimation->setTag(111);
    
    _alarmAnimation = SpriteAnimation::create("effects/crash_alarming.plist");
    _alarmAnimation->retain();
    _alarmAnimation->setTag(222);
    
    _removeSmoke = SpriteAnimation::create("effects/virpulis.plist");
    _removeSmoke->retain();
    
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
        _leftUpAnimation->setScale(GLOBAL_SCALE);
        _leftDownAnimation->setScale(GLOBAL_SCALE);
        _leftAnimation->setScale(GLOBAL_SCALE);
        _upAnimation->setScale(GLOBAL_SCALE);
        _downAnimation->setScale(GLOBAL_SCALE);
        
        _removeSmoke->setScale(GLOBAL_SCALE);
        _alarmAnimation->setScale(GLOBAL_SCALE);
        _crashAnimation->setScale(GLOBAL_SCALE);
    }
    
    _TargetIcon = CCSprite::create("dwarf_freeze.png");
    _TargetIcon->setPosition(ccp(5,0));
    _TargetIcon->setVisible(false);
    addChild(_TargetIcon,100);
    
//    setPowerButton(0);
	
	return true;
}

void Dwarf::onEnter()
{	
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void Dwarf::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}

void Dwarf::UpdateBagIcon()
{
    return;
    
    //No icon here
    if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission
       || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
       || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission)
    {
        return;
    }
    
    if(mCollectedCrystals>0){
        if(_BagIcon_1->isVisible()){
            _BagIcon_1->setVisible(false);
        }
        
        if(_BagIcon_2->isVisible()==false){
            _BagIcon_2->setVisible(true);
        }
        
        if(_BagIcon_3->isVisible()){
            _BagIcon_3->setVisible(false);
        }
    }
    
    if(mCollectedCrystals==0){
        if(_BagIcon_1->isVisible()==false){
            _BagIcon_1->setVisible(true);
        }
        
        if(_BagIcon_2->isVisible()){
            _BagIcon_2->setVisible(false);
        }
        
        if(_BagIcon_3->isVisible()){
            _BagIcon_3->setVisible(false);
        }
    }
    
    if(mCollectedCrystals>=mCollectedCrystalsMax){
        if(_BagIcon_1->isVisible()){
            _BagIcon_1->setVisible(false);
        }
        
        if(_BagIcon_2->isVisible()){
            _BagIcon_2->setVisible(false);
        }
        
        if(_BagIcon_3->isVisible()==false){
            _BagIcon_3->setVisible(true);
        }
    }
}

void Dwarf::ClearOldTraps()
{
    if(getTag()==999)
    {
        mCollectedCrystals = 0;
        mCrystalPoints = 0;
        
        mNewMissionTimeout = 5;
        
        _disabled = false;
        _knockOut = false;
        
        //Remove the alarm or crash
        if (_animation->getChildByTag(111))
        {
            _animation->removeChild(_crashAnimation,true);
            _crashAnimation->removeFromParent();
            _animation->resume();
        }
        
        //Check all the rest too !!!
        if(_leftAnimation->getChildByTag(111)){
            _leftAnimation->removeChild(_crashAnimation,true);
            _crashAnimation->removeFromParent();
            _leftAnimation->resume();
        }
        if(_leftDownAnimation->getChildByTag(111)){
            _leftDownAnimation->removeChild(_crashAnimation,true);
            _crashAnimation->removeFromParent();
            _leftDownAnimation->resume();
        }
        if(_leftUpAnimation->getChildByTag(111)){
            _leftUpAnimation->removeChild(_crashAnimation,true);
            _crashAnimation->removeFromParent();
            _leftUpAnimation->resume();
        }
        if(_upAnimation->getChildByTag(111)){
            _upAnimation->removeChild(_crashAnimation,true);
            _crashAnimation->removeFromParent();
            _upAnimation->resume();
        }
        if(_downAnimation->getChildByTag(111)){
            _downAnimation->removeChild(_crashAnimation,true);
            _crashAnimation->removeFromParent();
            _downAnimation->resume();
        }
        
        //The alarm
        if (getChildByTag(222) && _alarmAnimation->isVisible())
        {
            _canPlayAlarm = false;
            _alarmWasSet = false;
            _alarmAnimation->setVisible(false);
        }
    }
}

void Dwarf::update(float delta)
{
    mTimeInMap+=delta;
    
    if (_disabled)
    {
        //Activate dwarf when active
        if(User::getInstance()->mNewMissionBuild){
            if(getTag()==999)
            {
                mNewMissionTimeout-=delta;
                if(mNewMissionTimeout<=0)
                {
                    mCollectedCrystals = 0;
                    mCrystalPoints = 0;
                    
                    mNewMissionTimeout = 5;
                    
                    _disabled = false;
                    _knockOut = false;
                    
                    if(_direction == 1){setAngle(4.3);}
                    else if(_direction == 2){setAngle(3.5);}
                    else if(_direction == 3){setAngle(2.75);}
                    else if(_direction == 4){setAngle(1.96);}
                    else if(_direction == 5){setAngle(1.17);}
                    else if(_direction == 6){setAngle(0);}
                    else if(_direction == 7){setAngle(5.8);}
                    else if(_direction == 0){setAngle(5.1);}
                    
                    //Remove the alarm or crash
                    if (_animation->getChildByTag(111))
                    {
                        _animation->removeChild(_crashAnimation,true);
                        _crashAnimation->removeFromParent();
                        _animation->resume();
                    }
                    
                    //Check all the rest too !!!
                    if(_leftAnimation->getChildByTag(111)){
                        _leftAnimation->removeChild(_crashAnimation,true);
                        _crashAnimation->removeFromParent();
                        _leftAnimation->resume();
                    }
                    if(_leftDownAnimation->getChildByTag(111)){
                        _leftDownAnimation->removeChild(_crashAnimation,true);
                        _crashAnimation->removeFromParent();
                        _leftDownAnimation->resume();
                    }
                    if(_leftUpAnimation->getChildByTag(111)){
                        _leftUpAnimation->removeChild(_crashAnimation,true);
                        _crashAnimation->removeFromParent();
                        _leftUpAnimation->resume();
                    }
                    if(_upAnimation->getChildByTag(111)){
                        _upAnimation->removeChild(_crashAnimation,true);
                        _crashAnimation->removeFromParent();
                        _upAnimation->resume();
                    }
                    if(_downAnimation->getChildByTag(111)){
                        _downAnimation->removeChild(_crashAnimation,true);
                        _crashAnimation->removeFromParent();
                        _downAnimation->resume();
                    }
                    
                    //The alarm
                    if (getChildByTag(222) && _alarmAnimation->isVisible())
                    {
                        _canPlayAlarm = false;
                        _alarmWasSet = false;
                        _alarmAnimation->setVisible(false);
                    }
                }
            }
        }
        return;
    }
    
    if(_game->mTutorialEnabled)
    {
        if(_game->mTutorialStep == 21)
        {
            //Spawn the actions !!!
            _game->OnTutorialStepCompleted(21);
        }
    }
    
    if(_knockOut){
        if(_knockOutTime>0){
            //Remove time and allow to move futher !!!
            _knockOutTime-=delta;
            if(_knockOutTime<=0){
                _knockOut = false;
                _knockOutTime = 0;
                mBulletActive = false;
                
                // Remove the bullet indicator???
                if(getChildByTag(MT_BULLET_ID) != NULL){
                    removeChildByTag(MT_BULLET_ID,true);
                }
            }
        }
        return;
    }
    
    // We have some powerup
    if(mContainsPowerUp!=-1)
    {
        updateDwarfPowerZone();
    }
    
    if(_animation->getOpacity()<250)
    {
        if(_animation->getOpacity()+25>=255)
            _animation->setOpacity(255);
        else
            _animation->setOpacity(_animation->getOpacity()+25);
    }
    
	float x = getPositionX();
	float y = getPositionY();
    
    //--------------------------------------------------------------------------
    //Check if distance is greater then 100 - then can spawn next dwarf here !!!
    
    if(_SpawnID != -1)
    {
        //_lastSpawnPoints
        if (ccpDistanceSQ(_SpawnStart, getPosition()) >= 10000)
        {
            for (int i=0; i<_game->_lastSpawnPoints.size(); i++) {
                if(_game->_lastSpawnPoints[i] == _SpawnID){
                    //Remove this point !!! But how???
                    _game->_lastSpawnPoints.erase(_game->_lastSpawnPoints.begin()+i);
                    break;
                }
            }
            
            _SpawnID = -1;//Remove it
//            _game->_lastSpawnPoints.erase(1);
        }
    }
    
    //--------------------------------------------------------------------------
	
	if (_effect)
	{
		switch (_effect->getType())
		{
			case EFFECT_TYPE_WEB:
				_speed = 0.0f;
				break;
			case EFFECT_TYPE_ICE_BARRAGE:
				_speed = 0.0f;
				break;
			default:
				break;
		}
        
        if (_effect->getNeedToRemove())
		{
            removeEffect();
		}
	}
	else
	{
        if (_type==DWARF_TYPE_FAT)
            _speed = (_defaultSpeed * _game->getGameSpeed());//*mCrystalSlowDown;
        else
            _speed = (_defaultSpeed * _game->getGameSpeed());//*mCrystalSlowDown;
        
        if(_PoisonTime>0){
            _PoisonTime-=delta;
            if(_PoisonTime<=0){
                _PoisonTime = 0;
                //Remove the poison fx
                
            }
            
            _speed *= _PoisonSlowValue;
        }
        
        // If totem wants to slow down - do it !!!
        _speed*=mTotemSlow;
		
		if (_movePoints->count() > 0)
		{
			CCPoint point = _movePoints->getControlPointAtIndex(0);
			
			if (ccpDistanceSQ(point, getPosition()) <= 1000)
			{
				_movePoints->removeControlPointAtIndex(0);
				
				if (_line->getChildrenCount()>0) _line->removeChildAtIndex(0, false);
			}
			else
			{
				setAngle(atan2f(point.y - y, point.x - x));
			}
		}
		
		CCPoint newPosition = ccp(x + cosf(_angle) * delta * _speed,
								  y + sinf(_angle) * delta * _speed);
		
        //If is connected - then allow to hit it correct
		if (!_game->getMask(newPosition.x, newPosition.y))
		{
			setPosition(newPosition);
		}
		else
		{
            if (_isConnectedToCave)
            {
                //Continue
                setPosition(newPosition);
                
                
            }
            else
            {
                removeMovePoints();
                //Create side move !!!
                onMaskHit();
            }
		}
		
		_line->setPosition(-newPosition.x, -newPosition.y - 22.0f);
	}
    
//    if (mCoolDown>0)
//        mCoolDown-=delta;
    
    bool aHasAlarm = false;
	CCObject* entry;
    if (_game->mPowerItem_GhostsActive <=0 && _game->getShieldTimer() <= 0 && !_forceRemove)//If booster is active they cant colide
    {
        CCARRAY_FOREACH(_game->getDwarves(), entry)
        {
            Dwarf* otherDwarf = (Dwarf*)entry;
            
            if (otherDwarf!=this && !otherDwarf->getDisabled())
            {
                //The alamr stuff
                if (ccpDistanceSQ(getPosition(), otherDwarf->getPosition())<=powf(RADIUS,3) * GLOBAL_SCALE)
                {
                    if (!otherDwarf->_alarmWasSet)
                    {
                        otherDwarf->_canPlayAlarm = true;
                        otherDwarf->_alarmWasSet = true;
                    }
                    
                    aHasAlarm = true;
                    
                    if(User::getInstance()->_tutorial_small_3==0)
                    {
                        User::getInstance()->finishTutorial_Small_3();
                        
                        //1st time pause the game and show some stuff !!!
                        _game->stopInGameSound("Footsteps",true);
                        _game->stopInGameSound("troll_walk",true);
                        _game->stopInGameSound("dwarf_web_stuck",true);
                        
                        //Get the mid point !!!
                        int aShowX = getPositionX()-(getPositionX()-otherDwarf->getPositionX())*0.5f;
                        int aShowY = getPositionY()-(getPositionY()-otherDwarf->getPositionY())*0.5f;
                        
                        _game->CreateSmallTutorial(3,aShowX,aShowY);
                    }
                }
                
                //The danger !!!
                if (ccpDistanceSQ(getPosition(), otherDwarf->getPosition())<=powf(RADIUS + RADIUS, 2) * GLOBAL_SCALE)
                {
                    // New check
                    //Crash !!!
                    _disabled = true;
                    otherDwarf->setDisabled(true);
                    
                    createCrash();
                    
                    doDwarfBang(_angle);
                    otherDwarf->doDwarfBang(_angle+M_PI);
                    
                    //------------------------------------------------------------
                    
                    bool otherDwarfTaked = false;
                    for(int trollIndex = _game->_trolls->count() - 1; trollIndex >= 0; --trollIndex)//To catch dwarfs, what bang.
                    {
                        Troll* troll = static_cast<Troll*>(_game->_trolls->objectAtIndex(trollIndex));
                        
                        // This one wants to catch
                        if(troll->mCatchingDwarf == false && troll->mEnemySpawnID == 3)
                        {
                            if(otherDwarfTaked == false)
                            {
                                otherDwarfTaked = true;
                                troll->CatchDwarf(otherDwarf);
                            }
                            else
                            {
                                troll->CatchDwarf(this);
                                break;
                            }
                        }
                    }
                    
                    //------------------------------------------------------------
                    
                    if (otherDwarf->getChildByTag(222))
                    {
                        _canPlayAlarm = false;
                        _alarmWasSet = false;
                        otherDwarf->getAlarmAnimation()->setVisible(false);
                    }
                    
                    otherDwarf->setTag(999);//Will skip his pause !!!
                    this->setTag(999);
                    
                    //Create score off
                    if(User::getInstance()->mSpecial_17_Mission){
                        _game->createPoints(-200, 0, this->getPosition(), ccc3(0, 232, 225));
                    }
                    else{
                        _game->createPoints(-mCrystalPoints, 0, this->getPosition(), ccc3(0, 232, 225));
                        _game->createPoints(-otherDwarf->mCrystalPoints, 0, otherDwarf->getPosition(), ccc3(0, 232, 225));
                    }
                    
                    mCollectedCrystals = 0;
                    otherDwarf->mCollectedCrystals = 0;
                    
                    mCrystalPoints = 0;
                    otherDwarf->mCrystalPoints = 0;
                    
                    UpdateBagIcon();
                    otherDwarf->UpdateBagIcon();
                    
                }
            }
        }
    }
    
    //==========================================
    //Some booster code part
    
    if (_game->mPowerItem_GhostsActive > 0 || _game->getShieldTimer() > 0 || _PoisonTime>0)
    {
        //Set alpha for now 50%
        if(_animation && _animation->getOpacity()!=220)
        {
//            _animation->setColor(ccc3(0, 124, 220));
            _animation->setColor(ccc3(0, 164, 255));
            
            ccBlendFunc* someBlend = new ccBlendFunc();
            someBlend->src = GL_ONE;//OGLES.GL_ONE;
            someBlend->dst = GL_ONE;//OGLES.GL_ONE;
            _animation->setBlendFunc(*someBlend);// .BlendFunc = someBlend;
            
            _animation->setOpacity(254);
        }
    }
    else
    {
        //They should be visible at 100%
        if(_animation && _animation->getOpacity()==254)
        {
            ccBlendFunc* someBlend = new ccBlendFunc();
            someBlend->src = GL_SRC_ALPHA;//OGLES.GL_ONE;
            someBlend->dst = GL_ONE_MINUS_SRC_ALPHA;//OGLES.GL_ONE;
            _animation->setBlendFunc(*someBlend);// .BlendFunc = someBlend;
            
            _animation->setColor(ccc3(255,255,255));
            _animation->setOpacity(255);
        }
    }
    
    //==========================================
    
    if ((!aHasAlarm && !_alarmTroll && !_alarmDwarf) || _disabled)
    {
        //Remove alarm for this dwarf
        if (getChildByTag(222) && _alarmAnimation->isVisible())
        {
            _canPlayAlarm = false;
            _alarmWasSet = false;
            _alarmAnimation->setVisible(false);
        }
    }
    else
    {
        //Alarm !!!
        if (!getChildByTag(222))
        {
            addChild(_alarmAnimation);
            if (_canPlayAlarm)
			{
                _game->playInGameSound("dwarf_alarm");
			}
        }
        
        if (!_alarmAnimation->isVisible())
        {
            _alarmAnimation->setVisible(true);
            if (_canPlayAlarm)
			{
                _game->playInGameSound("dwarf_alarm");
			}
        }
    }
}

void Dwarf::onMaskHit()
{
    //Try to check one of the bounce areas
    float aNewAngle = _angle + M_PI;
    
    CCPoint aResultL = ccp(0,0);
    CCPoint aResultR = ccp(0,0);
    
    int aSearchX;
    int aSearchY;
    
    float aX = getPositionX();
    float aY = getPositionY();
    
    int aStartIndex = 0;
    
    //GET THE START BOUNCE
    if (_angle>=5.8875 || _angle<=0.3925)//Left
        aStartIndex = 0;
    else if (_angle>0.3925 && _angle<=1.1775)//Left Up
        aStartIndex = 1;
    else if (_angle>1.1775 && _angle<=1.9625)//Up
        aStartIndex = 2;
    else if (_angle>1.9625 && _angle<=2.7475)//Right Up
        aStartIndex = 3;
    else if (_angle>2.7475 && _angle<=3.5325)// Right
        aStartIndex = 4;
    else if (_angle>3.5325 && _angle<=4.3175)// Right Down
        aStartIndex = 5;
    else if (_angle>4.3175 && _angle<=5.1025)// Down
        aStartIndex = 6;
    else if (_angle>5.1025)// Left down
        aStartIndex = 7;
    
    int aLeftArr[6];
    int aRightArr[6];
    
    if (aStartIndex==0 || aStartIndex==8)
    {
        int a[6] = {5,5,0,10,-5,5};
        std::copy(a, a+6, aLeftArr);
//        free(a);
        
        int b[6] = {5,-5,0,-10,-5,-5};
        std::copy(b, b+6, aRightArr);
    }
    else if (aStartIndex == 1)
    {
        int a[6] = {0,10,-5,5,-10,0};
        std::copy(a, a+6, aLeftArr);
        
        int b[6] = {10,0,5,-5,0,-10};
        std::copy(b, b+6, aRightArr);
    }
    else if (aStartIndex == 2)
    {
        int a[6] = {-5,5,-10,0,-5,-5};
        std::copy(a, a+6, aLeftArr);
        
        int b[6] = {5,5,10,0,5,-5};
        std::copy(b, b+6, aRightArr);
    }
    else if (aStartIndex == 3)
    {
        int a[6] = {-10,0,-5,-5,0,-10};
        std::copy(a, a+6, aLeftArr);
        
        int b[6] = {0,10,5,5,10,0};
        std::copy(b, b+6, aRightArr);
    }
    else if (aStartIndex == 4)
    {
        int a[6] = {-5,5,0,10,5,5};
        std::copy(a, a+6, aLeftArr);
        
        int b[6] = {-5,-5,0,-10,5,-5};
        std::copy(b, b+6, aRightArr);
    }
    else if (aStartIndex == 5)
    {
        int a[6] = {-10,0,-5,5,0,10};
        std::copy(a, a+6, aLeftArr);
        
        int b[6] = {0,-10,5,-5,10,0};
        std::copy(b, b+6, aRightArr);
    }
    else if (aStartIndex == 6)
    {
        int a[6] = {-5,-5,-10,0,-5,5};
        std::copy(a, a+6, aLeftArr);
        
        int b[6] = {5,-5,10,0,5,5};
        std::copy(b, b+6, aRightArr);
    }
    else if (aStartIndex == 7)
    {
        int a[6] = {0,-10,-5,-5,-10,0};
        std::copy(a, a+6, aLeftArr);
        
        int b[6] = {10,0,5,5,0,10};
        std::copy(b, b+6, aRightArr);
    }
    
    bool aLeftSideFirst = false;
    int aLeftFirst = rand()%2;
    if (aLeftFirst == 1)
        aLeftSideFirst = true;
    
    //Checks bouth sides and takes the free one !!!
    for (int i=0; i<3; i++)
    {
        aResultL = ccp(aLeftArr[i*2],aLeftArr[i*2+1]);
        aResultR = ccp(aRightArr[i*2],aRightArr[i*2+1]);
        
        //The search
        if (aLeftSideFirst)
        {
            aSearchX = aX+aResultL.x;
            aSearchY = aY+aResultL.y;
        }
        else
        {
            aSearchX = aX+aResultR.x;
            aSearchY = aY+aResultR.y;
        }
        
        if (!_game->getMask(aSearchX, aSearchY))
        {
            aNewAngle = atan2f(aSearchY - aY, aSearchX - aX);
            break;
        }
        
        if (aLeftSideFirst)
        {
            aSearchX = aX+aResultR.x;
            aSearchY = aY+aResultR.y;
        }
        else
        {
            aSearchX = aX+aResultL.x;
            aSearchY = aY+aResultL.y;
        }
        
        if (!_game->getMask(aSearchX, aSearchY))
        {
            aNewAngle = atan2f(aSearchY - aY, aSearchX - aX);
            break;
        }
    }
    
    setAngle(aNewAngle);
}

void Dwarf::removeFromSave()
{
    setAnimation(_removeSmoke);
    //Add timer to remove from game after 2sec
    CCActionInterval* aRemoveDelay = CCDelayTime::create(0.5f);
    CCCallFunc* aRemoveCall = CCCallFuncN::create(this, callfuncN_selector(Dwarf::setForceRemove));
    CCSequence* aRemoveTrollSeq = CCSequence::create(aRemoveDelay,aRemoveCall,NULL);
    
    runAction(aRemoveTrollSeq);
}

void Dwarf::setForceRemove()
{
    _forceRemove = true;
}

void Dwarf::removeFromTroll()
{
    _BagIcon_1->setVisible(false);
    _BagIcon_2->setVisible(false);
    _BagIcon_3->setVisible(false);
    
    setAnimation(_removeSmoke);
    //Add timer to remove from game after 2sec
    CCActionInterval* aRemoveDelay = CCDelayTime::create(0.5f);
    CCCallFunc* aRemoveCall = CCCallFuncN::create(this, callfuncN_selector(Dwarf::removeThisDwarf));
    CCSequence* aRemoveTrollSeq = CCSequence::create(aRemoveDelay,aRemoveCall,NULL);
    
    runAction(aRemoveTrollSeq);
}

void Dwarf::removeThisDwarf()
{
    _game->removeNode(this);
}

void Dwarf::ClearAlarm()
{
    if (getChildByTag(222) && _alarmAnimation->isVisible())
    {
        _canPlayAlarm = false;
        _alarmWasSet = false;
        _alarmAnimation->setVisible(false);
    }
}

void Dwarf::clearCrash(bool theSameDirection)
{
    _disabled = false;//Continues to move in the same or oposite direction !!!
    
    if (getChildByTag(222) && _alarmAnimation->isVisible())
    {
        _canPlayAlarm = false;
        _alarmWasSet = false;
        _alarmAnimation->setVisible(false);
    }
    
//    if (getChildByTag(111))
//        removeChild(_crashAnimation);
    if (_animation->getChildByTag(111))
    {
        _animation->removeChild(_crashAnimation,true);
        _crashAnimation->removeFromParent();
    }
    
    //Clear the anim
//    if (_animation)
//        removeChild(_animation);
    
     _animation->resumeSchedulerAndActions();
    
    //For now
    if (theSameDirection)
        setAngle(_angle);
    else
        setAngle(_angle+M_PI);
}

void Dwarf::createTrollCrash()
{
    _disabled = true;
    createCrash();
    doDwarfBang(_angle);
    
    if (getChildByTag(222) && _alarmAnimation->isVisible())
    {
        _canPlayAlarm = false;
        _alarmWasSet = false;
        _alarmAnimation->setVisible(false);
    }
    
    _game->playInGameSound("dwarf_crash");
    
//    if (getChildByTag(111))
//        removeChild(_crashAnimation);
}

void Dwarf::doDwarfBang(float theAngle)
{
    removeMovePoints();
    
    //Test for colision between dwarfs
    int aX = 0;
    int aY = 0;
    
    int aOffX = 10;
    int aOffY = 0;
    
    if (theAngle >= 15.0f * M_PI / 8.0f || theAngle < M_PI / 8.0f)
	{
		//right
        aX = -50;
        aY = 0;
        
        if (_type == DWARF_TYPE_FAT)
            aOffX = 36;
        else
            aOffX = 45;
	}
	else if (theAngle < 3.0f * M_PI / 8.0f)
	{
		//right up
        aX = -25;
        aY = -25;
        
        if (_type == DWARF_TYPE_FAT)
            aOffX = 42;
        else
            aOffX = 32;
	}
	else if (theAngle < 5.0f * M_PI / 8.0f)
	{
		//up
        aX = 0;
        aY = -50;
        
        if (_type == DWARF_TYPE_FAT)
        {
            aOffX = -5;
        }
        else
        {
            aOffX = -3;
            aOffY = 6;
        }
	}
	else if (theAngle < 7.0f * M_PI / 8.0f)
	{
		//left up
        aX = 25;
        aY = -25;
	}
	else if (theAngle < 9.0f * M_PI / 8.0f)
	{
		//left
        aX = 50;
        aY = 0;
	}
	else if (theAngle < 11.0f * M_PI / 8.0f)
	{
		//left down
        aX = 25;
        aY = 25;
	}
	else if (theAngle < 13.0f * M_PI / 8.0f)
	{
		//down
        aX = 0;
        aY = 50;
        
        if (_type == DWARF_TYPE_FAT)
        {
            aOffX = -2;
            aOffY = -4;
        }
        else
        {
            aOffX = -3;
        }
	}
	else
	{
		//right down
        aX = -25;
        aY = 25;
        
        if (_type == DWARF_TYPE_FAT)
            aOffX = 26;
        else
            aOffX = 30;
	}
    
	_animation->pauseSchedulerAndActions();

//	if (getChildByTag(111))
//		removeChild(_crashAnimation);
    if (_animation->getChildByTag(111))
    {
        _animation->removeChild(_crashAnimation,true);
        _crashAnimation->removeFromParent();
    }
    
    _animation->addChild(_crashAnimation);
//	addChild(_crashAnimation);

	//Update animation offset
//	_crashAnimation->setPosition(ccp(((_crashAnimation->getContentSize().width-_animation->getContentSize().width)/2)+aOffX,50+aOffY));
//    _crashAnimation->setPosition(ccp(50,100));
    
    if (_type == DWARF_TYPE_FAT)
    {
        switch (_direction) {
            case 4:aOffX = 50;aOffY = 95;break;
            case 0:aOffX = 46;aOffY = 110;break;
                
            case 2:aOffX = 80;aOffY = 100;break;
            case 6:aOffX = 60;aOffY = 100;break;
                
            case 1:aOffX = 80;aOffY = 110;break;
            case 3:aOffX = 75;aOffY = 95;break;
                
            case 7:aOffX = 70;aOffY = 110;break;
            case 5:aOffX = 60;aOffY = 100;break;
                
            default:
                break;
        }
    }
    else
    {
        switch (_direction) {
            case 4:aOffX = 50;aOffY = 110;break;
            case 0:aOffX = 50;aOffY = 130;break;
                
            case 2:aOffX = 110;aOffY = 100;break;
            case 6:aOffX = 60;aOffY = 100;break;
                
            case 3:aOffX = 70;aOffY = 120;break;
            case 1:aOffX = 75;aOffY = 120;break;
                
            case 5:aOffX = 60;aOffY = 115;break;
            case 7:aOffX = 60;aOffY = 115;break;
                
            default:
                break;
        }
    }
    
    _crashAnimation->setPosition(ccp(aOffX,aOffY));

	if (getChildByTag(222) && _alarmAnimation->isVisible())
    {
        _canPlayAlarm = false;
        _alarmWasSet = false;
        _alarmAnimation->setVisible(false);
    }

	//Check for safe dwarf move !!!
	if (_game->getMask(getPositionX()+aX, getPositionY()+aY))
	{
		aX = 1;
		aY = 1;
	}

	CCActionInterval* move = CCMoveBy::create(0.5, ccp(aX,aY));
	CCActionInterval* ease1 = CCEaseExponentialOut::create(move);
	runAction(ease1);

}

void Dwarf::pauseAnimation()
{
    setTag(kFreezeSprite);
    _animation->pauseSchedulerAndActions();
}

void Dwarf::resumeAnimation()
{
    setTag(1);
    _animation->resumeSchedulerAndActions();
    //Try to set the same anim !!!
    setAngle(_angle);
}

void Dwarf::setAnimationVisibility(bool theValue)
{
    if (_animation && _animation->getParent()!=NULL)
        _animation->setVisible(theValue);
}

void Dwarf::createCrash()
{
    
    
    //Create kaboom !!!
    CCSprite *aCrash = CCSprite::create("effects/dwarf_crash_boom.png");
    aCrash->setOpacity(0);
    aCrash->setScale(0);
    aCrash->setTag(999);//Pause will not impact
    
    CCActionInterval* fadeIn = CCFadeIn::create(0.20);
    CCActionInterval* fadeOut = CCFadeOut::create(0.20);
    CCActionInterval* scale = CCScaleTo::create(0.4, 1.25f,1.5f);
    CCActionInterval* delay = CCDelayTime::create(0.20);
    
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    
    CCSequence* seq1 = CCSequence::create(delay,fadeOut,func,NULL);
    CCSpawn* spawn = CCSpawn::create(scale,fadeIn,seq1,NULL);
    
    aCrash->runAction(spawn);
    
    aCrash->setPosition(ccp(getPositionX(),getPositionY()));
    _game->addChild(aCrash);
    
    _game->playInGameSound("dwarf_crash");
}

bool Dwarf::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
	bool result = false;
    
    if(_disabled){
        return false;
    }
    
    //Special stuff
    if(_game->mTutorialEnabled && ccpDistanceSQ(touch->getLocation(), getPosition()) <= PICK_RADIUS * PICK_RADIUS)
    {
        if(_game->mTutorialStep == 3)//3
        {
            if (_type == DWARF_TYPE_FAT)
            {
                _game->RemoveDrawHand();
            }
            else
            {
                //Dont allow to touch other dwarf
                return false;
            }
        }
        else if(_game->mTutorialStep == 2)//2
        {
            if (_type == DWARF_TYPE_TALL)
            {
                _game->RemoveDrawHand();
            }
            else
            {
                //Dont allow to touch other dwarf
                return false;
            }
        }
        else if(_game->mTutorialStep == 5)
        {
            if (_type == DWARF_TYPE_TALL)
            {
                _game->RemoveDrawHand();
            }
            else
            {
                //Dont allow to touch other dwarf
                return false;
            }
        }
        else if(_game->mTutorialStep == 6)
        {
            if (_type == DWARF_TYPE_FAT)
            {
                _game->RemoveDrawHand();
            }
            else
            {
                //Dont allow to touch other dwarf
                return false;
            }
        }
        else if(_game->mTutorialStep==12)
        {
            _game->RemoveDrawHand();
        }
        else if(_game->mTutorialStep==14)
        {
            _game->RemoveDrawHand();
        }
        else if(_game->mTutorialStep==24)
        {
            _game->RemoveDrawHand();
        }
        else
        {
            return false;
        }
    }
	
	if (ccpDistanceSQ(touch->getLocation(), getPosition()) <= PICK_RADIUS * PICK_RADIUS)
	{
		removeMovePoints();
		startLine();
		
		//if has effect
		if (_effect != NULL)
		{
			if (_effect->getType() == EFFECT_TYPE_ICE_BARRAGE)
			{
				_effect->tap();
				
				//if ice barrage is tapped 5 times, remove it
//				if (_effect->getTapCount() >= 5)
                if (_effect->getNeedToRemove())
				{
					removeEffect();
				}
			}
			
			result = false;
		}
		else 
		{
			//addMovePoint(touch->getLocation());
			
			result = true;
			_touchEnded = false;
		}
        
        //Highlight correct cave
        if (_type == DWARF_TYPE_FAT)
		{
            _game->highlightBigDoor(false);
		}
        else if (_type == DWARF_TYPE_TALL)
		{
            _game->highlightSmallDoor(false);
		}
	}
	
	return result;
}

void Dwarf::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(_disabled){
        return;
    }
    
	if (_touchEnded)
	{
		return;
	}
    
	
	CCPoint previousPoint = touch->getStartLocation();
	
	if (_movePoints->count() > 0)
	{
		previousPoint = _movePoints->getControlPointAtIndex(_movePoints->count() - 1);
	}
	
	// if this will be the first point, start only when it is away from pick point
	if (ccpDistanceSQ(touch->getLocation(), previousPoint) > POINT_RADIUS * POINT_RADIUS)
	{
		CCPoint position = touch->getLocation();
		
		if (addMovePoint(position, previousPoint,false))
		{
			CCPoint cavePosition = _game->getCavePoint(_type);
			
			//snapped to cave
            if (_type == DWARF_TYPE_TALL)
            {
                _game->OnCaveOrangeOpen();
                
//                CCLOG("DWARF_TYPE_TALL distance:%f",ccpDistanceSQ(cavePosition, position));
                
//                if (ccpDistanceSQ(cavePosition, position) <= 40 * 40)
                if (ccpDistanceSQ(cavePosition, position) <= TALL_SNAP_TO_CAVE && User::getInstance()->mSpecial_12_Mission==false)
                {
                    bool aDidConnectCorrect = true;
                    if(_game->mTutorialEnabled)
                    {
                        if((_game->mTutorialStep == 12 || _game->mTutorialStep == 14 || _game->mTutorialStep==24) && !_game->mTutorialFlag_1)
                        {
                            //Did not connect !!! reset all
                            aDidConnectCorrect = false;
                        }
                        
                        if(_game->mTutorialStep == 5 && _game->mTutorialFlag_1)
                            aDidConnectCorrect = false;
                    }
                    
                    if(aDidConnectCorrect)
                    {
                        if(_game->mBlockTallCave == true){
                    	crashLine();
                    	}else{
                    	addMovePoint(cavePosition, position,false);
                        _touchEnded = true;
                        connectLine();
                        vibrate();	
                    	}
                    }
                    else
                    {
                        _touchEnded = true;
                        removeMovePoints();
                    }
                }
            }
            else if (_type == DWARF_TYPE_FAT)
            {
//                CCLOG("DWARF_TYPE_FAT distance:%f",ccpDistanceSQ(cavePosition, position));
                
//                if (ccpDistanceSQ(cavePosition, position) <= 30 * 30)
                if (ccpDistanceSQ(cavePosition, position) <= FAT_SNAP_TO_CAVE && User::getInstance()->mSpecial_12_Mission==false)//1500
                {
                    bool aDidConnectCorrect = true;
                    if(_game->mTutorialEnabled)
                    {
                        if((_game->mTutorialStep == 12 || _game->mTutorialStep == 6 || _game->mTutorialStep == 14) && !_game->mTutorialFlag_1)
                        {
                            //Did not connect !!! reset all
                            aDidConnectCorrect = false;
                        }
                        else if(_game->mTutorialStep == 24 && _game->mTutorialFlag_1)
                        {
                            aDidConnectCorrect = false;
                        }
                    }
                    
                    if(aDidConnectCorrect)
                    {
                        if(_game->mBlockFatCave==true){
                    	crashLine();
                    	}else{
                    	addMovePoint(cavePosition, position,false);
                        _touchEnded = true;
                        connectLine();
                        vibrate();	
                    	}
                    }
                    else
                    {
                        _touchEnded = true;
                        removeMovePoints();
                    }
                }
            }
            
            //Check if does not want to connect to troll
            if(mContainsPowerUp!=-1)
            {
                //Check if does not want to snap to troll
//                mSnapedTroll
                
                // By default - always on
                mCanSearchForTrollsForSnap = false;
                mSnapedToMasterTroll = false;
                mSnapedToTotem = false;
                
                // By spell type - can do correct actions only !!!
                if(mContainsPowerUp == DWARF_SPELL_FREEZER || mContainsPowerUp == DWARF_SPELL_ELECTRIFY) //Freezer and Snap only for enemy trolls
                {
                    for (int trollIndex = _game->_trolls->count() - 1; trollIndex >= 0; --trollIndex)
                    {
                        Troll* troll = static_cast<Troll*>(_game->_trolls->objectAtIndex(trollIndex));
                        
                        //Little update - warning light on gnome !!!
                        if (troll->isVisible() && troll->getTouchable() && troll->getCanMove())
                        {
                            if(ccpDistanceSQ(troll->getPosition(), position) <= FAT_SNAP_TO_CAVE)
                            {
                                //Snap to troll
                                mSnapedTroll = troll;
                                
                                addMovePoint(troll->getPosition(), position,false);
                                _touchEnded = true;
                                connectLine();
                                vibrate();
                                
                                break;
                            }
                        }
                    }
                }
                else if(mContainsPowerUp >= 100) // The spell actions
                {
                    if(_game->mTotem != NULL && mSnapedToMasterTroll == false)
                    {
                        float theDistance2 = sqrtf((position.x-_game->mTotem->getPositionX())*(position.x-_game->mTotem->getPositionX()) +
                                                   (position.y-_game->mTotem->getPositionY())*(position.y-_game->mTotem->getPositionY()));
                        
                        // Get 1st item from shop?
                        if(theDistance2<_game->mCurrentMission.DEBUG_Electrify_range)
                        {
                            mSnapedToTotem = true;
                            
                            addMovePoint(_game->mTotem->getPosition(), position,false);
                            _touchEnded = true;
                            connectLine();
                            vibrate();
                        }
                    }
                }
                else
                {
                    // OTher speelllss
                    if(ccpDistanceSQ(_game->_MasterTrollBase->getPosition(), position) <= 6000)
                    {
                        mSnapedToMasterTroll = true;
                        
                        addMovePoint(_game->_MasterTrollBase->getPosition(), position,false);
                        _touchEnded = true;
                        connectLine();
                        vibrate();
                    }
                }
                
                /*
                if(_game->mDwarfCollectMachine)
                {
                    // Check if does not want to attack troll
//                    if(_game->_MasterTrollBase)
                    if(ccpDistanceSQ(_game->_MasterTrollBase->getPosition(), position) <= 6000)
                    {
                        mSnapedToMasterTroll = true;
                        
                        addMovePoint(_game->_MasterTrollBase->getPosition(), position,false);
                        _touchEnded = true;
                        connectLine();
                        vibrate();
                    }
                }
                
                if(_game->mTotem != NULL && mSnapedToMasterTroll == false)
                {
                    float theDistance2 = sqrtf((position.x-_game->mTotem->getPositionX())*(position.x-_game->mTotem->getPositionX()) +
                                               (position.y-_game->mTotem->getPositionY())*(position.y-_game->mTotem->getPositionY()));
                    
                    if(theDistance2<90)
                    {
                        mSnapedToTotem = true;
                        
                        addMovePoint(_game->mTotem->getPosition(), position,false);
                        _touchEnded = true;
                        connectLine();
                        vibrate();
                    }
                }
                
                if(_game->mDwarfCollectMachine)
                {
                    if(mSnapedToMasterTroll == false && mSnapedToTotem == false)
                    {
                        mCanSearchForTrollsForSnap = true;
                    }
                }
                
                
                if(mCanSearchForTrollsForSnap)
                {
                    for (int trollIndex = _game->_trolls->count() - 1; trollIndex >= 0; --trollIndex)
                    {
                        Troll* troll = static_cast<Troll*>(_game->_trolls->objectAtIndex(trollIndex));
                        
                        //Little update - warning light on gnome !!!
                        if (troll->isVisible() && troll->getTouchable() && troll->getCanMove())
                        {
                            if(ccpDistanceSQ(troll->getPosition(), position) <= FAT_SNAP_TO_CAVE)
                            {
                                //Snap to troll
                                mSnapedTroll = troll;
                                
                                addMovePoint(troll->getPosition(), position,false);
                                _touchEnded = true;
                                connectLine();
                                vibrate();
                                
                                break;
                            }
                        }
                    }
                }
                */
            }
		}
		else
		{
			_touchEnded = true;
            crashLine();
//            vibrate();
		}
	}
	
}

void Dwarf::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(_disabled){
        return;
    }
    
	if (!_touchEnded)
	{
		//addMovePoint(touch->getLocation());
	}
    
    //Highlight correct cave
    if (_type == DWARF_TYPE_FAT)
	{
        _game->highlightBigDoor(true);
	}
    else if (_type == DWARF_TYPE_TALL)
	{
        _game->highlightSmallDoor(true);
	}
    
    if(_game->mTutorialEnabled && !_isConnectedToCave)
    {
        if(_game->mTutorialStep<5)
        {
            removeMovePoints();
            startLine();
            
            if(_game->mTutorialStep == 2)
                _game->CreateDrawHand(2);//2
            else
                _game->CreateDrawHand(1);//1
        }
        else if(_game->mTutorialStep==5)//13
        {
            removeMovePoints();
            startLine();
            
            _game->CreateDrawHand(10);
        }
        else if(_game->mTutorialStep==6)//13
        {
            removeMovePoints();
            startLine();
            
            _game->CreateDrawHand(11);
        }
        else if(_game->mTutorialStep==12)//13
        {
            removeMovePoints();
            startLine();
            
            _game->CreateDrawHand(3);
        }
        else if(_game->mTutorialStep==14)
        {
            removeMovePoints();
            startLine();
            
            _game->CreateDrawHand(4);
        }
        else if(_game->mTutorialStep==24)
        {
            removeMovePoints();
            startLine();
            
            _game->CreateDrawHand(5);
        }
    }
}

void Dwarf::PickedSpecialObject(int theType)
{
    mSpecialObjectPicked = true;
    
    //By type add some fx maybe
    CCSprite* aDummy=CCSprite::create("Crystals/yellow_under.png");
    addChild(aDummy);
}

void Dwarf::setAngle(float value)
{
	_angle = wrapTwoPI(value);
    
    int aOffX = 0;
    int aOffY = 0;
    
    if (IsConnectedBlockTime==true && _game->mBlockFatCave==true){
    	if (_type == DWARF_TYPE_FAT)
    	{
    		removeMovePoints();
    		//_isConnectedToCave = false;
    	}
	}
	
	if (IsConnectedBlockTime==true && _game->mBlockTallCave==true){
    	if (_type == DWARF_TYPE_TALL)
    	{
		removeMovePoints();
    	//_isConnectedToCave = false;
		}
	}
    
    if (_diognalMove)
    {
        if (_angle>=5.8875 || _angle<=0.3925)//right
        {
            _direction = 2;
            
            _leftAnimation->setFlipX(true);
            _leftAnimation->setFlipY(false);
            setAnimation(_leftAnimation);
            
            if (_type == DWARF_TYPE_FAT)
                aOffX = 30;
            else
                aOffX = 40;
        }
        else if (_angle>0.3925 && _angle<=1.1775)//right up
        {
            _direction = 1;
            
            _leftUpAnimation->setFlipX(true);
            _leftUpAnimation->setFlipY(false);
            setAnimation(_leftUpAnimation);
            
            if (_type == DWARF_TYPE_FAT)
            {
                aOffX = 32;
                aOffY = 8;
            }
            else
            {
                aOffX = 16;
                aOffY = 6;
            }
        }
        else if (_angle>1.1775 && _angle<=1.9625)
        {
            _direction = 0;
            
            setAnimation(_upAnimation);
            
            if (_type == DWARF_TYPE_FAT)
            {
                aOffX = -16;
                aOffY = 10;
            }
            else
            {
                aOffX = -8;
                aOffY = 12;
            }
        }
        else if (_angle>1.9625 && _angle<=2.7475)
        {
            _direction = 7;
            
            _leftUpAnimation->setFlipX(false);
            _leftUpAnimation->setFlipY(false);
            setAnimation(_leftUpAnimation);
            
            aOffY = 10;
        }
        else if (_angle>2.7475 && _angle<=3.5325)
        {
            _direction = 6;
            
            //left
            _leftAnimation->setFlipX(false);
            _leftAnimation->setFlipY(false);
            setAnimation(_leftAnimation);
        }
        else if (_angle>3.5325 && _angle<=4.3175)
        {
            _direction = 5;
            
            //left down
            _leftDownAnimation->setFlipX(false);
            _leftDownAnimation->setFlipY(false);
            setAnimation(_leftDownAnimation);
        }
        else if (_angle>4.3175 && _angle<=5.1025)
        {
            _direction = 4;
            
            //down
            setAnimation(_downAnimation);
            
            if (_type == DWARF_TYPE_FAT)
            {
                aOffX = -14;
                aOffY = 0;
            }
            else
            {
                aOffX = -8;
                aOffY = 10;
            }
        }
        else if (_angle>5.1025)
        {
            _direction = 3;
            
            _leftDownAnimation->setFlipX(true);
            _leftDownAnimation->setFlipY(false);
            setAnimation(_leftDownAnimation);
            
            if (_type == DWARF_TYPE_FAT)
            {
                aOffX = 10;
                aOffY = 4;
            }
            else
            {
                aOffX = 12;
                aOffY = 6;
            }
        }
        
        
        /*
        if (_angle >= 15.0f * M_PI / 8.0f || _angle < M_PI / 8.0f)
        {
            //right
            _leftAnimation->setFlipX(true);
            _leftAnimation->setFlipY(false);
            setAnimation(_leftAnimation);
            
            if (_type == DWARF_TYPE_FAT)
                aOffX = 30;
            else
                aOffX = 40;
        }
        else if (_angle < 3.0f * M_PI / 8.0f)
        {
            //right up
            _leftUpAnimation->setFlipX(true);
            _leftUpAnimation->setFlipY(false);
            setAnimation(_leftUpAnimation);
            
            if (_type == DWARF_TYPE_FAT)
            {
                aOffX = 32;
                aOffY = 8;
            }
            else
            {
                aOffX = 16;
                aOffY = 6;
            }
        }
        else if (_angle < 5.0f * M_PI / 8.0f)
        {
            //up
            setAnimation(_upAnimation);
            
            if (_type == DWARF_TYPE_FAT)
            {
                aOffX = -16;
                aOffY = 10;
            }
            else
            {
                aOffX = -8;
                aOffY = 12;
            }
            
        }
        else if (_angle < 7.0f * M_PI / 8.0f)
        {
            //left up
            _leftUpAnimation->setFlipX(false);
            _leftUpAnimation->setFlipY(false);
            setAnimation(_leftUpAnimation);
            
            aOffY = 10;
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
            
            if (_type == DWARF_TYPE_FAT)
            {
                aOffX = -14;
                aOffY = 0;
            }
            else
            {
                aOffX = -8;
                aOffY = 10;
            }
        }
        else
        {
            //right down
            _leftDownAnimation->setFlipX(true);
            _leftDownAnimation->setFlipY(false);
            setAnimation(_leftDownAnimation);
            
            if (_type == DWARF_TYPE_FAT)
            {
                aOffX = 10;
                aOffY = 4;
            }
            else
            {
                aOffX = 12;
                aOffY = 6;
            }
        }
        */
    }
    else
    {
        if (_angle <= 1.57)//(_angle > 1.57 && _angle<=3.14)
        {
            //Right Up
            _leftUpAnimation->setFlipX(true);
            _leftUpAnimation->setFlipY(false);
            setAnimation(_leftUpAnimation);
            
            if (_type == DWARF_TYPE_FAT)
            {
                aOffX = 32;
                aOffY = 8;
            }
            else
            {
                aOffX = 16;
                aOffY = 6;
            }
        }
        else if (_angle <= 3.14)
        {
            //Left Up
            _leftUpAnimation->setFlipX(false);
            _leftUpAnimation->setFlipY(false);
            setAnimation(_leftUpAnimation);
            
            aOffY = 10;
        }
        else if (_angle <= 4.72)
        {
            //Left down
            _leftDownAnimation->setFlipX(false);
            _leftDownAnimation->setFlipY(false);
            setAnimation(_leftDownAnimation);
        }
        else
        {
            //Right down
            _leftDownAnimation->setFlipX(true);
            _leftDownAnimation->setFlipY(false);
            setAnimation(_leftDownAnimation);
            
            if (_type == DWARF_TYPE_FAT)
            {
                aOffX = 10;
                aOffY = 4;
            }
            else
            {
                aOffX = 12;
                aOffY = 6;
            }
        }
    }
	
    //Hardcode for now - while animations offsets have glich
    if (_alarmAnimation)
        _alarmAnimation->setPosition(ccp(((_alarmAnimation->getContentSize().width-_animation->getContentSize().width)/2)+aOffX,100+aOffY));
    
    //Check here anim
}

void Dwarf::setAnimation(SpriteAnimation* animation)
{
	if (_animation != animation || !_animation)
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
        
//        //DEbug
//        if (_animation->getChildByTag(111))
//            _animation->removeChild(_crashAnimation,true);
//        
//        _crashAnimation->removeFromParent();
//        
//        _animation->addChild(_crashAnimation);
//        
//        int aOffX = 0;
//        int aOffY = 0;
//        
//        CCLog("mDirection:%i",mDirection);
//        
//        if (_type == DWARF_TYPE_FAT)
//        {
//            switch (mDirection) {
//                case 4:aOffX = 50;aOffY = 95;break;
//                case 0:aOffX = 50;aOffY = 110;break;
//                    
//                default:
//                    break;
//            }
//            
//            if (aOffX!=0 && aOffY!=0)
//                _crashAnimation->setPosition(ccp(aOffX,aOffY));
//            else
//                _crashAnimation->setPosition(ccp(60,100));
//        }
//        else
//        {
//            switch (mDirection) {
//                case 4:aOffX = 52;aOffY = 110;break;
//                case 0:aOffX = 50;aOffY = 160;break;
//                    
//                case 2:aOffX = 110;aOffY = 100;break;
//                case 6:aOffX = 50;aOffY = 100;break;
//                    
//                default:
//                    break;
//            }
//            
//            if (aOffX!=0 && aOffY!=0)
//                _crashAnimation->setPosition(ccp(aOffX,aOffY));
//            else
//                _crashAnimation->setPosition(ccp(110,100));
//        }
        
//            _animation->removeChild(_crashAnimation,true);
//        
//        _animation->addChild(_crashAnimation);
//        _crashAnimation->setPosition(ccp(50,100));
	}
    
    if(_disabled && _animation!=NULL)
    {
        _animation->setOpacity(0);
    }
}

void Dwarf::setEffect(Effect* value)
{
	if (_effect)
	{
		_effect->release();
		removeChild(_effect);
	}
	
	_effect = value;
	
	if (_effect)
	{
		removeMovePoints();
		
		_effect->retain();
        
        _game->stopInGameSound("Footsteps");
		
		switch (_effect->getType())
		{
			case EFFECT_TYPE_ICE_BARRAGE:
                _game->playInGameSound("dwarf_freeze");
				break;
			case EFFECT_TYPE_WEB:
                _game->playInGameSound("dwarf_web_enter");
				break;
			default:
				break;
		}
		
        if(_effect->getType() == EFFECT_TYPE_WIND)
        {
            
        }
        else
        {
            _effect->setPosition(0.0f, 0.0f);
            addChild(_effect);
        }
	}
}

void Dwarf::removeEffect()
{
//	unschedule(schedule_selector(Dwarf::removeWeb));
	
	if (_effect)
	{
        bool aNeedToResume = true;
        if (_effect->getType()==EFFECT_TYPE_WEB)
            aNeedToResume = false;
//        else if (_effect->getType() == EFFECT_TYPE_ICE_BARRAGE)
//            _game->playInGameSound("dwarf_unfreeze");
        
		_effect->release();
		removeChild(_effect);
		_effect = NULL;
        
		//set beck the animation
        if (aNeedToResume)
		{
            resumeAnimation();
		}
        else
		{
            setAngle(_angle);
		}
        
        if(!_game->mTutorialEnabled)
            _game->playInGameSound("Footsteps");
	}
}

bool Dwarf::addMovePoint(const cocos2d::CCPoint& point, const cocos2d::CCPoint& previousPoint, bool ingoreTexture)
{
	CCPoint diff = point - previousPoint;
		
	float angle = atan2f(diff.y, diff.x);
	
	float dx = 20.0f * cosf(angle);
	float dy = 20.0f * sinf(angle);
	
	float x = previousPoint.x + dx;
	float y = previousPoint.y + dy;
    
    if(_game->mTutorialEnabled)
    {
        //Todo check diamond or crystal reach
        if(_game->mTutorialStep<5)
        {
            if(_line->getChildren()->count()>20)
            {
                if(_game->mTutorialStep == 2)
                    _game->CreateDrawHand(2);//2
                else
                    _game->CreateDrawHand(1);//1
                return false;
            }
        }
        else if(_game->mTutorialStep==5)
        {
            if(_line->getChildren()->count()>20)
            {
                _game->CreateDrawHand(10);
                return false;
            }
        }
        else if(_game->mTutorialStep==6)
        {
            if(_line->getChildren()->count()>28)//22
            {
                _game->CreateDrawHand(11);
                return false;
            }
        }
        else if(_game->mTutorialStep==12)
        {
            if(_line->getChildren()->count()>50)
            {
                _game->CreateDrawHand(3);
                return false;
            }
        }
        else if(_game->mTutorialStep==14)
        {
            if(_line->getChildren()->count()>28)
            {
                _game->CreateDrawHand(4);
                return false;
            }
        }
        else if(_game->mTutorialStep==24)
        {
            if(_line->getChildren()->count()>30)
            {
                _game->CreateDrawHand(5);
                return false;
            }
        }
    }
	
	while (true)
	{
		if (dx >= 0.0f && x >= point.x) return true;
		if (dy >= 0.0f && y >= point.y) return true;
		if (dx < 0.0f && x <= point.x) return true;
		if (dy < 0.0f && y <= point.y) return true;
		
        //For now disabled cave mask check
		if (_game->getMask(x, y))
		{
			return false;
		}
        
        if(_game->mTutorialEnabled)
        {
            if(_game->mTutorialStep == 5)
            {
                if(_game->getTutorialMask(x,y))
                {
                    _game->mTutorialFlag_1 = true;//Collected crystal
                }
            }
            if(_game->mTutorialStep == 6)
            {
                if(_game->getTutorialMask(x,y))
                {
                    _game->mTutorialFlag_1 = true;//Collected crystal
                }
            }
            if(_game->mTutorialStep == 12)
            {
                if(_game->getTutorialMask(x,y))
                {
                    _game->mTutorialFlag_1 = true;//Collected crystal
                }
            }
            else if(_game->mTutorialStep == 14)
            {
                if(_game->getTutorialMask(x,y))
                {
                    _game->mTutorialFlag_1 = true;//Collected crystal
                }
            }
            else if(_game->mTutorialStep == 24)
            {
                if(_game->getTutorialMask(x,y))
                {
                    _game->mTutorialFlag_1 = true;//Collected crystal
                }
            }
        }
		
		_movePoints->addControlPoint(ccp(x, y));
		
		CCSprite* dot = CCSprite::create("trajectory_dot_white.png");
        
        // Check if line has the same texture !!!
        if(ingoreTexture)
        {
            //Get texture by the same name !!!
            
//            dot = CCSprite::cre
            if(_line->getTexture()->getName() != dot->getTexture()->getName()){
                
                //Possible this is yellow dot
                dot = CCSprite::create("trajectory_dot_yellow.png");
                if(_line->getTexture()->getName() != dot->getTexture()->getName()){
                    return false;
                }
            }
        }
        else
        {
            if(_line->getTexture()->getName() != dot->getTexture()->getName()){
                return false;// Stop it
            }
        }
        
        
//		dot->setPosition(ccp(x, y));
        //Add some fx
        dot->setOpacity(0);
        dot->setPosition(ccp(x,y+30));
        CCMoveTo* aActionMove = CCMoveTo::create(0.1f,ccp(x,y));
        
        CCScaleTo* aActionScale_1 = CCScaleTo::create(0.05f,1.5f);
        CCScaleTo* aActionScale_2 = CCScaleTo::create(0.05f,1.0f);
        CCDelayTime* aActionDelay = CCDelayTime::create(0.05f);
        
        CCFadeIn* aActionFade = CCFadeIn::create(0.01f);
        
        CCSequence* aActionSeq1 = CCSequence::create(aActionDelay,aActionScale_1,aActionScale_2,NULL);
        CCSequence* aActionSeq2 = CCSequence::create(aActionMove,aActionSeq1,NULL);
        CCSpawn* aActionSpawn = CCSpawn::create(aActionFade,aActionSeq2,NULL);
//        CCSpawn* aActionSpawn = CCSpawn::create(aActionMove,aActionSeq1,NULL);
        dot->setTag(mLineDotIndex);
        mLineDotIndex++;
        dot->runAction(aActionSpawn);
        
		_line->addChild(dot);
		
		x += dx;
		y += dy;
	}
	
	return true;
}

void Dwarf::removeMovePoints()
{
    if(mSnapedTroll!=NULL){
        mSnapedTroll = NULL;
    }
    
	// remove all control points
	while (_movePoints->count() != 0)
	{
		_movePoints->removeControlPointAtIndex(0);
	}
    
    if(_game->mTutorialEnabled)
    {
        _game->mTutorialFlag_1 = false;//Collected crystal
        _game->mTutorialFlag_2 = false;//Collected crystal
    }
    
    mLineDotIndex = 1;
    
	_line->removeAllChildrenWithCleanup(false);
    _isConnectedToCave = false;
}

void Dwarf::startLine()
{
	_line->setTexture(CCTextureCache::sharedTextureCache()->addImage("trajectory_dot_white.png"));
    _isConnectedToCave = false;
    IsConnectedBlockTime = false;
}

void Dwarf::connectLine()
{
    //If this is tutorial then check if crystal collect
    //Make some boom back !!!
    CCSprite* aDummy;
    CCScaleTo* aScale1;
    CCScaleTo* aScale2;
    
    CCDelayTime* aScaleDelay1;
    CCDelayTime* aScaleDelay2;
    
    CCMoveTo* aMove1;
    CCMoveTo* aMove2;
    
    CCSequence* aSeq1;
    CCSequence* aSeq2;
    
    CCSpawn* aSpaw1;
    
//    for(int i=0;i<_line->getChildrenCount();i++)
//    for(int i = mLineDotIndex;i>0;i--)
    /*
    float aTimer = _line->getChildrenCount()*0.05;
    
    for(int i=0;i<_line->getChildrenCount();i++)
    {
        CCLog("i:%i",i);
        aDummy = (CCSprite*)_line->getChildren()->objectAtIndex(i);
//        aDummy = (CCSprite*)_line->getChildByTag(i);
        if(aDummy == NULL)
            continue;
//        aDummy->stopAllActions();
        
        aScale1 = CCScaleTo::create(0.2f,1.5f);
        aScale2 = CCScaleTo::create(0.2f,1.2f);
//        aScaleDelay1 = CCDelayTime::create(1-(i*0.05f+0.02f));
        aScaleDelay1 = CCDelayTime::create(aTimer-(i*0.05f));
        
//        aScaleDelay2 = CCDelayTime::create(1-(i*0.05f));
        aScaleDelay2 = CCDelayTime::create(aTimer-(i*0.05f));
        aMove1 = CCMoveTo::create(0.1f,ccp(aDummy->getPositionX(),aDummy->getPositionY()+10));
        aMove2 = CCMoveTo::create(0.1f,ccp(aDummy->getPositionX(),aDummy->getPositionY()));
        
        aSeq2 = CCSequence::create(aScaleDelay2,aMove1,aMove2,NULL);
        aSeq1 = CCSequence::create(aScaleDelay1,aScale1,aScale2,NULL);
        
        aSpaw1 = CCSpawn::create(aSeq1,aSeq2,NULL);
        
        aDummy->runAction(aSpaw1);
    }
    */
    
	_line->setTexture(CCTextureCache::sharedTextureCache()->addImage("trajectory_dot_yellow.png"));
	_isConnectedToCave = true;
	IsConnectedBlockTime = true;
//	CCLOG("Connected");
    
    //Special stuff
    if(_game->mTutorialEnabled)
    {
        if(_game->mTutorialStep == 2)
            _game->OnTutorialStepCompleted(3);
        else if(_game->mTutorialStep == 3)
            _game->OnTutorialStepCompleted(4);
        else if(_game->mTutorialStep == 5)
            _game->OnTutorialStepCompleted(6);
        else if(_game->mTutorialStep == 6)
            _game->OnTutorialStepCompleted(7);
        else if(_game->mTutorialStep ==12)
            _game->OnTutorialStepCompleted(12);
        else if(_game->mTutorialStep == 14)
            _game->OnTutorialStepCompleted(14);
        else if(_game->mTutorialStep == 24)
            _game->OnTutorialStepCompleted(24);
    }
}

void Dwarf::crashLine()
{
    _line->setTexture(CCTextureCache::sharedTextureCache()->addImage("trajectory_dot_red.png"));
    _isConnectedToCave = false;
}

void Dwarf::removeWeb(float delta)
{
	removeEffect();
}

void Dwarf::setDisabled(bool value)
{
	_disabled = value;
}

// Actions that can be set to the dwarf
void Dwarf::setAction(int theType)
{
    if(theType == MASTER_ACTION_BULLET_POISON)
    {
        _PoisonTime = 5.0f;
        _PoisonSlowValue = 0.5f;
    }
    else if(theType == MASTER_ACTION_BULLET)
    {
        _knockOutTime = 3;
        _knockOut = true;
        createCrash();
    }
    else if(theType == MASTER_ACTION_BULLET_ICE)
    {
        _game->FreezeDwarfTotal(this);
    }
    else if(theType == MASTER_ACTION_BULLET_STRAIGHT || theType == MASTER_ACTION_BULLET_SPLIT_UP || theType == MASTER_ACTION_BULLET_ONE_LINE || theType == MASTER_ACTION_BULLET_DECOMPOSE_BEGINNING || theType == MASTER_ACTION_BULLET_ZIGZAG || theType == MASTER_ACTION_BULLET_SPLIT_MIDDLE)
    {
        removeFromSave();
    }
}

void Dwarf::FireBulletAtTroll(int thePowerID)
{
    CCSprite* aDummyBullet = CCSprite::create("small_dot_blue.png");
    aDummyBullet->setPosition(ccp(getPositionX(),getPositionY()));
    
    //Move bullet to troll
    CCMoveTo* aMoveAction;
    
    if(mSnapedToMasterTroll)
    {
        aMoveAction = CCMoveTo::create(0.5f,ccp(_game->_MasterTrollBase->getPositionX(),_game->_MasterTrollBase->getPositionY()));
    }
    else if(mSnapedToTotem)
    {
        aMoveAction = CCMoveTo::create(0.5f,ccp(_game->mTotem->getPositionX(),_game->mTotem->getPositionY()));
    }
    else
    {
        aMoveAction = CCMoveTo::create(0.5f,ccp(mSnapedTroll_FallBack->getPositionX(),mSnapedTroll_FallBack->getPositionY()));
    }
    
    CCCallFuncN* aFunction = CCCallFuncN::create(this, callfuncN_selector(Dwarf::OnFireBulletHitTroll));
    CCSequence* aSequence = CCSequence::create(aMoveAction,aFunction,NULL);
    aDummyBullet->setTag(thePowerID);
    aDummyBullet->runAction(aSequence);
    
    // Add particle for fx
    CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/bullet_part.plist");
    p->setPosition(ccp(aDummyBullet->getContentSize().width/2,aDummyBullet->getContentSize().height/2));
    p->setAutoRemoveOnFinish(true);
    aDummyBullet->addChild(p,-1);
    
    _game->addChild(aDummyBullet);
}

void Dwarf::OnFireBulletHitTroll(CCNode* sender)
{
    CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/bullet_explode.plist");
    p->setPosition(ccp(sender->getPositionX(),sender->getPositionY()));
    p->setAutoRemoveOnFinish(true);
    
    if(mSnapedToMasterTroll)
    {
        _game->OnAttackHitTroll(NULL);
    }
    else if(mSnapedToTotem)
    {
        // New stuff - check by item powa
        _game->OnAttackHitTotem(ccp(getPositionX(),getPositionY()),mSpellForAttack);
    }
    else
    {
        if(sender->getTag() == 0)
        {
            mSnapedTroll_FallBack->removeFromSave();
            if(_game->mKillTrollsAmountLeft>0){
                _game->mKillTrollsAmountLeft-=1;
                if(_game->mKillTrollsAmountLeft <= 0){
                    // Win win
                    _game->showWinScreen();
                }
            }
        }
        else if(sender->getTag() == 1)
        {
            // Do all the magic here
            mSnapedTroll_FallBack->mFreezedTime = 10;//Get from missions some param?
            
            // Add for now blue troll FX
            mSnapedTroll_FallBack->_animation->setColor(ccc3(0, 164, 255));
            
            // Play some attack sound
            _game->playInGameSound("dwarf_crash");
        }
    }
    
    // Remove old particle
    _game->removeChild(sender, true);
    
    // Add new particle
    _game->addChild(p,1000);
    
    // No more snaped trolls
    mSnapedTroll_FallBack = NULL;
    
    // Finish
    mSnapedToMasterTroll = false;
    mSnapedToTotem = false;
}

// The dull snap crap
void Dwarf::updateDwarfPowerZone()
{
    if(mContainsPowerUp == -1){
        return;// No need to go futher
    }
    
    if(mSnapedToTotem)
    {
        float theDistance2 = sqrtf((getPositionX()-_game->mTotem->getPositionX())*(getPositionX()-_game->mTotem->getPositionX()) +
                                   (getPositionY()-_game->mTotem->getPositionY())*(getPositionY()-_game->mTotem->getPositionY()));
//        if(theDistance2 <= 220)
//        if(theDistance2 <= _game->mCurrentMission.DEBUG_Electrify_range)
        if(theDistance2 <= mSpellForAttack.range)
        {
            FireBulletAtTroll(mContainsPowerUp);
            
            mContainsPowerUp = -1;
            
            if(mPowerUpIcon != NULL){
                removeChild(mPowerUpIcon);
            }
            
            return;
        }
    }
    
    if(mSnapedToMasterTroll)
    {
        float theDistance2 = sqrtf((getPositionX()-_game->_MasterTrollBase->getPositionX())*(getPositionX()-_game->_MasterTrollBase->getPositionX()) +
                                   (getPositionY()-_game->_MasterTrollBase->getPositionY())*(getPositionY()-_game->_MasterTrollBase->getPositionY()));
        if(theDistance2 <= 250)
        {
            FireBulletAtTroll(mContainsPowerUp);
            
            mContainsPowerUp = -1;
            
            if(mPowerUpIcon != NULL){
                removeChild(mPowerUpIcon);
            }
            
            return;
        }
    }
    
    // If has troll attached - then follow to him?
    if(mSnapedTroll != NULL)
    {
        //Check what are the zone range
        float theDistance2 = sqrtf((getPositionX()-mSnapedTroll->getPositionX())*(getPositionX()-mSnapedTroll->getPositionX()) +
                                   (getPositionY()-mSnapedTroll->getPositionY())*(getPositionY()-mSnapedTroll->getPositionY()));
        if(theDistance2 <= 210)//140
        {
            //Fire some fast particle
            mSnapedTroll_FallBack = mSnapedTroll;
            mSnapedTroll = NULL;
            
            FireBulletAtTroll(mContainsPowerUp);
            
            mContainsPowerUp = -1;
            
            /*
            // Kill or freeze the troll
            if(mContainsPowerUp == 0)
            {
                mSnapedTroll->removeFromSave();
            }
            else if(mContainsPowerUp == 1)
            {
                // Do all the magic here
                mSnapedTroll->mFreezedTime = 10;//Get from missions some param?
                
                // Add for now blue troll FX
                mSnapedTroll->_animation->setColor(ccc3(0, 164, 255));
                
                // Create some particles and sound ???
                CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/bullet_explode.plist");
                p->setPosition(mSnapedTroll->getPositionX(), mSnapedTroll->getPositionY());
                p->setAutoRemoveOnFinish(true);
                _game->addChild(p,5002);
                
                _game->playInGameSound("dwarf_crash");
            }
            
            // Remove from dwarf it !!!
            mSnapedTroll = NULL;
            
            //Remove magic from dwarf
            mContainsPowerUp = -1;
            
            if(mPowerUpIcon != NULL){
                removeChild(mPowerUpIcon);
            }
            */
            
            if(mPowerUpIcon != NULL){
                removeChild(mPowerUpIcon);
            }
            
            return;
        }
        
        
        // Try to follow?
        //Snap to troll
        /*
        CCPoint previousPoint = touch->getStartLocation();
        
        if (_movePoints->count() > 0)
        {
            previousPoint = _movePoints->getControlPointAtIndex(_movePoints->count() - 1);
        }
        
        // if this will be the first point, start only when it is away from pick point
        if (ccpDistanceSQ(touch->getLocation(), previousPoint) > POINT_RADIUS * POINT_RADIUS)
        {
            CCPoint position = touch->getLocation();
            
            if (addMovePoint(position, previousPoint))
            {
                CCPoint cavePosition = _game->getCavePoint(_type);
            }
        }
        */
        
        
        CCPoint previousPoint = mSnapedTroll->getPosition();//touch->getStartLocation();
        
        if (_movePoints->count() > 0)
        {
            previousPoint = _movePoints->getControlPointAtIndex(_movePoints->count() - 1);
        }
        
        if (ccpDistanceSQ(mSnapedTroll->getPosition(), previousPoint) > POINT_RADIUS * POINT_RADIUS)
        {
            CCPoint position = mSnapedTroll->getPosition();//touch->getLocation();
            addMovePoint(position, previousPoint,true);
        }
    }
}

void Dwarf::setPowerButton(int theID)
{
    mContainsPowerUp = theID;
    
    if(mPowerUpIcon != NULL){
        removeChild(mPowerUpIcon);
    }
    
    CCLog("Set spell with id: %i",theID);
    
    if(theID == DWARF_SPELL_ELECTRIFY)
    {
        mPowerUpIcon = CCSprite::create("button_electro.png");
    }
    else if (theID == DWARF_SPELL_FREEZER)
    {
        mPowerUpIcon = CCSprite::create("button_freez.png");
    }
    else if(theID >= 100) // The spell stuff
    {
        mSpellForAttack = User::getInstance()->getItemDataManager().getSpellByID(mContainsPowerUp);
        mPowerUpIcon = CCSprite::create("button_spell.png");
    }
    
    addChild(mPowerUpIcon,-1);
}
