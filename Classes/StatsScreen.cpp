//
//  StatsScreen.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 09/03/14.
//
//

#include "StatsScreen.h"
//#include "TutorialScene.h"
//#include "IntroScene.h"
//#include "StaticSceneManager.h"
#include <SimpleAudioEngine.h>

#include "JniCalls.h"
#include "User.h"
#include "AppMacros.h"
#include "MainMenuScene.h"
#include "OptionsScene.h"

USING_NS_CC;

CCScene* StatsScreen::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StatsScreen *layer = StatsScreen::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void StatsScreen::RemovePopup()
{
    if(mRemoving)
        return;
    mRemoving = true;
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Tap_Back").c_str());
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Popup_Exit").c_str());
    
    CCLayerColor* aLayer = (CCLayerColor*)getChildByTag(888);
    CCFadeTo* aFade = CCFadeTo::create(0.25f,0);
    aLayer->runAction(aFade);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(visibleSize.width/2,visibleSize.height+200));
    CCEaseElasticIn* aEase1 = CCEaseElasticIn::create(aMove1,1.0f);
    CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(StatsScreen::RealRemove));
    CCSequence* aSeq = CCSequence::create(aEase1,func2,NULL);
    aBaseBase->runAction(aSeq);
    //    this->getParent()->removeChild(this);
}

void StatsScreen::RealRemove()
{
    //    CCScene* options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
    //    options->mOptionsOpen = false;
    //    this->getParent()->mOptionsOpen = false;
//    this->getParent()->removeChild(this,true);
    
//    MainMenuScene* mainMenu = (MainMenuScene*)this->getParent();
//    mainMenu->mOptionsOpen = false;
    
    OptionsScene* aScreen = (OptionsScene*)getParent();
    aScreen->OnRemoveStats();
    
}

void StatsScreen::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void StatsScreen::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

// on "init" you need to initialize your instance
bool StatsScreen::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("Popup_Enter").c_str());
    
    mRemoving = false;
    
    // enables back button on android
    /*
    this->setKeypadEnabled(true);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //Maybe fade it in?
    CCLayerColor* aBG = CCLayerColor::create(ccc4(0,0,0,64),visibleSize.width,visibleSize.height);
    aBG->setOpacity(0);
    aBG->setTag(888);
    addChild(aBG,0);
    
    CCFadeTo* aFade1 = CCFadeTo::create(0.25f,64);
    aBG->runAction(aFade1);
    
    aBaseBase = CCSprite::create("small_dot_red.png");
    aBaseBase->setOpacity(0);
    aBaseBase->setPosition(ccp(visibleSize.width/2,visibleSize.height+200));
    addChild(aBaseBase);
    
    //The center bg
    aBase = CCSprite::create("Interfeiss/stats/bigpop.png");
    aBase->setPosition(ccp(aBaseBase->getContentSize().width/2+50,aBaseBase->getContentSize().height/2-35));
    aBase->setTag(999);
    aBaseBase->addChild(aBase,1);
    
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(visibleSize.width/2,visibleSize.height/2-30));
    CCEaseElasticOut* aEase1 = CCEaseElasticOut::create(aMove1,1.0f);
    aBaseBase->runAction(aEase1);
    
    //Add the header !!!
    //Add the clouse button
    CCMenuItemImage* clouseButton = CCMenuItemImage::create(
                                                            "Interfeiss/buttons/x_btn0001.png",
                                                            "Interfeiss/buttons/x_btn0002.png",
                                                            this,
                                                            menu_selector(StatsScreen::RemovePopup));
    CCMenu* aClouse = CCMenu::create(clouseButton,NULL);
    aClouse->setPosition(ccp(aBase->getContentSize().width-20,aBase->getContentSize().height-20));
    aBase->addChild(aClouse,1);

    //the header
    CCSprite* aHeaderBase = CCSprite::create("Interfeiss/challenges/header_title.png");
    aHeaderBase->setPosition(ccp(aBase->getContentSize().width/2-20,aBase->getContentSize().height+33));
    aBase->addChild(aHeaderBase);

    //Add the decor icons
    CCSprite* aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(108,aHeaderBase->getContentSize().height/2));
    aDecor->setColor(ccc3(255,0,0));
    aHeaderBase->addChild(aDecor);

    aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(248,aHeaderBase->getContentSize().height/2));
    aDecor->setFlipX(true);
    aDecor->setColor(ccc3(255,0,0));
    aHeaderBase->addChild(aDecor);

    CCLabelTTF* aStoreHeader = CCLabelTTF::create("Stats", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8, CCSize(100, aHeaderBase->getContentSize().height), kCCTextAlignmentCenter);
    aStoreHeader->setPosition(ccp(178,aHeaderBase->getContentSize().height/2-4));
    aStoreHeader->setColor(ccc3(89, 19, 12));
    aHeaderBase->addChild(aStoreHeader);
    */

    //Lets add all the crap !!!
