//
//  StatsScene.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 10/6/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "StatsScene.h"
#include "StaticSceneManager.h"
#include "User.h"
#include "Utils.h"
#include "AppMacros.h"
#include "Levels.h"
#include "ExperienceBar.h"

USING_NS_CC;
using namespace DF::Stats;

CCScene* StatsScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StatsScene *layer = StatsScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

static void addLabelTxt(CCNode* node, std::string text, int y, int points)
{
    CCLabelTTF* lbl = CCLabelTTF::create(text.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.6,
                                         CCSize(470, 75), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    lbl->setAnchorPoint(ccp(0,0));
    lbl->setPosition(ccp(210, y));
    lbl->setColor(ccc3(0x53, 0x99, 0x8C));
    node->addChild(lbl);
    
    std::string p;
    for (int i = 0; i < points; i++)
    {
        p += " .";
    }
    
    CCLabelTTF* pointsL = CCLabelTTF::create(p.c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.6,
                                         CCSize(470, 75), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    pointsL->setAnchorPoint(ccp(0,0));
    pointsL->setPosition(ccp(210, y));
    pointsL->setColor(ccc3(0x53, 0x99, 0x8C));
    node->addChild(pointsL);
}

static CCLabelTTF* rezTxt(int y)
{
    CCLabelTTF* lbl = CCLabelTTF::create("", FONT_SKRANJI, TITLE_FONT_SIZE*0.6,
                                         CCSize(150, 75), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    lbl->setAnchorPoint(ccp(0,0));
    lbl->setPosition(ccp(690, y));
    lbl->setColor(ccc3(0xEE, 0x8E, 0x23));
    return lbl;
}

bool StatsScene::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    // enables back button on android
    this->setKeypadEnabled(true);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/bg_2.png");
    back->setAnchorPoint(ccp(0,0));
    addChild(back, 0);
    
    
    
    
    CCSprite* statsBack = CCSprite::create("Interfeiss/stats/stats_window.png");
    statsBack->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2 - 30));
    addChild(statsBack);
    
    
    CCSprite* bar = CCSprite::create("Interfeiss/level_end/progress_bar_level.png");
    bar->setAnchorPoint(ccp(0, 0.5));
    bar->setPosition(ccp(195, visibleSize.height - 90));// 15 -10  180 h-80
    addChild(bar);
    
    barFill = DF::LevelEnd::ExperienceFill::create();
    barFill->setAnchorPoint(ccp(0, 0));
    barFill->setPosition(ccp(214, visibleSize.height-110));
    //
    
    addChild(barFill);
    
    levelTxt = CCLabelTTF::create(toString(User::getInstance()->getLevel()+1).c_str(), FONT_SKRANJI, TITLE_FONT_SIZE, CCSize(75, 75), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    levelTxt->setPosition(ccp(822, visibleSize.height-130));
    levelTxt->setColor(ccc3(0xFF, 0xA0, 0x01));
    levelTxt->setAnchorPoint(ccp(0,0));
    addChild(levelTxt);
    
    CCSprite* statsTitle = CCSprite::create("Interfeiss/stats/stats_title.png");
    statsTitle->setAnchorPoint(ccp(0, 1));
    statsTitle->setPosition(ccp(60, visibleSize.height - 30));
    addChild(statsTitle);
    
    // --------
    
    CCLabelTTF* tSingleLabel = CCLabelTTF::create("SINGLE GAME", FONT_SKRANJI, TITLE_FONT_SIZE*0.7, CCSize(690, 75), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    tSingleLabel->setAnchorPoint(ccp(0,0));
    tSingleLabel->setPosition(ccp(135, visibleSize.height-195));
    tSingleLabel->setColor(ccc3(0x53, 0x99, 0x8C));
    addChild(tSingleLabel);
    
    addLabelTxt(this, "High score", visibleSize.height-240, 18);
    addLabelTxt(this, "Most Dwarfs rescued", visibleSize.height-280, 9);
    
    highScore = rezTxt(visibleSize.height-240);
    addChild(highScore);
    mostDwarfs = rezTxt(visibleSize.height-280);
    addChild(mostDwarfs);
    
    CCLabelTTF* tTotalLabel = CCLabelTTF::create("LIFE TIME", FONT_SKRANJI, TITLE_FONT_SIZE*0.7, CCSize(690, 75), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    tTotalLabel->setAnchorPoint(ccp(0,0));
    tTotalLabel->setPosition(ccp(135, visibleSize.height-350));
    tTotalLabel->setColor(ccc3(0x53, 0x99, 0x8C));
    addChild(tTotalLabel);
    
    addLabelTxt(this, "Total Games", visibleSize.height-405, 16);
    addLabelTxt(this, "Total Green crystals collected", visibleSize.height-445, 2);
    addLabelTxt(this, "Total Blue crystals collected", visibleSize.height-485, 4);
    addLabelTxt(this, "Total Red crystals collected", visibleSize.height-525, 4);
    addLabelTxt(this, "Total Yellow crystals collected", visibleSize.height-565, 2);
    addLabelTxt(this, "Total Diamonds collected", visibleSize.height-605, 6);
    addLabelTxt(this, "Total Dwarfs rescued", visibleSize.height-645, 9);
    
    totalGames = rezTxt(visibleSize.height-405);
    addChild(totalGames);
    totalGreen = rezTxt(visibleSize.height-445);
    addChild(totalGreen);
    totalBlue = rezTxt(visibleSize.height-485);
    addChild(totalBlue);
    totalRed = rezTxt(visibleSize.height-525);
    addChild(totalRed);
    totalYellow = rezTxt(visibleSize.height-565);
    addChild(totalYellow);
    totalDiamonds = rezTxt(visibleSize.height-605); 
    addChild(totalDiamonds);
    totalDwarfs = rezTxt(visibleSize.height-645);
    addChild(totalDwarfs);
    
    // back
    
	CCMenuItemImage* backButton = CCMenuItemImage::create(
														  "Interfeiss/back_btn0001.png",
														  "Interfeiss/back_btn0002.png",
														  this,
														  menu_selector(StatsScene::menuBack));
	backButton->setAnchorPoint(ccp(0,0));
    
    CCMenu* backMenu = CCMenu::create(backButton, NULL);
    backMenu->setPosition(0, 0);
    this->addChild(backMenu);
    
    updateInfo();
    
    this->scheduleUpdate();
    
    return true;
}

void StatsScene::keyBackClicked()
{
    menuBack();
}

void StatsScene::updateInfo()
{
//    float pos = (float)(User::getInstance()->getExperience() - Levels::getInstance()->currentLevelPoints()) /
//				(Levels::getInstance()->nextLevelPoints() - Levels::getInstance()->currentLevelPoints());
    
    float pos = (float)(User::getInstance()->getDwarfs() - Levels::getInstance()->currentLevelPoints()) /
				(Levels::getInstance()->nextLevelPoints() - Levels::getInstance()->currentLevelPoints());
    
    barFill->xpos = pos;
    
    levelTxt->setString(toString(User::getInstance()->getLevel()+1).c_str());
    
    highScore->setString(toString(User::getInstance()->getStats().getHighscore()).c_str());
    mostDwarfs->setString(toString(User::getInstance()->getStats().getMostDwarfsRescued()).c_str());
    totalGames->setString(toString(User::getInstance()->getStats().getTotalGames()).c_str());
    totalGreen->setString(toString(User::getInstance()->getStats().getGreenCrystals()).c_str());
    totalBlue->setString(toString(User::getInstance()->getStats().getBlueCrystals()).c_str());
    totalRed->setString(toString(User::getInstance()->getStats().getRedCrystals()).c_str());
    totalYellow->setString(toString(User::getInstance()->getStats().getYellowCrystals()).c_str());
    totalDiamonds->setString(toString(User::getInstance()->getStats().getDiamondsCollected()).c_str());
    totalDwarfs->setString(toString(User::getInstance()->getStats().getTotalDwarfsrescued()).c_str());
}

void StatsScene::onEnter()
{
    CCLayer::onEnter();
    
    updateInfo();
}

void StatsScene::menuBack()
{
    CCScene* mainMenu = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::OPTIONS);
	CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, mainMenu);
	CCDirector::sharedDirector()->replaceScene(transition);
}
