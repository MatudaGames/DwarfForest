#include <SimpleAudioEngine.h>
#include "MainMenuScene.h"
#include "GameScene.h"
#include "TutorialScene.h"
#include "AppMacros.h"
#include "SpriteAnimation.h"
#include "TimedSpriteAnimation.h"
#include "FreeDiamondsScene.h"
#include "StoreScene.h"
#include "JniCalls.h"
#include "User.h"
#include "Utils.h"
#include "DailyChallengesPopUp.h"
#include "StaticSceneManager.h"
#include "DebugScene.h"
#include "BeforeQuitScene.h"
#include "DailyChallengesPopUp.h"
#include "GameStats.h"
#include "RateGamePopup.h"
#include "StoreOfferPopup.h"
#include <time.h>

#include "PauseScene.h"
#include "LevelEndScene.h"
#include "OptionsScene.h"

#include "WorldMap.h"

#include <iostream>

//#include "FlurryX.h"
#include "AnalyticX.h"

#include "UpgradeScene.h"
#include "MissionScene.h"

#include "User.h"

#include "Machine_Stamp.h"
#include "Machine_Ghost.h"
#include "Machine_Enemies.h"

#include "ChallengesScene.h"


# if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

USING_NS_CC;
using namespace CocosDenshion;

CCScene* MainMenuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainMenuScene *layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool MainMenuScene::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(mOptionsOpen)
        return false;
    
    if(touch->getLocation().y<100)
        return false;
    return true;
}

void MainMenuScene::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    mResetGameVisual = true;
    
//    CCLOG("Did touch screen");
    //Continue to play screen!!!
    CCSprite* aDummy = NULL;//(CCSprite*)getChildByTag(100);
    
    CCScaleTo* aScale1 = CCScaleTo::create(1.0f,0.40f);
    CCDelayTime* aDelay = CCDelayTime::create(0.1f);
    CCCallFuncN* aFunctionFinish = CCCallFuncN::create(this, callfuncN_selector(MainMenuScene::OnCompletedZoomOut));
    CCSequence* aSeq = NULL;//CCSequence::create(aScale1,aDelay,aFunctionFinish,NULL);
//    aDummy->runAction(aSeq);
    
    CCMoveTo* aMove1 = NULL;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    aScale1 = CCScaleTo::create(1.0f,1.0f);
    aMove1 = CCMoveTo::create(1.0f,ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    CCSpawn* aSpawn = CCSpawn::create(aScale1,aMove1,NULL);
    
    aDelay = CCDelayTime::create(0.1f);
    aFunctionFinish = CCCallFuncN::create(this, callfuncN_selector(MainMenuScene::OnCompletedZoomOut));
    aSeq = CCSequence::create(aSpawn,aDelay,aFunctionFinish,NULL);
    
    aDummy = (CCSprite*)getChildByTag(101);
    aDummy->runAction(aSeq);
    
    
    aScale1 = CCScaleTo::create(1.0f,1.0f);
    aMove1 = CCMoveTo::create(1.0f,CCPointZero);
    aSpawn = CCSpawn::create(aScale1,aMove1,NULL);
    _batchNode->runAction(aSpawn);
    
    //The other stuff
    aDummy = (CCSprite*)getChildByTag(102);
    CCMoveTo* aMovePart = CCMoveTo::create(2.0f,ccp(visibleSize.width/2,-aDummy->getContentSize().height-60));
//    CCEaseExponentialIn* aEaseOut = CCEaseExponentialIn::create(aMovePart);
    aDummy->runAction(aMovePart);
    
    aDummy = (CCSprite*)getChildByTag(103);
    aMovePart = CCMoveTo::create(2.0f,ccp(-aDummy->getContentSize().width,visibleSize.height/2));
//    aEaseOut = CCEaseExponentialIn::create(aMovePart);
    aDummy->runAction(aMovePart);
    
    aDummy = (CCSprite*)getChildByTag(104);
    aMovePart = CCMoveTo::create(2.0f,ccp(visibleSize.width+aDummy->getContentSize().width,visibleSize.height/2));
//    aEaseOut = CCEaseExponentialIn::create(aMovePart);
    aDummy->runAction(aMovePart);
    
    aDummy = (CCSprite*)getChildByTag(105);
    aMovePart = CCMoveTo::create(1.0f,ccp(visibleSize.width/2,visibleSize.height+aDummy->getContentSize().height+60));
//    aEaseOut = CCEaseExponentialIn::create(aMovePart);
    aDummy->runAction(aMovePart);
    
//    aDummy = (CCSprite*)getChildByTag(106);
//    aMovePart = CCMoveTo::create(1.0f,ccp(visibleSize.width/2,visibleSize.height+aDummy->getContentSize().height));
//    aEaseOut = CCEaseExponentialIn::create(aMovePart);
//    aDummy->runAction(aMovePart);
    
}

void MainMenuScene::OnCompletedZoomOut()
{
    //Continue to game - cross fade !!!
//    FlurryX::logEvent("Tap to play!");
    
    User::getInstance()->mSpecial_10_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_11_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_12_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_13_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_14_Mission = false;//Not now !!!
    
    User::getInstance()->mSpecial_16_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_17_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_18_Mission = false;//Not now !!!
    
    User::getInstance()->mSpecial_19_Mission = false;
    User::getInstance()->mSpecial_20_Mission = false;
    
    User::getInstance()->mSpecial_21_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_22_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_23_Mission = false;//Not now !!!
    
    User::getInstance()->mNewSplitCaves = false;
    
    AnalyticX::flurryLogEvent("Game Starts");
    
    CCScene* scene = GameScene::scene();
    
//    CCTransitionScene* trans = CCTransitionCrossFade::create(0.1f,scene);
//    CCDirector::sharedDirector()->replaceScene(trans);
    CCDirector::sharedDirector()->replaceScene(scene);
//    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
    //Debug achievement show!!!
//    JniCalls::completedAchievement("achievement_play_the_game",1);//completeAchievment
}

void MainMenuScene::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    
}

void MainMenuScene::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    if ( !CCLayer::init() ) return false;
    
    mResetGameVisual = false;
    
    mOptionsOpen = false;
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    this->setKeypadEnabled(true);
#endif
    
    //=====================
    //---------------------
    // New Flurry stuff !!!
    
    AnalyticX::flurrySetAppVersion("v_1_01");
//    cocos2d::CCLog("--->>>get flurry version = %s", AnalyticX::flurryGetFlurryAgentVersion());
//    AnalyticX::flurrySetDebugLogEnabled(true);
//    AnalyticX::flurrySetShowErrorInLogEnabled(true);
    AnalyticX::flurrySetEventLoggingEnabled(true);
    
    AnalyticX::flurrySetSessionReportsOnPauseEnabled(false);
    
