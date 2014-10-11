//
//  ExperienceBar.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 9/30/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ExperienceBar.h"
#include "SpriteAnimation.h"
#include "AppMacros.h"
#include "Utils.h"
#include "User.h"
#include "Levels.h"
#include "Crystal.h"
#include "LevelUpPopUp.h"
#include "LevelEndScene.h"
#include "SupportPopup.h"

using namespace DF::LevelEnd;
USING_NS_CC;

ExperienceBar* DF::LevelEnd::ExperienceBar::create(Stats::GameStats& stats)
{
    ExperienceBar *pRet = new ExperienceBar();
    if (pRet && pRet->init(stats))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool DF::LevelEnd::ExperienceBar::init(Stats::GameStats& stats)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* back = CCSprite::create("Interfeiss/level_end/forest.png");
    back->setAnchorPoint(ccp(0.5f,0.5f));
    back->setPosition(ccp(visibleSize.width/2,visibleSize.height/2+110));
    addChild(back);
    
    // dwarfs + troll
//    pointsToFill = stats.points;
    pointsToFill = stats.rescuedDwarfs[DWARF_TYPE_FAT]+stats.rescuedDwarfs[DWARF_TYPE_TALL];
    
    trollContainer = CCSprite::create();
    trollContainer->setPosition(ccp(visibleSize.width/2,1000));
    addChild(trollContainer);
    
    troll = SpriteAnimation::create("Interfeiss/level_end/troll_dance.plist");
    troll->setVisible(false);
//    troll->pause();
//    troll->setScale(1);
    trollContainer->addChild(troll);
    
    fat = CCSprite::create("Interfeiss/level_end/fatass_dead.png");
//    fat->setPosition(ccp(270, 140));
    fat->setPosition(ccp(visibleSize.width/2-125, 340));
    addChild(fat);
    
    fatStars = SpriteAnimation::create("Interfeiss/level_end/stars_spin.plist");
//    fatStars->setPosition(ccp(285, 220));
//    fatStars->setPosition(ccp(visibleSize.width/2-85, 400));
//    addChild(fatStars);
    fatStars->setPosition(ccp(120,150));
    fat->addChild(fatStars);
    
    tall = CCSprite::create("Interfeiss/level_end/tall_dead.png");
//    tall->setPosition(ccp(500, 140));
    tall->setPosition(ccp(visibleSize.width/2+155, 360));
    addChild(tall);
    
    tallStars = SpriteAnimation::create("Interfeiss/level_end/stars_spin.plist");
//    tallStars->setPosition(ccp(510, 225));
//    tallStars->setPosition(ccp(visibleSize.width/2+110, 400));
//    addChild(tallStars);
    tallStars->setPosition(ccp(130,164));
    tall->addChild(tallStars);
    
    // exp bar
    
//    CCSprite* bar = CCSprite::create("Interfeiss/level_end/progress_bar_level.png");
//    bar->setAnchorPoint(ccp(0, 0.5));
//    bar->setPosition(ccp(15, -10));
//    bar->cocos2d::CCNode::setVisible(false);
//    addChild(bar);
    
//    barFill = ExperienceFill::create();
//    barFill->setAnchorPoint(ccp(0, 0));
//    barFill->setPosition(ccp(34, -30));
    //
    
//    float pos = (float)(User::getInstance()->getExperience() - Levels::getInstance()->currentLevelPoints())
    
//    float pos = (float)(User::getInstance()->getDwarfs() - Levels::getInstance()->currentLevelPoints()) /
//				(Levels::getInstance()->nextLevelPoints() - Levels::getInstance()->currentLevelPoints());
//    
//    barFill->xpos = pos;
//    barFill->setVisible(false);
//    
//    addChild(barFill);
//    
//    levelTxt = CCLabelTTF::create(toString(User::getInstance()->getLevel()+1).c_str(), FONT_SKRANJI, TITLE_FONT_SIZE, CCSize(75, 75), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//    levelTxt->setPosition(ccp(642, -50));
//    levelTxt->setColor(ccc3(0xFF, 0xA0, 0x01));
//    levelTxt->setAnchorPoint(ccp(0,0));
//    levelTxt->setVisible(false);
//    addChild(levelTxt);
    
    
    // cystal stats
    /*//For now disabled
    CCSprite* cGreen = CCSprite::create("Interfeiss/level_end/green.png");
    cGreen->setAnchorPoint(ccp(0.5, 0));
    cGreen->setPosition(ccp(150, -125));
    addChild(cGreen);
    
    CCLabelTTF* cGreenTxt = CCLabelTTF::create(toString(stats.gatheredCrystals[CRYSTAL_COLOR_GREEN]).c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.9, CCSize(70, 70), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    cGreenTxt->setPosition(ccp(42, -135));
    cGreenTxt->setAnchorPoint(ccp(0,0));
    cGreenTxt->setColor(ccc3(0xB9, 0xFF, 0x00));
    addChild(cGreenTxt);
    
    CCSprite* cBlue = CCSprite::create("Interfeiss/level_end/blue.png");
    cBlue->setAnchorPoint(ccp(0.5, 0));
    cBlue->setPosition(ccp(300, -130));
    addChild(cBlue);
    
    CCLabelTTF* cBlueTxt = CCLabelTTF::create(toString(stats.gatheredCrystals[CRYSTAL_COLOR_BLUE]).c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.9, CCSize(70, 70), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    cBlueTxt->setPosition(ccp(182, -135));
    cBlueTxt->setAnchorPoint(ccp(0,0));
    cBlueTxt->setColor(ccc3(0x01, 0xF7, 0xFF));
    addChild(cBlueTxt);
    
    CCSprite* cRed = CCSprite::create("Interfeiss/level_end/red.png");
    cRed->setAnchorPoint(ccp(0.5, 0));
    cRed->setPosition(ccp(450, -134));
    addChild(cRed);
    
    CCLabelTTF* cRedTxt = CCLabelTTF::create(toString(stats.gatheredCrystals[CRYSTAL_COLOR_RED]).c_str(),
                                             FONT_SKRANJI, TITLE_FONT_SIZE*0.9, CCSize(70, 70), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    cRedTxt->setPosition(ccp(332, -135));
    cRedTxt->setAnchorPoint(ccp(0,0));
    cRedTxt->setColor(ccc3(0xFF, 0x5F, 0x01));
    addChild(cRedTxt);
    
    CCSprite* cYellow = CCSprite::create("Interfeiss/level_end/yellow.png");
    cYellow->setAnchorPoint(ccp(0.5, 0));
    cYellow->setPosition(ccp(600, -135));
    addChild(cYellow);
    
    CCLabelTTF* cYellowTxt = CCLabelTTF::create(toString(stats.gatheredCrystals[CRYSTAL_COLOR_YELLOW]).c_str(), FONT_SKRANJI, TITLE_FONT_SIZE*0.9, CCSize(70, 70), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    cYellowTxt->setPosition(ccp(487, -135));
    cYellowTxt->setAnchorPoint(ccp(0,0));
    cYellowTxt->setColor(ccc3(0xFF, 0xFF, 0x01));
    addChild(cYellowTxt);
    */
    
    return true;
}

