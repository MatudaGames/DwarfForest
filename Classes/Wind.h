//
//  Wind.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 13/01/14.
//
//

#pragma once

#include <cocos2d.h>
#include "Effect.h"
#include "Dwarf.h"
#include "Troll.h"

class SpriteAnimation;
class GameScene;

class Wind: public Effect
{
public:
	static Wind* create(GameScene* gameScene);
	
	Wind();
    virtual ~Wind();
	
	virtual bool init(GameScene* gameScene);
	
	virtual void touch(Dwarf* dwarf,Troll* troll);
    
    virtual void update(float delta);
    
    void OnCompletedScale();
    
    void ClearAll();
    void onComplete();
    void onComplete_Special();
    void finishStuckAnim();
    void startStuckAnim();
    
    void CreateTutorialAnim();
    
    void OnCompleteWalkToCircle();
    
//    cocos2d::CCPointArray* _movePointsWind;
    
    void MoveToPos(int theX,int theY,int theCurve);
    
    int mMoveClock;
    int mMoveIndex;
    bool mMoveByNodes;
    float mTimeToLive;
    bool mContainsObject;
    
    float _speed;
    float _angle;
	
private:
	GameScene *_game;
	
    cocos2d::CCSprite* _endFly;
    
	SpriteAnimation* _animation;
    
    SpriteAnimation* _startAnimation;
    SpriteAnimation* _endAnimation;
    
    SpriteAnimation* _activeAnim;
	
	int _dwarfType;
    Dwarf* _dwarf;
    Troll* _troll;
};