//    AnalyticX::flurrySetSessionContinueSeconds(143);
//    AnalyticX::flurrySetSecureTransportEnabled(false);
//    AnalyticX::flurrySetCrashReportingEnabled(true);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AnalyticX::flurryStartSession("9JRYT64JSQYNYSBPFNXF");
//    AnalyticX::flurrySetCrashReportingEnabled(true);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    AnalyticX::flurryStartSession("2ZCVWPY7VCRNSVQTWJJB");
#endif
    
    /*
    AnalyticX::flurryLogEvent("SomeNewLog_1");
    
    AnalyticX::flurryLogEvent("event_3");
    AnalyticX::flurryLogEventTimed(" log event timed test...", false);
    
    CCDictionary *testDict = new CCDictionary();
    testDict->autorelease();
    
    CCString *testCCString;
    
    testCCString = CCString::create("obj 0");
    
    testDict->setObject(testCCString, "key 0");
    
    testCCString = CCString::create("obj 1");
    
    
    testDict->setObject(testCCString, "key 1");
    
    CCDictionary* testDictInternal = new CCDictionary();
    testDictInternal->autorelease();
    
    testDict->setObject(testDictInternal, "key 2 : dictionary");
    
    CCArray* testArrayInternal = new CCArray();
    testArrayInternal->autorelease();
    
    testDict->setObject(testArrayInternal, "key 3 : array");
    
    testDict->setObject(CCInteger::create(5), "key 4 : int");
    
    testDict->setObject(CCDirector::sharedDirector(), "key 5 : CCDirector");
    
    AnalyticX::flurryLogEventWithParameters("event_5_with_params_no_timed", testDict);
    
    AnalyticX::flurryLogEventWithParametersTimed("test flurryLogEventWithParameters + timed", testDict, true);
    AnalyticX::flurryEndTimedEventWithParameters("test end event...", NULL);
    */
    
    
    //This should be at game end !!!
    /*
    CCDictionary *gameEndStats = new CCDictionary();
    gameEndStats->autorelease();
    
    gameEndStats->setObject(CCInteger::create(100), "In Game Time");
    gameEndStats->setObject(CCInteger::create(1000), "Points");
    gameEndStats->setObject(CCInteger::create(1), "Activated Blitz");
    gameEndStats->setObject(CCInteger::create(0), "Activated Stamp");
    gameEndStats->setObject(CCInteger::create(2), "Activated Ghoust");
    gameEndStats->setObject(CCInteger::create(4), "Collected Crystals");
    gameEndStats->setObject(CCInteger::create(10), "Diamonds");
    gameEndStats->setObject(CCInteger::create(0), "Active Mission");
    
    AnalyticX::flurryLogEventWithParameters("GameOverStats", gameEndStats);
    
    //For all other stuff !!!
    AnalyticX::flurryLogEvent("Click Shop");
    AnalyticX::flurryLogEvent("Clouse Shop");
    */
    
    
    
//    CCDictionary::create();
//    CCDictionary* testDictInternal2 = CCDictionary::createWithContentsOfFile("100","LevelTime","10","DiamondAmount","0","Something");
//    AnalyticX::flurryLogEventWithParameters("GamePlayStats", testDictInternal2);
    
    AnalyticX::flurryLogPageView();

    
    /*
    AnalyticX::flurrySetUserID("fake_user_id");
    AnalyticX::flurrySetAge(34);
    AnalyticX::flurrySetGender("f");
    AnalyticX::flurrySetReportLocation(false);
    
    AnalyticX::flurryLogEvent("event_3");
    AnalyticX::flurryLogEventTimed(" log event timed test...", false);
    
    CCDictionary *testDict = new CCDictionary();
    testDict->autorelease();
    
    CCString *testCCString;
    
    testCCString = CCString::create("obj 0");
    
    testDict->setObject(testCCString, "key 0");
    
    testCCString = CCString::create("obj 1");
    
    
    testDict->setObject(testCCString, "key 1");
    
    CCDictionary* testDictInternal = new CCDictionary();
    testDictInternal->autorelease();
    
    testDict->setObject(testDictInternal, "key 2 : dictionary");
    
    CCArray* testArrayInternal = new CCArray();
    testArrayInternal->autorelease();
    
    testDict->setObject(testArrayInternal, "key 3 : array");
    
    testDict->setObject(CCInteger::create(5), "key 4 : int");
    
    testDict->setObject(CCDirector::sharedDirector(), "key 5 : CCDirector");
    
    AnalyticX::flurryLogEventWithParameters("event_5_with_params_no_timed", testDict);
    
    AnalyticX::flurryLogEventWithParametersTimed("test flurryLogEventWithParameters + timed", testDict, true);
    AnalyticX::flurryEndTimedEventWithParameters("test end event...", NULL);
    
    AnalyticX::flurryLogPageView();
    */
    
    //---------------------
    //=====================
    
//    setTouchPriority(-100);
//    this->setTouchEnabled(false);
    
    //---------------------------------------
    // Here comes the new stuff !!!
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
//    CCSprite* MainCave = CCSprite::create("Interfeiss/main_menu/new/cave_big.png");
    
    CCSprite* MainCave = CCSprite::create("beta_feedback.png");
    MainCave->setTag(100);
    MainCave->setScale(0.4f);
    MainCave->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y-10));
