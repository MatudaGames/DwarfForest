//
//  Wind.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 13/01/14.
//
//

#include "Wind.h"
#include "GameScene.h"
#include "Dwarf.h"
#include "SpriteAnimation.h"
#include "User.h"
#include "Utils.h"

USING_NS_CC;

Wind* Wind::create(GameScene* gameScene)
{
	Wind *pRet = new Wind();
    if (pRet && pRet->init(gameScene))
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

Wind::Wind():
_animation(NULL),_startAnimation(NULL),_endAnimation(NULL),_activeAnim(NULL)
{
}

Wind::~Wind()
{
    if (_animation) _animation->release();
    if (_startAnimation) _startAnimation->release();
    if (_endAnimation) _endAnimation->release();
    if (_activeAnim) _activeAnim->release();
}

bool Wind::init(GameScene* gameScene)
{
	if (!Effect::init(gameScene))
	{
		return false;
	}
    
    _troll = NULL;
    _dwarf = NULL;
    
    mMoveByNodes = false;
    mMoveClock = false;
    
    _needToRemove = false;
    _speed = 75.0f;
    
    mTimeToLive = 10.0f;
    
    mContainsObject = false;
	
	_effectType = EFFECT_TYPE_WIND;
	
//	_sprite = CCSprite::create("meteorits/effect_area.png");
//	addChild(_sprite);
    
    //Thiss will have a instant loop anim !!!
//    tornado
    
    _activeAnim = SpriteAnimation::create("effects/wind/tornado.plist");
    _activeAnim->setPosition(ccp(0,50));
    _activeAnim->retain();
    addChild(_activeAnim);
    
    //Create the walk path around the map !!!
    mMoveIndex = 0;
    
//    _movePoints->addControlPoint(ccp(x, y));
//    _movePointsWind
    
    
//    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
//    
//    float precision = .05f;
//    float cir = 2 * M_PI;
//    int mRadius = 50;
//    for (float a = .0f; a < cir; a += precision)
//    {
//        float x = visibleSize.width/2 + mRadius * cos(a);
//        float y = visibleSize.height/2 + mRadius * sin(a);
////        vertex.push_back(ccp(x,y));
//        
//        CCSprite* aDummyDot = CCSprite::create("small_dot_red.png");
//        aDummyDot->setPosition(ccp(x,y));
//        addChild(aDummyDot);
//    }
    
//    _startAnimation = SpriteAnimation::create("sticky_web/web_intro.plist");
//    _startAnimation->retain();
//    
//    if (GLOBAL_SCALE!=1.0f)
//    {
//        _startAnimation->setScale(GLOBAL_SCALE);
//        _sprite->setScale(GLOBAL_SCALE);
//    }
//    
//    _startAnimation->setPosition(ccp(12,14));
    
    //is this really needed?
    this->scheduleUpdate();
    
    //Just give a path for the wind !!!
//    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //Get some random position !!!
//    MoveToPos(visibleSize.width-200, visibleSize.height-200);
    
    _dwarfType = 0;
    
    _game = gameScene;
	
	return  true;
}


void Wind::MoveToPos(int theX,int theY,int theCurve)
{
    //Some new stuff here !!!
    
    //First move to the 1st dot - then start to move in circle | then if troll move out of the map !!!
    mMoveIndex = theCurve;
    if(theX>0)
        mMoveClock = true;
    
    //Continues to move like was on circle
    CCPoint aMoveOne = _game->_movePointsWind->getControlPointAtIndex(mMoveIndex);
    
    CCMoveTo* aMove1 = CCMoveTo::create(5.0f,aMoveOne);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Wind::OnCompleteWalkToCircle));
    CCSequence* aSeq1 = CCSequence::create(aMove1,func,NULL);
    runAction(aSeq1);
    
    /*
    ccBezierConfig bezier;
    
    if(theCurve == 1)
    {
        bezier.controlPoint_1 = ccp(getPositionX(),getPositionY());//1096,168
        bezier.controlPoint_2 = ccp(getPositionX(), theY);//635,105
        bezier.endPosition = ccp(theX, theY);//160
    }
    else
    {
        bezier.controlPoint_1 = ccp(getPositionX(),getPositionY());//1096,168
        bezier.controlPoint_2 = ccp(theX, getPositionY());//635,105
        bezier.endPosition = ccp(theX, theY);//160
    }
    
    //1ST TIME STUFF
    if(User::getInstance()->_tutorial_small_7==0)
    {
        schedule(schedule_selector(Wind::CreateTutorialAnim), 0.0f, 0, 5.0f);
    }

    CCBezierTo* aToPos_1a = CCBezierTo::create(15.0f, bezier);
    
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Wind::onComplete_Special));
    
    CCSequence* aSeq = CCSequence::create(aToPos_1a,func,NULL);
    
    runAction(aSeq);
    */
}

