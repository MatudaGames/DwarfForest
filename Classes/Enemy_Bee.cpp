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
_game(NULL), _animation(NULL), _upAnimation(NULL), _downAnimation(NULL), _shootDownAnimation(NULL), _shootUpAnimation(NULL)
{
    
}

Enemy_Bee::~Enemy_Bee()
{
    if (_game)
	{
	_game->release();
	}
	if (_upAnimation) _upAnimation->release();
	if (_downAnimation) _downAnimation->release();
	if (_shootDownAnimation) _shootDownAnimation->release();
	if (_shootUpAnimation) _shootUpAnimation->release();
}

bool Enemy_Bee::init(GameScene* game)
{
    if (!CCNode::init())
    {
        return false;
    }
    
    //------------------
    _movePoints = CCPointArray::create(0);
    _movePoints->retain();
    
    mEnemyID = -1;
    
    _game = game;
    
    _allCreated = false;
    
    beeWillShoot = false;
    
    canMove = true;
    
    _movingToFinish = true;
    _speed = 10;
    _angle = 0;
    bulletCount = 0;
    _beeIdleBeforeFire = 0;
    bullet_speed = 1;
    
    _bulletArr = CCArray::create();
    _bulletArr->retain();
    
    mCatchRadar = CCDrawNode::create();
    mCatchRadar->setVisible(false);
    this->addChild( mCatchRadar ,100);//dnode
    
    //_base = CCSprite::create("beta/bee.png");
    //addChild(_base);
    
    _downAnimation = SpriteAnimation::create("Characters/bee/bee_fly_down.plist");
	_downAnimation->retain();
	
	_upAnimation = SpriteAnimation::create("Characters/bee/bee_fly_up.plist");
	_upAnimation->retain();
	
	_shootDownAnimation = SpriteAnimation::create("Characters/bee/bee_shoot_down.plist");
	_shootDownAnimation->retain();
	
	_shootUpAnimation = SpriteAnimation::create("Characters/bee/bee_shoot_up.plist");
	_shootUpAnimation->retain();
	
	
	if (GLOBAL_SCALE!=1.0f)
    {
		_upAnimation->setScale(GLOBAL_SCALE);
        _downAnimation->setScale(GLOBAL_SCALE);
        _shootDownAnimation->setScale(GLOBAL_SCALE);
        _shootUpAnimation->setScale(GLOBAL_SCALE);
	}
    
    return true;
}



