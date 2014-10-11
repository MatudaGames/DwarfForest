#include "OptionsScene.h"
#include "TutorialScene.h"
#include "IntroScene.h"
#include "StaticSceneManager.h"
#include <SimpleAudioEngine.h>

#include "JniCalls.h"
#include "User.h"
#include "AppMacros.h"
#include "MainMenuScene.h"

#include "StatsScreen.h"

#include "AnalyticX.h"

USING_NS_CC;

CCScene* OptionsScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    OptionsScene *layer = OptionsScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

bool OptionsScene::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(touch->getLocation().y<aBase->getPositionY()+aBase->getContentSize().height/2 && touch->getLocation().y>aBase->getPositionY()-aBase->getContentSize().height/2 &&
       touch->getLocation().x>aBase->getPositionX()-aBase->getContentSize().width/2 && touch->getLocation().x<aBase->getPositionX()+aBase->getContentSize().width/2)
        return false;
    
    //Clouse it by outside click !!!
    RemovePopup();
    
    return true;
}

void OptionsScene::RemovePopup()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Tap_Back").c_str());
    
    CCLayerColor* aLayer = (CCLayerColor*)getChildByTag(888);
    CCFadeTo* aFade = CCFadeTo::create(0.25f,0);
    aLayer->runAction(aFade);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(visibleSize.width/2,visibleSize.height+200));
    CCEaseElasticIn* aEase1 = CCEaseElasticIn::create(aMove1,1.0f);
    CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(OptionsScene::RealRemove));
    CCSequence* aSeq = CCSequence::create(aEase1,func2,NULL);
    aBase->runAction(aSeq);
//    this->getParent()->removeChild(this);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Popup_Exit").c_str());
    
    AnalyticX::flurryLogEvent("Menu Clouse: Options - MainMenu");
}

void OptionsScene::RealRemove()
{
    MainMenuScene* mainMenu = (MainMenuScene*)this->getParent();
    mainMenu->mOptionsOpen = false;
    
//    CCScene* options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
//    options->mOptionsOpen = false;
//    this->getParent()->mOptionsOpen = false;
    this->getParent()->removeChild(this);
}

void OptionsScene::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
//    CCLOG("Did touch screen");
}

void OptionsScene::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    
}

void OptionsScene::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void OptionsScene::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

// on "init" you need to initialize your instance
bool OptionsScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Popup_Enter").c_str());
    
//    setTouchPriority(-128);
	
    // enables back button on android
    this->setKeypadEnabled(true);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //The new stuff comes here !!!
    
    //Maybe fade it in?
    CCLayerColor* aBG = CCLayerColor::create(ccc4(0,0,0,64),visibleSize.width,visibleSize.height);
    aBG->setOpacity(0);
    aBG->setTag(888);
    addChild(aBG,0);
    
    CCFadeTo* aFade1 = CCFadeTo::create(0.25f,64);
    aBG->runAction(aFade1);
    
    //The center bg
    aBase = CCSprite::create("Interfeiss/options/new/options_bg.png");
//    aBase->setPosition(ccp(visibleSize.width/2,visibleSize.height/2+30));
    aBase->setPosition(ccp(visibleSize.width/2,visibleSize.height+200));
    aBase->setTag(999);
    addChild(aBase,1);
    
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(visibleSize.width/2,visibleSize.height/2+30));
    CCEaseElasticOut* aEase1 = CCEaseElasticOut::create(aMove1,1.0f);
    aBase->runAction(aEase1);
    
    //The clouse button ??? can't find it - skip
    
    //The header
    CCLabelTTF* aHeader = CCLabelTTF::create("Options", FONT_SKRANJI, TITLE_FONT_SIZE, CCSize(200, 60), kCCTextAlignmentCenter);
    aHeader->setPosition(ccp(aBase->getContentSize().width/2,aBase->getContentSize().height-44));
    aHeader->setColor(ccc3(104, 85, 49));
    aBase->addChild(aHeader);
    
    //The google sing out stuff !!!
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCMenuItemImage* signOutGoogle = CCMenuItemImage::create(
                                                            "Interfeiss/main_menu/new/googlegames_btn0001.png",
                                                            "Interfeiss/main_menu/new/googlegames_btn0002.png",
                                                            this,
                                                            menu_selector(OptionsScene::menuSingOutClick));
    CCMenu* menuGoogle = CCMenu::create(signOutGoogle,NULL);
    menuGoogle->setPosition(ccp(aBase->getContentSize().width/2, -50));//526
    menuGoogle->setTag(2222);//Remove the button after all done !!!
