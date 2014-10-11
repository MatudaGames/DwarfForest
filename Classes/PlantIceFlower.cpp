//
//  Plant_IceFlower.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 10/8/13.
//
//

#include "PlantIceFlower.h"
#include "GameScene.h"
#include "SpriteAnimation.h"
#include "User.h"
#include "Utils.h"

USING_NS_CC;

PlantIceFlower* PlantIceFlower::create(GameScene* gameScene)
{
	PlantIceFlower *pRet = new PlantIceFlower();
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

PlantIceFlower::PlantIceFlower():
_LeafRight(NULL),_LeafLeft(NULL),_LeafCenter(NULL),_LeafBottom(NULL),_animation(NULL)
{
}

PlantIceFlower::~PlantIceFlower()
{
    if (_animation) _animation->release();
}

bool PlantIceFlower::init(GameScene* gameScene)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    mTouched = false;
    mCanTouch = false;
    
    _gameScene = gameScene;
    
    //The magic
    _LeafCenter = CCSprite::create("powerup/Ice_Flower/DFIceFlower.png");
    
    //Grow up an pop
    _LeafCenter->setScale(0.0);
    _LeafCenter->setAnchorPoint(ccp(0.5,0));
    addChild(_LeafCenter);
    
    //scale up
    CCScaleTo* aScaleUp_4 = CCScaleTo::create(30, 1.0);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(PlantIceFlower::onFinishedGrow));
    CCSequence* seqfin = CCSequence::create(aScaleUp_4,func,NULL);
    _LeafCenter->runAction(seqfin);
    
    return true;
}

void PlantIceFlower::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void PlantIceFlower::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}

void PlantIceFlower::onFinishedGrow()
{
    //Pop some random crystals
    //Can touch now
    mCanTouch = true;
    //Start count time to remove it - if will not touch it !!!
//    schedule(schedule_selector(Plant_IceFlower::removeThisPlantAndMakeNew), 30.0f, 0, 0.0f);
    
    _animation = SpriteAnimation::create("Crystals/shiny_stuff.plist");
    _animation->retain();
    _animation->setPositionY(_LeafCenter->getContentSize().height);
    addChild(_animation);
}

void PlantIceFlower::removeThisPlantAndMakeNew()
{
    mCanTouch = false;
    unschedule(schedule_selector(PlantIceFlower::removeThisPlantAndMakeNew));
    _gameScene->removeNode(this);
    //Create a new node somewhere?
    _gameScene->generatePlantIceFlower();
}

//The touch stuff
bool PlantIceFlower::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (!mCanTouch)
        return false;
    
    bool result = false;
	mTouched = false;
    
	if (ccpDistanceSQ(touch->getLocation(), CCPoint(getPositionX()+20, getPositionY()+70)) <= 450)
	{
        unschedule(schedule_selector(PlantIceFlower::removeThisPlantAndMakeNew));
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
            _gameScene->activateGhoustDwarfs(getPositionX(),getPositionY());
            //Try to end it now !!!
            _gameScene->removeNode(this);
        }
        
        //Activate the booster clear all
//        _gameScene->menuPowerupSlowPlayCallback(NULL);
        //Try to end it now !!!
//        _gameScene->removeNode(this);
	}
	
	return result;
}

void PlantIceFlower::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (mTouched)
    {
        
    }
}