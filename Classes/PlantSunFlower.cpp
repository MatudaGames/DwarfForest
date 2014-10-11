//
//  Plant_SunFlower.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 10/8/13.
//
//

#include "PlantSunFlower.h"
#include "GameScene.h"
#include "SpriteAnimation.h"
#include "User.h"
#include "Utils.h"

USING_NS_CC;

PlantSunFlower* PlantSunFlower::create(GameScene* gameScene)
{
	PlantSunFlower *pRet = new PlantSunFlower();
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

PlantSunFlower::PlantSunFlower():
_LeafRight(NULL),_LeafLeft(NULL),_LeafCenter(NULL),_LeafBottom(NULL),_animation(NULL)
{
}

PlantSunFlower::~PlantSunFlower()
{
    if (_animation) _animation->release();
}

bool PlantSunFlower::init(GameScene* gameScene)
{
	if (!CCNode::init())
	{
		return false;
	}
    
    mTouched = false;
    mCanTouch = false;
    
    _gameScene = gameScene;
    
    //The magic
    _LeafCenter = CCSprite::create("powerup/Sun_Flower/DFsunflower.png");
    _LeafCenter->setAnchorPoint(ccp(0.5,0));
    
    //Grow up an pop
    _LeafCenter->setScale(0.0);
    addChild(_LeafCenter);
    
    //scale up
    CCScaleTo* aScaleUp_4 = CCScaleTo::create(30, 1.0);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(PlantSunFlower::onFinishedGrow));
    CCSequence* seqfin = CCSequence::create(aScaleUp_4,func,NULL);
    _LeafCenter->runAction(seqfin);
    
    return true;
}

void PlantSunFlower::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	CCNode::onEnter();
}

void PlantSunFlower::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCNode::onExit();
}

void PlantSunFlower::onFinishedGrow()
{
    //Pop some random crystals
    //Can touch now
    mCanTouch = true;
//    schedule(schedule_selector(Plant_SunFlower::removeThisPlantAndMakeNew), 30.0f, 0, 0.0f);//Not for the sunflower
    
    _animation = SpriteAnimation::create("Crystals/shiny_stuff.plist");
    _animation->retain();
    _animation->setPositionY(_LeafCenter->getContentSize().height);
    addChild(_animation);
}

void PlantSunFlower::removeThisPlantAndMakeNew()
{
    mCanTouch = false;
    unschedule(schedule_selector(PlantSunFlower::removeThisPlantAndMakeNew));
    _gameScene->removeNode(this);
    //Create new plant
    _gameScene->generatePlantSunFlower();
}

//The touch stuff
bool PlantSunFlower::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (!mCanTouch)
        return false;
    
    bool result = false;
	mTouched = false;
    
	if (ccpDistanceSQ(touch->getLocation(), CCPoint(getPositionX()+20, getPositionY()+70)) <= 450)
	{
        //Try to get money from player !!!
        mTouched = true;
        
        //Check if can activate it ???
        
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
            _gameScene->activateNoEnemies(getPositionX(),getPositionY());
            //Try to end it now !!!
            _gameScene->removeNode(this);
        }
	}
	
	return result;
}

void PlantSunFlower::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (mTouched)
    {
        
    }
}
