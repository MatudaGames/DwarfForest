//
//  Plant_IceFlower.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 10/8/13.
//
//

#pragma once

#include <cocos2d.h>

class GameScene;
//class Dwarf;
class SpriteAnimation;

class PlantIceFlower: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
    static PlantIceFlower* create(GameScene* gameScene);
    
	PlantIceFlower();
    virtual ~PlantIceFlower();
    
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
    void removeThisPlantAndMakeNew();
    
private:
	cocos2d::CCSprite* _LeafRight;
    cocos2d::CCSprite* _LeafLeft;
    cocos2d::CCSprite* _LeafCenter;
    cocos2d::CCSprite* _LeafBottom;
    
    bool mNeedToRemove;
    
    bool mCanTouch;
	
	bool mTouched;
	cocos2d::CCPoint* mSpriteSize;
    
    SpriteAnimation* _animation;
    GameScene* _gameScene;
};