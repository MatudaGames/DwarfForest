#include "PauseScene.h"
#include "MainMenuScene.h"
#include "TutorialScene.h"
#include "GameScene.h"
#include <SimpleAudioEngine.h>
#include "PopUp.h"
#include "Utils.h"
#include "BeforeQuitScene.h"
#include "MissionManager.h"

#include "AppMacros.h"
#include "User.h"
#include "DailyDaysCompletePopup.h"

#include "AnalyticX.h"


USING_NS_CC;

CCScene* PauseScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    PauseScene *layer = PauseScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PauseScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    AnalyticX::flurryLogEvent("Game Menu: Pause");
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
    back->setAnchorPoint(ccp(0,0));
//    addChild(back, 0);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Tap_Main").c_str());
    
    //The resume button
    CCMenuItemImage* resumeButton = CCMenuItemImage::create(
                                                            "Interfeiss/pause_menu/resume_game_btn0001.png",
                                                            "Interfeiss/pause_menu/resume_game_btn0002.png",
                                                            this,
                                                            menu_selector(PauseScene::menuResumeCallback));
//    resumeButton->setAnchorPoint(ccp(0, 0.5));
//    CCMenu* mainMenu = CCMenu::create(resumeButton, NULL);
////    mainMenu->alignItemsVerticallyWithPadding(-40);
//    mainMenu->alignItemsHorizontally();
//    mainMenu->setPositionY(visibleSize.height-50);
//    mainMenu->setPositionX(visibleSize.width/2);
//    this->addChild(mainMenu, 1);
    
    mWeaklyActive = true;
    
    //Now lets add the mission bar !!!
    CCSprite* aDwarfHead = CCSprite::create("Interfeiss/pause_menu/darf_head.png");
    
//    CCSprite* aMissionDwarf = CCSprite::create("Interfeiss/pause_menu/dwarf_speech.png");
//    CCSprite* aMissionDwarf = CCSprite::create("Interfeiss/missions/frame_mission_info.png");
    
    //For now add dwarf forest logo for nicer look
    CCSprite* aMissionDwarf = CCSprite::create("Interfeiss/missions/frame_mission_info.png");
    aMissionDwarf->setVisible(false); // For now
    aMissionDwarf->setAnchorPoint(ccp(0.5,0.5));
    aMissionDwarf->setPositionX(visibleSize.width/2-aDwarfHead->getContentSize().width/2);
    aMissionDwarf->setPositionY(visibleSize.height-aMissionDwarf->getContentSize().height);//+40
    addChild(aMissionDwarf);
    
    //Add the dwaft head !!!
    
    aDwarfHead->setPositionX(aMissionDwarf->getContentSize().width+aDwarfHead->getContentSize().width/2);
    aDwarfHead->setPositionY(aDwarfHead->getContentSize().height/2-30);//+40
    aMissionDwarf->addChild(aDwarfHead);
    
//    CCMenuItemImage* s1 = CCMenuItemImage::create("Interfeiss/checkbox_empty.png",
//                                                  "Interfeiss/checkbox_checked.png", this, NULL)
    
    
    CCMenuItemImage* btn_tab_1 = CCMenuItemImage::create(
                                                         "Interfeiss/pause_menu/tab_weekly.png",
                                                         "Interfeiss/pause_menu/tab_weekly.png",
                                                         this,
                                                         menu_selector(PauseScene::OnShowWeakly));
    btn_tab_1->setOpacity(0);
    
    
    
    CCMenuItemImage* btn_tab_2 = CCMenuItemImage::create(
                                                        "Interfeiss/pause_menu/tab_daily.png",
                                                        "Interfeiss/pause_menu/tab_daily.png",
                                                        this,
                                                        menu_selector(PauseScene::OnShowDaily));
    btn_tab_2->setOpacity(0);
    
    
    CCMenu* ChallangeBar = CCMenu::create(btn_tab_1, btn_tab_2, NULL);
    ChallangeBar->alignItemsVerticallyWithPadding(0);
//    lowBar->setPosition(visibleSize.width/2, 70);
    
    
    CCSprite* aChallangeBG = CCSprite::create("Interfeiss/pause_menu/challenge_base.png");
    aChallangeBG->setVisible(false); // For now disabled
    