void Wind::OnCompleteWalkToCircle()
{
    mMoveByNodes = true;
}

void Wind::CreateTutorialAnim()
{
    //1ST TIME STUFF
    if(User::getInstance()->_tutorial_small_7==0)
    {
        //1st time pause the game and show some stuff !!!
        _gameScene->stopInGameSound("Footsteps",true);
        _gameScene->stopInGameSound("troll_walk",true);
        _gameScene->stopInGameSound("dwarf_web_stuck",true);
        
        //Get the mid point !!!
        int aShowX = getPositionX();
        int aShowY = getPositionY();
        
        _gameScene->CreateSmallTutorial(7,aShowX,aShowY);
    }
}

void Wind::startStuckAnim()
{
    unschedule(schedule_selector(Wind::startStuckAnim));
//    _dwarf->setAnimationVisibility(false);
    
    if(_troll)
        _troll->setAnimationVisibility(false);
    else
        _dwarf->setAnimationVisibility(false);
    
    if (_dwarfType == DWARF_TYPE_FAT)
    {
        _animation = SpriteAnimation::create("effects/wind/tornado_fatdwarfed.plist");
        _animation->retain();
    }
    else if (_dwarfType == DWARF_TYPE_TALL)
    {
        _animation = SpriteAnimation::create("effects/wind/tornado_talldwarfed.plist");
        _animation->retain();
    }
    else if(_troll)
    {
        _animation = SpriteAnimation::create("effects/wind/tornado_trolled.plist");
        _animation->retain();
    }
    
    _animation->setPosition(ccp(0,50));
    
//    _game->playInGameSound("dwarf_web_stuck");
    
//    if (GLOBAL_SCALE!=1.0f)
//    {
//        _animation->setScale(GLOBAL_SCALE);
//    }
    
//    removeChild(_startAnimation);
    _activeAnim->setVisible(false);//For now
    
    mTimeToLive = -1;
    
    addChild(_animation);
    schedule(schedule_selector(Wind::finishStuckAnim), 0.0f, 0, 5.0f);
}

void Wind::update(float delta)
{
//    CCLog("ud");
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
            _angle = wrapTwoPI(atan2f(point.y - y, point.x - x));
        }
        
        CCPoint newPosition = ccp(x + cosf(_angle) * delta * _speed,
								  y + sinf(_angle) * delta * _speed);
        
        setPosition(newPosition);
    }
    
    if(mContainsObject)
    {
        //Nothing - do this stuff and quit !!!
        
    }
    else
    {
        //We can be quited
        if(mTimeToLive>0)
        {
            mTimeToLive-=delta;
            if(mTimeToLive<=0)
            {
                //Fade out !!!
                onComplete_Special();
            }
        }
    }
}

void Wind::finishStuckAnim()
{
    unschedule(schedule_selector(Wind::finishStuckAnim));
    
    if(_dwarf != NULL)
    {
        _dwarf->setPosition(getPosition());
        
        _game->CreateTornadoThrow(_dwarf,NULL,getPositionX(),getPositionY()+40);
        
        removeChild(_animation);
        _activeAnim->setVisible(true);//Run away tornado ??? or get small?
        schedule(schedule_selector(Wind::onComplete), 0.0f, 1, 0.0f);
    }
    else
    {
        mMoveByNodes = false;
        
        CCPoint finishPosition = ccp(0,0);
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        if(getPositionX()<visibleSize.width/2)
        {
            if(getPositionY()<visibleSize.height/2)
                finishPosition = ccp(-100,visibleSize.height/2-200);
            else
                finishPosition = ccp(-100,visibleSize.height/2+200);
        }
        else
        {
            if(getPositionY()<visibleSize.height/2)
                finishPosition = ccp(visibleSize.width+100,visibleSize.height/2-200);
            else
                finishPosition = ccp(visibleSize.width+100,visibleSize.height/2+200);
        }
        
        CCMoveTo* aMove1 = CCMoveTo::create(5.0f,finishPosition);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Wind::OnCompletedScale));
        CCSequence* aSeq1 = CCSequence::create(aMove1,func,NULL);
        runAction(aSeq1);
        
    }
    
