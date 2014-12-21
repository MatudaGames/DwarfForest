//
//  InGamePowers.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 01/11/14.
//
//


#ifndef INGAMEPOWERS_H_
#define INGAMEPOWERS_H_

#include "cocos2d.h"
#include "GameScene.h"
#include "Troll.h"

#define FLY_BALL_MASTER_TROLL 5001

/**
 * The layer displays the current score.
 */
class InGamePowers:
public cocos2d::CCLayer
{
public:
    InGamePowers();
    bool init();
    void updateScore(int score);
    
    // implement the "static node()" method manually
    CREATE_FUNC(InGamePowers);
    
    cocos2d::CCMenuItemImage *button_1;
    cocos2d::CCMenuItemImage *button_2;
    cocos2d::CCMenuItemImage *button_3;
    
    void onButton_1(CCObject* sender);
    void onButton_2(CCObject* sender);
    void onButton_3(CCObject* sender);
    
    // The powers
    bool mFreezeActive;
    bool mElectroActive;
    
    Troll* mCurrentActiveTroll;
    
    void OnFreezeTroll();
    void OnElectroTroll();
    
    void OnRemoveTrollIndicators();
    void OnFreezeBallHit(CCNode* sender);
    void OnPlayerClickTroll(Troll* theTroll);
    
    void OnGhoustDwarfs();
    
    GameScene* mGameScene;
    
    void OnResetAllVars();
    
private:
    /**
     * This label is used to show the current score.
     */
    cocos2d::CCLabelTTF*	__pScoreLabel;
};

#endif /* HUDLAYER_H_ */

/*

#pragma once

#include <cocos2d.h>
#include "GameScene.h"
#include <SimpleAudioEngine.h>

class InGamePowers: public cocos2d::CCLayer
{
public:
    // implement the "static node()" method manually
    CREATE_FUNC(InGamePowers);
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onEnter();
    virtual void onExit();
    
    void onButton_1(CCObject* sender);
    void onButton_2(CCObject* sender);
    void onButton_3(CCObject* sender);
    
private:
    
    GameScene* _game;
};

*/