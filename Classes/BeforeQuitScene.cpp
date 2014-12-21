#include "BeforeQuitScene.h"
#include "StaticSceneManager.h"
#include "PauseScene.h"

USING_NS_CC;

CCScene* BeforeQuitScene::scene(bool endingGame)
{
    CCScene *scene = CCScene::create();
    BeforeQuitScene *layer = BeforeQuitScene::create(endingGame);
    scene->addChild(layer);
    return scene;
}

BeforeQuitScene* BeforeQuitScene::create(bool endingGame)
{
    BeforeQuitScene *pGOL = new BeforeQuitScene();
    if (pGOL && pGOL->init(endingGame)) {
        pGOL->autorelease();
        return pGOL;
    }
    CC_SAFE_DELETE(pGOL);
    return NULL;
}

bool BeforeQuitScene::init(bool endingGame)
{
    if (!CCLayer::init()) return false;
    
    _endingGame = endingGame;
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
    back->setAnchorPoint(ccp(0,0));
//    addChild(back, 0);
    
    CCLayerColor* aBG = CCLayerColor::create(ccc4(0,0,0,128),visibleSize.width,visibleSize.height);
    addChild(aBG);
    
    CCSprite* popup = CCSprite::create("Interfeiss/before_quit/dont_leave.png");
    popup->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
    addChild(popup);
    
    
    CCMenuItemImage* okBtn = CCMenuItemImage::create("Interfeiss/before_quit/check_btn0001.png",
                                                     "Interfeiss/before_quit/check_btn0002.png",
                                                     this,
                                                     menu_selector(BeforeQuitScene::leaveCallback));
    CCMenuItemImage* returnBtn = CCMenuItemImage::create("Interfeiss/before_quit/btn_cross.png", NULL,
                                                     this,
                                                     menu_selector(BeforeQuitScene::returnCallback));
    
    CCMenu* menu = CCMenu::create(okBtn, returnBtn, NULL);
    menu->alignItemsHorizontally();
    menu->setPosition(ccp(465, 262));
    returnBtn->setPositionY(returnBtn->getPositionY()+28);
    addChild(menu);
    
    return true;
}

void BeforeQuitScene::leaveCallback()
{
    if (_endingGame)
    {
        CCScene* options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MISSIONS);
//        CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, options);
        CCTransitionScene* transition = CCTransitionFade::create(0.5f,options,ccBLACK);
        CCDirector::sharedDirector()->replaceScene(transition);
    }
    else
    {
        // TODO: quit app - not working on ios simulator
        CCDirector::sharedDirector()->end();
    }
}

void BeforeQuitScene::returnCallback()
{
    if (_endingGame)
    {
        PauseScene* aScene = (PauseScene*)this->getParent()->getParent();
        aScene->OnClousePop();
        
//        this->getParent()->removeChild(this);
    }
    else
    {
        CCScene* options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
        CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, options);
        CCDirector::sharedDirector()->replaceScene(transition);
    }
}
