//
//  Rain.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 13/01/14.
//
//

#include "Rain.h"
#include "GameScene.h"
#include "Dwarf.h"
#include "SpriteAnimation.h"
#include "User.h"

USING_NS_CC;

Rain* Rain::create(GameScene* gameScene)
{
	Rain *pRet = new Rain();
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

Rain::Rain():
_animation(NULL),_startAnimation(NULL),_endAnimation(NULL),_crashAnimation(NULL)
{
}

Rain::~Rain()
{
    if (_animation) _animation->release();
    if (_startAnimation) _startAnimation->release();
    if (_endAnimation) _endAnimation->release();
    if (_crashAnimation) _crashAnimation->release();
}

bool Rain::init(GameScene* gameScene)
{
	if (!Effect::init(gameScene))
	{
		return false;
	}
    
    _needToRemove = false;
	
	_effectType = EFFECT_TYPE_RAIN;
	
	_sprite = CCSprite::create("effects/rain/puddle_ready.png");
    _sprite->setPosition(ccp(0.5,-0.5));
	addChild(_sprite);
    
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
    
    _dwarfType = 0;
    
    _game = gameScene;
	
	return  true;
}

void Rain::startStuckAnim()
{
    unschedule(schedule_selector(Rain::startStuckAnim));
    _dwarf->setAnimationVisibility(false);
    
    //Add the chiv chiv anim above !!!
    _crashAnimation = SpriteAnimation::create("effects/crash_stars.plist");
    _crashAnimation->retain();
    _crashAnimation->setOpacity(0);
    
    CCFadeIn* aFadeIn = CCFadeIn::create(0.1f);
    CCDelayTime* aDelay = CCDelayTime::create(0.5f);
    CCSequence* aSeq = CCSequence::create(aDelay,aFadeIn,NULL);
    _crashAnimation->runAction(aSeq);
    
//    _crashAnimation->setOpacity(120);
    
    if (_dwarfType == DWARF_TYPE_FAT)
    {
        _animation = SpriteAnimation::create("Characters/fat_dwarf/fatdwarf_puddlefall.plist",false);
        _animation->retain();
        _animation->setPositionY(20);
        
        //Check if need to mirror it !!!
        if(_dwarf->_direction<4)
        {
            _animation->setFlipX(true);
            _crashAnimation->setPosition(ccp(_animation->getContentSize().width/2+10,_animation->getContentSize().height/2+16));
        }
        else
        {
            _crashAnimation->setPosition(ccp(_animation->getContentSize().width/2,_animation->getContentSize().height/2+16));
        }
        
        _animation->addChild(_crashAnimation);
    }
    else if (_dwarfType == DWARF_TYPE_TALL)
    {
        _animation = SpriteAnimation::create("Characters/tall_dwarf/talldwarf_puddlefall.plist",false);
        _animation->retain();
        _animation->setPositionY(20);
        
        if(_dwarf->_direction<4)
        {
            _animation->setFlipX(true);
            _crashAnimation->setPosition(ccp(_animation->getContentSize().width/2+10,_animation->getContentSize().height/2+16));
        }
        else
        {
            _crashAnimation->setPosition(ccp(_animation->getContentSize().width/2,_animation->getContentSize().height/2+16));
        }
        _animation->addChild(_crashAnimation);
    }
    
//    _game->playInGameSound("dwarf_web_stuck");
    
    if (GLOBAL_SCALE!=1.0f)
    {
        _animation->setScale(GLOBAL_SCALE);
    }
    
//    removeChild(_startAnimation);
    addChild(_animation);
    schedule(schedule_selector(Rain::finishStuckAnim), 0.0f, 0, 4.0f);
}

void Rain::FadeOutEffect()
{
    CCFadeOut* aFade = CCFadeOut::create(1.0f);
    _sprite->runAction(aFade);
}

void Rain::finishStuckAnim()
{
    unschedule(schedule_selector(Rain::finishStuckAnim));
    
//    if (_dwarfType == DWARF_TYPE_FAT)
//    {
//        _endAnimation = SpriteAnimation::create("sticky_web/fatass_free.plist");
//        _endAnimation->retain();
//    }
//    else if (_dwarfType == DWARF_TYPE_TALL)
//    {
//        _endAnimation = SpriteAnimation::create("sticky_web/tallass_free.plist");
//        _endAnimation->retain();
//    }
//    
//    if (GLOBAL_SCALE!=1.0f)
//    {
//        _endAnimation->setScale(GLOBAL_SCALE);
//    }
//    
//    _game->stopInGameSound("dwarf_web_stuck");
//    _game->playInGameSound("dwarf_web_exit");
    
    removeChild(_animation);
//    addChild(_endAnimation);
    schedule(schedule_selector(Rain::onComplete), 0.0f, 1, 0.0f);
}

void Rain::onComplete()
{
    unschedule(schedule_selector(Rain::onComplete));
    
    _needToRemove = true;
    _dwarf->setAnimationVisibility(true);
    
    //Remove the warning
}

void Rain::touch(Dwarf* dwarf,Troll* troll)
{
	Effect::touch(dwarf,NULL);
	
	removeChild(_sprite);
    
    _dwarfType = dwarf->getType();
    _dwarf = dwarf;
    
    _dwarf->clearCrash(false);
    
    _dwarf->ClearAlarm();
    
    //Play the intro
//    addChild(_startAnimation);
    schedule(schedule_selector(Rain::startStuckAnim), 0, 0, 0.0f);
}