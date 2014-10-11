//
//  DailyChallengesPopUp.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 9/17/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "DailyChallengesPopUp.h"
#include "GameScene.h"
#include "Utils.h"
#include "User.h"
#include "AppMacros.h"
#include <algorithm>

USING_NS_CC;

CCScene* DailyChallengesPopUp::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    DailyChallengesPopUp *layer = DailyChallengesPopUp::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool DailyChallengesPopUp::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    User::getInstance()->getDailyChallenges().checkDayCounter();
    
    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
    back->setAnchorPoint(ccp(0,0));
//    addChild(back, 0);
    
    
    CCSprite* ripa = CCSprite::create("Interfeiss/challenges/pop_up/bg_ripaa.png");
    ripa->setPosition(ccp(480, 390));
    addChild(ripa);
    
    
    CCSprite* footer = CCSprite::create("Interfeiss/challenges/pop_up/pop_daily.png");
    footer->setPosition(ccp(480, 60));
    addChild(footer);
    
    CCSprite* title = CCSprite::create("Interfeiss/challenges/pop_up/daily_popup_title.png");
    title->setPosition(ccp(480, 620));
    addChild(title);
 
    // slot machine
    
    CCSprite* boxBack = CCSprite::create("Interfeiss/challenges/pop_up/diamond_box.png");
    boxBack->setPosition(ccp(590, 300));
    addChild(boxBack, 3);
    
    diamondBox = CCSprite::create();
    diamondBox->setAnchorPoint(ccp(0,0));
    addChild(diamondBox, 4);
    
    CCSprite* machine = CCSprite::create("Interfeiss/challenges/pop_up/slotmachine_base.png");
    machine->setPosition(ccp(504, 470));
    addChild(machine, 5);

    
    
    bar1 = SmallSpinBar::create();
    bar1->setAnchorPoint(ccp(0,0));
    bar1->setPosition(ccp(487,428));
    bar1->setSpinPosition(User::getInstance()->getDailyChallenges().getBar1Color(), User::getInstance()->getDailyChallenges().getBar1Count());
    addChild(bar1, 4);
    
    bar2 = SmallSpinBar::create();
    bar2->setAnchorPoint(ccp(0,0));
    bar2->setPosition(ccp(559,428));
    bar2->setSpinPosition(User::getInstance()->getDailyChallenges().getBar2Color(), User::getInstance()->getDailyChallenges().getBar2Count());
    addChild(bar2, 4);
    
    bar3 = SmallSpinBar::create();
    bar3->setAnchorPoint(ccp(0,0));
    bar3->setPosition(ccp(631,428));
    bar3->setSpinPosition(User::getInstance()->getDailyChallenges().getBar3Color(), User::getInstance()->getDailyChallenges().getBar3Count());
    addChild(bar3, 4);

    

    SmallHandleBar* handle = SmallHandleBar::create();
    handle->setPosition(ccp(732, 370));
    addChild(handle, 4);

    CCMenuItemImage* spinItem = CCMenuItemImage::create(
                                                             "Interfeiss/challenges/pop_up/spin_btn0001.png",
                                                             "Interfeiss/challenges/pop_up/spin_btn0002.png",
                                                             this,
                                                             menu_selector(DailyChallengesPopUp::Spin));
    
    CCMenu* spinBt = CCMenu::create(spinItem, NULL);
    spinBt->setPosition(ccp(475, 225));
    spinBt->setRotation(-5);
    addChild(spinBt);
    
    
    
//    spinBt = CCSprite::create("Interfeiss/challenges/pop_up/spin_btn0001.png");
//    spinBt->setPosition(ccp(505, 185));
//    spinBt->setRotation(-5);
//    addChild(spinBt);
    
    return true;
}

void DailyChallengesPopUp::Spin()
{
    if (Time::isToday(User::getInstance()->getDailyChallenges().getLastSpinTime()) == false)
    {
        SpinTheBars();
    }
}

