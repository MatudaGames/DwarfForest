//
//  FreeDiamondsScene.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 7/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "FreeDiamondsScene.h"
#include "MainMenuScene.h"
#include "User.h"
#include "PopUp.h"
#include <iostream>

//#include "FlurryX.h"
#include "JniCalls.h"

USING_NS_CC;
//using namespace CocosDenshion;

CCScene* FreeDiamondsScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    FreeDiamondsScene *layer = FreeDiamondsScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool FreeDiamondsScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
//    FlurryX::logEvent("Free Diamonds Opened");
    
    //navigation
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
    back->setAnchorPoint(ccp(0,0));
    addChild(back, 0);
    
    CCMenuItemImage* backButton = CCMenuItemImage::create(
														  "Interfeiss/backbtn0001.png",
														  "Interfeiss/backbtn0002.png",
														  this,
														  menu_selector(FreeDiamondsScene::menuBackCallback));
	backButton->setAnchorPoint(ccp(0,0));
    
    CCMenu* backMenu = CCMenu::create(backButton, NULL);
    this->addChild(backMenu, 1);
    backMenu->setPosition(10, 10);
    
    
    CCSprite* titles = CCSprite::create("Interfeiss/free_diamonds/free_diamond_text.png");
    titles->setAnchorPoint(ccp(0.5, 1));
    titles->setPosition(ccp(visibleSize.width / 2, visibleSize.height - 120));
    addChild(titles);
    
    //Check what to show if all done correct
    // if you use SimpleAudioEngine, it must resume here
    CCMenuItemImage* fbButton;
    
    int aDummyInt = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("facebook_bonus", 0);
    if (aDummyInt == 1)//We got the bonus already
    {
        //Do not show the buttons !!!
        fbButton = CCMenuItemImage::create(
                                                            "Interfeiss/free_diamonds/diamond_btn.png",
                                                            "Interfeiss/free_diamonds/diamond_btn_pressed.png",
                                                            this,
                                                            NULL);
    }
    else
    {
        fbButton = CCMenuItemImage::create(
                                                            "Interfeiss/free_diamonds/diamond_btn.png",
                                                            "Interfeiss/free_diamonds/diamond_btn_pressed.png",
                                                            this,
                                                            menu_selector(FreeDiamondsScene::facebookCallback));
    }
    
    CCMenuItemImage* tweetButton;
    aDummyInt = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("twitter_bonus", 0);
    if (aDummyInt == 1)//We got the bonus already
    {
        tweetButton = CCMenuItemImage::create(
                                              "Interfeiss/free_diamonds/diamond_btn.png",
                                              "Interfeiss/free_diamonds/diamond_btn_pressed.png",
                                              this,
                                              menu_selector(FreeDiamondsScene::twitterCallback));
    }
    else
    {
        tweetButton = CCMenuItemImage::create(
                                              "Interfeiss/free_diamonds/diamond_btn.png",
                                              "Interfeiss/free_diamonds/diamond_btn_pressed.png",
                                              this,
                                              menu_selector(FreeDiamondsScene::twitterCallback));
    }
    
    CCMenu* mainMenu = CCMenu::create(fbButton, tweetButton, NULL);
    mainMenu->alignItemsVerticallyWithPadding(50);
    this->addChild(mainMenu, 1);
    
    return true;
}

void FreeDiamondsScene::menuBackCallback(CCObject* sender)
{
	CCScene* mainMenu = MainMenuScene::scene();
	CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, mainMenu);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void FreeDiamondsScene::facebookCallback(cocos2d::CCObject *sender)
{
    int aDummyInt = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("facebook_bonus", 0);
    if(aDummyInt==1)
        return;
    
//    FlurryX::logEvent("Free_Diamonds Facebook");
    
    User::getInstance()->addDiamonds(20);
    
//    NSURL * url = [NSURL URLWithString:@"http://stackoverflow.com"];
//    [[UIApplication sharedApplication] openURL: url];
    
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("facebook_bonus", 1);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
    JniCalls::showFacebookFollow();
    
    PopUp::create(this, "+20 diamonds", NULL, false);
}

void FreeDiamondsScene::twitterCallback(cocos2d::CCObject *sender)
{
    int aDummyInt = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("twitter_bonus", 0);
    if(aDummyInt==1)
        return;
    
//    FlurryX::logEvent("Free_Diamonds Twitter");
    
    User::getInstance()->addDiamonds(20);
    
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("twitter_bonus", 1);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
    JniCalls::showTwitterFollow();
    
    PopUp::create(this, "+20 diamonds", NULL, false);
}

