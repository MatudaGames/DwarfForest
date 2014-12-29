#include "SaveMeScene.h"
#include "MainMenuScene.h"
#include "TutorialScene.h"
#include <SimpleAudioEngine.h>
#include "SpriteAnimation.h"
#include "GameScene.h"
#include "User.h"
#include "Utils.h"

USING_NS_CC;

CCScene* SaveMeScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SaveMeScene *layer = SaveMeScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SaveMeScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
//    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
//    back->setAnchorPoint(ccp(0,0));
//    addChild(back, 0);
    
    CCLayerColor* back = CCLayerColor::create(ccc4(0,0,0,60), visibleSize.width, visibleSize.height);
    addChild(back);
    
    CCSprite* logo = CCSprite::create("Interfeiss/save_me/saveme_logs.png");
    logo->setPositionX(visibleSize.width/2);
    logo->setPositionY(visibleSize.height/2);
    addChild(logo);
    
    //Add the 2 buttons
    CCMenuItemImage *skipButton = CCMenuItemImage::create(
                                                         "Interfeiss/save_me/skip_btn0001.png",
                                                         "Interfeiss/save_me/skip_btn0002.png",
                                                         this,
                                                         menu_selector(SaveMeScene::menuSkipCallback));
    
    CCMenuItemImage *saveButton = CCMenuItemImage::create(
                                                          "Interfeiss/save_me/TheCrystalButtonHolder.png",
                                                          "Interfeiss/save_me/TheCrystalButtonHolder.png",
                                                          this,
                                                          menu_selector(SaveMeScene::menuSaveCallback));
    
    CCMenu* mainMenu = CCMenu::create(saveButton, skipButton, NULL);
    mainMenu->alignItemsHorizontally();
    mainMenu->setPositionY(240);
    this->addChild(mainMenu, 1);
    
    skipButton->setPosition(100, 0);
    saveButton->setPosition(-110, 40);
    
//    //The animations
//    SpriteAnimation* animationDimant = SpriteAnimation::create("Interfeiss/save_me/diamond_btn.plist");
//    animationDimant->setPosition(ccp(visibleSize.width/2-120, visibleSize.height/2-80));
//	animationDimant->retain();
//	addChild(animationDimant);
    
//    //Add the dimand need amount to the button !!!
//    GameScene* gameScene = dynamic_cast<GameScene*>(this->getParent());
//    int aPriceDimand = gameScene->mLastSavePrice*2;
//    
//    //Show the amount in the button with animation !!!
//    //Split the number in 2 parts if needed
//    
//    char amountString[32];
//    sprintf(amountString, "%i", aPriceDimand);
//    
//    char amount1_String[32];
//    sprintf(amount1_String, "Interfeiss/save_me/%i.png", amountString[0]);
//    
//    CCSprite* aNum1 = CCSprite::create(amount1_String);
    
    
//    SpriteAnimation* animationTimer = SpriteAnimation::create("Interfeiss/save_me/saveme_timer.plist");
//    animationTimer->setPosition(ccp(visibleSize.width/2+80, visibleSize.height/2-10));
//	animationTimer->retain();
//	addChild(animationTimer);
//    
//    schedule(schedule_selector(SaveMeScene::menuSkipCallback), 3.2f, 1, 0.0f);//A bit later !!! [0.7f, 1, 0.0f]
    
    /*
    CCSprite* labels = CCSprite::create("Interfeiss/pause_menu/pause_text.png");
    labels->setAnchorPoint(ccp(0.5, 1));
    labels->setPositionX(visibleSize.width / 2);
    labels->setPositionY(visibleSize.height - 50);
    addChild(labels);
    
    CCMenuItemImage* s1 = CCMenuItemImage::create("Interfeiss/checkbox_empty.png",
                                                  "Interfeiss/checkbox_checked.png", this, NULL);
    
    CCMenuItemImage* s2 = CCMenuItemImage::create("Interfeiss/checkbox_checked.png",
                                                  "Interfeiss/checkbox_empty.png", this, NULL);
    
    CCMenuItemToggle* soundButton = CCMenuItemToggle::createWithTarget(
                                                                       this, menu_selector(SaveMeScene::menuSoundCallback), s1, s2, NULL);
    
    
    CCMenuItemImage* m1 = CCMenuItemImage::create("Interfeiss/checkbox_empty.png",
                                                  "Interfeiss/checkbox_checked.png", this, NULL);
    
    CCMenuItemImage* m2 = CCMenuItemImage::create("Interfeiss/checkbox_checked.png",
                                                  "Interfeiss/checkbox_empty.png", this, NULL);
    
    CCMenuItemToggle* musicButton = CCMenuItemToggle::createWithTarget(
                                                                       this, menu_selector(SaveMeScene::menuMusicCallback), m1, m2, NULL);
    soundButton->setPosition(85, 185);
    musicButton->setPosition(85, 95);
    
    bool musicEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true);
    bool soundsEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("soundsEnabled", true);
    
    musicButton->setSelectedIndex(musicEnabled);
    soundButton->setSelectedIndex(soundsEnabled);
    
    CCMenu* musicMenu = CCMenu::create(soundButton, musicButton, NULL);
    this->addChild(musicMenu, 1);
    
    
    CCMenuItemImage* resumeButton = CCMenuItemImage::create(
                                                              "Interfeiss/pause_menu/resume_game_btn.png",
                                                              "Interfeiss/pause_menu/resume_game_btn_pressed.png",
                                                              this,
                                                              menu_selector(SaveMeScene::menuResumeCallback));
    CCMenuItemImage* mainMenuButton = CCMenuItemImage::create(
                                                             "Interfeiss/pause_menu/main_menu_btn.png",
                                                             "Interfeiss/pause_menu/main_menu_btn_pressed.png",
                                                             this,
                                                             menu_selector(SaveMeScene::menuMainMenuCallback));
    CCMenuItemImage* tutorialButton = CCMenuItemImage::create(
                                                             "Interfeiss/options/tutorial_btn.png",
                                                             "Interfeiss/options/tutorial_btn_pressed.png",
                                                             this,
                                                             menu_selector(SaveMeScene::menuTutorialCallback));
    
    CCMenu* mainMenu = CCMenu::create(resumeButton, mainMenuButton, tutorialButton, NULL);
    mainMenu->alignItemsVerticallyWithPadding(-40);
    mainMenu->setPositionY(240);
    this->addChild(mainMenu, 1);
    */

    return true;
}

