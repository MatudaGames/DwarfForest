//
//  UniversalPowerItem.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 02/03/15.
//
//

#pragma once

#include <cocos2d.h>
#include "ItemDataManager.h"

class GameScene;
class Dwarf;
class SpriteAnimation;

#define ITEM_CRYSTAL_PLANT 1001
#define ITEM_CRYSTAL_DOUBLER 1002
#define ITEM_GHOST 1003
#define ITEM_CRYSTAL_REFINERY 1004

class Universal_PowerItem: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
    static Universal_PowerItem* create(GameScene* gameScene,int theID);
    
    Universal_PowerItem();
    virtual ~Universal_PowerItem();
    virtual bool init(GameScene* gameScene,int theID);
    
    //..........................................
    // ITEM_CRYSTAL_PLANT
    
    void CreateCrystalPlant();
    int mCrystalPlant_SpawnCrystals;
    int mCrystalPlant_PlantsFallenDown;
    
    void onGrowCompleted();
    void onPlantFallDown(CCNode* sender);
    void onRemoveSmoke(CCNode* sender);
    
    //..........................................
    // ITEM_GHOST
    
    void CreateGhostMushroom();
    void MushroomGhost_onFinishedTrans();
    void MushroomGhost_onFinishedShowUp();
    void MushroomGhost_onRemove();
    void MushroomGhost_createRemove();
    
    cocos2d::CCSprite* mMushroom_Ghost_Base;
    cocos2d::CCSprite* mMushroom_Ghost_Over;
    
    bool mMushroomGhost_NeedToRemove;
    
    //..........................................
    // ITEM_CRYSTAL_DOUBLER
    
    void CreateCrystalDoublerMushroom();
    
    void MushroomCrystalDuble_onFinishedTrans();
    void MushroomCrystalDuble_onFinishedShowUp();
    void MushroomCrystalDuble_onRemove();
    void MushroomCrystalDuble_createRemove();
    
    cocos2d::CCSprite* mMushroom_CrystalDuble_Base;
    cocos2d::CCSprite* mMushroom_CrystalDuble_Over;
    
    bool mMushroomCrystalDuble_NeedToRemove;
    
    //..........................................
    // The global parametrs for all items
    
    PowerInfo mItemInfo;
    
    int mLevel;
    
    void onRemoveThisItem();
    void onDwarfPickUp(Dwarf* theDwarWhoPicked);
    
    
    
    
private:
    GameScene* _gameScene;
    cocos2d::CCSprite* _LeafRight;
    cocos2d::CCSprite* _LeafLeft;
    cocos2d::CCSprite* _LeafCenter;
    cocos2d::CCSprite* _LeafBottom;
    
    bool mNeedToRemove;
    bool mTouched;
    SpriteAnimation* _removeSmoke;
};