void DailyChallengesPopUp::SpinTheBars()
{
    User::getInstance()->getDailyChallenges().setLastSpinTime(time(0));
    
    bar1->spin(0);
    bar2->spin(1);
    bar3->spin(2);
    
    spinAnim = SpriteAnimation::create("Interfeiss/challenges/pop_up/spinning_animation.plist", true);
    spinAnim->setAnchorPoint(ccp(0.5, 0.5));
    spinAnim->setPosition(ccp(505, 185));
    spinAnim->setRotation(-5);
    addChild(spinAnim);
    removeChild(spinBt);
    
    // can calculate diamonds amount and drop it out
    this->runAction(CCSequence::create(CCDelayTime::create(5.0), CCCallFunc::create(this, callfunc_selector(DailyChallengesPopUp::dropDiamonds)), NULL));
    
    // lets calculate reward
    uint32_t reward = 0;
    
    std::map<CrystalColor, int> modifiers;
    modifiers[CRYSTAL_COLOR_GREEN] = 1.0;
    modifiers[CRYSTAL_COLOR_BLUE] = 2.0;
    modifiers[CRYSTAL_COLOR_RED] = 3.0;
    modifiers[CRYSTAL_COLOR_YELLOW] = 5.0;
    
    uint32_t points = modifiers[bar1->getCrystalColor()] * bar1->getCrystalCount()
    + modifiers[bar2->getCrystalColor()] * bar2->getCrystalCount()
    + modifiers[bar3->getCrystalColor()] * bar3->getCrystalCount();
    
    if (points <= 5)
        reward = 1;
    else if (points <= 10)
        reward = 2;
    else if (points <= 15)
        reward = 3;
    else if (points <= 20)
        reward = 4;
    else if (points <= 25)
        reward = 5;
    else
        reward = 6;
    
    User::getInstance()->getDailyChallenges().setBar1Color(bar1->getCrystalColor());
    User::getInstance()->getDailyChallenges().setBar2Color(bar2->getCrystalColor());
    User::getInstance()->getDailyChallenges().setBar3Color(bar3->getCrystalColor());
    
    User::getInstance()->getDailyChallenges().setBar1Count(bar1->getCrystalCount());
    User::getInstance()->getDailyChallenges().setBar2Count(bar2->getCrystalCount());
    User::getInstance()->getDailyChallenges().setBar3Count(bar3->getCrystalCount());
    
    User::getInstance()->getDailyChallenges().setReward(reward);
}

void DailyChallengesPopUp::dropDiamonds()
{
    diamondBox->removeAllChildren();
    
    for (int i = 0; i < User::getInstance()->getDailyChallenges().getReward(); i++)
    {
        SmallFallingDiamond* fd = SmallFallingDiamond::create();
        fd->setPosition(ccp(565 + (((i+1) / 2) * 30 * (i % 2 == 1 ? -1 : 1)), 350));
        diamondBox->addChild(fd, 10-i);
        
        int r = i * 2;
        CCMoveTo* mt = CCMoveTo::create(0.95-0.01 * r, ccp(fd->getPositionX(), 280 + r));
        fd->runAction(CCEaseBounceOut::create(mt));
    }
    
//    spinAnim = SpriteAnimation::create("Interfeiss/challenges/pop_up/spinning_animation.plist", true);
//    spinAnim->setAnchorPoint(ccp(0.5, 0.5));
//    spinAnim->setPosition(ccp(505, 185));
//    spinAnim->setRotation(-5);
//    addChild(spinAnim);
//    removeChild(spinBt);
    
    this->runAction(CCSequence::create(CCDelayTime::create(0.1),
                                       CCCallFunc::create(this, callfunc_selector(DailyChallengesPopUp::ShowPlay)),
                                       NULL));
}

void DailyChallengesPopUp::ShowPlay()
{
    if (spinAnim != NULL)
    {
        CCMenuItemImage* moreGamesItem = CCMenuItemImage::create(
                                                                 "Interfeiss/challenges/pop_up/play_btn0001.png",
                                                                 "Interfeiss/challenges/pop_up/play_btn0002.png",
                                                                 this,
                                                                 menu_selector(DailyChallengesPopUp::Play));
        
        CCMenu* menu = CCMenu::create(moreGamesItem, NULL);
        menu->setPosition(ccp(475, 225));
        menu->setRotation(-5);
        addChild(menu);
        
        removeChild(spinAnim);
        spinAnim = NULL;
    }
}

