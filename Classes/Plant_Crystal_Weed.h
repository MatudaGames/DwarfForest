//
//  Plant_Crystal_Weed.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 04/11/13.
//
//

#pragma once

#include <cocos2d.h>

class GameScene;
class Dwarf;
class SpriteAnimation;

class Plant_Crystal_Weed: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
    static Plant_Crystal_Weed* create(GameScene* gameScene);
    
	Plant_Crystal_Weed();
    virtual ~Plant_Crystal_Weed();
    
    virtual bool init(GameScene* gameScene);
    
    void onRemoveThisPlant();
    void onGrowCompleted();
    void onPlantFallDown(CCNode* sender);
    void onRemoveSmoke(CCNode* sender);
    
    int mLevel;
    int mSpawnCrystals;
    int mPlantsFallenDown;

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