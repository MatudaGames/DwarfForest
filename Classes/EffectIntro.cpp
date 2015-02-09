//
//  EffectIntro.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 10.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <SimpleAudioEngine.h>
#include "EffectIntro.h"
#include "GameScene.h"
#include "SpriteAnimation.h"
#include "Effect.h"
#include "CCShake.h"
#include "User.h"

USING_NS_CC;
using namespace CocosDenshion;

EffectIntro* EffectIntro::create(GameScene* game, Effect* effect)
{
	EffectIntro *pRet = new EffectIntro();
    if (pRet && pRet->init(game, effect))
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

EffectIntro::EffectIntro():
	_shadowSprite(NULL), _meteoriteSprite(NULL), _collisionAnimation(NULL),
	_fallIntro(NULL)
{
}

EffectIntro::~EffectIntro()
{
	if (_collisionAnimation) _collisionAnimation->release();
}

void EffectIntro::OnRainCompleted()
{
    if(User::getInstance()->_tutorial_small_8==0)
    {
        //1st time pause the game and show some stuff !!!
        _game->stopInGameSound("Footsteps",true);
        _game->stopInGameSound("troll_walk",true);
        _game->stopInGameSound("dwarf_web_stuck",true);
        
        //Get the mid point !!!
        int aShowX = getPositionX();
        int aShowY = getPositionY();
        
        _game->CreateSmallTutorial(8,aShowX,aShowY);
    }
    
    _meteoriteFadeIn->release();
    _meteoriteFadeIn = NULL;
    
    _areaSprite->setOpacity(255);
    _collisionAnimation->stopAllActions();
    
    removeChild(_collisionAnimation);
    schedule(schedule_selector(IntroAnimation::finished), 0.0f, 1, 0.0f);
}

void EffectIntro::OnWebCompleted()
{
	stickyGrowAnimation->stopAllActions();
	removeChild(stickyGrowAnimation);
    schedule(schedule_selector(IntroAnimation::finished), 0.0f, 1, 0.0f);
}

void EffectIntro::WebIntro()
{
	 CCScaleTo* aScale = CCScaleTo::create(1.5f, GLOBAL_SCALE);
	 stickyGrowAnimation = SpriteAnimation::create("sticky_web/trap_sticky_intro.plist");
	 stickyGrowAnimation->setScale(GLOBAL_SCALE);
     stickyGrowAnimation->retain();
     stickyGrowAnimation->setTag(45);
     addChild(stickyGrowAnimation);
}

bool EffectIntro::init(GameScene* game, Effect* effect)
{
	if (!IntroAnimation::init(INTRO_EFFECT, game))
	{
		return false;
	}
    
    _placeSprite = NULL;
	
	_effect = effect;
    
    if(effect->getType() == EFFECT_TYPE_RAIN)
    {
        //Start some fade in !!!
        _collisionAnimation = SpriteAnimation::create("effects/rain/puddle_anim.plist",false);
        _collisionAnimation->retain();
        addChild(_collisionAnimation);
        
        //Some other logic !!!
//        _meteoriteSprite = CCSprite::create("effects/rain/rain_holder.png");
//        _meteoriteSprite->setOpacity(0);
//        addChild(_meteoriteSprite);
        
        _areaSprite = CCSprite::create("effects/rain/puddle_ready.png");
        _areaSprite->setOpacity(0);
        addChild(_areaSprite);
        
        //Just fade in the place
        _meteoriteFadeIn = CCDelayTime::create(1.0f);
        _meteoriteFadeIn->retain();
        
        CCCallFuncN* aFunc = CCCallFuncN::create(this, callfuncN_selector(EffectIntro::OnRainCompleted));
        CCSequence* aSeqFun = CCSequence::create(_meteoriteFadeIn,aFunc,NULL);
        
        runAction(aSeqFun);
//        runAction(_meteoriteFadeIn);
        
        return true;
    }
    
    	if(effect->getType() == EFFECT_TYPE_WEB)
    {
        //stickyIntroAnimation = SpriteAnimation::create("sticky_web/trap_sticky_intro.plist",true);
        //stickyIntroAnimation->retain();
        //addChild(stickyIntroAnimation);
        
        CCScaleTo* aScale = CCScaleTo::create(1.5f, GLOBAL_SCALE);
        _meteoriteFadeIn = CCDelayTime::create(0.0f);
        _meteoriteFadeIn->retain();
        _introDelay = CCDelayTime::create(2.0f);
        _introDelay->retain();
        
        
        CCCallFuncN* aFunc = CCCallFuncN::create(this, callfuncN_selector(EffectIntro::WebIntro));
        CCCallFuncN* bFunc = CCCallFuncN::create(this, callfuncN_selector(EffectIntro::OnWebCompleted));
        CCSequence* aSeqFun = CCSequence::create(aFunc,_introDelay,bFunc,NULL);//,bFunc,NULL);
        runAction(aSeqFun);
        
        /*_meteoriteFadeIn,aScale,*/
        
        removeChildByTag(45);
        
        return true;
    }
    
    _placeSprite = CCSprite::create("meteo_fall.png");
    addChild(_placeSprite);
    
	
	_shadowSprite = CCSprite::create("meteorits/eena.png");
	_meteoriteSprite = CCSprite::create("meteorits/meteorits.png");
    
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
        _shadowSprite->setScale(GLOBAL_SCALE);
        _meteoriteSprite->setScale(GLOBAL_SCALE);
    }
	
//	_collisionAnimation = SpriteAnimation::create("meteorits/sadursme.plist");
//	_collisionAnimation->retain();
//    addChild(_collisionAnimation);
	
	addChild(_shadowSprite);
	addChild(_meteoriteSprite);
	
    _shadowSprite->setScale(0.0f);
    CCDelayTime* aDelay = CCDelayTime::create(0.0f);
    CCScaleTo* aScale = CCScaleTo::create(1.5f, GLOBAL_SCALE);
    _fallIntro = CCSequence::create(aDelay,aScale,NULL);
    _fallIntro->retain();
    _shadowSprite->runAction(_fallIntro);
    
	_meteoriteSprite->setPositionY(2000.0f);
	
	_meteoriteFadeIn = CCMoveTo::create(1.5f, ccp(0.0f, _meteoriteSprite->getContentSize().height / 2));
	_meteoriteFadeIn->retain();
	_meteoriteSprite->runAction(_meteoriteFadeIn);
    
    //New stuff
    _boomSprite = CCSprite::create("meteorits/boom.png");
    _areaSprite = CCSprite::create("meteorits/effect_area.png");
    
    //For debug
    if (GLOBAL_SCALE!=1.0f)
    {
        _boomSprite->setScale(GLOBAL_SCALE);
        _areaSprite->setScale(GLOBAL_SCALE);
    }
    
    _boomSprite->setScale(0);
    _areaSprite->setScale(0);
    
    addChild(_boomSprite);
    addChild(_areaSprite);
	
	return true;
}