void SaveMeScene::onCreatePrice()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //The animations
    SpriteAnimation* animationDimant = SpriteAnimation::create("Interfeiss/save_me/diamond_btn.plist");
    animationDimant->setPosition(ccp(visibleSize.width/2-120, visibleSize.height/2-80));
	animationDimant->retain();
	addChild(animationDimant);
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //Add the dimand need amount to the button !!!
    GameScene* gameScene = dynamic_cast<GameScene*>(this->getParent());
    int aPriceDimand = gameScene->getLastSavePrice() * 2;
//    aPriceDimand = 2;
    
    //Show the amount in the button with animation !!!
    //Split the number in 2 parts if needed
    
//    aPriceDimand = 444;
    
    char amountString[32];
    sprintf(amountString, "%i", aPriceDimand);
    
    char amount1_String[32];
    sprintf(amount1_String, "Interfeiss/save_me/%c.png", amountString[0]);
    
    CCSprite* aNum1 = NULL;
    CCSprite* aNum2 = NULL;
    CCSprite* aNum3 = NULL;
    CCSprite* aMulti = NULL;
    
    aMulti = CCSprite::create("Interfeiss/save_me/x.png");
    animationDimant->addChild(aMulti);
    
    aNum1 = CCSprite::create(amount1_String);
    aNum1->setAnchorPoint(ccp(0,0));
    aMulti->addChild(aNum1);
    
    aMulti->setPosition(ccp(0,-4));
    aNum1->setPosition(ccp(aMulti->getContentSize().width,0));
    
    aMulti->setRotation(10);
    aMulti->cocos2d::CCNode::setPosition(ccp(190,90));
    
    if (aPriceDimand>9)
    {
        sprintf(amount1_String, "Interfeiss/save_me/%c.png", amountString[1]);
        aNum2 = CCSprite::create(amount1_String);
        aNum2->setAnchorPoint(ccp(0,0));
        aNum1->addChild(aNum2);
        
        if (aPriceDimand>99)
        {
            aNum2->setPosition(ccp(aNum1->getContentSize().width,0));
            
            sprintf(amount1_String, "Interfeiss/save_me/%c.png", amountString[2]);
            aNum3 = CCSprite::create(amount1_String);
            aNum3->setAnchorPoint(ccp(0,0));
            aNum1->addChild(aNum3);
            
            aMulti->setPosition(ccp(140,40));
            aNum1->setPosition(ccp(aMulti->getContentSize().width+5,0));
            aNum2->setPosition(ccp(aNum1->getContentSize().width+5,0));
            aNum3->setPosition(ccp(aNum2->getPosition().x+aNum3->getContentSize().width,0));
        }
        else
        {
           aNum2->setPosition(ccp(aNum1->getContentSize().width,0));
        }
    }
    
    //Lets set the animation