//    aChallangeBG->setPosition(ccp(visibleSize.width/2,aMissionDwarf->getPositionY()-70-aChallangeBG->getContentSize().height));
    addChild(aChallangeBG);
    
    aChallangeBG->addChild(ChallangeBar, 2);
    ChallangeBar->setPosition(ccp(aChallangeBG->getContentSize().width+btn_tab_1->getContentSize().width/2-10,
                                  aChallangeBG->getContentSize().height/2));
    
    //The 2 tabs !!!
    mWeaklyTab = CCSprite::create("Interfeiss/pause_menu/tab_weekly.png");
    mWeaklyTab->setColor(ccc3(255,255,255));
    mWeaklyTab->setPosition(ccp(aChallangeBG->getContentSize().width+mWeaklyTab->getContentSize().width/2-10,
                             aChallangeBG->getContentSize().height-mWeaklyTab->getContentSize().height/2));
    aChallangeBG->addChild(mWeaklyTab);
    aChallangeBG->reorderChild(mWeaklyTab, 1);
    
    mDailyTab = CCSprite::create("Interfeiss/pause_menu/tab_daily.png");
    mDailyTab->setColor(ccc3(160,160,160));
    mDailyTab->setPosition(ccp(aChallangeBG->getContentSize().width+mDailyTab->getContentSize().width/2-10,
                             aChallangeBG->getContentSize().height/2-mDailyTab->getContentSize().height/2));
    aChallangeBG->addChild(mDailyTab);
    aChallangeBG->reorderChild(mDailyTab, -1);
    
    aChallangeBG->setPosition(ccp(visibleSize.width/2-mDailyTab->getContentSize().width/2-10,
                                  aMissionDwarf->getPositionY()-70-aChallangeBG->getContentSize().height));
    
    //Create the weakly part !!!
    mDailyBG = CCSprite::create("trajectory_dot_white.png");
    mDailyBG->setPosition(ccp(aChallangeBG->getContentSize().width/2+30,
                              aChallangeBG->getContentSize().height/2-15));
    mDailyBG->setVisible(false);
    mDailyBG->setOpacity(0);
    aChallangeBG->addChild(mDailyBG);
    
    DailyChallenges daily = User::getInstance()->getDailyChallenges();
    
    CCSprite* icon1 = getBarIconPause(daily.getBar1Color());
    CCSprite* icon2 = getBarIconPause(daily.getBar2Color());
    CCSprite* icon3 = getBarIconPause(daily.getBar3Color());
    
    icon1->setAnchorPoint(ccp(0.5, 0));
    icon2->setAnchorPoint(ccp(0.5, 0));
    icon3->setAnchorPoint(ccp(0.5, 0));
    
    icon1->setPosition(ccp(-120, 5));
    icon2->setPosition(ccp(-20, 5));
    icon3->setPosition(ccp(80, 5));
    
    mDailyBG->addChild(icon1);
    mDailyBG->addChild(icon2);
    mDailyBG->addChild(icon3);
    
    b1 = CCLabelTTF::create("2/5", FONT_SKRANJI, TITLE_FONT_SIZE*0.45, CCSize(65, 50), kCCTextAlignmentCenter);
    b2 = CCLabelTTF::create("2/5", FONT_SKRANJI, TITLE_FONT_SIZE*0.45, CCSize(65, 50), kCCTextAlignmentCenter);
    b3 = CCLabelTTF::create("2/5", FONT_SKRANJI, TITLE_FONT_SIZE*0.45, CCSize(65, 50), kCCTextAlignmentCenter);
    
    b1->setAnchorPoint(ccp(0.5, 0));
    b2->setAnchorPoint(ccp(0.5, 0));
    b3->setAnchorPoint(ccp(0.5, 0));
    
    b1->setColor(ccc3(56, 23, 0));
    b2->setColor(ccc3(56, 23, 0));
    b3->setColor(ccc3(56, 23, 0));
    
    b1->setPosition(ccp(-78, -14));
    b2->setPosition(ccp(22, -14));
    b3->setPosition(ccp(122, -14));
    
    mDailyBG->addChild(b1);
    mDailyBG->addChild(b2);
    mDailyBG->addChild(b3);
    
    std::map<CrystalColor, int> map = _stats.gatheredCrystals;
    
    int v = 0;
    
    v = (map[daily.getBar1Color()] < daily.getBar1Count() ?
         map[daily.getBar1Color()] : daily.getBar1Count());
    map[daily.getBar1Color()] -= v;
    
    b1->setString((toString(v) + "/" + toString(daily.getBar1Count())).c_str());
    
    v = (map[daily.getBar2Color()] < daily.getBar2Count() ?
         map[daily.getBar2Color()] : daily.getBar2Count());
    map[daily.getBar2Color()] -= v;
    
    b2->setString((toString(v) + "/" + toString(daily.getBar2Count())).c_str());
    
    v = (map[daily.getBar3Color()] < daily.getBar3Count() ?
         map[daily.getBar3Color()] : daily.getBar3Count());
    map[daily.getBar3Color()] -= v;
    
    b3->setString((toString(v) + "/" + toString(daily.getBar3Count())).c_str());
    
    //==========
    
    mWaklyBG = CCSprite::create("trajectory_dot_white.png");
    mWaklyBG->setOpacity(0);
    mWaklyBG->setPosition(ccp(aChallangeBG->getContentSize().width/2,aChallangeBG->getContentSize().height/2));
    aChallangeBG->addChild(mWaklyBG);
    
    if(User::getInstance()->isEasterActive())
    {
        CCSprite* aDummy = CCSprite::create("Interfeiss/challenges/weekly/easter/egg_big.png");
        aDummy->setPosition(ccp(60,0));
        mWaklyBG->addChild(aDummy);
        
        int max = 20;
        int value = User::getInstance()->mEasterEggCount;
        
        std::stringstream theAmount;
        theAmount<<value<<"/"<<max;
        
        CCLabelTTF* _EasterProgress = CCLabelTTF::create(theAmount.str().c_str(), FONT_SKRANJI, TITLE_FONT_SIZE,
                                             CCSize(140, 50), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
        _EasterProgress->setPosition(ccp(-54,-4));
        _EasterProgress->setColor(ccc3(92, 20, 11));
        mWaklyBG->addChild(_EasterProgress);
    }
    else
    {
        //Add some text fields
        const WeeklyTask* wt = User::getInstance()->getWeeklyChallenges().getTasks();
        
        CCLabelTTF* titleSh = CCLabelTTF::create(wt[0].getTaskInfo().c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                                 CCSize(700, 70), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
        titleSh->setAnchorPoint(ccp(0,0));
        titleSh->setPosition(ccp(-aChallangeBG->getContentSize().width/2+110,aChallangeBG->getContentSize().height/4+20));
        titleSh->setColor(ccc3(0, 0, 0));
        mWaklyBG->addChild(titleSh);
        
        CCSprite* backProgress = CCSprite::create("Interfeiss/pause_menu/progresbar_base.png");
        backProgress->setAnchorPoint(ccp(0,0));
        backProgress->setPosition(ccp(-aChallangeBG->getContentSize().width/2+76,aChallangeBG->getContentSize().height/4-8));
        mWaklyBG->addChild(backProgress);
        
        CCSprite* progress = CCSprite::create("Interfeiss/pause_menu/challenge_progresbar.png");
        progress->setAnchorPoint(ccp(0,0));
        progress->setPosition(ccp(5,4));
        
        int value = User::getInstance()->getWeeklyChallenges().getTask1Progress();
        int max = wt[0].getRequirement();
        backProgress->addChild(progress);
        progress->setTextureRect(CCRect(0, 0,
                                        progress->getTexture()->getContentSize().width*((float)value/max),
                                        progress->getTexture()->getContentSize().height));
        
        //the 2nd challange
        titleSh = CCLabelTTF::create(wt[1].getTaskInfo().c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                     CCSize(700, 70), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
        titleSh->setAnchorPoint(ccp(0,0));
        titleSh->setPosition(ccp(-aChallangeBG->getContentSize().width/2+110,8));
        titleSh->setColor(ccc3(0, 0, 0));
        mWaklyBG->addChild(titleSh);
        
        backProgress = CCSprite::create("Interfeiss/pause_menu/progresbar_base.png");
        backProgress->setAnchorPoint(ccp(0,0));
        backProgress->setPosition(ccp(-aChallangeBG->getContentSize().width/2+76,-20));
        mWaklyBG->addChild(backProgress);
        
        progress = CCSprite::create("Interfeiss/pause_menu/challenge_progresbar.png");
        progress->setAnchorPoint(ccp(0,0));
        progress->setPosition(ccp(5,4));
        
        value = User::getInstance()->getWeeklyChallenges().getTask2Progress();
        max = wt[1].getRequirement();
        backProgress->addChild(progress);
        progress->setTextureRect(CCRect(0, 0,
                                        progress->getTexture()->getContentSize().width*((float)value/max),
                                        progress->getTexture()->getContentSize().height));
        
        //the 3rd challange
        titleSh = CCLabelTTF::create(wt[2].getTaskInfo().c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                     CCSize(700, 70), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
        titleSh->setAnchorPoint(ccp(0,0));
        titleSh->setPosition(ccp(-aChallangeBG->getContentSize().width/2+110,-aChallangeBG->getContentSize().height/4-4));
        titleSh->setColor(ccc3(0, 0, 0));
        mWaklyBG->addChild(titleSh);
        
        backProgress = CCSprite::create("Interfeiss/pause_menu/progresbar_base.png");
        backProgress->setAnchorPoint(ccp(0,0));
        backProgress->setPosition(ccp(-aChallangeBG->getContentSize().width/2+76,-aChallangeBG->getContentSize().height/4-32));
        mWaklyBG->addChild(backProgress);
        
        progress = CCSprite::create("Interfeiss/pause_menu/challenge_progresbar.png");
        progress->setAnchorPoint(ccp(0,0));
        progress->setPosition(ccp(5,4));
        
        value = User::getInstance()->getWeeklyChallenges().getTask3Progress();
        max = wt[2].getRequirement();
        backProgress->addChild(progress);
        progress->setTextureRect(CCRect(0, 0,
                                        progress->getTexture()->getContentSize().width*((float)value/max),
                                        progress->getTexture()->getContentSize().height));

    }
    
    //Get the current active mission
    /*
    MissionSet aMission = User::getInstance()->getMissionManager().GetActiveMission();
    
    int theMissionID = aMission.id-1;
    
    int aMissionAmount = aMission.mSubMission_1.size();
    
    int aMissionOffsetX = 0;//-20
    int aMissionSpaceY2 = -20;
    int aMissionSpaceY1 = 30;
    
    if(aMissionAmount == 2)
    {
        aMissionSpaceY1 = 60;
        aMissionSpaceY2 = 0;
        aMissionOffsetX = -46;
    }
    else if(aMissionAmount == 1)
    {
        aMissionSpaceY1 = 50;
        aMissionSpaceY2 = 0;
        
        aMissionOffsetX = -66;
    }
    
    //The mission name !!!
    std::stringstream theMission;
    theMission<<"Mission set #"<<(aMission.id);
    std::string aMissionFinal = theMission.str();
    
    CCLabelTTF* mission_header = CCLabelTTF::create(aMissionFinal.c_str(),
                                                     FONT_SKRANJI, TITLE_FONT_SIZE*0.9,
                                                     CCSize(400, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    mission_header->setColor(ccc3(0, 0, 0));
    mission_header->setPositionX(aMissionDwarf->getContentSize().width/2);
    mission_header->setPositionY(aMissionDwarf->getContentSize().height-40);
    aMissionDwarf->addChild(mission_header);
    
    //Lets check the missions !!!
    std::vector<SubMission> aSubMiss = aMission.mSubMission_1;
    std::string aName = User::getInstance()->getMissionManager().GetSubMissionText(aMission.id-1,0,true,false);
    
    std::stringstream theBoxImage;
    bool aCompleted = false;
    if(aSubMiss[0].current_amount>=aSubMiss[0].amount || aSubMiss[0].best_amount>=aSubMiss[0].amount)
    {
        aCompleted = true;
        theBoxImage<<"Interfeiss/checkbox_checked.png";
    }
    else
    {
        theBoxImage<<"Interfeiss/checkbox_empty.png";
    }
    std::string aBoxImage = theBoxImage.str();
    
    CCSprite* aSubMissionBox = CCSprite::create(aBoxImage.c_str());
    aSubMissionBox->setScale(0.65f);
    aSubMissionBox->setPositionX(65);
    aSubMissionBox->setPositionY(aMissionDwarf->getContentSize().height/2+aMissionSpaceY1+aMissionOffsetX);
    aMissionDwarf->addChild(aSubMissionBox);
    
    //the 1st line !!!
    CCLabelTTF* mMissionBox_Text1 = CCLabelTTF::create("11111111111111",
                                           FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                           CCSize(450, 40), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    mMissionBox_Text1->setAnchorPoint(ccp(0,0.5f));
    mMissionBox_Text1->setColor(ccc3(73, 123, 122));
    mMissionBox_Text1->setPositionX(aSubMissionBox->getPositionX()+40);
    mMissionBox_Text1->setPositionY(aSubMissionBox->getPositionY()+8);
    aMissionDwarf->addChild(mMissionBox_Text1);
    
    //the 2nd line !!!
    CCLabelTTF* mMissionBox_Text1p = CCLabelTTF::create("222222222222",
                                            FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                            CCSize(450, 40), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    mMissionBox_Text1p->setAnchorPoint(ccp(0,0.5f));
    mMissionBox_Text1p->setColor(ccc3(181, 149, 85));
    mMissionBox_Text1p->setPositionX(aSubMissionBox->getPositionX()+40);
    mMissionBox_Text1p->setPositionY(aSubMissionBox->getPositionY()-14);
    aMissionDwarf->addChild(mMissionBox_Text1p);
    
    mMissionBox_Text1->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,0,false,true).c_str());
    mMissionBox_Text1p->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,0,true,false).c_str());
    
    //==========================================
    
    //The 2nd mission
    if(aMissionAmount>1)
    {
        aName = User::getInstance()->getMissionManager().GetSubMissionText(aMission.id-1,1,true,false);
        aCompleted = false;
        theBoxImage.str("");
        
        if(aSubMiss[1].current_amount>=aSubMiss[1].amount || aSubMiss[1].best_amount>=aSubMiss[1].amount)
        {
            aCompleted = true;
            theBoxImage<<"Interfeiss/checkbox_checked.png";
        }
        else
        {
            theBoxImage<<"Interfeiss/checkbox_empty.png";
        }
        aBoxImage = theBoxImage.str();
        
        CCSprite* aSubMissionBox_2 = CCSprite::create(aBoxImage.c_str());
        aSubMissionBox_2->setScale(0.65f);
        aSubMissionBox_2->setPositionX(65);
        aSubMissionBox_2->setPositionY(aMissionDwarf->getContentSize().height/2+aMissionSpaceY2+aMissionOffsetX);
        aMissionDwarf->addChild(aSubMissionBox_2);
        
        CCLabelTTF* mMissionBox_Text2 = CCLabelTTF::create("",
                                               FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                               CCSize(400, 80), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        mMissionBox_Text2->setAnchorPoint(ccp(0,0.5f));
        mMissionBox_Text2->setColor(ccc3(73, 123, 122));
        mMissionBox_Text2->setPositionX(aSubMissionBox_2->getPositionX()+40);
        mMissionBox_Text2->setPositionY(aSubMissionBox_2->getPositionY()+8);
        aMissionDwarf->addChild(mMissionBox_Text2);
        
        CCLabelTTF* mMissionBox_Text2p = CCLabelTTF::create("",
                                                FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                                CCSize(400, 80), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        mMissionBox_Text2p->setAnchorPoint(ccp(0,0.5f));
        mMissionBox_Text2p->setColor(ccc3(181, 149, 85));
        mMissionBox_Text2p->setPositionX(aSubMissionBox_2->getPositionX()+40);
        mMissionBox_Text2p->setPositionY(aSubMissionBox_2->getPositionY()-14);
        aMissionDwarf->addChild(mMissionBox_Text2p);
        
        mMissionBox_Text2->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,1,false,true).c_str());
        mMissionBox_Text2p->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,1,true,false).c_str());
    }

    //The 3rd mission
    if(aMissionAmount>2)
    {
        aName = User::getInstance()->getMissionManager().GetSubMissionText(aMission.id-1,2,true,false);
        aCompleted = false;
        theBoxImage.str("");
        
        if(aSubMiss[2].current_amount>=aSubMiss[2].amount || aSubMiss[2].best_amount>=aSubMiss[2].amount)
        {
            aCompleted = true;
            theBoxImage<<"Interfeiss/checkbox_checked.png";
        }
        else
        {
            theBoxImage<<"Interfeiss/checkbox_empty.png";
        }
        aBoxImage = theBoxImage.str();
        
        CCSprite* aSubMissionBox_3 = CCSprite::create(aBoxImage.c_str());
        aSubMissionBox_3->setScale(0.65f);
        aSubMissionBox_3->setPositionX(65);
        aSubMissionBox_3->setPositionY(aMissionDwarf->getContentSize().height/2-70+aMissionOffsetX);
        aMissionDwarf->addChild(aSubMissionBox_3);
        
        CCLabelTTF* mMissionBox_Text3 = CCLabelTTF::create("",
                                               FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                               CCSize(400, 80), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        mMissionBox_Text3->setAnchorPoint(ccp(0,0.5f));
        mMissionBox_Text3->setColor(ccc3(73, 123, 122));
        mMissionBox_Text3->setPositionX(aSubMissionBox_3->getPositionX()+40);
        mMissionBox_Text3->setPositionY(aSubMissionBox_3->getPositionY()+8);
        aMissionDwarf->addChild(mMissionBox_Text3);
        
        CCLabelTTF* mMissionBox_Text3p = CCLabelTTF::create("",
                                                FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                                CCSize(400, 80), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        mMissionBox_Text3p->setAnchorPoint(ccp(0,0.5f));
        mMissionBox_Text3p->setColor(ccc3(181, 149, 85));
        mMissionBox_Text3p->setPositionX(aSubMissionBox_3->getPositionX()+40);
        mMissionBox_Text3p->setPositionY(aSubMissionBox_3->getPositionY()-14);
        aMissionDwarf->addChild(mMissionBox_Text3p);
        
        mMissionBox_Text3->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,2,false,true).c_str());
        mMissionBox_Text3p->setString(User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,2,true,false).c_str());
    }
    */
    
    
    //Lets add the mission progress bar here !!!
    //-----------------------------------------------------------------
    //Create the ultra combo bar !!!
    
    CCSprite* aBaseProgress = CCSprite::create("small_dot_red.png");
    aBaseProgress->setVisible(false); // Disabled for now !!!
    aMissionDwarf->addChild(aBaseProgress);
    aBaseProgress->setPosition(ccp(300,150));
    aBaseProgress->setScaleX(0.5);
    aBaseProgress->setScaleY(1.2);
    
    CCSprite* aDummy = CCSprite::create("Interfeiss/challenges/daily/progress_days.png");
    aDummy->setPosition(ccp(-10,0));
    aBaseProgress->addChild(aDummy);
    
    int aOneStep = aDummy->getContentSize().width/4;
    

    
    _mission_progress_bar_1 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
    _mission_progress_bar_1->setScaleX(0.25);
    _mission_progress_bar_1->setAnchorPoint(ccp(0,0.5f));
    _mission_progress_bar_1->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6,aDummy->getPositionY()));
    aBaseProgress->addChild(_mission_progress_bar_1);
    
    _mission_progress_bar_2 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
    _mission_progress_bar_2->setAnchorPoint(ccp(0,0.5f));
    _mission_progress_bar_2->setScaleX(0.25);
    _mission_progress_bar_2->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep,aDummy->getPositionY()));
    aBaseProgress->addChild(_mission_progress_bar_2);
    
    _mission_progress_bar_3 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
    _mission_progress_bar_3->setAnchorPoint(ccp(0,0.5f));
    _mission_progress_bar_3->setScaleX(0.25);
    _mission_progress_bar_3->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep*2,aDummy->getPositionY()));
    aBaseProgress->addChild(_mission_progress_bar_3);
    
    _mission_progress_bar_4 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
    _mission_progress_bar_4->setAnchorPoint(ccp(0,0.5f));
    _mission_progress_bar_4->setScaleX(0.23);
    _mission_progress_bar_4->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep*3,aDummy->getPositionY()));
    aBaseProgress->addChild(_mission_progress_bar_4);
    
    
    
    CCSprite* aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
    aDummySpe->setPosition(ccp(-aOneStep,0));
    aBaseProgress->addChild(aDummySpe);
    
    aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
    aDummySpe->setPosition(ccp(0,0));
    aBaseProgress->addChild(aDummySpe);
    
    aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
    aDummySpe->setPosition(ccp(aOneStep,0));
    aBaseProgress->addChild(aDummySpe);
    
    
    //-----------------------------------------------------------------
    
    //Create the 3 stars
    _mission_star_1 = CCSprite::create("new_mission/zvaigzne_final.png");
    _mission_star_1->setPosition(ccp(-aOneStep,-45));
    _mission_star_1->setOpacity(80);
    _mission_star_1->setScaleX(2.2);
    _mission_star_1->setScaleY(1.0);
    aBaseProgress->addChild(_mission_star_1,100+1);
    
    _mission_star_2 = CCSprite::create("new_mission/zvaigzne_final.png");
    _mission_star_2->setPosition(ccp(0,-45));
    _mission_star_2->setOpacity(80);
    _mission_star_2->setScaleX(2.2);
    _mission_star_2->setScaleY(1.0);
    aBaseProgress->addChild(_mission_star_2,100+1);
    
    _mission_star_3 = CCSprite::create("new_mission/zvaigzne_final.png");
    _mission_star_3->setPosition(ccp(aOneStep,-45));
    _mission_star_3->setOpacity(80);
    _mission_star_3->setScaleX(2.2);
    _mission_star_3->setScaleY(1.0);
    aBaseProgress->addChild(_mission_star_3,100+1);
    
    