//    addChild(MainCave,1);
    
    CCSprite* MapBack = CCSprite::create("gadalaiki/summer/map_summer.png");
    MapBack->setTag(101);
    MapBack->setScale(2.5f);
    MapBack->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y-50));
    addChild(MapBack,0);
    
    MapBack->addChild(MainCave);
    
    CreateStartMapForMenu();
    
    //The side trees
    CCSprite* MainGrass = CCSprite::create("Interfeiss/main_menu/new/sidetrees.png");
    MainGrass->setTag(103);
    MainGrass->setPosition(ccp(0,visibleSize.height/2));
    addChild(MainGrass);
    
    MainGrass = CCSprite::create("Interfeiss/main_menu/new/sidetrees.png");
    MainGrass->setTag(104);
    MainGrass->setPosition(ccp(visibleSize.width,visibleSize.height/2));
    addChild(MainGrass);
    
    //Add the logo
    MainGrass = CCSprite::create("Interfeiss/main_menu/new/grass_bottom.png");
    MainGrass->setTag(102);
    MainGrass->setPosition(ccp(visibleSize.width/2,MainGrass->getContentSize().height/2));
    addChild(MainGrass);
    
    //The animated logo
    TimedSpriteAnimation* animationLogo = TimedSpriteAnimation::create("Interfeiss/main_menu/title_shine.plist", 4.0f);
    animationLogo->setAnchorPoint(ccp(0.5, 1));
    animationLogo->setTag(105);
    animationLogo->setPosition(ccp(visibleSize.width / 2, visibleSize.height-20));
	animationLogo->retain();
	addChild(animationLogo);
    
    CCLabelTTF* aPlay = CCLabelTTF::create("Tap to play!",
                               FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
                               CCSize(300, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    aPlay->setColor(ccc3(0xFF, 0xE3, 0x0));
    aPlay->setTag(106);
    aPlay->setPosition(ccp(animationLogo->getContentSize().width/2,-20));
//    addChild(aPlay);
    animationLogo->addChild(aPlay);
    
    CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.2f);
    CCEaseExponentialIn* aEase1 = CCEaseExponentialIn::create(aScale1);
    CCScaleTo* aScale2 = CCScaleTo::create(0.5f,1.0f);
    CCEaseBounceOut* aEase2 = CCEaseBounceOut::create(aScale2);
    CCDelayTime* aDelay = CCDelayTime::create(3.0f);
    
    CCSequence* aSeq = CCSequence::create(aEase1,aEase2,aDelay,NULL);
    CCRepeatForever* aRepeat = CCRepeatForever::create(aSeq);
    aPlay->runAction(aRepeat);
    
    //The buttons !!!
    //Lets add settings and google stuff
    if(User::getInstance()->mFeedBackGiven==0)
    {
        CCMenuItemImage* feedItem = CCMenuItemImage::create(
                                                            "beta_feedback.png",
                                                            "beta_feedback.png",
                                                            this,
                                                            menu_selector(MainMenuScene::sendFeedback));
        CCMenu* FeedBack = CCMenu::create(feedItem,NULL);
        FeedBack->setPosition(ccp(visibleSize.width/2-20,130));
        getChildByTag(102)->addChild(FeedBack);
    }
    
    
    CCSprite * storeNormalSprite= CCSprite::create("Interfeiss/main_menu/new/store_btn_new0001.png");
    _storePoint1 = StoreRedDot::create();
    _storePoint1->setPosition(ccp(storeNormalSprite->getContentSize().width-30,storeNormalSprite->getContentSize().height-30));
    storeNormalSprite->addChild(_storePoint1);
    
    CCSprite * storeSelectedlSprite= CCSprite::create("Interfeiss/main_menu/new/store_btn_new0002.png");
    _storePoint2 = StoreRedDot::create();
    _storePoint2->setPosition(ccp(storeSelectedlSprite->getContentSize().width-30,storeSelectedlSprite->getContentSize().height-30));
    storeSelectedlSprite->addChild(_storePoint2);
    
    CCMenuItemSprite *storeItem = CCMenuItemSprite::create(storeNormalSprite, storeSelectedlSprite,
                                                           this,
                                                           menu_selector(MainMenuScene::menuStoreCallback));
    
    
    CCSprite * missionNormalSprite= CCSprite::create("Interfeiss/main_menu/new/missions_btn_new0001.png");
    CCSprite * missionSelectedlSprite= CCSprite::create("Interfeiss/main_menu/new/missions_btn_new0002.png");
    
    CCMenuItemSprite *missionItem = CCMenuItemSprite::create(missionNormalSprite, missionSelectedlSprite,
                                                           this,
                                                           menu_selector(MainMenuScene::menuMoreGamesCallback));
    
    
    CCSprite * challengesNormalSprite= CCSprite::create("Interfeiss/main_menu/new/challenges_btn_new0001.png");
    _challengesPoint1 = ChallengesRedDot::create();
    _challengesPoint1->setPosition(ccp(challengesNormalSprite->getContentSize().width-30,challengesNormalSprite->getContentSize().height-30));
    challengesNormalSprite->addChild(_challengesPoint1);
    
    CCSprite * challengesSelectedlSprite= CCSprite::create("Interfeiss/main_menu/new/challenges_btn_new0002.png");
    _challengesPoint2 = ChallengesRedDot::create();
    _challengesPoint2->setPosition(ccp(challengesSelectedlSprite->getContentSize().width-30,challengesSelectedlSprite->getContentSize().height-30));
    challengesSelectedlSprite->addChild(_challengesPoint2);
    
    CCMenuItemSprite *challengesItem = CCMenuItemSprite::create(challengesNormalSprite, challengesSelectedlSprite,
                                                             this,
                                                             menu_selector(MainMenuScene::menuChallengesCallback));
    
    mMainMenu = CCMenu::create(storeItem,
                                  missionItem,
                                  challengesItem,
                                  NULL);
    
//    menu->alignItemsHorizontally();
    
    storeItem->setPosition(ccp(-190,0));
    challengesItem->setPosition(ccp(220,0));
    
    // adjusting positions
    
    mMainMenu->setPosition(ccp(visibleSize.width/2-27,44));
//    menu->setPositionX(visibleSize.width - freeDiamondsItem->getContentSize().width / 2 + 10);
    
    getChildByTag(102)->addChild(mMainMenu, 1);
    
    //Lets add settings and google stuff
    CCMenuItemImage* optionsItem = CCMenuItemImage::create(
                                                           "Interfeiss/main_menu/new/options_btn0001.png",
                                                           "Interfeiss/main_menu/new/options_btn0002.png",
                                                           this,
                                                           menu_selector(MainMenuScene::menuOptionsCallback));
//    optionsItem->setAnchorPoint(ccp(0, 0));
    
    CCMenu* Options = CCMenu::create(optionsItem,NULL);
    Options->setPosition(ccp(370,660));
    getChildByTag(103)->addChild(Options);
    
    //the google side
    CCMenuItemImage* leaderboardsItem = CCMenuItemImage::create(
                                                                "Interfeiss/main_menu/new/leaderboards_btn0001.png",
                                                                "Interfeiss/main_menu/new/leaderboards_btn0002.png",
                                                                this,
                                                                menu_selector(MainMenuScene::menuLeaderboardsCallback));
//    leaderboardsItem->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* achievementsItem = CCMenuItemImage::create(
                                                                "Interfeiss/main_menu/new/achievments_btn0001.png",
                                                                "Interfeiss/main_menu/new/achievments_btn0002.png",
                                                                this,
                                                                menu_selector(MainMenuScene::menuAchievementsCallback));
//    achievementsItem->setAnchorPoint(ccp(0,0));
    
    CCMenu* optionsMenu = CCMenu::create(leaderboardsItem, NULL);
//    optionsMenu->setPosition(ccp(75, -10));
    leaderboardsItem->setPosition(ccp(0,0));
    achievementsItem->setPosition(ccp(0,-70));
    optionsMenu->setPosition(270,664);
    
    getChildByTag(104)->addChild(optionsMenu);
    
    _waitForGoogle = false;
    
    /* -----------  GOOGLE sign in button ---------------- */
    
    // Sign in with Google to earn achievements and enable cloud sync!
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    CCMenuItemImage* signInGoogle = CCMenuItemImage::create(
                                                            "Interfeiss/main_menu/new/googlegames_btn0001.png",
                                                            "Interfeiss/main_menu/new/googlegames_btn0002.png",
                                                            this,
                                                            menu_selector(MainMenuScene::menuSignInGoogle));
    
//    signInGoogle->setAnchorPoint(ccp(0,0));
    
    CCMenu* menuGoogle = CCMenu::create(signInGoogle,NULL);
    menuGoogle->setPosition(ccp(270, 596));//526
    menuGoogle->setTag(2222);//Remove the button after all done !!!
    
    //Try to ask nice about to sign in
    //    std::cout << "Will check if player is on\n";
    if (JniCalls::isSignedIn())
    {
        //        CCLog("ALL OK - PLAYER IS SIGNED IN");
    }
    else
    {
        //        CCLog("ALL BAD - PLAYER IS NOT SIGNED IN");
        getChildByTag(104)->addChild(menuGoogle, 1);//Show the sign in button !!!
    }
#endif
    /* -----------  +++++++++++++++++++++ ---------------- */
    
    this->scheduleUpdate();
    
//    return true;
    //---------------------------------------
    //The old main menu stuff !!!
    
    
    
    // enables back button on android
//    this->setKeypadEnabled(true);
    
    _waitForGoogle = false;
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MainMenuScene::onLoginSuccess), EVENT_GOT_BONUS, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MainMenuScene::onRecievedSign), EVENT_GOT_SIGN, NULL);
    