void DailyChallengesPopUp::Play()
{
//    CCScene* scene = GameScene::scene();
//    CCDirector::sharedDirector()->replaceScene(scene);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(0,visibleSize.height+200));
    CCEaseElasticIn* aEase1 = CCEaseElasticIn::create(aMove1,1.0f);
    CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(DailyChallengesPopUp::RealRemove));
    CCSequence* aSeq = CCSequence::create(aEase1,func2,NULL);
    runAction(aSeq);
}

void DailyChallengesPopUp::RealRemove()
{
    GameScene* mainMenu = (GameScene*)this->getParent();
    mainMenu->OnResumeFromDaily();
    
    this->getParent()->removeChild(this);
}

void DailyChallengesPopUp::EmptyHandlePull()
{
    
}

// -----------------------------

SmallSpinBar::SmallSpinBar()
{
    boxShader1 = new CCGLProgram();
    boxShader1->initWithVertexShaderFilename("slot.vsh", "slot.fsh");
    
    boxShader2 = new CCGLProgram();
    boxShader2->initWithVertexShaderFilename("slot.vsh", "slot.fsh");
    
    resultingPosition = -1;
}

bool SmallSpinBar::init()
{
	positions[0] = SpinPosition(CRYSTAL_COLOR_GREEN, 5);
	positions[1] = SpinPosition(CRYSTAL_COLOR_BLUE, 3);
	positions[2] = SpinPosition(CRYSTAL_COLOR_GREEN, 2);
	positions[3] = SpinPosition(CRYSTAL_COLOR_RED, 3);
	positions[4] = SpinPosition(CRYSTAL_COLOR_GREEN, 1);
	positions[5] = SpinPosition(CRYSTAL_COLOR_YELLOW, 2);
	positions[6] = SpinPosition(CRYSTAL_COLOR_GREEN, 3);
	positions[7] = SpinPosition(CRYSTAL_COLOR_BLUE, 1);
	positions[8] = SpinPosition(CRYSTAL_COLOR_RED, 1);
	positions[9] = SpinPosition(CRYSTAL_COLOR_YELLOW, 1);
	positions[10] = SpinPosition(CRYSTAL_COLOR_BLUE, 2);
	positions[11] = SpinPosition(CRYSTAL_COLOR_RED, 2);
	positions[12] = SpinPosition(CRYSTAL_COLOR_GREEN, 4);
	positions[13] = SpinPosition(CRYSTAL_COLOR_BLUE, 4);
    
    slide = CCLayer::create();
    
	CCSprite* mainSlide = CCSprite::create("Interfeiss/challenges/pop_up/striipa_x_14_small.png");
	mainSlide->setAnchorPoint(ccp(0,1));
    mainSlide->setShaderProgram(boxShader1);
	slide->addChild(mainSlide);
    
    boxShader1->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    boxShader1->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    boxShader1->link();
    boxShader1->updateUniforms();
    
    sh1_minyLoc = glGetUniformLocation(boxShader1->getProgram(), "u_miny");
    sh1_maxyLoc = glGetUniformLocation(boxShader1->getProgram(), "u_maxy");
    
    
	
    CCSprite* secondSlide = CCSprite::create("Interfeiss/challenges/pop_up/striipa_x_14_small.png");
	secondSlide->setAnchorPoint(ccp(0,1));
	secondSlide->setPositionY(-872);
    secondSlide->setShaderProgram(boxShader2);
    slide->addChild(secondSlide);
    
    boxShader2->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    boxShader2->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    boxShader2->link();
    boxShader2->updateUniforms();
    
    sh2_minyLoc = glGetUniformLocation(boxShader2->getProgram(), "u_miny");
    sh2_maxyLoc = glGetUniformLocation(boxShader2->getProgram(), "u_maxy");
	
    
    addChild(slide);
    
    setSpinPosition(rand() % 14);
    
    this->scheduleUpdate();
    
	return true;
}

