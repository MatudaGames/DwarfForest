//
//  SupportPopup.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 12/05/14.
//
//

#include "SupportPopup.h"
#include "AppMacros.h"
#include "GameScene.h"
#include "JniCalls.h"
#include "User.h"

USING_NS_CC;

SupportPopup* SupportPopup::create(cocos2d::CCLayer* parent)
{
    SupportPopup *layer = new SupportPopup();
    
    if (layer && layer->init(parent))
    {
        // Set to autorelease
        layer->autorelease();
        
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool SupportPopup::init(cocos2d::CCLayer* parent)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCLayerColor* back = CCLayerColor::create(ccc4(0, 0, 0, 160), visibleSize.width, visibleSize.height);
    back->setAnchorPoint(ccp(0.5,0.5));
//    back->setPosition(ccp(visibleSize.width/2,visibleSize.height/2));
    addChild(back);
    
    //The center stuff
//    support.png
    CCSprite* aDebug1 = CCSprite::create("support.png");
    aDebug1->setPosition(ccp(back->getContentSize().width/2,back->getContentSize().height/2));
    back->addChild(aDebug1);
    
    CCMenuItemImage* image = CCMenuItemImage::create("beta_feedback.png", "beta_feedback.png",
                                                     this, menu_selector(SupportPopup::menuOkCallback));
    CCMenuItemImage* image2 = NULL;
    
    //Quick update to 2 buttons
    image2 = CCMenuItemImage::create("Interfeiss/buttons/x_btn0001.png", "Interfeiss/buttons/x_btn0002.png",
                                     this, menu_selector(SupportPopup::menuCancel));
    image2->setPosition(300, 400);
    image->setPosition(0,120);
    
    CCMenu* menu = CCMenu::create(image,image2, NULL);
    menu->setPosition(ccp(visibleSize.width / 2, 150));
    addChild(menu);
    
//    CCLabelTTF* label = CCLabelTTF::create(message.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.6, CCSize(visibleSize.width-200, visibleSize.height-300), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//    
//    label->setPosition(ccp(100, 200));
//    label->setAnchorPoint(ccp(0, 0));
//    label->setColor(ccc3(0,0,0));
//    addChild(label);
    
    parent->addChild(this, 100000);
    
    //Pause the game if in game
    if (parent->getTag()==11)
    {
        //This is the game tag !!!
        GameScene* gameScene = dynamic_cast<GameScene*>(parent);
        gameScene->setGamePause(true);
        gameScene->pauseSchedulerAndActionsRecursive(gameScene,false);
    }
    
    return true;
}

void SupportPopup::menuOkCallback(CCObject* sender)
{
    User::getInstance()->mFeedBackGiven = 1;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("feedback_beta", 1);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
//    CCObject* parent = (CCObject*)this->getParent();
    JniCalls::openURLJNI("http://bit.ly/1o7kP7z");
    //Small check for pause!!!
    this->getParent()->removeChild(this);
}

void SupportPopup::menuCancel(CCObject* sender)
{
//    CCObject* parent = (CCObject*)this->getParent();
    this->getParent()->removeChild(this);
}