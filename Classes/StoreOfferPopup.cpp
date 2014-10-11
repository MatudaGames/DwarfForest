#include "StoreOfferPopup.h"
#include "AppMacros.h"
#include "Utils.h"
#include "User.h"
#include "GameScene.h"
#include <sys/time.h>

USING_NS_CC;

CCScene* StoreOfferPopup::scene()
{
    CCScene *scene = CCScene::create();
    StoreOfferPopup *layer = StoreOfferPopup::create();
    scene->addChild(layer);
    return scene;
}

bool StoreOfferPopup::init()
{
    if (!CCLayer::create()) return false;
    
    User::getInstance()->setStoreOfferDay();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* fons = CCSprite::create("Interfeiss/bg_2.png");
    fons->setAnchorPoint(ccp(0,0));
    addChild(fons, 0);
    
    CCSprite* back = CCSprite::create("Interfeiss/store_popup/weekly_offer.png");
    back->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
    addChild(back);
    
    std::string price = "6.99$";
    std::string reward = "+400";
    
    // active sprite
    CCSprite* activeBt = CCSprite::create("Interfeiss/store_popup/offer0001.png");
    
    CCLabelTTF* aRew = CCLabelTTF::create(reward.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*1.2, CCSize(250, 100), kCCTextAlignmentCenter);
    aRew->setColor(ccc3(255,255,255));
    aRew->setPosition(ccp(200, 140));
    aRew->enableShadow(CCSize(-1,1), 2, 4, true);
    activeBt->addChild(aRew);
    
    CCLabelTTF* aPrice = CCLabelTTF::create(price.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.75, CCSize(250, 100), kCCTextAlignmentCenter);
    aPrice->setColor(ccc3(255,255,255));
    aPrice->setPosition(ccp(215, 18));
    aPrice->setRotation(-13);
    activeBt->addChild(aPrice);
    
    // inactive sprite
    CCSprite* inactiveBt = CCSprite::create("Interfeiss/store_popup/offer0002.png");
    
    CCLabelTTF* iRew = CCLabelTTF::create(reward.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*1.2, CCSize(250, 100), kCCTextAlignmentCenter);
    iRew->setColor(ccc3(255,255,255));
    iRew->setPosition(ccp(200, 140));
    iRew->enableShadow(CCSize(-1,1), 2, 4, true);
    iRew->setRotation(3);
    inactiveBt->addChild(iRew);
    
    CCLabelTTF* iPrice = CCLabelTTF::create(price.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.75, CCSize(250, 100), kCCTextAlignmentCenter);
    iPrice->setColor(ccc3(255,255,255));
    iPrice->setPosition(ccp(216, 16));
    iPrice->setRotation(-13);
    inactiveBt->addChild(iPrice);
    
    CCMenuItemSprite* offerBt = CCMenuItemSprite::create(activeBt, inactiveBt, this, menu_selector(StoreOfferPopup::storeOfferCallback));
    
    CCMenu* offerMenu = CCMenu::create(offerBt, NULL);
    offerMenu->setPosition(ccp(640, 320));
    addChild(offerMenu);
    
    // counteris
    h1 = CCLabelTTF::create("2", FONT_SKRANJI, TITLE_FONT_SIZE*0.8, CCSize(60, 60), kCCTextAlignmentCenter);
    h1->setColor(ccc3(0,0,0));
    h1->setPosition(ccp(432, 217));
    addChild(h1);
    
    h2 = CCLabelTTF::create("2", FONT_SKRANJI, TITLE_FONT_SIZE*0.8, CCSize(60, 60), kCCTextAlignmentCenter);
    h2->setColor(ccc3(0,0,0));
    h2->setPosition(ccp(469, 218));
    addChild(h2);
    
    delim = CCLabelTTF::create(":", FONT_SKRANJI, TITLE_FONT_SIZE*0.8, CCSize(60, 60), kCCTextAlignmentCenter);
    delim->setColor(ccc3(0,0,0));
    delim->setPosition(ccp(492, 227));
    addChild(delim);
    
    m1 = CCLabelTTF::create("2", FONT_SKRANJI, TITLE_FONT_SIZE*0.8, CCSize(60, 60), kCCTextAlignmentCenter);
    m1->setColor(ccc3(0,0,0));
    m1->setPosition(ccp(516, 219));
    addChild(m1);
    
    m2 = CCLabelTTF::create("2", FONT_SKRANJI, TITLE_FONT_SIZE*0.8, CCSize(60, 60), kCCTextAlignmentCenter);
    m2->setColor(ccc3(0,0,0));
    m2->setPosition(ccp(553, 220));
    addChild(m2);
    
    // Close
    CCMenuItemImage* notNowBt = CCMenuItemImage::create("Interfeiss/rate_game_popup/notnow_btn0001.png",
                                                        "Interfeiss/rate_game_popup/notnow_btn0002.png",
                                                        this, menu_selector(StoreOfferPopup::notNowCallback));
    CCMenu* notNowMenu = CCMenu::create(notNowBt, NULL);
    notNowMenu->setPosition(ccp(500, 140));
    addChild(notNowMenu);
    
    this->scheduleUpdate();
    
    return true;
}

void StoreOfferPopup::update(float delta)
{
    updateCounter();
}

void StoreOfferPopup::updateCounter()
{
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    
    float time = 24 * 3600 - (t->tm_hour * 3600 + t->tm_min * 60 + t->tm_sec); // seconds till the end
    if (time < 0) time = 0;
    
    if (time >= 3600) // 1 hour
    {
        time = ceil(time / 60);
    }
    
    int first = floor(time / 60);
    int second = (int)time % 60;
    
    h1->setString(toString((int)floor(first / 10)).c_str());
    h2->setString(toString((int)floor(first % 10)).c_str());
    
    m1->setString(toString((int)floor(second / 10)).c_str());
    m2->setString(toString((int)floor(second % 10)).c_str());
    
    timeval ctime;
	gettimeofday(&ctime, NULL);

    if (ctime.tv_sec % 2 == 0)
        delim->setVisible(true);
    else
        delim->setVisible(false);
}

void StoreOfferPopup::notNowCallback()
{
    CCScene* scene = GameScene::scene();
    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, scene);
    CCDirector::sharedDirector()->replaceScene(transition);
}

void StoreOfferPopup::storeOfferCallback()
{
    // TODO: handle ingame payement
}
