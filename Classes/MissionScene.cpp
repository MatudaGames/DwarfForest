//
//  MissionScene.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 12/01/14.
//
//

#include "MissionScene.h"
#include "TutorialScene.h"
#include "IntroScene.h"
#include "StaticSceneManager.h"
#include <SimpleAudioEngine.h>

#include "JniCalls.h"
#include "User.h"
#include "Utils.h"
#include "AppMacros.h"

#include "SpriteAnimation.h"
//#include "CCScrollView.h"

#include "JniCalls.h"
#include <SimpleAudioEngine.h>
//#include "FlurryX.h"
#include <iostream>
#include <SimpleAudioEngine.h>
#include "GameScene.h"
#include "StoreOfferPopup.h"
#include "StoreScene.h"

#include "AnalyticX.h"

//#include <GUI/CCScrollView/CCScrollView.h>

USING_NS_CC;

CCScene* MissionScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MissionScene *layer = MissionScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void MissionScene::MapGenChange(cocos2d::CCObject *sender)
{
    int enabled = dynamic_cast<cocos2d::CCMenuItemToggle*>(sender)->getSelectedIndex();
    if (enabled)
    {
        User::getInstance()->mNewMaps = false;
    }
    else
    {
        User::getInstance()->mNewMaps = true;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

// on "init" you need to initialize your instance
bool MissionScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Popup_Enter").c_str());
    
    //Should come in to active mission !!!
    mCurrentPage = 1;
    mLastPage = 1;
    mMaxPages = 5;
    
    mPrevIndicator = NULL;
    
    mMoveBackScroll = false;
    
    mLastClickedMission = 0;
    
    mExtraOffsetPage = 0;
    
    mScreenSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //Add the debug map button !!!
//    mNewMaps;
    CCMenuItemImage* v1 = CCMenuItemImage::create("map_gen_enabled.png",
                                                  "map_gen_disabled.png", this, NULL);
    
    CCMenuItemImage* v2 = CCMenuItemImage::create("map_gen_disabled.png",
                                                  "map_gen_enabled.png", this, NULL);
    
    CCMenuItemToggle* vibrateButton = CCMenuItemToggle::createWithTarget(
                                                                         this, menu_selector(MissionScene::MapGenChange), v1, v2, NULL);
    vibrateButton->setSelectedIndex(!User::getInstance()->mNewMaps);
    
    CCMenu* lowBar = CCMenu::create(vibrateButton, NULL);
    lowBar->alignItemsHorizontallyWithPadding(20);
    lowBar->setPosition(mScreenSize.width/2, mScreenSize.height/2);
    this->addChild(lowBar, 1);
    
    //--------------------------------------
    
    //Add the back button !!!
    CCMenuItemImage* backButtonReload = CCMenuItemImage::create(
                                                          "DebugStuff/load_mission_1.png",
                                                          "DebugStuff/load_mission_1.png",
                                                          this,
                                                          menu_selector(MissionScene::reloadMissions));
    backButtonReload->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* backButtonReload2 = CCMenuItemImage::create(
                                                                "DebugStuff/load_mission_2.png",
                                                                "DebugStuff/load_mission_2.png",
                                                                this,
                                                                menu_selector(MissionScene::downloadExtra_1));
    backButtonReload2->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* backButtonReload3 = CCMenuItemImage::create(
                                                                 "DebugStuff/load_mission_3.png",
                                                                 "DebugStuff/load_mission_3.png",
                                                                 this,
                                                                 menu_selector(MissionScene::downloadExtra_2));
    backButtonReload3->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* backButtonReload4 = CCMenuItemImage::create(
                                                                 "DebugStuff/load_mission_4.png",
                                                                 "DebugStuff/load_mission_4.png",
                                                                 this,
                                                                 menu_selector(MissionScene::downloadExtra_3));
    backButtonReload4->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* backButtonReload5 = CCMenuItemImage::create(
                                                                 "DebugStuff/load_mission_TEST.png",
                                                                 "DebugStuff/load_mission_TEST.png",
                                                                 this,
                                                                 menu_selector(MissionScene::downloadExtra_4));
    backButtonReload5->setAnchorPoint(ccp(0,0));
    
    CCMenu* backMenuReload = CCMenu::create(backButtonReload,backButtonReload2,backButtonReload3,backButtonReload4,backButtonReload5, NULL);
    this->addChild(backMenuReload, 1);
    backMenuReload->alignItemsVertically();
    backMenuReload->setPosition(20, 400);
    
    //--------------------------------------
    
    
//    CCScrollView* aView = CCScrollView::create();
//    CCScrollView* aView = CCScrollView::create();
    
    
//    CCSprite* atest =CCSprite::create("Interfeiss/checkbox_checked.png");
//    
//    CCLayer *layer = CCLayer::create();
//    layer->setContentSize( CCSizeMake( 200,200 ) );
//    layer->addChild( atest );
//    
//    cocos2d::extension::CCScrollView* aScrol = cocos2d::extension::CCScrollView::create();
//    aScrol->retain();
//    aScrol->setContentSize(CCSizeMake(200, 200));
//    aScrol->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical);
//    aScrol->setPosition(ccp(0,0));
//    aScrol->setContainer(layer);
//    
//    addChild(aScrol);
//    aScrol->setContentOffset( CCPointZero );
//    return true;
    
//    cocos2d::extension::CCScrollView
    
//    cocos2d::extension::CCScrollView
    
    
    
    //Todo - create scrollable stuff !!!
//    CCSprite* aBase = CCSprite::create("Interfeiss/missions/background_tile_missions.png");
    
    //------------------------------------------------------------
    
    CCSprite* aSky_1 = CCSprite::create("Interfeiss/missions/bg_sky.png");
    aSky_1->setAnchorPoint(ccp(0,0));
    aSky_1->setPositionX(0);
    aSky_1->setPositionY(getContentSize().height/2);
    aSky_1->setTag(2001);
    addChild(aSky_1, 0);
    
    aSky_1 = CCSprite::create("Interfeiss/missions/bg_sky.png");
    aSky_1->setAnchorPoint(ccp(0,0));
    aSky_1->setPositionX(-aSky_1->getContentSize().width+1);
    aSky_1->setPositionY(getContentSize().height/2);
    aSky_1->setTag(2000);
    addChild(aSky_1, 0);
    
    aSky_1 = CCSprite::create("Interfeiss/missions/bg_sky.png");
    aSky_1->setAnchorPoint(ccp(0,0));
    aSky_1->setPositionX(aSky_1->getContentSize().width-1.5);
    aSky_1->setPositionY(getContentSize().height/2);
    aSky_1->setTag(2002);
    addChild(aSky_1, 0);
    
    //------------------------------------------------------------
    
    mLayer_1 = CCSprite::create("Interfeiss/missions/bg_woods.png");
    mLayer_1->setAnchorPoint(ccp(0,0));
    mLayer_1->setPositionX(0);
    mLayer_1->setTag(1001);
    addChild(mLayer_1, 0);
    
    mLayer_2 = CCSprite::create("Interfeiss/missions/bg_woods.png");
    mLayer_2->setAnchorPoint(ccp(0,0));
    mLayer_2->setPositionX(-mLayer_2->getContentSize().width+1);
    mLayer_2->setTag(1000);
    addChild(mLayer_2, 0);
    
    mLayer_3 = CCSprite::create("Interfeiss/missions/bg_woods.png");
    mLayer_3->setAnchorPoint(ccp(0,0));
    mLayer_3->setPositionX(mLayer_3->getContentSize().width-1.5);
    mLayer_3->setTag(1002);
    addChild(mLayer_3, 0);
    
    //------------------------------------------------------------
    
    aSky_1 = CCSprite::create("Interfeiss/missions/bg_grass.png");
    aSky_1->setAnchorPoint(ccp(0,0));
    aSky_1->setPositionX(0);
//    aSky_1->setPositionY(getContentSize().height/2);
    aSky_1->setTag(3001);
    addChild(aSky_1, 0);
    
    aSky_1 = CCSprite::create("Interfeiss/missions/bg_grass.png");
    aSky_1->setAnchorPoint(ccp(0,0));
    aSky_1->setPositionX(-aSky_1->getContentSize().width+1);
//    aSky_1->setPositionY(getContentSize().height/2);
    aSky_1->setTag(3000);
    addChild(aSky_1, 0);
    
    aSky_1 = CCSprite::create("Interfeiss/missions/bg_grass.png");
    aSky_1->setAnchorPoint(ccp(0,0));
    aSky_1->setPositionX(aSky_1->getContentSize().width-1.5);
//    aSky_1->setPositionY(getContentSize().height/2);
    aSky_1->setTag(3002);
    addChild(aSky_1, 0);
    
    //------------------------------------------------------------
    
//    CCSprite* header = CCSprite::create("Interfeiss/missions/moon_title.png");
//    header->setAnchorPoint(ccp(0,0.5));
//    header->setPosition(ccp(20,mScreenSize.height-header->getContentSize().height/2));
//    addChild(header);
    
    CCSprite* skiper = CCSprite::create("Interfeiss/missions/skippers.png");
    skiper->setPosition(ccp(mScreenSize.width/2,skiper->getContentSize().height/2));
    addChild(skiper);
    
    //Add the mission talker
//    mMissionBox = CCSprite::create("Interfeiss/pause_menu/dwarf_speech.png");
    mMissionBox = CCSprite::create("Interfeiss/missions/frame_mission_info.png");
    mMissionBox->setPosition(ccp(mMissionBox->getContentSize().width/2+30,mScreenSize.height/2+150));
    addChild(mMissionBox);
    
    //The complete last mission stuff
    mMissionBox_CompletPrev = CCLabelTTF::create("Complete\nMission set #1",
                                           "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8,
                                           CCSize(500, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    mMissionBox_CompletPrev->setPosition(ccp(mMissionBox->getContentSize().width/2,mMissionBox->getContentSize().height/2+6));
    mMissionBox_CompletPrev->enableShadow(CCSize(3,-3),3,false);
    mMissionBox->addChild(mMissionBox_CompletPrev);
    
    //Some default stuff !!!
    mMissionBox_Header = CCLabelTTF::create("",
                                            FONT_SKRANJI, TITLE_FONT_SIZE*0.9,
                                            CCSize(400, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    mMissionBox_Header->setColor(ccc3(255, 255, 255));
    mMissionBox_Header->setPositionX(mMissionBox->getContentSize().width/2);
    mMissionBox_Header->setPositionY(mMissionBox->getContentSize().height+30);
    mMissionBox->addChild(mMissionBox_Header);
    
    //the mission checkboxes
    // the 1st
    mMissionBox_Check1_On = CCSprite::create("Interfeiss/checkbox_checked.png");
    mMissionBox_Check1_On->setScale(0.65f);
    mMissionBox_Check1_On->setPositionX(65);
    mMissionBox_Check1_On->setVisible(false);
    mMissionBox_Check1_On->setPositionY(mMissionBox->getContentSize().height/2+65);
    mMissionBox->addChild(mMissionBox_Check1_On);
    
    mMissionBox_Check1_Off = CCSprite::create("Interfeiss/checkbox_empty.png");
    mMissionBox_Check1_Off->setScale(0.65f);
    mMissionBox_Check1_Off->setPositionX(65);
    mMissionBox_Check1_Off->setVisible(true);
    mMissionBox_Check1_Off->setPositionY(mMissionBox->getContentSize().height/2+65);
    
    //Add some number !!!
    CCLabelTTF* mMissionBox_Number = CCLabelTTF::create("1",
                                                       "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7,
                                                       CCSize(50, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    mMissionBox_Number->setPosition(ccp(mMissionBox_Check1_Off->getContentSize().width/2,mMissionBox_Check1_On->getContentSize().height/2-5));
    mMissionBox_Check1_Off->addChild(mMissionBox_Number);
    
    
    mMissionBox->addChild(mMissionBox_Check1_Off);
    
    aBox1_StartY = mMissionBox_Check1_Off->getPositionY();
    
    mMissionBox_Text1 = CCLabelTTF::create("11111111111111",
                                                   "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                                   CCSize(450, 40), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
//    mMissionBox_Text1->enableShadow(CCSize(0,1), 1.5f, 2,true);
    mMissionBox_Text1->enableStroke(ccBLACK, 0.5f,true);
    
    mMissionBox_Text1->setAnchorPoint(ccp(0,0.5f));
    mMissionBox_Text1->setColor(ccc3(73, 123, 122));
    mMissionBox_Text1->setPositionX(mMissionBox_Check1_On->getPositionX()+40);
    mMissionBox_Text1->setPositionY(mMissionBox_Check1_On->getPositionY()+10);
    mMissionBox->addChild(mMissionBox_Text1);
    
    //now comes the 2nd line/progress
    mMissionBox_Text1p = CCLabelTTF::create("222222222222",
                                           "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                           CCSize(450, 40), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    //    mMissionBox_Text1->enableShadow(CCSize(0,1), 1.5f, 2,true);
    mMissionBox_Text1p->enableStroke(ccBLACK, 0.5f,true);
    
    mMissionBox_Text1p->setAnchorPoint(ccp(0,0.5f));
    mMissionBox_Text1p->setColor(ccc3(181, 149, 85));
    mMissionBox_Text1p->setPositionX(mMissionBox_Check1_On->getPositionX()+40);
    mMissionBox_Text1p->setPositionY(mMissionBox_Check1_On->getPositionY()-10);
    mMissionBox->addChild(mMissionBox_Text1p);

    
    aText1_StartY = mMissionBox_Text1->getPositionY();
    
    // the 2nd
    mMissionBox_Check2_On = CCSprite::create("Interfeiss/checkbox_checked.png");
    mMissionBox_Check2_On->setScale(0.65f);
    mMissionBox_Check2_On->setPositionX(65);
    mMissionBox_Check2_On->setVisible(false);
    mMissionBox_Check2_On->setPositionY(mMissionBox->getContentSize().height/2);
    mMissionBox->addChild(mMissionBox_Check2_On);
    
    mMissionBox_Check2_Off = CCSprite::create("Interfeiss/checkbox_empty.png");
    mMissionBox_Check2_Off->setScale(0.65f);
    mMissionBox_Check2_Off->setPositionX(65);
    mMissionBox_Check2_Off->setVisible(true);
    mMissionBox_Check2_Off->setPositionY(mMissionBox->getContentSize().height/2);
    mMissionBox->addChild(mMissionBox_Check2_Off);
    
    //Add some number !!!
    mMissionBox_Number = CCLabelTTF::create("2",
                                                        "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7,
                                                        CCSize(50, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    mMissionBox_Number->setPosition(ccp(mMissionBox_Check1_Off->getContentSize().width/2,mMissionBox_Check1_On->getContentSize().height/2-5));
    mMissionBox_Check2_Off->addChild(mMissionBox_Number);
    
    aBox2_StartY = mMissionBox_Check2_Off->getPositionY();
    
    mMissionBox_Text2 = CCLabelTTF::create("",
                                           "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                           CCSize(400, 80), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    mMissionBox_Text2->enableStroke(ccBLACK, 0.5f,true);
    
    mMissionBox_Text2->setAnchorPoint(ccp(0,0.5f));
    mMissionBox_Text2->setColor(ccc3(73, 123, 122));
    mMissionBox_Text2->setPositionX(mMissionBox_Check2_On->getPositionX()+40);
    mMissionBox_Text2->setPositionY(mMissionBox_Check2_On->getPositionY()+10);
    mMissionBox->addChild(mMissionBox_Text2);
    
    mMissionBox_Text2p = CCLabelTTF::create("",
                                           "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                           CCSize(400, 80), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    mMissionBox_Text2p->enableStroke(ccBLACK, 0.5f,true);
    
    mMissionBox_Text2p->setAnchorPoint(ccp(0,0.5f));
    mMissionBox_Text2p->setColor(ccc3(181, 149, 85));
    mMissionBox_Text2p->setPositionX(mMissionBox_Check2_On->getPositionX()+40);
    mMissionBox_Text2p->setPositionY(mMissionBox_Check2_On->getPositionY()-10);
    mMissionBox->addChild(mMissionBox_Text2p);
    
    aText2_StartY = mMissionBox_Text2->getPositionY();
    
    // the 3rd
    mMissionBox_Check3_On = CCSprite::create("Interfeiss/checkbox_checked.png");
    mMissionBox_Check3_On->setScale(0.65f);
    mMissionBox_Check3_On->setPositionX(65);
    mMissionBox_Check3_On->setVisible(false);
    mMissionBox_Check3_On->setPositionY(mMissionBox->getContentSize().height/2-65);
    mMissionBox->addChild(mMissionBox_Check3_On);
    
    mMissionBox_Check3_Off = CCSprite::create("Interfeiss/checkbox_empty.png");
    mMissionBox_Check3_Off->setScale(0.65f);
    mMissionBox_Check3_Off->setPositionX(65);
    mMissionBox_Check3_Off->setVisible(true);
    mMissionBox_Check3_Off->setPositionY(mMissionBox->getContentSize().height/2-65);
    mMissionBox->addChild(mMissionBox_Check3_Off);
    
    mMissionBox_Number = CCLabelTTF::create("3",
                                            "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7,
                                            CCSize(50, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    mMissionBox_Number->setPosition(ccp(mMissionBox_Check1_Off->getContentSize().width/2,mMissionBox_Check1_On->getContentSize().height/2-5));
    mMissionBox_Check3_Off->addChild(mMissionBox_Number);
    
    aBox3_StartY = mMissionBox_Check3_Off->getPositionY();
    
    mMissionBox_Text3 = CCLabelTTF::create("",
                                           "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                           CCSize(400, 80), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    mMissionBox_Text3->enableStroke(ccBLACK, 0.5f,true);
    
    mMissionBox_Text3->setAnchorPoint(ccp(0,0.5f));
    mMissionBox_Text3->setColor(ccc3(73, 123, 122));
    mMissionBox_Text3->setPositionX(mMissionBox_Check3_On->getPositionX()+40);
    mMissionBox_Text3->setPositionY(mMissionBox_Check3_On->getPositionY()-4);
    mMissionBox->addChild(mMissionBox_Text3);
    
    mMissionBox_Text3p = CCLabelTTF::create("",
                                            "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                            CCSize(400, 80), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    mMissionBox_Text3p->enableStroke(ccBLACK, 0.5f,true);
    
    mMissionBox_Text3p->setAnchorPoint(ccp(0,0.5f));
    mMissionBox_Text3p->setColor(ccc3(181, 149, 85));
    mMissionBox_Text3p->setPositionX(mMissionBox_Check3_On->getPositionX()+40);
    mMissionBox_Text3p->setPositionY(mMissionBox_Check3_On->getPositionY()-10);
    mMissionBox->addChild(mMissionBox_Text3p);
    
    aText3_StartY = mMissionBox_Text3->getPositionY();
    
    //The reward header
    mMissionRewardBox = CCSprite::create("Interfeiss/missions/frame_rewards.png");
    mMissionRewardBox->setAnchorPoint(ccp(0.5,1.0));
    mMissionRewardBox->setPosition(ccp(mScreenSize.width-mMissionBox->getContentSize().width/4-10,
                                       mMissionBox->getPositionY()+mMissionBox->getContentSize().height/2));
    addChild(mMissionRewardBox);
    
    if(User::getInstance()->mNewMissionBuild){
        mMissionRewardBox->setVisible(false);
        mMissionBox->setVisible(false);
    }
    
    //The reward
    CCLabelTTF* mission_reward = CCLabelTTF::create("Reward:",
                                                    FONT_SKRANJI, TITLE_FONT_SIZE*0.8,
                                                    CCSize(240, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    mission_reward->setAnchorPoint(ccp(0.5f,0.5f));
    mission_reward->setColor(ccc3(255, 255, 255));
    mission_reward->setPositionX(mMissionRewardBox->getContentSize().width/2);
    mission_reward->setPositionY(mMissionRewardBox->getContentSize().height+25);
    mMissionRewardBox->addChild(mission_reward);
    
    //The real reward
//This will be something special
//    mMissionBox_Reward = CCLabelTTF::create("",
//                                                           "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.6,
//                                                           CCSize(260, 120), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//    mMissionBox_Reward->setAnchorPoint(ccp(0,0.5f));
//    mMissionBox_Reward->setRotation(2);
//    mMissionBox_Reward->setColor(ccc3(255, 255,255));
//    mMissionBox_Reward->setPositionX(mMissionBox_Check1_On->getPositionX()+160);
//    mMissionBox_Reward->setPositionY(46);//54
//    
//    mMissionBox_Reward->enableShadow(CCSize(0,1), 1.5f, 2,true);
////    mMissionBox_Reward->enableStroke(ccRED, 3,true);
//    mMissionBox->addChild(mMissionBox_Reward);
    
    
    //Add the back button !!!
    CCMenuItemImage* backButton = CCMenuItemImage::create(
														  "Interfeiss/back_btn0001.png",
														  "Interfeiss/back_btn0002.png",
														  this,
														  menu_selector(MissionScene::menuBackCallback));
	backButton->setAnchorPoint(ccp(0,0));
    
    CCMenu* backMenu = CCMenu::create(backButton, NULL);
    this->addChild(backMenu, 1);
    backMenu->setPosition(-10, -10);
    
    //Add the play button!!!
    backButton = CCMenuItemImage::create(
														  "Interfeiss/missions/play_btn_small0001.png",
														  "Interfeiss/missions/play_btn_small0002.png",
														  this,
														  menu_selector(MissionScene::menuPlayCallback));
    
    backMenu = CCMenu::create(backButton, NULL);
    backMenu->setTag(778);
    this->addChild(backMenu, 1);
    
    if(User::getInstance()->mNewMissionBuild){
        backMenu->setPosition(mScreenSize.width/2,mScreenSize.height-150);
    }
    else{
        backMenu->setPosition(mMissionRewardBox->getPositionX(),
                              mMissionRewardBox->getPositionY()-mMissionRewardBox->getContentSize().height-backButton->getContentSize().height/2+15);
    }
    
    //Create the missions !!!
    CreateMissions();
    
    return true;
}

void MissionScene::reloadMissions(CCObject* sender)
{
    User::getInstance()->getMissionManager().ReDownloadStuff();
}

void MissionScene::downloadExtra_1(CCObject* sender)
{
    User::getInstance()->getMissionManager().GetExtraMissions_1();
}

void MissionScene::downloadExtra_2(CCObject* sender)
{
    User::getInstance()->getMissionManager().GetExtraMissions_2();
}

void MissionScene::downloadExtra_3(CCObject* sender)
{
    User::getInstance()->getMissionManager().GetExtraMissions_3();
}

void MissionScene::downloadExtra_4(CCObject* sender)
{
    User::getInstance()->getMissionManager().OnDownloadSpecialMissions();
}

void MissionScene::menuBackCallback(CCObject* sender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Tap_Back").c_str());
    
    AnalyticX::flurryLogEvent("Menu Mission: Clouse");
    
	CCScene* mainMenu = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
//	CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, mainMenu);
    CCTransitionFade* transition = CCTransitionFade::create(0.25f,mainMenu,ccBLACK);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void MissionScene::menuPlayCallback(CCObject* sender)
{
    
//    FlurryX::logEvent("Press Play Game");
    CCLOG("mLastClickedMission:%i",mLastClickedMission);
    
    User::getInstance()->getMissionManager().mCurrentActiveMission = mLastClickedMission;
    
    
    AnalyticX::flurryLogEvent("Menu Mission: Play");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Tap_Back").c_str());
    
    CCScene* scene;
    
    User::getInstance()->mSpecial_10_Mission = false;
    User::getInstance()->mSpecial_11_Mission = false;
    User::getInstance()->mSpecial_12_Mission = false;
    User::getInstance()->mSpecial_13_Mission = false;
    User::getInstance()->mSpecial_14_Mission = false;
    
    User::getInstance()->mSpecial_16_Mission = false;
    User::getInstance()->mSpecial_17_Mission = false;
    User::getInstance()->mSpecial_18_Mission = false;
    
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
        /*
        //Activate the hack mission for test !!!
        if(mLastClickedMission == 10){
            User::getInstance()->mSpecial_10_Mission = true;
        }
        else if(mLastClickedMission == 11){
            User::getInstance()->mSpecial_11_Mission = true;
        }
        else if(mLastClickedMission == 12){
            User::getInstance()->mSpecial_12_Mission = true;
        }
        else if(mLastClickedMission == 13){
            User::getInstance()->mSpecial_13_Mission = true;
        }
        else if(mLastClickedMission == 14){
            User::getInstance()->mSpecial_14_Mission = true;
        }
        else if(mLastClickedMission == 16){
            User::getInstance()->mSpecial_16_Mission = true;
            User::getInstance()->setMachine_EnemiesLvl(1);
        }
        else if(mLastClickedMission == 17){
            User::getInstance()->mSpecial_17_Mission = true;
            User::getInstance()->setMachine_EnemiesLvl(1);
            
        }
        else if(mLastClickedMission == 18){
            User::getInstance()->mSpecial_16_Mission = true;
            User::getInstance()->mDynamicTrolls = true;
            User::getInstance()->setMachine_EnemiesLvl(1);
        }
        else if(mLastClickedMission == 19){
            User::getInstance()->mSpecial_18_Mission = true;
            User::getInstance()->mDynamicTrolls = true;
            User::getInstance()->setMachine_EnemiesLvl(1);
        }
        else if(mLastClickedMission == 20){
            User::getInstance()->mSpecial_19_Mission = true;
            User::getInstance()->mDynamicTrolls = true;
            User::getInstance()->setMachine_EnemiesLvl(1);
        }
        else if(mLastClickedMission == 21){
            User::getInstance()->mNewSplitCaves = true;
            User::getInstance()->mSpecial_20_Mission = true;
            User::getInstance()->mDynamicTrolls = true;
            User::getInstance()->setMachine_EnemiesLvl(1);
        }
        else if(mLastClickedMission == 22){
            User::getInstance()->mSpecial_21_Mission = true;
            User::getInstance()->mDynamicTrolls = true;
            User::getInstance()->setMachine_EnemiesLvl(1);
        }
        else if(mLastClickedMission == 23){
            User::getInstance()->mSpecial_22_Mission = true;
            User::getInstance()->mDynamicTrolls = true;
            User::getInstance()->setMachine_EnemiesLvl(1);
        }
        else if(mLastClickedMission == 24){
            User::getInstance()->mSpecial_23_Mission = true;
            User::getInstance()->mDynamicTrolls = true;
            User::getInstance()->setMachine_EnemiesLvl(1);
        }
        */
        
        
        scene = GameScene::scene();
    }
    
    CCTransitionScene* transition = CCTransitionFade::create(0.5f, scene);
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, scene);
    CCDirector::sharedDirector()->replaceScene(transition);
    
	
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
    //Debug achievement show!!!
    JniCalls::completedAchievement("achievement_play_the_game",1);//completeAchievment
}

void MissionScene::OnClickedMission(CCObject* sender)
{
    //get the button id !!!
    
    CCMenuItemImage* aButton =(CCMenuItemImage*)sender;
    int aID = aButton->getTag()-5001;
    
//    if(aID>2)
//        return;//Not added now
    PlaceIndicator(aButton);
    OnSelectMission(aID);
}

void MissionScene::ShowRewardInfo(int theMissionID)
{
    /*
    MissionSet aMission = User::getInstance()->getMissionManager().GetMissionByID(theMissionID);
    //What do we have in reward???
    int aDoubleReward = false;
    
    CCSprite* aMissionRewardIcon = NULL;
    CCSprite* aMissionUnlockIcon = NULL;
    
    int aOffIconX = 0;
    
    //Clear the old off
    if(mMissionRewardBox->getChildByTag(1111))
        mMissionRewardBox->removeChildByTag(1111);
    
    if(mMissionRewardBox->getChildByTag(1112))
        mMissionRewardBox->removeChildByTag(1112);
    
    if(aMission.reward_type==REWARD_DIAMONDS)
    {
        aOffIconX = 25;
        
        aMissionRewardIcon = CCSprite::create("Interfeiss/missions/reward_diamond.png");
        aMissionRewardIcon->setTag(1111);
        
        //Add some text !!!
        std::stringstream theDiamondAmount;
        theDiamondAmount<<"+"<<aMission.reward_amount;
        std::string aFinalTxt = theDiamondAmount.str();
        
        CCLabelTTF* aRewardTxt = CCLabelTTF::create(aFinalTxt.c_str(),
                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*0.9,
                                         CCSize(120, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        aRewardTxt->setColor(ccc3(255, 255, 255));
        aRewardTxt->setRotation(-10);
        aRewardTxt->setPositionX(0);
        aRewardTxt->setPositionY(aMissionRewardIcon->getContentSize().height/2-10);
        aRewardTxt->setTag(100);
        aMissionRewardIcon->addChild(aRewardTxt);
    }
    else if(aMission.reward_type == REWARD_CRYSTALS)
    {
        aOffIconX = 8;
        
        aMissionRewardIcon = CCSprite::create("Interfeiss/level_end/highscore_cup.png");
        aMissionRewardIcon->setScale(0.8f);
        aMissionRewardIcon->setTag(1111);
        
        //Add some text !!!
        std::stringstream theDiamondAmount;
        theDiamondAmount<<"+"<<aMission.reward_amount;
        std::string aFinalTxt = theDiamondAmount.str();
        
        CCLabelTTF* aRewardTxt = CCLabelTTF::create(aFinalTxt.c_str(),
                                                    FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*0.9,
                                                    CCSize(200, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        aRewardTxt->setColor(ccc3(255, 255, 255));
//        CCLog("box width %f",aRewardTxt->getContentSize().width);
//        aRewardTxt->setRotation(-10);
        aRewardTxt->setPositionX(aMissionRewardIcon->getContentSize().width/2-10);
        aRewardTxt->setPositionY(aMissionRewardIcon->getContentSize().height/2-30);
        aRewardTxt->setTag(100);
        aMissionRewardIcon->addChild(aRewardTxt);
    }
    else if(aMission.reward_type == REWARD_SCORE_MULTY)
    {
        aOffIconX = 10;
        
        aMissionRewardIcon = CCSprite::create("small_dot_red.png");
        aMissionRewardIcon->setTag(1111);
        aMissionRewardIcon->setOpacity(0);
        
        //Add some text !!!
        std::stringstream theDiamondAmount;
        theDiamondAmount<<"x"<<aMission.reward_amount;
        std::string aFinalTxt = theDiamondAmount.str();
        
        CCLabelTTF* aRewardTxt = CCLabelTTF::create(aFinalTxt.c_str(),
                                                    FONT_SKRANJI, TITLE_FONT_SIZE*1.0,
                                                    CCSize(200, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        aRewardTxt->setColor(ccc3(255, 255, 255));
//        CCLog("box width %f",aRewardTxt->getContentSize().width);
        //        aRewardTxt->setRotation(-10);
        aRewardTxt->setPositionX(aMissionRewardIcon->getContentSize().width/2-10);
        aRewardTxt->setPositionY(aMissionRewardIcon->getContentSize().height+15);
        aRewardTxt->setTag(100);
        aMissionRewardIcon->addChild(aRewardTxt);
        
        //Comes another text about score multiplier
        theDiamondAmount.str("");
        theDiamondAmount<<"Score";
        aFinalTxt = theDiamondAmount.str();
        
        aRewardTxt = CCLabelTTF::create(aFinalTxt.c_str(),
                                        FONT_SKRANJI, TITLE_FONT_SIZE*0.5,
                                        CCSize(200, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        aRewardTxt->setPositionX(aMissionRewardIcon->getContentSize().width/2-10);
        aRewardTxt->setPositionY(aMissionRewardIcon->getContentSize().height/2-15);
        aRewardTxt->setTag(101);
        aRewardTxt->setColor(ccc3(73, 123, 122));
        aMissionRewardIcon->addChild(aRewardTxt);
        
        
        theDiamondAmount.str("");
        theDiamondAmount<<"multiplier";
        aFinalTxt = theDiamondAmount.str();
        
        aRewardTxt = CCLabelTTF::create(aFinalTxt.c_str(),
                                        FONT_SKRANJI, TITLE_FONT_SIZE*0.5,
                                        CCSize(200, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        aRewardTxt->setPositionX(aMissionRewardIcon->getContentSize().width/2-10);
        aRewardTxt->setPositionY(aMissionRewardIcon->getContentSize().height/2-40);
        aRewardTxt->setTag(102);
        aRewardTxt->setColor(ccc3(73, 123, 122));
        aMissionRewardIcon->addChild(aRewardTxt);
    }
    
    //Check if we have some unlock too !!!
    if(aMission.unlock_type>0)
    {
        //We have !!!
        aDoubleReward = true;
        
        if(aMission.unlock_type == UNLOCK_CRYSTAL_PLANT)
        {
            aMissionUnlockIcon = CCSprite::create("Interfeiss/upgrade_screen/plant_1.png");
            aMissionUnlockIcon->setTag(1112);
            aMissionUnlockIcon->setScale(0.8);
        }
        else if(aMission.unlock_type == UNLOCK_TESLA_MACHINE)
        {
            aMissionUnlockIcon = CCSprite::create("Interfeiss/upgrade_screen/elektro_1.png");
            aMissionUnlockIcon->setTag(1112);
            aMissionUnlockIcon->setScale(0.7);
        }
        else
        {
            //Suprise !!!
            aMissionUnlockIcon = CCSprite::create("Interfeiss/upgrade_screen/star_gold.png");
            aMissionUnlockIcon->setTag(1112);
            aMissionUnlockIcon->setScale(0.8);
        }
    }
    
    //Now position all that stuff !!!
    if(aDoubleReward)
    {
        //Simple center !!!
        if(aMissionRewardIcon!=NULL)
        {
            aMissionRewardIcon->setPosition(ccp(mMissionRewardBox->getContentSize().width/2+aOffIconX-50,
                                                mMissionRewardBox->getContentSize().height/2));
            mMissionRewardBox->addChild(aMissionRewardIcon);
        }
        
        if(aMissionUnlockIcon!=NULL)
        {
            aMissionUnlockIcon->setPosition(ccp(mMissionRewardBox->getContentSize().width/2+aOffIconX+50,
                                                mMissionRewardBox->getContentSize().height/2));
            mMissionRewardBox->addChild(aMissionUnlockIcon);
        }
    }
    else
    {
        //Simple center !!!
        if(aMissionRewardIcon!=NULL)
        {
            aMissionRewardIcon->setPosition(ccp(mMissionRewardBox->getContentSize().width/2+aOffIconX,
                                                mMissionRewardBox->getContentSize().height/2));
            mMissionRewardBox->addChild(aMissionRewardIcon);
        }
    }
    */
}

void MissionScene::CreateMissions()
{
//    mission_pad
    int aMoveY = 100;
    
    //Create the hot spot places !!!
    int aMoveX = (mScreenSize.width/5)/2;
    int aMoveRealX = aMoveX+10;
    
    CCLabelTTF* mission_num;
    bool aNeedText = true;
    bool aNeedDone = false;
    int aMissionID = 1;
    
    CCArray* aButtons3 = CCArray::create();
//    aButtons3->retain();
    CCMenuItemImage* aButtonReal;
    CCMenu* lowBar;
    
    //This is only at the start created !!!
    mCurrentPage = 1;//For now always 1 reward_amount
    
    if(User::getInstance()->mSpecialMissionBuild)
    {
        mCurrentPage = 3;
        mExtraOffsetPage = 1;
    }
    
    //Where to put our control point !!!
    mActiveMission = 1;//User::getInstance()->getMissionManager().GetActiveMissionID()+1;//The completed mission stuff !!!
    
    if(User::getInstance()->mNewMissionBuild){
        mActiveMission = User::getInstance()->getMissionManager().GetActiveMissionID();
        
        //Check if we don't want to show some new stuff - then -1
        if(User::getInstance()->mShowNewPart){
            mActiveMission-=1;
        }
    }
    
    MissionSet aMission;
    CCSprite* aDummy;
    
    if(mCurrentPage==1)
    {
        aMissionID = 1;
        aButtons3->removeAllObjects();
        
        if(mCurrentPage % 2 == 1)
            aMoveY = 100;
        else
            aMoveY = 140;
        
        for(int i=0;i<5;i++)
        {
            CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
            aPad_1->setAnchorPoint(ccp(0.5,0.0));
            aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
            
            //Now comes the real magic for missions !!!
            aMission = User::getInstance()->getMissionManager().GetMissionByID(aMissionID-1);
            
            //First check if this is not the mission player is standing on !!!
            if(mActiveMission==aMissionID)
            {
                //Add player chars here !!!
                mDwarfMove = CCSprite::create("Interfeiss/missions/mission_dwarfs.png");
                mDwarfMove->setPosition(ccp(aPad_1->getContentSize().width/2,aPad_1->getContentSize().height/2+40));
                aPad_1->addChild(mDwarfMove);
            }
            else
            {
                //Check if was not completed
                if(aMissionID<mActiveMission)
                {
                    //This is completed
                    aDummy = CCSprite::create("Interfeiss/missions/mission_check.png");
                    aDummy->setPosition(ccp(aPad_1->getContentSize().width/2+10,aPad_1->getContentSize().height/2+40));
                    aPad_1->addChild(aDummy);
                }
                else
                {
                    if(aMission.unlock_type>0)
                    {
                        //Try to show what unlocks here !!!
                        aPad_1 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aMissionID-1).c_str());
                        aPad_1->setAnchorPoint(ccp(0.5,0.0));
                        aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
                    }
                    else
                    {
                        mission_num = CCLabelTTF::create(toString(aMissionID).c_str(),
                                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                                         CCSize(120, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
                        mission_num->setColor(ccc3(251, 255, 195));
                        mission_num->setPositionX(aPad_1->getContentSize().width/2);
                        mission_num->setPositionY(aPad_1->getContentSize().height/2+30);
                        mission_num->setTag(100);
                        aPad_1->addChild(mission_num);
                    }
                }
            }
            
            //============================================================
            //Add button above?
            aButtonReal = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                  "Interfeiss/missions/MissionSubClick.png",
                                                  this,
                                                  menu_selector(MissionScene::OnClickedMission));
            aButtonReal->setTag(5000+aMissionID);
            aButtonReal->setOpacity(128);
            aButtonReal->setPosition(aMoveRealX,aMoveY);
            aButtonReal->cocos2d::CCMenuItem::setEnabled(false);
            aButtons3->addObject(aButtonReal);
            
            if(mActiveMission == aMissionID)
                PlaceIndicator(aButtonReal);
            //============================================================
            
            aMissionID+=1;
            
            aMoveRealX+=aMoveX+90;
            aPad_1->setTag(i+1000);
            
            //hack - if this is the dwarf pad - add layer above all
            if(mActiveMission==aMissionID){
                getChildByTag(1001)->addChild(aPad_1,1000);
            }
            else{
                getChildByTag(1001)->addChild(aPad_1);
            }
            
            if(aMoveY==100)
                aMoveY = 140;
            else
                aMoveY = 100;
        }
        
        lowBar = CCMenu::createWithArray(aButtons3);
        lowBar->setTag(90);
        lowBar->setPosition(0, 50);
        getChildByTag(1001)->addChild(lowBar, 1);
        
        aMoveRealX = aMoveX+10;
        aButtons3->removeAllObjects();
        
        //The 2nd page !!!
        for(int i=0;i<5;i++)
        {
            CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
            aPad_1->setAnchorPoint(ccp(0.5,0.0));
            aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
            
            //Now comes the real magic for missions !!!
            aMission = User::getInstance()->getMissionManager().GetMissionByID(aMissionID-1);
            
            //First check if this is not the mission player is standing on !!!
            if(mActiveMission==aMissionID)
            {
                //Add player chars here !!!
                mDwarfMove = CCSprite::create("Interfeiss/missions/mission_dwarfs.png");
                mDwarfMove->setPosition(ccp(aPad_1->getContentSize().width/2,aPad_1->getContentSize().height/2+40));
                aPad_1->addChild(mDwarfMove);
            }
            else
            {
                //Check if was not completed
                if(aMissionID<mActiveMission)
                {
                    //This is completed
                    aDummy = CCSprite::create("Interfeiss/missions/mission_check.png");
                    aDummy->setPosition(ccp(aPad_1->getContentSize().width/2+10,aPad_1->getContentSize().height/2+40));
                    aPad_1->addChild(aDummy);
                }
                else
                {
                    if(aMission.unlock_type>0)
                    {
                        //Try to show what unlocks here !!!
                        aPad_1 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aMissionID-1).c_str());
                        aPad_1->setAnchorPoint(ccp(0.5,0.0));
                        aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
                    }
                    else
                    {
                        mission_num = CCLabelTTF::create(toString(aMissionID).c_str(),
                                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                                         CCSize(120, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
                        mission_num->setColor(ccc3(251, 255, 195));
                        mission_num->setPositionX(aPad_1->getContentSize().width/2);
                        mission_num->setPositionY(aPad_1->getContentSize().height/2+30);
                        mission_num->setTag(100);
                        aPad_1->addChild(mission_num);
                    }
                }
            }
            
            //============================================================
            //Add button above?
            aButtonReal = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                  "Interfeiss/missions/MissionSubClick.png",
                                                  this,
                                                  menu_selector(MissionScene::OnClickedMission));
            aButtonReal->setTag(5000+aMissionID);
            aButtonReal->setOpacity(0);
            aButtonReal->setPosition(aMoveRealX,aMoveY);
            aButtonReal->cocos2d::CCMenuItem::setEnabled(false);
            aButtons3->addObject(aButtonReal);
            
            if(mActiveMission == aMissionID)
                PlaceIndicator(aButtonReal);
            //============================================================
            
            aMissionID+=1;
            
            aMoveRealX+=aMoveX+90;
            aPad_1->setTag(i+1000);
            if(mActiveMission==aMissionID){
                getChildByTag(1002)->addChild(aPad_1,1000);
            }
            else{
                getChildByTag(1002)->addChild(aPad_1);
            }
            
            
            if(aMoveY==100)
                aMoveY = 140;
            else
                aMoveY = 100;
            
        }
        
        
        lowBar = CCMenu::createWithArray(aButtons3);
        lowBar->setTag(90);
        lowBar->setPosition(0, 50);
        getChildByTag(1002)->addChild(lowBar, 1);
    }
    else if(mCurrentPage == mMaxPages)
    {
        aMissionID = (mCurrentPage-2)*5+1;
        aButtons3->removeAllObjects();
        
        if(mCurrentPage-1 % 2 == 1)
            aMoveY = 100;
        else
            aMoveY = 140;
        
        for(int i=0;i<5;i++)
        {
            CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
            aPad_1->setAnchorPoint(ccp(0.5,0.0));
            aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
            
            //Now comes the real magic for missions !!!
            aMission = User::getInstance()->getMissionManager().GetMissionByID(aMissionID-1);
            
            //First check if this is not the mission player is standing on !!!
            if(mActiveMission==aMissionID)
            {
                //Add player chars here !!!
                mDwarfMove = CCSprite::create("Interfeiss/missions/mission_dwarfs.png");
                mDwarfMove->setPosition(ccp(aPad_1->getContentSize().width/2,aPad_1->getContentSize().height/2+40));
                aPad_1->addChild(mDwarfMove);
            }
            else
            {
                //Check if was not completed
                if(aMissionID<mActiveMission)
                {
                    //This is completed
                    aDummy = CCSprite::create("Interfeiss/missions/mission_check.png");
                    aDummy->setPosition(ccp(aPad_1->getContentSize().width/2+10,aPad_1->getContentSize().height/2+40));
                    aPad_1->addChild(aDummy);
                }
                else
                {
                    if(aMission.unlock_type>0)
                    {
                        //Try to show what unlocks here !!!
                        aPad_1 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aMissionID-1).c_str());
                        aPad_1->setAnchorPoint(ccp(0.5,0.0));
                        aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
                    }
                    else
                    {
                        mission_num = CCLabelTTF::create(toString(aMissionID).c_str(),
                                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                                         CCSize(120, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
                        mission_num->setColor(ccc3(251, 255, 195));
                        mission_num->setPositionX(aPad_1->getContentSize().width/2);
                        mission_num->setPositionY(aPad_1->getContentSize().height/2+30);
                        mission_num->setTag(100);
                        aPad_1->addChild(mission_num);
                    }
                }
            }
            
            //============================================================
            //Add button above?
            aButtonReal = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                  "Interfeiss/missions/MissionSubClick.png",
                                                  this,
                                                  menu_selector(MissionScene::OnClickedMission));
            aButtonReal->setTag(5000+aMissionID);
            aButtonReal->setOpacity(0);
            aButtonReal->setPosition(aMoveRealX,aMoveY);
            aButtonReal->cocos2d::CCMenuItem::setEnabled(false);
            aButtons3->addObject(aButtonReal);
            
            if(mActiveMission == aMissionID)
                PlaceIndicator(aButtonReal);
            //============================================================
            
            aMissionID+=1;
            
            aMoveRealX+=aMoveX+90;
            aPad_1->setTag(i+1000);
            if(mActiveMission==aMissionID){
                getChildByTag(1000)->addChild(aPad_1,1000);
            }
            else{
                getChildByTag(1000)->addChild(aPad_1);
            }
            
            
            if(aMoveY==100)
                aMoveY = 140;
            else
                aMoveY = 100;
        }
        
        lowBar = CCMenu::createWithArray(aButtons3);
        lowBar->setTag(90);
        lowBar->setPosition(0, 50);
        getChildByTag(1000)->addChild(lowBar, 1);
        
        aMoveRealX = aMoveX+10;
        aButtons3->removeAllObjects();
        
        for(int i=0;i<5;i++)
        {
            CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
            aPad_1->setAnchorPoint(ccp(0.5,0.0));
            aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
            
            //Now comes the real magic for missions !!!
            aMission = User::getInstance()->getMissionManager().GetMissionByID(aMissionID-1);
            
            //First check if this is not the mission player is standing on !!!
            if(mActiveMission==aMissionID)
            {
                //Add player chars here !!!
                mDwarfMove = CCSprite::create("Interfeiss/missions/mission_dwarfs.png");
                mDwarfMove->setPosition(ccp(aPad_1->getContentSize().width/2,aPad_1->getContentSize().height/2+40));
                aPad_1->addChild(mDwarfMove);
            }
            else
            {
                //Check if was not completed
                if(aMissionID<mActiveMission)
                {
                    //This is completed
                    aDummy = CCSprite::create("Interfeiss/missions/mission_check.png");
                    aDummy->setPosition(ccp(aPad_1->getContentSize().width/2+10,aPad_1->getContentSize().height/2+40));
                    aPad_1->addChild(aDummy);
                }
                else
                {
                    if(aMission.unlock_type>0)
                    {
                        //Try to show what unlocks here !!!
                        aPad_1 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aMissionID-1).c_str());
                        aPad_1->setAnchorPoint(ccp(0.5,0.0));
                        aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
                    }
                    else
                    {
                        mission_num = CCLabelTTF::create(toString(aMissionID).c_str(),
                                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                                         CCSize(120, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
                        mission_num->setColor(ccc3(251, 255, 195));
                        mission_num->setPositionX(aPad_1->getContentSize().width/2);
                        mission_num->setPositionY(aPad_1->getContentSize().height/2+30);
                        mission_num->setTag(100);
                        aPad_1->addChild(mission_num);
                    }
                }
            }
            
            //============================================================
            //Add button above?
            aButtonReal = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                  "Interfeiss/missions/MissionSubClick.png",
                                                  this,
                                                  menu_selector(MissionScene::OnClickedMission));
            aButtonReal->setTag(5000+aMissionID);
            aButtonReal->setOpacity(0);
            aButtonReal->setPosition(aMoveRealX,aMoveY);
            aButtonReal->cocos2d::CCMenuItem::setEnabled(false);
            aButtons3->addObject(aButtonReal);
            
            if(mActiveMission == aMissionID)
                PlaceIndicator(aButtonReal);
            //============================================================
            
            aMissionID+=1;
            
            aMoveRealX+=aMoveX+90;
            aPad_1->setTag(i+1000);
            
            if(mActiveMission==aMissionID){
                getChildByTag(1001)->addChild(aPad_1,1000);
            }
            else{
                getChildByTag(1001)->addChild(aPad_1);
            }
            
            
            if(aMoveY==100)
                aMoveY = 140;
            else
                aMoveY = 100;
        }
        
        lowBar = CCMenu::createWithArray(aButtons3);
        lowBar->setTag(90);
        lowBar->setPosition(0, 50);
        getChildByTag(1001)->addChild(lowBar, 1);
    }
    else
    {
        //The magic place !!!
        aMissionID = (mCurrentPage-2)*5+1-mExtraOffsetPage;
        aButtons3->removeAllObjects();
        
        if(mCurrentPage-1 % 2 == 1)
            aMoveY = 100;
        else
            aMoveY = 140;
        
        for(int i=0;i<5;i++)
        {
            CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
            aPad_1->setAnchorPoint(ccp(0.5,0.0));
            aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
            
            //Now comes the real magic for missions !!!
            aMission = User::getInstance()->getMissionManager().GetMissionByID(aMissionID-1);
            
            //First check if this is not the mission player is standing on !!!
            if(mActiveMission==aMissionID)
            {
                //Add player chars here !!!
                mDwarfMove = CCSprite::create("Interfeiss/missions/mission_dwarfs.png");
                mDwarfMove->setPosition(ccp(aPad_1->getContentSize().width/2,aPad_1->getContentSize().height/2+40));
                aPad_1->addChild(mDwarfMove);
            }
            else
            {
                //Check if was not completed
                if(aMissionID<mActiveMission)
                {
                    //This is completed
                    aDummy = CCSprite::create("Interfeiss/missions/mission_check.png");
                    aDummy->setPosition(ccp(aPad_1->getContentSize().width/2+10,aPad_1->getContentSize().height/2+40));
                    aPad_1->addChild(aDummy);
                }
                else
                {
                    if(aMission.unlock_type>0)
                    {
                        //Try to show what unlocks here !!!
                        aPad_1 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aMissionID-1).c_str());
                        aPad_1->setAnchorPoint(ccp(0.5,0.0));
                        aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
                    }
                    else
                    {
                        mission_num = CCLabelTTF::create(toString(aMissionID).c_str(),
                                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                                         CCSize(120, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
                        mission_num->setColor(ccc3(251, 255, 195));
                        mission_num->setPositionX(aPad_1->getContentSize().width/2);
                        mission_num->setPositionY(aPad_1->getContentSize().height/2+30);
                        mission_num->setTag(100);
                        aPad_1->addChild(mission_num);
                    }
                }
            }
            
            //============================================================
            //Add button above?
            aButtonReal = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                  "Interfeiss/missions/MissionSubClick.png",
                                                  this,
                                                  menu_selector(MissionScene::OnClickedMission));
            aButtonReal->setTag(5000+aMissionID);
            aButtonReal->setOpacity(0);
            aButtonReal->setPosition(aMoveRealX,aMoveY);
            aButtonReal->cocos2d::CCMenuItem::setEnabled(false);
            aButtons3->addObject(aButtonReal);
            
            if(mActiveMission == aMissionID)
                PlaceIndicator(aButtonReal);
            //============================================================
            
            aMissionID+=1;
            
            aMoveRealX+=aMoveX+90;
            aPad_1->setTag(i+1000);
            if(mActiveMission==aMissionID){
                getChildByTag(1000)->addChild(aPad_1,1000);
            }
            else{
                getChildByTag(1000)->addChild(aPad_1);
            }
            
            
            if(aMoveY==100)
                aMoveY = 140;
            else
                aMoveY = 100;
        }
        
        lowBar = CCMenu::createWithArray(aButtons3);
        lowBar->setTag(90);
        lowBar->setPosition(0, 50);
        getChildByTag(1000)->addChild(lowBar, 1);
        
        //-----------------------------------------------------------------
        aMoveRealX = aMoveX+10;
        aButtons3->removeAllObjects();
        
        for(int i=0;i<5;i++)
        {
            CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
            aPad_1->setAnchorPoint(ccp(0.5,0.0));
            aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
            
            //Now comes the real magic for missions !!!
            aMission = User::getInstance()->getMissionManager().GetMissionByID(aMissionID-1);
            
            //First check if this is not the mission player is standing on !!!
            if(mActiveMission==aMissionID)
            {
                //Add player chars here !!!
                mDwarfMove = CCSprite::create("Interfeiss/missions/mission_dwarfs.png");
                mDwarfMove->setPosition(ccp(aPad_1->getContentSize().width/2,aPad_1->getContentSize().height/2+40));
                aPad_1->addChild(mDwarfMove);
            }
            else
            {
                //Check if was not completed
                if(aMissionID<mActiveMission)
                {
                    //This is completed
                    aDummy = CCSprite::create("Interfeiss/missions/mission_check.png");
                    aDummy->setPosition(ccp(aPad_1->getContentSize().width/2+10,aPad_1->getContentSize().height/2+40));
                    aPad_1->addChild(aDummy);
                }
                else
                {
                    if(aMission.unlock_type>0)
                    {
                        //Try to show what unlocks here !!!
                        aPad_1 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aMissionID-1).c_str());
                        aPad_1->setAnchorPoint(ccp(0.5,0.0));
                        aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
                    }
                    else
                    {
                        mission_num = CCLabelTTF::create(toString(aMissionID).c_str(),
                                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                                         CCSize(120, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
                        mission_num->setColor(ccc3(251, 255, 195));
                        mission_num->setPositionX(aPad_1->getContentSize().width/2);
                        mission_num->setPositionY(aPad_1->getContentSize().height/2+30);
                        mission_num->setTag(100);
                        aPad_1->addChild(mission_num);
                    }
                }
            }
            
            //============================================================
            //Add button above?
            aButtonReal = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                  "Interfeiss/missions/MissionSubClick.png",
                                                  this,
                                                  menu_selector(MissionScene::OnClickedMission));
            aButtonReal->setTag(5000+aMissionID);
            aButtonReal->setOpacity(0);
            aButtonReal->setPosition(aMoveRealX,aMoveY);
            aButtonReal->cocos2d::CCMenuItem::setEnabled(false);
            aButtons3->addObject(aButtonReal);
            
            if(mActiveMission == aMissionID)
                PlaceIndicator(aButtonReal);
            //============================================================
            
            aMissionID+=1;
            
            aMoveRealX+=aMoveX+90;
            aPad_1->setTag(i+1000);
            if(mActiveMission==aMissionID){
                getChildByTag(1001)->addChild(aPad_1,1000);
            }
            else{
                getChildByTag(1001)->addChild(aPad_1);
            }
            
            
            if(aMoveY==100)
                aMoveY = 140;
            else
                aMoveY = 100;
        }
        
        lowBar = CCMenu::createWithArray(aButtons3);
        lowBar->setTag(90);
        lowBar->setPosition(0, 50);
        getChildByTag(1001)->addChild(lowBar, 1);
        
        //-----------------------------------------------------------------
        aMoveRealX = aMoveX+10;
        aButtons3->removeAllObjects();
        
        for(int i=0;i<5;i++)
        {
            CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
            aPad_1->setAnchorPoint(ccp(0.5,0.0));
            aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
            
            //Now comes the real magic for missions !!!
            aMission = User::getInstance()->getMissionManager().GetMissionByID(aMissionID-1);
            
            //First check if this is not the mission player is standing on !!!
            if(mActiveMission==aMissionID)
            {
                //Add player chars here !!!
                mDwarfMove = CCSprite::create("Interfeiss/missions/mission_dwarfs.png");
                mDwarfMove->setPosition(ccp(aPad_1->getContentSize().width/2,aPad_1->getContentSize().height/2+40));
                aPad_1->addChild(mDwarfMove);
            }
            else
            {
                //Check if was not completed
                if(aMissionID<mActiveMission)
                {
                    //This is completed
                    aDummy = CCSprite::create("Interfeiss/missions/mission_check.png");
                    aDummy->setPosition(ccp(aPad_1->getContentSize().width/2+10,aPad_1->getContentSize().height/2+40));
                    aPad_1->addChild(aDummy);
                }
                else
                {
                    if(aMission.unlock_type>0)
                    {
                        //Try to show what unlocks here !!!
                        aPad_1 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aMissionID-1).c_str());
                        aPad_1->setAnchorPoint(ccp(0.5,0.0));
                        aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
                    }
                    else
                    {
                        mission_num = CCLabelTTF::create(toString(aMissionID).c_str(),
                                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                                         CCSize(120, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
                        mission_num->setColor(ccc3(251, 255, 195));
                        mission_num->setPositionX(aPad_1->getContentSize().width/2);
                        mission_num->setPositionY(aPad_1->getContentSize().height/2+30);
                        mission_num->setTag(100);
                        aPad_1->addChild(mission_num);
                    }
                }
            }
            
            //============================================================
            //Add button above?
            aButtonReal = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                  "Interfeiss/missions/MissionSubClick.png",
                                                  this,
                                                  menu_selector(MissionScene::OnClickedMission));
            aButtonReal->setTag(5000+aMissionID);
            aButtonReal->setOpacity(0);
            aButtonReal->setPosition(aMoveRealX,aMoveY);
            aButtonReal->cocos2d::CCMenuItem::setEnabled(false);
            aButtons3->addObject(aButtonReal);
            
            if(mActiveMission == aMissionID)
                PlaceIndicator(aButtonReal);
            //============================================================
            
            aMissionID+=1;
            
            aMoveRealX+=aMoveX+90;
            aPad_1->setTag(i+1000);
            if(mActiveMission==aMissionID){
                getChildByTag(1002)->addChild(aPad_1,1000);
            }
            else{
                getChildByTag(1002)->addChild(aPad_1);
            }
            
            
            if(aMoveY==100)
                aMoveY = 140;
            else
                aMoveY = 100;
        }
        
        lowBar = CCMenu::createWithArray(aButtons3);
        lowBar->setTag(90);
        lowBar->setPosition(0, 50);
        getChildByTag(1002)->addChild(lowBar, 1);
    }
    
    OnSelectMission(mActiveMission-1);
    
    return;
    
    mActiveMission = User::getInstance()->getMissionManager().GetActiveMissionID();
    
    mMissionPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
    mMissionPad_1->setAnchorPoint(ccp(0.5,0.0));
    mMissionPad_1->setPositionY(aMoveY);
    mMissionPad_1->setPositionX(aMoveRealX);
//    mMissionPad_1->setScale(0);
    addChild(mMissionPad_1);
    
    if(mActiveMission > 0)
        aNeedDone = true;
    
    if(aNeedDone)
    {
        SpriteAnimation* aCompletedAnim = SpriteAnimation::create("Interfeiss/missions/mission_checl.plist",false);
        aCompletedAnim->retain();
        aCompletedAnim->setPosition(ccp(aCompletedAnim->getContentSize().width/2+20,aCompletedAnim->getContentSize().height/2+20));
        mMissionPad_1->addChild(aCompletedAnim);
    }
    aNeedDone = false;
    
    //Add some click zone for this magic !!! TODO add the holders to the background !!!
//    MissionSubClick - TODO make move !!!
    CCMenuItemImage* button_1 = CCMenuItemImage::create(
                                                        "Interfeiss/missions/MissionSubClick.png",
                                                        "Interfeiss/missions/MissionSubClick.png",
                                                        this,
                                                        menu_selector(MissionScene::OnClickedMission));
    button_1->setTag(0);
    button_1->setOpacity(0);
    
    CCMenuItemImage* button_2 = CCMenuItemImage::create(
                                                        "Interfeiss/missions/MissionSubClick.png",
                                                        "Interfeiss/missions/MissionSubClick.png",
                                                        this,
                                                        menu_selector(MissionScene::OnClickedMission));
    button_2->setTag(1);
    button_2->setOpacity(0);
    
    CCMenuItemImage* button_3 = CCMenuItemImage::create(
                                                        "Interfeiss/missions/MissionSubClick.png",
                                                        "Interfeiss/missions/MissionSubClick.png",
                                                        this,
                                                        menu_selector(MissionScene::OnClickedMission));
    button_3->setTag(2);
    button_3->setOpacity(0);
    
    CCMenuItemImage* button_4 = CCMenuItemImage::create(
                                                        "Interfeiss/missions/MissionSubClick.png",
                                                        "Interfeiss/missions/MissionSubClick.png",
                                                        this,
                                                        menu_selector(MissionScene::OnClickedMission));
    button_4->setTag(3);
    button_4->setOpacity(0);
    
    CCMenuItemImage* button_5 = CCMenuItemImage::create(
                                                        "Interfeiss/missions/MissionSubClick.png",
                                                        "Interfeiss/missions/MissionSubClick.png",
                                                        this,
                                                        menu_selector(MissionScene::OnClickedMission));
    button_5->setTag(4);
    button_5->setOpacity(0);
    
    lowBar = CCMenu::create(button_1, button_2,button_3,button_4,button_5,NULL);
    lowBar->alignItemsHorizontallyWithPadding(60);
    lowBar->setPosition(mScreenSize.width/2, 160);
    this->addChild(lowBar, 1);
    
    //Add futher
    aMoveRealX+=aMoveX+90;
    
    mMissionPad_2 = CCSprite::create("Interfeiss/missions/mission_pad.png");
    mMissionPad_2->setAnchorPoint(ccp(0.5,0.0));
    mMissionPad_2->setPositionY(aMoveY);
    mMissionPad_2->setPositionX(aMoveRealX);
//    mMissionPad_2->setScale(0);
    addChild(mMissionPad_2);
    
    if(mActiveMission >= 1)
        aNeedText = false;
    
    if(aNeedText)
    {
        mission_num = CCLabelTTF::create("2",
                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                         CCSize(100, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        mission_num->setColor(ccc3(255, 233, 0));
        mission_num->setPositionX(mMissionPad_2->getContentSize().width/2);
        mission_num->setPositionY(mMissionPad_2->getContentSize().height/2+30);
        mission_num->setTag(100);
        mMissionPad_2->addChild(mission_num);
    }
    
    if(mActiveMission > 1)
        aNeedDone = true;
    
    if(aNeedDone)
    {
        SpriteAnimation* aCompletedAnim = SpriteAnimation::create("Interfeiss/missions/mission_checl.plist",false);
        aCompletedAnim->retain();
        aCompletedAnim->setPosition(ccp(aCompletedAnim->getContentSize().width/2+20,aCompletedAnim->getContentSize().height/2+20));
        mMissionPad_2->addChild(aCompletedAnim);
    }
    aNeedDone = false;
    
    //Add futher
    aMoveRealX+=aMoveX+90;
    aNeedText = true;
    
    mMissionPad_3 = CCSprite::create("Interfeiss/missions/mission_pad.png");
    mMissionPad_3->setAnchorPoint(ccp(0.5,0.0));
    mMissionPad_3->setPositionY(aMoveY);
    mMissionPad_3->setPositionX(aMoveRealX);
//    mMissionPad_3->setScale(0);
    addChild(mMissionPad_3);
    
    if(mActiveMission >= 2)
        aNeedText = false;
    
    if(aNeedText)
    {
        mission_num = CCLabelTTF::create("3",
                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                         CCSize(100, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        mission_num->setColor(ccc3(255, 233, 0));
        mission_num->setPositionX(mMissionPad_3->getContentSize().width/2);
        mission_num->setPositionY(mMissionPad_3->getContentSize().height/2+30);
        mission_num->setTag(100);
        mMissionPad_3->addChild(mission_num);
    }
    
    if(mActiveMission > 2)
        aNeedDone = true;
    
    if(aNeedDone)
    {
        SpriteAnimation* aCompletedAnim = SpriteAnimation::create("Interfeiss/missions/mission_checl.plist",false);
        aCompletedAnim->retain();
        aCompletedAnim->setPosition(ccp(aCompletedAnim->getContentSize().width/2+20,aCompletedAnim->getContentSize().height/2+20));
        mMissionPad_3->addChild(aCompletedAnim);
    }
    aNeedDone = false;
    
    //Add futher
    aMoveRealX+=aMoveX+90;
    
    if(mActiveMission == 3)
        mMissionPad_4 = CCSprite::create("Interfeiss/missions/mission_pad.png");
    else
        mMissionPad_4 = CCSprite::create("Interfeiss/missions/unlock_mission4.png");
    
    mMissionPad_4->setAnchorPoint(ccp(0.5,0.0));
    mMissionPad_4->setPositionY(aMoveY);
    mMissionPad_4->setPositionX(aMoveRealX);
//    mMissionPad_4->setScale(0);
    addChild(mMissionPad_4);
    //    if(aNeedText)
    //    {
    //        mission_num = CCLabelTTF::create("4",
    //                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
    //                                         CCSize(60, 60), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    //        mission_num->setColor(ccc3(255, 233, 0));
    //        mission_num->setPositionX(mission_pad_2->getContentSize().width/2);
    //        mission_num->setPositionY(50);
    //        mission_num->setTag(100);
    //        mission_pad_4->addChild(mission_num);
    //    }
    
    //Add futher
    aMoveRealX+=aMoveX+90;
    aNeedText = true;
    
    mMissionPad_5 = CCSprite::create("Interfeiss/missions/mission_pad.png");
    mMissionPad_5->setAnchorPoint(ccp(0.5,0.0));
    mMissionPad_5->setPositionY(aMoveY);
    mMissionPad_5->setPositionX(aMoveRealX);
//    mMissionPad_5->setScale(0);
    if(aNeedText)
    {
        mission_num = CCLabelTTF::create("5",
                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                         CCSize(100, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        mission_num->setColor(ccc3(255, 233, 0));
        mission_num->setPositionX(mMissionPad_5->getContentSize().width/2);
        mission_num->setPositionY(mMissionPad_5->getContentSize().height/2+30);
        mission_num->setTag(100);
        mMissionPad_5->addChild(mission_num);
    }
    addChild(mMissionPad_5);
    
    
//    CCDelayTime* aTime = CCDelayTime::create(1.0f);
//    CCCallFuncN* aFunction = CCCallFuncN::create(this, callfuncN_selector(LevelEndMissions::GrowPads));
//    CCSequence* aSeq = CCSequence::create(aTime,aFunction,NULL);
//    runAction(aSeq);
    
    mMissionWalker = CCSprite::create("Interfeiss/missions/Dwarf_Move_Fig.png");
    mMissionWalker->setScale(0.7);
    
    //Check on what mission should be the walker
    int aWalkerX = 0;
    if(mActiveMission == 0)
        aWalkerX = mMissionPad_1->getPositionX();
    else if(mActiveMission == 1)
        aWalkerX = mMissionPad_2->getPositionX();
    else if(mActiveMission == 2)
        aWalkerX = mMissionPad_3->getPositionX();
    else if(mActiveMission == 3)
        aWalkerX = mMissionPad_4->getPositionX();
    
    mMissionWalker->setPositionX(aWalkerX);
    mMissionWalker->setPositionY(mMissionPad_1->getPositionY()+mMissionPad_1->getContentSize().height/2+50);
//    mMissionWalker->setScale(0);
    addChild(mMissionWalker);
    
    //Get the last active mission
    OnSelectMission(mActiveMission);
}

void MissionScene::RemoveIndicator(CCNode* sender)
{
    if(sender->getParent())
        sender->removeFromParent();
}

void MissionScene::PlaceIndicator(CCMenuItemImage* theSprite)
{
    if(User::getInstance()->mSpecialMissionBuild)
    {
        //Ignore it !!!
    }
    else
    {
        if(theSprite->getChildByTag(777))
            return;
    }
    
    //Remove from the old !!!
    if(mPrevIndicator!=NULL)
    {
        CCScaleTo* aRemoeScale = CCScaleTo::create(0.25f,0.0f);
        CCEaseIn* aRemoveEase = CCEaseIn::create(aRemoeScale,0.25f);//(aRemoeScale);
        CCCallFuncN* aFunc = CCCallFuncN::create(this, callfuncN_selector(MissionScene::RemoveIndicator));
        CCSequence* aRemoveSeq = CCSequence::create(aRemoveEase,aFunc,NULL);
        mPrevIndicator->runAction(aRemoveSeq);
        
        mPrevIndicator = NULL;
    }
    
    mPrevIndicator = CCSprite::create("Interfeiss/missions/notifikators.png");
    mPrevIndicator->setScale(0.0f);
    mPrevIndicator->setPosition(ccp(theSprite->getContentSize().width/2,theSprite->getContentSize().height+45));
    mPrevIndicator->setTag(777);
    
    CCScaleTo* aRemoeScale1 = CCScaleTo::create(0.25f,1.0f);
    CCEaseIn* aRemoveEase1 = CCEaseIn::create(aRemoeScale1,0.25f);
    mPrevIndicator->runAction(aRemoveEase1);
    
    theSprite->addChild(mPrevIndicator);
}

void MissionScene::OnSelectMission(int theMissionID)
{
    mLastClickedMission = theMissionID;
    
    return;
    if(User::getInstance()->mNewMissionBuild)
    {
        //Check if this is not the 10 mission then erase all progress !!!
        if(theMissionID == 9){
            //Erase all to start !!!
            User::getInstance()->mNewMissionProgress = 1;
            
            User::getInstance()->mSpecialMissionBuild = false;
            User::getInstance()->mSpecialMissionProgress = 0;
            User::getInstance()->mShowNewPart = false;
            
            //Better restart
            User::getInstance()->ResetData();
            
            //Dwarf crash always is disabled for now !!!
            User::getInstance()->_tutorial_small_3 = 1;
            
            //Move pages to 0
            mCurrentPage = 1;
            mLastPage = 1;
            mMaxPages = 5;
            
            mPrevIndicator = NULL;
            
            mMoveBackScroll = false;
            
            mLastClickedMission = 0;
            
            mExtraOffsetPage = 0;
            
            CCScene* mainMenu = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
            CCTransitionFade* transition = CCTransitionFade::create(0.25f,mainMenu,ccBLACK);
            CCDirector::sharedDirector()->replaceScene(transition);
        }
        
        if(theMissionID >= 15 && theMissionID<=25){
            mLastClickedMission = theMissionID+1;
        }
        
        return;
    }
    
    /*
    
    //update the mission base !!!
    MissionSet aMission = User::getInstance()->getMissionManager().GetMissionByID(theMissionID);
    //lets update all the stuff here !!!
    
//    CCScrollView
    
    mLastClickedMission = theMissionID+1;
    
    std::stringstream theLog;
    theLog<<"Menu Mission Click  #"<<(aMission.ID);
    std::string aMissionLog = theLog.str();
    
    AnalyticX::flurryLogEvent(aMissionLog.c_str());
    
    int aMissionAmount = aMission.mSubMission_1.size();
    
    int aMissionOffsetX = 0;
    if(aMissionAmount == 2)
        aMissionOffsetX = -25;
    else if(aMissionAmount == 1)
        aMissionOffsetX = -50;
    
    //The mission name !!!
    std::stringstream theMission;
    theMission<<"Mission set #"<<(aMission.ID);
    std::string aMissionFinal = theMission.str();
    
    mMissionBox_Header->setString(aMissionFinal.c_str());
    
    //The 1st mission info
    std::vector<SubMission> aSubMiss = aMission.mSubMission_1;
    if(aSubMiss.size()==0)
        return;//Do not show anythinh
    
    //Check if this is not over current mission set
    if(theMissionID > User::getInstance()->getMissionManager().GetActiveMissionID())
    {
        //Hide reward and move play button a bit up
        if(mMissionRewardBox->getChildByTag(1111))
            mMissionRewardBox->removeChildByTag(1111);
        
        if(mMissionRewardBox->getChildByTag(1112))
            mMissionRewardBox->removeChildByTag(1112);
        
        mMissionRewardBox->setVisible(false);
        
        getChildByTag(778)->setPosition(mMissionRewardBox->getPositionX(),
                              mMissionRewardBox->getPositionY()-mMissionRewardBox->getContentSize().height-135/2+78);
        
        mMissionBox_Check1_On->setVisible(false);
        mMissionBox_Check1_Off->setVisible(false);
        mMissionBox_Text1->setVisible(false);
        mMissionBox_Text1p->setVisible(false);
        
        mMissionBox_Check2_On->setVisible(false);
        mMissionBox_Check2_Off->setVisible(false);
        mMissionBox_Text2->setVisible(false);
        mMissionBox_Text2p->setVisible(false);
        
        mMissionBox_Check3_On->setVisible(false);
        mMissionBox_Check3_Off->setVisible(false);
        mMissionBox_Text3->setVisible(false);
        mMissionBox_Text3p->setVisible(false);
        
        //Show the complete prev mission
        theMission.str("");
        theMission.clear();
        
        theMission<<"Complete\nMission set #"<<(theMissionID);
        
        mMissionBox_CompletPrev->setString(theMission.str().c_str());
        mMissionBox_CompletPrev->setVisible(true);
        
        return;
    }
    else
    {
        mMissionBox_CompletPrev->setVisible(false);
        
        getChildByTag(778)->setPosition(mMissionRewardBox->getPositionX(),
                                        mMissionRewardBox->getPositionY()-mMissionRewardBox->getContentSize().height-135/2+15);
        
        mMissionRewardBox->setVisible(true);
        ShowRewardInfo(theMissionID);
    }
    
    std::string aName = aSubMiss[0].name;
//    std::stringstream theMissionProgress;
    
    //The checkboxes
    
    mMissionBox_Check1_Off->setPositionY(aBox1_StartY+aMissionOffsetX);
    mMissionBox_Check1_On->setPositionY(aBox1_StartY+aMissionOffsetX);
//    mMissionBox_Text1->setPositionY(aText1_StartY+aMissionOffsetX);
    
    mMissionBox_Text1->setPositionX(mMissionBox_Check1_On->getPositionX()+40);
    mMissionBox_Text1->setPositionY(mMissionBox_Check1_On->getPositionY()+10);
    
    mMissionBox_Text1p->setPositionX(mMissionBox_Check1_On->getPositionX()+40);
    mMissionBox_Text1p->setPositionY(mMissionBox_Check1_On->getPositionY()-17);
    
    mMissionBox_Text1->setVisible(true);
    mMissionBox_Text1p->setVisible(true);
    
    mMissionBox_Check1_On->setVisible(false);
    mMissionBox_Check1_Off->setVisible(false);
    if(aSubMiss[0].current_amount>=aSubMiss[0].amount || aSubMiss[0].best_amount>=aSubMiss[0].amount)
        mMissionBox_Check1_On->setVisible(true);
    else
        mMissionBox_Check1_Off->setVisible(true);
    //the text of mission 1
    mMissionBox_Text1->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,0,false,true).c_str());
    mMissionBox_Text1p->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,0,false,false).c_str());
    //------------------------------------------------------
    
    //The 2nd mission info
    if(aMissionAmount>1)
    {
        aSubMiss = aMission.mSubMission_1;
        aName = aSubMiss[1].name;
        
        mMissionBox_Check2_Off->setPositionY(aBox2_StartY+aMissionOffsetX);
        mMissionBox_Check2_On->setPositionY(aBox2_StartY+aMissionOffsetX);
//        mMissionBox_Text2->setPositionY(aText2_StartY+aMissionOffsetX);
        
        mMissionBox_Text2->setPositionX(mMissionBox_Check2_On->getPositionX()+40);
        mMissionBox_Text2->setPositionY(mMissionBox_Check2_On->getPositionY()+10);
        
        mMissionBox_Text2p->setPositionX(mMissionBox_Check2_On->getPositionX()+40);
        mMissionBox_Text2p->setPositionY(mMissionBox_Check2_On->getPositionY()-17);
        
        //The checkboxes
        mMissionBox_Check2_On->setVisible(false);
        mMissionBox_Check2_Off->setVisible(false);
        if(aSubMiss[1].current_amount>=aSubMiss[1].amount || aSubMiss[1].best_amount>=aSubMiss[1].amount)
            mMissionBox_Check2_On->setVisible(true);
        else
            mMissionBox_Check2_Off->setVisible(true);
        //the text of mission 1
        mMissionBox_Text2->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,1,false,true).c_str());
        mMissionBox_Text2p->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,1,false,false).c_str());
        mMissionBox_Text2->setVisible(true);
        mMissionBox_Text2p->setVisible(true);
        
        //------------------------------------------------------
    }
    else
    {
        mMissionBox_Check2_On->setVisible(false);
        mMissionBox_Check2_Off->setVisible(false);
        mMissionBox_Text2->setVisible(false);
        mMissionBox_Text2p->setVisible(false);
    }
    
    if(aMissionAmount>2)
    {
        mMissionBox_Check3_Off->setPositionY(aBox3_StartY+aMissionOffsetX);
        mMissionBox_Check3_On->setPositionY(aBox3_StartY+aMissionOffsetX);
//        mMissionBox_Text3->setPositionY(aText3_StartY+aMissionOffsetX);
        
        mMissionBox_Text3->setPositionX(mMissionBox_Check3_On->getPositionX()+40);
        mMissionBox_Text3->setPositionY(mMissionBox_Check3_On->getPositionY()+10);
        
        mMissionBox_Text3p->setPositionX(mMissionBox_Check3_On->getPositionX()+40);
        mMissionBox_Text3p->setPositionY(mMissionBox_Check3_On->getPositionY()-17);
        
        //The 3rd mission info
        aSubMiss = aMission.mSubMission_1;
        aName = aSubMiss[2].name;
        
        //The checkboxes
        mMissionBox_Check3_On->setVisible(false);
        mMissionBox_Check3_Off->setVisible(false);
        if(aSubMiss[2].current_amount>=aSubMiss[2].amount || aSubMiss[2].best_amount>=aSubMiss[2].amount)
            mMissionBox_Check3_On->setVisible(true);
        else
            mMissionBox_Check3_Off->setVisible(true);
        //the text of mission 1
        mMissionBox_Text3->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,2,false,true).c_str());
        mMissionBox_Text3p->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,2,false,false).c_str());
        mMissionBox_Text3->setVisible(true);
        mMissionBox_Text3p->setVisible(true);
        //------------------------------------------------------
    }
    else
    {
        mMissionBox_Check3_On->setVisible(false);
        mMissionBox_Check3_Off->setVisible(false);
        mMissionBox_Text3->setVisible(false);
        mMissionBox_Text3p->setVisible(false);
    }
    
    */
    
    //The reward
//    theMission.str("");
//    theMission<<(aMission.reward_name);
//    aMissionFinal = theMission.str();
    
//    mMissionBox_Reward->setString(aMissionFinal.c_str());
}

//When scroll - update to correct missions !!!
void MissionScene::UpdateMissions(bool addPage)
{
    //    mission_pad
    int aMoveY = 100;
    
    //Create the hot spot places !!!
    int aMoveX = (mScreenSize.width/5)/2;
    int aMoveRealX = aMoveX+10;
    
    CCLabelTTF* mission_num;
    int aMissionID = 1;
    
    CCArray* aButtons3 = CCArray::create();
    aButtons3->retain();
    CCMenuItemImage* aButtonReal;
    CCMenu* lowBar;
    
    MissionSet aMission;
    CCSprite* aDummy;
    
    bool _DidAddIndicator = false;
    
//    CCLog("Current Page: %i",mCurrentPage);
    
    if(addPage)
    {
        mExtraOffsetPage = 0;
        
        //To the right
        getChildByTag(1000)->removeAllChildren();//Clear the 1st page
        aMissionID=mCurrentPage*5+1;
        
        if((mCurrentPage-1) % 2 == 1)
            aMoveY = 100;
        else
            aMoveY = 140;
        
        if(mCurrentPage<mMaxPages)
        {
            //Only now we need to create mission pads
            for(int i=0;i<5;i++)
            {
                CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
                aPad_1->setAnchorPoint(ccp(0.5,0.0));
                aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
                
                //Now comes the real magic for missions !!!
                aMission = User::getInstance()->getMissionManager().GetMissionByID(aMissionID-1);
                
                //First check if this is not the mission player is standing on !!!
                if(mActiveMission==aMissionID)
                {
                    //Add player chars here !!!
                    mDwarfMove = CCSprite::create("Interfeiss/missions/mission_dwarfs.png");
                    mDwarfMove->setPosition(ccp(aPad_1->getContentSize().width/2,aPad_1->getContentSize().height/2+40));
                    aPad_1->addChild(mDwarfMove);
                }
                else
                {
                    //Check if was not completed
                    if(aMissionID<mActiveMission)
                    {
                        //This is completed
                        aDummy = CCSprite::create("Interfeiss/missions/mission_check.png");
                        aDummy->setPosition(ccp(aPad_1->getContentSize().width/2+10,aPad_1->getContentSize().height/2+40));
                        aPad_1->addChild(aDummy);
                    }
                    else
                    {
                        if(aMission.unlock_type>0)
                        {
                            //Try to show what unlocks here !!!
                            aPad_1 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aMissionID-1).c_str());
                            aPad_1->setAnchorPoint(ccp(0.5,0.0));
                            aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
                        }
                        else
                        {
                            mission_num = CCLabelTTF::create(toString(aMissionID).c_str(),
                                                             FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                                             CCSize(120, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
                            mission_num->setColor(ccc3(251, 255, 195));
                            mission_num->setPositionX(aPad_1->getContentSize().width/2);
                            mission_num->setPositionY(aPad_1->getContentSize().height/2+30);
                            mission_num->setTag(100);
                            aPad_1->addChild(mission_num);
                        }
                    }
                }
                
                //============================================================
                //Add button above?
                aButtonReal = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                      "Interfeiss/missions/MissionSubClick.png",
                                                      this,
                                                      menu_selector(MissionScene::OnClickedMission));
                aButtonReal->setTag(5000+aMissionID);
                aButtonReal->setOpacity(0);
                aButtonReal->setPosition(aMoveRealX,aMoveY);
                aButtonReal->cocos2d::CCMenuItem::setEnabled(false);
                aButtons3->addObject(aButtonReal);
                
                if(mActiveMission == aMissionID)
                {
                    _DidAddIndicator = true;
                    PlaceIndicator(aButtonReal);
                }
                //============================================================
                
                aMissionID+=1;
                
                aMoveRealX+=aMoveX+90;
                aPad_1->setTag(i+1000);
                if(mActiveMission==aMissionID){
                    getChildByTag(1000)->addChild(aPad_1,1000);
                }
                else{
                    getChildByTag(1000)->addChild(aPad_1);
                }
                
                
                if(aMoveY==100)
                    aMoveY = 140;
                else
                    aMoveY = 100;
            }
            
            lowBar = CCMenu::createWithArray(aButtons3);
            lowBar->setTag(90);
            lowBar->setPosition(0, 50);
            getChildByTag(1000)->addChild(lowBar, 1);
        }
    }
    else
    {
        mExtraOffsetPage = 0;
        
        getChildByTag(1002)->removeAllChildren();//Clear the 1st page
        aMissionID=(mCurrentPage-2)*5+1;
        
        if((mCurrentPage-1) % 2 == 1)
            aMoveY = 100;
        else
            aMoveY = 140;
        
        //Check whats going on here
        if(mCurrentPage>1)
        {
            //Only now we need to create mission pads
            for(int i=0;i<5;i++)
            {
                CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
                aPad_1->setAnchorPoint(ccp(0.5,0.0));
                aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
                
                //Now comes the real magic for missions !!!
                aMission = User::getInstance()->getMissionManager().GetMissionByID(aMissionID-1);
                
                //First check if this is not the mission player is standing on !!!
                if(mActiveMission==aMissionID)
                {
                    //Add player chars here !!!
                    mDwarfMove = CCSprite::create("Interfeiss/missions/mission_dwarfs.png");
                    mDwarfMove->setPosition(ccp(aPad_1->getContentSize().width/2,aPad_1->getContentSize().height/2+40));
                    aPad_1->addChild(mDwarfMove);
                }
                else
                {
                    //Check if was not completed
                    if(aMissionID<mActiveMission)
                    {
                        //This is completed
                        aDummy = CCSprite::create("Interfeiss/missions/mission_check.png");
                        aDummy->setPosition(ccp(aPad_1->getContentSize().width/2+10,aPad_1->getContentSize().height/2+40));
                        aPad_1->addChild(aDummy);
                    }
                    else
                    {
                        if(aMission.unlock_type>0)
                        {
                            //Try to show what unlocks here !!!
                            aPad_1 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aMissionID-1).c_str());
                            aPad_1->setAnchorPoint(ccp(0.5,0.0));
                            aPad_1->setPosition(ccp(aMoveRealX,aMoveY));
                        }
                        else
                        {
                            mission_num = CCLabelTTF::create(toString(aMissionID).c_str(),
                                                             FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                                             CCSize(120, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
                            mission_num->setColor(ccc3(251, 255, 195));
                            mission_num->setPositionX(aPad_1->getContentSize().width/2);
                            mission_num->setPositionY(aPad_1->getContentSize().height/2+30);
                            mission_num->setTag(100);
                            aPad_1->addChild(mission_num);
                        }
                    }
                }
                
                //============================================================
                //Add button above?
                aButtonReal = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                      "Interfeiss/missions/MissionSubClick.png",
                                                      this,
                                                      menu_selector(MissionScene::OnClickedMission));
                aButtonReal->setTag(5000+aMissionID);
                aButtonReal->setOpacity(0);
                aButtonReal->setPosition(aMoveRealX,aMoveY);
                aButtonReal->cocos2d::CCMenuItem::setEnabled(false);
                aButtons3->addObject(aButtonReal);
                
                if(mActiveMission == aMissionID)
                {
                    _DidAddIndicator = true;
                    PlaceIndicator(aButtonReal);
                }
                //============================================================
                
                aMissionID+=1;
                
                aMoveRealX+=aMoveX+90;
                aPad_1->setTag(i+1000);
                if(mActiveMission == aMissionID){
                    getChildByTag(1002)->addChild(aPad_1,1000);
                }
                else{
                    getChildByTag(1002)->addChild(aPad_1);
                }
                
                
                if(aMoveY==100)
                    aMoveY = 140;
                else
                    aMoveY = 100;
            }
            
            lowBar = CCMenu::createWithArray(aButtons3);
            lowBar->setTag(90);
            lowBar->setPosition(0, 50);
            getChildByTag(1002)->addChild(lowBar, 1);
            
            //Try to add the mission indicator
//            if(lowBar->getChildByTag(5000+mActiveMission)!=NULL)
//            {
//                PlaceIndicator((CCMenuItemImage*)lowBar->getChildByTag(5000+mActiveMission));
//            }
        }
    }
    
    //Clear it !!!
    if(!_DidAddIndicator)
    {
        mPrevIndicator = NULL;
    }
}


//The custom handmade scroll for missions by Kristaps :)

void MissionScene::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
//        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/main_music.ogg", true);
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
//        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/main_music.mp3", true);
//#endif
    

}

void MissionScene::NewMissionCompleted()
{
    CCLayerColor* aLayerFake = CCLayerColor::create(ccc4(0,0,0,64),mScreenSize.width,mScreenSize.height);
    aLayerFake->setTag(2014);
    addChild(aLayerFake,1000);
    
    CCSprite* aSpriteDum = CCSprite::create("Interfeiss/challenges/complete_popup/dont_leave.png");
    aLayerFake->addChild(aSpriteDum);
    aSpriteDum->setPosition(ccp(aLayerFake->getContentSize().width/2,aLayerFake->getContentSize().height/2));
    
    //Add some text !!!
    cocos2d::CCLabelTTF* _PopText;
    //Add the text field !!!
    _PopText = CCLabelTTF::create("",
                                  "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                  CCSize(400, 280), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    _PopText->setAnchorPoint(ccp(0.5,0.5f));
    _PopText->setColor(ccc3(79, 65, 33));
    _PopText->setPositionX(350);//This can change by reward type
    _PopText->setPositionY(300);
    aSpriteDum->addChild(_PopText);
    
    std::stringstream theTextField;
    
    if(User::getInstance()->mNewMissionBuild){
        
        theTextField<<"You have completed the\n demo version of Dwarf Forest.\nMore missions coming soon!";
        
        //Add some part image !!!
        CCSprite* aDebugPic = CCSprite::create("MissionCompleted.png");
        
        
        aDebugPic->setPosition(ccp(300,-40));
        
        aSpriteDum->addChild(aDebugPic);
    }
    
    _PopText->setString(theTextField.str().c_str());
    
    //Add some button for clouse
    CCMenuItemImage* debug_button_1 = CCMenuItemImage::create(
                                                              "Interfeiss/before_quit/check_btn0001.png",
                                                              "Interfeiss/before_quit/check_btn0002.png",
                                                              this,
                                                              menu_selector(MissionScene::OnClouseFinish));
    
    CCMenu* debugMenu = CCMenu::create(debug_button_1,NULL);
    debugMenu->setPosition(ccp(500,100));
    aSpriteDum->addChild(debugMenu);
}

void MissionScene::OnClouseFinish()
{
    removeChildByTag(2014);
}

void MissionScene::onEnterTransitionDidFinish()
{
    if(User::getInstance()->mSpecialMissionBuild || User::getInstance()->mNewMissionBuild)
    {
        //Check if need to show new part collect !!!
        if(User::getInstance()->mShowNewPart)
        {
            User::getInstance()->mShowNewPart = false;
            
            //If this is mission 5 - then game over
            if(User::getInstance()->mSpecialMissionProgress == 5){
                NewMissionCompleted();
                return;
            }
            
            if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_18_Mission
               || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission
               || User::getInstance()->mSpecial_21_Mission || User::getInstance()->mSpecial_22_Mission
               || User::getInstance()->mSpecial_23_Mission){
                NewMissionCompleted();
                return;
            }
            
            if(User::getInstance()->mSpecial_17_Mission){
                NewMissionCompleted();
                return;
            }
            
            
            //Create some pop to pop some stuff
            CCLayerColor* aLayerFake = CCLayerColor::create(ccc4(0,0,0,64),mScreenSize.width,mScreenSize.height);
            aLayerFake->setTag(2014);
            addChild(aLayerFake,100);
            
            CCSprite* aSpriteDum = CCSprite::create("Interfeiss/upgrade_screen/infopanel.png");
            aLayerFake->addChild(aSpriteDum);
            aSpriteDum->setTag(20);
            aSpriteDum->setPosition(ccp(aLayerFake->getContentSize().width/2,aLayerFake->getContentSize().height/2));
            
            CCLabelTTF* _PopText = CCLabelTTF::create("Tesla",
                                          "fonts/Marker Felt.ttf", TITLE_FONT_SIZE,
                                          CCSize(400, 140), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            _PopText->setAnchorPoint(ccp(0.5,0.5f));
            _PopText->setColor(ccc3(79, 65, 33));
            _PopText->setPositionX(aSpriteDum->getContentSize().width/2);//This can change by reward type
            _PopText->setPositionY(aSpriteDum->getContentSize().height+10);
            aSpriteDum->addChild(_PopText);
            
            //The continue button
            CCMenuItemImage* debug_button_1 = CCMenuItemImage::create(
                                                                      "Interfeiss/before_quit/check_btn0001.png",
                                                                      "Interfeiss/before_quit/check_btn0002.png",
                                                                      this,
                                                                      menu_selector(MissionScene::OnShowNextSpecialMission));
            
            CCMenu* debugMenu = CCMenu::create(debug_button_1,NULL);
            debugMenu->setPosition(ccp(aSpriteDum->getContentSize().width/2,-50));
            aSpriteDum->addChild(debugMenu);
            
            //Fall the item down !!!
            CCSprite* aSpriteDum2;
            CCSprite* aBase;
            
            CCMoveTo* aMove;
            
            if(User::getInstance()->mSpecialMissionProgress>1)
            {
                //Add already some stuff
                aBase = CCSprite::create("powerup/base_big.png");
                aSpriteDum->addChild(aBase);
                aBase->setPosition(ccp(aSpriteDum->getContentSize().width/2,aSpriteDum->getContentSize().height/2));
            }
            if(User::getInstance()->mSpecialMissionProgress>2)
            {
                //Add already some stuff
                aSpriteDum2 = CCSprite::create("powerup/top_big.png");
                aSpriteDum->addChild(aSpriteDum2);
                aSpriteDum2->setPosition(ccp(aBase->getPositionX(),aBase->getPositionY()+100));
            }
            if(User::getInstance()->mSpecialMissionProgress>3)
            {
                //Add already some stuff
                aSpriteDum2 = CCSprite::create("powerup/rats_big.png");
                aSpriteDum->addChild(aSpriteDum2);
                aSpriteDum2->setPosition(ccp(aBase->getPositionX()-100,aBase->getPositionY()));
            }
            if(User::getInstance()->mSpecialMissionProgress>4)
            {
                //Add already some stuff
                aSpriteDum2 = CCSprite::create("powerup/progres_big.png");
                aSpriteDum->addChild(aSpriteDum2);
                aSpriteDum2->setPosition(ccp(aBase->getPositionX()+20,aBase->getPositionY()-10));
            }
            
            
            if(User::getInstance()->mSpecialMissionProgress==1)
            {
                //Drop this
                aSpriteDum2 = CCSprite::create("powerup/base_big.png");
                aSpriteDum2->setPosition(ccp(aSpriteDum->getContentSize().width/2,mScreenSize.height));
                aMove = CCMoveTo::create(0.5f,ccp(aSpriteDum->getContentSize().width/2,aSpriteDum->getContentSize().height/2));
            }
            else if(User::getInstance()->mSpecialMissionProgress==2)
            {
                //Drop this
                aSpriteDum2 = CCSprite::create("powerup/top_big.png");
                aSpriteDum2->setPosition(ccp(aSpriteDum->getContentSize().width/2,mScreenSize.height));
                aMove = CCMoveTo::create(0.5f,ccp(aBase->getPositionX(),aBase->getPositionY()+100));
            }
            else if(User::getInstance()->mSpecialMissionProgress==3)
            {
                //Drop this
                aSpriteDum2 = CCSprite::create("powerup/rats_big.png");
                aSpriteDum2->setPosition(ccp(aSpriteDum->getContentSize().width/2,mScreenSize.height));
                aMove = CCMoveTo::create(0.5f,ccp(aBase->getPositionX()-100,aBase->getPositionY()));
            }
            else if(User::getInstance()->mSpecialMissionProgress==4)
            {
                //Drop this
                aSpriteDum2 = CCSprite::create("powerup/progres_big.png");
                aSpriteDum2->setPosition(ccp(aSpriteDum->getContentSize().width/2,mScreenSize.height));
                aMove = CCMoveTo::create(0.5f,ccp(aBase->getPositionX()+20,aBase->getPositionY()-10));
                
                CCDelayTime* aDelay = CCDelayTime::create(0.5f);
                CCCallFuncN* aFUnc = CCCallFuncN::create(this, callfuncN_selector(MissionScene::OnBlitz));
                CCSequence* aSeq1 = CCSequence::create(aDelay,aFUnc,NULL);
                aSpriteDum->runAction(aSeq1);
            }
            
            CCEaseBackOut* aEase1 = CCEaseBackOut::create(aMove);
            aSpriteDum2->runAction(aEase1);
            aSpriteDum->addChild(aSpriteDum2);
            
        }
    }
}

void MissionScene::OnBlitz()
{
    CCSprite* aspr = (CCSprite*)getChildByTag(2014)->getChildByTag(20);
    
    if(aspr != NULL)
    {
        //Add extra blitz !!!
        SpriteAnimation* aBlitz = SpriteAnimation::create("powerup/Machine_Enemies/elektro_strike.plist",false);
        aBlitz->retain();
        aBlitz->setAnchorPoint(ccp(0.5,0));
        aBlitz->setPosition(ccp(aspr->getContentSize().width/2,40));
        aspr->addChild(aBlitz,100);
    }
}

void MissionScene::OnShowNextSpecialMission()
{
//    User::getInstance()->mSpecialMissionProgress = 1;
    //Move the dwarfs?
    removeChildByTag(2014);
    
    if(mDwarfMove != NULL)
    {
        //This is completed
        CCSprite* aPad_1 = (CCSprite*)mDwarfMove->getParent();
        
        CCSprite* aDummy = CCSprite::create("Interfeiss/missions/mission_check.png");
        aDummy->setPosition(ccp(aPad_1->getContentSize().width/2+10,aPad_1->getContentSize().height/2+40));
//        aPad_1->addChild(aDummy);
        
        //Move it a bit up !!!
        CCMoveBy* aMove1;
        
//        mDwarfMove->runAction(aEase1);
        
        //Try to set pad active next !!!
        
        //Check if we have any button under it !!!
        CCMenu* aMenu = (CCMenu*)getChildByTag(1000)->getChildByTag(90);
        CCMenuItem* aItem;
        
        int aMissionID = 1;
        if(User::getInstance()->mSpecialMissionProgress == 1)
        {
            aMove1 = CCMoveBy::create(0.5f,ccp(180,50));
            aMissionID = 1;
            
            CCSprite* aDeletSprite = (CCSprite*)getChildByTag(1001)->getChildByTag(1000);
            if(aDeletSprite){
                aDeletSprite->addChild(aDummy);
            }
            
            aDeletSprite = (CCSprite*)getChildByTag(1001);//->getChildByTag(1001);
            if(aDeletSprite->getChildByTag(1001)!=NULL){
                //The delet obj
                CCSprite* aDelet = (CCSprite*)aDeletSprite->getChildByTag(1001);
                int aPosY = 100;
                int aPosX = aDelet->getPositionX();
                if(aDelet->getPositionY()>100){aPosY = 140;}
                
                aDeletSprite->removeChildByTag(1001);
                
                //Add some basic pad
                CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
                aPad_1->setAnchorPoint(ccp(0.5,0.0));
                aPad_1->setPosition(ccp(aPosX,aPosY));
                aDeletSprite->addChild(aPad_1,0);
            }
            
        }
        else if(User::getInstance()->mSpecialMissionProgress == 2){
            aMove1 = CCMoveBy::create(0.5f,ccp(180,-50));
            aMissionID = 2;
            
            CCSprite* aDeletSprite = (CCSprite*)getChildByTag(1001)->getChildByTag(1001);
            if(aDeletSprite){
                aDeletSprite->addChild(aDummy);
            }
            
            aDeletSprite = (CCSprite*)getChildByTag(1001);//->getChildByTag(1001);
            if(aDeletSprite->getChildByTag(1002)!=NULL){
                //The delet obj
                CCSprite* aDelet = (CCSprite*)aDeletSprite->getChildByTag(1002);
                int aPosY = 100;
                int aPosX = aDelet->getPositionX();
                if(aDelet->getPositionY()>100){aPosY = 140;}
                
                aDeletSprite->removeChildByTag(1002);
                
                //Add some basic pad
                CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
                aPad_1->setAnchorPoint(ccp(0.5,0.0));
                aPad_1->setPosition(ccp(aPosX,aPosY));
                aDeletSprite->addChild(aPad_1,0);
            }
        }
        else if(User::getInstance()->mSpecialMissionProgress == 3){
            aMove1 = CCMoveBy::create(0.5f,ccp(180,50));
            aMissionID = 3;
            
            CCSprite* aDeletSprite = (CCSprite*)getChildByTag(1001)->getChildByTag(1002);
            if(aDeletSprite){
//                aDeletSprite->removeAllChildren();
                aDeletSprite->addChild(aDummy);
            }
            
            aDeletSprite = (CCSprite*)getChildByTag(1001);//->getChildByTag(1001);
            if(aDeletSprite->getChildByTag(1003)!=NULL){
                //The delet obj
                CCSprite* aDelet = (CCSprite*)aDeletSprite->getChildByTag(1003);
                int aPosY = 100;
                int aPosX = aDelet->getPositionX();
                if(aDelet->getPositionY()>100){aPosY = 140;}
                
                aDeletSprite->removeChildByTag(1003);
                
                //Add some basic pad
                CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
                aPad_1->setAnchorPoint(ccp(0.5,0.0));
                aPad_1->setPosition(ccp(aPosX,aPosY));
                aDeletSprite->addChild(aPad_1,0);
            }
        }
        else if(User::getInstance()->mSpecialMissionProgress == 4){
            aMove1 = CCMoveBy::create(0.5f,ccp(180,-50));
            aMissionID = 4;
            
            CCSprite* aDeletSprite = (CCSprite*)getChildByTag(1001)->getChildByTag(1003);
            if(aDeletSprite){
//                aDeletSprite->removeAllChildren();
                aDeletSprite->addChild(aDummy);
            }
            
            aDeletSprite = (CCSprite*)getChildByTag(1001);//->getChildByTag(1001);
            if(aDeletSprite->getChildByTag(1004)!=NULL){
                //The delet obj
                CCSprite* aDelet = (CCSprite*)aDeletSprite->getChildByTag(1004);
                int aPosY = 100;
                int aPosX = aDelet->getPositionX();
                if(aDelet->getPositionY()>100){aPosY = 140;}
                
                aDeletSprite->removeChildByTag(1004);
                
                //Add some basic pad
                CCSprite* aPad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
                aPad_1->setAnchorPoint(ccp(0.5,0.0));
                aPad_1->setPosition(ccp(aPosX,aPosY));
                aDeletSprite->addChild(aPad_1,0);
            }
            
            //Activate tesla - set in game active
//            User::getInstance()->setMachine_EnemiesLvl(1);
        }
        
        aMissionID+=1;
        
        CCEaseElasticInOut* aEase1 = CCEaseElasticInOut::create(aMove1,0.5f);
        mDwarfMove->runAction(aEase1);
        
        if(aMenu)
        {
            for(int i=0;i<5;i++)
            {
                aItem = (CCMenuItem*)aMenu->getChildByTag(5000+aMissionID+i);
                if(aItem)
                {
                    //We have found it !!! Show this mission
                    OnSelectMission(aItem->getTag()-5001);
                    PlaceIndicator((CCMenuItemImage*)aItem);
                    break;
                }
            }
        }
        
        //Check next scene !!!
        aMenu = (CCMenu*)getChildByTag(1001)->getChildByTag(90);
        
        if(aMenu)
        {
            for(int i=0;i<5;i++)
            {
                aItem = (CCMenuItem*)aMenu->getChildByTag(5000+aMissionID+i);
                if(aItem)
                {
                    //We have found it !!! Show this mission
                    OnSelectMission(aItem->getTag()-5001);
                    PlaceIndicator((CCMenuItemImage*)aItem);
                    
                    
//                    CCSprite* aDeletSprite = (CCSprite*)getChildByTag(1001)->getChildByTag(i+1000+1);
//                    if(aDeletSprite){aDeletSprite->removeAllChildren();}
                    
                    
                    //Remove from this item stuff if can
                    break;
                }
            }
        }
        
        reorderChild(mDwarfMove->getParent(),1000);
        reorderChild(mDwarfMove,1000);
    }
}

void MissionScene::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool MissionScene::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    mDidMoveFinger = false;
    
    if (_moveStarted == false)
    {
        _moveStarted = true;
        _dragStart = touch->getLocation();
        
        if(getChildByTag(1000)->numberOfRunningActions()>0)
        {
            //Reset all layers to default !!! or wait for cooldown
            
            getChildByTag(1000)->stopAllActions();
            getChildByTag(1001)->stopAllActions();
            getChildByTag(1002)->stopAllActions();
            
            if(!mSlidingFree)
            {
                getChildByTag(1000)->setPositionX(-getChildByTag(1000)->getContentSize().width);
                getChildByTag(1001)->setPositionX(0);
                getChildByTag(1002)->setPositionX(getChildByTag(1000)->getContentSize().width);
            }
        }
        mLayer_2X = getChildByTag(1001)->getPositionX();
        
        _dragStart_sky = touch->getLocation();
        if(getChildByTag(2000)->numberOfRunningActions()>0)
        {
            //Reset all layers to default !!! or wait for cooldown
            
            getChildByTag(2000)->stopAllActions();
            getChildByTag(2001)->stopAllActions();
            getChildByTag(2002)->stopAllActions();
            
//            getChildByTag(2000)->setPositionX(-getChildByTag(2000)->getContentSize().width);
//            getChildByTag(2001)->setPositionX(0);
//            getChildByTag(2002)->setPositionX(getChildByTag(2000)->getContentSize().width);
        }
        mLayer_1X = getChildByTag(2001)->getPositionX();
        
        _dragStart_bottom = touch->getLocation();
        if(getChildByTag(3000)->numberOfRunningActions()>0)
        {
            //Reset all layers to default !!! or wait for cooldown
            
            getChildByTag(3000)->stopAllActions();
            getChildByTag(3001)->stopAllActions();
            getChildByTag(3002)->stopAllActions();
            
//            getChildByTag(3000)->setPositionX(-getChildByTag(3000)->getContentSize().width);
//            getChildByTag(3001)->setPositionX(0);
//            getChildByTag(3002)->setPositionX(getChildByTag(3000)->getContentSize().width);
        }
        mLayer_3X = getChildByTag(3001)->getPositionX();
        
        mSlidingFree = false;
        
        return true;
    }
    else
    {
        return false;
    }
}

void MissionScene::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    _moveStarted = false;
    
    
    if(mMoveBackScroll)
    {
        mMoveBackScroll = false;
        //Check on what side to scroll !!!
        
        int aMoveOffset = getChildByTag(1001)->getPositionX();
        
        CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(-getChildByTag(1000)->getContentSize().width,0));
        CCEaseBackOut* aEase1 = CCEaseBackOut::create(aMove1);
        getChildByTag(1000)->runAction(aEase1);
        
        CCMoveTo* aMove2 = CCMoveTo::create(0.5f,ccp(0,0));
        CCEaseBackOut* aEase2 = CCEaseBackOut::create(aMove2);
        getChildByTag(1001)->runAction(aEase2);
        
        CCMoveTo* aMove3 = CCMoveTo::create(0.5f,ccp(getChildByTag(1002)->getContentSize().width,0));
        CCEaseBackOut* aEase3 = CCEaseBackOut::create(aMove3);
        getChildByTag(1002)->runAction(aEase3);
        
        //Check the other sliders !!!--------------------
        aMove1 = CCMoveTo::create(0.5f,ccp(-getChildByTag(2000)->getContentSize().width,getContentSize().height/2));
        aEase1 = CCEaseBackOut::create(aMove1);
        getChildByTag(2000)->runAction(aEase1);
        
        aMove2 = CCMoveTo::create(0.5f,ccp(0,getContentSize().height/2));
        aEase2 = CCEaseBackOut::create(aMove2);
        getChildByTag(2001)->runAction(aEase2);
        
        aMove3 = CCMoveTo::create(0.5f,ccp(getChildByTag(2002)->getContentSize().width,getContentSize().height/2));
        aEase3 = CCEaseBackOut::create(aMove3);
        getChildByTag(2002)->runAction(aEase3);
        
        //Check the other sliders !!!--------------------
        CCMoveBy* aMoveBy1;
        //Move a bit back ground
        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*-0.5f,0));
        aEase1 = CCEaseBackOut::create(aMoveBy1);
        getChildByTag(3000)->runAction(aEase1);
        
        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*-0.5f,0));
        aEase1 = CCEaseBackOut::create(aMoveBy1);
        getChildByTag(3001)->runAction(aEase1);
        
        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*-0.5f,0));
        aEase1 = CCEaseBackOut::create(aMoveBy1);
        getChildByTag(3002)->runAction(aEase1);
        
        //the sky
        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*0.25f,0));
        aEase1 = CCEaseBackOut::create(aMoveBy1);
        getChildByTag(2000)->runAction(aEase1);
        
        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*0.25f,0));
        aEase1 = CCEaseBackOut::create(aMoveBy1);
        getChildByTag(2001)->runAction(aEase1);
        
        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*0.25f,0));
        aEase1 = CCEaseBackOut::create(aMoveBy1);
        getChildByTag(2002)->runAction(aEase1);
    }
    else if(mDidMoveFinger)
    {
        mSlidingFree = true;
        
        int aMoveOffset = getChildByTag(1001)->getPositionX();
        int aForceX = (aMoveOffset-mLayer_2X)*0.5f;
        if(aForceX>300)
            aForceX = 300;
        else if(aForceX<-300)
            aForceX = -300;
        
        CCMoveBy* aMove1 = CCMoveBy::create(0.5f,ccp(aForceX,0));
        CCEaseBackOut* aEase1 = CCEaseBackOut::create(aMove1);
        getChildByTag(1000)->runAction(aEase1);
        
        aMove1 = CCMoveBy::create(0.5f,ccp(aForceX,0));
        aEase1 = CCEaseBackOut::create(aMove1);
        getChildByTag(1001)->runAction(aEase1);
        
        aMove1 = CCMoveBy::create(0.5f,ccp(aForceX,0));
        aEase1 = CCEaseBackOut::create(aMove1);
        getChildByTag(1002)->runAction(aEase1);
        
        //The other layers !!!
        aMove1 = CCMoveBy::create(0.5f,ccp(aForceX*0.5,0));
        aEase1 = CCEaseBackOut::create(aMove1);
        getChildByTag(2000)->runAction(aEase1);
        
        aMove1 = CCMoveBy::create(0.5f,ccp(aForceX*0.5,0));
        aEase1 = CCEaseBackOut::create(aMove1);
        getChildByTag(2001)->runAction(aEase1);
        
        aMove1 = CCMoveBy::create(0.5f,ccp(aForceX*0.5,0));
        aEase1 = CCEaseBackOut::create(aMove1);
        getChildByTag(2002)->runAction(aEase1);
        
        //The land
        aMove1 = CCMoveBy::create(0.5f,ccp(aForceX*1.5,0));
        aEase1 = CCEaseBackOut::create(aMove1);
        getChildByTag(3000)->runAction(aEase1);
        
        aMove1 = CCMoveBy::create(0.5f,ccp(aForceX*1.5,0));
        aEase1 = CCEaseBackOut::create(aMove1);
        getChildByTag(3001)->runAction(aEase1);
        
        aMove1 = CCMoveBy::create(0.5f,ccp(aForceX*1.5,0));
        aEase1 = CCEaseBackOut::create(aMove1);
        getChildByTag(3002)->runAction(aEase1);
    }
    
    //Check if did move or not - if not then activate some mission that was under the click !!!
    if(!mDidMoveFinger)
    {
        int aMissionID = 1;
        CCPoint aLocation = touch->getLocation();
        bool aMissionFound = false;
        
        if(mCurrentPage==1)
        {
            aMissionID = 1;
        }
        else if(mCurrentPage==mMaxPages)
        {
            aMissionID = (mCurrentPage-2)*5+1;
        }
        else
        {
            aMissionID = (mCurrentPage-2)*5+1-mExtraOffsetPage;
        }
        
        //Check if we have any button under it !!!
        CCMenu* aMenu = (CCMenu*)getChildByTag(1000)->getChildByTag(90);
        CCMenuItem* aItem;
        
        if(aMenu && aLocation.x>getChildByTag(1000)->getPositionX()
           && aLocation.x<getChildByTag(1000)->getPositionX()+getChildByTag(1000)->getContentSize().width)
        {
            for(int i=0;i<5;i++)
            {
                aItem = (CCMenuItem*)aMenu->getChildByTag(5000+aMissionID+i);
                if(aItem && aLocation.x>aItem->getPositionX()+getChildByTag(1000)->getPositionX()
                   && aLocation.x<(aItem->getPositionX()+getChildByTag(1000)->getPositionX()+aItem->getContentSize().width))
                {
                    //We have the x now check the y
                    if(aLocation.y>aItem->getPositionY() &&
                       aLocation.y<aItem->getPositionY()+aItem->getContentSize().height)
                    {
                        //We have found it !!! Show this mission
                        OnSelectMission(aItem->getTag()-5001);
                        PlaceIndicator((CCMenuItemImage*)aItem);
                        aMissionFound = true;
                        break;
                    }
                }
            }
        }
        
        if(mCurrentPage!=1)
            aMissionID+=5;
        
        //Check next scene !!!
        aMenu = (CCMenu*)getChildByTag(1001)->getChildByTag(90);
        
        if(aMenu && !aMissionFound && aLocation.x>getChildByTag(1001)->getPositionX()
           && aLocation.x<getChildByTag(1001)->getPositionX()+getChildByTag(1001)->getContentSize().width)
        {
            for(int i=0;i<5;i++)
            {
                aItem = (CCMenuItem*)aMenu->getChildByTag(5000+aMissionID+i);
                if(aItem && aLocation.x>aItem->getPositionX()+getChildByTag(1001)->getPositionX()-aItem->getContentSize().width/2 &&
                   aLocation.x< aItem->getPositionX()+getChildByTag(1001)->getPositionX()+aItem->getContentSize().width/2)
                {
                    //We have the x now check the y
                    if(aLocation.y>aItem->getPositionY() &&
                       aLocation.y<aItem->getPositionY()+aItem->getContentSize().height)
                    {
                        //We have found it !!! Show this mission
                        OnSelectMission(aItem->getTag()-5001);
                        PlaceIndicator((CCMenuItemImage*)aItem);
                        aMissionFound = true;
                        break;
                    }
                }
            }
        }
        
        aMissionID+=5;
        
        //Check next scene !!!
        aMenu = (CCMenu*)getChildByTag(1002)->getChildByTag(90);
        if(aMenu && !aMissionFound && aLocation.x>getChildByTag(1002)->getPositionX()
           && aLocation.x<getChildByTag(1002)->getPositionX()+getChildByTag(1002)->getContentSize().width)
        {
            for(int i=0;i<5;i++)
            {
                aItem = (CCMenuItem*)aMenu->getChildByTag(5000+aMissionID+i);
                if(aItem && aLocation.x>aItem->getPositionX()+getChildByTag(1002)->getPositionX()-aItem->getContentSize().width/2 &&
                   aLocation.x< aItem->getPositionX()+getChildByTag(1002)->getPositionX()+aItem->getContentSize().width/2)
                {
                    //We have the x now check the y
                    if(aLocation.y>aItem->getPositionY() &&
                       aLocation.y<aItem->getPositionY()+aItem->getContentSize().height)
                    {
                        //We have found it !!! Show this mission
                        CCMenuItemImage* aItem_Check = (CCMenuItemImage*)aItem;
                        int aItemTagID = aItem_Check->getTag()-5001;
                        OnSelectMission(aItemTagID);
                        
//                        OnSelectMission(aItem->getTag()-5001);
                        PlaceIndicator((CCMenuItemImage*)aItem);
//                        aMissionFound = true;
                        break;
                    }
                }            }
        }
    }
    else if(!mMoveBackScroll)
    {
        //Add small move to factor !!!
//        int aMoveOffset = getChildByTag(1001)->getPositionX();
//        int aForceX = aMoveOffset-mLayer_2X;
//        if(aForceX>50)
//            aForceX = 50;
//        else if(aForceX<-50)
//            aForceX = -50;
//        
//        CCMoveBy* aMove1 = CCMoveBy::create(0.5f,ccp(aForceX,0));
//        CCEaseBackOut* aEase1 = CCEaseBackOut::create(aMove1);
//        getChildByTag(1000)->runAction(aEase1);
//        
//        aMove1 = CCMoveBy::create(0.5f,ccp(aForceX,0));
//        aEase1 = CCEaseBackOut::create(aMove1);
//        getChildByTag(1001)->runAction(aEase1);
//        
//        aMove1 = CCMoveBy::create(0.5f,ccp(aForceX,0));
//        aEase1 = CCEaseBackOut::create(aMove1);
//        getChildByTag(1002)->runAction(aEase1);
        
//        CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(-getChildByTag(1000)->getContentSize().width,0));
//        CCEaseBackOut* aEase1 = CCEaseBackOut::create(aMove1);
//        getChildByTag(1000)->runAction(aEase1);
//        
//        CCMoveTo* aMove2 = CCMoveTo::create(0.5f,ccp(0,0));
//        CCEaseBackOut* aEase2 = CCEaseBackOut::create(aMove2);
//        getChildByTag(1001)->runAction(aEase2);
//        
//        CCMoveTo* aMove3 = CCMoveTo::create(0.5f,ccp(getChildByTag(1002)->getContentSize().width,0));
//        CCEaseBackOut* aEase3 = CCEaseBackOut::create(aMove3);
//        getChildByTag(1002)->runAction(aEase3);
//        
//        //Check the other sliders !!!--------------------
//        aMove1 = CCMoveTo::create(0.5f,ccp(-getChildByTag(2000)->getContentSize().width,getContentSize().height/2));
//        aEase1 = CCEaseBackOut::create(aMove1);
//        getChildByTag(2000)->runAction(aEase1);
//        
//        aMove2 = CCMoveTo::create(0.5f,ccp(0,getContentSize().height/2));
//        aEase2 = CCEaseBackOut::create(aMove2);
//        getChildByTag(2001)->runAction(aEase2);
//        
//        aMove3 = CCMoveTo::create(0.5f,ccp(getChildByTag(2002)->getContentSize().width,getContentSize().height/2));
//        aEase3 = CCEaseBackOut::create(aMove3);
//        getChildByTag(2002)->runAction(aEase3);
//        
//        //Check the other sliders !!!--------------------
//        CCMoveBy* aMoveBy1;
//        //Move a bit back ground
//        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*-0.5f,0));
//        aEase1 = CCEaseBackOut::create(aMoveBy1);
//        getChildByTag(3000)->runAction(aEase1);
//        
//        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*-0.5f,0));
//        aEase1 = CCEaseBackOut::create(aMoveBy1);
//        getChildByTag(3001)->runAction(aEase1);
//        
//        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*-0.5f,0));
//        aEase1 = CCEaseBackOut::create(aMoveBy1);
//        getChildByTag(3002)->runAction(aEase1);
//        
//        //the sky
//        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*0.25f,0));
//        aEase1 = CCEaseBackOut::create(aMoveBy1);
//        getChildByTag(2000)->runAction(aEase1);
//        
//        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*0.25f,0));
//        aEase1 = CCEaseBackOut::create(aMoveBy1);
//        getChildByTag(2001)->runAction(aEase1);
//        
//        aMoveBy1 = CCMoveBy::create(0.5f,ccp(aMoveOffset*0.25f,0));
//        aEase1 = CCEaseBackOut::create(aMoveBy1);
//        getChildByTag(2002)->runAction(aEase1);
    }
}

