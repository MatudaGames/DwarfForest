//
//  GameItem_PowerUp.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 05/01/15.
//
//

#pragma once

#include <cocos2d.h>
#include "ItemDataManager.h"

class GameScene;

class GameItem_PowerUp: public cocos2d::CCNode
{
public:
    static GameItem_PowerUp* create(GameScene* gameScene,int theTimeOnMap,float realTimeOnMap);
    
    GameItem_PowerUp();
    virtual ~GameItem_PowerUp();
    
    virtual bool init(GameScene* gameScene,int theTimeOnMap,float realTimeOnMap);
    
    void onFinishedShowUp();
    void onFinishedTrans();
    void onRemove();
    void createRemove();
    
    int mPowerID;
    
    bool _needToRemove;
    
    SpellInfo mSpell;
    
private:
    GameScene* _gameScene;
    cocos2d::CCSprite* _mushroomSpriteOn;
    cocos2d::CCSprite* _mushroomSpriteOff;
    
    cocos2d::CCClippingNode* _specialNode;
    
    
};