//    UpdateMissionStarsPause();
    
    //------------------------------------------------------------------
    
    //The reward
//    CCLabelTTF* mission_reward = CCLabelTTF::create("Reward:",
//                                                   FONT_SKRANJI, TITLE_FONT_SIZE*0.8,
//                                                   CCSize(240, 52), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
//    mission_reward->setAnchorPoint(ccp(0,0.5f));
//    mission_reward->setColor(ccc3(0, 0, 0));
//    mission_reward->setPositionX(aSubMissionBox->getPositionX());
//    mission_reward->setPositionY(50);
//    aMissionDwarf->addChild(mission_reward);
//    
//    theMission.str("");
//    theMission<<(aMission.reward_name);
//    aMissionFinal = theMission.str();
//    
//    CCLabelTTF* mission_reward_result = CCLabelTTF::create(aMissionFinal.c_str(),
//                                                           "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.6,
//                                                           CCSize(260, 120), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//    mission_reward_result->setAnchorPoint(ccp(0,0.5f));
//    mission_reward_result->setRotation(2);
//    mission_reward_result->setColor(ccc3(255, 255,255));
//    mission_reward_result->setPositionX(aSubMissionBox->getPositionX()+160);
//    mission_reward_result->setPositionY(54);
//    
//    mission_reward_result->enableShadow(CCSize(0,1), 1.5f, 2,true);
////    mission_reward_result->enableStroke(ccRED, 3,true);
//    aMissionDwarf->addChild(mission_reward_result);
    
    
    //The challange stuff !!! for now disabled
