//
//  IceBarrage.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 17.03.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "IceBarrage.h"
#include "GameScene.h"
#include "Dwarf.h"
#include "SpriteAnimation.h"
#include "User.h"

USING_NS_CC;

IceBarrage* IceBarrage::create(GameScene* gameScene)
{
	IceBarrage *pRet = new IceBarrage();
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

IceBarrage::IceBarrage():
	_iceBarrage1(NULL), _iceBarrage2(NULL), _iceBarrage3(NULL), _iceBarrage4(NULL), _sprites(NULL), _freezeAnim(NULL), _unfreezeAnim(NULL)
{
}

IceBarrage::~IceBarrage()
{
	if (_sprites) _sprites->release();
    if (_freezeAnim) _freezeAnim->release();
    if (_unfreezeAnim) _unfreezeAnim->release();
}

bool IceBarrage::init(GameScene* gameScene)
{
	if (!Effect::init(gameScene))
	{
		return false;
	}
    
    _canTapIce = false;
    _needToRemove = false;
    
    mOffset = ccp(0,15);
	
	_effectType = EFFECT_TYPE_ICE_BARRAGE;
    
    _game = gameScene;
	
	//TODO: change
	_sprite = CCSprite::create("meteorits/effect_area.png");
	addChild(_sprite);
	
	_iceBarrage1 = CCSprite::create("ice_barrage/state_1.png");
	_iceBarrage2 = CCSprite::create("ice_barrage/state_2.png");
	_iceBarrage3 = CCSprite::create("ice_barrage/state_3.png");
	_iceBarrage4 = CCSprite::create("ice_barrage/state_4.png");
    
    _iceBarrage1->setPosition(mOffset);
    _iceBarrage2->setPosition(mOffset);
    _iceBarrage3->setPosition(ccp(mOffset.x-1,mOffset.y-4));
    _iceBarrage4->setPosition(ccp(mOffset.x,mOffset.y-2));
	
	_sprites = CCArray::create();
	_sprites->retain();
	
	_sprites->addObject(_iceBarrage1);
	_sprites->addObject(_iceBarrage2);
	_sprites->addObject(_iceBarrage3);
	_sprites->addObject(_iceBarrage4);
    
    //The animations
    _freezeAnim = SpriteAnimation::create("ice_barrage/freezing_animation.plist");
    _freezeAnim->retain();
    _freezeAnim->setTag(10);
    
    _unfreezeAnim = SpriteAnimation::create("ice_barrage/breaking_out.plist");
    _unfreezeAnim->retain();
    _unfreezeAnim->setTag(11);
    
    //Offsets
    _freezeAnim->setPosition(ccp(12+mOffset.x,24+mOffset.y));//24 is good
    _unfreezeAnim->setPosition(ccp(4+mOffset.x,0+mOffset.y));//0 is good
    
    if (GLOBAL_SCALE!=1.0f)
    {
        _iceBarrage1->setScale(GLOBAL_SCALE);
        _iceBarrage2->setScale(GLOBAL_SCALE);
        _iceBarrage3->setScale(GLOBAL_SCALE);
        _iceBarrage4->setScale(GLOBAL_SCALE);
        
        _sprite->setScale(GLOBAL_SCALE);
        
        _freezeAnim->setScale(GLOBAL_SCALE);
        _unfreezeAnim->setScale(GLOBAL_SCALE);
    }
	
	return true;
}

void IceBarrage::onAnimFreezeFinish()
{
    unschedule(schedule_selector(IceBarrage::onAnimFreezeFinish));
    //Show the 1st freeze step and allow to click
    _canTapIce = true;
    
    removeChild(_freezeAnim);
    addChild((CCSprite*)_sprites->objectAtIndex(_tapCount));
}

void IceBarrage::onShowAnimUnFreeze()
{
    schedule(schedule_selector(IceBarrage::onAnimUnFreezeFinish), 0.0f, 1, 0.3f);
    addChild(_unfreezeAnim);
}

void IceBarrage::onAnimUnFreezeFinish()
{
    unschedule(schedule_selector(IceBarrage::onAnimUnFreezeFinish));
    removeChild(_unfreezeAnim);
    //We finished it
    _needToRemove = true;
}

void IceBarrage::touch(Dwarf* dwarf,Troll* troll)
{
	Effect::touch(dwarf,NULL);
	
	removeChild(_sprite);
	
    //Play the freeze anim
    schedule(schedule_selector(IceBarrage::onAnimFreezeFinish), 0.0f, 1, 0.7f);//0.7
    addChild(_freezeAnim);
    
//    addChild((CCSprite*)_sprites->objectAtIndex(_tapCount));
//    schedule(schedule_selector(IceBarrage::onAnimFreezeFinish), 0.0f, 1, 0.1f);//0.7
//    addChild(_freezeAnim);
}

void IceBarrage::tap()
{
    if (!_canTapIce)
        return;
    
	Effect::tap();
	
	if (_tapCount <= 4)
	{
        if (_tapCount<4)
            ((CCSprite*)_sprites->objectAtIndex(_tapCount))->setVisible(true);
        
		removeChild((CCSprite*)_sprites->objectAtIndex(_tapCount - 1));
        
        if (_tapCount==4)
        {
            _canTapIce = false;
            onShowAnimUnFreeze();
            _game->playInGameSound("dwarf_unfreeze");
            
            _game->RemoveSmallTutorial();//Continue the game !!!
        }
        else
        {
            char aSoundFX[32];
            sprintf(aSoundFX,"ice_break_%i",_tapCount);
            //Add some ranom here?
            _game->playInGameSound(aSoundFX);
        }
	}
	
	if (_tapCount < 4)
	{
		addChild((CCSprite*)_sprites->objectAtIndex(_tapCount));
	}
}
