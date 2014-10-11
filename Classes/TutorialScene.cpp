//#include <SimpleAudioEngine.h>
#include "TutorialScene.h"
#include "MainMenuScene.h"
#include "OptionsScene.h"
#include "GameScene.h"
#include "User.h"
#include "PauseScene.h"

USING_NS_CC;
//using namespace CocosDenshion;

CCScene* TutorialScene::scene(bool firstTime)
{
    // 'scene' is an autorelease object
    CCScene* scene = CCScene::create();
    
    // 'layer' is an autorelease object
    TutorialScene* layer = TutorialScene::create(firstTime);
    
    std::cout << "FT: " << firstTime << "\n";
    
    if (firstTime)
    {
        User::getInstance()->setPlayed();
    }
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

TutorialScene* TutorialScene::create(bool firstTime)
{
    TutorialScene *pRet = new TutorialScene();
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

// on "init" you need to initialize your instance
bool TutorialScene::init(bool firstTime)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    
    //navigation
    _firstTime = firstTime;
    _moveStarted = false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCLayerColor* background = CCLayerColor::create(ccc4(0, 0, 0, 255), visibleSize.width, visibleSize.height);
    background->setAnchorPoint(ccp(0,0));
    addChild(background);

    CCMenuItemImage* nextImgItem = CCMenuItemImage::create(
                                                             "Interfeiss/buttons/arrow_r_btn0001.png",
                                                             "Interfeiss/buttons/arrow_r_btn0002.png",
                                                             this,
                                                             menu_selector(TutorialScene::btNextCallback));
    
    CCMenuItemImage* prevImgItem = CCMenuItemImage::create(
                                                           "Interfeiss/buttons/arrow_l_btn0001.png",
                                                           "Interfeiss/buttons/arrow_l_btn0002.png",
                                                           this,
                                                           menu_selector(TutorialScene::btPrevCallback));
    
    CCMenuItemImage* closeItem = CCMenuItemImage::create(
                                                           "Interfeiss/buttons/close_btn0001.png",
                                                           "Interfeiss/buttons/close_btn0002.png",
                                                           this,
                                                           menu_selector(TutorialScene::btCloseCallback));
    
    _images[0] = "Interfeiss/tutorial/1.png";
    _images[1] = "Interfeiss/tutorial/2.png";
    _images[2] = "Interfeiss/tutorial/3.png";
    _images[3] = "Interfeiss/tutorial/4.png";
    _images[4] = "Interfeiss/tutorial/5.png";
    _images[5] = "Interfeiss/tutorial/6.png";
    
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
    _activeImage->setPositionY((visibleSize.height - _activeImage->getContentSize().height) / 2);
    addChild(_activeImage,0);
    _activeImg = 0;
    showFirstPageAnimation();
    
    addChild(nextMenu, 1);
    addChild(prevMenu, 2);
//    addChild(closeMenu, 3);
    
    return true;
}

void TutorialScene::showFirstPageAnimation()
{
    _firstPageAnimation = CCLayer::create();
    _firstPageAnimation->setAnchorPoint(ccp(0,0));
    addChild(_firstPageAnimation);
    
    // finger
    _finger = CCSprite::create("Interfeiss/tutorial/hand_cursor.png");
    _finger->setAnchorPoint(ccp(0.149, 0.925)); // finger pointer is at 20, 10
    _finger->setPosition(ccp(1000, 400));
    _finger->setOpacity(0);
    
    ccBezierConfig bezier;
    bezier.controlPoint_1 = ccp(1096, 168);
    bezier.controlPoint_2 = ccp(635, 105);
    bezier.endPosition = ccp(575, 360);
    
    CCAction* fingerAction = CCRepeatForever::create(CCSequence::create(CCDelayTime::create(0.5),
                                                CCFadeIn::create(0),
                                                CCMoveTo::create(0.5, ccp(770, 480)),
                                                CCScaleTo::create(0.1, 0.9),
                                                CCCallFunc::create(this, callfunc_selector(TutorialScene::showPoint)),
                                                CCScaleTo::create(0.1, 1),
                                                CCBezierTo::create(3, bezier),
                                                CCDelayTime::create(1.5),
                                                CCFadeOut::create(0),
                                                CCMoveTo::create(0, ccp(1000, 400)),
                                                NULL));
    
    _finger->runAction(fingerAction);
    
    _firstPageAnimation->addChild(_finger, 1);
}

void TutorialScene::showPoint()
{
    _point = CCSprite::create("Interfeiss/tutorial/ripple_effect.png");
    _point->setPosition(ccp(770, 480));
    _point->setScale(0.6);
    _point->setOpacity(0);
    _firstPageAnimation->addChild(_point, 0);
    
    _point->runAction(CCScaleTo::create(0.4, 1));
    _point->runAction(CCSequence::create(
                                        CCFadeTo::create(0.15, 128),
                                        CCFadeTo::create(0.15, 0),
                                        NULL));
}

void TutorialScene::hideFirstPageAnimation()
{
    if (_firstPageAnimation != NULL)
    {
        if (_finger != NULL)
            _finger->stopAllActions();
        if (_point != NULL)
            _point->stopAllActions();
        removeChild(_firstPageAnimation);
        _firstPageAnimation = NULL;
    }
}

void TutorialScene::btNextCallback(cocos2d::CCObject *sender)
{
    _activeImg++;
    if (_activeImg < IMG_CNT)
    {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        
        CCSprite* newImg = CCSprite::create(_images[_activeImg].c_str());
        newImg->cocos2d::CCNode::setAnchorPoint(ccp(0,0));
        newImg->setOpacity(0);
        addChild(newImg);
        newImg->setPositionY((visibleSize.height - newImg->getContentSize().height) / 2);
        
        CCAction* a1 = CCFadeOut::create(0.5);
        CCAction* a2 = CCFadeIn::create(0.5);
        
        _activeImage->runAction(a1);
        newImg->runAction(a2);
        
        _activeImage = newImg;
        
        if (_activeImg == 0)
        {
            showFirstPageAnimation();
        }
        else
        {
            hideFirstPageAnimation();
        }
    }
    else
    {
        btCloseCallback(NULL);
    }
}

void TutorialScene::btPrevCallback(cocos2d::CCObject *sender)
{
    if (_activeImg > 0)
    {
        _activeImg--;
        
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        
        CCSprite* newImg = CCSprite::create(_images[_activeImg].c_str());
        newImg->cocos2d::CCNode::setAnchorPoint(ccp(0,0));
        newImg->setOpacity(0);
        addChild(newImg);
        newImg->setPositionY((visibleSize.height - newImg->getContentSize().height) / 2);
        
        
        CCAction* a1 = CCFadeOut::create(0.5);
        CCAction* a2 = CCFadeIn::create(0.5);
        
        _activeImage->runAction(a1);
        newImg->runAction(a2);
        
        _activeImage = newImg;
        
        if (_activeImg == 0)
        {
            showFirstPageAnimation();
        }
        else
        {
            hideFirstPageAnimation();
        }
    }
}

void TutorialScene::btCloseCallback(cocos2d::CCObject *sender)
{
    if (_firstTime)
    { // this is set as scene not layer
        CCScene* scene = GameScene::scene();
        CCTransitionScene* transition = CCTransitionProgressOutIn::create(0.5f, scene);
        CCDirector::sharedDirector()->replaceScene(transition);
    }
    else
    {
//        CCActionInterval* actionTo = CCScaleTo::create(0.5, 0);
//        CCFiniteTimeAction* action = CCEaseOut::create(actionTo, 3);
//        this->runAction(CCSequence::create(action,
//                                           CCCallFunc::create(this, callfunc_selector(TutorialScene::onCloseTutorial)),
//                                           NULL));
        if(getTag()==9119)
        {
            PauseScene* aOption = (PauseScene*)getParent()->getParent();
            aOption->OnExitTutorial();
        }
        else
        {
            OptionsScene* aOption = (OptionsScene*)getParent()->getParent();
            aOption->OnExitTutorial();
        }
        

    }
}

void TutorialScene::onCloseTutorial()
{
    this->getParent()->removeChild(this);
}

void TutorialScene::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void TutorialScene::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool TutorialScene::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
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

void TutorialScene::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if(abs(_dragStart.x - touch->getLocation().x)<10)
        btNextCallback(NULL);
    
    _moveStarted = false;
}

void TutorialScene::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
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