//    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
//    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    return true;

    CCSprite* background = CCSprite::create("Interfeiss/main_menu/background.png");
    background->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(background, 0);
    
    TimedSpriteAnimation* animation = TimedSpriteAnimation::create("Interfeiss/main_menu/title_shine.plist", 4.0f);
    animation->setAnchorPoint(ccp(0.5, 1));
    animation->setPosition(ccp(visibleSize.width / 3.5, visibleSize.height-20));
	animation->retain();
	addChild(animation);
    
    /* ----------- Main menu ------------- */
    
    /*
    CCMenuItemImage* playItem = CCMenuItemImage::create(
                                        "Interfeiss/main_menu/play_btn0001.png",
                                        "Interfeiss/main_menu/play_btn0002.png",
                                        this,
                                        menu_selector(MainMenuScene::menuPlayCallback));
	
	CCSprite * storeNormalSprite= CCSprite::create("Interfeiss/main_menu/store_btn0001.png");
    _storePoint1 = StoreRedDot::create();
    _storePoint1->setPosition(ccp(235, 118));
    storeNormalSprite->addChild(_storePoint1);
    
    CCSprite * storeSelectedlSprite= CCSprite::create("Interfeiss/main_menu/store_btn0002.png");
    _storePoint2 = StoreRedDot::create();
    _storePoint2->setPosition(ccp(238, 115));
    storeSelectedlSprite->addChild(_storePoint2);
    
    CCMenuItemSprite *storeItem = CCMenuItemSprite::create(storeNormalSprite, storeSelectedlSprite,
                                                           this,
                                                           menu_selector(MainMenuScene::menuStoreCallback));
	
    CCSprite * challengesNormalSprite= CCSprite::create("Interfeiss/main_menu/challenges_btn0001.png");
    _challengesPoint1 = ChallengesRedDot::create();
    _challengesPoint1->setPosition(ccp(300, 120));
    challengesNormalSprite->addChild(_challengesPoint1);
    
    CCSprite * challengesSelectedlSprite= CCSprite::create("Interfeiss/main_menu/challenges_btn0002.png");
    _challengesPoint2 = ChallengesRedDot::create();
    _challengesPoint2->setPosition(ccp(305, 128));
    challengesSelectedlSprite->addChild(_challengesPoint2);

    CCMenuItemSprite * challengesItem = CCMenuItemSprite::create(challengesNormalSprite, challengesSelectedlSprite,
                                                                 this,
                                                                 menu_selector(MainMenuScene::menuChallengesCallback));
    
	
    CCSprite * upgradesNormalSprite= CCSprite::create("Interfeiss/main_menu/upgrades_btn0001.png");
    _upgradesPoint1 = UpgradesRedDot::create();
    _upgradesPoint1->setPosition(ccp(300, 120));
    upgradesNormalSprite->addChild(_upgradesPoint1);
    
    CCSprite * upgradesSelectedSprite= CCSprite::create("Interfeiss/main_menu/upgrades_btn0002.png");
    _upgradesPoint2 = UpgradesRedDot::create();
    _upgradesPoint2->setPosition(ccp(305, 128));
    upgradesSelectedSprite->addChild(_upgradesPoint2);
    
	CCMenuItemSprite* upgradesItem = CCMenuItemSprite::create(
                                        upgradesNormalSprite,
                                        upgradesSelectedSprite,
                                        this,
                                        menu_selector(MainMenuScene::menuUpgradesCallback));
    

	CCMenuItemImage* freeDiamondsItem = CCMenuItemImage::create(
                                        "Interfeiss/main_menu/diamonds_btn0001.png",
                                        "Interfeiss/main_menu/diamonds_btn0002.png",
                                        this,
                                        menu_selector(MainMenuScene::menuFreeDiamondsCallback));
    
    CCMenuItemImage* moreGamesItem = CCMenuItemImage::create(
                                                             "Interfeiss/main_menu/moregames_btn0001.png",
                                                             "Interfeiss/main_menu/moregames_btn0002.png",
                                                             this,
                                                             menu_selector(MainMenuScene::menuMoreGamesCallback));

    CCMenu* menu = CCMenu::create(playItem,
                                  storeItem,
                                  challengesItem,
                                  upgradesItem,
                                  freeDiamondsItem,
                                  moreGamesItem,
                                  NULL);
    
    menu->alignItemsVerticallyWithPadding(-44);
    
    // adjusting positions
    
    playItem->setPositionX(playItem->getPositionX()-50);
    storeItem->setPositionX(storeItem->getPositionX()+50);
    storeItem->setPositionY(storeItem->getPositionY()+12);
    challengesItem->setPositionY(challengesItem->getPositionY()+4);
    
    menu->setPositionX(visibleSize.width - freeDiamondsItem->getContentSize().width / 2 + 10);
    
    this->addChild(menu, 1);

    
     ---------  Secondary Menu ------------
    
    CCMenuItemImage* optionsItem = CCMenuItemImage::create(
                                                           "Interfeiss/main_menu/options_btn0001.png",
                                                           "Interfeiss/main_menu/options_btn0002.png",
                                                           this,
                                                           menu_selector(MainMenuScene::menuOptionsCallback));
    optionsItem->setAnchorPoint(ccp(0, 0));
    
    CCMenuItemImage* leaderboardsItem = CCMenuItemImage::create(
                                                                "Interfeiss/main_menu/leaderboards_btn0001.png",
                                                                "Interfeiss/main_menu/leaderboards_btn0002.png",
                                                                this,
                                                                menu_selector(MainMenuScene::menuLeaderboardsCallback));
    leaderboardsItem->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* achievementsItem = CCMenuItemImage::create(
                                                                "Interfeiss/main_menu/achievments_btn0001.png",
                                                                "Interfeiss/main_menu/achievments_btn0002.png",
                                                                this,
                                                                menu_selector(MainMenuScene::menuAchievementsCallback));
    achievementsItem->setAnchorPoint(ccp(0,0));
    
    
    CCMenu* optionsMenu = CCMenu::create(optionsItem, leaderboardsItem, achievementsItem, NULL);
    optionsMenu->setPosition(ccp(75, -10));
    optionsMenu->alignItemsHorizontallyWithPadding(-30);
    leaderboardsItem->setPositionX(leaderboardsItem->getPositionX() + 5);
    
    this->addChild(optionsMenu);
    
    
     -----------  GOOGLE sign in button ----------------
    
    // Sign in with Google to earn achievements and enable cloud sync!
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    CCMenuItemImage* signInGoogle = CCMenuItemImage::create(
                                                            "Interfeiss/main_menu/google_btn0001.png",
                                                            "Interfeiss/main_menu/google_btn0002.png",
                                                            this,
                                                            menu_selector(MainMenuScene::menuSignInGoogle));
    
    signInGoogle->setAnchorPoint(ccp(0,0));
    
    CCMenu* menuGoogle = CCMenu::create(signInGoogle,NULL);
    menuGoogle->setPosition(ccp(250, -10));
    menuGoogle->setTag(2222);//Remove the button after all done !!!
    //    this->addChild(menuGoogle, 1);
    
    //Try to ask nice about to sign in
    std::cout << "Will check if player is on\n";
    if (JniCalls::isSignedIn())
    {
        CCLog("ALL OK - PLAYER IS SIGNED IN");
        //        std::cout << "ALL OK - PLAYER IS SIGNED IN\n";
    }
    else
    {
        CCLog("ALL BAD - PLAYER IS NOT SIGNED IN");
        //        std::cout << "ALL BAD - PLAYER IS NOT SIGNED IN\n";
        this->addChild(menuGoogle, 1);//Show the sign in button !!!
    }
    
#endif
    
    */
	
//    //        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/music1.mp3", true);
//    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/main_music.ogg", true);
//    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/main_music.mp3", true);
//    #endif
//    
//    if (CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true) == false)
//    {
//        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
//    }
//    
//    if (CCUserDefault::sharedUserDefault()->getBoolForKey("soundsEnabled", true))
//    {
//        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
//    }
//    else
//    {
//        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
//    }
    
