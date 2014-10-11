//
//  Plant_Crystal.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 10/6/13.
//
//

#pragma once

#include <cocos2d.h>

class GameScene;
class Dwarf;
class SpriteAnimation;

class PlantCrystal: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
    static PlantCrystal* create(GameScene* gameScene);
    
	PlantCrystal();
    virtual ~PlantCrystal();
    
    virtual bool init(GameScene* gameScene);
    
    virtual void onEnter();
	virtual void onExit();
    
    void onFinishedShowUp();
    void onFinishedTrans();
    void onRemove();
    void createRemove();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    void onFinishedGrow();
    void onFinishShowUp();
    void onStartToShake();
    void onRemoveThisPlant();
    void spawnCrystals();
    
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

