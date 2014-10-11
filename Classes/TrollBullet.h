//
//  TrollBullet.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 01/08/14.
//
//

#pragma once

#include <cocos2d.h>
#include "Dwarf.h"

class GameScene;

class TrollBullet: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
	static TrollBullet* create(GameScene* game);
	
	TrollBullet();
	virtual ~TrollBullet();
	
	virtual bool init(GameScene* game);
	
	virtual void update(float delta);
    
    virtual void onEnter();
	virtual void onExit();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    bool _touched;
    bool _canMove;
    bool _touchable;
    bool _isDisabled;
    
    cocos2d::CCSprite* _sprite;
    
    Dwarf* _dwarf;
    
    float _angle;
    float _speed;
    
    float GetAngle();
    void setAngle(float value);
	
private:
	GameScene* _game;
};
