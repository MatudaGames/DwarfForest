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
class SpriteAnimation;

class TrollBullet: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
	static TrollBullet* create(GameScene* game,int theType);
	
	TrollBullet();
	virtual ~TrollBullet();
	
	virtual bool init(GameScene* game,int theType);
	
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
    
    void OnDoAction(Dwarf* theForced); // When want to do some bullet action
    
    cocos2d::CCSprite* _sprite;
    
    cocos2d::CCPoint _straightCords;
    Dwarf* _dwarf;
    
    float _angle;
    float _speed; //Current speed
    
    float _type;// Ice,Poison or kaboom
    
    float _speedMax; //Ends with fast
    float _speedAddValue;
    
    float _timeActive; // new stuff for totem bullets with life
    
    float _distanceActive;
    int _startX;
    int _startY;
    
    float GetAngle();
    void setAngle(float value);
    
    float timeToStartFly;
    SpriteAnimation* _animation;
    
	
private:
	GameScene* _game;
};