void ExperienceBar::dropTroll()
{
    std::cout << "dropTroll\n";
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    trollContainer->runAction(CCSequence::create(CCMoveTo::create(0.2, ccp(visibleSize.width/2, 450)),
                                        CCCallFunc::create(this, callfunc_selector(ExperienceBar::bumpDwarfs)),
                                        NULL));
    troll->setVisible(true);
//    barFill->fill(pointsToFill);
}

void ExperienceBar::bumpDwarfs()
{
//    troll->resume();
    
    fat->runAction(CCSequence::create(CCMoveTo::create(0.1, ccp(fat->getPositionX(), fat->getPositionY()+70)),
                                      CCMoveTo::create(0.1, ccp(fat->getPositionX(), fat->getPositionY())),
                                      NULL));
//    fatStars->runAction(CCSequence::create(CCMoveTo::create(0.1, ccp(fatStars->getPositionX(), 220+70)),
//                                           CCMoveTo::create(0.1, ccp(fatStars->getPositionX(), 220)),
//                                           NULL));
    tall->runAction(CCSequence::create(CCMoveTo::create(0.1, ccp(tall->getPositionX(), tall->getPositionY()+70)),
                                       CCMoveTo::create(0.1, ccp(tall->getPositionX(), tall->getPositionY())),
                                       NULL));
//    tallStars->runAction(CCSequence::create(CCMoveTo::create(0.1, ccp(tallStars->getPositionX(), 225+70)),
//                                            CCMoveTo::create(0.1, ccp(tallStars->getPositionX(), 225)),
//                                            NULL));
    
    //Check if this is 2nd time !!!
//    SupportPopup::
    
}

