//
//  Mushroom.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 31.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

class GameScene;
class Dwarf;

class Mushroom: public cocos2d::CCNode
{
public:
    static Mushroom* create(GameScene* gameScene,int theTimeOnMap);
    
	Mushroom();
    virtual ~Mushroom();
    
    virtual bool init(GameScene* gameScene,int theTimeOnMap);
    
    void onFinishedShowUp();
    void onFinishedTrans();
    void onRemove();
    void createRemove();
	
private:
    GameScene* _gameScene;
	cocos2d::CCSprite* _mushroomSpriteOn;
    cocos2d::CCSprite* _mushroomSpriteOff;
    
    bool _needToRemove;
};
