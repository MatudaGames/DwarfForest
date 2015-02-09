//
//  StoreScene.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 7/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "StoreScene.h"
#include "MainMenuScene.h"
#include <SimpleAudioEngine.h>

//#include "FlurryX.h"

#include "StoreBox.h"
#include "AppMacros.h"

#include "User.h"
#include "LevelEndMenu.h"

#include "AnalyticX.h"
#include "WorldMap.h"

USING_NS_CC;
//using namespace CocosDenshion;

CCScene* StoreScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StoreScene *layer = StoreScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void StoreScene::RemovePopup()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Tap_Back").c_str());
    
    CCLayerColor* aLayer = (CCLayerColor*)getChildByTag(888);
    CCFadeTo* aFade = CCFadeTo::create(0.25f,0);
    aLayer->runAction(aFade);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(visibleSize.width/2,visibleSize.height+200));
    CCEaseElasticIn* aEase1 = CCEaseElasticIn::create(aMove1,1.0f);
    CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(StoreScene::RealRemove));
    CCSequence* aSeq = CCSequence::create(aEase1,func2,NULL);
    aBaseBase->runAction(aSeq);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Popup_Exit").c_str());
    
    AnalyticX::flurryLogEvent("Menu Clouse: Store");
}

void StoreScene::RealRemove()
{
    //Check if this is level end or main menu !!!
    
    WorldMap* map = (WorldMap*)this->getParent();
    map->OnRemoveStore();
    
    return;
    
//    MainMenuScene* mainMenu = (MainMenuScene*)this->getParent();
//    mainMenu->mOptionsOpen = false;
//    this->getParent()->removeChild(this);
    if(getParent()->getTag()==9889)
    {
        DF::LevelEnd::LevelEndMenu* levelMenu = (DF::LevelEnd::LevelEndMenu*)getParent();
        levelMenu->OnRemoveUpgrade();
    }
    else
    {
        MainMenuScene* mainMenu = (MainMenuScene*)getParent();
        mainMenu->OnRemoveStore();
    }
}

// on "init" you need to initialize your instance
bool StoreScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    mLevelEndCase = false;
    
    //Play the slide in sound
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Popup_Enter").c_str());
    
    //navigation
    
