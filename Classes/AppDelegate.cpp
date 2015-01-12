#include "AppDelegate.h"
#include "HelloWorldScene.h"

#include <SimpleAudioEngine.h>

#include <vector>
#include <string>

#include "IntroScene.h"
#include "LoadingScene.h"
#include "AppMacros.h"
#include "Utils.h"
#include "StaticSceneManager.h"
#include "AnalyticX.h"
#include "User.h"
#include "UpgradeScene.h"
#include "MissionScene.h"
#include "PauseScene.h"
#include "LoadingScreen.h"
#include "WorldMap.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
    
    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionExactFit); //kResolutionNoBorder
    
    CCSize frameSize = pEGLView->getFrameSize();
    
    //Resource paths
    vector<string> searchPath;
    
    searchPath.push_back("new_resources");
    // set searching path
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
	
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    //Test resources
//    CCSprite* MainCave = CCSprite::create("Interfeiss/main_menu/new/cave_big.png");
    
    //Sprite sheets !!!
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("gadalaiki.plist");
    
    
    CCScene* aLoading = LoadingScreen::scene();
    pDirector->runWithScene(aLoading);
    
    
    // Quick map make
    // create a scene. it's an autorelease object
    
    
//    CCScene *pScene = WorldMap::scene();
    // run
//    pDirector->runWithScene(pScene);
    
//    WorldMap().scene;
//    CCScene *pScene = WorldMap::scene();
//    pDirector->runWithScene(pScene);
    
//    CCDirector::sharedDirector()->replaceScene(WorldMap);
    
    if(User::getInstance()->FirstSessionDone<2)
    {
        int _FirstSession = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("userFirstSession", 0);
        _FirstSession+=1;
        
        User::getInstance()->FirstSessionDone = _FirstSession;
        cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userFirstSession", _FirstSession);
        cocos2d::CCUserDefault::sharedUserDefault()->flush();
    }
    
    //randomize seed
    srand(Time::getTime());
    
    //Lets init the Userdata already here ?
    User::getInstance()->getDailyChallenges().checkDayCounter();
    
    User::getInstance()->setActivationCount(User::getInstance()->getActivationCount() + 1);
    User::getInstance()->startSessionTime();

    // create a scene. it's an autorelease object
//    CCScene *pScene = HelloWorld::scene();
    // run
//    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    
    CCUserDefault::sharedUserDefault()->flush();
    
    AnalyticX::flurryEndSession();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    AnalyticX::flurryStartSession("2ZCVWPY7VCRNSVQTWJJB");
#endif
    
    CCDirector::sharedDirector()->stopAnimation();
    CCDirector::sharedDirector()->resume();
    CCDirector::sharedDirector()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true))
    {
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
    
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("soundEnabled",true))
    {
        SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    }
    
    User::getInstance()->setActivationCount(User::getInstance()->getActivationCount() + 1);
    User::getInstance()->startSessionTime();
}

void AppDelegate::aplicationPathRecieved()
{
    CCLog("done !!!");
}
