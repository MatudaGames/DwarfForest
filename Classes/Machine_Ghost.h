//
//  Machine_Ghost.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 02/11/13.
//
//

#pragma once

#include <cocos2d.h>

class GameScene;
class Dwarf;
class SpriteAnimation;
class TimedSpriteAnimation;

struct ElementInfo
{
	int32_t x;
	int32_t y;
	float angle;
};

class Machine_Ghost: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
    static Machine_Ghost* create(GameScene* gameScene);
    
	Machine_Ghost();
    virtual ~Machine_Ghost();
    
    virtual bool init(GameScene* gameScene);
    
    virtual void onEnter();
	virtual void onExit();
    
    bool mCanBeActivated;
    
    SpriteAnimation* _removeSmoke;
    
    SpriteAnimation* mJelly;
    SpriteAnimation* mSiksna;
    SpriteAnimation* mSmokeBlue;
    
    cocos2d::CCSprite* _LeafRight;
    cocos2d::CCSprite* _LeafLeft;
    cocos2d::CCSprite* _LeafCenter;
    cocos2d::CCSprite* _LeafBottom;
    
    int mLevel;
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    bool mTouched;
    
    //The real stuff
    cocos2d::CCSprite* mBlast;
    cocos2d::CCSprite* mBroken;
    
    //The body of machine
    cocos2d::CCSprite* mBase;
    cocos2d::CCSprite* mLegLeft;
    cocos2d::CCSprite* mLegRight;
    cocos2d::CCSprite* mHandLeft;
    cocos2d::CCSprite* mHandRight;
    cocos2d::CCSprite* mBaseBlock;
    
    cocos2d::CCSprite* mJelly_FakeMask;
    
    //The steam anim
    SpriteAnimation* mSmoke;
    
    int mUpgradeLevel;
    
    void CreateMachineByLevel(int theLevel);
    void ClearOldParts();
    
    void SetLeftSmoke();
    void SetRightSmoke();
    
    void SetAnimation_Grow();
    void SetAnimation_Ready();
    void SetAnimation_Use(int theTime);
    
    int mCurrentNumberID;
    void ShowNumber();
    void OnRemove(CCNode* sender);
    
    void Create2ndAction();
    
    ElementInfo mChimney_1_IdleStart;
    ElementInfo mChimney_2_IdleStart;
    ElementInfo mChimney_1_IdleEnd;
    ElementInfo mChimney_2_IdleEnd;
    
    ElementInfo mChimney_1_UseStart;
    ElementInfo mChimney_2_UseStart;
    ElementInfo mChimney_1_UseEnd;
    ElementInfo mChimney_2_UseEnd;
    
    
    
    
    
    
    
private:
    GameScene* _gameScene;
};