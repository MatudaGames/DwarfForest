//
//  LoadingScreen.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 07/02/14.
//
//

#include "LoadingScreen.h"
#include <SimpleAudioEngine.h>
#include "Utils.h"
#include "StaticSceneManager.h"
#include "AppMacros.h"
#include "User.h"

USING_NS_CC;
using namespace CocosDenshion;

CCScene* LoadingScreen::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LoadingScreen *layer = LoadingScreen::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

bool LoadingScreen::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    mMissionMiscLoopFix = false;
    
    setTag(888);
	
    // enables back button on android
    this->setKeypadEnabled(true);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
//    CCSprite* back = CCSprite::create("loading_screen.png");
    CCSprite* back = CCSprite::create("LoadingScreen.png");
    back->setAnchorPoint(ccp(0,0));
    back->setTag(123);
    addChild(back, 0);
    
    //The loading bar !!!
    mLoadingBar = CCSprite::create("loadingbar_960.png");
    mLoadingBar->setTag(1111);
    mLoadingBar->setPosition(ccp(92,720-250));
    mLoadingBar->setAnchorPoint(ccp(0,0));
    mLoadingBar->setScaleX(0);
    back->addChild(mLoadingBar);
    
//    DF::StaticSceneManager::getInstance();
    
//    onPreloadSounds();
//    onPreloadImages();
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    
//#endif
    
    //Create the game tips
    std::stringstream aTipText;
    
    srand(Time::getTime());
    int aRandomTip = rand()%24;
    
    switch (aRandomTip) {
        case 1:
            aTipText<<"Game Tip: Complete Daily Challenge to earn diamonds";
            break;
        case 2:
            aTipText<<"Game Tip: Completing missions will grant you valuable rewards";
            break;
        case 3:
            aTipText<<"Game Tip: You can upgrade your power-up machines to make them work better";
            break;
        case 4:
            aTipText<<"Game Tip: Upgrade crystal plant, for it to yield greater harvest";
            break;
        case 5:
            aTipText<<"Game Tip: Trolls always spawn near the dwarf cave";
            break;
        case 6:
            aTipText<<"Game Tip: Master Combos by sending multiple dwarfs in cave with 2 sec interval";
            break;
        case 7:
            aTipText<<"Game Tip: If a dwarf gets stuck in the Ice cube - tap on it 3 times to free him";
            break;
        case 8:
            aTipText<<"Game Tip: Watch out for meteorite fall patches – it’s a trap";
            break;
        case 9:
            aTipText<<"Game Tip: Using power-up machines costs 5 diamonds.\nYou can earn them or buy at the store.";
            break;
        case 10:
            aTipText<<"Game Tip: Short on diamonds?\nComplete missions or buy them at the store.";
            break;
        case 11:
            aTipText<<"Game Tip: Use crystals to improve your power-up machines";
            break;
        case 12:
            aTipText<<"Game Tip: Lost a game?\nDon’t worry you can use “Save me” to continue playing.";
            break;
        case 13:
            aTipText<<"Game Tip: If a tornado pulls a dwarf in and crashes into another dwarf game will end.";
            break;
        case 14:
            aTipText<<"Game Tip: Don’t step into the puddles – dwarfs will slip and fall.";
            break;
        case 15:
            aTipText<<"Game Tip: You can turn off sound and music in the settings menu";
            break;
        case 16:
            aTipText<<"Game Tip: Like „Dwarf Forrest” on Facebook and get diamonds in reward";
            break;
        case 17:
            aTipText<<"Game Tip: Fallow „Dwarf Forrest” on Twitter and get diamonds in reward";
            break;
        case 18:
            aTipText<<"Game Tip: Check out Challenges to see how you can earn more diamonds";
            break;
        case 19:
            aTipText<<"Game Tip: Gather mushrooms to give yourself a boost";
            break;
        case 20:
            aTipText<<"Game Tip: If you get stuck in the Web or fall in to the puddle\n– consider using Smasher power-up to clear game map.";
            break;
        case 21:
            aTipText<<"Game Tip: Too many dwarfs to handle?\nUse Ghost Dwarf power up.";
            break;
        case 22:
            aTipText<<"Game Tip: Trolls getting on your nerves?\nUse Tesla power-up to lighting blast them.";
            break;
        case 23:
            aTipText<<"Game Tip: Game getting too easy?\n Use „fast forward” button to speed the game up. ";
            break;
            
        default:
            aTipText<<"Game Tip: Orange Dwarf moves faster than the Blue one.\nPlan trajectory accordingly";
            break;
    }
    
    //Create some random game tip
    CCLabelTTF* aStoreHeader = CCLabelTTF::create(aTipText.str().c_str(), "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                                  CCSize(visibleSize.width, 100), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setPosition(ccp(visibleSize.width/2,50));
    aStoreHeader->setColor(ccc3(255, 255, 255));
    aStoreHeader->enableShadow(CCSize(3,-3),3,false);
    back->addChild(aStoreHeader);
    
    mMusic1_On = false;
    mMusic2_On = false;
    
    this->scheduleUpdate();
    
    CCDelayTime* aDelay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(LoadingScreen::StartLoading));
    CCSequence* aSeq = CCSequence::create(aDelay,func,NULL);
    this->runAction(aSeq);
    
    return true;
}