void SmallSpinBar::setSpinPosition(CrystalColor col, uint32_t count)
{
    // find position, if it is not possible - set random
    uint32_t pos = rand() % 14;
    
    for (int i = 0; i < sizeof(positions) / sizeof(positions[0]); i++)
    {
        if (col == positions[i].type && count == positions[i].count)
        {
            pos = i;
            break;
        }
    }
    
    setSpinPosition(pos);
}

void SmallSpinBar::update(float delta)
{
    //    std::cout << "P: " <<(float)slide->getPosition().y << "\n";
    boxShader1->use();
    
    sh1_miny = ((float)slide->getPosition().y - 49) / 872;
    sh1_maxy = ((float)slide->getPosition().y + 49) / 872;
    
    glUniform1f(sh1_minyLoc, this->sh1_miny);
    glUniform1f(sh1_maxyLoc, this->sh1_maxy);
    
    boxShader2->use();
    
    sh2_miny = ((float)slide->getPosition().y - 49 - 872) / 872;
    sh2_maxy = ((float)slide->getPosition().y + 49 - 872) / 872;
    
    glUniform1f(sh2_minyLoc, this->sh2_miny);
    glUniform1f(sh2_maxyLoc, this->sh2_maxy);
    
}

int SmallSpinBar::getYFromPos(unsigned int p)
{
    p = p % 14;
    unsigned int addH = p == 0 ? 872 : 0;
    return p * 62 + 31 + addH;
}

void SmallSpinBar::setSpinPosition(unsigned int p)
{
	slide->setPositionY(getYFromPos(p));
}

void SmallSpinBar::spin(int order)
{
    
    // randoms - liidz cik griezties
    
    // 5x pilns un 1x liidz vajadziigajam
    resultingPosition = abs(rand()) % 14;
    
    CCSequence* seq = CCSequence::create(
                                         CCMoveTo::create(0.5, ccp(0, getYFromPos(14))),
                                         CCCallFunc::create(this, callfunc_selector(SmallSpinBar::resestSlide)),
                                         NULL);
    CCRepeat* rep = CCRepeat::create(seq, 5 + order * 2);
    
    // jaaieliek move uz 0 poziiciju
    CCSequence* seq2 = CCSequence::create(
                                          CCMoveTo::create(0.5 - 0.5 * (slide->getPositionY() / getYFromPos(14)), ccp(0, getYFromPos(14))),
                                          CCCallFunc::create(this, callfunc_selector(SmallSpinBar::resestSlide)),
                                          rep,
                                          CCCallFunc::create(this, callfunc_selector(SmallSpinBar::finalizeSpin)),
                                          NULL);
    
    slide->runAction(seq2);
}

void SmallSpinBar::resestSlide()
{
    setSpinPosition(1);
}

void SmallSpinBar::finalizeSpin()
{
    float time = 0.5 * ((resultingPosition + 13) % 14 / 14);
    CCMoveTo* m = CCMoveTo::create(time, ccp(0, getYFromPos(resultingPosition)));
    
    slide->runAction(m);
}

CrystalColor SmallSpinBar::getCrystalColor()
{
	if (resultingPosition > -1)
    {
        return positions[resultingPosition].type;
    }
    return CRYSTAL_COLOR_COUNT;
}

unsigned int SmallSpinBar::getCrystalCount()
{
	if (resultingPosition > -1)
    {
        return positions[resultingPosition].count;
    }
    return 1;
}

// --------------------------

bool SmallHandleBar::init()
{
    setAnchorPoint(ccp(0, 0));
    
    handle = CCSprite::create("Interfeiss/challenges/pop_up/klokis_small.png");
    handle->setAnchorPoint(ccp(0,0));
    addChild(handle, 1);
    
    moveStarted = false;
    
    return true;
}

void SmallHandleBar::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void SmallHandleBar::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool SmallHandleBar::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (moveStarted == false
        && touch->getLocation().x > 750
        && touch->getLocation().x < 850
        && touch->getLocation().y > 500
        && touch->getLocation().y < 600)
    {
        moveStarted = true;
        dragStart = touch->getLocation();
        
        return true;
    }
    else
    {
        return false;
    }
}

void SmallHandleBar::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    moveStarted = false;
}

