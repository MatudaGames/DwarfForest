//
//  ChallengesScene.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 09/03/14.
//
//

#include "ChallengesScene.h"
#include "TutorialScene.h"
#include "IntroScene.h"
#include "StaticSceneManager.h"
#include <SimpleAudioEngine.h>

#include "JniCalls.h"
#include "User.h"
#include "AppMacros.h"
#include "MainMenuScene.h"

#include "WeeklyChallenges.h"
#include "Utils.h"

#include "AnalyticX.h"
#include "WorldMap.h"

USING_NS_CC;

CCScene* ChallengesScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ChallengesScene *layer = ChallengesScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

//void ChallengesScene::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
//{
//    CCLOG("Did touch screen");
//}
//
//void ChallengesScene::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
//{
//    
//}
//
//bool ChallengesScene::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
//{
//    return true;
//}

void ChallengesScene::onEnter()
{
    CCLayer::onEnter();
    update();
}

void ChallengesScene::OnFinishedEnter()
{
    mDidFinishEnter = true;
}

void ChallengesScene::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool ChallengesScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Popup_Enter").c_str());
    
    mWeeklyBase = NULL;
    mDailyBase = NULL;
    
    //    setTouchPriority(-128);
    
    mDidFinishEnter = false;
	
    // enables back button on android
    this->setKeypadEnabled(true);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //The bg
    CCLayerColor* aBG = CCLayerColor::create(ccc4(0,0,0,0),visibleSize.width,visibleSize.height);
    aBG->setTag(888);
    addChild(aBG);
    
    //Run the intro for bg
    CCFadeTo* aFade = CCFadeTo::create(0.5f,64);
    aBG->runAction(aFade);
    
    //The other screen part !!!
    aBG2 = CCSprite::create("small_dot_red.png");
    aBG2->setPosition(ccp(visibleSize.width/2,visibleSize.height-100));
    addChild(aBG2);
    
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(visibleSize.width/2,visibleSize.height/2));
    CCEaseElasticOut* aEase1 = CCEaseElasticOut::create(aMove1,1.0f);
    CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(ChallengesScene::OnFinishedEnter));
    CCSequence* aSe2 = CCSequence::create(aEase1,func2,NULL);
    aBG2->runAction(aSe2);
    
    CCSprite* aBase = CCSprite::create("Interfeiss/challenges/mainboard.png");
    aBase->setPosition(ccp(aBG2->getContentSize().width/2+40,aBG2->getContentSize().height/2-28));//40
    aBG2->addChild(aBase);
    
    //Add the tabs
    CCSprite* aDummtTab;
    CCLabelTTF* aStoreHeader;
    
    //lest add the not active now
    aDummtTab = CCSprite::create("Interfeiss/challenges/tab_challenges_inactive.png");
    aDummtTab->setPosition(ccp(-396,-178));//-150
    aDummtTab->setTag(101);
    aBG2->addChild(aDummtTab,-1);
    
    //Add some text
    aStoreHeader = CCLabelTTF::create("Weekly", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(195, 75),
                                      kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setRotation(-90);
    aStoreHeader->setPosition(ccp(aDummtTab->getContentSize().width/2,aDummtTab->getContentSize().height/2));
    aStoreHeader->setColor(ccc3(129, 94, 44));
    aDummtTab->addChild(aStoreHeader);
    
    aDummtTab = CCSprite::create("Interfeiss/challenges/tab_challenges_inactive.png");
    aDummtTab->setPosition(ccp(-396,135));//163
    aDummtTab->setTag(201);
    aBG2->addChild(aDummtTab,-1);
    
    aStoreHeader = CCLabelTTF::create("Daily", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(195, 75),
                                      kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setRotation(-90);
    aStoreHeader->setPosition(ccp(aDummtTab->getContentSize().width/2,aDummtTab->getContentSize().height/2));
    aStoreHeader->setColor(ccc3(129, 94, 44));
    aDummtTab->addChild(aStoreHeader);
    
    //=============
    
    aDummtTab = CCSprite::create("Interfeiss/challenges/tab_challenges_active.png");
    aDummtTab->setPosition(ccp(-396,-177));
    aDummtTab->setTag(100);
    aBG2->addChild(aDummtTab);
    
    aStoreHeader = CCLabelTTF::create("Weekly", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(195, 75),
                                      kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setRotation(-90);
    aStoreHeader->setPosition(ccp(aDummtTab->getContentSize().width/2,aDummtTab->getContentSize().height/2));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    aDummtTab->addChild(aStoreHeader);
    
    aDummtTab = CCSprite::create("Interfeiss/challenges/tab_challenges_active.png");
    aDummtTab->setPosition(ccp(-396,137));
    aDummtTab->setTag(200);
    aBG2->addChild(aDummtTab);
    
    aStoreHeader = CCLabelTTF::create("Daily", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.7, CCSize(195, 75),
                                      kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    aStoreHeader->setRotation(-90);
    aStoreHeader->setPosition(ccp(aDummtTab->getContentSize().width/2,aDummtTab->getContentSize().height/2));
    aStoreHeader->setColor(ccc3(92, 20, 11));
    aDummtTab->addChild(aStoreHeader);
    
    //Add the header
    CCSprite* aHeader = CCSprite::create("Interfeiss/challenges/header_title.png");
    aHeader->setPosition(ccp(0,335));
    aBG2->addChild(aHeader);
    
    CCSprite* aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(60,aHeader->getContentSize().height/2));
    aDecor->setColor(ccc3(255,0,0));
    aHeader->addChild(aDecor);
    
    aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(297,aHeader->getContentSize().height/2));
    aDecor->setFlipX(true);
    aDecor->setColor(ccc3(255,0,0));
    aHeader->addChild(aDecor);
    
    aStoreHeader = CCLabelTTF::create("Challenges", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8, CCSize(aHeader->getContentSize().width, aHeader->getContentSize().height), kCCTextAlignmentCenter);
    aStoreHeader->setPosition(ccp(aHeader->getContentSize().width/2,aHeader->getContentSize().height/2-4));
    aStoreHeader->setColor(ccc3(89, 19, 12));
    aHeader->addChild(aStoreHeader);
    
    //The clouse button !!!
    //the extra clouse button!
    CCMenuItemImage* clouseButton = CCMenuItemImage::create(
                                                            "Interfeiss/buttons/x_btn0001.png",
                                                            "Interfeiss/buttons/x_btn0002.png",
                                                            this,
                                                            menu_selector(ChallengesScene::RemovePopup));
    CCMenu* aClouse = CCMenu::create(clouseButton,NULL);
    aClouse->setPosition(ccp(aBase->getContentSize().width-20,aBase->getContentSize().height-20));
    aBase->addChild(aClouse,1);
    
    //Now create the click areas !!!
    CCMenuItemImage* tab_1 = CCMenuItemImage::create(
                                                     "Interfeiss/challenges/tab_challenges_active.png",
                                                     "Interfeiss/challenges/tab_challenges_active.png",
                                                     this,
                                                     menu_selector(ChallengesScene::OnTab1));
    CCMenuItemImage* tab_2 = CCMenuItemImage::create(
                                                     "Interfeiss/challenges/tab_challenges_active.png",
                                                     "Interfeiss/challenges/tab_challenges_active.png",
                                                     this,
                                                     menu_selector(ChallengesScene::OnTab2));
    
    
    
    CCMenu* aTabs = CCMenu::create(tab_1,tab_2,NULL);
    aTabs->alignItemsVerticallyWithPadding(-14);
    aTabs->setOpacity(0);
//    aTabs->setColor(ccRED);
    aTabs->setPosition(ccp(-400,-26));
    aBG2->addChild(aTabs,2);
    
    OnTab1(true);
    
    update();
    
    this->scheduleUpdate();
    
    return true;
}

//tab actions
void ChallengesScene::OnTab1(bool aFirstTime)
{
    if(!aFirstTime)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("TabSwap").c_str());
        mDidFinishEnter = true;
    }
    
    AnalyticX::flurryLogEvent("Menu Challenges: Tab Daily");
    
    aBG2->getChildByTag(100)->setVisible(false);
    aBG2->getChildByTag(101)->setVisible(true);
    
    aBG2->getChildByTag(200)->setVisible(true);
    aBG2->getChildByTag(201)->setVisible(false);
    
    SetActiveTab(1);
}