//    onPreloadImages();//This helps only a bit - we need sprite sheets for all small stuff - todo
//    onPreloadSounds();
    
//    PauseScene* pauseLayer = PauseScene::create();
//    pauseLayer->setAnchorPoint(ccp(0,0));
//    this->addChild(pauseLayer, 1000+10);
    
//    DF::Stats::GameStats _stats;
//    CCScene* end = DF::LevelEnd::LevelEndScene::scene(_stats);
//	CCDirector::sharedDirector()->replaceScene(end);
//    CCDirector::sharedDirector()->runWithScene(end);
    
    return true;
}

void MainMenuScene::OnRecreateSingIn()
{
    // Sign in with Google to earn achievements and enable cloud sync!
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _waitForGoogle = false;
    
    CCMenuItemImage* signInGoogle = CCMenuItemImage::create(
                                                            "Interfeiss/main_menu/new/googlegames_btn0001.png",
                                                            "Interfeiss/main_menu/new/googlegames_btn0002.png",
                                                            this,
                                                            menu_selector(MainMenuScene::menuSignInGoogle));
    
    CCMenu* menuGoogle = CCMenu::create(signInGoogle,NULL);
    menuGoogle->setPosition(ccp(270, 596));//526
    menuGoogle->setTag(2222);//Remove the button after all done !!!
    getChildByTag(104)->addChild(menuGoogle, 1);
#endif
}

void MainMenuScene::sendFeedback()
{
    User::getInstance()->mFeedBackGiven = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("feedback_beta", 1);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
    JniCalls::openURLJNI("http://bit.ly/1o7kP7z");
}

void MainMenuScene::keyBackClicked()
{
    CCScene* scene = BeforeQuitScene::scene(false);
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, scene);
    CCTransitionFade* transition = CCTransitionFade::create(0.25f,scene,ccBLACK);//CCTransitionSlideInR::create(0.5f, missions);
    CCDirector::sharedDirector()->replaceScene(transition);
}

void MainMenuScene::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
    
    //Reset all stuff
    if(mResetGameVisual)
    {
        mBackMusicVolume = 1.0f;
        
        mOptionsOpen = false;
        
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
        mResetGameVisual = false;
        
        CCSprite* MapBack = (CCSprite*)getChildByTag(101);
        MapBack->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y-50));
        MapBack->setScale(2.5f);
        
        TimedSpriteAnimation* animationLogo = (TimedSpriteAnimation*)getChildByTag(105);
        animationLogo->setPosition(ccp(visibleSize.width / 2, visibleSize.height-20));
        
        CCSprite* MainGrass = (CCSprite*)getChildByTag(103);
        MainGrass->setPosition(ccp(0,visibleSize.height/2));
        
        MainGrass = (CCSprite*)getChildByTag(104);
        MainGrass->setPosition(ccp(visibleSize.width,visibleSize.height/2));
        
        //Add the logo
        MainGrass = (CCSprite*)getChildByTag(102);
        MainGrass->setPosition(ccp(visibleSize.width/2,MainGrass->getContentSize().height/2));
        
        this->scheduleUpdate();
        
        mFadeOutMusic = true;
    }
    else
    {
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/main_music.ogg", true);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/main_music.mp3", true);
#endif
        
    }
    
    //        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/music1.mp3", true);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
//        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/main_music.ogg", true);
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
//        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/main_music.mp3", true);
//#endif
    
    //Lets cross fade it?
    
    
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true) == false)
    {
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
    
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("soundsEnabled", true))
    {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
    }
    else
    {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
    }
    
    updateUI();
}

void MainMenuScene::OnFinishedMusicFadeOut()
{
    mBackMusicVolume = 0;
    
//    std::stringstream theMusicPath;
//    theMusicPath << "music/music_"<< mCurrentMusicID <<".mp3";
//    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(theMusicPath.str().c_str(), true);
    
    //Check if music is on or off
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true) == false)
        return;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/main_music.ogg", true);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/main_music.mp3", true);
#endif
    
    mFadeOutMusic = false;
    mFadeInMusic = true;
}

void MainMenuScene::update(float delta)
{
    if(mFadeOutMusic)
    {
        //Wtf !!!
        if(mFadeInMusic)
            mFadeInMusic = false;
        
        mBackMusicVolume-=delta;
        
        if(mBackMusicVolume<0)
            mBackMusicVolume = 0;
        
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(mBackMusicVolume);
        
        if(mBackMusicVolume==0)
            OnFinishedMusicFadeOut();
    }
    
    if(mFadeInMusic)
    {
        mBackMusicVolume+=delta;
        if(mBackMusicVolume>=0.8)
        {
            mBackMusicVolume = 0.8f;
            mFadeInMusic = false;
        }
        
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(mBackMusicVolume);
    }
}

void MainMenuScene::updateUI()
{
    //For now disabled
//    return;
    
    _challengesPoint1->update();
    _challengesPoint2->update();
    _storePoint1->update();
    _storePoint2->update();
    
//    _upgradesPoint1->update();
//    _upgradesPoint2->update();
    
    if (User::getInstance()->getActivationCount() % 3 == 0
        && User::getInstance()->showRateGamePopup()
        && User::getInstance()->getSessionLength() > 180
    )
    {
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        
        if(t->tm_yday != User::getInstance()->getRateGameDay())
        {
            RateGamePopup* popup = RateGamePopup::create();
            addChild(popup, 1000);
            
        }
    }
}

void MainMenuScene::onPreloadSounds()
{
    //Old stuff
//    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/171646__fins__scale-e6.wav");
//	SimpleAudioEngine::sharedEngine()->preloadEffect("meteorits/meteor_fall.wav");
//	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/Pretinieks paradas.wav");
//	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/Pretinieks pazud.wav");
//	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/soli.wav");
//	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/kristala skana.wav");
//	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/rukis sasniedz alu.wav");
//	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/rukjis saskrienas.wav");
//    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/pogas skana.wav");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // Android likes ogg :)
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/button_click.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/button_speed_off.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/button_speed_on.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/button_start_game.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/crystal_pick_up.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/diamond_intro.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_alarm.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_crash.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_enter_cave.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_freeze.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_pass_troll.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_unfreeze.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_walk.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_web_enter.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_web_exit.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_web_stuck.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/ice_break_1.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/ice_break_2.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/ice_break_3.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/meteorite_fall.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/meteorite_hit_ground.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/pause_count.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/save_me_clear.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/save_me_wait.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/troll_exit.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/troll_intro.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/troll_walk.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/troll_win.ogg");
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //iOS wav for now ???
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/button_click.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/button_speed_off.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/button_speed_on.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/button_start_game.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/crystal_pick_up.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/diamond_intro.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_alarm.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_crash.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_enter_cave.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_freeze.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_pass_troll.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_unfreeze.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_walk.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_web_enter.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_web_exit.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/dwarf_web_stuck.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/ice_break_1.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/ice_break_2.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/ice_break_3.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/meteorite_fall.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/meteorite_hit_ground.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/pause_count.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/save_me_clear.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/save_me_wait.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/troll_exit.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/troll_intro.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/troll_walk.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/troll_win.wav");
#endif
}

