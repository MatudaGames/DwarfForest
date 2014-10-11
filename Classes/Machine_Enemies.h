//
//  Machine_Enemies.h
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

class Machine_Enemies: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
    static Machine_Enemies* create(GameScene* gameScene);
    
	Machine_Enemies();
    virtual ~Machine_Enemies();
    
    virtual bool init(GameScene* gameScene);
    
    virtual void onEnter();
	virtual void onExit();
    
    bool mCanBeActivated;
    
    SpriteAnimation* _removeSmoke;
    
    cocos2d::CCSprite* _LeafRight;
    cocos2d::CCSprite* _LeafLeft;
    cocos2d::CCSprite* _LeafCenter;
    cocos2d::CCSprite* _LeafBottom;
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    bool mTouched;
    
    
    
    //The real stuff
    cocos2d::CCSprite* mBlast;
    cocos2d::CCSprite* mBroken;
    
    cocos2d::CCSprite* mProgress;
    cocos2d::CCSprite* mJelly_FakeMask;
    
    SpriteAnimation* mSparkAnim;
    SpriteAnimation* mSparkActive;
    
    //The body of machine
    cocos2d::CCSprite* mBase;
    cocos2d::CCSprite* mLegLeft;
    cocos2d::CCSprite* mLegRight;
    cocos2d::CCSprite* mHandLeft;
    cocos2d::CCSprite* mHandRight;
    cocos2d::CCSprite* mBaseBlock;
    
    //The steam anim
    SpriteAnimation* mSmoke;
    
    int mUpgradeLevel;
    
    void CreateMachineByLevel(int theLevel);
    void ClearOldParts();
    
    void ForceSetReady();
    void SetAnimation_Grow();
    void SetAnimation_Ready();
    void SetAnimation_Use(int theTime);
    
    int mCurrentNumberID;
    void ShowNumber();
    void OnRemove(CCNode* sender);
    
    
private:
    GameScene* _gameScene;
};