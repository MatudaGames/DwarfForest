//
//  LoadingScene.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 24/11/13.
//
//

#include "LoadingScene.h"
#include "StaticSceneManager.h"
#include "User.h"
#include "LoadingLayer.h"
#include <iostream>
#include <SimpleAudioEngine.h>
#include "AppMacros.h"
#include "Utils.h"

USING_NS_CC;
using namespace CocosDenshion;

CCScene* LoadingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LoadingScene* layer = LoadingScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

LoadingScene* LoadingScene::create()
{
    LoadingScene *pRet = new LoadingScene();
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

bool LoadingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //Lets create the intro
    CCSprite* aBG = CCSprite::create("Interfeiss/GV_logo/background.png");
    aBG->setAnchorPoint(ccp(0,0));
    addChild(aBG);
    
    //============================================================================
    //Play the stuff
    
    CCSprite* aLogo = CCSprite::create("Interfeiss/GV_logo/gv_mobile.png");
    aLogo->setPosition(ccp(visibleSize.width/2,visibleSize.height/2+50));
    aLogo->setOpacity(0);
    addChild(aLogo);
    
    CCFadeTo* aLogoAction1 = CCFadeTo::create(0.5f,255);
    CCDelayTime* aLogoDelay1 = CCDelayTime::create(0.5f);
    
    CCCallFuncN* aFunc1 = CCCallFuncN::create(this, callfuncN_selector(LoadingScene::playSound1));
    
    CCSequence* aLogoSeq1 = CCSequence::create(aLogoDelay1,aFunc1,aLogoAction1,NULL);
    aLogo->runAction(aLogoSeq1);
    
    //============================================================================
    // The mobile frame
    
    CCSprite* aMobileFrame = CCSprite::create("Interfeiss/GV_logo/frame.png");
    aMobileFrame->setPosition(ccp(visibleSize.width/2,visibleSize.height/2+50));
    aMobileFrame->setOpacity(0);
    addChild(aMobileFrame);
    
    CCFadeTo* aMobAction1 = CCFadeTo::create(0.5f,255);
    CCDelayTime* aMobDelay1 = CCDelayTime::create(1.5f);
    
    CCCallFuncN* aFunc2 = CCCallFuncN::create(this, callfuncN_selector(LoadingScene::playSound2));
    
    CCSequence* aMobSeq1 = CCSequence::create(aMobDelay1,aFunc2,aMobAction1,NULL);
    aMobileFrame->runAction(aMobSeq1);
    
    //============================================================================
    // The mobile button
    
    CCSprite* aButoonFrame = CCSprite::create("Interfeiss/GV_logo/button.png");
    aButoonFrame->setPosition(ccp(aMobileFrame->getContentSize().width/2,60));
    aButoonFrame->setOpacity(0);
    aMobileFrame->addChild(aButoonFrame);
    
    CCFadeTo* aButoonAction1 = CCFadeTo::create(0.5f,255);
    CCDelayTime* aButoonDelay1 = CCDelayTime::create(2.5f);
    
    CCCallFuncN* aFunc3 = CCCallFuncN::create(this, callfuncN_selector(LoadingScene::playSound3));
    
    CCSequence* aButoonSeq1 = CCSequence::create(aButoonDelay1,aFunc3,aButoonAction1,NULL);
    aButoonFrame->runAction(aButoonSeq1);
    
    //The mask stuff
//    CCLayerColor* background = CCLayerColor::create(ccc4(0, 0, 0, 255), visibleSize.width, visibleSize.height);
//    ClippingLayer* aLayer = ClippingLayer::create();
//    addChild(aLayer);
    
//    ClippingLayer* aLayer = ClippingLayer::create();
//    aLayer->SetSi
//    addChild(aLayer);
    
    //Run the animation shine
//    CCSprite* aShine = CCSprite::create("Interfeiss/GV_logo/shine.png");
    
//    mLayer = ClippingLayer::create();
//    CCLayerColor* aLayer = CCLayerColor::create(ccc4(255, 255, 255, 255), 100, 100);
//    aLayer->visit();
//    addChild(aLayer);
    
    LoadingLayer* aLayer2 = LoadingLayer::create(this);
//    aLayer2->setAnchorPoint(ccp(0,0));
    addChild(aLayer2);
    
    CCSprite* aShine = CCSprite::create("Interfeiss/GV_logo/shine.png");
//    aShine->setFlipX(true);
    aShine->setRotation(-20);
//    CCSprite* aShine = CCSprite::create("Interfeiss/GV_logo/button.png");
    aShine->setPosition(ccp(visibleSize.width/2,visibleSize.height/2+400));
    aLayer2->addChild(aShine);
    
    CCDelayTime* aShineFadeDelay1 = CCDelayTime::create(3.5f);
    CCFadeTo* aShineFadeIn = CCFadeTo::create(0.25f,255);
    CCDelayTime* aShineFadeDelay2 = CCDelayTime::create(0.8f);
    CCFadeTo* aShadeFadeOut = CCFadeTo::create(0.25f,0);
    CCSequence* aShadeFadeSeq = CCSequence::create(aShineFadeDelay1,aShineFadeIn,aShineFadeDelay2,aShadeFadeOut,NULL);
    
    CCMoveTo* aShineMove = CCMoveTo::create(0.5f,ccp(visibleSize.width/2,visibleSize.height/2-300));
    CCDelayTime* aShineDelay = CCDelayTime::create(3.5);
    
    CCCallFuncN* aFunc4 = CCCallFuncN::create(this, callfuncN_selector(LoadingScene::playSound4));
    
    CCCallFuncN* aFunc5 = CCCallFuncN::create(this, callfuncN_selector(LoadingScene::exitScene));
    
    CCSequence* aShineSeq = CCSequence::create(aShineDelay,aFunc4,aShineMove,aFunc5,NULL);
    
    CCSpawn* aShineSpawn = CCSpawn::create(aShadeFadeSeq,aShineSeq,NULL);
    
    aShine->runAction(aShineSpawn);
    
    
//    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("logo_sound").c_str(),false);
    
    
    return true;
}

void LoadingScene::exitScene()
{
//    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("logo_sound_1").c_str(),false);
    //Play the menu
    CCScene* tutorial = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);;
	CCTransitionScene* transition = CCTransitionFade::create(0.5f, tutorial);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void LoadingScene::playSound1()
{
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("logo_sound_1").c_str(),false);
}

void LoadingScene::playSound2()
{
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("logo_sound_2").c_str(),false);
}

void LoadingScene::playSound3()
{
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("logo_sound_3").c_str(),false);
}

void LoadingScene::playSound4()
{
    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("logo_sound_4").c_str(),false);
}

//void LoadingScene::visit()
//{
////    kmGLPushMatrix();
//    
//    glEnable(GL_SCISSOR_TEST);
////    glScissor(100, 100, 100, 100);
//    CCEGLView::sharedOpenGLView()->setScissorInPoints(100 , 100, 100 , 100);
//    CCNode::visit();
//    glDisable(GL_SCISSOR_TEST);
//    
////    kmGLPopMatrix();
//    
////    glScissor(clippingRegion.origin.x + position_.x, clippingRegion.origin.y + position_.y,
////              clippingRegion.size.width, clippingRegion.size.height);
//}

void LoadingScene::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void LoadingScene::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool LoadingScene::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (_moveStarted == false)
    {
        _moveStarted = true;
        return true;
    }
    else
    {
        return false;
    }
}

void LoadingScene::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    _moveStarted = false;
}

