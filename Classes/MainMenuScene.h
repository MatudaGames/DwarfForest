#pragma once

#include <cocos2d.h>

#define EVENT_GOT_BONUS "printSomething"
#define EVENT_GOT_SIGN "onRecievedSingStatus"

class ChallengesRedDot:public cocos2d::CCLayer
{
public:
    virtual bool init();
    CREATE_FUNC(ChallengesRedDot);
    void update();
    
    cocos2d::CCLabelTTF* count;
};

class StoreRedDot:public cocos2d::CCLayer
{
public:
    virtual bool init();
    CREATE_FUNC(StoreRedDot);
    void update();
    
    cocos2d::CCLabelTTF* count;
};

class UpgradesRedDot:public cocos2d::CCLayer
{
public:
    virtual bool init();
    CREATE_FUNC(UpgradesRedDot);
    void update();
    
    cocos2d::CCLabelTTF* count;
};

class MainMenuScene: public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
//    virtual void onEnter();
    virtual void onExit();
    
    void OnRecreateSingIn();
    
    int mCurrentMusicID;
    void OnFinishedMusicFadeOut();
    bool mFadeOutMusic;
    bool mFadeInMusic;
    float mBackMusicVolume;
    
    bool mOptionsOpen;
    
    cocos2d::CCMenu* mMainMenu;
    
    void OnCompletedZoomOut();
    void CreateStartMapForMenu();
    cocos2d::CCSpriteBatchNode * _batchNode;
    bool mResetGameVisual;
    
    virtual void keyBackClicked();
    
    void sendFeedback();
    
    // a selector callback
    void menuPlayCallback(CCObject* pSender);
	void menuStoreCallback(CCObject* sender);
    void menuChallengesCallback(CCObject* sender);
    void menuLeaderboardsCallback(CCObject* sender);
    void menuUpgradesCallback(CCObject* sender);
    void menuAchievementsCallback(CCObject* sender);
    void menuFreeDiamondsCallback(CCObject* sender);
    void menuMoreGamesCallback(CCObject* sender);
    void menuOptionsCallback(CCObject* sender);
    
    void menuSignInGoogle(CCObject* sender);
    
    void onLoginSuccess(CCObject* obj);
    void onRecievedSign(CCObject* obj);
    
    void onPreloadImages();
    void onPreloadSounds();
    
    void OnRemoveStore();
    
    // implement the "static node()" method manually
    CREATE_FUNC(MainMenuScene);
    
    void onEnter();
    void updateUI();
//    void keyBackClicked();
    
    void update(float delta);
    
private:
    void testCall();
	
	bool _waitForGoogle;
	ChallengesRedDot* _challengesPoint1;
    ChallengesRedDot* _challengesPoint2;
    StoreRedDot* _storePoint1;
    StoreRedDot* _storePoint2;
    
    UpgradesRedDot* _upgradesPoint1;
    UpgradesRedDot* _upgradesPoint2;
};
