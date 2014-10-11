//
//  Effect.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 17.03.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "Effect.h"
#include "GameScene.h"
#include "Dwarf.h"
#include "Troll.h"
#include "Rain.h"

USING_NS_CC;

Effect::Effect():
	_tapCount(0), _needToRemove(false)
{
}

Effect::~Effect()
{
}

bool Effect::init(GameScene* gameScene)
{
	if (!CCNode::init())
	{
		return false;
	}
	
	_gameScene = gameScene;
    
    _disabled = false;
	
//	schedule(schedule_selector(Effect::expire), 0.0f, 1, 10.0f);
	
	return true;
}

void Effect::onFinishedIntro()
{
    if(_gameScene->mTutorialEnabled)
    {
        //Remove when need
    }
    else
    {
        schedule(schedule_selector(Effect::expire), 0.0f, 1, 10.0f);//for now disabled
    }
}


void Effect::expire(float delta)
{
    if(this->getType() == EFFECT_TYPE_RAIN)
    {
        Rain* theEffect = (Rain*)this;//->finishStuckAnim;
        theEffect->FadeOutEffect();
    }
    
	_gameScene->removeEffect(this);
}

void Effect::touch(Dwarf* dwarf,Troll* troll = NULL)
{
	unschedule(schedule_selector(Effect::expire));
	
    if(dwarf)
        dwarf->setEffect(this);
    else if(troll)
        troll->setEffect(this);
	
	CCLOG("Touched effect");
}
