//
//  Enemy_Totem.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 09/02/15.
//
//

#pragma once

#include <cocos2d.h>
#include "MissionManager.h"
#include "Dwarf.h"

class GameScene;
class SpriteAnimation;
class Effect;

class Enemy_Totem: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
    static Enemy_Totem* create(GameScene* game);
    
    Enemy_Totem();
    virtual ~Enemy_Totem();
    
    virtual bool init(GameScene* game);
    
    virtual void update(float delta);
    
    virtual void onEnter();
    virtual void onExit();
    
    bool _allCreated;
    cocos2d::CCSprite* _base;
    
    void SetMissionStuff(int theX,int theY,int HP,int BulletEvent,int BulletFreq,int BubbleShieldEvent,int BubbleShieldFreq,int BubbleShieldActive,int FlameFreq,int FlameRadius,int FlameActive);
    
    int mHP;
    float mCurrentHP;
    float mNeedHP;
    
    int mBubbleShield_Event;
    float mBubble_TimeCurrent;
    float mBubble_ActiveTime;
    float mBubble_ActiveTimeCurrent;
    int mBubbleShield_Freq;
    
    int mBullet_Event;
    float mBullet_TimeCurrent;
    int mBullet_Freq;
    
    int mFlame_Freq;
    float mFlame_CurrentTime;
    int mFlame_Radius;
    float mFlame_ActiveTime;
    float mFlame_ActiveTimeCurrent;
    
    cocos2d::CCSprite* mBar_TotemBase;
    cocos2d::CCSprite* mBar_TotemHP;
    
    cocos2d::CCArray* _movePoints;
    
    void OnShowedUp();
    
    float mRadius;
    
    // New stuff
    cocos2d::CCLabelTTF* mDebugTimerLabel;
    std::stringstream mDebugTimerStr;
    
    
private:
    GameScene* _game;
};
