#include "RateGamePopup.h"
#include "User.h"

USING_NS_CC;

bool RateGamePopup::init()
{
    if (!CCLayer::create()) return false;
    
    background = CCLayerColor::create(ccc4(0, 0, 0, 255));
    background->setOpacity(200);
    addChild(background);
    
    container = CCLayer::create();
    container->setAnchorPoint(ccp(0,0));
    addChild(container);
    
    setAnchorPoint(ccp(0,0));
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/rate_game_popup/dwarf_rater.png");
    back->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
    container->addChild(back);
    
    CCMenuItemImage* rateBt = CCMenuItemImage::create("Interfeiss/rate_game_popup/rate_btn0001.png",
                                                      "Interfeiss/rate_game_popup/rate_btn0002.png",
                                                      this, menu_selector(RateGamePopup::rateCallback));
    CCMenu* rateMenu = CCMenu::create(rateBt, NULL);
    rateMenu->setPosition(ccp(565, 330));
    container->addChild(rateMenu);
    
    // 1 CB
    CCMenuItemImage* v1 = CCMenuItemImage::create("Interfeiss/rate_game_popup/checkbox_empty.png",
                                                  NULL, this, NULL);
    
    CCMenuItemImage* v2 = CCMenuItemImage::create("Interfeiss/rate_game_popup/checkbox_checked.png",
                                                  NULL, this, NULL);
    
    cbButton = CCMenuItemToggle::createWithTarget(this, NULL, v1, v2, NULL);
    CCMenu* cbMenu = CCMenu::create(cbButton, NULL);
    cbMenu->setPosition(ccp(572, 225));
    container->addChild(cbMenu);
    
    
    CCMenuItemImage* notNowBt = CCMenuItemImage::create("Interfeiss/rate_game_popup/notnow_btn0001.png",
                                                        "Interfeiss/rate_game_popup/notnow_btn0002.png",
                                                        this, menu_selector(RateGamePopup::notNowCallback));
    CCMenu* notNowMenu = CCMenu::create(notNowBt, NULL);
    notNowMenu->setPosition(ccp(470, 135));
    container->addChild(notNowMenu);
    
    return true;
}

void RateGamePopup::notNowCallback()
{
    if (cbButton->getSelectedIndex())
    {
        User::getInstance()->setShowRateGamePopup(true);
    }
    
    User::getInstance()->setRateGameDay();
    
    background->runAction(CCFadeTo::create(0.5, 0));
    container->runAction(CCSequence::create(
                                            CCMoveTo::create(0.5, ccp(0,-720)),
                                            CCCallFunc::create(this, callfunc_selector(RateGamePopup::remove)),
                                            NULL
                                            ));
}

void RateGamePopup::remove()
{
    this->getParent()->removeChild(this);
}

void RateGamePopup::rateCallback()
{
    notNowCallback();
    
    // TODO: market rate call
}

bool RateGamePopup::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    return true;
}

void RateGamePopup::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    
    background->setOpacity(0);
    container->setPosition(ccp(0,-720));
    
    background->runAction(CCFadeTo::create(0.5, 200));
    container->runAction(CCMoveTo::create(0.5, ccp(0,0)));
    
    CCLayer::onEnter();
}

void RateGamePopup::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
