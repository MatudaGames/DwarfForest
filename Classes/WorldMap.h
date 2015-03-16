#ifndef __DwarfForest__WorldMap__
#define __DwarfForest__WorldMap__

#include "cocos2d.h"
#include "MissionManager.h"

class WorldMap : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    void onEnterTransitionDidFinish();
    
    // implement the "static node()" method manually
    CREATE_FUNC(WorldMap);
    
    // The map of the map base
    cocos2d::CCSprite *map_base;
    
    // Adds the cool map stuff
    void AddMovingObjects();
    
    void UpdateStats();
    
    void CreateLevels();
    
    void OnClickedMission(CCObject* sender);
    void OnClickedPlayer(CCObject* sender);
    
    void MissionTaskInditificator(int theID);
    
    void CreatePlayer();
    
    // The all node cords
//    int worldNodeCords;//[] = {35,296,118,308,203,278,208,203,128,173,76,120,111,52,218,43,310,78,360,147};
    std::vector<int> SplitString(const std::string s,char delim);
    std::vector<int> mWorldNodeCords;
    cocos2d::CCSize mScreenSize;
    
    void PrepeareSmallMissionScreen();
    void ShowMissionScreen(int theID);
    void HideMissionScreen(CCObject * pSender);
    
    void OnPlayerFinishedMove();
    
    void ResetStats();
    void BuyMoreDwarfs();
    
    cocos2d::CCMenu* mPlayer;
    cocos2d::CCLabelTTF * taskInfo;
    cocos2d::CCLabelTTF * dwarfCount;
    cocos2d::CCLabelTTF * _diamondsLabel;
    cocos2d::CCLabelTTF * _crystalsLabel;
    cocos2d::CCLabelBMFont * dwarfPrice;
    cocos2d::CCLabelTTF * _totemHP;
    cocos2d::CCLabelTTF * _totemShield;
    cocos2d::CCLabelTTF * _totemAttack;
    
    bool moveBackground;
    
    int mCurrentMissionID;
    int mLastMissionID;
    int WhatMission;
    
    int TESTdwarfCount;
    
    MissionSet mCall;
    
    cocos2d::CCLayerColor* mSmallMissionScreen;
    
    // Hud stuff
    void CreateHud();
    void OnReloadStuff();
    void Hud_ShowOptions(CCObject* sender);
    void Hud_ShowStore(CCObject* sender);
    void OnRemoveStore();
    void Hud_ShowChallenges(CCObject* sender);
    void Hud_ShowUpgades(CCObject* sender);
    

};

#endif // __DwarfForest__WorldMap__
