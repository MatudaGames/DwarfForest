//
//  Enemy_Bee.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 13/11/14.
//
//

#include "Enemy_Bee.h"

#include <SimpleAudioEngine.h>
#include "GameScene.h"
#include "AppMacros.h"
#include "Utils.h"
#include "User.h"


USING_NS_CC;
using namespace CocosDenshion;

Enemy_Bee* Enemy_Bee::create(GameScene* game)
{
    Enemy_Bee *pRet = new Enemy_Bee();
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

Enemy_Bee::Enemy_Bee():
_game(NULL)
{
    
}

Enemy_Bee::~Enemy_Bee()
{
    if (_game){_game->release();}
}

bool Enemy_Bee::init(GameScene* game)
{
    if (!CCNode::init())
    {
        return false;
    }
    
    _game = game;
    
    _allCreated = false;
    
    _movingToFinish = true;
    _speed = 10;
    _angle = 0;
    _beeIdleBeforeFire = 0;
    
    _bulletArr = CCArray::create();
    _bulletArr->retain();
    
    mCatchRadar = CCDrawNode::create();
    mCatchRadar->setVisible(false);
    this->addChild( mCatchRadar ,100);//dnode
    
    _base = CCSprite::create("beta/bee.png");
    addChild(_base);
    
    return true;
}



void Enemy_Bee::update(float delta)
{
    CCLog("Update bee");
    
    // Check if did get to final place !!!
    if(!_allCreated)return;
    
    //
    
    for (int bulletIndex = _bulletArr->count() - 1; bulletIndex >= 0; --bulletIndex)
    {
        CCSprite* _bullet = static_cast<CCSprite*>(_bulletArr->objectAtIndex(bulletIndex));
        
        if(_bullet!=NULL)
        {
            
            if(_bullet->getPositionX()>_game->visibleSize.width || _bullet->getPositionX()<0 || _bullet->getPositionY()>_game->visibleSize.height || _bullet->getPositionY()<0){
                //Gameover for bullet
                
                _bulletArr->removeObject(_bullet);
                _game->removeChild(_bullet);
                _bullet = NULL;
            }
            else{
                //Check if did not hit any dwarf
                
                for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
                {
                    Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
                    if (ccpDistanceSQ(dwarf->getPosition(), _bullet->getPosition())<= powf(40, 2)*GLOBAL_SCALE)
                    {
                        _game->stopInGameSound("Footsteps");
                        _game->stopInGameSound("troll_walk");
                        _game->stopInGameSound("dwarf_web_stuck",true);
                        
                        dwarf->setDisabled(true);
                        dwarf->createCrash();
                        dwarf->doDwarfBang(dwarf->_angle);
                        dwarf->setTag(999);//Will skip his pause !!!
                        
                        _game->lose();
                        break;
                    }
                }
            }
        }
    }
    
    
    //Walk by the movepoints
    float x = getPositionX();
    float y = getPositionY();
    
    if(_beeIdleBeforeFire>0){
        _beeIdleBeforeFire-=delta;
        
        if(_beeIdleBeforeFire<=0){
            mCatchRadar->setVisible(false);
            
            _beeIdleBeforeFire = 0;
            //Fire the bullet !!!
            CCSprite* aBullet = CCSprite::create("beta/stinger.png");
            aBullet->setPosition(getPosition());
            
            CCMoveBy* aMoveBy;
            
            aBullet->setFlipX(!_base->isFlipX());
            if(!_base->isFlipX()){
                aMoveBy = CCMoveBy::create(1.0f,ccp(bullet_speed,0));
            }
            else{
                aMoveBy = CCMoveBy::create(1.0f,ccp(-bullet_speed,0));
            }
            
            //Where will it move???
            
            CCRepeatForever* aRepeat = CCRepeatForever::create(aMoveBy);
            aBullet->runAction(aRepeat);
            
            _bulletArr->addObject(aBullet);
            
            _game->addChild(aBullet);
        }
        
        return;
    }
    
    
    if (ccpDistanceSQ(_moveTo, getPosition()) <= 800)
    {
        _movingToFinish = !_movingToFinish;
        
        _beeIdleBeforeFire = 3;
        
        if(_movingToFinish){
            _moveTo = ccp(_finishX,_finishY);
        }
        else{
            _moveTo = ccp(_startX,_startY);
        }
        
        setAngle(atan2f(_moveTo.y - y, _moveTo.x - x));
        
        //Create conus???
        
        
        // Draw the conus from settings
        std::vector<CCPoint> points;
        points.push_back(ccp(0,0));
        
        mCatchRadar->clear();
        
        // ----------------------------
        coneWidth = 8;
        coneRadius = sqrtf((getPositionX()-_moveTo.x)*(getPositionX()-_moveTo.x) + (getPositionY()-_moveTo.y)*(getPositionY()-_moveTo.y));;
        
        for (float ii = 0; ii < coneWidth; ii += 0.1)
        {
            points.push_back(ccp(0 + coneRadius * cos(ii * (M_PI / 180)), 0 + coneRadius * sin(ii * (M_PI / 180))));
        }
        
        points.push_back(ccp(0,0));
        
        mCatchRadar->drawPolygon_fromVector(points, points.size(), ccc4f(1, 0, 0, 0.4f), 2, ccc4f(0, 0, 0, 0.1) );
        
        int aCurrentAngle = (-_angle * 180.0 / M_PI)+coneWidth/2;
        mCatchRadar->setRotation(aCurrentAngle);
        
//        mCatchRadar->setRotation(30);
        
        mCatchRadar->setVisible(true);
    }
    else
    {
        setAngle(atan2f(_moveTo.y - y, _moveTo.x - x));
    }
    
    CCLog("angle of bee: %f",_angle);
    
    CCPoint newPosition = ccp(x + cosf(_angle) * delta * _speed,
                              y + sinf(_angle) * delta * _speed);
    
    cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
    
}

void Enemy_Bee::setAngle(float value)
{
    _angle = wrapTwoPI(value);
    
    if(_angle>3){
        if(!_base->isFlipX()){
            _base->setFlipX(true);
        }
    }
    else{
        if(_base->isFlipX()){
            _base->setFlipX(false);
        }
    }
    
    //Chek to what dir will current sprite fli
    /*
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
    */
}

void Enemy_Bee::CreateFromMissionParams()
{
    _movingToFinish = true;
    _moveTo = ccp(_finishX,_finishY);
    _allCreated = true;
}

void Enemy_Bee::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    CCNode::onEnter();
}

void Enemy_Bee::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCNode::onExit();
}