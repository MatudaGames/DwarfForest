//
//  Dwarf.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 22.02.2013.
//
//

#pragma once

#include <cocos2d.h>
#include <string.h>
#include "ItemDataManager.h"

class GameScene;
class Effect;
class SpriteAnimation;
class Troll;

#define MT_BULLET_ID 344

enum DwarfType
{
	DWARF_TYPE_TALL,
	DWARF_TYPE_FAT,
	DWARF_TYPE_COUNT
};

class Dwarf: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
	static Dwarf* create(GameScene* game,int theType = -1);
	
	Dwarf();
	virtual ~Dwarf();
	
	virtual bool init(GameScene* game, int theType);
	
	virtual void update(float delta);
	
	virtual void onEnter();
	virtual void onExit();
	
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	
	DwarfType getType() const { return _type; }
    
    // The power button stuff
    SpellInfo mSpellForAttack;
    
    void setPowerButton(int theID);
    void updateDwarfPowerZone();
    void FireBulletAtTroll(int thePowerID);
    void OnFireBulletHitTroll(CCNode* sender);
    
    Troll* mSnapedTroll;
    Troll* mSnapedTroll_FallBack;
    
    bool mSnapedToTotem;
    bool mSnapedToMasterTroll;
    bool mCanSearchForTrollsForSnap;
    
    int mContainsPowerUp;
    
    int mLostDwarfCount;
    
    cocos2d::CCSprite *mPowerUpIcon;
    
    //--------------------------------------
	
	void setEffect(Effect* value);
	void removeEffect();
	Effect* getEffect() const { return _effect; };
    
    float mTimeInMap;// How long is this dwarf in map?
    
    int mCollectedCrystalsMax;
    cocos2d::CCSprite *_BagIcon_1;
    cocos2d::CCSprite *_BagIcon_2;
    cocos2d::CCSprite *_BagIcon_3;
    
    cocos2d::CCSprite *_TargetIcon;
    
    void setAction(int theType);
    float _PoisonTime;
    float _PoisonSlowValue;
    
    void ClearOldTraps();
    
    bool mBulletActive;
    
    void UpdateBagIcon();
    int mCrystalPoints;
    float mNewMissionTimeout;
    
	
	void setAngle(float value);
	void setAnimation(SpriteAnimation* animation);
    
    void doDwarfBang(float theAngle);
    
    void createCrash();
    void createTrollCrash();
    void clearCrash(bool theSameDirection);
    void removeFromSave();
    void setForceRemove();
    void removeThisDwarf();
    void removeFromTroll();
    
    void ClearAlarm();
    
    void onMaskHit();
    
    void pauseAnimation();
    void resumeAnimation();
    void setAnimationVisibility(bool theValue);
    
	void setDisabled(bool value);
	bool getDisabled() const { return _disabled; }
    
    int _SpawnID;
    cocos2d::CCPoint _SpawnStart;
	
	bool getForceRemove() const { return _forceRemove; }
	bool getCanPlayAlarm() const { return _canPlayAlarm; }
	void setCanPlayAlarm(bool value) { _canPlayAlarm = value; }
	void setAlarmTroll(bool value) { _alarmTroll = value; }
	
	bool getIsConnectedToCave() const { return _isConnectedToCave; }
	
	bool IsConnectedBlockTime;
	
	SpriteAnimation* getAlarmAnimation() const { return _alarmAnimation; }
    
    int mCollectedCrystals;
    float mCrystalSlowDown;
    void removeMovePoints();
    
    int _direction;
    bool _knockOut;
    
    float _knockOutTime;
    
    int mLineDotIndex;
    
    void PickedSpecialObject(int theType);
    bool mSpecialObjectPicked;
    
    float _angle;
    
    bool _disabled;
    
    float _speed;
    
    float _defaultSpeed;
    
    bool _dwarfIsRemoving;
    float mTotemSlow;
    float mTotemZoneIn;
    
    Effect* _effect;
	
private:
	bool addMovePoint(const cocos2d::CCPoint& point, const cocos2d::CCPoint& previousPoint,bool ingoreTexture);
	
	void startLine();
	void connectLine();
    void crashLine();
	void removeWeb(float delta);
	
	GameScene* _game;
	
	DwarfType _type;
	
	SpriteAnimation* _introAnimation;
	
	SpriteAnimation* _leftDownAnimation;
	SpriteAnimation* _leftUpAnimation;
	SpriteAnimation* _leftAnimation;
	SpriteAnimation* _upAnimation;
	SpriteAnimation* _downAnimation;
    
    SpriteAnimation* _crashAnimation;
    SpriteAnimation* _alarmAnimation;
    
    SpriteAnimation* _removeSmoke;
	
	SpriteAnimation* _animation;
    
    cocos2d::CCSprite* mGhoustBG;
	
	cocos2d::CCPointArray* _movePoints;
	cocos2d::CCSpriteBatchNode* _line;
	bool _touchEnded;
	
	
	
	
	bool _alarmTroll;
    bool _alarmDwarf;
	
	bool _forceRemove;
    
    
    
    
    bool _canPlayAlarm;
    bool _alarmWasSet;
    
    bool _diognalMove;
	
	unsigned int _stepSound;
	
	float _coolDown;
    
    bool _isConnectedToCave;
    
    
    
};