//    aMulti->setRotation(10);
//    aMulti->cocos2d::CCNode::setPosition(ccp(190,90));
    
    CCScaleTo* anim_scale1 = CCScaleTo::create(0.1f, 1.0f);
    CCScaleTo* anim_scale2 = CCScaleTo::create(0.1f, 1.2f);
    CCDelayTime* anim_delay = CCDelayTime::create(0.1f);
    CCSequence* anim_seq = CCSequence::create(anim_scale1,anim_scale2,anim_delay,NULL);
    aMulti->runAction(CCRepeatForever::create(anim_seq));
    
    ////////////////////////////////////////////////////////////////////////////////////////
    SpriteAnimation* animationTimer = SpriteAnimation::create("Interfeiss/save_me/saveme_timer.plist");
    animationTimer->setPosition(ccp(visibleSize.width/2+80, visibleSize.height/2-10));
	animationTimer->retain();
	addChild(animationTimer);
    
    schedule(schedule_selector(SaveMeScene::menuSkipCallback), 3.2f, 1, 0.0f);//A bit later !!! [0.7f, 1, 0.0f]//For now disabled
    
    gameScene->playInGameSound("save_me_wait");
}

void SaveMeScene::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
    
    //Create now the price dynamic stuff !!!
    onCreatePrice();
}

void SaveMeScene::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool SaveMeScene::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    return true;
}

void SaveMeScene::menuSkipCallback(CCObject* sender)
{
    unschedule(schedule_selector(SaveMeScene::menuSkipCallback));
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
    GameScene* gameScene = dynamic_cast<GameScene*>(this->getParent());
    gameScene->lose(true);
    
    this->getParent()->removeChild(this);
}

void SaveMeScene::menuSaveCallback(CCObject* sender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
    //Check if we can save it !!!
    //Check if has money - if not - ask to buy? how will this work in game?
    GameScene* gameScene = dynamic_cast<GameScene*>(this->getParent());
    int aPriceDimand = gameScene->getLastSavePrice() * 2;
    
    int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPriceDimand);//Whats the price for save mee!!!
    if (aDidUseDiamonds<0)
    {
        //Show popup that no money
        
        return;
    }
    
    gameScene->stopInGameSound("save_me_wait");
    
    //Update to the new price !!!
    gameScene->setLastSavePrice(aPriceDimand);
    gameScene->onSaveFromPause();
    
    //Update booster buttons
    gameScene->checkBoosterAvailability();
    
    this->getParent()->removeChild(this);
}

void SaveMeScene::menuResumeCallback(CCObject* sender)
{
	this->getParent()->removeChild(this);
}

void SaveMeScene::menuMainMenuCallback(cocos2d::CCObject *sender)
{
    GameScene* gameScene = dynamic_cast<GameScene*>(this->getParent());
    gameScene->stopInGameSound("save_me_wait");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    CCScene* tutorial = MainMenuScene::scene();
	CCTransitionScene* transition = CCTransitionProgressInOut::create(0.5f, tutorial);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void SaveMeScene::menuTutorialCallback(cocos2d::CCObject *sender)
{
//    CCScene* tutorial = TutorialScene::scene();
//	CCTransitionScene* transition = CCTransitionProgressInOut::create(0.5f, tutorial);
//	CCDirector::sharedDirector()->pushScene(transition);
    
    TutorialScene* tutorial = TutorialScene::create();
    tutorial->setScale(0);
    CCActionInterval* actionTo = CCScaleTo::create(0.5, 1);
    CCAction* action = CCEaseOut::create(actionTo, 3);
    tutorial->runAction(action);
    
    addChild(tutorial, 10);
    
}

void SaveMeScene::menuSoundCallback(cocos2d::CCObject *sender)
{
    int enabled = dynamic_cast<cocos2d::CCMenuItemToggle*>(sender)->getSelectedIndex();
    if (enabled)
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("soundsEnabled", true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
    }
    else
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("soundsEnabled", false);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
    }
}

void SaveMeScene::menuMusicCallback(cocos2d::CCObject *sender)
{
    int enabled = dynamic_cast<cocos2d::CCMenuItemToggle*>(sender)->getSelectedIndex();
    if (enabled)
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("musicEnabled", true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
    else
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("musicEnabled", false);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
}