//This is only for testing right now - will be better in future
void MainMenuScene::onPreloadImages()
{
    //Lets preload some images =)
    
    //The troll
    CCTextureCache::sharedTextureCache()->addImage("Characters/troll/troll_diagonal_down.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/troll/troll_diagonal_up.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/troll/troll_down_front.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/troll/troll_profile.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/troll/troll_up_back.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/troll/troll_victory_dance.png");
    
    //The tall dwarf
    CCTextureCache::sharedTextureCache()->addImage("Characters/tall_dwarf/tall_dwarf_diagonal_down.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/tall_dwarf/tall_dwarf_diagonal_up.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/tall_dwarf/tall_dwarf_down.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/tall_dwarf/tall_dwarf_profile.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/tall_dwarf/tall_dwarf_up.png");
    
    //The small dwarf
    CCTextureCache::sharedTextureCache()->addImage("Characters/fat_dwarf/fat_dwarf_diagonal_down.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/fat_dwarf/fat_dwarf_diagonal_up.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/fat_dwarf/fat_dwarf_down.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/fat_dwarf/fat_dwarf_profile.png");
    CCTextureCache::sharedTextureCache()->addImage("Characters/fat_dwarf/fat_dwarf_up.png");
    
    //Some game objects
    CCTextureCache::sharedTextureCache()->addImage("diamond/diamond.png");
    
    CCTextureCache::sharedTextureCache()->addImage("effects/virpulis.png");
    CCTextureCache::sharedTextureCache()->addImage("effects/fog.png");
    CCTextureCache::sharedTextureCache()->addImage("effects/dwarf_intro_light.png");
    CCTextureCache::sharedTextureCache()->addImage("effects/dwarf_crash_boom.png");
    CCTextureCache::sharedTextureCache()->addImage("effects/crash_stars.png");
    CCTextureCache::sharedTextureCache()->addImage("effects/crash_alarming.png");
    
    CCTextureCache::sharedTextureCache()->addImage("ice_barrage/freezing_animation.png");
    CCTextureCache::sharedTextureCache()->addImage("ice_barrage/breaking_out.png");
    CCTextureCache::sharedTextureCache()->addImage("ice_barrage/state_1.png");
    CCTextureCache::sharedTextureCache()->addImage("ice_barrage/state_2.png");
    CCTextureCache::sharedTextureCache()->addImage("ice_barrage/state_3.png");
    CCTextureCache::sharedTextureCache()->addImage("ice_barrage/state_4.png");
    
    CCTextureCache::sharedTextureCache()->addImage("Crystals/blue_crystal.png");
    CCTextureCache::sharedTextureCache()->addImage("Crystals/blue_glow.png");
    CCTextureCache::sharedTextureCache()->addImage("Crystals/blue_under.png");
    
    CCTextureCache::sharedTextureCache()->addImage("Crystals/green_crystal.png");
    CCTextureCache::sharedTextureCache()->addImage("Crystals/green_glow.png");
    CCTextureCache::sharedTextureCache()->addImage("Crystals/green_under.png");
    
    CCTextureCache::sharedTextureCache()->addImage("Crystals/red_crystal.png");
    CCTextureCache::sharedTextureCache()->addImage("Crystals/red_glow.png");
    CCTextureCache::sharedTextureCache()->addImage("Crystals/red_under.png");
    
    CCTextureCache::sharedTextureCache()->addImage("Crystals/yellow_crystal.png");
    CCTextureCache::sharedTextureCache()->addImage("Crystals/yellow_glow.png");
    CCTextureCache::sharedTextureCache()->addImage("Crystals/yellow_under.png");
    
    //Some UI test???
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/diamond_btn.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/saveme_logs.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/saveme_timer.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/skip_btn0001.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/skip_btn0002.png");
    
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/x.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/0.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/1.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/2.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/3.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/4.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/5.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/6.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/7.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/8.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/9.png");
    
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/save_me/TheCrystalButtonHolder.png");
    
    //The level up and upgrades!!!
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/blue.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/fatass_dead.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/forest.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/green.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/highscore_cup.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/highscore.png");
    
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/menu0001.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/menu0002.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/play0001.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/play0002.png");
    
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/progresbar_fill.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/progress_bar_level.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/red.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/scorebar.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/share.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/stars_spin.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/tall_dead.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/troll_dance_le.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/upgrade_btn0001.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/upgrade_btn0002.png");
    CCTextureCache::sharedTextureCache()->addImage("Interfeiss/level_end/yellow.png");
    
    std::stringstream theDummy;
    for(int x=1;x<6;x++)
    {
        theDummy << "Interfeiss/upgrade_screen/elektro_" << x << ".png";
        CCTextureCache::sharedTextureCache()->addImage(theDummy.str().c_str());
        
        theDummy << "Interfeiss/upgrade_screen/ghost_" << x << ".png";
        CCTextureCache::sharedTextureCache()->addImage(theDummy.str().c_str());
        
        theDummy << "Interfeiss/upgrade_screen/plant_" << x << ".png";
        CCTextureCache::sharedTextureCache()->addImage(theDummy.str().c_str());
        
        theDummy << "Interfeiss/upgrade_screen/stomp_" << x << ".png";
        CCTextureCache::sharedTextureCache()->addImage(theDummy.str().c_str());
    }
}

////////////////////////////////////////////////////////////////////////

void MainMenuScene::onLoginSuccess(CCObject* obj)
{
    if (getChildByTag(2222))
        this->removeChildByTag(2222);
    
    if(getChildByTag(104)->getChildByTag(2222))
        getChildByTag(104)->removeChildByTag(2222);
}

void MainMenuScene::onRecievedSign(CCObject* obj)
{
    _waitForGoogle = false;//Can try again
}

void MainMenuScene::menuSignInGoogle(CCObject* sender)
{
    if (_waitForGoogle)
	{
        return;
	}
	
    _waitForGoogle = true;
    
    //Check if can sign in !!!
    JniCalls::signPlayerIn();
}

////////////////////////////////////////////////////////////////////////

void MainMenuScene::menuPlayCallback(CCObject* sender)
{
//    FlurryX::logEvent("Press Play Game");
    
    CCScene* scene;
    
    User::getInstance()->mSpecial_10_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_11_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_12_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_13_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_14_Mission = false;//Not now !!!
    
    User::getInstance()->mSpecial_16_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_17_Mission = false;//Not now !!!
    User::getInstance()->mSpecial_18_Mission = false;//Not now !!!
    
    User::getInstance()->mSpecial_19_Mission = false;
    User::getInstance()->mSpecial_20_Mission = false;
    
    User::getInstance()->mSpecial_21_Mission = false;
    User::getInstance()->mSpecial_22_Mission = false;
    User::getInstance()->mSpecial_23_Mission = false;
    
    User::getInstance()->mNewSplitCaves = false;
    
    if (StoreScene::getSpecialOfferCount() != 0
        && User::getInstance()->getStoreOfferDay() != Time::getYDay())
    {
        scene = StoreOfferPopup::scene();
    }
    else
    {
        scene = GameScene::scene();
    }
    
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, scene);
    CCTransitionFade* transition = CCTransitionFade::create(0.25f,scene,ccBLACK);
    CCDirector::sharedDirector()->replaceScene(transition);
    
	
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
    //Debug achievement show!!!
    JniCalls::completedAchievement("achievement_play_the_game",1);//completeAchievment
    
//    FlurryX::logEvent("Start Game");
}

void MainMenuScene::menuStoreCallback(CCObject* sender)
{
    AnalyticX::flurryLogEvent("Menu Show: Store");
    
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
//    CCScene* options = StoreScene::scene();
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, options);
//    CCTransitionFade* transition = CCTransitionFade::create(0.25f,options,ccBLACK);
//	CCDirector::sharedDirector()->pushScene(transition);
    
    if(mOptionsOpen)
        return;
    
