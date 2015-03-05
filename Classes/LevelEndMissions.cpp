//
//  LevelEndMissions.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 11/01/14.
//
//

#include "LevelEndMissions.h"
#include "ExperienceBar.h"
#include "SpriteAnimation.h"
#include "AppMacros.h"
#include "Utils.h"
#include "User.h"
#include "Levels.h"
#include "Crystal.h"
#include "LevelUpPopUp.h"
#include "LevelEndScene.h"

#include "SpriteAnimation.h"

using namespace DF::LevelEnd;
USING_NS_CC;

LevelEndMissions* DF::LevelEnd::LevelEndMissions::create()
{
    LevelEndMissions *pRet = new LevelEndMissions();
    if (pRet && pRet->init())
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

bool DF::LevelEnd::LevelEndMissions::init()
{
//    CCSprite* back = CCSprite::create("Interfeiss/level_end/forest.png");
//    back->setAnchorPoint(ccp(0, 0));
//    addChild(back);
    
    //Complete the 1st session!!!
//    if(User::getInstance()->FirstSessionDone==0)
//    {
//        User::getInstance()->FirstSessionDone = 1;
//        cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userFirstSession", 1);
//        cocos2d::CCUserDefault::sharedUserDefault()->flush();
//    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    int aMoveX = (visibleSize.width/5)/2;
    int aMoveRealX = aMoveX+10;
    
    //For now disabled
//    int aCurrentMissionID = User::getInstance()->getMissionManager().GetActiveMissionID();
    int aStartNum = User::getInstance()->getMissionManager().GetActiveMissionID()+2;
//    if(User::getInstance()->getMissionManager().DidCompleteMission() != false)
//        aStartNum-=1;//Lets lower it
    
//    aStartNum+=1;
    
    //The move char !!!
    mission_walker = CCSprite::create("Interfeiss/missions/mission_dwarfs.png");
    mission_walker->setScale(1.5f);
    
    //Create a button for them !!!
    CCMenuItemImage* backButton2 = CCMenuItemImage::create(
														  "Interfeiss/missions/mission_dwarfs.png",
														  "Interfeiss/missions/mission_dwarfs.png",
														  this,
														  menu_selector(LevelEndMissions::OnPadClick));
    
    CCMenu* backMenu2 = CCMenu::create(backButton2, NULL);
//    backMenu2->setColor(ccRED);
    backMenu2->setOpacity(0);
    backMenu2->setPosition(ccp(mission_walker->getContentSize().width/2,mission_walker->getContentSize().height/2));
    mission_walker->addChild(backMenu2, 1);
    
    //The other missions
    
    
    //Check what pad is needed to show now !!!
    //The reward

//    aMissionDwarf->addChild(mission_num);
    CCLabelTTF* mission_num;
//    bool aNeedText = true;
    
    mission_pad_1 = CCSprite::create("Interfeiss/missions/mission_pad.png");
    mission_pad_1->setAnchorPoint(ccp(0.5,0.0));
    mission_pad_1->setPositionY(-20);
    mission_pad_1->setPositionX(aMoveRealX);
    mission_pad_1->setScale(0);
    addChild(mission_pad_1);
    //Add futher
    aMoveRealX+=aMoveX+90;
    
    int aMissionID = User::getInstance()->getMissionManager().GetActiveMissionID()+2;
    CCMenuItemImage* aButtonReal = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                          "Interfeiss/missions/MissionSubClick.png",
                                          this,
                                          menu_selector(LevelEndMissions::OnClickedMission));
    aButtonReal->setTag(5000+aMissionID);
    aButtonReal->setOpacity(0);
    aButtonReal->setPosition(0,0);
    
    CCMenuItemImage* aButtonReal2 = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                           "Interfeiss/missions/MissionSubClick.png",
                                                           this,
                                                           menu_selector(LevelEndMissions::OnClickedMission));
    aButtonReal2->setTag(5000+aMissionID+1);
    aButtonReal2->setOpacity(0);
    aButtonReal2->setPosition(0,0);
    
    CCMenuItemImage* aButtonReal3 = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                            "Interfeiss/missions/MissionSubClick.png",
                                                            this,
                                                            menu_selector(LevelEndMissions::OnClickedMission));
    aButtonReal3->setTag(5000+aMissionID+2);
    aButtonReal3->setOpacity(0);
    aButtonReal3->setPosition(0,0);
    
    CCMenuItemImage* aButtonReal4 = CCMenuItemImage::create("Interfeiss/missions/MissionSubClick.png",
                                                            "Interfeiss/missions/MissionSubClick.png",
                                                            this,
                                                            menu_selector(LevelEndMissions::OnClickedMission));
    aButtonReal4->setTag(5000+aMissionID+3);
    aButtonReal4->setOpacity(0);
    aButtonReal4->setPosition(0,0);
    
    CCMenu* aMenuButtons = CCMenu::create(aButtonReal,aButtonReal2,aButtonReal3,aButtonReal4,NULL);
    aMenuButtons->alignItemsHorizontallyWithPadding(55);
    aMenuButtons->setPosition(visibleSize.width/2+90, 60);
    addChild(aMenuButtons);
    
    //Check what to show here !!!
    mission_pad_2 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aStartNum-1).c_str());
    mission_pad_2->setAnchorPoint(ccp(0.5,0.0));
    mission_pad_2->setPositionY(-20);
    mission_pad_2->setPositionX(aMoveRealX);
    mission_pad_2->setScale(0);
    addChild(mission_pad_2);
    
    std::stringstream theNumber;
    theNumber << aStartNum;
    std::string theNumberStr = theNumber.str();
    
    //Check if we have some reward stuff
    
    if(User::getInstance()->getMissionManager().ShowMissionNumber(aStartNum-1))
    {
        mission_num = CCLabelTTF::create(theNumberStr.c_str(),
                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                         CCSize(100, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        mission_num->setColor(ccc3(255, 233, 0));
        mission_num->setPositionX(mission_pad_1->getContentSize().width/2);
        mission_num->setPositionY(mission_pad_2->getPositionY()+mission_pad_2->getContentSize().height+30);
        mission_num->setTag(100);
        mission_pad_2->addChild(mission_num);
    }
    
    //Add futher
    aStartNum+=1;
    theNumber.str("");
    theNumber<< aStartNum;
    theNumberStr = theNumber.str();
    
    aMoveRealX+=aMoveX+90;
    
    mission_pad_3 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aStartNum-1).c_str());
    mission_pad_3->setAnchorPoint(ccp(0.5,0.0));
    mission_pad_3->setPositionY(-20);
    mission_pad_3->setPositionX(aMoveRealX);
    mission_pad_3->setScale(0);
    addChild(mission_pad_3);
    
    if(User::getInstance()->getMissionManager().ShowMissionNumber(aStartNum-1))
    {
        mission_num = CCLabelTTF::create(theNumberStr.c_str(),
                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                         CCSize(100, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        mission_num->setColor(ccc3(255, 233, 0));
        mission_num->setPositionX(mission_pad_1->getContentSize().width/2);
        mission_num->setPositionY(mission_pad_3->getPositionY()+mission_pad_3->getContentSize().height+30);
        mission_num->setTag(100);
        mission_pad_3->addChild(mission_num);
    }
    
    //Add futher
    aStartNum+=1;
    theNumber.str("");
    theNumber<< aStartNum;
    theNumberStr = theNumber.str();
    
    aMoveRealX+=aMoveX+90;
    
    mission_pad_4 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aStartNum-1).c_str());
    mission_pad_4->setAnchorPoint(ccp(0.5,0.0));
    mission_pad_4->setPositionY(-20);
    mission_pad_4->setPositionX(aMoveRealX);
    mission_pad_4->setScale(0);
    addChild(mission_pad_4);
    if(User::getInstance()->getMissionManager().ShowMissionNumber(aStartNum-1))
    {
        mission_num = CCLabelTTF::create(theNumberStr.c_str(),
                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                         CCSize(100, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        mission_num->setColor(ccc3(255, 233, 0));
        mission_num->setPositionX(mission_pad_1->getContentSize().width/2);
        mission_num->setPositionY(mission_pad_4->getPositionY()+mission_pad_4->getContentSize().height+30);
        mission_num->setTag(100);
        mission_pad_4->addChild(mission_num);
    }
    
    //Add futher
    aStartNum+=1;
    theNumber.str("");
    theNumber<< aStartNum;
    theNumberStr = theNumber.str();
    
    aMoveRealX+=aMoveX+90;
    
    mission_pad_5 = CCSprite::create(User::getInstance()->getMissionManager().GetMissionPad(aStartNum-1).c_str());
    mission_pad_5->setAnchorPoint(ccp(0.5,0.0));
    mission_pad_5->setPositionY(-20);
    mission_pad_5->setPositionX(aMoveRealX);
    mission_pad_5->setScale(0);
    if(User::getInstance()->getMissionManager().ShowMissionNumber(aStartNum-1))
    {
        mission_num = CCLabelTTF::create(theNumberStr.c_str(),
                                         FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*1.5,
                                         CCSize(100, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        mission_num->setColor(ccc3(255, 233, 0));
        mission_num->setPositionX(mission_pad_1->getContentSize().width/2);
        mission_num->setPositionY(mission_pad_5->getPositionY()+mission_pad_5->getContentSize().height+30);
        mission_num->setTag(100);
        mission_pad_5->addChild(mission_num);
    }
    addChild(mission_pad_5);
    
    
    CCDelayTime* aTime = CCDelayTime::create(1.0f);
    CCCallFuncN* aFunction = CCCallFuncN::create(this, callfuncN_selector(LevelEndMissions::GrowPads));
    CCSequence* aSeq = CCSequence::create(aTime,aFunction,NULL);
    runAction(aSeq);
    
    mission_walker->setPositionX(mission_pad_1->getPositionX());
    mission_walker->setPositionY(44);
    mission_walker->setScale(0);
    addChild(mission_walker);
    
    //Create the mission pop box
    mMissionBox = CCSprite::create("Interfeiss/missions/frame_mission_info.png");
    mMissionBox->setPosition(ccp(visibleSize.width/2,visibleSize.height+mMissionBox->getContentSize().height+50));
    
    //Add clouse button !!!
    CCMenuItemImage* backButton = CCMenuItemImage::create(
                                                          "Interfeiss/buttons/x_btn0001.png",
                                                          "Interfeiss/buttons/x_btn0002.png",
                                                          this,
														  menu_selector(LevelEndMissions::OnClouseMission));
    
    CCMenu* backMenu = CCMenu::create(backButton, NULL);
    backMenu->setPosition(mMissionBox->getContentSize().width-5,mMissionBox->getContentSize().height-5);
//    mMissionBox->setVisible(false);
    mMissionBox->addChild(backMenu, 1);
    
    //Add all the mission stuff!!!
    mMissionBox_Header = CCLabelTTF::create("",
                                            FONT_SKRANJI, TITLE_FONT_SIZE*0.9,
                                            CCSize(400, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    mMissionBox_Header->setColor(ccc3(255, 255, 255));
    mMissionBox_Header->setPositionX(mMissionBox->getContentSize().width/2);
    mMissionBox_Header->setPositionY(mMissionBox->getContentSize().height+30);
    mMissionBox->addChild(mMissionBox_Header);
    mMissionBox_Header->setVisible(false);//Not needed for now
    
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
    
    addChild(mMissionBox);
    
    OnSelectMission(1);
    
    return true;
}

void LevelEndMissions::OnClickedMission(CCObject* sender)
{
    CCMenuItemImage* aButton =(CCMenuItemImage*)sender;
    int aID = aButton->getTag()-5001;
    
    CCSize mScreenSize = CCDirector::sharedDirector()->getVisibleSize();
    
    mMissionBox->setPosition(ccp(mScreenSize.width/2,mScreenSize.height+mMissionBox->getContentSize().height+50));
    
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(mScreenSize.width/2,mScreenSize.height/2+20));
    CCEaseElasticOut* aEase1 = CCEaseElasticOut::create(aMove1,0.5f);
    mMissionBox->runAction(aEase1);
    
    //Show current completed mission status
    OnSelectMission(aID);
}

void LevelEndMissions::OnPadClick()
{
    CCSize mScreenSize = CCDirector::sharedDirector()->getVisibleSize();
    
    mMissionBox->setPosition(ccp(mScreenSize.width/2,mScreenSize.height+mMissionBox->getContentSize().height+50));
    
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(mScreenSize.width/2,mScreenSize.height/2+20));
    CCEaseElasticOut* aEase1 = CCEaseElasticOut::create(aMove1,0.5f);
    mMissionBox->runAction(aEase1);
    
    //Show current completed mission status
    OnSelectMission(User::getInstance()->getMissionManager().GetActiveMissionID());
//    mMissionBox->setVisible(true);
}

void LevelEndMissions::OnClouseMission()
{
    CCSize mScreenSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(mScreenSize.width/2,mScreenSize.height+mMissionBox->getContentSize().height+50));
    CCEaseElasticIn* aEase1 = CCEaseElasticIn::create(aMove1,1.0f);
    mMissionBox->runAction(aEase1);
    
//    mMissionBox->setVisible(false);
}

void LevelEndMissions::OnSelectMission(int theMissionID)
{
    //update the mission base !!!
    MissionSet aMission = User::getInstance()->getMissionManager().GetMissionByID(theMissionID);
    
    /*
    int aMissionAmount = aMission.mSubMission_1.size();
    
    int aMissionOffsetX = 0;
    if(aMissionAmount == 2)
        aMissionOffsetX = -25;
    else if(aMissionAmount == 1)
        aMissionOffsetX = -50;
    
    //The mission name !!!
    std::stringstream theMission;
    theMission<<"Mission set #"<<(aMission.id);
    std::string aMissionFinal = theMission.str();
    
    mMissionBox_Header->setString(aMissionFinal.c_str());
    
    //The 1st mission info
    std::vector<SubMission> aSubMiss = aMission.mSubMission_1;
    if(aSubMiss.size()==0)
        return;//Do not show anythinh
    
    //Check if this is not over current mission set
//    if(theMissionID > User::getInstance()->getMissionManager().GetActiveMissionID())
//    {
//        //Hide reward and move play button a bit up
//        if(mMissionRewardBox->getChildByTag(1111))
//            mMissionRewardBox->removeChildByTag(1111);
//        
//        if(mMissionRewardBox->getChildByTag(1112))
//            mMissionRewardBox->removeChildByTag(1112);
//        
//        mMissionRewardBox->setVisible(false);
//        
//        getChildByTag(778)->setPosition(mMissionRewardBox->getPositionX(),
//                                        mMissionRewardBox->getPositionY()-mMissionRewardBox->getContentSize().height-135/2+78);
//        
//        mMissionBox_Check1_On->setVisible(false);
//        mMissionBox_Check1_Off->setVisible(false);
//        mMissionBox_Text1->setVisible(false);
//        mMissionBox_Text1p->setVisible(false);
//        
//        mMissionBox_Check2_On->setVisible(false);
//        mMissionBox_Check2_Off->setVisible(false);
//        mMissionBox_Text2->setVisible(false);
//        mMissionBox_Text2p->setVisible(false);
//        
//        mMissionBox_Check3_On->setVisible(false);
//        mMissionBox_Check3_Off->setVisible(false);
//        mMissionBox_Text3->setVisible(false);
//        mMissionBox_Text3p->setVisible(false);
//        
//        //Show the complete prev mission
//        theMission.str("");
//        theMission.clear();
//        
//        theMission<<"Complete\nMission set #"<<(theMissionID);
//        
//        mMissionBox_CompletPrev->setString(theMission.str().c_str());
//        mMissionBox_CompletPrev->setVisible(true);
//        
//        return;
//    }
//    else
//    {
//        mMissionBox_CompletPrev->setVisible(false);
//        
//        getChildByTag(778)->setPosition(mMissionRewardBox->getPositionX(),
//                                        mMissionRewardBox->getPositionY()-mMissionRewardBox->getContentSize().height-135/2+15);
//        
//        mMissionRewardBox->setVisible(true);
//        ShowRewardInfo(theMissionID);
//    }
    
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
    
    //The reward
    theMission.str("");
    theMission<<(aMission.reward_name);
    aMissionFinal = theMission.str();
    */
}

void LevelEndMissions::GrowPads()
{
    float aDelayTimer = 0;
    
    CCDelayTime* aDelay = CCDelayTime::create(aDelayTimer);
    CCScaleTo* aScale = CCScaleTo::create(0.25f,1.0f);
    CCEaseBackOut* aEase = CCEaseBackOut::create(aScale);
    CCSequence* aSeq1 = CCSequence::create(aDelay,aEase,NULL);
    mission_pad_1->runAction(aSeq1);
    
    aDelay = CCDelayTime::create(aDelayTimer);
    aScale = CCScaleTo::create(0.25f,1.0f);
    aEase = CCEaseBackOut::create(aScale);
    aSeq1 = CCSequence::create(aDelay,aEase,NULL);
    mission_walker->runAction(aSeq1);
    
    aDelayTimer+=0.1;
    aDelay = CCDelayTime::create(aDelayTimer);
    aScale = CCScaleTo::create(0.25f,1.0f);
    aEase = CCEaseBackOut::create(aScale);
    aSeq1 = CCSequence::create(aDelay,aEase,NULL);
    mission_pad_2->runAction(aSeq1);
    
    aDelayTimer+=0.1;
    aDelay = CCDelayTime::create(aDelayTimer);
    aScale = CCScaleTo::create(0.25f,1.0f);
    aEase = CCEaseBackOut::create(aScale);
    aSeq1 = CCSequence::create(aDelay,aEase,NULL);
    mission_pad_3->runAction(aSeq1);
    
    aDelayTimer+=0.1;
    aDelay = CCDelayTime::create(aDelayTimer);
    aScale = CCScaleTo::create(0.25f,1.0f);
    aEase = CCEaseBackOut::create(aScale);
    aSeq1 = CCSequence::create(aDelay,aEase,NULL);
    mission_pad_4->runAction(aSeq1);
    
    aDelayTimer+=0.1;
    aDelay = CCDelayTime::create(aDelayTimer);
    aScale = CCScaleTo::create(0.25f,1.0f);
    aEase = CCEaseBackOut::create(aScale);
    CCCallFuncN* aFunction = CCCallFuncN::create(this, callfuncN_selector(LevelEndMissions::CheckIfCompletedMission));
    aSeq1 = CCSequence::create(aDelay,aEase,aFunction,NULL);
    mission_pad_5->runAction(aSeq1);
    
}

void LevelEndMissions::CheckIfCompletedMission()
{
    //Did we complete a mission ???
//    OnMoveCompleted();
//    return;//Forced
    
    //For now disabled - lets debug this finish mission stuff!!!
    if(User::getInstance()->getMissionManager().DidCompleteMission() == false)
        return;//Dont do anything else
    User::getInstance()->getMissionManager().GetNewLevel();
    mCompletedMission = User::getInstance()->getMissionManager().GetCompletedMission();
    //----------------------------------------------------------------------
    //Check if something was unlocked
    
    //Check what to unlock
    /* // Old and deprecated
    if(mCompletedMission.unlock_type == UNLOCK_TESLA_MACHINE)
    {
        //Check if this machine is not unlocked already !!!
        if(User::getInstance()->getMachine_EnemiesLvl()<1)
            User::getInstance()->setMachine_EnemiesLvl(1);
    }
    else if(mCompletedMission.unlock_type == UNLOCK_GHOST_MACHINE)
    {
        //Check if this machine is not unlocked already !!!
        if(User::getInstance()->getMachine_GhostLvl()<1)
            User::getInstance()->setMachine_GhostLvl(1);
    }
    else if(mCompletedMission.unlock_type == UNLOCK_CRYSTAL_PLANT)
    {
        if(User::getInstance()->getMachine_PlantLvl()<1)
            User::getInstance()->setMachine_PlantLvl(1);
    }
    */
    
    //----------------------------------------------------------------------
    
//    mCompletedMission = User::getInstance()->getMissionManager().GetMissionByID(1);
    
//    OnCompletedMission();
    CCCallFuncN* aFunction_start = CCCallFuncN::create(this, callfuncN_selector(LevelEndMissions::OnCompletedMission));
    CCCallFuncN* aFunction_reward = CCCallFuncN::create(this, callfuncN_selector(LevelEndMissions::OnMoveCompleted));
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //Create the text with completed mission
    CCSprite* aCongratHeader = CCSprite::create("MissionCompleted.png");
    aCongratHeader->setScale(0);
    aCongratHeader->setPosition(ccp(visibleSize.width/2,140));
    addChild(aCongratHeader);
    
    CCScaleTo* aHeaderScale = CCScaleTo::create(0.25f,1.0);
    CCDelayTime* aHeaderDelay = CCDelayTime::create(1.0f);
    
//    CCDelayTime* aWaitTime = CCDelayTime::create(0.25f);
//    CCSequence* aQuick = CCSequence::create(aWaitTime,aFunction_reward,NULL);
    
    CCScaleTo* aHeaderScaleOff = CCScaleTo::create(1.0f,1.5f);
    CCFadeOut* aHeaderFadeOff = CCFadeOut::create(0.25f);
    CCSpawn* aHeaderSpawn = CCSpawn::create(aHeaderScaleOff,aHeaderFadeOff,NULL);
//    CCSpawn* aHeaderSpawn = CCSpawn::create(aHeaderScaleOff,aHeaderFadeOff,aQuick,NULL);
    
//    CCSequence* aHeaderSeq = CCSequence::create(aHeaderScale,aFunction_start,aHeaderDelay,aHeaderSpawn,NULL);
    CCSequence* aHeaderSeq = CCSequence::create(aHeaderScale,aFunction_start,aHeaderDelay,aHeaderSpawn,aFunction_reward,NULL);
    aCongratHeader->runAction(aHeaderSeq);
}

void LevelEndMissions::OnCompletedMission()
{
    //Create the check on prev mission !!!
    CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/GhoustActivate.plist");
    p->setPosition(mission_pad_1->getPositionX()-40, mission_pad_1->getPositionY()+50);
    p->setAutoRemoveOnFinish(true);
    mission_pad_2->addChild(p,100);
    
    CCDelayTime* aFadeDelay = CCDelayTime::create(0.25f);
    CCFadeOut* aFadeOut = CCFadeOut::create(0.25);
    CCSequence* aFadeSeq = CCSequence::create(aFadeDelay,aFadeOut,NULL);
    if(mission_pad_2->getChildByTag(100) == NULL)
    {
        
    }
    else
    {
        mission_pad_2->getChildByTag(100)->runAction(aFadeSeq);
    }
    
    
    //Do the complete mission magic !!!
    CCDelayTime* aDelay = CCDelayTime::create(0.5f);
    //Special case
    CCMoveBy* aMove;
//    if(mCompletedMission.id == 2)
//        aMove = CCMoveBy::create(0.4f,ccp(mission_pad_2->getPositionX()-mission_pad_2->getContentSize().width/2-20,0));
//    else
//        aMove = CCMoveBy::create(0.4f,ccp(mission_pad_2->getPositionX()-mission_pad_2->getContentSize().width/2-40,0));
    aMove = CCMoveBy::create(0.4f,ccp(mission_pad_2->getPositionX()-mission_walker->getContentSize().width/2-20,0));
    CCEaseBackInOut* aEase = CCEaseBackInOut::create(aMove);
//    CCCallFuncN* aFunction = CCCallFuncN::create(this, callfuncN_selector(LevelEndMissions::OnMoveCompleted));
    CCCallFuncN* aFunction_start = CCCallFuncN::create(this, callfuncN_selector(LevelEndMissions::OnCreateCheckAnim));
    CCSequence* aSeq = CCSequence::create(aDelay,aFunction_start,aEase,NULL);
    
    mission_walker->runAction(aSeq);
}

void LevelEndMissions::OnCreateCheckAnim()
{
    SpriteAnimation* aCompletedAnim = SpriteAnimation::create("Interfeiss/missions/mission_checl.plist",false);
    aCompletedAnim->retain();
    aCompletedAnim->setPosition(ccp(aCompletedAnim->getContentSize().width/2+20,aCompletedAnim->getContentSize().height/2+20));
    mission_pad_1->addChild(aCompletedAnim);
}

void LevelEndMissions::OnMoveCompleted()
{
    //Hardcore
//    mCompletedMission.reward_type = REWARD_SCORE_MULTY;
    
    
    //Do some puff and reward show !!!
    //Give the reward and other stuff !!!
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* aBase = CCSprite::create("trajectory_dot_white.png");
    aBase->setPosition(ccp(visibleSize.width/2,160));
    aBase->setScale(0.0f);
    
    /*
    
    //What did we unlock!!!
    CCSprite* aDiamond;
    if(mCompletedMission.reward_type == REWARD_DIAMONDS)
    {
//        aDiamond = CCSprite::create("diamond/diamond.png");
        aDiamond = CCSprite::create("Interfeiss/missions/reward_diamond.png");
        aDiamond->setScale(2.0f);
        aDiamond->setRotation(25);
        aDiamond->setPosition(ccp(60,0));
        
        std::stringstream theDiamondAmount;
        theDiamondAmount<<"+"<<mCompletedMission.reward_amount;
        std::string aFinalTxt = theDiamondAmount.str();
        
        //Add some text to diamond how much did get
        CCDelayTime* aNumDelay = CCDelayTime::create(1.0f);
        CCFadeOut* aNumFade = CCFadeOut::create(0.25f);
        CCSequence* aNumSeq = CCSequence::create(aNumDelay,aNumFade,NULL);
        
        CCLabelTTF* aRewardTxt = CCLabelTTF::create(aFinalTxt.c_str(),
                                                    FONT_SKRANJI, TITLE_FONT_SIZE*2.0,
                                                    CCSize(300, 200), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        aRewardTxt->setColor(ccc3(255, 255, 255));
//        CCLog("box width %f",aRewardTxt->getContentSize().width);
        aRewardTxt->setRotation(-10);
        aRewardTxt->setPositionX(aBase->getContentSize().width/2-50);
        aRewardTxt->setPositionY(aBase->getContentSize().height-20);
        aRewardTxt->setTag(100);
        aBase->addChild(aRewardTxt);
        aRewardTxt->runAction(aNumSeq);
        
        User::getInstance()->addDiamonds(mCompletedMission.reward_amount);
        
        aBase->addChild(aDiamond);
        
        aNumDelay = CCDelayTime::create(1.0f);
        aNumFade = CCFadeOut::create(0.25f);
        aNumSeq = CCSequence::create(aNumDelay,aNumFade,NULL);
        aDiamond->runAction(aNumSeq);
    }
    else if(mCompletedMission.reward_type == REWARD_CRYSTALS)
    {
        aDiamond = CCSprite::create("Interfeiss/level_end/highscore_cup.png");
        aDiamond->setScale(1.5f);
        aDiamond->setPositionY(30);
        
        aBase->addChild(aDiamond);
        
        std::stringstream theDiamondAmount;
        theDiamondAmount<<"+"<<mCompletedMission.reward_amount;
        std::string aFinalTxt = theDiamondAmount.str();
        
        //Add some text to diamond how much did get
        CCDelayTime* aNumDelay = CCDelayTime::create(1.0f);
        CCFadeOut* aNumFade = CCFadeOut::create(0.25f);
        CCSequence* aNumSeq = CCSequence::create(aNumDelay,aNumFade,NULL);
        
        CCLabelTTF* aRewardTxt = CCLabelTTF::create(aFinalTxt.c_str(),
                                                    FONT_SKRANJI, TITLE_FONT_SIZE*1.3,
                                                    CCSize(300, 200), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        aRewardTxt->setColor(ccc3(255, 255, 255));
        //        CCLog("box width %f",aRewardTxt->getContentSize().width);
//        aRewardTxt->setRotation(-10);
        aRewardTxt->setPositionX(aBase->getContentSize().width/2-14);
        aRewardTxt->setPositionY(aBase->getContentSize().height-24);
        aRewardTxt->setTag(100);
        aBase->addChild(aRewardTxt);
        aRewardTxt->runAction(aNumSeq);
        
        User::getInstance()->addCrystals(mCompletedMission.reward_amount);
        
        
        
        aNumDelay = CCDelayTime::create(1.0f);
        aNumFade = CCFadeOut::create(0.25f);
        aNumSeq = CCSequence::create(aNumDelay,aNumFade,NULL);
        aDiamond->runAction(aNumSeq);
    }
    else if(mCompletedMission.reward_type == REWARD_SCORE_MULTY)
    {
        aDiamond = CCSprite::create("Interfeiss/missions/unlock_shine.png");
        aDiamond->setScale(0.0f);
        aDiamond->setPositionY(30);
        
        CCDelayTime* aBonusStartDelay = CCDelayTime::create(0.0f);
        CCScaleTo* aBonusScale = CCScaleTo::create(0.25f,1.5f);
        CCEaseBackOut* aBonusEaseOut = CCEaseBackOut::create(aBonusScale);
        CCDelayTime* aBonusDelay = CCDelayTime::create(0.5f);
        CCFadeOut* aBonusFadeOut = CCFadeOut::create(0.5f);
        
        CCSequence* aBonusSeq = CCSequence::create(aBonusStartDelay,aBonusEaseOut,aBonusDelay,aBonusFadeOut,NULL);
        aDiamond->runAction(aBonusSeq);
        
        aBase->addChild(aDiamond);
        
        CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/KaboomFx.plist");
        p->setPosition(0,0);
        p->setScale(2.0f);
        p->setAutoRemoveOnFinish(true);
        aBase->addChild(p);
        
        User::getInstance()->setScore_Multi(mCompletedMission.reward_amount);
        
//        aDiamond = CCSprite::create("diamond/diamond.png");
        CCDelayTime* aNumDelay = CCDelayTime::create(1.0f);
        CCFadeOut* aNumFade = CCFadeOut::create(0.25f);
        CCSequence* aNumSeq = CCSequence::create(aNumDelay,aNumFade,NULL);
        
        //Add some text !!!
        std::stringstream theDiamondAmount;
        theDiamondAmount<<"x"<<mCompletedMission.reward_amount;
        std::string aFinalTxt = theDiamondAmount.str();
        
        CCLabelTTF* aRewardTxt = CCLabelTTF::create(aFinalTxt.c_str(),
                                                    FONT_SKRANJI, TITLE_FONT_SIZE*2.0,
                                                    CCSize(300, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        aRewardTxt->setColor(ccc3(255, 255, 255));
//        CCLog("box width %f",aRewardTxt->getContentSize().width);
        //        aRewardTxt->setRotation(-10);
        aRewardTxt->setPositionX(aBase->getContentSize().width/2-10);
        aRewardTxt->setPositionY(aBase->getContentSize().height+30);
        aRewardTxt->setTag(100);
//        aRewardTxt->enableStroke(ccBLACK, 1.5f,true);
        aRewardTxt->enableShadow(CCSize(4,-4),4,false);
        aBase->addChild(aRewardTxt);
        aRewardTxt->runAction(aNumSeq);
        
        //Comes another text about score multiplier
        theDiamondAmount.str("");
        theDiamondAmount<<"Score";
        aFinalTxt = theDiamondAmount.str();
        
        aRewardTxt = CCLabelTTF::create(aFinalTxt.c_str(),
                                        FONT_SKRANJI, TITLE_FONT_SIZE*1.0,
                                        CCSize(300, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        aRewardTxt->setPositionX(aBase->getContentSize().width/2-10);
        aRewardTxt->setPositionY(aBase->getContentSize().height/2-30);
        aRewardTxt->setTag(101);
        aRewardTxt->setColor(ccc3(73, 123, 122));
        aRewardTxt->enableShadow(CCSize(4,-4),4,false);
        aBase->addChild(aRewardTxt);
        
        aNumDelay = CCDelayTime::create(1.0f);
        aNumFade = CCFadeOut::create(0.25f);
        aNumSeq = CCSequence::create(aNumDelay,aNumFade,NULL);
        aRewardTxt->runAction(aNumSeq);
        
        
        theDiamondAmount.str("");
        theDiamondAmount<<"multiplier";
        aFinalTxt = theDiamondAmount.str();
        
        aRewardTxt = CCLabelTTF::create(aFinalTxt.c_str(),
                                        FONT_SKRANJI, TITLE_FONT_SIZE*1.0,
                                        CCSize(300, 100), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        aRewardTxt->setPositionX(aBase->getContentSize().width/2-10);
        aRewardTxt->setPositionY(aBase->getContentSize().height/2-80);
        aRewardTxt->setTag(102);
        aRewardTxt->setColor(ccc3(73, 123, 122));
        aBase->addChild(aRewardTxt);
        
        aNumDelay = CCDelayTime::create(1.0f);
        aNumFade = CCFadeOut::create(0.25f);
        aNumSeq = CCSequence::create(aNumDelay,aNumFade,NULL);
        aRewardTxt->runAction(aNumSeq);
    }
    
    //The default anim !!!
//    CCDelayTime* aNumDelay = CCDelayTime::create(0.75f);
//    CCFadeOut* aNumFade = CCFadeOut::create(0.25f);
//    CCSequence* aNumSeq = CCSequence::create(aNumDelay,aNumFade,NULL);
//    aDiamond->runAction(aNumSeq);
    
//    aBase->addChild(aDiamond);
    
//    CCLabelTTF* mission_num_diamond = CCLabelTTF::create("+3",FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*2.0f,CCSize(60, 60), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//    mission_num_diamond->setPositionX(-50);
//    mission_num_diamond->setPositionY(-20);
//    mission_num_diamond->setTag(100);
//    aBase->addChild(mission_num_diamond);
    
    //Add some text to diamond how much did get
//    CCLabelTTF* mission_num = CCLabelTTF::create("+3",
//                                     FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*2.0,
//                                     CCSize(60, 60), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//    mission_num->setColor(ccc3(255, 255, 255));
//    mission_num->setPositionX(-50);
//    mission_num->setPositionY(-20);
//    mission_num->setRotation(-10);
//    mission_num->setTag(100);
    
//    CCDelayTime* aNumDelay_1 = CCDelayTime::create(0.75f);
//    CCFadeOut* aNumFade_1 = CCFadeOut::create(0.25f);
//    CCSequence* aNumSeq_1 = CCSequence::create(aNumDelay_1,aNumFade_1,NULL);
//    mission_num->runAction(aNumSeq_1);
    
//    aBase->addChild(mission_num);
    
    */
    
    addChild(aBase);
    
    CCScaleTo* aScale = CCScaleTo::create(0.25f,1.0f);
    CCEaseBackOut* aEase = CCEaseBackOut::create(aScale);
    
    CCDelayTime* aDelay = CCDelayTime::create(0.5f);
    
    CCFadeOut* aFade = CCFadeOut::create(0.25f);
    
    if(mCompletedMission.unlock_type==0)
    {
        CCSequence* aSeq = CCSequence::create(aEase,aDelay,aFade,NULL);
        aBase->runAction(aSeq);
    }
    else
    {
        CCCallFuncN* aFunction = CCCallFuncN::create(this, callfuncN_selector(LevelEndMissions::CreteUnlock));
        CCSequence* aSeq = CCSequence::create(aEase,aDelay,aFade,aFunction,NULL);
        aBase->runAction(aSeq);
    }
}

void LevelEndMissions::CreteUnlock()
{
    //Did we have some unlock??
    if(mCompletedMission.unlock_type==0)
        return;//Do not do anything !!!
    
//    //Check what to unlock
//    if(mCompletedMission.unlock_type == UNLOCK_TESLA_MACHINE)
//    {
//        //Check if this machine is not unlocked already !!!
//        if(User::getInstance()->getMachine_EnemiesLvl()<1)
//            User::getInstance()->setMachine_EnemiesLvl(1);
//    }
//    else if(mCompletedMission.unlock_type == UNLOCK_GHOST_MACHINE)
//    {
//        //Check if this machine is not unlocked already !!!
//        if(User::getInstance()->getMachine_GhostLvl()<1)
//            User::getInstance()->setMachine_GhostLvl(1);
//    }
//    else if(mCompletedMission.unlock_type == UNLOCK_CRYSTAL_PLANT)
//    {
//        if(User::getInstance()->getMachine_PlantLvl()<1)
//            User::getInstance()->setMachine_PlantLvl(1);
//    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* aBase = CCSprite::create("trajectory_dot_white.png");
    aBase->setPosition(ccp(visibleSize.width/2,160));
    aBase->setOpacity(0);
//    aBase->setScale(0.0f);
    
    CCSprite* aShine = CCSprite::create("Interfeiss/missions/unlock_shine.png");
    aShine->setPosition(ccp(0,0));
    aShine->setScale(0.0f);//1.5
    
//    CCDelayTime* aBonusDelay = CCDelayTime::create(1.0f);
//    CCFadeOut* aBonusFade = CCFadeOut::create(0.5f);
//    CCScaleTo* aBonusScale = CCScaleTo::create(1.0f,1.5f);
//    CCSpawn* aBonusSpawn = CCSpawn::create(aBonusFade,aBonusScale,NULL);
//    CCSequence* aBonusSeq = CCSequence::create(aBonusDelay,aBonusSpawn,NULL);
//    aShine->runAction(aBonusSeq);
    
    CCDelayTime* aBonusStartDelay = CCDelayTime::create(0.25f);
    CCScaleTo* aBonusScale = CCScaleTo::create(0.5f,1.5f);
    CCEaseBackOut* aBonusEaseOut = CCEaseBackOut::create(aBonusScale);
    CCDelayTime* aBonusDelay = CCDelayTime::create(1.0f);
    CCFadeOut* aBonusFadeOut = CCFadeOut::create(0.5f);
    
    CCSequence* aBonusSeq = CCSequence::create(aBonusStartDelay,aBonusEaseOut,aBonusDelay,aBonusFadeOut,NULL);
    aShine->runAction(aBonusSeq);
    
    aBase->addChild(aShine);
    
    //What did we unlock
    CCSprite* aBonus;
    if(mCompletedMission.unlock_type==UNLOCK_CRYSTAL_PLANT)
        aBonus = CCSprite::create("Interfeiss/upgrade_screen/plant_1.png");
    else if(mCompletedMission.unlock_type==UNLOCK_GHOST_MACHINE)
        aBonus = CCSprite::create("Interfeiss/upgrade_screen/ghost_1.png");
    else if(mCompletedMission.unlock_type==UNLOCK_TESLA_MACHINE)
        aBonus = CCSprite::create("Interfeiss/upgrade_screen/elektro_1.png");
    else
        aBonus = CCSprite::create("Interfeiss/missions/unlock_chest.png");
    aBonus->setScale(0.0f);
    aBonus->setPosition(ccp(0,0));
    
    aBonusStartDelay = CCDelayTime::create(0.25f);
    aBonusScale = CCScaleTo::create(0.5f,1.25f);
    aBonusEaseOut = CCEaseBackOut::create(aBonusScale);
    aBonusDelay = CCDelayTime::create(1.0f);
    CCScaleTo* aBonusScale2 = CCScaleTo::create(0.5f,1.5f);
    aBonusFadeOut = CCFadeOut::create(0.5f);
    CCSpawn* aBonusSpawn = CCSpawn::create(aBonusScale2,aBonusFadeOut,NULL);
    aBonusSeq = CCSequence::create(aBonusStartDelay,aBonusEaseOut,aBonusDelay,aBonusSpawn,NULL);
    aBonus->runAction(aBonusSeq);
    
    aBase->addChild(aBonus);
    
    //The unlock feature
    CCSprite* aUnlock = CCSprite::create("Interfeiss/missions/unlock.png");
    aUnlock->setPosition(ccp(0,0));
    aUnlock->setScale(0.0f);
    
    aBonusStartDelay = CCDelayTime::create(0.3f);
    aBonusScale = CCScaleTo::create(0.5f,1.2f);
    aBonusEaseOut = CCEaseBackOut::create(aBonusScale);
    
    CCRotateTo* aLockRotate1 = CCRotateTo::create(0.1f,10);
    CCRotateTo* aLockRotate2 = CCRotateTo::create(0.1f,-10);
    CCSequence* aLockSeq1 = CCSequence::create(aLockRotate1,aLockRotate2,NULL);
    CCRepeat* aLockRepeat = CCRepeat::create(aLockSeq1, 3);
    
    CCSpawn* aLockSpawn = CCSpawn::create(aBonusEaseOut,aLockRepeat,NULL);
    
    CCFadeOut* aLockFadeOut = CCFadeOut::create(0.5f);
    
    CCSequence* aLockSeq = CCSequence::create(aBonusStartDelay,aLockSpawn,aLockFadeOut,NULL);
    aUnlock->runAction(aLockSeq);
    
    aBase->addChild(aUnlock);
    
    addChild(aBase);
    
//    CCScaleTo* aScaleBase = CCScaleTo::create(0.25f,1.0f);
//    CCDelayTime* aDelayBase = CCDelayTime::create(1.0f);
//    CCFadeOut* aFadeBase = CCFadeOut::create(0.25f);
//    CCCallFuncN* aFuncBase = CCCallFuncN::create(this, callfuncN_selector(LevelEndMissions::OnCompletedAll));
//    CCSequence* aSeqBase = CCSequence::create(aScaleBase,aDelayBase,aFadeBase,aFuncBase,NULL);
//    aBase->runAction(aSeqBase);
//    CCDelayTime* aDelayBase = CCDelayTime::create(2.0f);
//    CCCallFuncN* aFuncBase = CCCallFuncN::create(this, callfuncN_selector(LevelEndMissions::OnCompletedAll));
//    CCSequence* aSeqBase = CCSequence::create(aScaleBase,aDelayBase,aFadeBase,aFuncBase,NULL);
//    aBase->runAction(aSeqBase);
}

void LevelEndMissions::OnCompletedAll()
{
    
}

//Do some other magic !!!




