#include "FreeStuffPopup.h"
#include "User.h"
#include "LevelEndScene.h"

USING_NS_CC;

//CCScene* FreeStuffPopup::scene()
//{
//    // 'scene' is an autorelease object
//    CCScene *scene = CCScene::create();
//    
//    // 'layer' is an autorelease object
//    FreeStuffPopup *layer = FreeStuffPopup::create();
//	
//    // add layer as a child to scene
//    scene->addChild(layer);
//	
//    // return the scene
//    return scene;
//}

bool FreeStuffPopup::init()
{
    if (!CCLayer::init()) return false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/free_diamonds_popup/free_stuff_pop.png");
    back->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
    addChild(back);
    
    // 3 menus
    
    CCMenuItemImage* notNowBt = CCMenuItemImage::create("Interfeiss/rate_game_popup/notnow_btn0001.png",
                                                        "Interfeiss/rate_game_popup/notnow_btn0002.png",
                                                        this, menu_selector(FreeStuffPopup::notNowCallback));
    CCMenu* notNowMenu = CCMenu::create(notNowBt, NULL);
    notNowMenu->setPosition(ccp(400, 150));
    addChild(notNowMenu);
    
    CCMenuItemImage* facebookBt = CCMenuItemImage::create("Interfeiss/free_diamonds_popup/diamonds0001.png",
                                                          "Interfeiss/free_diamonds_popup/diamonds0002.png",
                                                          this, menu_selector(FreeStuffPopup::facebookCallback));
    CCMenu* facebookMenu = CCMenu::create(facebookBt, NULL);
    facebookMenu->setPosition(ccp(605, 365));
    addChild(facebookMenu);
    
    CCMenuItemImage* twitterBt = CCMenuItemImage::create("Interfeiss/free_diamonds_popup/diamonds0001.png",
                                                          "Interfeiss/free_diamonds_popup/diamonds0002.png",
                                                          this, menu_selector(FreeStuffPopup::facebookCallback));
    CCMenu* twitterMenu = CCMenu::create(twitterBt, NULL);
    twitterMenu->setPosition(ccp(650, 265));
    addChild(twitterMenu);
    
    // 1 CB
    CCMenuItemImage* v1 = CCMenuItemImage::create("Interfeiss/rate_game_popup/checkbox_empty.png",
                                                  NULL, this, NULL);
    
    CCMenuItemImage* v2 = CCMenuItemImage::create("Interfeiss/rate_game_popup/checkbox_checked.png",
                                                  NULL, this, NULL);
    
    cbButton = CCMenuItemToggle::createWithTarget(this, NULL, v1, v2, NULL);
    CCMenu* cbMenu = CCMenu::create(cbButton, NULL);
    cbMenu->setPosition(ccp(472, 240));
    addChild(cbMenu);

    return true;
}

void FreeStuffPopup::notNowCallback()
{
    if (cbButton->getSelectedIndex())
    {
        User::getInstance()->setShowFreeStuffPopup(false);
    }
    
    User::getInstance()->setFreeDmdsWeek();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    this->runAction(CCSequence::create(CCMoveTo::create(0.5, ccp(0, visibleSize.height)),
                                       CCCallFunc::create(this, callfunc_selector(FreeStuffPopup::remove)),
                                       NULL));
}

void FreeStuffPopup::facebookCallback()
{
    notNowCallback();
    
    // TODO: implement facebook call
}

void FreeStuffPopup::twitterCallback()
{
    notNowCallback();
    
    // TODO: implement twitter call
}

void FreeStuffPopup::remove()
{
//    _gameScene->CreateLevelEnd(this);
    ((DF::LevelEnd::LevelEndScene*)this->getParent())->showNextPopup();
}

void FreeStuffPopup::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void FreeStuffPopup::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool FreeStuffPopup::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    return true;
}