void Enemy_Bee::update(float delta)
{
//    CCLog("Update bee");
    
    // Check if did get to final place !!!
    if(!_allCreated)return;
    
    if(_timeOnMap>0){
        // Remove from map when can !!!
        _timeOnMap-=delta;
        if(_timeOnMap<=0){
            
            //Remove all bullets too !!!
            for (int bulletIndex = _bulletArr->count() - 1; bulletIndex >= 0; --bulletIndex)
            {
                CCSprite* _bullet = static_cast<CCSprite*>(_bulletArr->objectAtIndex(bulletIndex));
                if(_bullet!=NULL)
                {
                    _bulletArr->removeObject(_bullet);
                    _game->removeChild(_bullet);
                    _bullet = NULL;
                }
            }
            
            // Game over for this bee
            _game->_otherEnemy->removeObject(this);
            _game->removeNode(this);
            return;
        }
    }
    
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
                        /*
                        dwarf->setDisabled(true);
                        dwarf->createCrash();
                        dwarf->doDwarfBang(dwarf->_angle);
                        dwarf->setTag(999);//Will skip his pause !!!
                        */
                        //_game->lose();
                        //_game->showWinScreen();
        				dwarf->removeFromSave();
        				//_allCreated = false;
        
                        break;
                    }
                }
            }
        }
    }
    
    
    //Walk by the movepoints
    float x = getPositionX();
    float y = getPositionY();
    
    mMoveIndex2 = 0;
    
    CCPoint point = _movePoints->getControlPointAtIndex(mMoveIndex);
    CCPoint point2 = _movePoints->getControlPointAtIndex(mMoveIndex2);
    CCPoint point3 = _movePoints->getControlPointAtIndex(mMoveIndex3+1);
	
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
        bulletCount = 2;
        setAngle(atan2f(_moveTo.y - y, _moveTo.x - x));
        _beeIdleBeforeFire = 1,5;//1,5 i hope will be good.
        
        // The radar?
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
        
        //mCatchRadar->drawPolygon_fromVector(points, points.size(), ccc4f(1, 0, 0, 0.4f), 2, ccc4f(0, 0, 0, 0.1) );
        
        int aCurrentAngle = (-_angle * 180.0 / M_PI)+coneWidth/2;
        mCatchRadar->setRotation(aCurrentAngle);
        
        mCatchRadar->setVisible(true);
    }
    else
    {
        setAngle(atan2f(point.y - y, point.x - x));
    }
    
    if(_beeIdleBeforeFire>0){
        _beeIdleBeforeFire-=delta;
        bulletCount--;
        if(_beeIdleBeforeFire<=0){
            mCatchRadar->setVisible(false);
            beeWillShoot = true;
            _beeIdleBeforeFire = 0;
            //Fire the bullet !!!
            CCSprite* aBullet = CCSprite::create("Characters/bee/dzelonis2.png");
            aBullet->setPosition(getPosition());
            CCMoveBy* aMoveBy;
            aBullet->setFlipX(!_animation->isFlipX());
            /*
			if(!_animation->isFlipX()){
                aMoveBy = CCMoveBy::create(bullet_speed,ccp(0, 10));
            }
            else{
                aMoveBy = CCMoveBy::create(1.0f,ccp(bullet_speed,0));
            }
            */
            /*
            if (_angle < 5.0f * M_PI / 8.0f){
            	aMoveBy = CCMoveBy::create(0.1f,ccp(0, 10));
				aBullet->setRotation(-90);	
            }else if (_angle < 13.0f * M_PI / 8.0f){
            	aMoveBy = CCMoveBy::create(0.1f,ccp(0, -10));
            	aBullet->setRotation(90);
            }else if(_angle >= 15.0f * M_PI / 2.0f || _angle < M_PI / 8.0f || _angle < 3.0f * M_PI / 8.0f){
            	aMoveBy = CCMoveBy::create(0.1f,ccp(0, 0));
            	aBullet->setRotation(atan2f(90, 0));	
            }else if (_angle < 9.0f * M_PI / 8.0f || _angle < 7.0f * M_PI / 8.0f || _angle < 11.0f * M_PI / 8.0f){
            	aMoveBy = CCMoveBy::create(0.1f,ccp(0, 0));
            	aBullet->setRotation(atan2f(-90, 0));
            }
            */
            
         	if(point2.x == point3.x)
         	{
         		if (_angle < 5.0f * M_PI / 8.0f){
            	aMoveBy = CCMoveBy::create(0.1f,ccp(0, 10));
				aBullet->setRotation(90);	
            }else{
            	aMoveBy = CCMoveBy::create(0.1f,ccp(0, -10));
            	aBullet->setRotation(270);
            	aBullet->setFlipX(true);
			}
         	}else{
         		if(!_animation->isFlipX()){
                aMoveBy = CCMoveBy::create(0.1f,ccp(10, 0));
				aBullet->setRotation(180);	
            	}
            else{
                aMoveBy = CCMoveBy::create(0.1f,ccp(-10, 0));
                aBullet->setRotation(540);
            	}
         	}
            
            //Where will it move???
            
            CCRepeatForever* aRepeat = CCRepeatForever::create(aMoveBy);
            aBullet->runAction(aRepeat);
           
            _bulletArr->addObject(aBullet);
            canMove = true;
            _game->addChild(aBullet);
            //beeWillShoot = false;
        	
        	CCDelayTime* aDelay = CCDelayTime::create(2.0f);
            CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Enemy_Bee::letsMove));
            CCSequence* aSeq1 = CCSequence::create(aDelay,func,NULL);
            runAction(aSeq1);
        	
        	CCLog("Play music [bulletCount]:%i",bulletCount);
        }

        return;
    }
    
     mGULGUL = 0;
	if(canMove==false)
	{
    CCPoint point2 = _movePoints->getControlPointAtIndex(mGULGUL);
   
    CCPoint newPosition = ccp(x + cosf(_angle) * delta * (_speed * _game->getGameSpeed()),
                              y + sinf(_angle) * delta * (_speed * _game->getGameSpeed()));
    
    cocos2d::CCNode::setPosition(newPosition.x,newPosition.y);
    /*
	}
	return;
	}
	*/
    
    /*
    if (ccpDistanceSQ(_moveTo, getPosition()) <= 800)
    {
        _movingToFinish = !_movingToFinish;
        
        _beeIdleBeforeFire = 3;
        
        _moveTo = _movePoints->getControlPointAtIndex(mMoveIndex);
        mMoveIndex+=_moveValue;
        if(mMoveIndex<0){
            mMoveIndex = 1;
            _moveValue = 1;
        }
        else if(mMoveIndex>_movePoints->count()){
            mMoveIndex = _movePoints->count()-1;
            _moveValue = -1;
        }
        
     
        //if(_movingToFinish){
        //    _moveTo = ccp(_finishX,_finishY);
        //}
        //else{
        //    _moveTo = ccp(_startX,_startY);
        //}
     
     
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
    */
    
	}
}

