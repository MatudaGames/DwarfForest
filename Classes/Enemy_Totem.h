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
#include "TrollBullet.h"

class GameScene;
class SpriteAnimation;
class Effect;

struct Quad_Action
{
    int32_t quad_id; // For what quad is this !!!
    
    int32_t type; // Is it a shield or etc
    // 1 - shield
    // 2 - deadzone
    // 3 - projectiles
    // 4 - no infor for now - so nope
    
    // More like global
    int32_t active_time; // How long is active
    int32_t activate_time; // After how long time will activate
    
    // For shield - what visual shield will be used?
    int32_t event_type;
    
    // For deadzone radius
    int32_t deadzone_radius;
    
    // For bullets
    int32_t bullet_distance; // How far will fly till explodes
    int32_t bullet_amount; // How much bullets can fire?
    
    // Flame sub stuff
    int32_t flame_radius; // Flame radius?
    int32_t flame_active_time; // How long flame throewr will be active?
    int32_t flame_rotate_speed; // How fast rotates flame thrower?
    
    float current_time_till_active; // Timer to next action
    float current_time_active; // Timer till will end active action
};

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
    
    // The quads
    void SetNewMissionStuff(MissionSet totemInfo);
    
    void craeteQuad(int theID,int theWidth,int theType,int theAngle);
    int collideAtPoint(cocos2d::CCPoint point);
    
    int mQuad_1_Size;
    int mQuad_2_Size;
    int mQuad_3_Size;
    int mQuad_4_Size;
    
    int mQuad_1_AngleWidth;
    int mQuad_2_AngleWidth;
    int mQuad_3_AngleWidth;
    int mQuad_4_AngleWidth;
    
    cocos2d::CCDrawNode* mPassive_Zone;
    
    cocos2d::CCDrawNode* mQuad_1;
    cocos2d::CCDrawNode* mQuad_2;
    cocos2d::CCDrawNode* mQuad_3;
    cocos2d::CCDrawNode* mQuad_4;
    
    // Pasive magic for totem
    int mPassive_EventID; // What event will this bee?
    int mPassive_Radius; // Whats the radius of this event
    
    // Quad magic for totem
    std::vector<Quad_Action> mQuad_Vector_1;
    std::vector<Quad_Action> mQuad_Vector_2;
    std::vector<Quad_Action> mQuad_Vector_3;
    std::vector<Quad_Action> mQuad_Vector_4;
    
    // the player actions
    void AttackFromPlayer(cocos2d::CCPoint position,int damage);
    void UpdateQuadSystem(float delta);
    void CreateSpecialBullet(int theType,int theStartX,int theStartY,int theAngle,int theLife);
    bool collideAtPassive(cocos2d::CCPoint point);
    
private:
    GameScene* _game;
};