//    CCSprite* aDummy1 = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
//    aDummy1->setPosition(ccp(0,visibleSize.height/2));
//    aBase->addChild(aDummy1);
//
//    boxShader1 = new CCGLProgram();
//    boxShader1->initWithVertexShaderFilename("slot.vsh", "slot.fsh");
//    
//    boxShader2 = new CCGLProgram();
//    boxShader2->initWithVertexShaderFilename("slot.vsh", "slot.fsh");
//    
//    slide = CCLayer::create();
//
//	CCSprite* mainSlide = CCSprite::create("Interfeiss/challenges/daily/streemele.png");
//	mainSlide->setAnchorPoint(ccp(0,1));
//    
//    CCSprite* aHeader1 = CCSprite::create("Interfeiss/stats/title_single_game.png");
//    aHeader1->setPosition(ccp(0,mainSlide->getContentSize().height-200));
//    mainSlide->addChild(aHeader1);
//
//    mainSlide->setShaderProgram(boxShader1);
//	slide->addChild(mainSlide);
//
//    boxShader1->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
//    boxShader1->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
//    boxShader1->link();
//    boxShader1->updateUniforms();
//    
//    sh1_minyLoc = glGetUniformLocation(boxShader1->getProgram(), "u_miny");
//    sh1_maxyLoc = glGetUniformLocation(boxShader1->getProgram(), "u_maxy");
//    
//    slide->setPosition(ccp(visibleSize.width/2,500));
//    
//    aDummy1->addChild(slide,2);
    
    //lest do it like in shop !!
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCLayerColor* aBG = CCLayerColor::create(ccc4(0,0,0,64),visibleSize.width,visibleSize.height);
    aBG->setOpacity(0);
    aBG->setTag(888);
    addChild(aBG,0);
    
    CCFadeTo* aFade1 = CCFadeTo::create(0.25f,64);
    aBG->runAction(aFade1);
    
    aBaseBase = CCSprite::create("small_dot_red.png");
    aBaseBase->setOpacity(0);
    aBaseBase->setPosition(ccp(visibleSize.width/2,visibleSize.height+200));
    addChild(aBaseBase);
    
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(visibleSize.width/2,visibleSize.height/2));
    CCEaseElasticOut* aEase1 = CCEaseElasticOut::create(aMove1,1.0f);
    aBaseBase->runAction(aEase1);
    
    aBase = CCSprite::create("Interfeiss/stats/bigpop.png");
//    aBase->setPosition(ccp(aBaseBase->getContentSize().width/2+50,aBaseBase->getContentSize().height/2-35));
    aBase->setPosition(ccp(aBaseBase->getContentSize().width/2,aBaseBase->getContentSize().height/2-35));
    aBaseBase->addChild(aBase);
    
    //Add the clouse button
    CCMenuItemImage* clouseButton = CCMenuItemImage::create(
                                                            "Interfeiss/buttons/x_btn0001.png",
                                                            "Interfeiss/buttons/x_btn0002.png",
                                                            this,
                                                            menu_selector(StatsScreen::RemovePopup));
    CCMenu* aClouse = CCMenu::create(clouseButton,NULL);
    aClouse->setPosition(ccp(aBase->getContentSize().width-20,aBase->getContentSize().height-20));
    aBase->addChild(aClouse,1);
    
    //the header
    CCSprite* aHeaderBase = CCSprite::create("Interfeiss/challenges/header_title.png");
    aHeaderBase->setPosition(ccp(aBase->getContentSize().width/2-20,aBase->getContentSize().height+33));
    aBase->addChild(aHeaderBase);
    
    //Add the decor icons
    CCSprite* aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(108,aHeaderBase->getContentSize().height/2));
    aDecor->setColor(ccc3(255,0,0));
    aHeaderBase->addChild(aDecor);
    
    aDecor = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aDecor->setPosition(ccp(248,aHeaderBase->getContentSize().height/2));
    aDecor->setFlipX(true);
    aDecor->setColor(ccc3(255,0,0));
    aHeaderBase->addChild(aDecor);
    
    CCLabelTTF* aStoreHeader = CCLabelTTF::create("Stats", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8, CCSize(100, aHeaderBase->getContentSize().height), kCCTextAlignmentCenter);
    aStoreHeader->setPosition(ccp(178,aHeaderBase->getContentSize().height/2-4));
    aStoreHeader->setColor(ccc3(89, 19, 12));
    aHeaderBase->addChild(aStoreHeader);
    
    
    float aE3 = CCEGLView::sharedOpenGLView()->getScaleX();
    float aE4 = CCEGLView::sharedOpenGLView()->getScaleY();
    mBox = StatsBox::create(CCRect((aBase->getPositionX()+90)*aE3,
                                   (aBase->getPositionY()+50)*aE4,
                                   (aBase->getContentSize().width-80)*aE3,(aBase->getContentSize().height-40)*aE4),
                            CCRect((aBase->getPositionX()-aBase->getContentSize().width/2+20),
                                   (aBase->getPositionY()-aBase->getContentSize().height/2+20),
                                   (aBase->getContentSize().width-80),(aBase->getContentSize().height-40)));
    aBaseBase->addChild(mBox);
    
//    this->scheduleUpdate();
    
    return true;
}