//
//  Web.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 17.03.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>
#include "Effect.h"
#include "Dwarf.h"

class SpriteAnimation;
class GameScene;

class Web: public Effect
{
public:
	static Web* create(GameScene* gameScene);
	
	Web();
    virtual ~Web();
	
	virtual bool init(GameScene* gameScene);
	
	virtual void touch(Dwarf* dwarf,Troll* troll);
    
    void onComplete();
    void finishStuckAnim();
    void startStuckAnim();
    void pauseGrow();
    void shotBulletAtDwarf();
	
private:
	GameScene *_game;
	
	cocos2d::CCActionInterval* _growDelay;
	
	SpriteAnimation* _animation;
    SpriteAnimation* _growAnimation;
    SpriteAnimation* _startAnimation;
    SpriteAnimation* _endAnimation;
	
	int _dwarfType;
    Dwarf* _dwarf;
};
