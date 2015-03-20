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
_game(NULL), _animation(NULL), _idleAnim(NULL), _spell_1_Anim(NULL), _spell_2_Anim(NULL) 
{
    
}

MasterDwarf::~MasterDwarf()
{
    if (_game)
    {
        _game->release();
    }
    if (_idleAnim) _idleAnim->release();
    if (_spell_1_Anim) _spell_1_Anim->release();
    if (_spell_2_Anim) _spell_2_Anim->release();
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
    //_base->setAnchorPoint(ccp(0.5f,1));
    addChild(_base);
    
    // The bottom of MD
    mBasePad = CCSprite::create("Characters/master_troll/mt_base.png");
    mBasePad->setAnchorPoint(ccp(0.3f,1));
    _base->addChild(mBasePad);
    
    _idleAnim = SpriteAnimation::create("Characters/dwarf_king/king_idle.plist");
    _idleAnim->retain();
    
    _spell_1_Anim = SpriteAnimation::create("Characters/dwarf_king/king_spell1.plist");
    _spell_1_Anim->retain();
    
    _spell_2_Anim = SpriteAnimation::create("Characters/dwarf_king/king_spell2.plist");
    _spell_2_Anim->retain();
    
    shootSpell = false;
    
    return true;
}



void MasterDwarf::update(float delta)
{
    CCLog("Update Master Dwarf");
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

void MasterDwarf::setAnimationByName(const char* theAnimation)
{
    // The checker if all correct and what to play how long?
    if(strcmp(theAnimation,"Idle") == 0)
    {
        // Lets hit the ground !!!
        setAnimation(_idleAnim);
    }
    else if(strcmp(theAnimation,"Spell1") == 0)
    {
		setAnimation(_spell_1_Anim);
		
    }
    else if(strcmp(theAnimation,"Spell2") == 0)
    {
        setAnimation(_spell_2_Anim);
        
        CCDelayTime* aDelay = CCDelayTime::create(3.0f);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(MasterDwarf::setSpellToDwarf));
        CCSequence* aSeq1 = CCSequence::create(aDelay,func,NULL);
        runAction(aSeq1);
    }
    
}

void MasterDwarf::setSpellToDwarf()
{
	setAnimation(_idleAnim);
    _game->addSpellBullet();
}

void MasterDwarf::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    setAnimation(_idleAnim);
    
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
