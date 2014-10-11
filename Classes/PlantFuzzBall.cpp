//
//  Plant_FuzzBall.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 10/6/13.
//
//

#include "PlantFuzzBall.h"
#include "GameScene.h"
//#include "Dwarf.h"
#include "SpriteAnimation.h"
#include "User.h"
#include "Utils.h"

USING_NS_CC;

PlantFuzzBall* PlantFuzzBall::create(GameScene* gameScene)
{
	PlantFuzzBall *pRet = new PlantFuzzBall();
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

PlantFuzzBall::PlantFuzzBall():
_LeafRight(NULL),_LeafLeft(NULL),_LeafCenter(NULL),_LeafBottom(NULL),_animation(NULL)
{
}

PlantFuzzBall::~PlantFuzzBall()
{
    if (_animation) _animation->release();
}

bool PlantFuzzBall::init(GameScene* gameScene)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    mTouched = false;
    mCanTouch = false;
    
    _gameScene = gameScene;
    
    //The magic
    _LeafCenter = CCSprite::create("powerup/Fuzz_Ball/DFFuzzball.png");
    
    //Grow up an pop
    _LeafCenter->setScale(0.0);
    _LeafCenter->setAnchorPoint(ccp(0.5,0));
    addChild(_LeafCenter);
    
    //scale up
    CCScaleTo* aScaleUp_4 = CCScaleTo::create(30, 1.0);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(PlantFuzzBall::onFinishedGrow));
    CCSequence* seqfin = CCSequence::create(aScaleUp_4,func,NULL);
    _LeafCenter->runAction(seqfin);
    
    return true;
}

void PlantFuzzBall::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void PlantFuzzBall::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}

void PlantFuzzBall::onFinishedGrow()
{
    //Pop some random crystals
    //Can touch now
    mCanTouch = true;
    
    _animation = SpriteAnimation::create("Crystals/shiny_stuff.plist");
    _animation->retain();
    _animation->setPositionY(_LeafCenter->getContentSize().height);
    addChild(_animation);
}

//The touch stuff
bool PlantFuzzBall::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (!mCanTouch)
        return false;
    
    bool result = false;
	mTouched = false;
    
	if (ccpDistanceSQ(touch->getLocation(), CCPoint(getPositionX()+20, getPositionY()+70)) <= 450)
	{
        //Try to get money from player !!!
        mTouched = true;
        
        int aPrice = 2;
        //Check if has money - if not - ask to buy? how will this work in game?
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);
        if (aDidUseDiamonds<0)
        {
            //Show popup that no money
            _gameScene->createTextFly("No\nDiamonds", ccp(getPositionX(),getPositionY()), 1);
        }
        else
        {
            _gameScene->getDiamondsLabel()->setString(toString(User::getInstance()->getDiamonds()).c_str());
            
            //Activate the booster clear all
            _gameScene->activateClearMap(getPositionX(),getPositionY());
            //Try to end it now !!!
            _gameScene->removeNode(this);
        }
        
        //Activate the booster clear all
//        _gameScene->menuPowerupNoEnemiesCallback(NULL);
//        _gameScene->clearAllMap();
        //Try to end it now !!!
//        _gameScene->removeNode(this);
	}
	
	return result;
}

void PlantFuzzBall::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (mTouched)
    {
//        //Activate the booster clear all
//        _gameScene->menuPowerupNoEnemiesCallback(NULL);
//        //Try to end it now !!!
//        _gameScene->removeNode(this);
    }
}
