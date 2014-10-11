//
//  LoseScene.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 13.03.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "LoseScene.h"
#include "AppMacros.h"
#include "MainMenuScene.h"
#include "User.h"
#include "Levels.h"
#include "JniCalls.h"
#include "Utils.h"
#include "PopUp.h"

#include <SimpleAudioEngine.h>

USING_NS_CC;

LoseScene::LoseScene()
{
}

LoseScene::~LoseScene()
{
}

LoseScene* LoseScene::create(const std::map<CrystalColor, int>& crystals, bool dailyChallengeCompleted)
{
    LoseScene *scene = new LoseScene();
    
    if (scene && scene->init(crystals, dailyChallengeCompleted))
    {
        // Set to autorelease
        scene->autorelease();

        return scene;
    }
    CC_SAFE_DELETE(scene);
    return NULL;
}

CCScene* LoseScene::scene(unsigned int points, std::map<CrystalColor, int>& crystals, bool dailyChallengeCompleted)
{
//	JniCalls::submitScore(points, User::getInstance()->getLevel() + 1);
    JniCalls::submitScroeToLeaderboard("leaderboard_easy", points);

	User::getInstance()->addExperience(points);

	if (Levels::getInstance()->isNextLevelReached())
	{
		User::getInstance()->increaseLevel();
	}

	// 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LoseScene *layer = LoseScene::create(crystals, dailyChallengeCompleted);
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

bool LoseScene::init(const std::map<CrystalColor, int>& crystals, bool dailyChallengeCompleted)
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	_gatheredCrystals = crystals;
    _dailyChallengeCompleted = dailyChallengeCompleted;
	
    if (_dailyChallengeCompleted && User::getInstance()->getDailyChallenges().isRewardDay())
    {
        PopUp::create(this, toString(User::getInstance()->getDailyChallenges().getDaysCompleted())+" days completed daily challenge award!", NULL,false);
    }
    
    
    // - -------------- DEBUG stuff
    
	/*CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCLabelTTF* label = CCLabelTTF::create("Game", "Arial", TITLE_FONT_SIZE);
	label->setString("You lose");
	
	label->setPosition(ccp(origin.x + visibleSize.width/2,
								 origin.y + visibleSize.height/2));
	
	this->addChild(label, 1);*/
	
	CCMenuItemImage* backButton = CCMenuItemImage::create(
														  "Interfeiss/backbtn0001.png",
														  "Interfeiss/backbtn0002.png",
														  this,
														  menu_selector(LoseScene::menuBackCallback));
	
    // create menu, it's an autorelease object
    CCMenu* menu = CCMenu::create(backButton, NULL);
    menu->alignItemsVertically();
    this->addChild(menu, 1);
	
    char lvlString[100];
    sprintf(lvlString, "LVL %d (%d / %d)",
    		User::getInstance()->getLevel()+1,
    		User::getInstance()->getExperience(),
    		Levels::getInstance()->nextLevelPoints());

    CCLabelTTF* lvl = CCLabelTTF::create(lvlString, "Arial", 30);
    lvl->setDimensions(CCSize(300, 100));
    lvl->setPositionY(500);
    lvl->setPositionX(480);
    addChild(lvl);

	return true;
}

void LoseScene::menuBackCallback(CCObject* sender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
	CCScene* mainMenu = MainMenuScene::scene();
	
	CCDirector::sharedDirector()->replaceScene(mainMenu);
}
