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
    
    SpellInfo mButtonSpell_1;
    SpellInfo mButtonSpell_2;
    
    cocos2d::CCMenuItemImage *button_1;
    cocos2d::CCMenuItemImage *button_2;
    cocos2d::CCMenuItemImage *button_3;
    
    void onButton_1(CCObject* sender);
    void onButton_2(CCObject* sender);
    void onButton_3(CCObject* sender);
    
    // Buttons for start
    
    //----------------------------------------------------
    // The one cool button
    
    cocos2d::CCSprite* mButton_1_Base;
    cocos2d::CCSprite* mButton_1_Top;
    cocos2d::CCSprite* mButton_1_TopBack;
    cocos2d::CCSprite* mButton_1_Icon;
    cocos2d::CCSprite* mButton_1_Bottom;
    
    cocos2d::CCLabelBMFont* mButton_1_Cost;
    
    std::stringstream mLoad_1_StringStream;
    cocos2d::CCLabelBMFont* mButton_1_Load;
    
    cocos2d::CCProgressTimer* mButton_1_Progress;
    
    //----------------------------------------------------
    // The 2nd cool button
    
    cocos2d::CCSprite* mButton_2_Base;
    cocos2d::CCSprite* mButton_2_Top;
    cocos2d::CCSprite* mButton_2_TopBack;
    cocos2d::CCSprite* mButton_2_Icon;
    cocos2d::CCSprite* mButton_2_Bottom;
    
    cocos2d::CCLabelBMFont* mButton_2_Cost;
    
    std::stringstream mLoad_2_StringStream;
    cocos2d::CCLabelBMFont* mButton_2_Load;
    
    cocos2d::CCProgressTimer* mButton_2_Progress;
    
    //----------------------------------------------------
    // The 3rd cool button
    
    cocos2d::CCSprite* mButton_3_Base;
    cocos2d::CCSprite* mButton_3_Top;
    cocos2d::CCSprite* mButton_3_TopBack;
    cocos2d::CCSprite* mButton_3_Icon;
    cocos2d::CCSprite* mButton_3_Bottom;
    
    cocos2d::CCLabelBMFont* mButton_3_Cost;
    
    std::stringstream mLoad_3_StringStream;
    cocos2d::CCLabelBMFont* mButton_3_Load;
    
    cocos2d::CCProgressTimer* mButton_3_Progress;
    
    //----------------------------------------------------
    
    int mPowerCost_1;
    int mPowerCost_2;
    int mPowerCost_3;
    
    float mPowerCoolDown_1;
    float mPowerCoolDown_2;
    float mPowerCoolDown_3;
    
    void UpdateButtons();
    
    void OnStartRecharge(int theID);
    
    void OnUpdateProgress_1(float Dt);
    void OnUpdateProgress_2(float Dt);
    void OnUpdateProgress_3(float Dt);
    
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