void MissionScene::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(mDidMoveFinger!=true)
        mDidMoveFinger = true;
    
    //it moves in ultimate length
    getChildByTag(1001)->setPositionX(mLayer_2X+(touch->getLocation().x - _dragStart.x));
    //Other get by mid positions !!!
    getChildByTag(1000)->setPositionX(getChildByTag(1001)->getPositionX()-getChildByTag(1001)->getContentSize().width);
    getChildByTag(1002)->setPositionX(getChildByTag(1001)->getPositionX()+getChildByTag(1001)->getContentSize().width);
    
    getChildByTag(2001)->setPositionX(mLayer_1X+(touch->getLocation().x - _dragStart_sky.x)*0.5f);
    getChildByTag(2000)->setPositionX(getChildByTag(2001)->getPositionX()-getChildByTag(2001)->getContentSize().width);
    getChildByTag(2002)->setPositionX(getChildByTag(2001)->getPositionX()+getChildByTag(2001)->getContentSize().width);
    
    getChildByTag(3001)->setPositionX(mLayer_3X+(touch->getLocation().x - _dragStart_bottom.x)*1.5f);
    getChildByTag(3000)->setPositionX(getChildByTag(3001)->getPositionX()-getChildByTag(3001)->getContentSize().width);
    getChildByTag(3002)->setPositionX(getChildByTag(3001)->getPositionX()+getChildByTag(3001)->getContentSize().width);
    
    if(mCurrentPage==mMaxPages)
    {
        if(getChildByTag(1001)->getPositionX()<0)
        {
            mMoveBackScroll = true;
        }
    }
    else
    {
        if(getChildByTag(1001)->getPositionX()<(-mScreenSize.width/2-100))
        {
            mCurrentPage+=1;
            UpdateMissions(true);
            
            getChildByTag(1000)->setPositionX(getChildByTag(1002)->getPositionX()+getChildByTag(1002)->getContentSize().width);
            getChildByTag(1000)->setTag(1003);
            
            getChildByTag(1001)->setTag(1000);
            getChildByTag(1002)->setTag(1001);
            getChildByTag(1003)->setTag(1002);
            
            _dragStart = touch->getLocation();
            mLayer_2X = getChildByTag(1001)->getPositionX();//+(touch->getLocation().x - _dragStart.x);
        }
    }
    
    if(getChildByTag(2001)->getPositionX()<(-mScreenSize.width/2-100))
    {
        getChildByTag(2000)->setPositionX(getChildByTag(2002)->getPositionX()+getChildByTag(2002)->getContentSize().width);
        getChildByTag(2000)->setTag(2003);
        
        getChildByTag(2001)->setTag(2000);
        getChildByTag(2002)->setTag(2001);
        getChildByTag(2003)->setTag(2002);
        
        _dragStart_sky = touch->getLocation();
        mLayer_1X = getChildByTag(2001)->getPositionX();//+(touch->getLocation().x - _dragStart.x);
    }
    
    if(getChildByTag(3001)->getPositionX()<(-mScreenSize.width/2-100))
    {
        getChildByTag(3000)->setPositionX(getChildByTag(3002)->getPositionX()+getChildByTag(3002)->getContentSize().width);
        getChildByTag(3000)->setTag(3003);
        
        getChildByTag(3001)->setTag(3000);
        getChildByTag(3002)->setTag(3001);
        getChildByTag(3003)->setTag(3002);
        
        _dragStart_bottom = touch->getLocation();
        mLayer_3X = getChildByTag(3001)->getPositionX();//+(touch->getLocation().x - _dragStart.x);
    }
    
    if(mCurrentPage==1)
    {
        if(getChildByTag(1001)->getPositionX()>0)
        {
            mMoveBackScroll = true;
        }
    }
    else
    {
        if(getChildByTag(1001)->getPositionX()>(mScreenSize.width/2+100))
        {
            mCurrentPage-=1;
            UpdateMissions(false);
            
            getChildByTag(1002)->setPositionX(getChildByTag(1000)->getPositionX()-getChildByTag(1000)->getContentSize().width);
            getChildByTag(1002)->setTag(1003);
            
            getChildByTag(1001)->setTag(1002);
            getChildByTag(1000)->setTag(1001);
            getChildByTag(1003)->setTag(1000);
            
            _dragStart = touch->getLocation();
            mLayer_2X = getChildByTag(1001)->getPositionX();//+(touch->getLocation().x - _dragStart.x);
        }
    }
    
    if(getChildByTag(2001)->getPositionX()>(mScreenSize.width/2+100))
    {
        getChildByTag(2002)->setPositionX(getChildByTag(2000)->getPositionX()-getChildByTag(2000)->getContentSize().width);
        getChildByTag(2002)->setTag(2003);
        
        getChildByTag(2001)->setTag(2002);
        getChildByTag(2000)->setTag(2001);
        getChildByTag(2003)->setTag(2000);
        
        _dragStart_sky = touch->getLocation();
        mLayer_1X = getChildByTag(2001)->getPositionX();//+(touch->getLocation().x - _dragStart.x);
    }
    
    if(getChildByTag(3001)->getPositionX()>(mScreenSize.width/2+100))
    {
        getChildByTag(3002)->setPositionX(getChildByTag(3000)->getPositionX()-getChildByTag(3000)->getContentSize().width);
        getChildByTag(3002)->setTag(3003);
        
        getChildByTag(3001)->setTag(3002);
        getChildByTag(3000)->setTag(3001);
        getChildByTag(3003)->setTag(3000);
        
        _dragStart_bottom = touch->getLocation();
        mLayer_3X = getChildByTag(3001)->getPositionX();//+(touch->getLocation().x - _dragStart.x);
    }
    
//    if(mCurrentPage!=mLastPage)
//    {
//        mLastPage = mCurrentPage;
//        CreateMissions();//Update missions in view
//    }
}