void LoadingScreen::update(float delta)
{
    float aVolume = 0;
    
    /*
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(!mMusic1_Done)
    {
        if(mMusic1_On)
        {
            //Check the volume
            channel1->getVolume(&aVolume);
            if(aVolume<1.0f)
            {
                aVolume+=delta;
                if(aVolume>=1.0f)
                {
                    aVolume = 1.0f;
                    mMusic1_Done = true;
                }
                channel1->setVolume(aVolume);
            }
        }
        else
        {
            channel1->getVolume(&aVolume);
            if(aVolume>0.0f)
            {
                aVolume-=delta;
                if(aVolume<=0.0f)
                {
                    aVolume = 0.0f;
                    mMusic1_Done = true;
                }
                channel1->setVolume(aVolume);
            }
        }
    }
    
    if(!mMusic2_Done)
    {
        if(mMusic2_On)
        {
            //Check the volume
            channel2->getVolume(&aVolume);
            if(aVolume<1.0f)
            {
                aVolume+=delta;
                if(aVolume>=1.0f)
                {
                    aVolume = 1.0f;
                    mMusic2_Done = true;
                }
                channel2->setVolume(aVolume);
            }
        }
        else
        {
            channel2->getVolume(&aVolume);
            if(aVolume>0.0f)
            {
                aVolume-=delta;
                if(aVolume<=0.0f)
                {
                    aVolume = 0.0f;
                    mMusic2_Done = true;
                }
                channel2->setVolume(aVolume);
            }
        }
    }
    
    if(system)
        system->update();
#endif
    */
}

void LoadingScreen::StartLoading()
{
    DF::StaticSceneManager::getInstance();
    
    mLoadingBar->setScaleX(0.5f);
    
    onPreloadSounds();
    
    mLoadingBar->setScaleX(0.75f);
    
    onPreloadImages();
    
    mLoadingBar->setScaleX(1.0f);
    
    AllCompleted();
}

/*
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
//        Common_Fatal("FMOD error %d - %s", result, FMOD_ErrorString(result));
        CCLog("FMOD error %d - %s", result, FMOD_ErrorString(result));
    }
}

const char *Common_MediaPath(const char *fileName)
{
    
//    CCLog("[[NSBundle mainBundle] resourcePath]: %s",[[NSBundle mainBundle] resourcePath]);
    
    
//    return [[NSString stringWithFormat:@"%@/media/%s",
//             [[NSBundle mainBundle] resourcePath],
//             "test"] UTF8String];
    
//    return [[NSString stringWithFormat:@"%@/media/%s", [[NSBundle mainBundle] resourcePath], fileName] UTF8String];
    
    return CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName).c_str();
    
    std::stringstream aString;
    aString<<"new_resources/Sound/"<<fileName<<".wav";
    
//    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
    
    std::vector<std::string> vector = CCFileUtils::sharedFileUtils()->getSearchPaths();
    
    
    std::stringstream pathE;
    pathE<<vector[0];
    
    CCLog("Path 0:%s",pathE.str().c_str());
    
    return aString.str().c_str();
}
*/