//    if(_dwarfType == DWARF_TYPE_FAT)
//    {
//        //Create fat fly and land stuff !!!
//        _endFly = CCSprite::create("effects/wind/fatass_fly.png");
//        
//    }
//    else if(_dwarfType == DWARF_TYPE_TALL)
//    {
//        _endFly = CCSprite::create("effects/wind/tallass_fly.png");
//    }
//    
//    addChild(_endFly);
    
    //Add the roll anim !!!
    
    
//    if (_dwarfType == DWARF_TYPE_FAT)
//    {
////        _endAnimation = SpriteAnimation::create("sticky_web/fatass_free.plist");
////        _endAnimation->retain();
//    }
//    else if (_dwarfType == DWARF_TYPE_TALL)
//    {
////        _endAnimation = SpriteAnimation::create("sticky_web/tallass_free.plist");
////        _endAnimation->retain();
//    }
    
//    CCLog("Tornado drop x: %f",getPositionX());
//    CCLog("Tornado drop y: %f",getPositionY());
    
//    if(_troll)
//    {
//        //For now disabled
//    }
//    else
//    {
//        _dwarf->setPosition(getPositionX(),getPositionY());
//    }
    
    //Just let the person free !!!
    
//    if (GLOBAL_SCALE!=1.0f)
//    {
//        _endAnimation->setScale(GLOBAL_SCALE);
//    }
    
//    _game->stopInGameSound("dwarf_web_stuck");
//    _game->playInGameSound("dwarf_web_exit");
    
    
//    removeChild(_animation);
//    _activeAnim->setVisible(true);//Run away tornado ??? or get small?
//    schedule(schedule_selector(Wind::onComplete), 0.0f, 1, 0.0f);
}

void Wind::onComplete_Special()
{
    unschedule(schedule_selector(Wind::finishStuckAnim));
    if(_needToRemove || _disabled)
        return;
    
    if(_dwarf)
    {
        _dwarf->setPosition(getPosition());
        
        _game->CreateTornadoThrow(_dwarf,NULL,getPositionX(),getPositionY()+40);
    }
    
//    if(_dwarf)
//    {
//        _dwarf->setPosition(getPositionX(),getPositionY());
//        
//        _dwarf->setAnimationVisibility(true);
//        _dwarf->setVisible(true);
//        _dwarf->setDisabled(false);
//    }
    
//    _needToRemove = true;
    _disabled = true;
    
    this->unscheduleUpdate();
    
    //Scale down?
    CCScaleTo* aScale = CCScaleTo::create(0.5f,0.0f);
//    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Wind::OnCompletedScale));
    CCSequence* aSeq = CCSequence::create(aScale,func,NULL);
    runAction(aSeq);
}

void Wind::OnCompletedScale()
{
    if(_troll!=NULL)
    {
        _troll->setVisible(true);
        _troll->setPosition(getPosition());
    }
    
    _needToRemove = true;
}

void Wind::ClearAll()
{
    if(_dwarf)
    {
        _dwarf->setDisabled(true);
        _dwarf->removeFromSave();
    }
}

void Wind::onComplete()
{
    unschedule(schedule_selector(Wind::onComplete));
    if(_needToRemove || _disabled)
        return;
    
//    _needToRemove = true;
    _disabled = true;
    
//    if(_dwarf)
//    {
//        _dwarf->setPosition(getPositionX(),getPositionY());
//        
////        CCLog("Dwarf drop x: %f",_dwarf->getPositionX());
////        CCLog("Dwarf drop y: %f",_dwarf->getPositionY());
//        
//        _dwarf->setAnimationVisibility(true);
//        _dwarf->setVisible(true);
//        _dwarf->setDisabled(false);
//    }
    
    this->unscheduleUpdate();
    
    //Scale down?
    CCScaleTo* aScale = CCScaleTo::create(0.5f,0.0f);
//    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Wind::OnCompletedScale));
    CCSequence* aSeq = CCSequence::create(aScale,func,NULL);
    runAction(aSeq);
    
//    _game->removeChild(this);
}

void Wind::touch(Dwarf* dwarf,Troll* troll)
{
    if(_needToRemove || _disabled)
        return;
    
    mContainsObject = true;
    
	Effect::touch(dwarf,troll);
	
//	removeChild(_sprite);
    
    if(dwarf)
    {
        _dwarfType = dwarf->getType();
        _dwarf = dwarf;
        _dwarf->setVisible(false);
        _dwarf->setDisabled(true);
        _dwarf->removeMovePoints();
    }
    else
    {
        _dwarfType = -1;
        _troll = troll;
        _troll->setVisible(false);
        _troll->_canMove = false;
    }
    
    //Play the intro
//    addChild(_startAnimation);
    schedule(schedule_selector(Wind::startStuckAnim), 0, 0, 0.0f);
}