//
//  Troll.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 22.02.2013.
//
//

#pragma once

#include <cocos2d.h>
#include "MissionManager.h"
#include "Dwarf.h"

#define TROLL_SELECT_INDICATOR 100
#define TROLL_WTF_INDICATOR 101

class GameScene;
class SpriteAnimation;
class Effect;

class Troll: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
	static Troll* create(GameScene* game);
	
	Troll();
	virtual ~Troll();
	
	virtual bool init(GameScene* game);
	
	virtual void update(float delta);
    void update_old(float delta);
    
    virtual void onEnter();
	virtual void onExit();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    int coneRadius; // konusa garums pikseļos
    int coneWidth; // konusa platums grādos
    int coneAngleOffset;
    int coneAngle;
    
    void setRadar(int theRadius,int theWidth);
    
//    cocos2d::CCDrawNode* dnode;
    cocos2d::CCDrawNode* mCatchRadar;
    cocos2d::CCProgressTo* pnode;
    
    int coneSwingAmp; // kāda būs konusa svārstību amplitūda grādos
    int coneSwingSpeed; // cik ātri svārstīsies konuss
    int loopPosition;
    
    bool _touched;
    bool _changedAnimation;
    
    float _testAngle;
    
    bool mTutorfix;
    
    bool mRadarSet;     // Will troll use radar
    
    int mCatchRadarAngle;
    
    void UpdateRadar(float delta);
    
    cocos2d::CCSprite* mDebugPoint1;
    cocos2d::CCSprite* mDebugPoint2;
    cocos2d::CCSprite* mDebugPoint3;
    
    bool collideAtPoint(cocos2d::CCPoint point);
    
    
    bool mTutorialTroll;
    bool mMoveByNodes;
    int mMoveIndex;
    bool mMoveClock;
    int mMoveCurrentDir;
    
    //Special stuff
    bool mPatrolPoints;
    bool mMoveToControl_1;
    cocos2d::CCPoint mTrollPatrolPoint_1;
    cocos2d::CCPoint mTrollPatrolPoint_2;
	
    cocos2d::CCSprite* mDrawLine;
    int mDrawLineID;
    
	void setAngle(float value);
	void setAnimation(SpriteAnimation* animation);
	
	bool getTouchable();
    
    void SetMissionStuff(MissionTroll theMission);
    MissionTroll _missionInfo;
    
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
    bool mCatchingDwarf;
    float mStartCatchDwarf;
    Dwarf* mDwarfToCatch;
//    cocos2d::CCSprite* mCatchRadar;
    cocos2d::CCSprite* mWarnIcon;
    float mTrollStartY;
    
    cocos2d::CCPointArray* _movePoints;
    bool _moveInCircle;
    int _moveValue;
    
    // New power stuff
    float mFreezedTime;
    
	
private:
	GameScene* _game;
	
	SpriteAnimation* _leftDownAnimation;
	SpriteAnimation* _leftUpAnimation;
	SpriteAnimation* _leftAnimation;
	SpriteAnimation* _upAnimation;
	SpriteAnimation* _downAnimation;
    SpriteAnimation* _victory;
    SpriteAnimation* _removeSmoke;
	

	
	bool _touchable;
	
    bool _isDisabled;
    bool _forceRemove;
};
