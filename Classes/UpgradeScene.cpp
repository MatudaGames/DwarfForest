//
//  UpgradeScene.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 27/11/13.
//
//

#include "UpgradeScene.h"
#include "IntroScene.h"
#include "StaticSceneManager.h"
#include <SimpleAudioEngine.h>

#include "JniCalls.h"
#include "AppMacros.h"

#include "User.h"

USING_NS_CC;

CCScene* UpgradeScene::scene(int theReturnType)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    UpgradeScene *layer = UpgradeScene::create(theReturnType);
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

UpgradeScene* UpgradeScene::create(int theReturnType)
{
    UpgradeScene *pRet = new UpgradeScene();
    if (pRet && pRet->init(theReturnType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

// on "init" you need to initialize your instance
bool UpgradeScene::init(int theReturnType)
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    mReturnType = theReturnType;
    
    User::getInstance()->addDiamonds(100);
    User::getInstance()->addCrystals(10000);
	
    // enables back button on android
    this->setKeypadEnabled(true);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
    back->setAnchorPoint(ccp(0,0));
    addChild(back, 0);
    
//    CCSprite* pointsBack2 = CCSprite::create("Interfeiss/upgrades/unlocknow_btn0001.png");
    //The logo
    CCSprite* aLogo = CCSprite::create("Interfeiss/upgrade_screen/upgrades_title.png");
    aLogo->setPosition(ccp(aLogo->getContentSize().width/2+20,visibleSize.height-aLogo->getContentSize().height/2-15));
    addChild(aLogo);
    
    //The other parts!!!
    
    int aBlockW = visibleSize.width/2;
    int aBlockH = visibleSize.height/2;
    
    //Electro stuff
    CCSprite* aBlitBG = CCSprite::create("Interfeiss/upgrade_screen/infopanel.png");
    aBlitBG->setPosition(ccp(visibleSize.width/2-aBlockW/2+115,
                             visibleSize.height/2-aBlockH/2+45));
    aBlitBG->setTag(100);
    addChild(aBlitBG,10);
    
    //-----------------------------------------------
    //The side hud
    
    //------------------------------------------------------------------------
    //Game crystals score
    
    _scoreLabel = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(115, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
	_scoreLabel->setString("0");
    _scoreLabel->setColor(ccc3(255,246,200));
    
    _diamondsLabel = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(90, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
	_diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
    _diamondsLabel->setColor(ccc3(255,246,200));
    
    CCSprite* pointsBack = CCSprite::create("Interfeiss/in_game/crystal_count.png");
    pointsBack->setAnchorPoint(ccp(0.5, 1));
    pointsBack->cocos2d::CCNode::setPosition((visibleSize.width-pointsBack->getContentSize().width/2), visibleSize.height-10);
    addChild(pointsBack, 10);
    
    _scoreLabel->setAnchorPoint(ccp(0.5,0.5));
	_scoreLabel->setPosition(ccp(pointsBack->getContentSize().width/2-40,pointsBack->getContentSize().height/2+1));
	pointsBack->addChild(_scoreLabel);
    
    //Game diamonds score
    CCSprite* diamondsBack = CCSprite::create("Interfeiss/in_game/diamond_count.png");
    diamondsBack->setAnchorPoint(ccp(0.5, 1));
    diamondsBack->cocos2d::CCNode::setPosition((visibleSize.width-diamondsBack->getContentSize().width/2)+21, visibleSize.height-55);
    addChild(diamondsBack, 10);
    
    _diamondsLabel->setAnchorPoint(ccp(0.5,0.5));
	_diamondsLabel->setPosition(ccp(diamondsBack->getContentSize().width/2-45,diamondsBack->getContentSize().height/2+2));//27
	diamondsBack->addChild(_diamondsLabel);
    
    //-----------------------------------------------
    //The buttons !!!
    mPlantButtonState = 1;//Unlock state
    /*
    if(User::getInstance()->getMachine_PlantLvl()>0)
        mPlantButtonState = 2;
    
    //The unlock
    CCMenuItemImage* plantUnlock = CCMenuItemImage::create(
                                                           "Interfeiss/upgrade_screen/unlocknow_btn0001.png",
                                                           "Interfeiss/upgrade_screen/unlocknow_btn0002.png",
                                                           this,
                                                           menu_selector(UpgradeScene::UnlockNow_Plant));
    CCMenu* plantMenuUnlock = CCMenu::create(plantUnlock, NULL);
    plantMenuUnlock->setPosition(aBlitBG->getPositionX(),aBlitBG->getPositionY()-110);
    plantMenuUnlock->setVisible(false);
    plantMenuUnlock->setTag(111);
    addChild(plantMenuUnlock, 10);
    
    //The upgrade
    CCMenuItemImage* plantUpgrade = CCMenuItemImage::create(
                                                              "Interfeiss/upgrade_screen/upgrade_btn0001.png",
                                                              "Interfeiss/upgrade_screen/upgrade_btn0002.png",
                                                              this,
                                                              menu_selector(UpgradeScene::UnlockNow_Plant));
    CCMenu* plantMenuUpgrade = CCMenu::create(plantUpgrade, NULL);
    plantMenuUpgrade->setPosition(aBlitBG->getPositionX(),aBlitBG->getPositionY()-110);
    plantMenuUpgrade->setVisible(false);
    plantMenuUpgrade->setTag(112);
    addChild(plantMenuUpgrade, 10);
    
    //The buy
    CCMenuItemImage* plantBuy = CCMenuItemImage::create(
                                                          "Interfeiss/upgrade_screen/buy_btn0001.png",
                                                          "Interfeiss/upgrade_screen/buy_btn0002.png",
                                                          this,
                                                          menu_selector(UpgradeScene::Buy_Plant));
    CCMenu* plantMenuBuy = CCMenu::create(plantBuy, NULL);
    plantMenuBuy->setPosition(aBlitBG->getPositionX(),aBlitBG->getPositionY()-110);
    plantMenuBuy->setVisible(false);
    plantMenuBuy->setTag(113);
    addChild(plantMenuBuy, 10);
    
    //set some star info
    for(int i=0;i<5;i++)
    {
        CCSprite* aStartEmpty = CCSprite::create("Interfeiss/upgrade_screen/star_empty.png");
        aStartEmpty->setPosition(ccp(52+i*44,50));
        aStartEmpty->setTag(120+i);
        aStartEmpty->setVisible(false);
        aBlitBG->addChild(aStartEmpty,10);
        
        //Check what star amount is visible
        CCSprite* aStartFull = CCSprite::create("Interfeiss/upgrade_screen/star_gold.png");
        aStartFull->setPosition(ccp(52+i*44,50));
        aStartFull->setTag(130+i);
        aStartFull->setVisible(false);
        aBlitBG->addChild(aStartFull,10);
    }
    
    //The text part !!!
    CCLabelTTF* aTxtPlantHeader = CCLabelTTF::create("Crystal weed", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.65, CCSize(200, 120), kCCTextAlignmentCenter);
    aTxtPlantHeader->setPosition(ccp(aBlitBG->getContentSize().width/2,aBlitBG->getContentSize().height-72));
    aTxtPlantHeader->setColor(ccc3(53, 113, 101));
    aBlitBG->addChild(aTxtPlantHeader);
    
    CCLabelTTF* aTxtPlantDesc = CCLabelTTF::create("This text is ultra smart. It contains only important gameplay strategy secrets.",
                                                     "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.42, CCSize(265, 100), kCCTextAlignmentCenter);
    aTxtPlantDesc->setPosition(ccp(aBlitBG->getContentSize().width/2,aBlitBG->getContentSize().height-96));
    aTxtPlantDesc->setColor(ccc3(82, 44, 19));
    aTxtPlantDesc->setTag(163);
    aBlitBG->addChild(aTxtPlantDesc);
    
    //Checks by state
    CCLabelTTF* aTxtPlantCost = CCLabelTTF::create("Upgrade cost:","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(265, 100), kCCTextAlignmentLeft);
    aTxtPlantCost->setPosition(ccp(aBlitBG->getContentSize().width/2+10,aBlitBG->getContentSize().height-170));
    aTxtPlantCost->setColor(ccc3(82, 44, 19));
    aTxtPlantCost->setTag(164);
    aBlitBG->addChild(aTxtPlantCost);
    
    //The price
    CCLabelTTF* aTxtPlantCostPrice = CCLabelTTF::create("50000","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.45, CCSize(150, 100), kCCTextAlignmentCenter);
    aTxtPlantCostPrice->setPosition(ccp(aBlitBG->getContentSize().width/2+60,aBlitBG->getContentSize().height-173));
    aTxtPlantCostPrice->setColor(ccWHITE);
    aTxtPlantCostPrice->setTag(165);
    aBlitBG->addChild(aTxtPlantCostPrice);
    
    //And the crystal and dimand for price
    CCSprite* aPlantCrystal = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
    aPlantCrystal->setPosition(ccp(aBlitBG->getContentSize().width-aPlantCrystal->getContentSize().width,aBlitBG->getContentSize().height-135));//This is the upgrade pos
    aPlantCrystal->setTag(150);
    aPlantCrystal->setVisible(false);
    aBlitBG->addChild(aPlantCrystal,10);
    
    CCSprite* aPlantDiamond = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
    aPlantDiamond->setPosition(ccp(aBlitBG->getContentSize().width-aPlantDiamond->getContentSize().width,aBlitBG->getContentSize().height-170));//This is the upgrade pos
    aPlantDiamond->setTag(151);
    aBlitBG->addChild(aPlantDiamond,10);
    
    //Set the states
    ChangeButtonState(1,mPlantButtonState);
    CreateMachineByLevel(1,User::getInstance()->getMachine_PlantLvl(),true);//Get from user stuff
    
    //---------------------------------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------
    //Ghost stuff
    CCSprite* aGhostBG = CCSprite::create("Interfeiss/upgrade_screen/infopanel.png");
    aGhostBG->setPosition(ccp(visibleSize.width/2+aBlockW/2+75,
                              visibleSize.height/2+aBlockH/2-68));
    aGhostBG->setTag(200);
    addChild(aGhostBG);
    
    //The buttons !!!
    mGhostButtonState = 1;//Unlock state
    if(User::getInstance()->getMachine_GhostLvl()>0)
        mGhostButtonState = 2;
    
    //The unlock
    CCMenuItemImage* ghostUnlock = CCMenuItemImage::create(
                                                             "Interfeiss/upgrade_screen/unlocknow_btn0001.png",
                                                             "Interfeiss/upgrade_screen/unlocknow_btn0002.png",
                                                             this,
                                                             menu_selector(UpgradeScene::UnlockNow_Ghost));
    CCMenu* ghostMenuUnlock = CCMenu::create(ghostUnlock, NULL);
    ghostMenuUnlock->setPosition(aGhostBG->getPositionX(),aGhostBG->getPositionY()-110);
    ghostMenuUnlock->setVisible(false);
    ghostMenuUnlock->setTag(211);
    addChild(ghostMenuUnlock, 10);
    
    //The upgrade
    CCMenuItemImage* ghostUpgrade = CCMenuItemImage::create(
                                                              "Interfeiss/upgrade_screen/upgrade_btn0001.png",
                                                              "Interfeiss/upgrade_screen/upgrade_btn0002.png",
                                                              this,
                                                              menu_selector(UpgradeScene::UnlockNow_Ghost));
    CCMenu* ghostMenuUpgrade = CCMenu::create(ghostUpgrade, NULL);
    ghostMenuUpgrade->setPosition(aGhostBG->getPositionX(),aGhostBG->getPositionY()-110);
    ghostMenuUpgrade->setVisible(false);
    ghostMenuUpgrade->setTag(212);
    addChild(ghostMenuUpgrade, 10);
    
    //The buy
    CCMenuItemImage* ghostBuy = CCMenuItemImage::create(
                                                          "Interfeiss/upgrade_screen/buy_btn0001.png",
                                                          "Interfeiss/upgrade_screen/buy_btn0002.png",
                                                          this,
                                                          menu_selector(UpgradeScene::Buy_Ghost));
    CCMenu* ghostMenuBuy = CCMenu::create(ghostBuy, NULL);
    ghostMenuBuy->setPosition(aGhostBG->getPositionX(),aGhostBG->getPositionY()-110);
    ghostMenuBuy->setVisible(false);
    ghostMenuBuy->setTag(213);
    addChild(ghostMenuBuy, 10);
    
    //set some star info
    for(int i=0;i<5;i++)
    {
        CCSprite* aStartEmpty = CCSprite::create("Interfeiss/upgrade_screen/star_empty.png");
        aStartEmpty->setPosition(ccp(52+i*44,50));
        aStartEmpty->setTag(220+i);
        aStartEmpty->setVisible(false);
        aGhostBG->addChild(aStartEmpty,10);
        
        //Check what star amount is visible
        CCSprite* aStartFull = CCSprite::create("Interfeiss/upgrade_screen/star_gold.png");
        aStartFull->setPosition(ccp(52+i*44,50));
        aStartFull->setTag(230+i);
        aStartFull->setVisible(false);
        aGhostBG->addChild(aStartFull,10);
    }
    
    //The text part !!!
    CCLabelTTF* aTxtGhostHeader = CCLabelTTF::create("Ghost stump", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.65, CCSize(200, 120), kCCTextAlignmentCenter);
    aTxtGhostHeader->setPosition(ccp(aGhostBG->getContentSize().width/2,aGhostBG->getContentSize().height-72));
    aTxtGhostHeader->setColor(ccc3(53, 113, 101));
    aGhostBG->addChild(aTxtGhostHeader);
    
    CCLabelTTF* aTxtGhostDesc = CCLabelTTF::create("This text is ultra smart. It contains only important gameplay strategy secrets.",
                                                     "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.42, CCSize(265, 100), kCCTextAlignmentCenter);
    aTxtGhostDesc->setPosition(ccp(aGhostBG->getContentSize().width/2,aGhostBG->getContentSize().height-96));
    aTxtGhostDesc->setColor(ccc3(82, 44, 19));
    aTxtGhostDesc->setTag(263);
    aGhostBG->addChild(aTxtGhostDesc);
    
    //Checks by state
    CCLabelTTF* aTxtGhostCost = CCLabelTTF::create("Upgrade cost:","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(265, 100), kCCTextAlignmentLeft);
    aTxtGhostCost->setPosition(ccp(aGhostBG->getContentSize().width/2+10,aGhostBG->getContentSize().height-170));
    aTxtGhostCost->setColor(ccc3(82, 44, 19));
    aTxtGhostCost->setTag(264);
    aGhostBG->addChild(aTxtGhostCost);
    
    //The price
    CCLabelTTF* aTxtGhostCostPrice = CCLabelTTF::create("50000","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.45, CCSize(150, 100), kCCTextAlignmentCenter);
    aTxtGhostCostPrice->setPosition(ccp(aGhostBG->getContentSize().width/2+60,aGhostBG->getContentSize().height-173));
    aTxtGhostCostPrice->setColor(ccWHITE);
    aTxtGhostCostPrice->setTag(265);
    aGhostBG->addChild(aTxtGhostCostPrice);
    
    //And the crystal and dimand for price
    CCSprite* aGhostCrystal = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
    aGhostCrystal->setPosition(ccp(aGhostBG->getContentSize().width-aGhostCrystal->getContentSize().width,aGhostBG->getContentSize().height-135));//This is the upgrade pos
    aGhostCrystal->setTag(250);
    aGhostCrystal->setVisible(false);
    aGhostBG->addChild(aGhostCrystal,10);
    
    CCSprite* aGhostDiamond = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
    aGhostDiamond->setPosition(ccp(aGhostBG->getContentSize().width-aGhostDiamond->getContentSize().width,aGhostBG->getContentSize().height-170));//This is the upgrade pos
    aGhostDiamond->setTag(251);
    aGhostBG->addChild(aGhostDiamond,10);
    
    //Set the states
    ChangeButtonState(2,mGhostButtonState);
    CreateMachineByLevel(2,User::getInstance()->getMachine_GhostLvl(),true);//Get from user stuff
    
    //---------------------------------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------
    //The Blitz stuff
    CCSprite* aPlantBG = CCSprite::create("Interfeiss/upgrade_screen/infopanel.png");
    aPlantBG->setPosition(ccp(visibleSize.width/2-aBlockW/2+115,
                             visibleSize.height/2+aBlockH/2-68));
    aPlantBG->setTag(300);
    addChild(aPlantBG);
    
    //The buttons !!!
    mElectroButtonState = 1;//Unlock state
    if(User::getInstance()->getMachine_EnemiesLvl()>0)
        mElectroButtonState = 2;
    
    //The unlock
    CCMenuItemImage* electroUnlock = CCMenuItemImage::create(
                                                           "Interfeiss/upgrade_screen/unlocknow_btn0001.png",
                                                           "Interfeiss/upgrade_screen/unlocknow_btn0002.png",
                                                           this,
                                                           menu_selector(UpgradeScene::UnlockNow_Electro));
    CCMenu* electroMenuUnlock = CCMenu::create(electroUnlock, NULL);
    electroMenuUnlock->setPosition(aPlantBG->getPositionX(),aPlantBG->getPositionY()-110);
    electroMenuUnlock->setVisible(false);
    electroMenuUnlock->setTag(311);
    addChild(electroMenuUnlock, 10);
    
    //The upgrade
    CCMenuItemImage* electroUpgrade = CCMenuItemImage::create(
                                                            "Interfeiss/upgrade_screen/upgrade_btn0001.png",
                                                            "Interfeiss/upgrade_screen/upgrade_btn0002.png",
                                                            this,
                                                            menu_selector(UpgradeScene::UnlockNow_Electro));
    CCMenu* electroMenuUpgrade = CCMenu::create(electroUpgrade, NULL);
    electroMenuUpgrade->setPosition(aPlantBG->getPositionX(),aPlantBG->getPositionY()-110);
    electroMenuUpgrade->setVisible(false);
    electroMenuUpgrade->setTag(312);
    addChild(electroMenuUpgrade, 10);
    
    //The buy
    CCMenuItemImage* electroBuy = CCMenuItemImage::create(
                                                        "Interfeiss/upgrade_screen/buy_btn0001.png",
                                                        "Interfeiss/upgrade_screen/buy_btn0002.png",
                                                        this,
                                                        menu_selector(UpgradeScene::Buy_Electro));
    CCMenu* electroMenuBuy = CCMenu::create(electroBuy, NULL);
    electroMenuBuy->setPosition(aPlantBG->getPositionX(),aPlantBG->getPositionY()-110);
    electroMenuBuy->setVisible(false);
    electroMenuBuy->setTag(313);
    addChild(electroMenuBuy, 10);
    
    //set some star info
    for(int i=0;i<5;i++)
    {
        CCSprite* aStartEmpty = CCSprite::create("Interfeiss/upgrade_screen/star_empty.png");
        aStartEmpty->setPosition(ccp(52+i*44,50));
        aStartEmpty->setTag(320+i);
        aStartEmpty->setVisible(false);
        aPlantBG->addChild(aStartEmpty,10);
        
        //Check what star amount is visible
        CCSprite* aStartFull = CCSprite::create("Interfeiss/upgrade_screen/star_gold.png");
        aStartFull->setPosition(ccp(52+i*44,50));
        aStartFull->setTag(330+i);
        aStartFull->setVisible(false);
        aPlantBG->addChild(aStartFull,10);
    }
    
    //The text part !!!
    CCLabelTTF* aTxtElectroHeader = CCLabelTTF::create("Elec-tree", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.65, CCSize(200, 120), kCCTextAlignmentCenter);
    aTxtElectroHeader->setPosition(ccp(aPlantBG->getContentSize().width/2,aPlantBG->getContentSize().height-72));
    aTxtElectroHeader->setColor(ccc3(53, 113, 101));
    aPlantBG->addChild(aTxtElectroHeader);
    
    CCLabelTTF* aTxtElectroDesc = CCLabelTTF::create("This text is ultra smart. It contains only important gameplay strategy secrets.",
                                                   "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.42, CCSize(265, 100), kCCTextAlignmentCenter);
    aTxtElectroDesc->setPosition(ccp(aPlantBG->getContentSize().width/2,aPlantBG->getContentSize().height-96));
    aTxtElectroDesc->setColor(ccc3(82, 44, 19));
    aTxtElectroDesc->setTag(363);
    aPlantBG->addChild(aTxtElectroDesc);
    
    //Checks by state
    CCLabelTTF* aTxtElectroCost = CCLabelTTF::create("Upgrade cost:","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(265, 100), kCCTextAlignmentLeft);
    aTxtElectroCost->setPosition(ccp(aPlantBG->getContentSize().width/2+10,aPlantBG->getContentSize().height-170));
    aTxtElectroCost->setColor(ccc3(82, 44, 19));
    aTxtElectroCost->setTag(364);
    aPlantBG->addChild(aTxtElectroCost);
    
    //The price
    CCLabelTTF* aTxtElectroCostPrice = CCLabelTTF::create("50000","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.45, CCSize(150, 100), kCCTextAlignmentCenter);
    aTxtElectroCostPrice->setPosition(ccp(aPlantBG->getContentSize().width/2+60,aPlantBG->getContentSize().height-173));
    aTxtElectroCostPrice->setColor(ccWHITE);
    aTxtElectroCostPrice->setTag(365);
    aPlantBG->addChild(aTxtElectroCostPrice);
    
    //And the crystal and dimand for price
    CCSprite* aElectroCrystal = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
    aElectroCrystal->setPosition(ccp(aPlantBG->getContentSize().width-aElectroCrystal->getContentSize().width,aPlantBG->getContentSize().height-135));//This is the upgrade pos
    aElectroCrystal->setTag(350);
    aElectroCrystal->setVisible(false);
    aPlantBG->addChild(aElectroCrystal,10);
    
    CCSprite* aElectroDiamond = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
    aElectroDiamond->setPosition(ccp(aPlantBG->getContentSize().width-aElectroDiamond->getContentSize().width,aPlantBG->getContentSize().height-170));//This is the upgrade pos
    aElectroDiamond->setTag(351);
    aPlantBG->addChild(aElectroDiamond,10);
    
    //Set the states
    ChangeButtonState(3,mElectroButtonState);
    CreateMachineByLevel(3,User::getInstance()->getMachine_EnemiesLvl(),true);//Get from user stuff

    //---------------------------------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------
    //The Stamp stuff
    CCSprite* aStampBG = CCSprite::create("Interfeiss/upgrade_screen/infopanel.png");
    aStampBG->setPosition(ccp(visibleSize.width/2+aBlockW/2+75,
                              visibleSize.height/2-aBlockH/2+45));
    aStampBG->setTag(400);
    addChild(aStampBG);
    
    //The buttons !!!
    mStampButtonState = 1;//Unlock state
    if(User::getInstance()->getMachine_StampLvl()>0)
        mStampButtonState = 2;
    
    //The unlock
    CCMenuItemImage* stampUnlock = CCMenuItemImage::create(
                                                             "Interfeiss/upgrade_screen/unlocknow_btn0001.png",
                                                             "Interfeiss/upgrade_screen/unlocknow_btn0002.png",
                                                             this,
                                                             menu_selector(UpgradeScene::UnlockNow_Stamp));
    CCMenu* stampMenuUnlock = CCMenu::create(stampUnlock, NULL);
    stampMenuUnlock->setPosition(aStampBG->getPositionX(),aStampBG->getPositionY()-110);
    stampMenuUnlock->setVisible(false);
    stampMenuUnlock->setTag(411);
    addChild(stampMenuUnlock, 10);
    
    //The upgrade
    CCMenuItemImage* stampUpgrade = CCMenuItemImage::create(
                                                              "Interfeiss/upgrade_screen/upgrade_btn0001.png",
                                                              "Interfeiss/upgrade_screen/upgrade_btn0002.png",
                                                              this,
                                                              menu_selector(UpgradeScene::UnlockNow_Stamp));
    CCMenu* stampMenuUpgrade = CCMenu::create(stampUpgrade, NULL);
    stampMenuUpgrade->setPosition(aStampBG->getPositionX(),aStampBG->getPositionY()-110);
    stampMenuUpgrade->setVisible(false);
    stampMenuUpgrade->setTag(412);
    addChild(stampMenuUpgrade, 10);
    
    //The buy
    CCMenuItemImage* stampBuy = CCMenuItemImage::create(
                                                          "Interfeiss/upgrade_screen/buy_btn0001.png",
                                                          "Interfeiss/upgrade_screen/buy_btn0002.png",
                                                          this,
                                                          menu_selector(UpgradeScene::Buy_Stamp));
    CCMenu* stampMenuBuy = CCMenu::create(stampBuy, NULL);
    stampMenuBuy->setPosition(aStampBG->getPositionX(),aStampBG->getPositionY()-110);
    stampMenuBuy->setVisible(false);
    stampMenuBuy->setTag(413);
    addChild(stampMenuBuy, 10);
    
    //set some star info
    for(int i=0;i<5;i++)
    {
        CCSprite* aStartEmpty = CCSprite::create("Interfeiss/upgrade_screen/star_empty.png");
        aStartEmpty->setPosition(ccp(52+i*44,50));
        aStartEmpty->setTag(420+i);
        aStartEmpty->setVisible(false);
        aStampBG->addChild(aStartEmpty,10);
        
        //Check what star amount is visible
        CCSprite* aStartFull = CCSprite::create("Interfeiss/upgrade_screen/star_gold.png");
        aStartFull->setPosition(ccp(52+i*44,50));
        aStartFull->setTag(430+i);
        aStartFull->setVisible(false);
        aStampBG->addChild(aStartFull,10);
    }
    
    //The text part !!!
    CCLabelTTF* aTxtStampHeader = CCLabelTTF::create("Smasher", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.65, CCSize(200, 120), kCCTextAlignmentCenter);
    aTxtStampHeader->setPosition(ccp(aStampBG->getContentSize().width/2,aStampBG->getContentSize().height-72));
    aTxtStampHeader->setColor(ccc3(53, 113, 101));
    aStampBG->addChild(aTxtStampHeader);
    
    CCLabelTTF* aTxtStampDesc = CCLabelTTF::create("This text is ultra smart. It contains only important gameplay strategy secrets.",
                                                     "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.42, CCSize(265, 100), kCCTextAlignmentCenter);
    aTxtStampDesc->setPosition(ccp(aStampBG->getContentSize().width/2,aStampBG->getContentSize().height-96));
    aTxtStampDesc->setColor(ccc3(82, 44, 19));
    aTxtStampDesc->setTag(463);
    aStampBG->addChild(aTxtStampDesc);
    
    //Checks by state
    CCLabelTTF* aTxtStampCost = CCLabelTTF::create("Upgrade cost:","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(265, 100), kCCTextAlignmentLeft);
    aTxtStampCost->setPosition(ccp(aStampBG->getContentSize().width/2+10,aStampBG->getContentSize().height-170));
    aTxtStampCost->setColor(ccc3(82, 44, 19));
    aTxtStampCost->setTag(464);
    aStampBG->addChild(aTxtStampCost);
    
    //The price
    CCLabelTTF* aTxtStampCostPrice = CCLabelTTF::create("50000","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.45, CCSize(150, 100), kCCTextAlignmentCenter);
    aTxtStampCostPrice->setPosition(ccp(aStampBG->getContentSize().width/2+60,aStampBG->getContentSize().height-173));
    aTxtStampCostPrice->setColor(ccWHITE);
    aTxtStampCostPrice->setTag(465);
    aStampBG->addChild(aTxtStampCostPrice);
    
    //And the crystal and dimand for price
    CCSprite* aStampCrystal = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
    aStampCrystal->setPosition(ccp(aStampBG->getContentSize().width-aStampCrystal->getContentSize().width,aStampBG->getContentSize().height-135));//This is the upgrade pos
    aStampCrystal->setTag(450);
    aStampCrystal->setVisible(false);
    aStampBG->addChild(aStampCrystal,10);
    
    CCSprite* aStampDiamond = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
    aStampDiamond->setPosition(ccp(aStampBG->getContentSize().width-aStampDiamond->getContentSize().width,aStampBG->getContentSize().height-170));//This is the upgrade pos
    aStampDiamond->setTag(451);
    aStampBG->addChild(aStampDiamond,10);
    
    //Set the states
    ChangeButtonState(4,mStampButtonState);
    CreateMachineByLevel(4,User::getInstance()->getMachine_StampLvl(),true);//Get from user stuff
    
    UpdateStats();
    
    //Some other stuff
    CCMenuItemImage* backButton = CCMenuItemImage::create(
														  "Interfeiss/back_btn0001.png",
														  "Interfeiss/back_btn0002.png",
														  this,
														  menu_selector(UpgradeScene::onReturn));
	backButton->setAnchorPoint(ccp(0,0));
    
    CCMenu* backMenu = CCMenu::create(backButton, NULL);
    this->addChild(backMenu, 1);
    backMenu->setPosition(0, 0);
    */
    return true;
}

void UpgradeScene::onReturn()
{
    //Check where where we - menu or game !!!
    if(mReturnType == 1)
    {
        //Main menu
        CCScene* mainMenu = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
        CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, mainMenu);
        CCDirector::sharedDirector()->replaceScene(transition);
    }
    else
    {
        //Level end !!!
        
    }
}

void UpgradeScene::UpdateStats()
{
    _scoreLabel->setString(toString(User::getInstance()->getCrystals()).c_str());
    _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
}

//the button actions
void UpgradeScene::UnlockNow_Plant(cocos2d::CCObject *sender)
{
    //Flips button to buy or unlock
    ChangeButtonState(1,3);//Buy to unlock
}

void UpgradeScene::Buy_Plant(cocos2d::CCObject *sender)
{
    /*
    //Try to buy it !!!
    int aLevel = User::getInstance()->getMachine_PlantLvl();
    int aPrice = 35;
    
    if(aLevel == 0)
    {
        aPrice = 75;
        //We want to unlock it !!!
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        User::getInstance()->setMachine_PlantLvl(1);
        CreateMachineByLevel(1,1);
    }
    else
    {
        if(aLevel == 1)
            aPrice = 2000;
        else if(aLevel == 2)
            aPrice = 8000;
        else if(aLevel == 3)
            aPrice = 20000;
        else if(aLevel == 4)
            aPrice = 50000;
        
        //We just want to upgrade it !!!
        int aDidUseDiamonds = User::getInstance()->canUseCrystals(aPrice);
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        if(aLevel<5)
        {
            CreateMachineByLevel(1,aLevel+1,false);//Get from user stuff
            User::getInstance()->addMachine_PlantLvl(1);
        }
    }
    */
    
    UpdateStats();
    
    //Flips button to buy or unlock
    ChangeButtonState(1,2);//Upgrades enabled or shows next upgrade s
}

//The other stuff
void UpgradeScene::UnlockNow_Ghost(cocos2d::CCObject *sender)
{
    //Flips button to buy or unlock
    ChangeButtonState(2,3);//Buy to unlock
}

void UpgradeScene::Buy_Ghost(cocos2d::CCObject *sender)
{
    /*
    //Try to buy it !!!
    int aLevel = User::getInstance()->getMachine_GhostLvl();
    int aPrice = 50;
    
    if(aLevel == 0)
    {
        aPrice = 100;
        //We want to unlock it !!!
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        //TODO
        //        _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
        
        User::getInstance()->setMachine_GhostLvl(1);
        CreateMachineByLevel(2,1);
    }
    else
    {
        if(aLevel == 1)
            aPrice = 2000;
        else if(aLevel == 2)
            aPrice = 8000;
        else if(aLevel == 3)
            aPrice = 20000;
        else if(aLevel == 4)
            aPrice = 50000;
        
        //We just want to upgrade it !!!
        int aDidUseDiamonds = User::getInstance()->canUseCrystals(aPrice);
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        if(aLevel<5)
        {
            CreateMachineByLevel(2,aLevel+1,false);//Get from user stuff
            User::getInstance()->addMachine_GhostLvl(1);
        }
    }
    */
    UpdateStats();
    
    //Flips button to buy or unlock
    ChangeButtonState(2,2);//Upgrades enabled or shows next upgrade s
}

//The other stuff
void UpgradeScene::UnlockNow_Electro(cocos2d::CCObject *sender)
{
    //Flips button to buy or unlock
    ChangeButtonState(3,3);//Buy to unlock
}

void UpgradeScene::Buy_Electro(cocos2d::CCObject *sender)
{
    /*
    //Try to buy it !!!
    int aLevel = User::getInstance()->getMachine_EnemiesLvl();
    int aPrice = 35;
    
    if(aLevel == 0)
    {
        //We want to unlock it !!!
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        //TODO
        //        _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
        
        User::getInstance()->setMachine_EnemiesLvl(1);
        CreateMachineByLevel(3,1);
    }
    else
    {
        if(aLevel == 1)
            aPrice = 2000;
        else if(aLevel == 2)
            aPrice = 8000;
        else if(aLevel == 3)
            aPrice = 20000;
        else if(aLevel == 4)
            aPrice = 50000;
        
        //We just want to upgrade it !!!
        int aDidUseDiamonds = User::getInstance()->canUseCrystals(aPrice);
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        if(aLevel<5)
        {
            CreateMachineByLevel(3,aLevel+1,false);//Get from user stuff
            User::getInstance()->addMachine_EnemiesLvl(1);
        }
    }
    */
    
    UpdateStats();
    
    //Flips button to buy or unlock
    ChangeButtonState(3,2);//Upgrades enabled or shows next upgrade s
}

//The other stuff
void UpgradeScene::UnlockNow_Stamp(cocos2d::CCObject *sender)
{
    //Flips button to buy or unlock
    ChangeButtonState(4,3);//Buy to unlock
}

void UpgradeScene::Buy_Stamp(cocos2d::CCObject *sender)
{
    /*
    //Try to buy it !!!
    int aLevel = User::getInstance()->getMachine_StampLvl();
    int aPrice = 50;
    
    if(aLevel == 0)
    {
        aPrice = 150;
        //We want to unlock it !!!
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        //TODO
        //        _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
        
        User::getInstance()->setMachine_StampLvl(1);
        CreateMachineByLevel(4,1);
    }
    else
    {
        if(aLevel == 1)
            aPrice = 2000;
        else if(aLevel == 2)
            aPrice = 8000;
        else if(aLevel == 3)
            aPrice = 20000;
        else if(aLevel == 4)
            aPrice = 50000;
        
        //We just want to upgrade it !!!
//        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        int aDidUseDiamonds = User::getInstance()->canUseCrystals(aPrice);
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        if(aLevel<5)
        {
            CreateMachineByLevel(4,aLevel+1,false);//Get from user stuff
            User::getInstance()->addMachine_StampLvl(1);
        }
    }
    */
    
    UpdateStats();
    
    //Flips button to buy or unlock
    ChangeButtonState(4,2);//Upgrades enabled or shows next upgrade s
}

void UpgradeScene::ChangeButtonState(int theButton,int theState)
{
    CCMenu* aMenuButton = NULL;
    CCTextFieldTTF* aTextField = NULL;
    CCSprite* aBG = NULL;
    CCSprite* aDummySprite = NULL;
    
    bool aLocked = true;
    int aLevel = 0;
    int aPrice = 0;
    
    std::stringstream thePriceString;
    
    std::stringstream theDesc;
    std::stringstream theUnlockLevel;
    
    /*
    if(theButton == 1)
    {
        aBG = static_cast<CCSprite*>(getChildByTag(100));//Get the old one
        
        //static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(theType));
        aMenuButton = static_cast<CCMenu*>(getChildByTag(110+mPlantButtonState));//Get the old one
        aMenuButton->setVisible(false);
        
        aMenuButton = static_cast<CCMenu*>(getChildByTag(110+theState));
        aMenuButton->setVisible(true);
        
        //Update txt and other stuff
        aLevel = User::getInstance()->getMachine_PlantLvl();
        if(aLevel > 0)
        {
            aLocked = false;
            //GET THE CORRECT PRICE
            if(aLevel == 1)
                aPrice = 2000;
            else if(aLevel == 2)
                aPrice = 8000;
            else if(aLevel == 3)
                aPrice = 20000;
            else if(aLevel == 4)
                aPrice = 50000;
            else
                aPrice = -1;//Hide it ???
            
            theDesc << "Upgraded plant gives better crystals";
            theUnlockLevel << "Crystals tree will grow at random and produce extra crystals for your dwarfs";
        }
        else
        {
            aPrice = 75;
            theUnlockLevel << "Unlock from Mission Set #5";
            theDesc << "Crystals tree will grow at random and produce extra crystals for your dwarfs";
        }
        
        thePriceString << aPrice;
        
        mPlantButtonState = theState;//set the new state
    }
    else if(theButton == 2)
    {
        aBG = static_cast<CCSprite*>(getChildByTag(200));//Get the old one
        
        //static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(theType));
        aMenuButton = static_cast<CCMenu*>(getChildByTag(210+mGhostButtonState));//Get the old one
        aMenuButton->setVisible(false);
        
        aMenuButton = static_cast<CCMenu*>(getChildByTag(210+theState));
        aMenuButton->setVisible(true);
        
        //Update txt and other stuff
        aLevel = User::getInstance()->getMachine_GhostLvl();
        if(aLevel > 0)
        {
            aLocked = false;
            //GET THE CORRECT PRICE
            if(aLevel == 1)
                aPrice = 2000;
            else if(aLevel == 2)
                aPrice = 8000;
            else if(aLevel == 3)
                aPrice = 20000;
            else if(aLevel == 4)
                aPrice = 50000;
            else
                aPrice = -1;//Hide it ???
            
            theDesc << "Upgrade how long machine will work";
            theUnlockLevel << "Turn your dwarf into ghosts and they cannot run into one another.";
        }
        else
        {
            aPrice = 100;
            theUnlockLevel << "Unlock from Mission Set #8";
            theDesc << "Turn your dwarf into ghosts and they cannot run into one another.";
        }
        
        thePriceString << aPrice;
        
        mGhostButtonState = theState;//set the new state
    }
    else if(theButton == 3)
    {
        aBG = static_cast<CCSprite*>(getChildByTag(300));//Get the old one
        
        //static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(theType));
        aMenuButton = static_cast<CCMenu*>(getChildByTag(310+mElectroButtonState));//Get the old one
        aMenuButton->setVisible(false);
        
        aMenuButton = static_cast<CCMenu*>(getChildByTag(310+theState));
        aMenuButton->setVisible(true);
        
        //Update txt and other stuff
        aLevel = User::getInstance()->getMachine_EnemiesLvl();
        if(aLevel > 0)
        {
            aLocked = false;
            //GET THE CORRECT PRICE
            if(aLevel == 1)
                aPrice = 2000;
            else if(aLevel == 2)
                aPrice = 8000;
            else if(aLevel == 3)
                aPrice = 20000;
            else if(aLevel == 4)
                aPrice = 50000;
            else
                aPrice = -1;//Hide it ???
            
            theDesc << "Upgrade how long machine will work";
            theUnlockLevel << "For a limited time zap all enemies out of the existence";
        }
        else
        {
            aPrice = 35;
            theUnlockLevel << "Unlock from Mission Set #2";
            theDesc << "For a limited time zap all enemies out of the existence";
        }
        
        thePriceString << aPrice;
        
        mElectroButtonState = theState;//set the new state
    }
    else if(theButton == 4)
    {
        aBG = static_cast<CCSprite*>(getChildByTag(400));//Get the old one
        
        //static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(theType));
        aMenuButton = static_cast<CCMenu*>(getChildByTag(410+mStampButtonState));//Get the old one
        aMenuButton->setVisible(false);
        
        aMenuButton = static_cast<CCMenu*>(getChildByTag(410+theState));
        aMenuButton->setVisible(true);
        
        //Update txt and other stuff
        aLevel = User::getInstance()->getMachine_StampLvl();
        if(aLevel > 0)
        {
            aLocked = false;
            //GET THE CORRECT PRICE
            if(aLevel == 1)
                aPrice = 2000;
            else if(aLevel == 2)
                aPrice = 8000;
            else if(aLevel == 3)
                aPrice = 20000;
            else if(aLevel == 4)
                aPrice = 50000;
            else
                aPrice = -1;//Hide it ???
            
            theDesc << "Upgrade recharge time";
            theUnlockLevel << "Stomper smashes the earth and clears map of all trolls, traps and dwarfs alike";
        }
        else
        {
            aPrice = 150;
            theUnlockLevel << "Unlock from Mission Set #12";
            theDesc << "Stomper smashes the earth and clears map of all trolls, traps and dwarfs alike";
        }
        
        thePriceString << aPrice;
        
        mStampButtonState = theState;//set the new state
    }
    
    //The universal mega functional
    //Checks if is locked
    if(aLocked)
    {
        //The unlock steps
        if(theState == 1)
        {
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+63));//Get the old one
            aTextField->setString(theUnlockLevel.str().c_str());//Get the correct level req
            aTextField->setFontSize(TITLE_FONT_SIZE*0.6);
            
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+64));//Get the old one
            aTextField->setString("Unlock now for");//Get the correct level req
            aTextField->setHorizontalAlignment(kCCTextAlignmentCenter);
            
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+65));//Get the old one
            aTextField->setString(thePriceString.str().c_str());//Get the correct level req
            aTextField->setFontSize(TITLE_FONT_SIZE*0.8);
            aTextField->setHorizontalAlignment(kCCTextAlignmentCenter);
            aTextField->setPosition(ccp(aBG->getContentSize().width/2,aBG->getContentSize().height-198));
            
            //Position of the crystal
            aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+51));
            float aLength = strlen(aTextField->getString())*0.5f;
            aDummySprite->setPosition(ccp(aTextField->getPositionX()+aLength*25+aDummySprite->getContentSize().width/2,aBG->getContentSize().height-170));
        }
        else if(theState == 3)//want to unlock for money
        {
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+63));//Get the old one
            aTextField->setString(theDesc.str().c_str());//Get the correct level req
            aTextField->setFontSize(TITLE_FONT_SIZE*0.4);
            
            
        }
    }
    else
    {
        if(theState == 3)
        {
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+63));//Get the old one
            aTextField->setString(theDesc.str().c_str());//Get the correct level req
            aTextField->setFontSize(TITLE_FONT_SIZE*0.4);
        }
        else
        {
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+63));//Get the old one
            aTextField->setString(theUnlockLevel.str().c_str());//Get the correct level req
            aTextField->setFontSize(TITLE_FONT_SIZE*0.4);
        }
        
        //The upgrade steps
        if(theState == 1 || theState == 2)
        {
            //Check if button buy is visible !!!
//            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+63));//Get the old one
//            aTextField->setString(theDesc.str().c_str());//Get the correct level req
//            aTextField->setFontSize(TITLE_FONT_SIZE*0.4);
            
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+64));//Get the old one
            aTextField->setString("Upgrade cost:");//Get the correct level req
            aTextField->setHorizontalAlignment(kCCTextAlignmentLeft);
            
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+65));//Get the old one
            aTextField->setString(thePriceString.str().c_str());//Get the correct level req
            aTextField->setFontSize(TITLE_FONT_SIZE*0.45);
            aTextField->setHorizontalAlignment(kCCTextAlignmentCenter);
            aTextField->setPosition(ccp(aBG->getContentSize().width/2+60,aBG->getContentSize().height-173));
            
            aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+51));
            aDummySprite->setVisible(false);
            
            aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+50));
            aDummySprite->setVisible(true);
            
            //Set the stars amount !!!
            for(int i=0;i<5;i++)
            {
                aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+20+i));
                if(aLevel<=i)
                    aDummySprite->setVisible(true);
                else
                    aDummySprite->setVisible(false);
                
                aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+30+i));
                if(aLevel>i)
                    aDummySprite->setVisible(true);
                else
                    aDummySprite->setVisible(false);
            }
            
            if(aLevel == 5)//Max lvl
            {
                aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+64));//Get the old one
                aTextField->setString("Fully upgraded");//Get the correct level req
                aTextField->setHorizontalAlignment(kCCTextAlignmentCenter);
                
                aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+65));//Get the old one
                aTextField->setVisible(false);
                
                aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+50));
                aDummySprite->setVisible(false);
                
                aMenuButton->setVisible(false);
            }
        }
    }
    
    */
}

void UpgradeScene::CreateMachineByLevel(int theMachine,int theLevel,bool theCreate)
{
    bool aLocked = false;
    if(theLevel<1)
    {
        aLocked = true;
        theLevel = 1;
    }
    
    CCSprite* aIcon = NULL;
    
    char theFileName[64];
    //The electro
    if(theMachine == 1)
    {
        sprintf(theFileName, "Interfeiss/upgrade_screen/plant_%i.png", theLevel);
        
        if(!theCreate)
        {
            //Create some fx
            getChildByTag(100)->removeChildByTag(101,true);
            
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(101);
            aIcon->setPosition(ccp(-100,70));
            getChildByTag(100)->addChild(aIcon);
            
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/GhoustActivate.plist");
            p->setPosition(getChildByTag(100)->getPositionX()-250, getChildByTag(100)->getPositionY()-30);
            p->setAutoRemoveOnFinish(true);
            addChild(p,10);
        }
        else
        {
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(101);
            aIcon->setPosition(ccp(-100,70));
            getChildByTag(100)->addChild(aIcon);
        }
    }
    else if(theMachine == 2)
    {
        sprintf(theFileName, "Interfeiss/upgrade_screen/ghost_%i.png", theLevel);
        
        if(!theCreate)
        {
            getChildByTag(200)->removeChildByTag(201,true);
            
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(201);
            aIcon->setPosition(ccp(-68,60));
            getChildByTag(200)->addChild(aIcon);
            
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/GhoustActivate.plist");
            p->setPosition(getChildByTag(200)->getPositionX()-250, getChildByTag(200)->getPositionY()-30);
            p->setAutoRemoveOnFinish(true);
            addChild(p,10);
        }
        else
        {
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(201);
            aIcon->setPosition(ccp(-68,60));
            getChildByTag(200)->addChild(aIcon);
        }
    }
    else if(theMachine == 3)
    {
        sprintf(theFileName, "Interfeiss/upgrade_screen/elektro_%i.png", theLevel);
        
        if(!theCreate)
        {
            getChildByTag(300)->removeChildByTag(301,true);
            
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(301);
            aIcon->setPosition(ccp(-100,60));
            getChildByTag(300)->addChild(aIcon);
            
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/GhoustActivate.plist");
            p->setPosition(getChildByTag(300)->getPositionX()-250, getChildByTag(300)->getPositionY()-30);
            p->setAutoRemoveOnFinish(true);
            addChild(p,10);
        }
        else
        {
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(301);
            aIcon->setPosition(ccp(-100,60));
            getChildByTag(300)->addChild(aIcon);
        }
    }
    else if(theMachine == 4)
    {
        sprintf(theFileName, "Interfeiss/upgrade_screen/stomp_%i.png", theLevel);
        
        if(!theCreate)
        {
            getChildByTag(400)->removeChildByTag(401,true);
            
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(401);
            aIcon->setPosition(ccp(-76,95));
            getChildByTag(400)->addChild(aIcon);
            
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/GhoustActivate.plist");
            p->setPosition(getChildByTag(400)->getPositionX()-250, getChildByTag(400)->getPositionY()-30);
            p->setAutoRemoveOnFinish(true);
            addChild(p,10);
        }
        else
        {
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(401);
            aIcon->setPosition(ccp(-76,95));
            getChildByTag(400)->addChild(aIcon);
        }
    }
    
    //Check if is not locked
    if(aLocked && aIcon!=NULL)
    {
        CCSprite* aLock = CCSprite::create("Interfeiss/upgrade_screen/locked.png");
        aLock->setTag(1000);
        aLock->setPosition(ccp(aIcon->getContentSize().width/2,aIcon->getContentSize().height/2));
        aIcon->addChild(aLock);
    }
//    else if(aIcon!=NULL)
//    {
//        aIcon->removeChildByTag(1000,true);
//    }
}

void UpgradeScene::OnRemoveNode(CCNode* sender)
{
    this->removeChild(sender, true);
}

