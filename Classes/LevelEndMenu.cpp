//
//  Menu.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 10/2/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "LevelEndMenu.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "UpgradeScene.h"
#include <SimpleAudioEngine.h>
#include "User.h"
#include "AppMacros.h"
#include "StoreScene.h"

#include "AnalyticX.h"

using namespace DF::LevelEnd;

USING_NS_CC;

bool LevelEndMenu::init()
{
    setTag(9889);
    
    CCMenuItemImage* playItem = CCMenuItemImage::create("Interfeiss/level_end/play00010001.png",
                                                        "Interfeiss/level_end/play00010002.png",
                                                        this,
                                                        menu_selector(LevelEndMenu::playCall));
//    playItem->setAnchorPoint(ccp(1, 0.5));
    playItem->setPosition(ccp(0,-14));
    
    
    CCMenuItemImage* menuItem = CCMenuItemImage::create("Interfeiss/level_end/menu00010001.png",
                                                        "Interfeiss/level_end/menu00010002.png",
                                                        this,
                                                        menu_selector(LevelEndMenu::menuCall));
//    menuItem->setAnchorPoint(ccp(1, 0.5));
    menuItem->setPosition(ccp(-180,-8));
    
    CCSprite * upgradesNormalSprite= CCSprite::create("Interfeiss/level_end/upgrade_btn0001.png");
    _upgradesPoint1 = UpgradesRedDot2::create();
    _upgradesPoint1->setPosition(ccp(195, 98));//235
    upgradesNormalSprite->addChild(_upgradesPoint1);
    
    CCSprite * upgradesSelectedSprite= CCSprite::create("Interfeiss/level_end/upgrade_btn0002.png");
    _upgradesPoint2 = UpgradesRedDot2::create();
    _upgradesPoint2->setPosition(ccp(200, 102));//240
    upgradesSelectedSprite->addChild(_upgradesPoint2);
    
	CCMenuItemSprite* upgradeItem = CCMenuItemSprite::create(
                                                              upgradesNormalSprite,
                                                              upgradesSelectedSprite,
                                                              this,
                                                              menu_selector(LevelEndMenu::upgradeCall));
    
    upgradeItem->setPosition(ccp(200,-8));
    
//    CCMenuItemImage* upgradeItem = CCMenuItemImage::create("Interfeiss/level_end/upgrade_btn0001.png",
//                                                        "Interfeiss/level_end/upgrade_btn0002.png",
//                                                        this,
//                                                        menu_selector(LevelEndMenu::upgradeCall));
//    upgradeItem->setAnchorPoint(ccp(1, 0.5));
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCMenu* menu = CCMenu::create(playItem,upgradeItem,menuItem, NULL);
//    menu->alignItemsVerticallyWithPadding(-30);
    menu->setPosition(ccp(0,-100));
    addChild(menu);
    
    return true;
}

void LevelEndMenu::upgradeCall()
{
    //Updated stuff
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
//    if(mOptionsOpen)
//        return;
//    mOptionsOpen = true;
    if(getChildByTag(2223))
        return;//Nope
    
    AnalyticX::flurryLogEvent("EndScreen: Upgrade");
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    StoreScene* storeLayer = StoreScene::create();
    storeLayer->setTag(2223);
//    storeLayer->setPosition(ccp(-visibleSize.width/2,-visibleSize.height/2));
    getParent()->addChild(storeLayer,100);
//    addChild(storeLayer,100);
    
//    CCScene* scene = UpgradeScene::scene(1);
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, scene);
//    CCTransitionScene* transition = CCTransitionFade::create(0.5f, scene);
//    CCDirector::sharedDirector()->replaceScene(transition);
}

void LevelEndMenu::OnRemoveUpgrade()
{
    getParent()->removeChildByTag(2223,true);
    
//    removeChildByTag(2223,true);
}

void LevelEndMenu::playCall()
{
    AnalyticX::flurryLogEvent("EndScreen: Play");
    
    CCScene* scene = GameScene::scene();
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, scene);
    CCTransitionScene* transition = CCTransitionFade::create(0.5f, scene);
    CCDirector::sharedDirector()->replaceScene(transition);
}

void LevelEndMenu::menuCall()
{
    AnalyticX::flurryLogEvent("EndScreen: MainMenu");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    CCScene* scene = MainMenuScene::scene();
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, scene);
    CCTransitionScene* transition = CCTransitionFade::create(0.5f, scene);
    CCDirector::sharedDirector()->replaceScene(transition);
}

bool UpgradesRedDot2::init()
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

void UpgradesRedDot2::update()
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