void LoadingScreen::AllCompleted()
{
    /* //The FMOD test
    unsigned int      version;
    void             *extradriverdata = 0;
    
    result = FMOD::System_Create(&system);
    ERRCHECK(result);
    
    result = system->getVersion(&version);
    ERRCHECK(result);
    
    if (version < FMOD_VERSION)
    {
        CCLog("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
    }
    
    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    ERRCHECK(result);
    
//    CCLog("Path: %s",CCFileUtils::sharedFileUtils()->fullPathForFilename("music/music_1s.wav").c_str());
    
    result = system->createSound(Common_MediaPath("music/music_1s.wav"), FMOD_DEFAULT, 0, &sound1);
    ERRCHECK(result);
    
    result = system->createSound(Common_MediaPath("music/music_2s.wav"), FMOD_DEFAULT, 0, &sound2);
    ERRCHECK(result);
    
    result = system->createSound(Common_MediaPath("music/music_3s.wav"), FMOD_DEFAULT, 0, &sound3);
    ERRCHECK(result);
    
    //Create some sound external
    result = system->createSound(Common_MediaPath("Sound/combo_1.wav"), FMOD_DEFAULT, 0, &sound4);
    ERRCHECK(result);
    
    unsigned int lenms = 0;
    unsigned int ms = 0;
    bool         playing = false;
    bool         paused = false;
    
    result = system->playSound(sound1, 0, false, &channel);
    ERRCHECK(result);
    
    result = system->playSound(sound2, 0, false, &channel1);
    channel1->setVolume(0);
    ERRCHECK(result);

    result = system->playSound(sound3, 0, false, &channel2);
    channel2->setVolume(0);
    ERRCHECK(result);
    
    result = sound1->getLength(&lenms, FMOD_TIMEUNIT_MS);
    if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
    {
        ERRCHECK(result);
    }
    
    CCLog("Length,%u",(lenms / 1000 / 60));
    
    result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
    if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
    {
        ERRCHECK(result);
    }
    
    CCLog("Time %02d:%02d:%02d/%02d:%02d:%02d : %s", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100, paused ? "Paused " : playing ? "Playing" : "Stopped");
    
    system->update();
    
    CCMenuItemImage* clouseButton = CCMenuItemImage::create(
                                                            "Interfeiss/buttons/x_btn0001.png",
                                                            "Interfeiss/buttons/x_btn0002.png",
                                                            this,
                                                            menu_selector(LoadingScreen::AddExtraDwarf));
    
    CCMenuItemImage* clouseButton2 = CCMenuItemImage::create(
                                           "Interfeiss/buttons/x_btn0001.png",
                                           "Interfeiss/buttons/x_btn0002.png",
                                           this,
                                           menu_selector(LoadingScreen::RemoveDwarf));
    
    CCMenuItemImage* clouseButton3 = CCMenuItemImage::create(
                                                             "Interfeiss/buttons/x_btn0001.png",
                                                             "Interfeiss/buttons/x_btn0002.png",
                                                             this,
                                                             menu_selector(LoadingScreen::CreateRandomFX));
    
    CCMenu* aClouse = CCMenu::create(clouseButton,clouseButton2,clouseButton3,NULL);
    aClouse->alignItemsHorizontally();
    aClouse->setPosition(ccp(300,100));
    addChild(aClouse,1);
    
    return;
    */
    
    // Do not switch to mission screen - 1st check whats going on with the mission file !!!
    
    
    // Do this step only when mission manager has loaded all the needed data !!!
    /*
    CCScene* options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MISSIONS);;
    CCTransitionFade* transition = CCTransitionFade::create(0.25f,options,ccBLACK);//CCTransitionSlideInR::create(0.5f, missions);
    CCDirector::sharedDirector()->replaceScene(transition);
    */
    
    mResourceLoadingCompleted = true;
    
    if(mMissionLoadingCompleted){
        // If all is completed - go fuher
        CCLog("Did come to what???");
        OnContinueGameInit();
    }
    
    // Do the other stuff
    
//    if(User::getInstance()->FirstSessionDone<2)
//    {
//        int _FirstSession = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("userFirstSession", 0);
//        _FirstSession+=1;
//        
//        User::getInstance()->FirstSessionDone = _FirstSession;
//        cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userFirstSession", _FirstSession);
//        cocos2d::CCUserDefault::sharedUserDefault()->flush();
//    }
//    
//    //randomize seed
//    srand(Time::getTime());
//    
//    //Lets init the Userdata already here ?
////    User::getInstance()->getDailyChallenges().checkDayCounter();
//    
//    User::getInstance()->setActivationCount(User::getInstance()->getActivationCount() + 1);
//    User::getInstance()->startSessionTime();
    
//    DF::StaticSceneManager::getInstance();
    
    User::getInstance()->getMissionManager().OnDownloadSpecialMissions();
}