void Enemy_Bee::setAngle(float value)
{
    _angle = wrapTwoPI(value);
        
    //if(_angle>0 || _angle<0){
        //if(!_upAnimation->isFlipX()){
       //     _upAnimation->setFlipX(true);
            //setAnimation(_upAnimation);
       // }
    //}
    //else{
       // if(_upAnimation->isFlipX()){
          //  _upAnimation->setFlipX(false);
           // setAnimation(_upAnimation);
	//	}
    //}
    
    //Chek to what dir will current sprite fli
    
    if (_angle >= 15.0f * M_PI / 8.0f || _angle < M_PI / 8.0f)
    {
        //right
        //_downAnimation->setFlipX(false);
        //_downAnimation->setFlipY(false);
        //aBullet->setFlipX(false);
        //setAnimation(_downAnimation);
        if(_beeIdleBeforeFire<=0){
        	_downAnimation->setFlipX(false);
        	_downAnimation->setFlipY(false);
        	setAnimation(_downAnimation);
    	  }else if (_beeIdleBeforeFire>0){
    	  	_shootDownAnimation->setFlipX(false);
        	_shootDownAnimation->setFlipY(false);
    		setAnimation(_shootDownAnimation);	
    	}
    }
    else if (_angle < 3.0f * M_PI / 8.0f)
    {
        //right up
        if(_beeIdleBeforeFire<=0){
        	_upAnimation->setFlipX(true);
        	_upAnimation->setFlipY(false);
        	setAnimation(_upAnimation);
    	  }else if (_beeIdleBeforeFire>0){
    	  	_shootUpAnimation->setFlipX(true);
        	_shootUpAnimation->setFlipY(false);
    		setAnimation(_shootUpAnimation);	
    	}
    }
    else if (_angle < 5.0f * M_PI / 8.0f)
    {
        //up
        if(_beeIdleBeforeFire<=0){
        	setAnimation(_upAnimation);
    	  }else if (_beeIdleBeforeFire<1 || _beeIdleBeforeFire==0){
    		setAnimation(_shootUpAnimation);	
    	}
        //aBullet->setFlipX(false);
    }
    else if (_angle < 7.0f * M_PI / 8.0f)
    {
        //left up
        if(_beeIdleBeforeFire<=0){
        	_upAnimation->setFlipX(false);
        	_upAnimation->setFlipY(false);
        	setAnimation(_upAnimation);
    	  }else if (_beeIdleBeforeFire>0){
    	  	_shootUpAnimation->setFlipX(false);
        	_shootUpAnimation->setFlipY(false);
    		setAnimation(_shootUpAnimation);	
    	}
    }
    else if (_angle < 9.0f * M_PI / 8.0f)
    {
        //left
        if(_beeIdleBeforeFire<=0){
        	_downAnimation->setFlipX(true);
        	_downAnimation->setFlipY(false);
        	setAnimation(_downAnimation);
    	  }else if (_beeIdleBeforeFire>0){
    	  	_shootDownAnimation->setFlipX(true);
        	_shootDownAnimation->setFlipY(false);
    		setAnimation(_shootDownAnimation);	
    	}
    }
    else if (_angle < 11.0f * M_PI / 8.0f)
    {
        //left down
        if(_beeIdleBeforeFire<=0){
        	_downAnimation->setFlipX(true);
        	_downAnimation->setFlipY(false);
        	setAnimation(_downAnimation);
    	  }else if (_beeIdleBeforeFire>0){
    	  	_shootDownAnimation->setFlipX(true);
        	_shootDownAnimation->setFlipY(false);
    		setAnimation(_shootDownAnimation);	
    	}
    }
    else if (_angle < 13.0f * M_PI / 8.0f)
    {
        //down
        //setAnimation(_downAnimation);
          if(_beeIdleBeforeFire<=0){
        	setAnimation(_downAnimation);
    	  }else if (_beeIdleBeforeFire>0){
    		setAnimation(_shootDownAnimation);	
    	}
    }
    else
   {
        //right down
        if(_beeIdleBeforeFire<=0){
        	_downAnimation->setFlipX(false);
        	_downAnimation->setFlipY(false);
        	setAnimation(_downAnimation);
    	  }else if (_beeIdleBeforeFire>0){
    		setAnimation(_shootDownAnimation);	
    	}
   }
}