//    FlurryX::logEvent("Shop Opened");
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCLayerColor* aBG = CCLayerColor::create(ccc4(0,0,0,64),visibleSize.width,visibleSize.height);
    aBG->setOpacity(0);
    aBG->setTag(888);
    addChild(aBG,0);
    
    CCFadeTo* aFade1 = CCFadeTo::create(0.25f,64);
    aBG->runAction(aFade1);
    
    aBaseBase = CCSprite::create("small_dot_red.png");
    aBaseBase->setOpacity(0);
    aBaseBase->setPosition(ccp(visibleSize.width/2,visibleSize.height+200));
    addChild(aBaseBase);
    
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(visibleSize.width/2,visibleSize.height/2));
    CCEaseElasticOut* aEase1 = CCEaseElasticOut::create(aMove1,1.0f);
    aBaseBase->runAction(aEase1);
    
    aBase = CCSprite::create("Interfeiss/store/FRESH/mainboard.png");
    aBase->setPosition(ccp(aBaseBase->getContentSize().width/2+50,aBaseBase->getContentSize().height/2-35));
    aBaseBase->addChild(aBase);
    
    //Add the clouse button
    CCMenuItemImage* clouseButton = CCMenuItemImage::create(
                                                            "Interfeiss/buttons/x_btn0001.png",
                                                            "Interfeiss/buttons/x_btn0002.png",
                                                            this,
                                                            menu_selector(StoreScene::RemovePopup));
    CCMenu* aClouse = CCMenu::create(clouseButton,NULL);
    aClouse->setPosition(ccp(aBase->getContentSize().width-20,aBase->getContentSize().height-20));
    aBase->addChild(aClouse,1);
    
    //the header
    CCSprite* aHeaderBase = CCSprite::create("Interfeiss/store/FRESH/header_store.png");
    aHeaderBase->setPosition(ccp(aBase->getContentSize().width/2-20,aBase->getContentSize().height+33));
    aBase->addChild(aHeaderBase);
    
    //Add the decor icons
    CCSprite* aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(50,aHeaderBase->getContentSize().height/2));
    aDecor->setColor(ccc3(255,0,0));
    aHeaderBase->addChild(aDecor);
    
    aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(190,aHeaderBase->getContentSize().height/2));
    aDecor->setFlipX(true);
    aDecor->setColor(ccc3(255,0,0));
    aHeaderBase->addChild(aDecor);
    
    CCLabelTTF* aStoreHeader = CCLabelTTF::create("Store", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8, CCSize(100, aHeaderBase->getContentSize().height), kCCTextAlignmentCenter);
    aStoreHeader->setPosition(ccp(120,aHeaderBase->getContentSize().height/2-4));
    aStoreHeader->setColor(ccc3(89, 19, 12));
    aHeaderBase->addChild(aStoreHeader);
    
    //You have
    aStoreHeader = CCLabelTTF::create("You have:", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.6, CCSize(240, aHeaderBase->getContentSize().height),
                                      kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setPosition(ccp(340,aHeaderBase->getContentSize().height/2-2));
    aStoreHeader->setColor(ccc3(85, 72, 53));
    aHeaderBase->addChild(aStoreHeader);
    
    //How much do we have it !!!
    _diamondsLabel = CCLabelTTF::create("30000", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(240, aHeaderBase->getContentSize().height),
                                      kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
    _diamondsLabel->setPosition(ccp(380,aHeaderBase->getContentSize().height/2-2));
    _diamondsLabel->setColor(ccc3(255,255,255));
    _diamondsLabel->enableShadow(CCSize(1,-1),2.0f,true);
    aHeaderBase->addChild(_diamondsLabel);
    
    _scoreLabel = CCLabelTTF::create("25000", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(240, aHeaderBase->getContentSize().height),
                                      kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
    _scoreLabel->setPosition(ccp(540,aHeaderBase->getContentSize().height/2-2));
    _scoreLabel->setColor(ccc3(255,255,255));
    _scoreLabel->enableShadow(CCSize(1,-1),2.0f,true);
    aHeaderBase->addChild(_scoreLabel);
    
    //The icons
    CCSprite* aElectroCrystal = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
    aElectroCrystal->setPosition(ccp(680,aHeaderBase->getContentSize().height/2));//This is the upgrade pos
    aElectroCrystal->setScale(0.8f);
    aElectroCrystal->setTag(350);
    aHeaderBase->addChild(aElectroCrystal,10);
    
    CCSprite* aElectroDiamond = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
    aElectroDiamond->setScale(0.8f);
    aElectroDiamond->setPosition(ccp(524,aHeaderBase->getContentSize().height/2));//This is the upgrade pos
    aElectroDiamond->setTag(351);
    aHeaderBase->addChild(aElectroDiamond,10);
    
    //Add the side tabs
    //--------------------------------------------------------
    // The upgrades
    
    mTabUpgradesOff = CCSprite::create("Interfeiss/store/FRESH/tab_side_inactive.png");
    mTabUpgradesOff->setPosition(ccp(-mTabUpgradesOff->getContentSize().width/2+19,aBase->getContentSize().height-mTabUpgradesOff->getContentSize().height/2));
    aBase->addChild(mTabUpgradesOff,-1);
    
    aStoreHeader = CCLabelTTF::create("Upgrades", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(195, 75),
                                      kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setRotation(-90);
    aStoreHeader->setPosition(ccp(mTabUpgradesOff->getContentSize().width/2,mTabUpgradesOff->getContentSize().height/2));
    aStoreHeader->setColor(ccc3(129, 94, 44));
    mTabUpgradesOff->addChild(aStoreHeader);
    
    mTabUpgradesOn = CCSprite::create("Interfeiss/store/FRESH/tab_side_active.png");
    mTabUpgradesOn->setPosition(ccp(-mTabUpgradesOn->getContentSize().width/2+19,aBase->getContentSize().height-mTabUpgradesOn->getContentSize().height/2));
    aBase->addChild(mTabUpgradesOn,1);
    
    aStoreHeader = CCLabelTTF::create("Upgrades", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(195, 75),
                                      kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setRotation(-90);
    aStoreHeader->setPosition(ccp(mTabUpgradesOn->getContentSize().width/2,mTabUpgradesOn->getContentSize().height/2));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    mTabUpgradesOn->addChild(aStoreHeader);
    
    mTabUpgradesOff->setVisible(false);
    mTabUpgradesOn->setVisible(false);
    
    //--------------------------------------------------------
    //The shop
    
    mTabShopOff = CCSprite::create("Interfeiss/store/FRESH/tab_side_inactive.png");
    mTabShopOff->setPosition(ccp(-mTabShopOff->getContentSize().width/2+19,aBase->getContentSize().height/2));
    aBase->addChild(mTabShopOff,-1);
    
    aStoreHeader = CCLabelTTF::create("Shop", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(195, 75),
                                      kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setRotation(-90);
    aStoreHeader->setPosition(ccp(mTabShopOff->getContentSize().width/2,mTabShopOff->getContentSize().height/2));
    aStoreHeader->setColor(ccc3(129, 94, 44));
    mTabShopOff->addChild(aStoreHeader);
    
    mTabShopOn = CCSprite::create("Interfeiss/store/FRESH/tab_side_active.png");
    mTabShopOn->setPosition(ccp(-mTabShopOn->getContentSize().width/2+19,aBase->getContentSize().height/2));
    aBase->addChild(mTabShopOn,1);
    
    aStoreHeader = CCLabelTTF::create("Shop", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(195, 75),
                                      kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setRotation(-90);
    aStoreHeader->setPosition(ccp(mTabShopOn->getContentSize().width/2,mTabShopOn->getContentSize().height/2));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    mTabShopOn->addChild(aStoreHeader);
    
    mTabShopOff->setVisible(false);
    mTabShopOn->setVisible(false);
    
    //--------------------------------------------------------
    //The free stuff
    
    mTabFreeOff = CCSprite::create("Interfeiss/store/FRESH/tab_side_inactive.png");
    mTabFreeOff->setPosition(ccp(-mTabFreeOff->getContentSize().width/2+19,mTabFreeOff->getContentSize().height/2));
    aBase->addChild(mTabFreeOff,-1);
    
    //Add some more stuff
    aStoreHeader = CCLabelTTF::create("Free stuff", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(195, 75),
                                      kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setRotation(-90);
    aStoreHeader->setPosition(ccp(mTabFreeOff->getContentSize().width/2,mTabFreeOff->getContentSize().height/2));
    aStoreHeader->setColor(ccc3(129, 94, 44));
    mTabFreeOff->addChild(aStoreHeader);
    
    
    mTabFreeOn = CCSprite::create("Interfeiss/store/FRESH/tab_side_active.png");
    mTabFreeOn->setPosition(ccp(-mTabFreeOn->getContentSize().width/2+19,mTabFreeOn->getContentSize().height/2));
    aBase->addChild(mTabFreeOn,1);
    
    aStoreHeader = CCLabelTTF::create("Free stuff", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(195, 75),
                                      kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setRotation(-90);
    aStoreHeader->setPosition(ccp(mTabFreeOn->getContentSize().width/2,mTabFreeOn->getContentSize().height/2));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    mTabFreeOn->addChild(aStoreHeader);
    
    mTabFreeOff->setVisible(false);
    mTabFreeOn->setVisible(false);
    
    //Now create the click areas !!!
    CCMenuItemImage* tab_1 = CCMenuItemImage::create(
                                                            "Interfeiss/store/FRESH/tab_side_active.png",
                                                            "Interfeiss/store/FRESH/tab_side_active.png",
                                                            this,
                                                            menu_selector(StoreScene::OnTab1));
    CCMenuItemImage* tab_2 = CCMenuItemImage::create(
                                                     "Interfeiss/store/FRESH/tab_side_active.png",
                                                     "Interfeiss/store/FRESH/tab_side_active.png",
                                                     this,
                                                     menu_selector(StoreScene::OnTab2));
    CCMenuItemImage* tab_3 = CCMenuItemImage::create(
                                                     "Interfeiss/store/FRESH/tab_side_active.png",
                                                     "Interfeiss/store/FRESH/tab_side_active.png",
                                                     this,
                                                     menu_selector(StoreScene::OnTab3));
    
    
    
    CCMenu* aTabs = CCMenu::create(tab_1,tab_2,tab_3,NULL);
    aTabs->alignItemsVerticallyWithPadding(-14);
    aTabs->setOpacity(0);
    aTabs->setPosition(ccp(-mTabFreeOn->getContentSize().width/2+20,aBase->getContentSize().height/2));
    aBase->addChild(aTabs,2);
    
//    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
//    back->setAnchorPoint(ccp(0,0));
//    addChild(back, 0);
//    
//    CCMenuItemImage* backButton = CCMenuItemImage::create(
//														  "Interfeiss/backbtn0001.png",
//														  "Interfeiss/backbtn0002.png",
//														  this,
//														  menu_selector(StoreScene::menuBackCallback));
//	backButton->setAnchorPoint(ccp(0,0));
//    
//    CCMenu* backMenu = CCMenu::create(backButton, NULL);
//    this->addChild(backMenu, 1);
//    backMenu->setPosition(10, 10);
//    
//    
//    CCSprite* title = CCSprite::create("Interfeiss/store/store_title.png");
//    title->setAnchorPoint(ccp(0,1));
//    title->setPosition(ccp(70, visibleSize.height - 50));
//    addChild(title);
//    
//    
//    CCMenuItemImage* noAddsBt = CCMenuItemImage::create(
//                                                        "Interfeiss/store/no_ads_btn.png",
//                                                        "Interfeiss/store/no_ads_btn_pressed.png",
//                                                        this,
//                                                        menu_selector(StoreScene::btNoAddsCallback));
//    CCMenu* noAddsMenu = CCMenu::create(noAddsBt, NULL);
//    noAddsMenu->setPosition(ccp(110, 320));
//    this->addChild(noAddsMenu, 1);
//    
//    CCMenuItemImage* diamonds10Bt = CCMenuItemImage::create(
//                                                        "Interfeiss/store/10diamonds_btn.png",
//                                                        "Interfeiss/store/10diamonds_btn_pressed.png",
//                                                        this,
//                                                        menu_selector(StoreScene::bt10Callback));
//    CCMenu* diamonds10Menu = CCMenu::create(diamonds10Bt, NULL);
//    diamonds10Menu->setPosition(320, 400);
//    this->addChild(diamonds10Menu, 1);
//    
//    
//    
//    CCMenuItemImage* diamonds300Bt = CCMenuItemImage::create(
//                                                        "Interfeiss/store/300diamonds_btn.png",
//                                                        "Interfeiss/store/300diamonds_btn_pressed.png",
//                                                        this,
//                                                        menu_selector(StoreScene::bt300Callback));
//    CCMenu* diamonds300Menu = CCMenu::create(diamonds300Bt, NULL);
//    this->addChild(diamonds300Menu, 1);
//    diamonds300Menu->setPosition(780, 480);
//    
//    CCMenuItemImage* diamonds100Bt = CCMenuItemImage::create(
//                                                             "Interfeiss/store/100diamonds_btn.png",
//                                                             "Interfeiss/store/100diamonds_btn_pressed.png",
//                                                             this,
//                                                             menu_selector(StoreScene::bt100Callback));
//    CCMenu* diamonds100Menu = CCMenu::create(diamonds100Bt, NULL);
//    this->addChild(diamonds100Menu, 2);
//    diamonds100Menu->setPosition(530, 280);
    
    float aE3 = CCEGLView::sharedOpenGLView()->getScaleX();
    float aE4 = CCEGLView::sharedOpenGLView()->getScaleY();
    
//    mBox = StoreBox::create(CCRect((aBase->getPositionX()-aBase->getContentSize().width/2+20)*aE3,
//                                             (aBase->getPositionY()-aBase->getContentSize().height/2+20)*aE4,
//                                             (aBase->getContentSize().width-80)*aE3,(aBase->getContentSize().height-40)*aE4),
//                                      CCRect((aBase->getPositionX()-aBase->getContentSize().width/2+20),
//                                             (aBase->getPositionY()-aBase->getContentSize().height/2+20),
//                                             (aBase->getContentSize().width-80),(aBase->getContentSize().height-40)));
    mBox = StoreBox::create(CCRect((aBase->getPositionX()+90)*aE3,
                                   (aBase->getPositionY()+50)*aE4,
                                   (aBase->getContentSize().width-80)*aE3,(aBase->getContentSize().height-40)*aE4),
                            CCRect((aBase->getPositionX()-aBase->getContentSize().width/2+20),
                                   (aBase->getPositionY()-aBase->getContentSize().height/2+20),
                                   (aBase->getContentSize().width-80),(aBase->getContentSize().height-40)));
    aBaseBase->addChild(mBox);
    
    //For now !!!
//    SetActiveTab(1);
//    OnTab1();
    
    mTabFreeOff->setVisible(true);
    mTabFreeOn->setVisible(false);
    
    mTabUpgradesOff->setVisible(false);
    mTabUpgradesOn->setVisible(true);
    
    mTabShopOff->setVisible(true);
    mTabShopOn->setVisible(false);
    
    SetActiveTab(1);
    
    //---
    
    UpdateStats();
    
    return true;
}

void StoreScene::UpdateStats()
{
    std::stringstream aValue;
    int aCrystals = User::getInstance()->getCrystals();
    aValue<<aCrystals;
    _scoreLabel->setString(aValue.str().c_str());
    
    aValue.str("");
    aValue.clear();
    
    aCrystals = User::getInstance()->getDiamonds();
    aValue<<aCrystals;
    _diamondsLabel->setString(aValue.str().c_str());

}

void StoreScene::OnTab1()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("TabSwap").c_str());
    
    mTabFreeOff->setVisible(true);
    mTabFreeOn->setVisible(false);
    
    mTabUpgradesOff->setVisible(false);
    mTabUpgradesOn->setVisible(true);
    
    mTabShopOff->setVisible(true);
    mTabShopOn->setVisible(false);
    
    SetActiveTab(1);
    
//    FlurryX::logEvent("Store Upgrades");
    
    AnalyticX::flurryLogEvent("Menu Store: Tab Upgrades");
}

void StoreScene::OnTab2()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("TabSwap").c_str());
    
    mTabFreeOff->setVisible(true);
    mTabFreeOn->setVisible(false);
    
    mTabUpgradesOff->setVisible(true);
    mTabUpgradesOn->setVisible(false);
    
    mTabShopOff->setVisible(false);
    mTabShopOn->setVisible(true);
    
    SetActiveTab(2);
    
//    FlurryX::logEvent("Store Shop");
    
    AnalyticX::flurryLogEvent("Menu Store: Tab Shop");
}

void StoreScene::OnTab3()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("TabSwap").c_str());
    
    mTabUpgradesOff->setVisible(true);
    mTabUpgradesOn->setVisible(false);
    
    mTabFreeOff->setVisible(false);
    mTabFreeOn->setVisible(true);
    
    mTabShopOff->setVisible(true);
    mTabShopOn->setVisible(false);
    
    SetActiveTab(3);
    
//    FlurryX::logEvent("Store Free");
    
    AnalyticX::flurryLogEvent("Menu Store: Tab Free Stuff");
}

//Set the tab + update header !!!
void StoreScene::SetActiveTab(int theType)
{
    mActiveTab = theType;
    
//    mBox->add
    if(theType==1)
    {
        mBox->CreateUpgrades();
    }
    else if(theType==2)
    {
        mBox->CreateShop();
    }
    else if(theType==3)
    {
        mBox->CreateFreeStuff();
    }
}


uint32_t StoreScene::getSpecialOfferCount()
{
    //For now !!! disabled
    return 0;
    
    // TODO: should return real number here
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    
    return t->tm_wday == 6 ? 1 : 0;
}

void StoreScene::menuBackCallback(CCObject* sender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
	CCScene* mainMenu = MainMenuScene::scene();
	CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, mainMenu);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void StoreScene::btNoAddsCallback(cocos2d::CCObject *sender)
{
//    FlurryX::logEvent("Shop NO ADS");
}

void StoreScene::bt10Callback(cocos2d::CCObject *sender)
{
//    FlurryX::logEvent("Shop 10");
}

void StoreScene::bt100Callback(cocos2d::CCObject *sender)
{
//    FlurryX::logEvent("Shop 100");
}

void StoreScene::bt300Callback(cocos2d::CCObject *sender)
{
//    FlurryX::logEvent("Shop 300");
}
