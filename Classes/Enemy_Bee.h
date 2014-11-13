//
//  Enemy_Bee.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 13/11/14.
//
//

#pragma once

#include <cocos2d.h>
#include "Dwarf.h"

class GameScene;
class SpriteAnimation;
class Effect;

class Enemy_Bee: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
    static Enemy_Bee* create(GameScene* game);
    
    Enemy_Bee();
    virtual ~Enemy_Bee();
    
    virtual bool init(GameScene* game);
    
    virtual void update(float delta);
    
    virtual void onEnter();
    virtual void onExit();
    
    bool _allCreated;
    cocos2d::CCSprite* _bullet;
    cocos2d::CCArray* _bulletArr;
    
    void CreateFromMissionParams();
    
    cocos2d::CCDrawNode* mCatchRadar;
    int coneRadius; // konusa garums pikseļos
    int coneWidth; // konusa platums grādos
    
    cocos2d::CCSprite* _base;
    
    float _beeIdleBeforeFire;
    bool _movingToFinish;
    cocos2d::CCPoint _moveTo;
    void setAngle(float value);
    
    int _startX;
    int _startY;
    int _finishX;
    int _finishY;
    
    float _angle;
    float _speed;
    float bullet_speed;
    
    // Not needed for now
//    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
//    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
//    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    
private:
    GameScene* _game;
};