void Enemy_Bee::setAnimation(SpriteAnimation* animation)
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
        //_changedAnimation = true;
	}
}

void Enemy_Bee::letsMove()
{
	canMove = false;
}

void Enemy_Bee::AddShootAni()
{
	setAnimation(_shootDownAnimation);
}

void Enemy_Bee::CreateFromMissionParams()
{
	
    //_movingToFinish = true;
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

// The new stuff
void Enemy_Bee::SetMissionStuff(MissionTroll theMission)
{
	
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
    
    bullet_speed = theMission._beeBulletSpeed;
    
    mEnemyID = theMission._indexID;
    
    // Do wee need radar?
    setRadar(theMission._radar_radius,theMission._radar_width);
    
    _speed = theMission._speed;
    
    _moveInCircle = false;
   
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
        
        for (float a = 0.0f; a < cir; a += precision)
        {
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
        
        // get the next point
        mMoveIndex+=_moveValue;
        if(mMoveIndex<0){
            mMoveIndex = 1;
            _moveValue = 1;
        }
        else if(mMoveIndex>_movePoints->count()){
            mMoveIndex = _movePoints->count()-1;
            _moveValue = -1;
        }
        _moveTo = _movePoints->getControlPointAtIndex(mMoveIndex);
        
        setPosition(ccp(theMission._paths[theMission._pathStartIndex]->x,theMission._paths[theMission._pathStartIndex]->y));
        
        
        CCLog("end");
    }
    
}

void Enemy_Bee::setRadar(int theRadius,int theWidth)
{
	/*
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
    
    //mCatchRadar->drawPolygon_fromVector(points, points.size(), ccc4f(1, 0, 0, 0.4f), 2, ccc4f(0, 0, 0, 0.1) );
    
    mCatchRadar->setRotation(0);
    */
}



