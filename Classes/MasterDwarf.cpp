//
//  MasterDwarf.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 06/03/15.
//
//

#include "MasterDwarf.h"




#include <SimpleAudioEngine.h>
#include "GameScene.h"
#include "AppMacros.h"
#include "Utils.h"
#include "User.h"


USING_NS_CC;
using namespace CocosDenshion;

MasterDwarf* MasterDwarf::create(GameScene* game)
{
    MasterDwarf *pRet = new MasterDwarf();
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

MasterDwarf::MasterDwarf():
_game(NULL), _animation(NULL), _upAnimation(NULL), _downAnimation(NULL), _shootDownAnimation(NULL), _shootUpAnimation(NULL)
{
    
}

MasterDwarf::~MasterDwarf()
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

bool MasterDwarf::init(GameScene* game)
{
    if (!CCNode::init())
    {
        return false;
    }
    
    _game = game;
    
    // The base pad where Object stands
    _base = CCSprite::create("small_dot_red.png");
    _base->setAnchorPoint(ccp(0.5f,1));
    addChild(_base);
    
    // The bottom of MT
    mBasePad = CCSprite::create("small_dot_red.png");
    _base->addChild(mBasePad);
    
    _downAnimation = SpriteAnimation::create("Characters/bee/bee_fly_down.plist");
    _downAnimation->retain();
    
    _upAnimation = SpriteAnimation::create("Characters/bee/bee_fly_up.plist");
    _upAnimation->retain();
    
    _shootDownAnimation = SpriteAnimation::create("Characters/bee/bee_shoot_down.plist");
    _shootDownAnimation->retain();
    
    _shootUpAnimation = SpriteAnimation::create("Characters/bee/bee_shoot_up.plist");
    _shootUpAnimation->retain();
    
    return true;
}



void MasterDwarf::update(float delta)
{
    CCLog("Update Master Troll");
    
    
}

void MasterDwarf::setAnimation(SpriteAnimation* animation)
{
    //Glitch Fix!!!
    if(animation->getOpacity()<128){
        animation->setOpacity(255);
    }
    
    if (_animation != animation)
    {
        if (_animation){
            removeChild(_animation);
        }
        
        _animation = animation;
        
        if (_animation){
            addChild(_animation);
        }
    }
}

void MasterDwarf::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    CCNode::onEnter();
}

void MasterDwarf::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCNode::onExit();
}

// The new stuff
void MasterDwarf::SetMissionStuff(MissionSet theMission)
{
    //Check if circle then use the circle stuff
    setPosition(ccp(200,200));//Some def value for now !!!
    
    
}
