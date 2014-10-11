//
//  Popup.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 9/1/13.
//
//

#include "Popup.h"
#include "AppMacros.h"
#include "GameScene.h"

USING_NS_CC;

PopUp* PopUp::create(cocos2d::CCLayer* parent, const std::string& message, CallBack selector, bool theCancelButton,bool theExitGame)
{
    PopUp *layer = new PopUp();
    
    if (layer && layer->init(parent, message, selector, theCancelButton,theExitGame))
    {
        // Set to autorelease
        layer->autorelease();
        
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool PopUp::init(cocos2d::CCLayer* parent, const std::string& message, CallBack selector, bool theCancelButton,bool theExitGame)
{
	_callback = selector;
    _cancelButton = theCancelButton;
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCLayerColor* back = CCLayerColor::create(ccc4(255, 255, 255, 255), visibleSize.width - 200, visibleSize.height - 200);
    back->setAnchorPoint(ccp(0,0));
    back->setPosition(ccp(100,100));
    addChild(back);
    
    CCMenuItemImage* image = CCMenuItemImage::create("Interfeiss/backbtn0001.png", "Interfeiss/backbtn0002.png",
                                                     this, menu_selector(PopUp::menuOkCallback));
    CCMenuItemImage* image2 = NULL;
    
    //Quick update to 2 buttons
    if (_cancelButton)
    {
        image2 = CCMenuItemImage::create("Interfeiss/buttons/close_btn0001.png", "Interfeiss/buttons/close_btn0002.png",
                                         this, menu_selector(PopUp::menuCancel));
        image2->setPosition(200, 0);
        image->setPosition(-100,0);
    }
    
    CCMenu* menu = CCMenu::create(image,image2, NULL);
    menu->setPosition(ccp(visibleSize.width / 2, 150));
    addChild(menu);
    
    CCLabelTTF* label = CCLabelTTF::create(message.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.6, CCSize(visibleSize.width-200, visibleSize.height-300), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    
    label->setPosition(ccp(100, 200));
    label->setAnchorPoint(ccp(0, 0));
    label->setColor(ccc3(0,0,0));
    addChild(label);
    
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

void PopUp::menuOkCallback(CCObject* sender)
{
    CCObject* parent = (CCObject*)this->getParent();
    //Small check for pause!!!
    if (getParent()->getTag()==11)
    {
        GameScene* gameScene = dynamic_cast<GameScene*>(parent);
        gameScene->onPauseFinished();
    }
    
    this->getParent()->removeChild(this);
    
    if (_callback != NULL)
    {
        (parent->*_callback)();
    }
}

void PopUp::menuCancel(CCObject* sender)
{
    CCObject* parent = (CCObject*)this->getParent();
    //Small check for pause!!!
    if (getParent()->getTag()==11)
    {
        GameScene* gameScene = dynamic_cast<GameScene*>(parent);
        gameScene->onPauseFinished();
    }
    
    this->getParent()->removeChild(this);
}
