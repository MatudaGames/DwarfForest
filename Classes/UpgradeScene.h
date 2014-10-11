//
//  UpgradeScene.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 27/11/13.
//
//

#pragma once

#include <cocos2d.h>

#include "GameStats.h"
#include "Utils.h"

class UpgradeScene: public cocos2d::CCLayer
{
public:
	// implement the "static node()" method manually
//    CREATE_FUNC(UpgradeScene);
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(int theReturnType);
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(int theReturnType);
    
    static UpgradeScene* create(int theReturnType);
    
    int mReturnType;
	
//	void menuBackCallback(CCObject* sender);
//    void menuIntroCallback(CCObject* sender);
//    void menuTutorialCallback(CCObject* sender);
//    void menuCreditsCallback(CCObject* sender);
//    void menuStatsCallback(CCObject* sender);
//    void menuSoundCallback(CCObject* sender);
//    void menuMusicCallback(CCObject* sender);
//    void menuVibrationCallback(cocos2d::CCObject *sender);
//    
//    void loginFB(CCObject* sender);
//    void logoutFB(CCObject* sender);
//    
//    void onEnterTransitionDidFinish();
//	void keyBackClicked();
    
    int mPlantButtonState;
    int mGhostButtonState;
    int mStampButtonState;
    int mElectroButtonState;
    
    void CreateMachineByLevel(int theMachine,int theLevel,bool theCreate = false);
    void ChangeButtonState(int theButton,int theState);
    
    void UnlockNow_Plant(cocos2d::CCObject *sender);
    void Buy_Plant(cocos2d::CCObject *sender);
    
    void UnlockNow_Ghost(cocos2d::CCObject *sender);
    void Buy_Ghost(cocos2d::CCObject *sender);
    
    void UnlockNow_Electro(cocos2d::CCObject *sender);
    void Buy_Electro(cocos2d::CCObject *sender);
    
    void UnlockNow_Stamp(cocos2d::CCObject *sender);
    void Buy_Stamp(cocos2d::CCObject *sender);
    
    cocos2d::CCLabelTTF* _scoreLabel;
    cocos2d::CCLabelTTF* _diamondsLabel;
    
    DF::Stats::GameStats _stats;
    
    void UpdateStats();
    void onReturn();
    
private:
	//FB STUFF - FOR NOW
//    cocos2d::CCMenuItemImage* _fbButtonLogIn;
//    cocos2d::CCMenuItemImage* _fbButtonLogOut;
    
    void OnRemoveNode(CCNode* sender);
    
    void UpgradeElectroCall(cocos2d::CCObject *sender);
};
