//
//  MasterTroll.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 06/03/15.
//
//

#pragma once

#include <cocos2d.h>
#include "MissionManager.h"
#include "Dwarf.h"

class GameScene;
class SpriteAnimation;
class Effect;

class MasterTroll: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
    static MasterTroll* create(GameScene* game);
    
    MasterTroll();
    virtual ~MasterTroll();
    
    virtual bool init(GameScene* game);
    
    virtual void update(float delta);
    
    virtual void onEnter();
    virtual void onExit();
    
    cocos2d::CCSprite* _base;
    
    void setAnimation(SpriteAnimation* animation);
    void setAnimationByName(const char* theAnimation);
    
    // Not needed for now
    //    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    //    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    //    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    void SetMissionStuff(MissionSet theMission);
    
    // The new stuff for master troll
    cocos2d::CCSprite* mBasePad;
    cocos2d::CCSprite* mSplashSprite;
    
    void OnMasterHitGroundSFX(CCNode* sender);
    
    float mHitGroundTime;
    
    int mLastMagicAnimID;
    
private:
    GameScene* _game;
    
    //Animations for bee
    SpriteAnimation* _idleAnim;
    SpriteAnimation* _jumpAnim;
    SpriteAnimation* _magic_1_Anim;
    SpriteAnimation* _magic_2_Anim;
    SpriteAnimation* _shootAnim;
    SpriteAnimation* _smashAnim;
    
    SpriteAnimation* _animation;
};