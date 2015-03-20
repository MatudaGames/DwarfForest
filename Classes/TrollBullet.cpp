//
//  TrollBullet.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 01/08/14.
//
//

#include "TrollBullet.h"
#include <SimpleAudioEngine.h>
#include "GameScene.h"
#include "AppMacros.h"
#include "Utils.h"
#include "User.h"

#define kHUD_Z_Order 200

USING_NS_CC;
using namespace CocosDenshion;

const float TROLL_SPEED = 30.0f;
const float RADIUS = 40.0f;
const uint8_t TOUCHABLE_OPACITY = 255;
const uint8_t UNTOUCHABLE_OPACITY = 128;

const float PICK_RADIUS = 50.0f;

const uint32_t TOUCHABLE_ZONE = 100;

TrollBullet* TrollBullet::create(GameScene* game,int theType)
{
    TrollBullet *pRet = new TrollBullet();
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

TrollBullet::TrollBullet():
_game(NULL),_animation(NULL)
{
	
}

TrollBullet::~TrollBullet()
{
	if (_game){
        _game->release();
    }
    
    if (_animation) _animation->release();
}

bool TrollBullet::init(GameScene* game,int theType)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    _dwarf = NULL;
    
    _speedAddValue = 1;
    
    _type = theType;
    
    _timeActive = 0;
    
    _startX = -1;
    _startY = -1;
    
    //Create the sprite and add some particles :)
    /*
    _sprite = CCSprite::create("beta/target.png");
	addChild(_sprite, kHUD_Z_Order-1);
    */
    
    
//    CCParticleSystemQuad* p;
//    "Particles/bullet_part_red.plist"
    /*
    std::stringstream aaaaa;
    
    if(theType == MASTER_ACTION_BULLET_ICE)
    {
        _sprite->setColor(ccc3(0, 255, 0));
        
        ccBlendFunc* someBlend = new ccBlendFunc();
        someBlend->src = GL_ONE;//OGLES.GL_ONE;
        someBlend->dst = GL_ONE;//OGLES.GL_ONE;
        _sprite->setBlendFunc(*someBlend);// .BlendFunc = someBlend;
        
//        p = CCParticleSystemQuad::create("Particles/bullet_part.plist");
        aaaaa<<"Particles/bullet_part.plist";
    }
    else if(theType == MASTER_ACTION_BULLET_POISON)
    {
        _sprite->setColor(ccc3(0, 0, 255));
        
        ccBlendFunc* someBlend = new ccBlendFunc();
        someBlend->src = GL_ONE;//OGLES.GL_ONE;
        someBlend->dst = GL_ONE;//OGLES.GL_ONE;
        _sprite->setBlendFunc(*someBlend);// .BlendFunc = someBlend;
        
//        p = CCParticleSystemQuad::create("Particles/bullet_part_green.plist");
        aaaaa<<"Particles/bullet_part_green.plist";
    }
    else// if(theType == MASTER_ACTION_BULLET)
    {
        _sprite->setColor(ccc3(255, 0, 0));
        
        ccBlendFunc* someBlend = new ccBlendFunc();
        someBlend->src = GL_ONE;//OGLES.GL_ONE;
        someBlend->dst = GL_ONE;//OGLES.GL_ONE;
        _sprite->setBlendFunc(*someBlend);// .BlendFunc = someBlend;
        
//        p = CCParticleSystemQuad::create("Particles/bullet_part_red.plist");
        aaaaa<<"Particles/bullet_part_red.plist";
    }
    
    //Add the particle
    CCLog("aaaaa: %s",aaaaa.str().c_str());
    CCParticleSystemQuad* p = CCParticleSystemQuad::create(aaaaa.str().c_str());
    p->setPosition(getPositionX(), getPositionY());
    p->setAutoRemoveOnFinish(true);
    addChild(p,kHUD_Z_Order-1);
    */
    
    timeToStartFly = 1.0f;
    
    _game = game;
    _game->retain();
    _game->playInGameSound("troll_intro");
    
    _angle = 0;
    _speed = 150;
    
    _canMove = true;
    _isDisabled = false;
    _touched = false;
    _touchable = true;
    
    // The new style for bullets?
    _animation = SpriteAnimation::create("Characters/master_troll/mt_bullet.plist");
	
    _animation->retain();
    _animation->setOpacity(0);
    addChild(_animation, kHUD_Z_Order-1);
    
    CCFadeIn* aFadeIn = CCFadeIn::create(1.0f);
    _animation->runAction(aFadeIn);
	
	return true;
}

