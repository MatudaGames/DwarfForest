//
//  Rain.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 13/01/14.
//
//

#pragma once

#include <cocos2d.h>
#include "Effect.h"
#include "Dwarf.h"

class SpriteAnimation;
class GameScene;

class Rain: public Effect
{
public:
	static Rain* create(GameScene* gameScene);
	
	Rain();
    virtual ~Rain();
	
	virtual bool init(GameScene* gameScene);
	
	virtual void touch(Dwarf* dwarf,Troll* troll);
    
    void onComplete();
    void finishStuckAnim();
    void startStuckAnim();
    
    void FadeOutEffect();
	
private:
	GameScene *_game;
	
	SpriteAnimation* _animation;
    
    SpriteAnimation* _crashAnimation;
    
    SpriteAnimation* _startAnimation;
    SpriteAnimation* _endAnimation;
	
	int _dwarfType;
    Dwarf* _dwarf;
};