//    CCSprite* aChallangeBase = CCSprite::create("Interfeiss/pause_menu/challenge_base.png");
//    aChallangeBase->setAnchorPoint(ccp(0.5,0.5));
//    aChallangeBase->setPositionX(visibleSize.width/2);
//    aChallangeBase->setPositionY(visibleSize.height/2-aMissionDwarf->getContentSize().height/2);
//    addChild(aChallangeBase);
    
    //The lower buttons
    bool musicEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true);
    bool soundsEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("soundsEnabled", true);
    bool vibrationEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("vibrationEnabled", true);
    
    CCMenuItemImage* btn_home = CCMenuItemImage::create(
                                                            "Interfeiss/pause_menu/btn_home.png",
                                                            "Interfeiss/pause_menu/btn_home.png",
                                                            this,
                                                            menu_selector(PauseScene::menuMainMenuCallback));
    
    CCMenuItemImage* s1 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_sound_off.png",
                                                  "Interfeiss/pause_menu/btn_sound.png", this, NULL);
    
    CCMenuItemImage* s2 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_sound.png",
                                                  "Interfeiss/pause_menu/btn_sound_off.png", this, NULL);
    
    CCMenuItemToggle* soundButton = CCMenuItemToggle::createWithTarget(
                                                                       this, menu_selector(PauseScene::menuSoundCallback), s1, s2, NULL);
    soundButton->setSelectedIndex(soundsEnabled);
    
    CCMenuItemImage* m1 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_music_off.png",
                                                  "Interfeiss/pause_menu/btn_music.png", this, NULL);
    
    CCMenuItemImage* m2 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_music.png",
                                                  "Interfeiss/pause_menu/btn_music_off.png", this, NULL);
    
    CCMenuItemToggle* musicButton = CCMenuItemToggle::createWithTarget(
                                                                       this, menu_selector(PauseScene::menuMusicCallback), m1, m2, NULL);
    musicButton->setSelectedIndex(musicEnabled);
    
    CCMenuItemImage* v1 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_vbration_off.png",
                                                  "Interfeiss/pause_menu/btn_vbration.png", this, NULL);
    
    CCMenuItemImage* v2 = CCMenuItemImage::create("Interfeiss/pause_menu/btn_vbration.png",
                                                  "Interfeiss/pause_menu/btn_vbration_off.png", this, NULL);
    
    CCMenuItemToggle* vibrateButton = CCMenuItemToggle::createWithTarget(
                                                                       this, menu_selector(PauseScene::menuVibrationCallback), v1, v2, NULL);
    vibrateButton->setSelectedIndex(vibrationEnabled);
    
    CCMenuItemImage* btn_tutorial = CCMenuItemImage::create(
                                                        "Interfeiss/pause_menu/btn_tutorial.png",
                                                        "Interfeiss/pause_menu/btn_tutorial.png",
                                                        this,
                                                        menu_selector(PauseScene::menuTutorialCallback));
    
    CCMenu* lowBar = CCMenu::create(btn_home, soundButton,musicButton,vibrateButton,btn_tutorial,resumeButton, NULL);
    lowBar->alignItemsHorizontallyWithPadding(20);
    lowBar->setPosition(visibleSize.width/2, 70);
    lowBar->setTag(2222);
    this->addChild(lowBar, 1);
    