void LoadingScreen::OnMissionsLoaded()
{
    CCLog("### Downloaded and parsed all Mission info ###");
    mMissionLoadingCompleted = true;
    
    // Start with data donwload/parse now
    User::getInstance()->getItemDataManager().OnDownloadData();
}

// New steps - downloads shop & other info !!!
void LoadingScreen::OnItemDataDownloaded()
{
    CCLog("### Downloaded and parsed all Items Data ###");
    mItemDataCompleted = true;
    
    if(mResourceLoadingCompleted && mMissionLoadingCompleted){
        OnContinueGameInit();
    }
}

void LoadingScreen::OnContinueGameInit()
{
    CCLog("### All Loading completed - Start game ###");
    // If all is completed - go fuher
    CCScene* options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MISSIONS);;
    CCTransitionFade* transition = CCTransitionFade::create(0.25f,options,ccBLACK);//CCTransitionSlideInR::create(0.5f, missions);
    CCDirector::sharedDirector()->replaceScene(transition);
}


void LoadingScreen::onEnter()
{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void LoadingScreen::onExit()
{
    // Try to clean up stuff?
    
//    getChildByTag(123)->release();
    removeAllChildrenWithCleanup(true);
    
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

/*
void LoadingScreen::CreateRandomFX()
{
    result = system->playSound(sound4, 0, false, &channel3);
    ERRCHECK(result);
}

//Cross fade music
void LoadingScreen::AddExtraDwarf()
{
    mMusic1_On = !mMusic1_On;
    mMusic1_Done = false;
    
//    float aVolume = 0;
//    channel1->getVolume(&aVolume);
//    if(aVolume>0)
//    {
//        result = channel1->setVolume(0);
//        ERRCHECK(result);
//    }
//    else
//    {
//        result = channel1->setVolume(1);
//        ERRCHECK(result);
//    }
//    
//    system->update();
    
//    result = system->playSound(sound2, 0, false, &channel1);
//    ERRCHECK(result);

}

void LoadingScreen::RemoveDwarf()
{
    mMusic2_On = !mMusic2_On;
    mMusic2_Done = false;
    
//    float aVolume = 0;
//    channel2->getVolume(&aVolume);
//    if(aVolume>0)
//    {
//        result = channel2->setVolume(0);
//        ERRCHECK(result);
//    }
//    else
//    {
//        result = channel2->setVolume(1);
//        ERRCHECK(result);
//    }
//    
//    system->update();
}
*/

void LoadingScreen::onPreloadSounds()
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
    
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/music_GriegLoop.ogg");
//    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/music_loop.ogg");
//    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/music_intro_s.ogg");
    
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
void LoadingScreen::onPreloadImages()
{
    
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("gadalaiki.plist");
    CCTextureCache::sharedTextureCache()->addImage("gadalaiki/summer/map_summer.png");
    /*
    CCTextureCache::sharedTextureCache()->addImage("gadalaiki/autumn/map_autumn.png");
    CCTextureCache::sharedTextureCache()->addImage("gadalaiki/winter/map_winter.png");
    CCTextureCache::sharedTextureCache()->addImage("gadalaiki/spring/map_spring.png");
    */
    
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
    /*
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
    */
    
    /*
    std::stringstream theDummy;
    for(int x=1;x<6;x++)
    {
        theDummy << "Interfeiss/upgrade_screen/elektro_" << x << ".png";
        CCTextureCache::sharedTextureCache()->addImage(theDummy.str().c_str());
        theDummy.str("");
        theDummy.clear();
        
        theDummy << "Interfeiss/upgrade_screen/ghost_" << x << ".png";
        CCTextureCache::sharedTextureCache()->addImage(theDummy.str().c_str());
        theDummy.str("");
        theDummy.clear();
        
        theDummy << "Interfeiss/upgrade_screen/plant_" << x << ".png";
        CCTextureCache::sharedTextureCache()->addImage(theDummy.str().c_str());
        theDummy.str("");
        theDummy.clear();
        
        theDummy << "Interfeiss/upgrade_screen/stomp_" << x << ".png";
        CCTextureCache::sharedTextureCache()->addImage(theDummy.str().c_str());
        theDummy.str("");
        theDummy.clear();
    }
    */
    
}