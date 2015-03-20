//
//  MasterDwarf.h
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

class MasterDwarf: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
    static MasterDwarf* create(GameScene* game);
    
    MasterDwarf();
    virtual ~MasterDwarf();
    
    virtual bool init(GameScene* game);
    
    virtual void update(float delta);
    
    virtual void onEnter();
    virtual void onExit();
    
    cocos2d::CCSprite* _base;
    cocos2d::CCSprite* mBasePad;
    
    void setAnimation(SpriteAnimation* animation);
    void setAnimationByName(const char* theAnimation);
    
    void setSpellToDwarf();
    
    bool shootSpell;
    
    cocos2d::CCArray* _spellBullets;
    
    // Not needed for now
    //    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    //    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    //    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    void SetMissionStuff(MissionSet theMission);
    
private:
    GameScene* _game;
    
    //Animations for bee
    SpriteAnimation* _idleAnim;
    SpriteAnimation* _spell_1_Anim;
    SpriteAnimation* _spell_2_Anim;
    
    SpriteAnimation* _animation;
};
