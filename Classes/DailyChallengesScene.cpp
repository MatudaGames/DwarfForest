/*
 * DailyChallengesScene.cpp
 *
 *  Created on: 2013. gada 11. aug.
 *      Author: agriic
 */

#include "DailyChallengesScene.h"
#include "AppMacros.h"
#include "StaticSceneManager.h"
#include "User.h"
#include "Utils.h"

#include <iostream>

USING_NS_CC;

CCScene* DailyChallengesScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    DailyChallengesScene *layer = DailyChallengesScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool DailyChallengesScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // enables back button on android
    this->setKeypadEnabled(true);
    
    User::getInstance()->getDailyChallenges().checkDayCounter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
    back->setAnchorPoint(ccp(0,0));
    addChild(back, 0);

    CCMenuItemImage* backButton = CCMenuItemImage::create(
                                                          "Interfeiss/challenges/back_arrow_v20001.png",
                                                          "Interfeiss/challenges/back_arrow_v20002.png",
                                                          this,
                                                          menu_selector(DailyChallengesScene::menuBackCallback));
    backButton->setAnchorPoint(ccp(0,0));

    CCMenu* backMenu = CCMenu::create(backButton, NULL);
    this->addChild(backMenu, 10);
    backMenu->setPosition(-10, -10);

    // title
    CCSprite* title = CCSprite::create("Interfeiss/challenges/challenges_title_2.png");
    title->setAnchorPoint(ccp(0,1));
    title->setPosition(ccp(40, visibleSize.height-20));
    addChild(title, 10);

    CCMenuItemImage* weeklyButton = CCMenuItemImage::create("Interfeiss/challenges/weekly_inactive.png",
            NULL, this,
            menu_selector(DailyChallengesScene::menuWeeklyCallback));

    CCMenu* mainMenu = CCMenu::create(weeklyButton, NULL);

    mainMenu->setAnchorPoint(ccp(0,0));
    mainMenu->alignItemsHorizontallyWithPadding(15);
    mainMenu->setPosition(ccp(visibleSize.width - 110, visibleSize.height - 60));
    this->addChild(mainMenu, 10);

    CCSprite* dailyImage = CCSprite::create("Interfeiss/challenges/daily_active.png");
    dailyImage->setPosition(ccp(visibleSize.width - 300, visibleSize.height - 60));
    this->addChild(dailyImage, 10);

    // footer

    CCSprite* footer = CCSprite::create("Interfeiss/challenges/footer.png");
    footer->setAnchorPoint(ccp(0,0));
    footer->setPosition(ccp(150, 25));
    addChild(footer, 10);

    uint32_t day = 0, reward = 0;
    User::getInstance()->getDailyChallenges().getNextDayReward(
                        User::getInstance()->getDailyChallenges().getDaysCompleted(), day, reward);
    
    std::string txt = "Beat the challenge "
                    + toString(day)
                    + " days in a row to get";
    
    footerText = CCLabelTTF::create(txt.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.55,
                                    CCSize(700, 55), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    footerText->setAnchorPoint(ccp(0,0));
    footerText->setColor(ccc3(0x27, 0x0a, 0x09));
    footerText->setPosition(ccp(190, 32));
    addChild(footerText, 10);

    footerText6 = CCLabelTTF::create(toString(reward).c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.8,
                                     CCSize(50, 55), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    footerText6->setAnchorPoint(ccp(0,0));
    footerText6->setColor(ccc3(0x27, 0x0a, 0x09));
    footerText6->setPosition(ccp(760, 30));
    addChild(footerText6, 10);

    CCSprite* footerDiamond = CCSprite::create("Interfeiss/challenges/award_diamond.png");
    footerDiamond->setAnchorPoint(ccp(0,0));
    footerDiamond->setPosition(ccp(780, 25));
    addChild(footerDiamond, 10);

    // Days counter

    CCSprite* daysBack = CCSprite::create("Interfeiss/challenges/day_counter.png");
    daysBack->setAnchorPoint(ccp(0,0));
    daysBack->setPosition(ccp(200, 120));
    addChild(daysBack, 10);

    uint32_t chDay = User::getInstance()->getDailyChallenges().getDaysCompleted();
    if (!Time::isToday(User::getInstance()->getDailyChallenges().getLastCompletionTime()))
    {
        chDay++;
    }
    
    daysN1 = CCLabelTTF::create(toString(chDay / 10).c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
                                CCSize(50, 55), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    daysN1->setAnchorPoint(ccp(0,0));
    daysN1->setColor(ccc3(0x0, 0x0, 0x0));
    daysN1->setPosition(ccp(293, 125));
    addChild(daysN1, 10);

    daysN2 = CCLabelTTF::create(toString(chDay % 10).c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.7,
                                CCSize(50, 55), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    daysN2->setAnchorPoint(ccp(0,0));
    daysN2->setColor(ccc3(0x0, 0x0, 0x0));
    daysN2->setPosition(ccp(334, 125));
    addChild(daysN2, 10);

    // Bubbles

    spinBubble = CCSprite::create("Interfeiss/challenges/speech_bubble_spin.png");
    spinBubble->setAnchorPoint(ccp(0,0));
    spinBubble->setPosition(ccp(10, 345));
    spinBubble->setVisible(false);
    addChild(spinBubble, 2);

    collectBubble = CCSprite::create("Interfeiss/challenges/speech_bubble_collect.png");
    collectBubble->setAnchorPoint(ccp(0,0));
    collectBubble->setPosition(ccp(10, 345));
    collectBubble->setVisible(false);
    addChild(collectBubble, 2);
    
    comebackBubble = CCSprite::create("Interfeiss/challenges/speach_bubble_comeback.png");
    comebackBubble->setAnchorPoint(ccp(0,0));
    comebackBubble->setPosition(ccp(10, 345));
    comebackBubble->setVisible(false);
    addChild(comebackBubble, 2);
    
    oneSpinBubble = CCSprite::create("Interfeiss/challenges/speach_bubble_spin_once.png");
    oneSpinBubble->setAnchorPoint(ccp(0,0));
    oneSpinBubble->setPosition(ccp(10, 345));
    oneSpinBubble->setVisible(false);
    addChild(oneSpinBubble, 2);

    if (User::getInstance()->getDailyChallenges().isTodaysCompleted())
    {
        comebackBubble->setVisible(true);
    }
    else if (Time::isToday(User::getInstance()->getDailyChallenges().getLastSpinTime()))
    {
        collectBubble->setVisible(true);
    }
    else
    {
        spinBubble->setVisible(true);
    }
    
    // slot machine

    CCSprite* boxBack = CCSprite::create("Interfeiss/challenges/diamond_box.png");
    boxBack->setPosition(ccp(650, 185));
    addChild(boxBack, 3);
    
    diamondBox = CCSprite::create();
    diamondBox->setAnchorPoint(ccp(0,0));
    addChild(diamondBox, 4);
    
    CCSprite* machine = CCSprite::create("Interfeiss/challenges/slotmachine_base.png");
    machine->setPosition(ccp(534, 400));
    addChild(machine, 5);

    /*
    bar1 = SpinBar::create();
    bar1->setAnchorPoint(ccp(0,0));
    bar1->setPosition(ccp(512,340));
    bar1->setSpinPosition(User::getInstance()->getDailyChallenges().getBar1Color(), User::getInstance()->getDailyChallenges().getBar1Count());
    addChild(bar1, 4);

    bar2 = SpinBar::create();
    bar2->setAnchorPoint(ccp(0,0));
    bar2->setPosition(ccp(609,340));
    bar2->setSpinPosition(User::getInstance()->getDailyChallenges().getBar2Color(), User::getInstance()->getDailyChallenges().getBar2Count());
    addChild(bar2, 4);

    bar3 = SpinBar::create();
    bar3->setAnchorPoint(ccp(0,0));
    bar3->setPosition(ccp(707,340));
    bar3->setSpinPosition(User::getInstance()->getDailyChallenges().getBar3Color(), User::getInstance()->getDailyChallenges().getBar3Count());
    addChild(bar3, 4);
    */

    
    /*
    HandleBar* handle = HandleBar::create();
    handle->setPosition(ccp(850, 280));
    addChild(handle, 4);
    */
    
    update();
    
    this->scheduleUpdate();
    
    return true;
}

void DailyChallengesScene::keyBackClicked()
{
    menuBackCallback(NULL);
}

void DailyChallengesScene::update(float delta)
{
    CCLayer::update(delta);
    
    /*
    bar1->setPosition(ccp(512,339));
    bar2->setPosition(ccp(609,339));
    bar3->setPosition(ccp(707,339));

    bar1->setPosition(ccp(512,340));
    bar2->setPosition(ccp(609,340));
    bar3->setPosition(ccp(707,340));
    */
}

void DailyChallengesScene::onEnter()
{
    CCLayer::onEnter();
    update();
}

void DailyChallengesScene::update()
{
    comebackBubble->setVisible(false);
    collectBubble->setVisible(false);
    spinBubble->setVisible(false);
    
    if (User::getInstance()->getDailyChallenges().isTodaysCompleted())
    {
        comebackBubble->setVisible(true);
    }
    else if (Time::isToday(User::getInstance()->getDailyChallenges().getLastSpinTime()))
    {
        collectBubble->setVisible(true);
    }
    else
    {
        spinBubble->setVisible(true);
    }
    
    uint32_t chDay = User::getInstance()->getDailyChallenges().getDaysCompleted();
    if (!Time::isToday(User::getInstance()->getDailyChallenges().getLastCompletionTime()))
    {
        chDay++;
    }
    
    daysN1->setString(toString(chDay / 10).c_str());
    daysN2->setString(toString(chDay % 10).c_str());
    
    uint32_t day = 0, reward = 0;
    User::getInstance()->getDailyChallenges().getNextDayReward(User::getInstance()->getDailyChallenges().getDaysCompleted(), day, reward);
    
    std::string txt = "Beat the challenge "
    + toString(day)
    + " days in a row to get";
    
    footerText->setString(txt.c_str());
    footerText6->setString(toString(reward).c_str());
}

void DailyChallengesScene::SpinTheBars()
{
    collectBubble->setVisible(true);
    spinBubble->setVisible(false);
    
    /*
    bar1->spin(0);
    bar2->spin(1);
    bar3->spin(2);
    */
    
    // can calculate diamonds amount and drop it out
    this->runAction(CCSequence::create(CCDelayTime::create(5.0), CCCallFunc::create(this, callfunc_selector(DailyChallengesScene::dropDiamonds)), NULL));
    
    // lets calculate reward
    uint32_t reward = 0;
    
    std::map<CrystalColor, int> modifiers;
    modifiers[CRYSTAL_COLOR_GREEN] = 1.0;
    modifiers[CRYSTAL_COLOR_BLUE] = 2.0;
    modifiers[CRYSTAL_COLOR_RED] = 3.0;
    modifiers[CRYSTAL_COLOR_YELLOW] = 5.0;
    
    /*
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
    User::getInstance()->getDailyChallenges().setLastSpinTime(time(0));
    */
    
}

void DailyChallengesScene::dropDiamonds()
{
    diamondBox->removeAllChildren();
    
    /*
    for (int i = 0; i < User::getInstance()->getDailyChallenges().getReward(); i++)
    {
        FallingDiamond* fd = FallingDiamond::create();
        fd->setPosition(ccp(630 + (((i+1) / 2) * 40 * (i % 2 == 1 ? -1 : 1)), 240));
        diamondBox->addChild(fd, 10-i);
        
        int r = i * 2;
        CCMoveTo* mt = CCMoveTo::create(0.95-0.01 * r, ccp(fd->getPositionX(), 150 + r));
        fd->runAction(CCEaseBounceOut::create(mt));
    }
    */
}

void DailyChallengesScene::menuBackCallback(cocos2d::CCObject *sender)
{
    CCScene* mainMenu = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
	CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, mainMenu);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void DailyChallengesScene::menuWeeklyCallback(cocos2d::CCObject *sender)
{
	CCScene* scene = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::WEEKLY_CHALLENGES);
	CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, scene);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void DailyChallengesScene::EmptyHandlePull()
{
    oneSpinBubble->setVisible(true);
    this->runAction(CCSequence::create(CCDelayTime::create(3),
                       CCCallFunc::create(this, callfunc_selector(DailyChallengesScene::HideOneSpinBubble)), NULL));
}

void DailyChallengesScene::HideOneSpinBubble()
{
    oneSpinBubble->setVisible(false);
}

/*
SpinBar::SpinBar()
{
    boxShader1 = new CCGLProgram();
    boxShader1->initWithVertexShaderFilename("slot.vsh", "slot.fsh");
    
    boxShader2 = new CCGLProgram();
    boxShader2->initWithVertexShaderFilename("slot.vsh", "slot.fsh");
    
    resultingPosition = -1;
}

SpinBar::~SpinBar()
{
    
}

bool SpinBar::init()
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
    
	CCSprite* mainSlide = CCSprite::create("Interfeiss/challenges/stremele.png");
	mainSlide->setAnchorPoint(ccp(0,1));
    mainSlide->setShaderProgram(boxShader1);
	slide->addChild(mainSlide);
    
    boxShader1->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    boxShader1->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    boxShader1->link();
    boxShader1->updateUniforms();
    
    sh1_minyLoc = glGetUniformLocation(boxShader1->getProgram(), "u_miny");
    sh1_maxyLoc = glGetUniformLocation(boxShader1->getProgram(), "u_maxy");
    
    
	
    CCSprite* secondSlide = CCSprite::create("Interfeiss/challenges/stremele.png");
	secondSlide->setAnchorPoint(ccp(0,1));
	secondSlide->setPositionY(-1192);
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
    this->update(0);
    
	return true;
}

void SpinBar::setSpinPosition(CrystalColor col, uint32_t count)
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

void SpinBar::update(float delta)
{
//    std::cout << "P: " <<(float)slide->getPosition().y << "\n";
    boxShader1->use();
    
    sh1_miny = ((float)slide->getPosition().y - 70) / 1192;
    sh1_maxy = ((float)slide->getPosition().y + 70) / 1192;
    
    glUniform1f(sh1_minyLoc, this->sh1_miny);
    glUniform1f(sh1_maxyLoc, this->sh1_maxy);
    
    boxShader2->use();
    
    sh2_miny = ((float)slide->getPosition().y - 70 - 1192) / 1192;
    sh2_maxy = ((float)slide->getPosition().y + 70 - 1192) / 1192;
    
    glUniform1f(sh2_minyLoc, this->sh2_miny);
    glUniform1f(sh2_maxyLoc, this->sh2_maxy);
    
}

int SpinBar::getYFromPos(unsigned int p)
{
    p = p % 14;
    unsigned int addH = p == 0 ? 1192 : 0;
    return p * 85 + 43 + addH;
}

void SpinBar::setSpinPosition(unsigned int p)
{
	slide->setPositionY(getYFromPos(p));
}

void SpinBar::spin(int order)
{
    
    // randoms - liidz cik griezties
    
    // 5x pilns un 1x liidz vajadziigajam
    resultingPosition = abs(rand()) % 14;
    
    CCSequence* seq = CCSequence::create(
        CCMoveTo::create(0.5, ccp(0, getYFromPos(14))),
        CCCallFunc::create(this, callfunc_selector(SpinBar::resestSlide)),
        NULL);
    CCRepeat* rep = CCRepeat::create(seq, 5 + order * 2);
    
    // jaaieliek move uz 0 poziiciju
    CCSequence* seq2 = CCSequence::create(
              CCMoveTo::create(0.5 - 0.5 * (slide->getPositionY() / getYFromPos(14)), ccp(0, getYFromPos(14))),
              CCCallFunc::create(this, callfunc_selector(SpinBar::resestSlide)),
              rep,
              CCCallFunc::create(this, callfunc_selector(SpinBar::finalizeSpin)),
              NULL);
    
    slide->runAction(seq2);
}

void SpinBar::resestSlide()
{
    setSpinPosition(1);
}

void SpinBar::finalizeSpin()
{
    float time = 0.5 * ((resultingPosition + 13) % 14 / 14);
    CCMoveTo* m = CCMoveTo::create(time, ccp(0, getYFromPos(resultingPosition)));
    
    slide->runAction(m);
}


CrystalColor SpinBar::getCrystalColor()
{
	if (resultingPosition > -1)
    {
        return positions[resultingPosition].type;
    }
    return CRYSTAL_COLOR_COUNT;
}

unsigned int SpinBar::getCrystalCount()
{
	if (resultingPosition > -1)
    {
        return positions[resultingPosition].count;
    }
    return 1;
}

bool HandleBar::init()
{
    setAnchorPoint(ccp(0, 0));
    
    handle = CCSprite::create("Interfeiss/challenges/klokis.png");
    handle->setAnchorPoint(ccp(0,0));
    addChild(handle, 1);
    
    moveStarted = false;
    
    return true;
}

void HandleBar::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void HandleBar::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool HandleBar::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (moveStarted == false
        && touch->getLocation().x > 850
        && touch->getLocation().x < 960
        && touch->getLocation().y > 450
        && touch->getLocation().y < 550)
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

void HandleBar::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    moveStarted = false;
}

void HandleBar::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (moveStarted && touch->getLocation().y < 370)
    {
        moveStarted = false;
        
        // noraujam klokji
        
        handle->setPositionY(115);
        handle->setScaleY(-1);
        
        shadow = CCSprite::create("Interfeiss/challenges/novilciens_fx.png");
        shadow->setAnchorPoint(ccp(0, 0.5));
        shadow->setPosition(ccp(25, 65));
        addChild(shadow, 0);
        //shadow->
        
        CCFadeOut* fade = CCFadeOut::create(0.3);
        CCCallFunc* call = CCCallFunc::create(this, callfunc_selector(HandleBar::onAnimationEnded));
        CCSequence* seq = CCSequence::create(fade, call, NULL);
        
        shadow->runAction(seq);
        
        // TODO: add sounds
        if (Time::isToday(User::getInstance()->getDailyChallenges().getLastSpinTime()) == false)
        {
            ((DailyChallengesScene*)this->getParent())->SpinTheBars();
        }
        else
        {
            // empty sound and show bubble
            ((DailyChallengesScene*)this->getParent())->EmptyHandlePull();
        }
        
    }
}

void HandleBar::onAnimationEnded()
{
    removeChild(shadow);
    shadow = NULL;
    
    handle->setPositionY(0);
    handle->setScaleY(1);
}


bool FallingDiamond::init()
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
    this->setScaleX(0.7 + 0.01 * (rand() % 30));
    this->setScaleY(getScaleX());
    
    diamond->setAnchorPoint(ccp(0,0));
    
    addChild(diamond);
    
    return true;
}
 */
