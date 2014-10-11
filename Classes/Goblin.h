//
//  Goblin.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 01/09/14.
//
//

#pragma once

#include <cocos2d.h>
#include "Dwarf.h"
#include "Crystal.h"

class GameScene;
class SpriteAnimation;
class Effect;

class Goblin: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
	static Goblin* create(GameScene* game);
	
	Goblin();
	virtual ~Goblin();
	
	virtual bool init(GameScene* game);
	
	virtual void update(float delta);
    
    virtual void onEnter();
	virtual void onExit();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    bool _touched;
    
    bool mRunToExit;
    
    int mExit_X;
    int mExit_Y;
    
    int mTimesTaped;
    float mTimeDown;
    void HitGoblin(bool theRemove);
    
    bool mTutorfix;
    
    Crystal* mCrystalCatch;
    
    float mGoblinScale;
    
    bool mTutorialGoblin;
    bool mMoveByNodes;
    int mMoveIndex;
    bool mMoveClock;
    int mMoveCurrentDir;
    
    //Special stuff
    bool mPatrolPoints;
    bool mMoveToControl_1;
    cocos2d::CCPoint mGoblinPatrolPoint_1;
    cocos2d::CCPoint mGoblinPatrolPoint_2;
	
    cocos2d::CCSprite* mDrawLine;
    int mDrawLineID;
    
	void setAngle(float value);
	void setAnimation(SpriteAnimation* animation);
	
	bool getTouchable();
    
    void setFadeIn();
    void setVictory();
    void removeFromSave();
    void setForceRemove();
    
    void setAnimationVisibility(bool theValue);
    
    float GetAngle();
    
    void setEffect(Effect* value);
    void removeEffect();
    
    void SetDrawLine(int theLine);
    
	bool getCanMove() const { return _canMove; }
	void setCanMove(bool value) { _canMove = value; }
	
	bool getForceRemove() const { return _forceRemove; }
	bool getIsDisabled() { return _isDisabled; }
    
    bool blitzHit;
    SpriteAnimation* _animation;
    
    bool _canMove;
    
    float _waitForReturn;
    
	float _angle;
	float _speed;
    
    float _lastSetAngle;
    
    float _timeInGame;
    
    void CatchDwarf(Dwarf* theDwarf);
    void CancelDwarfCatch(Dwarf* theCancelDwarf);
    bool mCatchingCrystal;
    float mStartCatchCrystal;
    Dwarf* mDwarfToCatch;
    cocos2d::CCSprite* mCatchRadar;
    cocos2d::CCSprite* mWarnIcon;
    float mGoblinStartY;
    
    cocos2d::CCSprite* mHeadIcon;
    
    SpriteAnimation* _crashAnimation;
    
    bool _touchable;
	
private:
	GameScene* _game;
	
	SpriteAnimation* _leftDownAnimation;
	SpriteAnimation* _leftUpAnimation;
	SpriteAnimation* _leftAnimation;
	SpriteAnimation* _upAnimation;
	SpriteAnimation* _downAnimation;
    SpriteAnimation* _victory;
    SpriteAnimation* _removeSmoke;
	
    
	
	
	
    bool _isDisabled;
    bool _forceRemove;
};