void TrollBullet::OnDoAction(Dwarf* theForced)
{
    _isDisabled = true;
    _canMove = false;
    
    // Check if this dwarf has no other kaboom on it !!!
    theForced->ClearOldTraps();
    
    if(_type == MASTER_ACTION_BULLET)
    {
        theForced->_knockOutTime = 3;
        theForced->_knockOut = true;
        theForced->createCrash();
    }
    else if(_type == MASTER_ACTION_BULLET_ICE)
    {
        _game->FreezeDwarfTotal(theForced);
    }
    else if(_type == MASTER_ACTION_BULLET_POISON)
    {
        // This would be better if all would go to one place - not like now :D
        theForced->setAction(_type);
    }
    else if (_type == MASTER_ACTION_BULLET_STRAIGHT|| _type == MASTER_ACTION_BULLET_SPLIT_MIDDLE || _type == MASTER_ACTION_BULLET_ZIGZAG || _type == MASTER_ACTION_BULLET_ONE_LINE || _type == MASTER_ACTION_BULLET_DECOMPOSE_BEGINNING || _type == MASTER_ACTION_BULLET_SPLIT_UP)
    {
//        _game->_mission_SaveDwarfs_KillMax-=1;
        theForced->setAction(_type);
    }else if (_type == MASTER_ACTION_BULLET_SPELL)
	{
		if(_game->mCurrentMission.Task_type == MissionType_DwarfSave || _game->mCurrentMission.Task_type == MissionType_DestroyTotem)
		{
			for(int otherIndex = _game->_powersOnMap->count()-1;otherIndex>=0;--otherIndex)
    		{
    		GameItem_PowerUp* bee = static_cast<GameItem_PowerUp*>(_game->_powersOnMap->objectAtIndex(otherIndex));
       			if (bee->mPowerID >= 100)
				{            
    			theForced->setPowerButton(bee->mPowerID);
    
    			bee->onRemove();
    			}
			}
		}
    }
}

void TrollBullet::update(float delta)
{
//    CCLog("The Angle at bullet in loop: %f",_angle);
    
    if(!_canMove){
        return;//Wait for start move allow !!!
    }
    
    if(timeToStartFly>0){
        timeToStartFly-=delta;
        return;
    }
    
    float x = getPositionX();
    float y = getPositionY();
    
    // Update the speed - from slow to faster !!!
    if(_speed<_speedMax){
        _speed+=delta*_speedAddValue;
    }
    
    if(_dwarf != NULL && _dwarf->getParent()!=NULL){
        //Check if it is not null

        
        CCPoint point = _dwarf->getPosition();
        
        if (ccpDistanceSQ(point, getPosition()) <= 1000)
        {
            //GameOver dwarf
            OnDoAction(_dwarf);
//            _dwarf->_knockOutTime = 3;
//            _dwarf->_knockOut = true;
//            _dwarf->createCrash();
            
            //Create some particles and sound !!!
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/bullet_explode.plist");
            p->setPosition(getPositionX(), getPositionY());
            p->setAutoRemoveOnFinish(true);
            _game->addChild(p,kHUD_Z_Order-1);
            
            _canMove = false;
        }
        else
        {
            setAngle(atan2f(point.y - y, point.x - x));
        }
        
        CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()),
                                  y + sinf(_angle) * delta * (_speed * _game->getGameSpeed()));
        cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
    }
    else{
        //Bullet just flys around the space while hits other object or fly out of the map !!!
        
        x += cosf(_angle) * delta * (_speed * _game->getGameSpeed());
        y += sinf(_angle) * delta * (_speed * _game->getGameSpeed());
        
        if(_timeActive>0)CCLog("The angle at bullet %f",_angle);
        
        cocos2d::CCNode::setPosition(x, y);
        
        _touchable = (x > 0 &&
                      y > 0 &&
                      x < designResolutionSize.width &&
                      y < designResolutionSize.height);
        
        if (!_touchable)
        {
            _isDisabled = true;
        }
    }
    
    if(_timeActive>0)
    {
        _timeActive-=delta*_game->getGameSpeed();
        if(_timeActive<=0)
        {
            //Disable it - remove
            _timeActive = -1;
            _isDisabled = true;
        }
    }
    
    if(_distanceActive>0)
    {
        if(_startX == -1 && _startY == -1)
        {
            _startX = getPositionX();
            _startY = getPositionY();
        }
        else
        {
            // Check when to explode
            float dwarfDistance = sqrtf((_startX-getPositionX())*(_startX-getPositionX()) +
                                        (_startY-getPositionY())*(_startY-getPositionY()));
            if(dwarfDistance>=_distanceActive)
            {
                _isDisabled = true;
                _distanceActive = -1;
            }
        }
    }
}

float TrollBullet::GetAngle()
{
    return _angle;
}

void TrollBullet::setAngle(float value)
{
	_angle = wrapTwoPI(value);
}

bool TrollBullet::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(_isDisabled){
        return false;
    }
    
    if(_canMove == false){
        _touched = false;
        return false;
    }
    
    if(_type == MASTER_ACTION_BULLET_STRAIGHT){
        return false;
    }
    
    if(ccpDistanceSQ(touch->getLocation(), getPosition()) <= PICK_RADIUS * PICK_RADIUS){
        _touched = true;
        _isDisabled = true;
        _canMove = false;
        
        //Create some particles and sound !!!
        CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/bullet_explode.plist");
        p->setPosition(getPositionX(), getPositionY());
        p->setAutoRemoveOnFinish(true);
        _game->addChild(p,kHUD_Z_Order-1);
        
        return true;
    }
    
    _touched = false;
    return false;
}

void TrollBullet::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    //Do nothing for now !!!
    
}

void TrollBullet::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(_isDisabled){
        return;
    }
    
    if(_touched){
        //Check if can blitz it !!!
//        if(_game->CanZipTroll()){
//            _canMove = false;
//            _game->CreateBlitz(getPositionX(),getPositionY()-80,this);
//        }
    }
    
    _touched = false;
}

void TrollBullet::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void TrollBullet::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}
