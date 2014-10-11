//
//  IntroAnimation.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 06.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "IntroAnimation.h"
#include "SpriteAnimation.h"
#include "GameScene.h"
#include "User.h"

USING_NS_CC;

IntroAnimation::IntroAnimation():
	_game(NULL), _finished(false)
{
}

IntroAnimation::~IntroAnimation()
{
	if (_game) _game->release();
}

bool IntroAnimation::init(IntroAnimationType type, GameScene* game)
{
	_type = type;
	_game = game;
	_game->retain();
	
	return true;
}

void IntroAnimation::update(float delta)
{
	
}

IntroAnimationType IntroAnimation::getType() const
{
	return _type;
}

void IntroAnimation::finished(CCObject* sender)
{
	if (!_finished)
	{
		_finished = true;
		
		if (_game) _game->introFinished(this);
	}
}