void SmallHandleBar::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (moveStarted && touch->getLocation().y < 370)
    {
        moveStarted = false;
        
        // noraujam klokji
        
        handle->setPositionY(95);
        handle->setScaleY(-1);
        
        shadow = CCSprite::create("Interfeiss/challenges/pop_up/novilciens_fx_small.png");
        shadow->setAnchorPoint(ccp(0, 0.5));
        shadow->setPosition(ccp(25, 40));
        addChild(shadow, 0);
        //shadow->
        
        CCFadeOut* fade = CCFadeOut::create(0.3);
        CCCallFunc* call = CCCallFunc::create(this, callfunc_selector(SmallHandleBar::onAnimationEnded));
        CCSequence* seq = CCSequence::create(fade, call, NULL);
        
        shadow->runAction(seq);
        
        if (Time::isToday(User::getInstance()->getDailyChallenges().getLastSpinTime()) == false)
        {
            ((DailyChallengesPopUp*)this->getParent())->SpinTheBars();
        }
        else
        {
            // empty sound and show bubble
            ((DailyChallengesPopUp*)this->getParent())->EmptyHandlePull();
        }
        
    }
}

void SmallHandleBar::onAnimationEnded()
{
    removeChild(shadow);
    shadow = NULL;
    
    handle->setPositionY(0);
    handle->setScaleY(1);
}

bool SmallFallingDiamond::init()
{
    this->setAnchorPoint(ccp(0,0));
    
    if (rand() % 2 == 0)
    {
        diamond = CCSprite::create("Interfeiss/challenges/diamond.png");
    }
    else
    {
        diamond = CCSprite::create("Interfeiss/challenges/diamond_2.png");
    }
    this->setScaleX((0.7 + 0.01 * (rand() % 30))*0.75);
    this->setScaleY(getScaleX());
    
    diamond->setAnchorPoint(ccp(0,0));
    
    addChild(diamond);
    
    return true;
}


/* --------------------------------------------------------------------------------------------------
|
|                                               IN GAME POP UP
|
\ -------------------------------------------------------------------------------------------------- */


