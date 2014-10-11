//
//  Spider.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 14/09/14.
//
//

#pragma once

#include <cocos2d.h>
#include "Dwarf.h"
#include "Crystal.h"

class GameScene;
class SpriteAnimation;
class Effect;

class Spider: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
	static Spider* create(GameScene* game);
	
	Spider();
	virtual ~Spider();
	
	virtual bool init(GameScene* game);
	
	virtual void update(float delta);
    
    virtual void onEnter();
	virtual void onExit();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    bool _touched;
    
    cocos2d::CCPoint mMoveToSpot;
    void TimeToMoveSpider(CCNode* sender);
    
    bool mRunToExit;
    
    int mExit_X;
    int mExit_Y;
    
    int mTimesTaped;
    float mTimeDown;
    float mTimeToMove;
    bool mSpecialSpider;
    void HitSpider(bool theRemove);
    
    bool mTutorfix;
    
    Crystal* mCrystalCatch;
    
    float mSpiderScale;
    
    bool mTutorialSpider;
    bool mMoveByNodes;
    int mMoveIndex;
    bool mMoveClock;
    int mMoveCurrentDir;
    
    //Special stuff
    bool mPatrolPoints;
    bool mMoveToControl_1;
    cocos2d::CCPoint mSpiderPatrolPoint_1;
    cocos2d::CCPoint mSpiderPatrolPoint_2;
	
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
    
    bool mAnimationsPaused;
    
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
    
    float _HideTime;
    int mCaveID;
    
    Dwarf* _dwarfCatch;
    
    void CheckDwarfs();
    
    void CatchDwarf(Dwarf* theDwarf);
    void CancelDwarfCatch(Dwarf* theCancelDwarf);
    bool mCatchingCrystal;
    float mStartCatchCrystal;
    Dwarf* mDwarfToCatch;
    cocos2d::CCSprite* mCatchRadar;
    cocos2d::CCSprite* mWarnIcon;
    float mSpiderStartY;
    
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