//    aBase->addChild(menuGoogle);
    std::cout << "Will check if player is on\n";
//    std::cout<< "JniCalls::isSignedIn():"<<JniCalls::isSignedIn();
//    CCLog("JniCalls::isSignedIn(): %i",JniCalls::isSignedIn());
    
    //Try to ask nice about to sign in
    if (JniCalls::isSignedIn())
    {
        aBase->addChild(menuGoogle);
    }
#endif
    
//    CCLog("JniCalls::isSignedIn(): %i",JniCalls::isSignedIn());
    //=========================================
    
    //Now comes the buttons UP part!!!
    bool musicEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true);
    bool soundsEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("soundsEnabled", true);
    bool vibrationEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("vibrationEnabled", true);
    
    CCMenuItemImage* s1 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_sound_off.png",
                                                  "Interfeiss/pause_menu/btn_sound.png", this, NULL);
    
    CCMenuItemImage* s2 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_sound.png",
                                                  "Interfeiss/pause_menu/btn_sound_off.png", this, NULL);
    
    CCMenuItemToggle* soundButton = CCMenuItemToggle::createWithTarget(
                                                                       this, menu_selector(OptionsScene::menuSoundCallback), s1, s2, NULL);
    soundButton->setSelectedIndex(soundsEnabled);
    
    CCMenuItemImage* m1 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_music_off.png",
                                                  "Interfeiss/pause_menu/btn_music.png", this, NULL);
    
    CCMenuItemImage* m2 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_music.png",
                                                  "Interfeiss/pause_menu/btn_music_off.png", this, NULL);
    
    CCMenuItemToggle* musicButton = CCMenuItemToggle::createWithTarget(
                                                                       this, menu_selector(OptionsScene::menuMusicCallback), m1, m2, NULL);
    musicButton->setSelectedIndex(musicEnabled);
    
    CCMenuItemImage* v1 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_vbration_off.png",
                                                  "Interfeiss/pause_menu/btn_vbration.png", this, NULL);
    
    CCMenuItemImage* v2 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_vbration.png",
                                                  "Interfeiss/pause_menu/btn_vbration_off.png", this, NULL);
    
    CCMenuItemToggle* vibrateButton = CCMenuItemToggle::createWithTarget(
                                                                         this, menu_selector(OptionsScene::menuVibrationCallback), v1, v2, NULL);
    vibrateButton->setSelectedIndex(vibrationEnabled);
    
    CCMenu* lowBar = CCMenu::create(soundButton,musicButton,vibrateButton, NULL);
    lowBar->alignItemsHorizontallyWithPadding(20);
    lowBar->setPosition(aBase->getContentSize().width/2, aBase->getContentSize().height-112);
    aBase->addChild(lowBar, 1);
    
    //Add some text fields for buttons !!!
    
    //the extra clouse button!
    CCMenuItemImage* clouseButton = CCMenuItemImage::create(
                                                           "Interfeiss/buttons/x_btn0001.png",
                                                           "Interfeiss/buttons/x_btn0002.png",
                                                           this,
                                                           menu_selector(OptionsScene::RemovePopup));
    CCMenu* aClouse = CCMenu::create(clouseButton,NULL);
    aClouse->setPosition(ccp(aBase->getContentSize().width-20,aBase->getContentSize().height-20));
    aBase->addChild(aClouse,1);
    
    
    // skaitlis
    CCLabelTTF* button_txt_sound = CCLabelTTF::create("Sound",
                                                      FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                                      CCSize(100, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    button_txt_sound->setColor(ccc3(33, 59, 60));
    button_txt_sound->setPositionX(lowBar->getPositionX()+soundButton->getPositionX());
    button_txt_sound->setPositionY(aBase->getContentSize().height-160);
    aBase->addChild(button_txt_sound);
    
    CCLabelTTF* button_txt_music = CCLabelTTF::create("Music",
                                                      FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                                      CCSize(100, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    button_txt_music->setColor(ccc3(33, 59, 60));
    button_txt_music->setPositionX(lowBar->getPositionX()+musicButton->getPositionX());
    button_txt_music->setPositionY(aBase->getContentSize().height-160);
    aBase->addChild(button_txt_music);
    
    CCLabelTTF* button_txt_vib = CCLabelTTF::create("Vibration",
                                                    FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                                    CCSize(100, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    button_txt_vib->setColor(ccc3(33, 59, 60));
    button_txt_vib->setPositionX(lowBar->getPositionX()+vibrateButton->getPositionX());
    button_txt_vib->setPositionY(aBase->getContentSize().height-160);
    aBase->addChild(button_txt_vib);
    
    //The 4 magic buttons
    
    CCMenuItemImage* introButton = CCMenuItemImage::create(
                                                           "Interfeiss/options/new/intro_btn0001.png",
                                                           "Interfeiss/options/new/intro_btn0002.png",
                                                           this,
                                                           menu_selector(OptionsScene::menuIntroCallback));
    
    introButton->setPosition(ccp(-88,35));
    
    CCMenuItemImage* tutorialButton = CCMenuItemImage::create(
                                                              "Interfeiss/options/new/tutorial_btn0001.png",
                                                              "Interfeiss/options/new/tutorial_btn0002.png",
                                                              this,
                                                              menu_selector(OptionsScene::menuTutorialCallback));
    
    tutorialButton->setPosition(ccp(88,-35));
    
    CCMenuItemImage* creditsButton = CCMenuItemImage::create(
                                                             "Interfeiss/options/new/credits_btn0001.png",
                                                             "Interfeiss/options/new/credits_btn0002.png",
                                                             this,
                                                             menu_selector(OptionsScene::menuCreditsCallback));
    
    creditsButton->setPosition(ccp(88,35));
    
    CCMenuItemImage* statsButton = CCMenuItemImage::create(
                                                           "Interfeiss/options/new/stats_btn0001.png",
                                                           "Interfeiss/options/new/stats_btn0002.png",
                                                           this,
                                                           menu_selector(OptionsScene::menuStatsCallback));
    
    statsButton->setPosition(ccp(-88,-35));
    
    
    
    
    CCMenu* creditsMenu = CCMenu::create(introButton,tutorialButton,statsButton, creditsButton, NULL);
    creditsMenu->setPosition(ccp(aBase->getContentSize().width/2,aBase->getContentSize().height/2-80));
//    creditsMenu->alignItemsHorizontallyWithPadding(-20);
//    creditsMenu->setPositionY(120);
    aBase->addChild(creditsMenu, 1);
    
    return true;
    //------------------------------------------------------------------
    
    //Some black bg
//    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
//    back->setOpacity(64);
//    back->setAnchorPoint(ccp(0,0));
//    addChild(back, 0);
    
    /*
	CCMenuItemImage* backButton = CCMenuItemImage::create(
														  "Interfeiss/back_btn0001.png",
														  "Interfeiss/back_btn0002.png",
														  this,
														  menu_selector(OptionsScene::menuBackCallback));
	backButton->setAnchorPoint(ccp(0,0));
    
    CCMenu* backMenu = CCMenu::create(backButton, NULL);
    this->addChild(backMenu, 1);
    backMenu->setPosition(0, 0);
    
    //Rest button
    CCMenuItemImage* resetButton = CCMenuItemImage::create(
														  "Interfeiss/reset_btn0001.png",
														  "Interfeiss/reset_btn0002.png",
														  this,
														  menu_selector(OptionsScene::menuResetCallback));
	resetButton->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* resetButton2 = CCMenuItemImage::create(
                                                           "Interfeiss/tutorial/got_it_btn0001.png",
                                                           "Interfeiss/tutorial/got_it_btn0002.png",
                                                           this,
                                                           menu_selector(OptionsScene::menuEnableDebugButtons));
	resetButton2->setAnchorPoint(ccp(0,0));
    
    CCMenu* resetMenu = CCMenu::create(resetButton,resetButton2, NULL);
    resetMenu->alignItemsVertically();
    this->addChild(resetMenu, 1);
    resetMenu->setPosition(50, visibleSize.height/2+125);
    
    
    // intro + tutorial
    
    CCMenuItemImage* introButton = CCMenuItemImage::create(
														  "Interfeiss/options/intro_btn.png",
														  "Interfeiss/options/intro_btn_pressed.png",
														  this,
														  menu_selector(OptionsScene::menuIntroCallback));
    
    CCMenuItemImage* tutorialButton = CCMenuItemImage::create(
                                                              "Interfeiss/options/tutorial_btn.png",
                                                              "Interfeiss/options/tutorial_btn_pressed.png",
                                                              this,
                                                              menu_selector(OptionsScene::menuTutorialCallback));
    

    CCMenu* introMenu = CCMenu::create(tutorialButton, introButton, NULL);
    introMenu->alignItemsHorizontallyWithPadding(-20);
    introMenu->setPositionY(250);
    this->addChild(introMenu, 1);
    
    // credits
    
    CCMenuItemImage* creditsButton = CCMenuItemImage::create(
                                                             "Interfeiss/options/credits_btn.png",
                                                             "Interfeiss/options/credits_btn_pressed.png",
                                                             this,
                                                             menu_selector(OptionsScene::menuCreditsCallback));
    
    CCMenuItemImage* statsButton = CCMenuItemImage::create(
                                                             "Interfeiss/options/stats.png",
                                                             "Interfeiss/options/stats_pressed.png",
                                                             this,
                                                             menu_selector(OptionsScene::menuStatsCallback));
    
    
    CCMenu* creditsMenu = CCMenu::create(statsButton, creditsButton, NULL);
    creditsMenu->alignItemsHorizontallyWithPadding(-20);
    creditsMenu->setPositionY(120);
    this->addChild(creditsMenu, 1);
    
    // title
    
    CCSprite* labels = CCSprite::create("Interfeiss/options/title_text.png");
    labels->setAnchorPoint(ccp(0.5, 1));
    labels->setPositionX(visibleSize.width / 2);
    labels->setPositionY(visibleSize.height - 50);
    addChild(labels);
    
    // options
    
    CCMenuItemImage* s1 = CCMenuItemImage::create("Interfeiss/checkbox_empty.png",
                                                  "Interfeiss/checkbox_checked.png", this, NULL);
    
    CCMenuItemImage* s2 = CCMenuItemImage::create("Interfeiss/checkbox_checked.png",
                                                  "Interfeiss/checkbox_empty.png", this, NULL);
    
    CCMenuItemToggle* soundButton = CCMenuItemToggle::createWithTarget(
                        this, menu_selector(OptionsScene::menuSoundCallback), s1, s2, NULL);
   
    
    CCMenuItemImage* m1 = CCMenuItemImage::create("Interfeiss/checkbox_empty.png",
                                                  "Interfeiss/checkbox_checked.png", this, NULL);
    
    CCMenuItemImage* m2 = CCMenuItemImage::create("Interfeiss/checkbox_checked.png",
                                                  "Interfeiss/checkbox_empty.png", this, NULL);
    
    CCMenuItemToggle* musicButton = CCMenuItemToggle::createWithTarget(
                        this, menu_selector(OptionsScene::menuMusicCallback), m1, m2, NULL);
    
    CCMenuItemImage* v1 = CCMenuItemImage::create("Interfeiss/checkbox_empty.png",
                                                  "Interfeiss/checkbox_checked.png", this, NULL);
    
    CCMenuItemImage* v2 = CCMenuItemImage::create("Interfeiss/checkbox_checked.png",
                                                  "Interfeiss/checkbox_empty.png", this, NULL);
    
    CCMenuItemToggle* vibrationButton = CCMenuItemToggle::createWithTarget(
                         this, menu_selector(OptionsScene::menuVibrationCallback), v1, v2, NULL);
    
    bool musicEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true);
    bool soundsEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("soundsEnabled", true);
    bool vibrationEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("vibrationEnabled", true);
    
    musicButton->setSelectedIndex(musicEnabled);
    soundButton->setSelectedIndex(soundsEnabled);
    vibrationButton->setSelectedIndex(vibrationEnabled);
    
    vibrationButton->setPosition(85, 5);
    soundButton->setPosition(85, 185);
    musicButton->setPosition(85, 95 );
    
    CCMenu* mainMenu = CCMenu::create(soundButton, musicButton, vibrationButton, NULL);
    this->addChild(mainMenu, 1);
    
    //Debug button for now???
    _fbButtonLogIn = CCMenuItemImage::create(
                                                           "Interfeiss/FB_Login.png",
                                                           "Interfeiss/FB_Login.png",
                                                           this,
                                                           menu_selector(OptionsScene::loginFB));
    _fbButtonLogOut = CCMenuItemImage::create(
                                                        "Interfeiss/FB_Logout.png",
                                                        "Interfeiss/FB_Logout.png",
                                                        this,
                                                        menu_selector(OptionsScene::logoutFB));
    
    CCMenu* fb_buttons = CCMenu::create(_fbButtonLogIn, _fbButtonLogOut, NULL);
    fb_buttons->setPosition(100, 300);
    this->addChild(fb_buttons, 1);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //Try to ask nice about to sign in
    std::cout << "Will check if player is on fb\n";
    if (JniCalls::isFacebookSigned())
    {
        CCLog("ALL OK - PLAYER IS SIGNED IN FB");
        _fbButtonLogOut->setVisible(true);
        _fbButtonLogIn->setVisible(false);
    }
    else
    {
        CCLog("ALL BAD - PLAYER IS NOT SIGNED IN FB");
        _fbButtonLogOut->setVisible(false);
        _fbButtonLogIn->setVisible(true);
    }
#endif
    
    return true;
    */
}

void OptionsScene::keyBackClicked()
{
    menuBackCallback(NULL);
}

//Try to log in to fb or out

void OptionsScene::loginFB(CCObject* sender)
{
    //Check if need to set button to logout??
    _fbButtonLogOut->setVisible(true);
    _fbButtonLogIn->setVisible(false);
    
    JniCalls::loginFacebook();
}

void OptionsScene::logoutFB(CCObject* sender)
{
    _fbButtonLogOut->setVisible(false);
    _fbButtonLogIn->setVisible(true);
    
    JniCalls::logoutFacebook();
}

void OptionsScene::onEnterTransitionDidFinish()
{
//    CCLog("TRANSITION FINISHED - CHECK FB");
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    //Try to ask nice about to sign in
//    if (JniCalls::isFacebookSigned())
//    {
//        CCLog("ALL OK - PLAYER IS SIGNED IN FB");
//        _fbButtonLogOut->setVisible(true);
//        _fbButtonLogIn->setVisible(false);
//    }
//    else
//    {
//        CCLog("ALL BAD - PLAYER IS NOT SIGNED IN FB");
//        _fbButtonLogOut->setVisible(false);
//        _fbButtonLogIn->setVisible(true);
//    }
//#endif
}


////////////////////////////

void OptionsScene::menuEnableDebugButtons(CCObject* sender)
{
//    User::getInstance()->getDebugButtonsState();
    User::getInstance()->ChangeDebugButtons();
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void OptionsScene::menuResetCallback(CCObject* sender)
{
    //Reset all game
    User::getInstance()->ResetData();
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void OptionsScene::menuBackCallback(CCObject* sender)
{
//	CCScene* mainMenu = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
//	CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, mainMenu);
//	CCDirector::sharedDirector()->replaceScene(transition);
    
//    GameScene* gameScene = static_cast<GameScene*>(this->getParent());
//    gameScene->onResumeFromPause();
    
	this->getParent()->removeChild(this);
}

void OptionsScene::menuCreditsCallback(cocos2d::CCObject *sender)
{
    //Reset game !!!
    User::getInstance()->ResetData();
    
    AnalyticX::flurryLogEvent("Menu Option: Show Credits");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void OptionsScene::menuStatsCallback(cocos2d::CCObject *sender)
{
//    CCScene* tutorial = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::STATS);
//	CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, tutorial);
//	CCDirector::sharedDirector()->replaceScene(transition);
    
    if(getChildByTag(12345))
        return;//Noo
    
    AnalyticX::flurryLogEvent("Menu Option: Show Stats");
    
    StatsScreen* aScreen = StatsScreen::create();
    aScreen->setTag(12345);
    addChild(aScreen,100);
    
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void OptionsScene::OnRemoveStats()
{
    AnalyticX::flurryLogEvent("Menu Option: Hide Stats");
    
    removeChildByTag(12345,true);
}

void OptionsScene::menuIntroCallback(cocos2d::CCObject *sender)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
//    CCScene* tutorial = IntroScene::scene();
//	CCTransitionScene* transition = CCTransitionProgressInOut::create(0.5f, tutorial);
//	CCDirector::sharedDirector()->replaceScene(transition);
    
    IntroScene* tutorial = IntroScene::create(false);
    tutorial->setScale(0.87f);
    
    AnalyticX::flurryLogEvent("Menu Option: Show Intro");
    
    tutorial->setPosition(ccp(-42,-34));
    
    CCSprite* aFrame = CCSprite::create("Interfeiss/stats/bigpop.png");
    aFrame->setPosition(ccp(visibleSize.width/2,visibleSize.height/2-30));
    //Add to this popup the intor + scale
    aFrame->addChild(tutorial);
    aFrame->setTag(1001);
    
    //---------------------------------
    //Add the header !!!
    
    //Add the clouse button
    CCMenuItemImage* clouseButton = CCMenuItemImage::create(
                                                            "Interfeiss/buttons/x_btn0001.png",
                                                            "Interfeiss/buttons/x_btn0002.png",
                                                            this,
                                                            menu_selector(OptionsScene::OnExitTutorial));
    
    CCMenu* aClouse = CCMenu::create(clouseButton,NULL);
    aClouse->setPosition(ccp(tutorial->getContentSize().width+10,tutorial->getContentSize().height));
    tutorial->addChild(aClouse,1);
    
    //the header
    CCSprite* aHeaderBase = CCSprite::create("Interfeiss/challenges/header_title.png");
    aHeaderBase->setPosition(ccp(tutorial->getContentSize().width/2,tutorial->getContentSize().height+42));
    tutorial->addChild(aHeaderBase);
    
    //Add the decor icons
    CCSprite* aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(88,aHeaderBase->getContentSize().height/2));
    aDecor->setColor(ccc3(255,0,0));
    aHeaderBase->addChild(aDecor);
    
    aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(268,aHeaderBase->getContentSize().height/2));
    aDecor->setFlipX(true);
    aDecor->setColor(ccc3(255,0,0));
    aHeaderBase->addChild(aDecor);
    
    CCLabelTTF* aStoreHeader = CCLabelTTF::create("Intro", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8, CCSize(200, aHeaderBase->getContentSize().height), kCCTextAlignmentCenter);
    aStoreHeader->setPosition(ccp(178,aHeaderBase->getContentSize().height/2-4));
    aStoreHeader->setColor(ccc3(89, 19, 12));
    aHeaderBase->addChild(aStoreHeader);
    
    
    //---------------------------------
    
    addChild(aFrame,100);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void OptionsScene::OnExitTutorial()
{
    CCSprite* aSprite = (CCSprite*)getChildByTag(1001);
    removeChild(aSprite);
    
    AnalyticX::flurryLogEvent("Menu Option: Hide Tutorial");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void OptionsScene::menuTutorialCallback(cocos2d::CCObject *sender)
{
    TutorialScene* tutorial = TutorialScene::create(false);
//    tutorial->setScale(0);
//    CCActionInterval* actionTo = CCScaleTo::create(0.5, 1);
//    CCAction* action = CCEaseOut::create(actionTo, 3);
//    tutorial->runAction(action);
//
//    addChild(tutorial, 10);
    
    AnalyticX::flurryLogEvent("Menu Option: Show Tutorial");
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    tutorial->setScale(0.87f);
    tutorial->setPosition(ccp(-42,-34));
    
    CCSprite* aFrame = CCSprite::create("Interfeiss/stats/bigpop.png");
    aFrame->setPosition(ccp(visibleSize.width/2,visibleSize.height/2-30));
    aFrame->setTag(1001);
    aFrame->addChild(tutorial);
    
    //---------------------------------
    //Add the header !!!
    
    //Add the clouse button
    CCMenuItemImage* clouseButton = CCMenuItemImage::create(
                                                            "Interfeiss/buttons/x_btn0001.png",
                                                            "Interfeiss/buttons/x_btn0002.png",
                                                            this,
                                                            menu_selector(OptionsScene::OnExitTutorial));
    
    CCMenu* aClouse = CCMenu::create(clouseButton,NULL);
    aClouse->setPosition(ccp(tutorial->getContentSize().width+10,tutorial->getContentSize().height));
    tutorial->addChild(aClouse,1);
    
    //the header
    CCSprite* aHeaderBase = CCSprite::create("Interfeiss/challenges/header_title.png");
    aHeaderBase->setPosition(ccp(tutorial->getContentSize().width/2,tutorial->getContentSize().height+42));
    tutorial->addChild(aHeaderBase);
    
    //Add the decor icons
    CCSprite* aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(78,aHeaderBase->getContentSize().height/2));
    aDecor->setColor(ccc3(255,0,0));
    aHeaderBase->addChild(aDecor);
    
    aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(278,aHeaderBase->getContentSize().height/2));
    aDecor->setFlipX(true);
    aDecor->setColor(ccc3(255,0,0));
    aHeaderBase->addChild(aDecor);
    
    CCLabelTTF* aStoreHeader = CCLabelTTF::create("Tutorial", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8, CCSize(200, aHeaderBase->getContentSize().height), kCCTextAlignmentCenter);
    aStoreHeader->setPosition(ccp(178,aHeaderBase->getContentSize().height/2-4));
    aStoreHeader->setColor(ccc3(89, 19, 12));
    aHeaderBase->addChild(aStoreHeader);
    
    
    //---------------------------------
    
    addChild(aFrame,100);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void OptionsScene::menuSingOutClick()
{
    //Hide this button now
    aBase->removeChildByTag(2222);
    JniCalls::signPlayerOut();
    
    //Set that user is not singed in too goolge !!!
    MainMenuScene* mainMenu = (MainMenuScene*)this->getParent();
    mainMenu->OnRecreateSingIn();
}

void OptionsScene::menuSoundCallback(cocos2d::CCObject *sender)
{
    int enabled = dynamic_cast<cocos2d::CCMenuItemToggle*>(sender)->getSelectedIndex();
    if (enabled)
    {
        AnalyticX::flurryLogEvent("Menu Option: Disable Sound FX");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("soundsEnabled", true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
    }
    else
    {
        AnalyticX::flurryLogEvent("Menu Option: Enable Sound FX");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("soundsEnabled", false);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void OptionsScene::menuMusicCallback(cocos2d::CCObject *sender)
{
    int enabled = dynamic_cast<cocos2d::CCMenuItemToggle*>(sender)->getSelectedIndex();
    if (enabled)
    {
        AnalyticX::flurryLogEvent("Menu Option: Disable Music");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("musicEnabled", true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
    else
    {
        AnalyticX::flurryLogEvent("Menu Option: Enable Music");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("musicEnabled", false);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void OptionsScene::menuVibrationCallback(cocos2d::CCObject *sender)
{
    int enabled = dynamic_cast<cocos2d::CCMenuItemToggle*>(sender)->getSelectedIndex();
    if (enabled)
    {
        AnalyticX::flurryLogEvent("Menu Option: Disable Vibro");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("vibrationEnabled", true);
    }
    else
    {
        AnalyticX::flurryLogEvent("Menu Option: Enable Vibro");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("vibrationEnabled", false);
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}