DailyChallengesInGamePopUp* DailyChallengesInGamePopUp::create(uint32_t reward)
{
    DailyChallengesInGamePopUp *layer = new DailyChallengesInGamePopUp();
    
    if (layer && layer->init(reward))
    {
        // Set to autorelease
        layer->autorelease();
        
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool DailyChallengesInGamePopUp::init(uint32_t reward)
{
    this->reward = reward;
    
    // add
    back = CCSprite::create("Interfeiss/challenges/ingame_pop_up/daily.png");
    back->setAnchorPoint(ccp(0.5, 1));
    
    addChild(back);
    
    return true;
}

void DailyChallengesInGamePopUp::onEnter()
{
    CCLayer::onEnter();
    
    back->runAction(CCSequence::create(CCMoveTo::create(0.5, ccp(0, 80)),
                                       CCCallFunc::create(this, callfunc_selector(DailyChallengesInGamePopUp::showCheck)),
                                       NULL));
    
}

void DailyChallengesInGamePopUp::showCheck()
{
    check = SpriteAnimation::create("Interfeiss/challenges/ingame_pop_up/check.plist", false);
    check->setAnchorPoint(ccp(0, 1));
    check->setPosition(ccp(95, 95));
    addChild(check);
    
    this->runAction(CCSequence::create(CCDelayTime::create(1.3),
                                       CCCallFunc::create(this, callfunc_selector(DailyChallengesInGamePopUp::showSpin)),
                                       NULL));
}

void DailyChallengesInGamePopUp::showSpin()
{
    spin = SpriteAnimation::create("Interfeiss/challenges/ingame_pop_up/daily_spinnng.plist", true);
    spin->setAnchorPoint(ccp(0.5, 1));
    spin->setPositionY(90);
    addChild(spin);
    
    removeChild(back);
    removeChild(check);
    
    this->runAction(CCSequence::create(CCDelayTime::create(0.5),
                                       CCCallFunc::create(this, callfunc_selector(DailyChallengesInGamePopUp::showReward)),
                                       NULL));
}

void DailyChallengesInGamePopUp::showReward()
{
    diamonds = CCLayer::create();
    
    CCSprite* sp = CCSprite::create("Interfeiss/challenges/ingame_pop_up/diamonds_plus.png");
    sp->setAnchorPoint(ccp(0.5, 1));
    diamonds->addChild(sp);
    
    std::string txt = "+" + toString(reward) + " diamonds!";
    
    CCLabelTTF* footerTextSh = CCLabelTTF::create(txt.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.65,
                                                CCSize(260, 55), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    footerTextSh->setAnchorPoint(ccp(0.5,1));
    footerTextSh->setColor(ccc3(0x0, 0, 0));
    footerTextSh->enableShadow(CCSize(1,1), 0.5, 3, true);
    footerTextSh->setPosition(ccp(-5, -3));
    diamonds->addChild(footerTextSh);
    
    CCLabelTTF* footerText = CCLabelTTF::create(txt.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.65,
                                                CCSize(260, 55), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    footerText->setAnchorPoint(ccp(0.5,1));
    footerText->setColor(ccc3(0xff, 0xff, 0xff));
    footerText->setPosition(ccp(-5, -3));
    diamonds->addChild(footerText);
    
    diamonds->setPositionY(80);
    addChild(diamonds);
    
    removeChild(spin);
    
    diamonds->runAction(CCSequence::create(CCDelayTime::create(1.0),
                                       CCMoveTo::create(0.5, ccp(0,0)),
                                       CCCallFunc::create(this, callfunc_selector(DailyChallengesInGamePopUp::remove)),
                                       NULL));
}

void DailyChallengesInGamePopUp::remove()
{
    if (this->getParent() != NULL)
    {
        this->getParent()->removeChild(this);
    }
}

void DailyChallengesInGamePopUp::onExit()
{
    CCLayer::onExit();

    this->stopAllActions();
}

DailyChallengesInGameInfo* DailyChallengesInGameInfo::create(const DF::Stats::GameStats& stats)
{
    DailyChallengesInGameInfo *layer = new DailyChallengesInGameInfo();
    
    if (layer && layer->init(stats))
    {
        // Set to autorelease
        layer->autorelease();
        
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

static CCSprite* getBarIcon(CrystalColor color)
{
    std::string path;
    switch (color)
    {
        case CRYSTAL_COLOR_BLUE:
            path = "Interfeiss/challenges/ingame_pop_up/blue_count.png";
            break;
        case CRYSTAL_COLOR_GREEN:
            path = "Interfeiss/challenges/ingame_pop_up/green_count.png";
            break;
        case CRYSTAL_COLOR_RED:
            path = "Interfeiss/challenges/ingame_pop_up/red_count.png";
            break;
        case CRYSTAL_COLOR_YELLOW:
            path = "Interfeiss/challenges/ingame_pop_up/yellow_count.png";
            break;
        default:
            break;
    }
    
    return CCSprite::create(path.c_str());
}

bool DailyChallengesInGameInfo::init(const DF::Stats::GameStats &stats)
{
    if (!CCLayer::init()) return false;
    
    setAnchorPoint(ccp(0.5, 0));
    
    popup = CCLayer::create();
    popup->setAnchorPoint(ccp(0.5, 0));
    popup->setPosition(ccp(480, -100));
    
    CCSprite* back = CCSprite::create("Interfeiss/challenges/ingame_pop_up/daily_counter_base.png");
    back->setAnchorPoint(ccp(0.5, 0));
    popup->addChild(back);
    
    // should add icons
    DailyChallenges daily = User::getInstance()->getDailyChallenges();
    
    CCSprite* icon1 = getBarIcon(daily.getBar1Color());
    CCSprite* icon2 = getBarIcon(daily.getBar2Color());
    CCSprite* icon3 = getBarIcon(daily.getBar3Color());
    
    icon1->setAnchorPoint(ccp(0.5, 0));
    icon2->setAnchorPoint(ccp(0.5, 0));
    icon3->setAnchorPoint(ccp(0.5, 0));
    
    icon1->setPosition(ccp(-120, 5));
    icon2->setPosition(ccp(-20, 5));
    icon3->setPosition(ccp(80, 5));
    
    popup->addChild(icon1);
    popup->addChild(icon2);
    popup->addChild(icon3);
    
    
    b1 = CCLabelTTF::create("2/5", FONT_SKRANJI, TITLE_FONT_SIZE*0.45, CCSize(65, 50), kCCTextAlignmentCenter);
    b2 = CCLabelTTF::create("2/5", FONT_SKRANJI, TITLE_FONT_SIZE*0.45, CCSize(65, 50), kCCTextAlignmentCenter);
    b3 = CCLabelTTF::create("2/5", FONT_SKRANJI, TITLE_FONT_SIZE*0.45, CCSize(65, 50), kCCTextAlignmentCenter);
    
    b1->setAnchorPoint(ccp(0.5, 0));
    b2->setAnchorPoint(ccp(0.5, 0));
    b3->setAnchorPoint(ccp(0.5, 0));
    
    b1->setColor(ccc3(56, 23, 0));
    b2->setColor(ccc3(56, 23, 0));
    b3->setColor(ccc3(56, 23, 0));
    
    b1->setPosition(ccp(-78, -14));
    b2->setPosition(ccp(22, -14));
    b3->setPosition(ccp(122, -14));
    
    popup->addChild(b1);
    popup->addChild(b2);
    popup->addChild(b3);
    
    updateNumbers(stats);
    
    waitCount = 0;
    
    addChild(popup);
    
    return true;
}

void DailyChallengesInGameInfo::updateNumbers(const DF::Stats::GameStats &stats)
{
    if (waitCount == 0) waitCount++;
    
    DailyChallenges daily = User::getInstance()->getDailyChallenges();
    
    std::map<CrystalColor, int> map = stats.gatheredCrystals;
    
    int v = 0;
    
    v = (map[daily.getBar1Color()] < daily.getBar1Count() ?
            map[daily.getBar1Color()] : daily.getBar1Count());
    map[daily.getBar1Color()] -= v;
    
    b1->setString((toString(v) + "/" + toString(daily.getBar1Count())).c_str());
    
    v = (map[daily.getBar2Color()] < daily.getBar2Count() ?
         map[daily.getBar2Color()] : daily.getBar2Count());
    map[daily.getBar2Color()] -= v;
    
    b2->setString((toString(v) + "/" + toString(daily.getBar2Count())).c_str());
    
    v = (map[daily.getBar3Color()] < daily.getBar3Count() ?
         map[daily.getBar3Color()] : daily.getBar3Count());
    map[daily.getBar3Color()] -= v;
    
    b3->setString((toString(v) + "/" + toString(daily.getBar3Count())).c_str());
}

bool DailyChallengesInGameInfo::isActive()
{
    return active;
}

void DailyChallengesInGameInfo::onEnter()
{
    CCLayer::onEnter();
    
    active = true;
    
    if(popup != NULL)
    {
        popup->stopAllActions();
        
        popup->runAction(CCSequence::create(CCMoveTo::create(0.5, ccp(480, 10)),
                                            CCCallFunc::create(this, callfunc_selector(DailyChallengesInGameInfo::afterShow)),
                                            NULL));
    }
}

void DailyChallengesInGameInfo::afterShow()
{
    popup->stopAllActions();
    
    this->schedule(schedule_selector(DailyChallengesInGameInfo::remove), 2);
}

void DailyChallengesInGameInfo::remove()
{
    if (waitCount > 0)
    {
        waitCount--;
        this->schedule(schedule_selector(DailyChallengesInGameInfo::remove), 2);
    }
    else
    {
        active = false;
        
        popup->stopAllActions();
        popup->runAction(CCSequence::create(CCMoveTo::create(0.5, ccp(480, -100)),
                                            CCCallFunc::create(this, callfunc_selector(DailyChallengesInGameInfo::destroy)),
                                            NULL));
    }
}

void DailyChallengesInGameInfo::destroy()
{
    if (((GameScene*)this->getParent())->dailyChallengeInfo == this)
        ((GameScene*)this->getParent())->dailyChallengeInfo = NULL;
    this->getParent()->removeChild(this);
}

void DailyChallengesInGameInfo::onExit()
{
    CCLayer::onExit();
    
    this->stopAllActions();
}