void ChallengesScene::OnTab2()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("TabSwap").c_str());
    
    AnalyticX::flurryLogEvent("Menu Challenges: Tab Weekly");
    
    mDidFinishEnter = false;
    bar1->setVisible(false);
    bar2->setVisible(false);
    bar3->setVisible(false);
    
    aBG2->getChildByTag(100)->setVisible(true);
    aBG2->getChildByTag(101)->setVisible(false);
    
    aBG2->getChildByTag(200)->setVisible(false);
    aBG2->getChildByTag(201)->setVisible(true);
    
    SetActiveTab(2);
}

//Set the tab + update header !!!
void ChallengesScene::SetActiveTab(int theType)
{
    mActiveTab = theType;
    
    //    mBox->add
    if(theType==1)
    {
        if(mWeeklyBase)
            mWeeklyBase->setVisible(false);
        
        //Show all the wakly stuff
        ShowDaily();
    }
    else if(theType==2)
    {
        if(mDailyBase)
            mDailyBase->setVisible(false);
        
        ShowWeekly();
    }
}

void ChallengesScene::ShowWeekly()
{
    if(mWeeklyBase)
    {
        mWeeklyBase->setVisible(true);
    }
    else
    {
        mWeeklyBase = CCSprite::create("small_dot_red.png");
        mWeeklyBase->setPosition(ccp(aBG2->getContentSize().width/2+40,aBG2->getContentSize().height/2));
        aBG2->addChild(mWeeklyBase);
        
        //Add the stuff here
        
        if(User::getInstance()->isEasterActive())
        {
            //Lets show the easter stuff !!
            CCSprite* aDummy = CCSprite::create("Interfeiss/challenges/weekly/easter/rukis_easterhead.png");
            aDummy->setPosition(ccp(250,60));
            mWeeklyBase->addChild(aDummy);
            
            aDummy = CCSprite::create("Interfeiss/challenges/weekly/easter/weekly_speechbuble.png");
            aDummy->setPosition(ccp(-100,80));
            mWeeklyBase->addChild(aDummy);
            
            //Add some text field here !!!
            _EasterSpeach = CCLabelTTF::create("Hello - I rabit\nGet some eggs for me\nwant breakfast :D", FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
                                        CCSize(450, 180), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
            aDummy->addChild(_EasterSpeach);
            _EasterSpeach->setRotation(-3);
            _EasterSpeach->setColor(ccc3(92, 20, 11));
            _EasterSpeach->setPosition(ccp(aDummy->getContentSize().width/2+30,aDummy->getContentSize().height/2+10));
            
            CCSprite* aDummy2 = CCSprite::create("Interfeiss/challenges/weekly/easter/progress_bar_frame.png");
            aDummy2->setPosition(ccp(0,-200));
            mWeeklyBase->addChild(aDummy2);
            
            //The progress bar
            
            CCSprite* progress = CCSprite::create("Interfeiss/challenges/weekly/progress_bar_fill.png");
            progress->setAnchorPoint(ccp(0,0));
            progress->setPosition(ccp(23,61));
            
            int max = 20;
            int value = User::getInstance()->mEasterEggCount;//How many eggs do we have ???
            if(value>max)
                value = max;
            
            progress->setTextureRect(CCRect(0, 0,
                                            progress->getTexture()->getContentSize().width*((float)value / max),
                                            progress->getTexture()->getContentSize().height));
            
            aDummy2->addChild(progress);
            
            //The reward for egs!!!
            
            aDummy = CCSprite::create("Interfeiss/challenges/weekly/reward_diamond.png");
            aDummy->setScale(0.8f);
            aDummy->setPosition(ccp(aDummy2->getContentSize().width-aDummy->getContentSize().width/2-12,86));
            aDummy2->addChild(aDummy);
            
            //----------------
            
            _EasterReward = CCLabelTTF::create("10000", FONT_SKRANJI, TITLE_FONT_SIZE*0.6,
                                               CCSize(140, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
            _EasterReward->setPosition(ccp(aDummy2->getContentSize().width-80,35));
            aDummy2->addChild(_EasterReward);
            
            _EasterTimeLeft = CCLabelTTF::create("Time left:", FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
                                                CCSize(200, 60), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
            _EasterTimeLeft->setColor(ccc3(92, 20, 11));
            _EasterTimeLeft->setPosition(ccp(50,8));
            aDummy2->addChild(_EasterTimeLeft);
            
            
            aDummy = CCSprite::create("Interfeiss/challenges/weekly/easter/egg_big.png");
            aDummy->setPosition(ccp(aDummy2->getPositionX()+40,aDummy2->getPositionY()+80));
            mWeeklyBase->addChild(aDummy);
            
            std::stringstream theAmount;
            theAmount<<value<<"/"<<max;
            
            _EasterProgress = CCLabelTTF::create(theAmount.str().c_str(), FONT_SKRANJI, TITLE_FONT_SIZE,
                                               CCSize(140, 50), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
            _EasterProgress->setPosition(ccp(-70,42));
            _EasterProgress->setColor(ccc3(92, 20, 11));
            aDummy->addChild(_EasterProgress);
            
            //The timer
            aDummy = CCSprite::create("Interfeiss/challenges/weekly/easter/counter_frame.png");
            aDummy->setPosition(ccp(aDummy2->getPositionX()-160,aDummy2->getPositionY()-64));
            mWeeklyBase->addChild(aDummy);
            
            CCLabelTTF* _EasterTimeName = CCLabelTTF::create("days", FONT_SKRANJI, TITLE_FONT_SIZE*0.55,
                                                 CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
            _EasterTimeName->setPosition(ccp(aDummy->getContentSize().width/2,-11));
            _EasterTimeName->setColor(ccc3(158, 131, 70));
            aDummy->addChild(_EasterTimeName);
            
            //=============================================================================
            //The magic time !!!
            _EasterTime_Days_1 = CCLabelTTF::create("1", FONT_SKRANJI, TITLE_FONT_SIZE*0.8,
                                                       CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            _EasterTime_Days_1->setPosition(ccp(aDummy->getContentSize().width/2-18,aDummy->getContentSize().height/2-4));
            _EasterTime_Days_1->setColor(ccc3(59, 26, 12));
            aDummy->addChild(_EasterTime_Days_1);
            
            _EasterTime_Days_2 = CCLabelTTF::create("0", FONT_SKRANJI, TITLE_FONT_SIZE*0.8,
                                                       CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            _EasterTime_Days_2->setPosition(ccp(aDummy->getContentSize().width/2+21,aDummy->getContentSize().height/2-4));
            _EasterTime_Days_2->setColor(ccc3(59, 26, 12));
            aDummy->addChild(_EasterTime_Days_2);
            //=============================================================================
            
            aDummy = CCSprite::create("Interfeiss/challenges/weekly/easter/counter_frame.png");
            aDummy->setPosition(ccp(aDummy2->getPositionX()+160,aDummy2->getPositionY()-64));
            mWeeklyBase->addChild(aDummy);
            
            _EasterTimeName = CCLabelTTF::create("seconds", FONT_SKRANJI, TITLE_FONT_SIZE*0.55,
                                                             CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
            _EasterTimeName->setPosition(ccp(aDummy->getContentSize().width/2,-11));
            _EasterTimeName->setColor(ccc3(158, 131, 70));
            aDummy->addChild(_EasterTimeName);
            
            //=============================================================================
            //The magic time !!!
            _EasterTime_Days_1 = CCLabelTTF::create("0", FONT_SKRANJI, TITLE_FONT_SIZE*0.8,
                                                    CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            _EasterTime_Days_1->setPosition(ccp(aDummy->getContentSize().width/2-18,aDummy->getContentSize().height/2-4));
            _EasterTime_Days_1->setColor(ccc3(59, 26, 12));
            aDummy->addChild(_EasterTime_Days_1);
            
            _EasterTime_Days_2 = CCLabelTTF::create("0", FONT_SKRANJI, TITLE_FONT_SIZE*0.8,
                                                    CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            _EasterTime_Days_2->setPosition(ccp(aDummy->getContentSize().width/2+21,aDummy->getContentSize().height/2-4));
            _EasterTime_Days_2->setColor(ccc3(59, 26, 12));
            aDummy->addChild(_EasterTime_Days_2);
            //=============================================================================
            
            aDummy = CCSprite::create("Interfeiss/challenges/weekly/easter/counter_frame.png");
            aDummy->setPosition(ccp(aDummy2->getPositionX()-54,aDummy2->getPositionY()-64));
            mWeeklyBase->addChild(aDummy);
            
            _EasterTimeName = CCLabelTTF::create("hours", FONT_SKRANJI, TITLE_FONT_SIZE*0.55,
                                                 CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
            _EasterTimeName->setPosition(ccp(aDummy->getContentSize().width/2,-11));
            _EasterTimeName->setColor(ccc3(158, 131, 70));
            aDummy->addChild(_EasterTimeName);
            
            //=============================================================================
            //The magic time !!!
            _EasterTime_Hours_1 = CCLabelTTF::create("0", FONT_SKRANJI, TITLE_FONT_SIZE*0.8,
                                                    CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            _EasterTime_Hours_1->setPosition(ccp(aDummy->getContentSize().width/2-18,aDummy->getContentSize().height/2-4));
            _EasterTime_Hours_1->setColor(ccc3(59, 26, 12));
            aDummy->addChild(_EasterTime_Hours_1);
            
            _EasterTime_Hours_2 = CCLabelTTF::create("0", FONT_SKRANJI, TITLE_FONT_SIZE*0.8,
                                                    CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            _EasterTime_Hours_2->setPosition(ccp(aDummy->getContentSize().width/2+21,aDummy->getContentSize().height/2-4));
            _EasterTime_Hours_2->setColor(ccc3(59, 26, 12));
            aDummy->addChild(_EasterTime_Hours_2);
            //=============================================================================
            
            aDummy = CCSprite::create("Interfeiss/challenges/weekly/easter/counter_frame.png");
            aDummy->setPosition(ccp(aDummy2->getPositionX()+54,aDummy2->getPositionY()-64));
            mWeeklyBase->addChild(aDummy);
            
            _EasterTimeName = CCLabelTTF::create("minutes", FONT_SKRANJI, TITLE_FONT_SIZE*0.55,
                                                 CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
            _EasterTimeName->setPosition(ccp(aDummy->getContentSize().width/2,-11));
            _EasterTimeName->setColor(ccc3(158, 131, 70));
            aDummy->addChild(_EasterTimeName);
            
            //=============================================================================
            //The magic time !!!
            _EasterTime_Minutes_1 = CCLabelTTF::create("0", FONT_SKRANJI, TITLE_FONT_SIZE*0.8,
                                                     CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            _EasterTime_Minutes_1->setPosition(ccp(aDummy->getContentSize().width/2-18,aDummy->getContentSize().height/2-4));
            _EasterTime_Minutes_1->setColor(ccc3(59, 26, 12));
            aDummy->addChild(_EasterTime_Minutes_1);
            
            _EasterTime_Minutes_2 = CCLabelTTF::create("0", FONT_SKRANJI, TITLE_FONT_SIZE*0.8,
                                                     CCSize(100, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            _EasterTime_Minutes_2->setPosition(ccp(aDummy->getContentSize().width/2+21,aDummy->getContentSize().height/2-4));
            _EasterTime_Minutes_2->setColor(ccc3(59, 26, 12));
            aDummy->addChild(_EasterTime_Minutes_2);
            //=============================================================================
            
        }
        else
        {
            const WeeklyTask* wt = User::getInstance()->getWeeklyChallenges().getTasks();
            
            ChallengeProgressBar* bar1 = ChallengeProgressBar::create(wt[0].getTaskInfo(),
                                                                      wt[0].getRequirement(),
                                                                      User::getInstance()->getWeeklyChallenges().getTask1Progress(),
                                                                      wt[0].getReward(),
                                                                      wt[0].getRewardType());
            bar1->setPosition(30,0);
            //Add it to some background
            CCSprite* aDummy = CCSprite::create("Interfeiss/challenges/weekly/panel_weekly_challenges.png");
            aDummy->setPosition(ccp(mWeeklyBase->getContentSize().width/2,160));
            aDummy->addChild(bar1);
            mWeeklyBase->addChild(aDummy);
            
            //--2nd
            ChallengeProgressBar* bar2 = ChallengeProgressBar::create(wt[1].getTaskInfo(),
                                                                      wt[1].getRequirement(),
                                                                      User::getInstance()->getWeeklyChallenges().getTask2Progress(),
                                                                      wt[1].getReward(),
                                                                      wt[1].getRewardType());
            bar2->setPosition(30,0);
            //Add it to some background
            aDummy = CCSprite::create("Interfeiss/challenges/weekly/panel_weekly_challenges.png");
            aDummy->setPosition(ccp(mWeeklyBase->getContentSize().width/2,-30));
            aDummy->addChild(bar2);
            mWeeklyBase->addChild(aDummy);
            
            //--3rd
            ChallengeProgressBar* bar3 = ChallengeProgressBar::create(wt[2].getTaskInfo(),
                                                                      wt[2].getRequirement(),
                                                                      User::getInstance()->getWeeklyChallenges().getTask3Progress(),
                                                                      wt[2].getReward(),
                                                                      wt[2].getRewardType());
            bar3->setPosition(30,0);
            //Add it to some background
            aDummy = CCSprite::create("Interfeiss/challenges/weekly/panel_weekly_challenges.png");
            aDummy->setPosition(ccp(mWeeklyBase->getContentSize().width/2,-220));
            aDummy->addChild(bar3);
            mWeeklyBase->addChild(aDummy);
        }
    }
}

void ChallengesScene::ShowDaily()
{
    if(mDailyBase)
    {
        mDailyBase->setVisible(true);
        
    }
    else
    {
        int aDayBarY = -250;
        mDailyBase = CCSprite::create("small_dot_red.png");
        mDailyBase->setPosition(ccp(aBG2->getContentSize().width/2+40,aBG2->getContentSize().height/2));
        aBG2->addChild(mDailyBase);
        
        //Lets add the days bar !!!
        uint32_t chDay = User::getInstance()->getDailyChallenges().getDaysCompleted();
        if (!Time::isToday(User::getInstance()->getDailyChallenges().getLastCompletionTime()))
        {
            chDay++;
        }
        
        CCSprite* aDummy = CCSprite::create("Interfeiss/challenges/daily/progress_days.png");
        aDummy->setPosition(ccp(-10,aDayBarY));
        mDailyBase->addChild(aDummy);
        
        CCSprite* aDummy2 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
        aDummy2->setAnchorPoint(ccp(0,0.5f));
        aDummy2->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6,aDummy->getPositionY()));
        mDailyBase->addChild(aDummy2);
        
        std::stringstream theDay;
        
        
        for(int i=1;i<6;i++)
        {
            if(i!=5)
            {
                CCSprite* aSeperator = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
                aSeperator->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+20+i*130,aDayBarY));
                mDailyBase->addChild(aSeperator);
            }
            
            //Add the text fields
            theDay<<"day "<<i;
            
            CCLabelTTF* dayTxt = CCLabelTTF::create(theDay.str().c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.5,
                                        CCSize(100, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
            dayTxt->setAnchorPoint(ccp(0,0));
            dayTxt->setColor(ccc3(85,70,30));
            if(i==1)
                dayTxt->cocos2d::CCNode::setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+28,aDayBarY-20));
            else if(i==2)
                dayTxt->cocos2d::CCNode::setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+162,aDayBarY-20));
            else if(i==3)
                dayTxt->cocos2d::CCNode::setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+295,aDayBarY-20));
            else if(i==4)
                dayTxt->cocos2d::CCNode::setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+426,aDayBarY-20));
            else if(i==5)
                dayTxt->cocos2d::CCNode::setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+550,aDayBarY-20));
            
            mDailyBase->addChild(dayTxt);
            
            theDay.str("");
            theDay.clear();
        }
        
        //place the day current stuff
        CCSprite* aDayCurrent = CCSprite::create("Interfeiss/challenges/daily/dayframe.png");
        aDayCurrent->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+aDayCurrent->getContentSize().width/2+10,
                                     aDummy->getPositionY()));
        mDailyBase->addChild(aDayCurrent);
        
//        chDay = 5;
        if(chDay == 1)
            aDayCurrent->setPositionX(aDummy->getPositionX()-aDummy->getContentSize().width/2+aDayCurrent->getContentSize().width/2+10);
        else if(chDay == 2)
            aDayCurrent->setPositionX(aDummy->getPositionX()-aDummy->getContentSize().width/2+aDayCurrent->getContentSize().width/2+10+137);
        else if(chDay == 3)
            aDayCurrent->setPositionX(aDummy->getPositionX()-aDummy->getContentSize().width/2+aDayCurrent->getContentSize().width/2+10+266);
        else if(chDay == 4)
            aDayCurrent->setPositionX(aDummy->getPositionX()-aDummy->getContentSize().width/2+aDayCurrent->getContentSize().width/2+10+395);
        else if(chDay == 5)
            aDayCurrent->setPositionX(aDummy->getPositionX()-aDummy->getContentSize().width/2+aDayCurrent->getContentSize().width/2+10+524);
        
        
//        chDay = 2;
        if(chDay == 4)
        {
            aDummy2->setTextureRect(CCRect(0, 0,
                                           aDummy2->getTexture()->getContentSize().width*((float)chDay / 5.1),
                                           aDummy2->getTexture()->getContentSize().height));
        }
        else
        {
            aDummy2->setTextureRect(CCRect(0, 0,
                                           aDummy2->getTexture()->getContentSize().width*((float)chDay / 5),
                                           aDummy2->getTexture()->getContentSize().height));
        }
        
        //Add the end day stuff
        CCSprite* aChest = CCSprite::create("Interfeiss/challenges/daily/mysterybox.png");
        aChest->setPosition(ccp(aDummy->getPositionX()+aDummy->getContentSize().width/2,aDummy->getPositionY()));
        mDailyBase->addChild(aChest);
        
        //Set the spin bar !!
        CCSprite* aSlotMachineBase = CCSprite::create("Interfeiss/challenges/daily/slotmachine_top.png");
        aSlotMachineBase->setPosition(ccp(mDailyBase->getContentSize().width/2+15,mDailyBase->getContentSize().height/2));
        mDailyBase->addChild(aSlotMachineBase,1);
        
        CCSprite* aSwipe = CCSprite::create("Interfeiss/challenges/daily/swipe.png");
        aSwipe->setPosition(ccp(340,0));
        mDailyBase->addChild(aSwipe,-1);
        
        //the popup for each case
        collectBubble = CCSprite::create("Interfeiss/challenges/daily/speech_bubble_collect.png");
        collectBubble->setPosition(ccp(80,320));
        collectBubble->setTag(1000);
        collectBubble->setVisible(false);
        aSlotMachineBase->addChild(collectBubble,-1);
        
        comebackBubble = CCSprite::create("Interfeiss/challenges/daily/speech_bubble_comeback.png");
        comebackBubble->setPosition(ccp(80,320));
        comebackBubble->setTag(1001);
        comebackBubble->setVisible(false);
        aSlotMachineBase->addChild(comebackBubble,-1);
        
        oneSpinBubble = CCSprite::create("Interfeiss/challenges/daily/speech_bubble_once.png");
        oneSpinBubble->setPosition(ccp(80,320));
        oneSpinBubble->setTag(1002);
        oneSpinBubble->setVisible(false);
        aSlotMachineBase->addChild(oneSpinBubble,-1);
        
        spinBubble = CCSprite::create("Interfeiss/challenges/daily/speech_bubble_spin.png");
        spinBubble->setPosition(ccp(80,320));
        spinBubble->setTag(1003);
        spinBubble->setVisible(false);
        aSlotMachineBase->addChild(spinBubble,-1);
        
        //Add the spiner !!!
        
        CCSprite* aSwipe1 = CCSprite::create("Interfeiss/challenges/daily/slotmachine_switch.png");
        aSwipe1->setPosition(ccp(aSlotMachineBase->getContentSize().width+6,260));
//        aSlotMachineBase->addChild(aSwipe1,-1);
        
        CCSprite* aSwipe2 = CCSprite::create("Interfeiss/challenges/daily/novilciens_fx2.png");
        aSwipe2->setPosition(ccp(aSlotMachineBase->getContentSize().width+20,200));
        aSwipe2->setVisible(false);
//        aSlotMachineBase->addChild(aSwipe2,-1);
        
        //The bottom
        diamondBox = CCSprite::create("Interfeiss/challenges/daily/slotmachine_bottom.png");
        diamondBox->setPosition(ccp(aSlotMachineBase->getContentSize().width/2+95,60));
        aSlotMachineBase->addChild(diamondBox,-1);
        
        bar1 = SpinBar::create();
        bar1->setVisible(false);
        bar1->setAnchorPoint(ccp(0,0));
//        bar1->setPosition(ccp(512,340));
        bar1->setPosition(ccp(0,0));
        bar1->setSpinPosition(User::getInstance()->getDailyChallenges().getBar1Color(), User::getInstance()->getDailyChallenges().getBar1Count());
        addChild(bar1, 4);
        
        bar2 = SpinBar::create();
        bar2->setAnchorPoint(ccp(0,0));
        bar2->setVisible(false);
//        bar2->setPosition(ccp(609,340));
        bar2->setPosition(ccp(0,300));
        bar2->setSpinPosition(User::getInstance()->getDailyChallenges().getBar2Color(), User::getInstance()->getDailyChallenges().getBar2Count());
        addChild(bar2, 4);
        
        bar3 = SpinBar::create();
        bar3->setAnchorPoint(ccp(0,0));
        bar3->setVisible(false);
//        bar3->setPosition(ccp(707,340));
        bar3->setPosition(ccp(300,300));
        bar3->setSpinPosition(User::getInstance()->getDailyChallenges().getBar3Color(), User::getInstance()->getDailyChallenges().getBar3Count());
        addChild(bar3, 4);
        
        HandleBar* handle = HandleBar::create();
        handle->setPosition(ccp(260,-40));
        mDailyBase->addChild(handle,-1);
    }
    
    if (User::getInstance()->getDailyChallenges().isTodaysCompleted())
    {
        comebackBubble->setVisible(true);
    }
    else if (Time::isToday(User::getInstance()->getDailyChallenges().getLastSpinTime()))
    {
        collectBubble->setVisible(true);
    }
    else
    {
        spinBubble->setVisible(true);
    }
}

void ChallengesScene::update(float delta)
{
    CCLayer::update(delta);
    
    //Set them visible or not !!!
    if(mDidFinishEnter)
    {
        if(!bar1->isVisible())
            bar1->setVisible(true);
        if(!bar2->isVisible())
            bar2->setVisible(true);
        if(!bar3->isVisible())
            bar3->setVisible(true);
    }
    
//    bar1->setPosition(ccp(512,339));
//    bar2->setPosition(ccp(609,339));
//    bar3->setPosition(ccp(707,339));
//    
//    bar1->setPosition(ccp(512,340));
//    bar2->setPosition(ccp(609,340));
//    bar3->setPosition(ccp(707,340));
    
    bar1->setPosition(ccp(516,339));
    bar2->setPosition(ccp(593,339));
    bar3->setPosition(ccp(672,339));
    
    bar1->setPosition(ccp(516,340));
    bar2->setPosition(ccp(593,340));
    bar3->setPosition(ccp(672,340));
}

void ChallengesScene::update()
{
    comebackBubble->setVisible(false);
    collectBubble->setVisible(false);
    spinBubble->setVisible(false);
    
    if (User::getInstance()->getDailyChallenges().isTodaysCompleted())
    {
        comebackBubble->setVisible(true);
    }
    else if (Time::isToday(User::getInstance()->getDailyChallenges().getLastSpinTime()))
    {
        collectBubble->setVisible(true);
    }
    else
    {
        spinBubble->setVisible(true);
    }
    
    uint32_t chDay = User::getInstance()->getDailyChallenges().getDaysCompleted();
    if (!Time::isToday(User::getInstance()->getDailyChallenges().getLastCompletionTime()))
    {
        chDay++;
    }
    
//    daysN1 = CCLabelTTF::create(toString(chDay / 10).c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
//                                CCSize(50, 55), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
//    daysN1->setAnchorPoint(ccp(0,0));
//    daysN1->setColor(ccc3(0x0, 0x0, 0x0));
//    daysN1->setPosition(ccp(293, 125));
//    addChild(daysN1, 10);
//    
//    daysN2 = CCLabelTTF::create(toString(chDay % 10).c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
//                                CCSize(50, 55), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
//    daysN2->setAnchorPoint(ccp(0,0));
//    daysN2->setColor(ccc3(0x0, 0x0, 0x0));
//    daysN2->setPosition(ccp(334, 125));
//    addChild(daysN2, 10);
//    
//    daysN1->setString(toString(chDay / 10).c_str());
//    daysN2->setString(toString(chDay % 10).c_str());
    
    uint32_t day = 0, reward = 0;
    User::getInstance()->getDailyChallenges().getNextDayReward(User::getInstance()->getDailyChallenges().getDaysCompleted(), day, reward);
    
//    std::string txt = "Beat the challenge "
//    + toString(day)
//    + " days in a row to get";
    
//    footerText->setString(txt.c_str());
//    footerText6->setString(toString(reward).c_str());
}

void ChallengesScene::SpinTheBars()
{
    AnalyticX::flurryLogEvent("Menu Challenges: Start Spin");
    
    collectBubble->setVisible(true);
    spinBubble->setVisible(false);
    
    bar1->spin(0);
    bar2->spin(1);
    bar3->spin(2);
    
    // can calculate diamonds amount and drop it out
    this->runAction(CCSequence::create(CCDelayTime::create(5.0), CCCallFunc::create(this, callfunc_selector(ChallengesScene::dropDiamonds)), NULL));
    
    // lets calculate reward
    uint32_t reward = 0;
    
    std::map<CrystalColor, int> modifiers;
    modifiers[CRYSTAL_COLOR_GREEN] = 1.0;
    modifiers[CRYSTAL_COLOR_BLUE] = 2.0;
    modifiers[CRYSTAL_COLOR_RED] = 3.0;
    modifiers[CRYSTAL_COLOR_YELLOW] = 5.0;
    
    uint32_t points = modifiers[bar1->getCrystalColor()] * bar1->getCrystalCount()
    + modifiers[bar2->getCrystalColor()] * bar2->getCrystalCount()
    + modifiers[bar3->getCrystalColor()] * bar3->getCrystalCount();
    
    if (points <= 5)
        reward = 1;
    else if (points <= 10)
        reward = 2;
    else if (points <= 15)
        reward = 3;
    else if (points <= 20)
        reward = 4;
    else if (points <= 25)
        reward = 5;
    else
        reward = 6;
    
    User::getInstance()->getDailyChallenges().setBar1Color(bar1->getCrystalColor());
    User::getInstance()->getDailyChallenges().setBar2Color(bar2->getCrystalColor());
    User::getInstance()->getDailyChallenges().setBar3Color(bar3->getCrystalColor());
    
    User::getInstance()->getDailyChallenges().setBar1Count(bar1->getCrystalCount());
    User::getInstance()->getDailyChallenges().setBar2Count(bar2->getCrystalCount());
    User::getInstance()->getDailyChallenges().setBar3Count(bar3->getCrystalCount());
    
    User::getInstance()->getDailyChallenges().setReward(reward);
    User::getInstance()->getDailyChallenges().setLastSpinTime(time(0));
    
}

void ChallengesScene::dropDiamonds()
{
    diamondBox->removeAllChildren();
    
    for (int i = 0; i < User::getInstance()->getDailyChallenges().getReward(); i++)
    {
        FallingDiamond* fd = FallingDiamond::create();
//        fd->setPosition(ccp(630 + (((i+1) / 2) * 40 * (i % 2 == 1 ? -1 : 1)), 240));
        fd->setPosition(ccp(120+i*50,250));
        diamondBox->addChild(fd, 10-i);
        
        int r = i * 2;
//        CCMoveTo* mt = CCMoveTo::create(0.95-0.01 * r, ccp(fd->getPositionX(), 150 + r));
        CCMoveTo* mt = CCMoveTo::create(1.0f,ccp(fd->getPositionX(),50+r));
        fd->runAction(CCEaseBounceOut::create(mt));
    }
}

void ChallengesScene::menuBackCallback(cocos2d::CCObject *sender)
{
    CCScene* mainMenu = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
	CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, mainMenu);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void ChallengesScene::menuWeeklyCallback(cocos2d::CCObject *sender)
{
	CCScene* scene = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::WEEKLY_CHALLENGES);
	CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, scene);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void ChallengesScene::EmptyHandlePull()
{
    oneSpinBubble->setVisible(true);
    this->runAction(CCSequence::create(CCDelayTime::create(3),
                                       CCCallFunc::create(this, callfunc_selector(ChallengesScene::HideOneSpinBubble)), NULL));
}

void ChallengesScene::HideOneSpinBubble()
{
    oneSpinBubble->setVisible(false);
}

//-----------------------------------------------------------------------

void ChallengesScene::RemovePopup()
{
	WorldMap* worldMaap = static_cast<WorldMap*>(this->getParent());
	worldMaap->moveBackground = false;

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Tap_Back").c_str());
    
    mDidFinishEnter = false;
    bar1->setVisible(false);
    bar2->setVisible(false);
    bar3->setVisible(false);
    
    CCLayerColor* aLayer = (CCLayerColor*)getChildByTag(888);
    CCFadeTo* aFade = CCFadeTo::create(0.25f,0);
    aLayer->runAction(aFade);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(visibleSize.width/2,visibleSize.height+200));
    CCEaseElasticIn* aEase1 = CCEaseElasticIn::create(aMove1,1.0f);
    CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(ChallengesScene::RealRemove));
    CCSequence* aSeq = CCSequence::create(aEase1,func2,NULL);
    aBG2->runAction(aSeq);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Popup_Exit").c_str());
    
    AnalyticX::flurryLogEvent("Menu Clouse: Challenges");

}

void ChallengesScene::RealRemove()
{
    WorldMap* map = (WorldMap*)this->getParent();
    this->getParent()->removeChild(this);
    
    /*
    MainMenuScene* mainMenu = (MainMenuScene*)this->getParent();
    mainMenu->mOptionsOpen = false;
    //Enable the low menu
    mainMenu->mMainMenu->setEnabled(true);
    
    this->getParent()->removeChild(this);
    */
}


//========================================================================
//The weakly progres bar info !!!

ChallengeProgressBar* ChallengeProgressBar::create(std::string text, unsigned int max, unsigned int value, int reward, WeeklyTask::RewardType rewardType)
{
    ChallengeProgressBar *pGOL = new ChallengeProgressBar();
    if (pGOL && pGOL->init(text, max, value, reward, rewardType)) {
        pGOL->autorelease();
        return pGOL;
    }
    CC_SAFE_DELETE(pGOL);
    return NULL;
}

bool ChallengeProgressBar::init(std::string text, unsigned int max, unsigned int value, int rewardAmount, WeeklyTask::RewardType rewardType)
{
    setAnchorPoint(ccp(0,0));
    
//    CCSprite* back = CCSprite::create("Interfeiss/challenges/progress_bar.png");
    CCSprite* back = CCSprite::create("Interfeiss/challenges/weekly/progress_bar_frame.png");
    back->setAnchorPoint(ccp(0,0));
    back->setPosition(ccp(-12,-20));
    addChild(back);
    
//    progress = CCSprite::create("Interfeiss/challenges/progress_bar_fill.png");
    progress = CCSprite::create("Interfeiss/challenges/weekly/progress_bar_fill.png");
    progress->setAnchorPoint(ccp(0,0));
    progress->setPosition(ccp(10,38));
    
    progress->setTextureRect(CCRect(0, 0,
                                    progress->getTexture()->getContentSize().width*((float)value / max),
                                    progress->getTexture()->getContentSize().height));
    
    addChild(progress);
    
//    titleSh = CCLabelTTF::create(text.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
//                                 CCSize(700, 70), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
//    titleSh->setAnchorPoint(ccp(0,0));
//    titleSh->cocos2d::CCNode::setPosition(ccp(25, 85));
//    titleSh->setColor(ccc3(0, 0, 0));
//    titleSh->enableShadow(CCSize(1,1), 0.5, 3, true);
//    addChild(titleSh);
    
    title = CCLabelTTF::create(text.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
                               CCSize(700, 70), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	title->setAnchorPoint(ccp(0.5f,0.5f));
	title->cocos2d::CCNode::setPosition(ccp(335,140));
	title->setColor(ccc3(84,71,52));
//    title->enableShadow(CCSize(ccp(1,1)),1.0f,true);
	addChild(title);
    
	char pointsStr[20];
	sprintf(pointsStr, "%d/%d", value, max);
    
//    pointsSh = CCLabelTTF::create(pointsStr, FONT_SKRANJI, TITLE_FONT_SIZE*0.6,
//                                  CCSize(560, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
//    pointsSh->setAnchorPoint(ccp(0,0));
//    pointsSh->cocos2d::CCNode::setPosition(ccp(14, 39));
//    pointsSh->setColor(ccc3(0, 0, 0));
//    pointsSh->enableShadow(CCSize(1,1), 0.5, 3, true);
//    addChild(pointsSh);
    
    points = CCLabelTTF::create(pointsStr, FONT_SKRANJI, TITLE_FONT_SIZE*0.6,
                                CCSize(560, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	points->setAnchorPoint(ccp(0,0));
	points->cocos2d::CCNode::setPosition(ccp(14, 28));
    points->enableShadow(CCSize(ccp(2,-2)),2.0f,false);
	addChild(points);
    
    char rewardStr[10];
    std::string iconStr;
    if (rewardType == WeeklyTask::RT_DIAMONDS) // diamonds
    {
    	sprintf(rewardStr, "x%d", rewardAmount);
    	iconStr = "Interfeiss/challenges/weekly/reward_diamond.png";
    }
    else // xp
    {
    	sprintf(rewardStr, "+%d", rewardAmount);
    	iconStr = "Interfeiss/challenges/weekly/reward_crystals.png";
    }
    
//    rewardSh = CCLabelTTF::create(rewardStr, FONT_SKRANJI, TITLE_FONT_SIZE*0.5,
//                                  CCSize(100, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
//    rewardSh->setAnchorPoint(ccp(0,0));
//    rewardSh->cocos2d::CCNode::setPosition(ccp(580, 2));
//    rewardSh->setColor(ccc3(0, 0, 0));
//    rewardSh->enableShadow(CCSize(1,1), 0.5, 3, true);
//    addChild(rewardSh);
    
    reward = CCLabelTTF::create(rewardStr, FONT_SKRANJI, TITLE_FONT_SIZE*0.5,
                                CCSize(100, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	reward->setAnchorPoint(ccp(0,0));
	reward->setColor(ccc3(255,255,255));
	reward->cocos2d::CCNode::setPosition(ccp(574, -6));
	addChild(reward);
    
	// icon
	icon = CCSprite::create(iconStr.c_str());
	icon->setAnchorPoint(ccp(0.5,0.5));
	icon->setPosition(ccp(635, 65));
	addChild(icon);
    
    update(text, max, value, rewardAmount, rewardType);
    
    return true;
}

void ChallengeProgressBar::update(std::string text,
                                  unsigned int max,
                                  unsigned int value,
                                  int rewardAmount,
                                  WeeklyTask::RewardType rewardType)
{
    progress->setTextureRect(CCRect(0, 0,
                                    progress->getTexture()->getContentSize().width*((float)value / max),
                                    progress->getTexture()->getContentSize().height));
    
//    titleSh->setString(text.c_str());
    title->setString(text.c_str());
    
	char pointsStr[20];
	sprintf(pointsStr, "%d/%d", value, max);
    
//    pointsSh->setString(pointsStr);
    points->setString(pointsStr);
    
    char rewardStr[10];
    std::string iconStr;
    if (rewardType == WeeklyTask::RT_DIAMONDS) // diamonds
    {
    	sprintf(rewardStr, "+%d", rewardAmount);
    	iconStr = "Interfeiss/challenges/weekly/reward_diamond.png";
    }
    else // xp
    {
    	sprintf(rewardStr, "+%d", rewardAmount);
    	iconStr = "Interfeiss/challenges/weekly/reward_crystals.png";
    }
    
//    rewardSh->setString(rewardStr);
    reward->setString(rewardStr);
    
	// icon
    if (icon != NULL)
    {
        removeChild(icon);
    }
	icon = CCSprite::create(iconStr.c_str());
	icon->setAnchorPoint(ccp(0.5,0.5));
	icon->setPosition(ccp(630, 68));
	addChild(icon);
}

SpinBar::SpinBar()
{
    boxShader1 = new CCGLProgram();
    boxShader1->initWithVertexShaderFilename("slot.vsh", "slot.fsh");
    
    boxShader2 = new CCGLProgram();
    boxShader2->initWithVertexShaderFilename("slot.vsh", "slot.fsh");
    
    resultingPosition = -1;
}

SpinBar::~SpinBar()
{
    
}

bool SpinBar::init()
{
	positions[0] = SpinPosition(CRYSTAL_COLOR_GREEN, 5);
	positions[1] = SpinPosition(CRYSTAL_COLOR_BLUE, 3);
	positions[2] = SpinPosition(CRYSTAL_COLOR_GREEN, 2);
	positions[3] = SpinPosition(CRYSTAL_COLOR_RED, 3);
	positions[4] = SpinPosition(CRYSTAL_COLOR_GREEN, 1);
	positions[5] = SpinPosition(CRYSTAL_COLOR_YELLOW, 2);
	positions[6] = SpinPosition(CRYSTAL_COLOR_GREEN, 3);
	positions[7] = SpinPosition(CRYSTAL_COLOR_BLUE, 1);
	positions[8] = SpinPosition(CRYSTAL_COLOR_RED, 1);
	positions[9] = SpinPosition(CRYSTAL_COLOR_YELLOW, 1);
	positions[10] = SpinPosition(CRYSTAL_COLOR_BLUE, 2);
	positions[11] = SpinPosition(CRYSTAL_COLOR_RED, 2);
	positions[12] = SpinPosition(CRYSTAL_COLOR_GREEN, 4);
	positions[13] = SpinPosition(CRYSTAL_COLOR_BLUE, 4);
    
    slide = CCLayer::create();
    
	CCSprite* mainSlide = CCSprite::create("Interfeiss/challenges/daily/streemele.png");
	mainSlide->setAnchorPoint(ccp(0,1));
    mainSlide->setShaderProgram(boxShader1);
	slide->addChild(mainSlide);
    
    boxShader1->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    boxShader1->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    boxShader1->link();
    boxShader1->updateUniforms();
    
    sh1_minyLoc = glGetUniformLocation(boxShader1->getProgram(), "u_miny");
    sh1_maxyLoc = glGetUniformLocation(boxShader1->getProgram(), "u_maxy");
    
    
	
    CCSprite* secondSlide = CCSprite::create("Interfeiss/challenges/daily/streemele.png");
	secondSlide->setAnchorPoint(ccp(0,1));
	secondSlide->setPositionY(-957);
    secondSlide->setShaderProgram(boxShader2);
    slide->addChild(secondSlide);
    
    boxShader2->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    boxShader2->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    boxShader2->link();
    boxShader2->updateUniforms();
    
    sh2_minyLoc = glGetUniformLocation(boxShader2->getProgram(), "u_miny");
    sh2_maxyLoc = glGetUniformLocation(boxShader2->getProgram(), "u_maxy");
	
    
    addChild(slide);
    
    setSpinPosition(rand() % 14);
    
    this->scheduleUpdate();
    this->update(0);
    
	return true;
}

void SpinBar::setSpinPosition(CrystalColor col, uint32_t count)
{
    // find position, if it is not possible - set random
    uint32_t pos = rand() % 14;
    
    for (int i = 0; i < sizeof(positions) / sizeof(positions[0]); i++)
    {
        if (col == positions[i].type && count == positions[i].count)
        {
            pos = i;
            break;
        }
    }
    
    setSpinPosition(pos);
}

void SpinBar::update(float delta)
{
    //    std::cout << "P: " <<(float)slide->getPosition().y << "\n";
    boxShader1->use();
    
    sh1_miny = ((float)slide->getPosition().y - 54) / 957;//1192
    sh1_maxy = ((float)slide->getPosition().y + 54) / 957;
    
    glUniform1f(sh1_minyLoc, this->sh1_miny);
    glUniform1f(sh1_maxyLoc, this->sh1_maxy);
    
    boxShader2->use();
    
    sh2_miny = ((float)slide->getPosition().y - 54 - 957) / 957;
    sh2_maxy = ((float)slide->getPosition().y + 54 - 957) / 957;
    
    glUniform1f(sh2_minyLoc, this->sh2_miny);
    glUniform1f(sh2_maxyLoc, this->sh2_maxy);
    
}

int SpinBar::getYFromPos(unsigned int p)
{
    p = p % 14;
    unsigned int addH = p == 0 ? 957 : 0;
//    return p * 85 + 43 + addH;
    return p * 68 + 43 + addH;
}

void SpinBar::setSpinPosition(unsigned int p)
{
	slide->setPositionY(getYFromPos(p));
}

void SpinBar::spin(int order)
{
    
    // randoms - liidz cik griezties
    
    // 5x pilns un 1x liidz vajadziigajam
    resultingPosition = abs(rand()) % 14;
    
    CCSequence* seq = CCSequence::create(
                                         CCMoveTo::create(0.5, ccp(0, getYFromPos(14))),
                                         CCCallFunc::create(this, callfunc_selector(SpinBar::resestSlide)),
                                         NULL);
    CCRepeat* rep = CCRepeat::create(seq, 5 + order * 2);
    
    // jaaieliek move uz 0 poziiciju
    CCSequence* seq2 = CCSequence::create(
                                          CCMoveTo::create(0.5 - 0.5 * (slide->getPositionY() / getYFromPos(14)), ccp(0, getYFromPos(14))),
                                          CCCallFunc::create(this, callfunc_selector(SpinBar::resestSlide)),
                                          rep,
                                          CCCallFunc::create(this, callfunc_selector(SpinBar::finalizeSpin)),
                                          NULL);
    
    slide->runAction(seq2);
}

void SpinBar::resestSlide()
{
    setSpinPosition(1);
}

void SpinBar::finalizeSpin()
{
    float time = 0.5 * ((resultingPosition + 13) % 14 / 14);
    CCMoveTo* m = CCMoveTo::create(time, ccp(0, getYFromPos(resultingPosition)));
    
    slide->runAction(m);
}

CrystalColor SpinBar::getCrystalColor()
{
	if (resultingPosition > -1)
    {
        return positions[resultingPosition].type;
    }
    return CRYSTAL_COLOR_COUNT;
}

unsigned int SpinBar::getCrystalCount()
{
	if (resultingPosition > -1)
    {
        return positions[resultingPosition].count;
    }
    return 1;
}

bool HandleBar::init()
{
    setAnchorPoint(ccp(0, 0));
    
    handle = CCSprite::create("Interfeiss/challenges/daily/slotmachine_switch.png");
    handle->setAnchorPoint(ccp(0,0));
    addChild(handle, 1);
    
    moveStarted = false;
    
    return true;
}

void HandleBar::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void HandleBar::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool HandleBar::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (moveStarted == false
        && touch->getLocation().x > 850
        && touch->getLocation().x < 960
        && touch->getLocation().y > 450
        && touch->getLocation().y < 550)
    {
        moveStarted = true;
        dragStart = touch->getLocation();
        
        return true;
    }
    else
    {
        return false;
    }
}

void HandleBar::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    moveStarted = false;
}

void HandleBar::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (moveStarted && touch->getLocation().y < 370)
    {
        moveStarted = false;
        
        // noraujam klokji
        
        handle->setPositionY(115);
        handle->setScaleY(-1);
        
        shadow = CCSprite::create("Interfeiss/challenges/daily/novilciens_fx2.png");
        shadow->setAnchorPoint(ccp(0, 0.5));
        shadow->setPosition(ccp(25, 65));
        addChild(shadow, 0);
        //shadow->
        
        CCFadeOut* fade = CCFadeOut::create(0.3);
        CCCallFunc* call = CCCallFunc::create(this, callfunc_selector(HandleBar::onAnimationEnded));
        CCSequence* seq = CCSequence::create(fade, call, NULL);
        
        shadow->runAction(seq);
        
        // TODO: add sounds
        if (Time::isToday(User::getInstance()->getDailyChallenges().getLastSpinTime()) == false)
        {
            ((ChallengesScene*)this->getParent()->getParent()->getParent())->SpinTheBars();
        }
        else
        {
            // empty sound and show bubble
            
            ((ChallengesScene*)this->getParent()->getParent()->getParent())->EmptyHandlePull();
        }
        
    }
}

void HandleBar::onAnimationEnded()
{
    removeChild(shadow);
    shadow = NULL;
    
    handle->setPositionY(0);
    handle->setScaleY(1);
}


bool FallingDiamond::init()
{
    this->setAnchorPoint(ccp(0,0));
    
    if (rand() % 2 == 0)
    {
        diamond = CCSprite::create("Interfeiss/challenges/diamond.png");
    }
    else
    {
        diamond = CCSprite::create("Interfeiss/challenges/diamond_2.png");
    }
    this->setScaleX(0.7 + 0.01 * (rand() % 30));
    this->setScaleY(getScaleX());
    
    diamond->setAnchorPoint(ccp(0,0));
    
    addChild(diamond);
    
    return true;
}
