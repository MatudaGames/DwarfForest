//
//  Machine_Stamp.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 30/10/13.
//
//

#pragma once

#include <cocos2d.h>

class GameScene;
class Dwarf;
class SpriteAnimation;

class Machine_Stamp: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
    static Machine_Stamp* create(GameScene* gameScene);
    
	Machine_Stamp();
    virtual ~Machine_Stamp();
    
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
    
    void ShakeGround();
    
    //The real stuff
    cocos2d::CCSprite* mBlast;
    cocos2d::CCSprite* mBroken;
    
    int mGrowTime;
    
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
    
    void SetAnimation_Grow();
    void SetAnimation_Ready();
    void SetAnimation_Use();
    
    void CreateSmoke();
    
    
private:
    GameScene* _gameScene;
};