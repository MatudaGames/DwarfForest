//
//  IntroScene.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 7/10/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "IntroScene.h"
#include "StaticSceneManager.h"
#include "User.h"
#include <iostream>
#include "OptionsScene.h"

USING_NS_CC;
//using namespace CocosDenshion;

CCScene* IntroScene::scene(bool firstTime)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    IntroScene* layer = IntroScene::create(firstTime);
    
    if (firstTime)
    {
        User::getInstance()->setOpened();
    }
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

IntroScene* IntroScene::create(bool firstTime)
{
    IntroScene *pRet = new IntroScene();
    if (pRet && pRet->init(firstTime))
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

bool IntroScene::init(bool firstTime)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    //navigation
    _firstTime = firstTime;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCMenuItemImage* nextImgItem = CCMenuItemImage::create(
                                                           "Interfeiss/buttons/arrow_r_btn0001.png",
                                                           "Interfeiss/buttons/arrow_r_btn0002.png",
                                                           this,
                                                           menu_selector(IntroScene::btNextCallback));
    
    CCMenuItemImage* prevImgItem = CCMenuItemImage::create(
                                                           "Interfeiss/buttons/arrow_l_btn0001.png",
                                                           "Interfeiss/buttons/arrow_l_btn0002.png",
                                                           this,
                                                           menu_selector(IntroScene::btPrevCallback));
    
    CCMenuItemImage* closeItem = CCMenuItemImage::create(
                                                         "Interfeiss/buttons/close_btn0001.png",
                                                         "Interfeiss/buttons/close_btn0002.png",
                                                         this,
                                                         menu_selector(IntroScene::btCloseCallback));
    
    _images[0] = "Interfeiss/intro/scene_1.png";
    _images[1] = "Interfeiss/intro/scene_2.png";
    _images[2] = "Interfeiss/intro/scene_3.png";
    _images[3] = "Interfeiss/intro/scene_4.png";
    _images[4] = "Interfeiss/intro/scene_5.png";
    
    nextImgItem->setAnchorPoint(ccp(1,0));
    prevImgItem->setAnchorPoint(ccp(0,0));
    closeItem->setAnchorPoint(ccp(1,1));
    
    CCMenu* nextMenu = CCMenu::create(nextImgItem, NULL);
    CCMenu* prevMenu = CCMenu::create(prevImgItem, NULL);
    CCMenu* closeMenu = CCMenu::create(closeItem, NULL);
    
    prevMenu->setPosition(ccp(10,10));
    nextMenu->setPosition(visibleSize.width - 10, 10);
    closeMenu->setPosition(visibleSize.width - 10, visibleSize.height - 10);
    
    _activeImage = CCSprite::create(_images[0].c_str());
    _activeImage->setAnchorPoint(ccp(0,0));
    addChild(_activeImage,0);
    _activeImg = 0;
    
    addChild(nextMenu,1);
    addChild(prevMenu,2);
//    addChild(closeMenu,3);
    
    return true;
}

void IntroScene::btNextCallback(cocos2d::CCObject *sender)
{
    _activeImg++;
    if (_activeImg < IMG_CNT)
    {
        CCSprite* newImg = CCSprite::create(_images[_activeImg].c_str());
        newImg->cocos2d::CCNode::setAnchorPoint(ccp(0,0));
        newImg->setOpacity(0);
        addChild(newImg);
        
        CCAction* a1 = CCFadeOut::create(0.5);
        CCAction* a2 = CCFadeIn::create(0.5);
        
        _activeImage->runAction(a1);
        newImg->runAction(a2);
        
        _activeImage = newImg;
    }
    else
    {
        btCloseCallback(NULL);
    }
}

void IntroScene::btPrevCallback(cocos2d::CCObject *sender)
{
    if (_activeImg > 0)
    {
        _activeImg--;
        
        CCSprite* newImg = CCSprite::create(_images[_activeImg].c_str());
        newImg->cocos2d::CCNode::setAnchorPoint(ccp(0,0));
        newImg->setOpacity(0);
        addChild(newImg);
        
        CCAction* a1 = CCFadeOut::create(0.5);
        CCAction* a2 = CCFadeIn::create(0.5);
        
        _activeImage->runAction(a1);
        newImg->runAction(a2);
        
        _activeImage = newImg;
    }
}

void IntroScene::btCloseCallback(cocos2d::CCObject *sender)
{
//    CCScene* tutorial;
//    if (_firstTime)
//    {
//        tutorial = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
//    }
//    else
//    {
//        tutorial = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::OPTIONS);
//    }
//	CCTransitionScene* transition = CCTransitionProgressOutIn::create(0.5f, tutorial);
//	CCDirector::sharedDirector()->replaceScene(transition);
    
    //Just remove it from main menu !!!
    if (_firstTime)
    {
        
    }
    else
    {
        OptionsScene* aOption = (OptionsScene*)getParent()->getParent();
        aOption->OnExitTutorial();
    }
}

void IntroScene::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void IntroScene::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool IntroScene::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (_moveStarted == false)
    {
        _moveStarted = true;
        _dragStart = touch->getLocation();
        
        return true;
    }
    else
    {
        return false;
    }
}

void IntroScene::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(abs(_dragStart.x - touch->getLocation().x)<10)
        btNextCallback(NULL);
    
    _moveStarted = false;
}

void IntroScene::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (_moveStarted && touch->getLocation().y < 370)
    {
        if (touch->getLocation().x - _dragStart.x > 150)
        {
            _moveStarted = false;
            btPrevCallback(NULL);
        }
        else if (touch->getLocation().x - _dragStart.x < -150)
        {
            _moveStarted = false;
            btNextCallback(NULL);
        }
    }
}