//    btn_tutorial->setPositionX(20);
    
    //Add some text fields for buttons !!!
    // skaitlis
    CCLabelTTF* button_txt_home = CCLabelTTF::create("Home",
                               FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                               CCSize(100, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    button_txt_home->setColor(ccc3(33, 99, 78));
    button_txt_home->setPositionX(lowBar->getPositionX()+btn_home->getPositionX());
    button_txt_home->setPositionY(24);
    addChild(button_txt_home);
    
    CCLabelTTF* button_txt_sound = CCLabelTTF::create("Sound",
                                                     FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                                     CCSize(100, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    button_txt_sound->setColor(ccc3(33, 99, 78));
    button_txt_sound->setPositionX(lowBar->getPositionX()+soundButton->getPositionX());
    button_txt_sound->setPositionY(24);
    addChild(button_txt_sound);
    
    CCLabelTTF* button_txt_music = CCLabelTTF::create("Music",
                                                      FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                                      CCSize(100, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    button_txt_music->setColor(ccc3(33, 99, 78));
    button_txt_music->setPositionX(lowBar->getPositionX()+musicButton->getPositionX());
    button_txt_music->setPositionY(24);
    addChild(button_txt_music);
    
    CCLabelTTF* button_txt_vib = CCLabelTTF::create("Vibration",
                                                      FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                                      CCSize(100, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    button_txt_vib->setColor(ccc3(33, 99, 78));
    button_txt_vib->setPositionX(lowBar->getPositionX()+vibrateButton->getPositionX());
    button_txt_vib->setPositionY(24);
    addChild(button_txt_vib);
    
    CCLabelTTF* button_txt_tut = CCLabelTTF::create("Tutorial",
                                                    FONT_SKRANJI, TITLE_FONT_SIZE*0.4,
                                                    CCSize(100, 52), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    button_txt_tut->setColor(ccc3(33, 99, 78));
    button_txt_tut->setPositionX(lowBar->getPositionX()+btn_tutorial->getPositionX());
    button_txt_tut->setPositionY(24);
    addChild(button_txt_tut);
    
    
    /*//The old pause scene
    CCSprite* labels = CCSprite::create("Interfeiss/pause_menu/pause_text.png");
    labels->setAnchorPoint(ccp(0, 1));
    labels->setPositionX(80);
    labels->setPositionY(visibleSize.height - 110);
    addChild(labels);
    
    CCMenuItemImage* s1 = CCMenuItemImage::create("Interfeiss/checkbox_empty.png",
                                                  "Interfeiss/checkbox_checked.png", this, NULL);
    
    CCMenuItemImage* s2 = CCMenuItemImage::create("Interfeiss/checkbox_checked.png",
                                                  "Interfeiss/checkbox_empty.png", this, NULL);
    
    CCMenuItemToggle* soundButton = CCMenuItemToggle::createWithTarget(
                                                                       this, menu_selector(PauseScene::menuSoundCallback), s1, s2, NULL);
    
    
    CCMenuItemImage* m1 = CCMenuItemImage::create("Interfeiss/checkbox_empty.png",
                                                  "Interfeiss/checkbox_checked.png", this, NULL);
    
    CCMenuItemImage* m2 = CCMenuItemImage::create("Interfeiss/checkbox_checked.png",
                                                  "Interfeiss/checkbox_empty.png", this, NULL);
    
    CCMenuItemToggle* musicButton = CCMenuItemToggle::createWithTarget(
                                                                       this, menu_selector(PauseScene::menuMusicCallback), m1, m2, NULL);
    
    CCMenuItemImage* v1 = CCMenuItemImage::create("Interfeiss/checkbox_empty.png",
                                                  "Interfeiss/checkbox_checked.png", this, NULL);
    
    CCMenuItemImage* v2 = CCMenuItemImage::create("Interfeiss/checkbox_checked.png",
                                                  "Interfeiss/checkbox_empty.png", this, NULL);
    
    CCMenuItemToggle* vibrationButton = CCMenuItemToggle::createWithTarget(
                                                                       this, menu_selector(PauseScene::menuVibrationCallback), v1, v2, NULL);
    
    soundButton->setPosition(0, 185);
    musicButton->setPosition(0, 95);
    vibrationButton->setPosition(0, 5);
    
    bool musicEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true);
    bool soundsEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("soundsEnabled", true);
    bool vibrationEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("vibrationEnabled", true);
    
    musicButton->setSelectedIndex(musicEnabled);
    soundButton->setSelectedIndex(soundsEnabled);
    vibrationButton->setSelectedIndex(vibrationEnabled);
    
    CCMenu* musicMenu = CCMenu::create(soundButton, musicButton, vibrationButton, NULL);
    musicMenu->setPosition(380, 270);
    this->addChild(musicMenu, 1);
    
    
    CCMenuItemImage* resumeButton = CCMenuItemImage::create(
                                                              "Interfeiss/pause_menu/resume_game_btn.png",
                                                              "Interfeiss/pause_menu/resume_game_btn_pressed.png",
                                                              this,
                                                              menu_selector(PauseScene::menuResumeCallback));
    resumeButton->setAnchorPoint(ccp(0, 0.5));
    CCMenuItemImage* mainMenuButton = CCMenuItemImage::create(
                                                             "Interfeiss/pause_menu/main_menu_btn.png",
                                                             "Interfeiss/pause_menu/main_menu_btn_pressed.png",
                                                             this,
                                                             menu_selector(PauseScene::menuMainMenuCallback));
    mainMenuButton->setAnchorPoint(ccp(0, 0.5));
    CCMenuItemImage* tutorialButton = CCMenuItemImage::create(
                                                             "Interfeiss/options/tutorial_btn.png",
                                                             "Interfeiss/options/tutorial_btn_pressed.png",
                                                             this,
                                                             menu_selector(PauseScene::menuTutorialCallback));
    tutorialButton->setAnchorPoint(ccp(0, 0.5));
    
    CCMenu* mainMenu = CCMenu::create(resumeButton, mainMenuButton, tutorialButton, NULL);
    mainMenu->alignItemsVerticallyWithPadding(-40);
    mainMenu->setPositionY(350);
    mainMenu->setPositionX(450);
    this->addChild(mainMenu, 1);
    */

    return true;
}

void PauseScene::UpdateMissionStarsPause()
{
    GameScene* gameScene = static_cast<GameScene*>(this->getParent());
    
    mMaxBarPoints = gameScene->_mission_star_points_3;
    mTotalPointsInGame = gameScene->mTotalPointsInGame;
    
    _mission_star_points_1 = gameScene->_mission_star_points_1;
    _mission_star_points_2 = gameScene->_mission_star_points_2;
    _mission_star_points_3 = gameScene->_mission_star_points_3;
    
    //maybe latter add some tween stuff !!!
    
    //Check by current mission and points
    if(mTotalPointsInGame>=_mission_star_points_1){
        _mission_star_1->setOpacity(255);
    }
    else{
        _mission_star_1->setOpacity(80);
    }
    
    if(mTotalPointsInGame>=_mission_star_points_2){
        _mission_star_2->setOpacity(255);
    }
    else{
        _mission_star_2->setOpacity(80);
    }
    
    if(mTotalPointsInGame>=_mission_star_points_3){
        _mission_star_3->setOpacity(255);
    }
    else{
        _mission_star_3->setOpacity(80);
    }
    
    //Check the points for each bar
    int aDummyPoints = mTotalPointsInGame;
    if(aDummyPoints>_mission_star_points_1){
        aDummyPoints = _mission_star_points_1;
    }
    _mission_progress_bar_1->setTextureRect(CCRect(0, 0,
                                                   _mission_progress_bar_1->getTexture()->getContentSize().width*((float)aDummyPoints / _mission_star_points_1),
                                                   _mission_progress_bar_1->getTexture()->getContentSize().height));
    
    aDummyPoints = mTotalPointsInGame-_mission_star_points_1;
    if(aDummyPoints<0){
        aDummyPoints = 0;
    }
    else if(aDummyPoints>_mission_star_points_2-_mission_star_points_1){
        aDummyPoints = _mission_star_points_2-_mission_star_points_1;
    }
    
    _mission_progress_bar_2->setTextureRect(CCRect(0, 0,
                                                   _mission_progress_bar_2->getTexture()->getContentSize().width*
                                                   ((float)aDummyPoints / (_mission_star_points_2-_mission_star_points_1)),
                                                   _mission_progress_bar_2->getTexture()->getContentSize().height));
    
    aDummyPoints = mTotalPointsInGame-_mission_star_points_2;
    if(aDummyPoints<0){
        aDummyPoints = 0;
    }
    else if(aDummyPoints>_mission_star_points_3-_mission_star_points_2){
        aDummyPoints = _mission_star_points_3-_mission_star_points_2;
    }
    
    _mission_progress_bar_3->setTextureRect(CCRect(0, 0,
                                                   _mission_progress_bar_3->getTexture()->getContentSize().width*
                                                   ((float)aDummyPoints / (_mission_star_points_3-_mission_star_points_2)),
                                                   _mission_progress_bar_3->getTexture()->getContentSize().height));
    
    aDummyPoints = mTotalPointsInGame-_mission_star_points_3;
    if(aDummyPoints<0){
        aDummyPoints = 0;
    }
    else if(aDummyPoints>(_mission_star_points_3+100)-_mission_star_points_3){
        aDummyPoints = (_mission_star_points_3+100)-_mission_star_points_3;
    }
    
    _mission_progress_bar_4->setTextureRect(CCRect(0, 0,
                                                   _mission_progress_bar_4->getTexture()->getContentSize().width*
                                                   ((float)aDummyPoints / ((_mission_star_points_3+100)-_mission_star_points_3)),
                                                   _mission_progress_bar_4->getTexture()->getContentSize().height));
}

//CCLayer PauseScene::CreateMissionText(std::string str,)
//{
//    CCLayer* aBase;
//    
//    int pos = 0;
//    int i = 0;
//    while(pos<str.length())
//    {
//        int end = 0;
//        int lastCut = -1;
//        bool finished=NO;
//        
//        while()
//    }
//    
//    while (pos<[str length]) {
//        int end = 0;
//        int lastCut = -1;
//        bool finished=NO;
//        while (finished==NO) {
//            CGSize actualSize = [[str substringWithRange:NSMakeRange(pos, end)] sizeWithFont:[UIFont fontWithName:name size:size]];
//            
//            if (actualSize.width > dimensions.width || pos+end == [str length]) {
//                if (pos+end == [str length] && actualSize.width <= dimensions.width) lastCut = end;
//                finished=YES;
//            }
//            else {
//                if ([[str substringWithRange:NSMakeRange(pos+end, 1)] isEqualToString:@" "] || [[str substringWithRange:NSMakeRange(pos+end, 1)] isEqualToString:@"."] || [[str substringWithRange:NSMakeRange(pos+end, 1)] isEqualToString:@","]) {
//                    lastCut = end;
//                }
//                end++;
//            }
//        }
//        
//        NSString * strLine = [str substringWithRange:NSMakeRange(pos, lastCut)];
//        
//        CCLabelTTF * line = [CCLabelTTF labelWithString:strLine dimensions:CGSizeMake(dimensions.width, size*2) hAlignment:alignment fontName:name fontSize:size];
//        [line setAnchorPoint:ccp(0,1)];
//        [line setPosition:ccp(0,-i*space)];
//        [self addChild:line];
//        
//        pos=pos+lastCut;
//        i++;
//    }
//}

void PauseScene::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void PauseScene::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool PauseScene::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(getChildByTag(1111))
        return false;
    
    return true;
}

void PauseScene::menuResumeCallback(CCObject* sender)
{
    if(getChildByTag(1111))
        return;
    
    
    AnalyticX::flurryLogEvent("Game Menu Pause: Continue Game");
    
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Popup_Exit").c_str());
    
    GameScene* gameScene = static_cast<GameScene*>(this->getParent());
    gameScene->onResumeFromPause();
    
	this->getParent()->removeChild(this);
}

void PauseScene::menuMainMenuCallback(cocos2d::CCObject *sender)
{
    if(getChildByTag(1111))
        return;
    User::getInstance()->getMissionManager().Reset(User::getInstance()->getMissionManager().GetActiveMissionID()); 
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
    AnalyticX::flurryLogEvent("Game Menu Pause: Exit To Main");
    
    //First show popup that all progress will be lost !!!
//    PopUp::create(this, "All progress will be lost - are you sure you want to continue?", (CallBack)(&PauseScene::menuContinueExit),true,true);
    
    //one more bg???
    
    CCScene* options = BeforeQuitScene::scene();
    options->setTag(1111);
    addChild(options,10);
    
    CCMenu* aMenu = (CCMenu*)getChildByTag(2222);
    aMenu->setEnabled(false);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
    
    //Complete the 1st session!!!
//    if(User::getInstance()->FirstSessionDone==0)
//    {
//        User::getInstance()->FirstSessionDone = 1;
//        cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("userFirstSession", 1);
//        cocos2d::CCUserDefault::sharedUserDefault()->flush();
//    }
    
//    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, options);
//	CCDirector::sharedDirector()->replaceScene(transition);
    
//    CCScene* tutorial = MainMenuScene::scene();
//	CCTransitionScene* transition = CCTransitionProgressInOut::create(0.5f, tutorial);
//	CCDirector::sharedDirector()->replaceScene(transition);
}

void PauseScene::OnClousePop()
{
    CCMenu* aMenu = (CCMenu*)getChildByTag(2222);
    aMenu->setEnabled(true);
    
    removeChildByTag(1111);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Popup_Exit").c_str());
}

void PauseScene::menuContinueExit()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    AnalyticX::flurryLogEvent("Game Menu Pause: Continue Exit");
    
    CCScene* tutorial = MainMenuScene::scene();
	CCTransitionScene* transition = CCTransitionProgressInOut::create(0.5f, tutorial);
	CCDirector::sharedDirector()->replaceScene(transition);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void PauseScene::menuTutorialCallback(cocos2d::CCObject *sender)
{
//    CCScene* tutorial = TutorialScene::scene();
//	CCTransitionScene* transition = CCTransitionProgressInOut::create(0.5f, tutorial);
//	CCDirector::sharedDirector()->pushScene(transition);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
//    TutorialScene* tutorial = TutorialScene::create();
//    tutorial->setScale(0);
//    CCActionInterval* actionTo = CCScaleTo::create(0.5, 1);
//    CCAction* action = CCEaseOut::create(actionTo, 3);
//    tutorial->runAction(action);
//    
//    addChild(tutorial, 10);
    
    TutorialScene* tutorial = TutorialScene::create(false);
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    tutorial->setScale(0.87f);
    tutorial->setTag(9119);
    tutorial->setPosition(ccp(-42,-34));
    
    CCSprite* aFrame = CCSprite::create("Interfeiss/stats/bigpop.png");
    aFrame->setPosition(ccp(visibleSize.width/2,visibleSize.height/2));
    aFrame->setTag(1001);
    aFrame->addChild(tutorial);
    addChild(aFrame,100);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void PauseScene::OnExitTutorial()
{
    CCSprite* aSprite = (CCSprite*)getChildByTag(1001);
    removeChild(aSprite);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

void PauseScene::menuSoundCallback(cocos2d::CCObject *sender)
{
    int enabled = dynamic_cast<cocos2d::CCMenuItemToggle*>(sender)->getSelectedIndex();
    if (enabled)
    {
        AnalyticX::flurryLogEvent("Game Menu Pause: Enable Sound FX");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("soundsEnabled", true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
    }
    else
    {
        AnalyticX::flurryLogEvent("Game Menu Pause: Disable Sound FX");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("soundsEnabled", false);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
    
    CCUserDefault::sharedUserDefault()->flush();
}

void PauseScene::menuMusicCallback(cocos2d::CCObject *sender)
{
    int enabled = dynamic_cast<cocos2d::CCMenuItemToggle*>(sender)->getSelectedIndex();
    if (enabled)
    {
        AnalyticX::flurryLogEvent("Game Menu Pause: Enable Music");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("musicEnabled", true);
        
        GameScene* gameScene = static_cast<GameScene*>(this->getParent());
        gameScene->FromOptionsEnableMusic();
//        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
    else
    {
        AnalyticX::flurryLogEvent("Game Menu Pause: Disable Music");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("musicEnabled", false);
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
    
    CCUserDefault::sharedUserDefault()->flush();
}

void PauseScene::menuVibrationCallback(cocos2d::CCObject *sender)
{
    if(getChildByTag(1111))
        return;
    
    int enabled = dynamic_cast<cocos2d::CCMenuItemToggle*>(sender)->getSelectedIndex();
    if (enabled)
    {
        AnalyticX::flurryLogEvent("Game Menu Pause: Enable Vibro");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("vibrationEnabled", true);
    }
    else
    {
        AnalyticX::flurryLogEvent("Game Menu Pause: Disable Vibro");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("vibrationEnabled", false);
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Toggle").c_str());
}

//The magic of challanges part !!!
void PauseScene::OnShowWeakly(cocos2d::CCObject *sender)
{
    if(getChildByTag(1111))
        return;
    
    if(mWeaklyActive)
        return;
    mWeaklyActive = true;
    
    AnalyticX::flurryLogEvent("Game Menu Pause: Tab Weekly");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("TabSwap").c_str());
    
    reorderChild(mWeaklyTab,1);
    mWeaklyTab->setColor(ccc3(255,255,255));
    
    reorderChild(mDailyTab,-1);
    mDailyTab->setColor(ccc3(160,160,160));
    
    //Clear all the daily stuff if had and show weakly part !!!
    mWaklyBG->setVisible(true);
    mDailyBG->setVisible(false);
}

void PauseScene::OnShowDaily(cocos2d::CCObject *sender)
{
    if(getChildByTag(1111))
        return;
    
    if(!mWeaklyActive)
        return;
    mWeaklyActive = false;
    
    AnalyticX::flurryLogEvent("Game Menu Pause: Tab Daily");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("TabSwap").c_str());
    
    reorderChild(mDailyTab,1);
    mDailyTab->setColor(ccc3(255,255,255));
    
    reorderChild(mWeaklyTab,-1);
    mWeaklyTab->setColor(ccc3(160,160,160));
    
    mWaklyBG->setVisible(false);
    mDailyBG->setVisible(true);
}

CCSprite* PauseScene::getBarIconPause(CrystalColor color)
{
    std::string path;
    switch (color)
    {
        case CRYSTAL_COLOR_BLUE:
            path = "Interfeiss/challenges/ingame_pop_up/blue_count.png";
            break;
        case CRYSTAL_COLOR_GREEN:
            path = "Interfeiss/challenges/ingame_pop_up/green_count.png";
            break;
        case CRYSTAL_COLOR_RED:
            path = "Interfeiss/challenges/ingame_pop_up/red_count.png";
            break;
        case CRYSTAL_COLOR_YELLOW:
            path = "Interfeiss/challenges/ingame_pop_up/yellow_count.png";
            break;
        default:
            path = "Interfeiss/challenges/ingame_pop_up/yellow_count.png";
            break;
    }
    
    return CCSprite::create(path.c_str());
}

