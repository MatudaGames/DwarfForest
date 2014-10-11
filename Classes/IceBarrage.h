//
//  IceBarrage.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 17.03.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>
#include "Effect.h"

class SpriteAnimation;
class GameScene;

class IceBarrage: public Effect
{
public:
	static IceBarrage* create(GameScene* gameScene);
    
	IceBarrage();
    virtual ~IceBarrage();
	
	bool init(GameScene* gameScene);
	
	virtual void touch(Dwarf* dwarf,Troll* troll);
	
	virtual void tap();
    
    void onAnimFreezeFinish();
    void onShowAnimUnFreeze();
    void onAnimUnFreezeFinish();
	
private:
	cocos2d::CCPoint mOffset;
    
    GameScene* _game;
	bool _canTapIce;
	
	cocos2d::CCArray* _sprites;
	
	cocos2d::CCSprite* _iceBarrage1;
	cocos2d::CCSprite* _iceBarrage2;
	cocos2d::CCSprite* _iceBarrage3;
	cocos2d::CCSprite* _iceBarrage4;
    
    SpriteAnimation* _freezeAnim;
    SpriteAnimation* _unfreezeAnim;
};
