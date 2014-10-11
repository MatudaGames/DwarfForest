#pragma once

#include <cocos2d.h>
#include "Crystal.h"
#include "GameStats.h"

class PauseScene: public cocos2d::CCLayer
{
public:
	// implement the "static node()" method manually
    CREATE_FUNC(PauseScene);
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    void menuContinueExit();
    
    void OnClousePop();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onEnter();
    virtual void onExit();
    
    cocos2d::CCSprite* mWeaklyTab;
    cocos2d::CCSprite* mDailyTab;
    
    cocos2d::CCSprite* mWaklyBG;
    cocos2d::CCSprite* mDailyBG;
    
    cocos2d::CCLabelTTF* b1;
    cocos2d::CCLabelTTF* b2;
    cocos2d::CCLabelTTF* b3;
    
    cocos2d::CCSprite* _mission_star_1;
    cocos2d::CCSprite* _mission_star_2;
    cocos2d::CCSprite* _mission_star_3;
    
    DF::Stats::GameStats _stats;
    
    cocos2d::CCSprite* _mission_progress_bar_1;
    cocos2d::CCSprite* _mission_progress_bar_2;
    cocos2d::CCSprite* _mission_progress_bar_3;
    cocos2d::CCSprite* _mission_progress_bar_4;
    
    int _mission_star_points_1;
    int _mission_star_points_2;
    int _mission_star_points_3;
    
    int mMaxBarPoints;
    int mTotalPointsInGame;
    
    void OnExitTutorial();
    
    void UpdateMissionStarsPause();
	
private:
    void menuResumeCallback(CCObject* sender);
    void menuMainMenuCallback(CCObject* sender);
    void menuTutorialCallback(CCObject* sender);
    
    void menuSoundCallback(CCObject* sender);
    void menuMusicCallback(CCObject* sender);
    void menuVibrationCallback(CCObject* sender);
    
    bool mWeaklyActive;
    void OnShowWeakly(cocos2d::CCObject *sender);
    void OnShowDaily(cocos2d::CCObject *sender);
    
    cocos2d::CCSprite* getBarIconPause(CrystalColor color);
};
