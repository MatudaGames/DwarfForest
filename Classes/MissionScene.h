//
//  MissionScene.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 12/01/14.
//
//

#pragma once

#include <cocos2d.h>

class MissionScene: public cocos2d::CCLayer
{
public:
	// implement the "static node()" method manually
    CREATE_FUNC(MissionScene);
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    cocos2d::CCSize mScreenSize;
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onEnter();
    virtual void onExit();
    
    void MapGenChange(cocos2d::CCObject *sender);
    
    int mLayer_1X;
    int mLayer_2X;
    int mLayer_3X;
    
    void OnShowNextSpecialMission();
    void OnBlitz();
    
    cocos2d::CCSprite* mLayer_1;
    cocos2d::CCSprite* mLayer_2;
    cocos2d::CCSprite* mLayer_3;
    
    void OnClouseFinish();
    void NewMissionCompleted();
    
    void onEnterTransitionDidFinish();
    
    cocos2d::CCSprite* mDwarfMove;
    
    int mLastPage;
    int mCurrentPage;
    int mExtraOffsetPage;
    int mMaxPages;
    bool mMoveBackScroll;
    
    bool mDidMoveFinger;
    
    bool mSlidingFree;
    
    bool _moveStarted;
    cocos2d::CCPoint _dragStart;
    
    cocos2d::CCPoint _dragStart_sky;
    cocos2d::CCPoint _dragStart_bottom;
    
    void ShowRewardInfo(int theMissionID);
    
    void CreateMissions();
    void UpdateMissions(bool addPage);
    void OnSelectMission(int theMissionID);
    
    void PlaceIndicator(cocos2d::CCMenuItemImage* theSprite);
    void RemoveIndicator(CCNode* sender);
    
    int mActiveMission;
    
    int mLastClickedMission;
    
    void OnClickedMission(CCObject* sender);
    void menuBackCallback(CCObject* sender);
    void menuPlayCallback(CCObject* sender);
    
    void reloadMissions(CCObject* sender);
    
    void downloadExtra_1(CCObject* sender);
    void downloadExtra_2(CCObject* sender);
    void downloadExtra_3(CCObject* sender);
    void downloadExtra_4(CCObject* sender);
    
    
    cocos2d::CCSprite* mMissionPad_1;
    cocos2d::CCSprite* mMissionPad_2;
    cocos2d::CCSprite* mMissionPad_3;
    cocos2d::CCSprite* mMissionPad_4;
    cocos2d::CCSprite* mMissionPad_5;
    
    cocos2d::CCSprite* mMissionBox;
    cocos2d::CCLabelTTF* mMissionBox_Header;
    
    cocos2d::CCLabelTTF* mMissionBox_CompletPrev;
    
    cocos2d::CCSprite* mMissionRewardBox;
    cocos2d::CCLabelTTF* mMissionRewardBox_Header;
    
    cocos2d::CCSprite* mPrevIndicator;
    
    int aBox1_StartY;
    int aText1_StartY;
    
    cocos2d::CCSprite* mMissionBox_Check1_On;
    cocos2d::CCSprite* mMissionBox_Check1_Off;
    cocos2d::CCLabelTTF* mMissionBox_Text1;
    cocos2d::CCLabelTTF* mMissionBox_Text1p;
    
    int aBox2_StartY;
    int aText2_StartY;
    
    cocos2d::CCSprite* mMissionBox_Check2_On;
    cocos2d::CCSprite* mMissionBox_Check2_Off;
    cocos2d::CCLabelTTF* mMissionBox_Text2;
    cocos2d::CCLabelTTF* mMissionBox_Text2p;
    
    int aBox3_StartY;
    int aText3_StartY;
    
    cocos2d::CCSprite* mMissionBox_Check3_On;
    cocos2d::CCSprite* mMissionBox_Check3_Off;
    cocos2d::CCLabelTTF* mMissionBox_Text3;
    cocos2d::CCLabelTTF* mMissionBox_Text3p;
    
    cocos2d::CCLabelTTF* mMissionBox_Reward;
    
    cocos2d::CCSprite* mMissionWalker;
    
private:
	//FB STUFF - FOR NOW
    cocos2d::CCMenuItemImage* _fbButtonLogIn;
    cocos2d::CCMenuItemImage* _fbButtonLogOut;
};