void ExperienceBar::updateLevel()
{
    levelTxt->setString(toString(User::getInstance()->getLevel() + 1).c_str());
}

// Fill bar -------------------------------------



ExperienceFill::ExperienceFill()
{
    boxShader = new CCGLProgram();
    boxShader->initWithVertexShaderFilename("Interfeiss/level_end/fill.vsh", "Interfeiss/level_end/fill.fsh");
    xpos = 0;
    pointsToFill = 0;
    pps = 40;
    _contrastDirection = 0;
    brght = 0;
    contr = 1;
}

bool ExperienceFill::init()
{
    _paused = false;
    
    CCSprite* mainSlide = CCSprite::create("Interfeiss/level_end/progresbar_fill.png");
	mainSlide->setAnchorPoint(ccp(0,0));
    mainSlide->setShaderProgram(boxShader);
	this->addChild(mainSlide);
    
    boxShader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    boxShader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    boxShader->link();
    boxShader->updateUniforms();
    
    xposLoc = glGetUniformLocation(boxShader->getProgram(), "u_posx");
    brghtLoc = glGetUniformLocation(boxShader->getProgram(), "u_brightness");
    contrLoc = glGetUniformLocation(boxShader->getProgram(), "u_contrast");
    
    
    
    this->scheduleUpdate();
    
    return true;
}

void ExperienceFill::fill(uint32_t points)
{
    pointsToFill = points;
}

void ExperienceFill::continueFill()
{
    _paused = false;
}

void ExperienceFill::update(float delta)
{
    CCLayer::update(delta);
    
    float contrastIncrease = delta * 1.5;
    
    _contrastDirection = -1;
    
    if (pointsToFill > 0 && !_paused)
    {
        _contrastDirection = 1;
        
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        
        uint32_t points = round(delta * pps);
        
        if (pointsToFill < points)
        {
            points = pointsToFill;
        }
        pointsToFill -= points;
        
//        User::getInstance()->addExperience(points);
        User::getInstance()->addDwarfs(points);
        
        while (Levels::getInstance()->isNextLevelReached())
        {
            User::getInstance()->increaseLevel();
            if (this->getParent() != NULL)
            {
                ((ExperienceBar*)this->getParent())->updateLevel();
            }
            
//            LevelUpPopUp* pop = LevelUpPopUp::create(User::getInstance()->getLevel());
//            pop->setAnchorPoint(ccp(0,0));
//            pop->setPosition(ccp(0,-visibleSize.height));
//            pop->runAction(CCMoveTo::create(0.5, ccp(0,0)));
//            this->getParent()->getParent()->addChild(pop, 100);
//            
//            ((LevelEndScene*)this->getParent()->getParent())->fadeOutLevelEnd();
//            
//            _paused = true;
        }
        
        // calculate pos
//        xpos = (float)(User::getInstance()->getExperience() - Levels::getInstance()->currentLevelPoints())
        xpos = (float)(User::getInstance()->getDwarfs() - Levels::getInstance()->currentLevelPoints()) /
				(Levels::getInstance()->nextLevelPoints() - Levels::getInstance()->currentLevelPoints());
    }
    
    if (_contrastDirection == 1 && contr + contrastIncrease > 1.8)
    {
        contrastIncrease = 1.8 - contr;
    }
    else if (_contrastDirection == -1 && contr - contrastIncrease < 1.0)
    {
        contrastIncrease = contr - 1;
    }
    this->contr += contrastIncrease * _contrastDirection;
    brght = (contr-1)*0.5;
    
    boxShader->use();
    glUniform1f(xposLoc, this->xpos);
    glUniform1f(brghtLoc, this->brght);
    glUniform1f(contrLoc, this->contr);
}