//    FlurryX::logEvent("Opened Store Screen");
    
    mOptionsOpen = true;
    
    StoreScene* storeLayer = StoreScene::create();
    storeLayer->setTag(2223);
    addChild(storeLayer,100);
    
//    OptionsScene* optionsLayer = OptionsScene::create();
//    optionsLayer->setAnchorPoint(ccp(0,0));
//    addChild(optionsLayer,100);
    
    JniCalls::completedAchievement("achievement_shop_time",0);
    
}

void MainMenuScene::OnRemoveStore()
{
    AnalyticX::flurryLogEvent("Menu Hide: Store");
    
    removeChildByTag(2223,true);
    mOptionsOpen = false;
}

void MainMenuScene::menuChallengesCallback(CCObject* sender)
{
    //Old stuff
    mOptionsOpen = true;
    
    mMainMenu->setEnabled(false);
    
    AnalyticX::flurryLogEvent("Menu Show: Challenges");
    
    ChallengesScene* challengeLayer = ChallengesScene::create();
    addChild(challengeLayer,100);
    
//    FlurryX::logEvent("Opened Challenges Screen");
    
////    CCScene* options = WeeklyChallengesScene::scene();
//    CCScene* options;
//    if (User::getInstance()->getDailyChallenges().isTodaysCompleted())
//    {
//        options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::WEEKLY_CHALLENGES);
//    }
//    else
//    {
//        options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::DAILY_CHALLENGES);
//    }
////    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, options);
//    CCTransitionFade* transition = CCTransitionFade::create(0.25f,options,ccBLACK);
//	CCDirector::sharedDirector()->replaceScene(transition);
	
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
}

void MainMenuScene::menuLeaderboardsCallback(CCObject* sender)
{
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
    JniCalls::showLeaderboards();
    
    AnalyticX::flurryLogEvent("Menu Show: Leaderboards");
    
//    FlurryX::logEvent("Opened Leaders Screen");
//    JniCalls::showTwitterFollow();
    
    //Add the money count in !!!
//    JniCalls::showFacebookFollow();
}

void MainMenuScene::menuAchievementsCallback(CCObject* sender)
{
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
    JniCalls::showAchievements();
}

void MainMenuScene::menuUpgradesCallback(CCObject* sender)
{
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
    CCScene* options = UpgradeScene::scene(1);
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, options);
    CCTransitionFade* transition = CCTransitionFade::create(0.25f,options,ccBLACK);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void MainMenuScene::testCall()
{
    std::cout << "BUM\n";
}

void MainMenuScene::menuFreeDiamondsCallback(CCObject* sender)
{
    CCScene* options = FreeDiamondsScene::scene();
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, options);
    CCTransitionFade* transition = CCTransitionFade::create(0.25f,options,ccBLACK);
	CCDirector::sharedDirector()->replaceScene(transition);
    
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
}

void MainMenuScene::menuMoreGamesCallback(CCObject* sender)
{
//    window.rootViewController;
    
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
    
    AnalyticX::flurryLogEvent("Menu Open: Missions");
    
    //Open mission screen !!!
    CCScene* missions = WorldMap::scene();
    CCTransitionFade* transition = CCTransitionFade::create(0.25f,missions,ccBLACK);//CCTransitionSlideInR::create(0.5f, missions);
//    CCTransitionCrossFade* transition = CCTransitionCrossFade::create(0.4f, missions);
	CCDirector::sharedDirector()->replaceScene(transition);
    
//    FlurryX::logEvent("Opened Mission Screen");
    
    //For now disabled
//    JniCalls::completedAchievement("achievement_want_more",0);
    return;
    
//    CCScene* options = DebugScene::scene();
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, options);
//	CCDirector::sharedDirector()->replaceScene(transition);
}

void MainMenuScene::menuOptionsCallback(CCObject* sender)
{
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
//	CCScene* options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::OPTIONS);
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, options);
//    CCTransitionFade* transition = CCTransitionFade::create(0.25f,options,ccBLACK);//CCTransitionSlideInR::create(0.5f, missions);
//	CCDirector::sharedDirector()->replaceScene(transition);
    
//    setTouchEnabled(false);
    
    if(mOptionsOpen)
        return;
    
    AnalyticX::flurryLogEvent("Menu Open: Options - MainMenu");
    
    mOptionsOpen = true;
    
//    FlurryX::logEvent("Opened Main Options Screen");
    
    OptionsScene* optionsLayer = OptionsScene::create();
    optionsLayer->setAnchorPoint(ccp(0,0));
    addChild(optionsLayer,100);
}

//////////////////
#ifdef __cplusplus
extern "C" {
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jint Java_lv_gamevision_dwarfforest_DwarfForest_onLoginSuccess(JNIEnv *env, jobject thiz)
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_GOT_BONUS, NULL);
    }
    
    jint Java_lv_gamevision_dwarfforest_DwarfForest_onRecievedSign(JNIEnv *env, jobject thiz)
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_GOT_SIGN, NULL);
    }
#endif
#ifdef __cplusplus
}
#endif