void EffectIntro::update(float delta)
{
    if (_fallIntro && _fallIntro->isDone())
    {
        _fallIntro->release();
        _fallIntro = NULL;
    }
    
	if (_meteoriteFadeIn && _meteoriteFadeIn->isDone())
	{
		_meteoriteFadeIn->release();
		_meteoriteFadeIn = NULL;
        
        if(_effect->getType() == EFFECT_TYPE_RAIN)
        {
//            _areaSprite->setOpacity(255);
//            
//            //Other plans
////            removeChild(_meteoriteSprite);
//            _collisionAnimation->stopAllActions();
//            
//            removeChild(_collisionAnimation);
//            schedule(schedule_selector(IntroAnimation::finished), 0.0f, 1, 0.0f);
            return;
        }
        
        if(_effect->getType() == EFFECT_TYPE_WEB || _effect->getType() == EFFECT_TYPE_ICE_BARRAGE )
        {
            if(User::getInstance()->_tutorial_small_9==0)
            {
                //1st time pause the game and show some stuff !!!
                _game->stopInGameSound("Footsteps",true);
                _game->stopInGameSound("troll_walk",true);
                _game->stopInGameSound("dwarf_web_stuck",true);
                
                //Get the mid point !!!
                int aShowX = getPositionX();
                int aShowY = getPositionY();
                
                _game->CreateSmallTutorial(9,aShowX,aShowY);
            }
        }
        
        if(_placeSprite!=NULL){
            removeChild(_placeSprite);
        }
		
		removeChild(_shadowSprite);
		removeChild(_meteoriteSprite);
        
        //Create code anim =)
        CCScaleTo* aScaleUp = CCScaleTo::create(0.25f, GLOBAL_SCALE);
        _areaSprite->runAction(aScaleUp);
        
        CCScaleTo* aScaleUp2 = CCScaleTo::create(0.25f, GLOBAL_SCALE);
        CCFadeOut* aFadeOut = CCFadeOut::create(0.25f);
        CCSpawn* aCombo = CCSpawn::create(aScaleUp2,aFadeOut,NULL);
        _boomSprite->runAction(aCombo);
        
//        _game->playInGameSound("meteorite_hit_ground");
		
		_game->runAction(CCShake::actionWithDuration( 1.0f, 10.0f ));
		
        schedule(schedule_selector(IntroAnimation::finished), 0.0f, 1, 0.5f);
	}
}

Effect* EffectIntro::getEffect() const
{
	return _effect;
}

