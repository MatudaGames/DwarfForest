//
//  HighScore.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 9/29/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "HighScore.h"
#include "MainMenuScene.h"
#include "TimedSpriteAnimation.h"
#include "AppMacros.h"
#include "Utils.h"
#include "User.h"

#include "JniCalls.h"

USING_NS_CC;

using namespace DF::LevelEnd;

HighScore* DF::LevelEnd::HighScore::create(uint32_t result, bool newRecord)
{
    HighScore *pRet = new HighScore();
    if (pRet && pRet->init(result, newRecord))
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

bool DF::LevelEnd::HighScore::init(uint32_t result, bool newRecord)
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
//    this->setAnchorPoint(ccp(0, 1));
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/level_end/scorebar.png");
//    back->setAnchorPoint(ccp(0, 1));
    addChild(back);
    
    aScoreTotal = result;
    JniCalls::submitScroeToLeaderboard("leaderboard_easy", aScoreTotal);
    
    CCLabelTTF* score = CCLabelTTF::create(toString(result).c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*1.1, CCSize(275, 60), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//    score->setPosition(ccp(30, -27));
    
    score->setPosition(ccp(back->getContentSize().width/2,back->getContentSize().height/2-7));
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    score->setPosition(ccp(back->getContentSize().width/2,back->getContentSize().height/2+4));
    #endif
    
    score->setColor(ccc3(255, 255, 255));
//    score->setAnchorPoint(ccp(0, 1));
//    addChild(score);
    back->addChild(score);
    
    //For now disabled !!!
//    if (newRecord)
//    {
//        CCSprite* newHighScore = CCSprite::create("Interfeiss/level_end/highscore.png");
//        newHighScore->setPosition(ccp(166, -125));
//        newHighScore->setScale(0.1);
//        newHighScore->setOpacity(0);
//        addChild(newHighScore);
//        newHighScore->runAction(CCSequence::create(
//                                                   CCDelayTime::create(1.2),
//                                                   CCFadeIn::create(0),
//                                                   CCEaseBackOut::create(CCScaleTo::create(0.5, 1)),
//                                                   NULL));
//        
//        
//        DropingCup* cup1 = DropingCup::create();
//        cup1->setPosition(ccp(-30, 28));
//        addChild(cup1);
//        
//        DropingCup* cup2 = DropingCup::create();
//        cup2->setPosition(ccp(268, 30));
//        addChild(cup2);
//    }
    
    
    shareBt = CCMenuItemImage::create("Interfeiss/level_end/df_score_share.png",
                                                       NULL,
                                                       this,
                                                       menu_selector(HighScore::share));
//    shareBt->setAnchorPoint(ccp(0, 1));
//    shareBt->setScaleX(1.2);
//    shareBt->setScaleY(0.8);
    
    CCMenu* menu = CCMenu::create(shareBt, NULL);
    menu->setPosition(0, -30);
    menu->setOpacity(0);
    addChild(menu);
    
    if(User::getInstance()->mFeedBackGiven==0)
    {
        //Lets add the beta report stuff
        CCMenuItemImage* feedItem = CCMenuItemImage::create(
                                                            "beta_feedback.png",
                                                            "beta_feedbackpng",
                                                            this,
                                                            menu_selector(HighScore::sendFeedback));
        CCMenu* FeedBack = CCMenu::create(feedItem,NULL);
        FeedBack->setPosition(ccp(-280,0));
        addChild(FeedBack);
    }
    
    
    TimedSpriteAnimation* anim = TimedSpriteAnimation::create("Interfeiss/level_end/share.plist", 2);
    anim->setRotation(-4);
//    anim->setPosition(ccp(visibleSize.width/2, visibleSize.height-180));
    anim->setPosition(ccp(0,-68));
    anim->setTag(1111);
    addChild(anim);
    
    
    return true;
}

void DF::LevelEnd::HighScore::sendFeedback()
{
    User::getInstance()->mFeedBackGiven = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("feedback_beta", 1);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
    JniCalls::openURLJNI("http://bit.ly/1o7kP7z");
}

void DF::LevelEnd::HighScore::share()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //Try to ask nice about to sign in
    if (JniCalls::isFacebookSigned())
    {
        JniCalls::shareScoreToFB(aScoreTotal);
        shareBt->setVisible(false);
        removeChildByTag(1111);
        //Hide the share button - todo - show that shared it !!!
    }
    else
    {
        JniCalls::loginFacebook();
    }
#endif
    
}

bool DF::LevelEnd::DropingCup::init()
{
    CCSprite* cup = CCSprite::create("Interfeiss/level_end/highscore_cup.png");
    cup->setAnchorPoint(ccp(0, 1));
    cup->setPosition(ccp(0, 200));
    addChild(cup);
    
    cup->runAction(CCSequence::create(CCDelayTime::create(1.2),
                                       CCEaseBounceOut::create(CCMoveTo::create(0.5, ccp(0, 0))),
                                       NULL));
    
    return true;
}