bool ChallengesRedDot::init()
{
    CCSprite* dot = CCSprite::create("Interfeiss/main_menu/info_dot.png");
    addChild(dot);
    
    // skaitlis
    count = CCLabelTTF::create(toString(User::getInstance()->getChallengesCount()).c_str(),
                                           FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
                                           CCSize(52, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    count->setColor(ccc3(0xFF, 0xE3, 0x0));
    addChild(count);
    
    if (User::getInstance()->getChallengesCount() == 0)
    {
        setVisible(false);
    }
    
    //Set the upgrade stuff
    
    return true;
}

void ChallengesRedDot::update()
{
    if (User::getInstance()->getChallengesCount() > 0)
    {
        setVisible(true);
        count->setString(toString(User::getInstance()->getChallengesCount()).c_str());
    }
    else
    {
        setVisible(false);
    }
}

bool StoreRedDot::init()
{
    CCSprite* dot = CCSprite::create("Interfeiss/main_menu/info_dot.png");
    addChild(dot);
    
    // skaitlis
    count = CCLabelTTF::create(toString(User::getInstance()->getChallengesCount()).c_str(),
                               FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
                               CCSize(52, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    count->setColor(ccc3(0xFF, 0xE3, 0x0));
    addChild(count);
    
    if (User::getInstance()->getChallengesCount() == 0)
    {
        setVisible(false);
    }
    return true;
}

void StoreRedDot::update()
{
    int aTotalStuff = StoreScene::getSpecialOfferCount() + User::getInstance()->getPossibleUgrades();
//    if (StoreScene::getSpecialOfferCount() > 0)
    if(aTotalStuff>0)
    {
        setVisible(true);
//        count->setString(toString(StoreScene::getSpecialOfferCount()).c_str());
        count->setString(toString(aTotalStuff).c_str());
    }
    else
    {
        setVisible(false);
    }
}

bool UpgradesRedDot::init()
{
    CCSprite* dot = CCSprite::create("Interfeiss/main_menu/info_dot.png");
    addChild(dot);
    
    // skaitlis
    count = CCLabelTTF::create(toString(User::getInstance()->getPossibleUgrades()).c_str(),
                               FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
                               CCSize(52, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    count->setColor(ccc3(0xFF, 0xE3, 0x0));
    addChild(count);
    
    if (User::getInstance()->getPossibleUgrades() == 0)
    {
        setVisible(false);
    }
    
    //Set the upgrade stuff
    
    return true;
}

void UpgradesRedDot::update()
{
    if (User::getInstance()->getPossibleUgrades() > 0)
    {
        setVisible(true);
        count->setString(toString(User::getInstance()->getPossibleUgrades()).c_str());
    }
    else
    {
        setVisible(false);
    }
}

//----------------The map Create
//The map change effects !!!
void MainMenuScene::CreateStartMapForMenu()
{
//    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
//    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    _batchNode = CCSpriteBatchNode::create("gadalaiki.png");
//    CCLOG("width: %f",_batchNode->getContentSize().width);
//    CCLOG("he: %f",_batchNode->getContentSize().height);
    _batchNode->setPosition(ccp(0,0));
    
    CCSprite* aDummy = (CCSprite*)getChildByTag(101);
    aDummy->addChild(_batchNode);
    
//    _batchNode->setScale(2.5f);
    
    float aSkewX = (rand()%10)*0.1f;
    int aDirSkewX = rand()%2;
    
    //Augseja salina !!!
    //Aukseja salina
    CCSprite *sprTest = CCSprite::createWithSpriteFrameName("summer_resnkoks.png");
    sprTest->setTag(10400);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setPosition(ccp(471,650));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_egle.png");
    sprTest->setTag(10100);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.75f);
    sprTest->setPosition(ccp(593,667));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10300);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.6f);
    sprTest->setPosition(ccp(590,646));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_garkoks.png");
    sprTest->setTag(10200);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(1.05f);
    sprTest->setPosition(ccp(544,616));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_garkoks.png");
    sprTest->setTag(10201);
    sprTest->setScale(1.0f);
    sprTest->setPosition(ccp(420,632));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,4);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_resnkoks.png");
    sprTest->setTag(10401);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(340,618));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,5);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_egle.png");
    sprTest->setTag(10101);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.8f);
    sprTest->setPosition(ccp(410,600));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,6);
    
    //===================================================================
    //Sanu laba aukseja salina
    sprTest = CCSprite::createWithSpriteFrameName("summer_egle.png");
    sprTest->setTag(10102);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.7f);
    sprTest->setPosition(ccp(804,651));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10301);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.6f);
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(831,620));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_resnkoks.png");
    sprTest->setTag(10402);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(false);
    sprTest->setScale(1.2f);
    sprTest->setPosition(ccp(844,614));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10302);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(1.0f);
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(872,570));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_garkoks.png");
    sprTest->setTag(10202);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(true);
    sprTest->setScale(0.82f);
    sprTest->setPosition(ccp(921,550));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,4);
    
    //===================================================================
    //Sanu laba vidus salina
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_resnkoks.png");
    sprTest->setTag(10403);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(true);
    sprTest->setScale(1.0f);
    sprTest->setPosition(ccp(861,314));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_egle.png");
    sprTest->setTag(10103);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.94f);
    sprTest->setPosition(ccp(920,267));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10303);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.67f);
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(897,250));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    //===================================================================
    //Sanu laba lejus dala
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10304);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(1.16f);
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(826,44));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_garkoks.png");
    sprTest->setTag(10203);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(false);
    sprTest->setScale(0.89f);
    sprTest->setPosition(ccp(909,-17));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_resnkoks.png");
    sprTest->setTag(10404);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(true);
    sprTest->setScale(1.18f);
    sprTest->setPosition(ccp(823,-56));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_egle.png");
    sprTest->setTag(10104);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(1.08f);
    sprTest->setPosition(ccp(888,-54));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    //===================================================================
    //vidus lejus dala
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10305);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(1.10f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(482,10));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10306);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.91f);
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(431,5));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10307);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.51f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(569,10));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_resnkoks.png");
    sprTest->setTag(10405);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(true);
    sprTest->setScale(1.15f);
    sprTest->setPosition(ccp(452,-97));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    //===================================================================
    //kreisa lejas dala
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10308);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.68f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(84,70));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_egle.png");
    sprTest->setTag(10105);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.76f);
    sprTest->setPosition(ccp(18,38));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_egle.png");
    sprTest->setTag(10106);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(1.0f);
    sprTest->setPosition(ccp(-45,0));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10309);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(1.3f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(50,-2));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_garkoks.png");
    sprTest->setTag(10204);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(false);
    sprTest->setScale(1.15f);
    sprTest->setPosition(ccp(19,-89));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,4);
    
    //===================================================================
    //kreisa vidus dala
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_egle.png");
    sprTest->setTag(10109);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.75f);
    sprTest->setPosition(ccp(-3,320));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_resnkoks.png");
    sprTest->setTag(10406);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(true);
    sprTest->setScale(1.08f);
    sprTest->setPosition(ccp(-79,290));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10310);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.75f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(5,280));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_garkoks.png");
    sprTest->setTag(10205);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(false);
    sprTest->setScale(0.65f);
    sprTest->setPosition(ccp(-26,264));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    //===================================================================
    //kreisa auksas dala
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_resnkoks.png");
    sprTest->setTag(10407);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(true);
    sprTest->setScale(0.82f);
    sprTest->setPosition(ccp(-22,668));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_egle.png");
    sprTest->setTag(10107);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(true);
    sprTest->setScale(1.15f);
    sprTest->setPosition(ccp(39,653));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_resnkoks.png");
    sprTest->setTag(10408);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(false);
    sprTest->setScale(0.82f);
    sprTest->setPosition(ccp(-47,610));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_kruums.png");
    sprTest->setTag(10311);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setScale(0.82f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(47,625));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    //
    sprTest = CCSprite::createWithSpriteFrameName("summer_garkoks.png");
    sprTest->setTag(10206);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(true);
    sprTest->setScale(0.60f);
    sprTest->setPosition(ccp(23,580));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,4);
    
    sprTest = CCSprite::createWithSpriteFrameName("summer_egle.png");
    sprTest->setTag(10108);
    
    if(aDirSkewX>0)
    {
        sprTest->setSkewX(aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(aSkewX);
    }
    else
    {
        sprTest->setSkewX(-aSkewX);
        User::getInstance()->_skewPositionFromMain.push_back(-aSkewX);
    }
    aSkewX = (rand()%10)*0.1f;
    aDirSkewX = rand()%2;
    
    sprTest->setFlipX(false);
    sprTest->setScale(0.88f);
    sprTest->setPosition(ccp(-41,548));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,5);

    CCSize _screenSize = CCDirector::sharedDirector()->getVisibleSize();
    
    Machine_Stamp* aMachine1 = Machine_Stamp::create(NULL);
    aMachine1->setPosition(ccp(_screenSize.width/2+50,_screenSize.height-100));
    aMachine1->setTag(1010);
    aDummy->addChild(aMachine1,1);
    
    //The Ghost
    Machine_Ghost* aMachine2 = Machine_Ghost::create(NULL);
    aMachine2->setPosition(ccp(100,350));
    aMachine2->setTag(1020);
    aDummy->addChild(aMachine2,1);
    
    //The Enemies
    Machine_Enemies* aMachine3 = Machine_Enemies::create(NULL);
    aMachine3->setPosition(ccp(_screenSize.width-90,315));
    aMachine3->setTag(1030);
    aDummy->addChild(aMachine3,1);
}

//void GameScene::keyBackClicked()
//{
//    // This ends your app
//    CCDirector::sharedDirector()->end();
//    
//    // or do something else like pause your
//    // game with a nice "exit yes/no" dialog.
//}
