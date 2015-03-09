//
//  WorldMap.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 15/12/14.
//
//

#include "WorldMap.h"
#include "PanZoomLayer.h"
#include "AppMacros.h"
#include "GameScene.h"
#include "User.h"
#include "StoreScene.h"
#include "ChallengesScene.h"
#include "UpgradeScene.h"

USING_NS_CC;

CCScene* WorldMap::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    WorldMap *layer = WorldMap::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool WorldMap::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    //Create the big map?
//    CCSprite* pSprite = CCSprite::create("WorldMap/WorldMap_1.png");
//    addChild(pSprite);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    mLastMissionID = User::getInstance()->mCurrentMissionLevel;
    
    // Whats the screen size !!!
    mScreenSize = CCDirector::sharedDirector()->getVisibleSize();
    
    // Init the world map cords
//    std::string positions = "35,296,118,308,203,278,208,203,128,173,76,120,111,52,218,43,310,78,360,147";
    std::string positions =   "70,592,236,616,406,556,416,406,256,346,152,240,222,104,436,86,620,156,720,294";
    mWorldNodeCords = SplitString(positions,',');
    
    map_base = CCSprite::create( "button_freez.png" );
    map_base->setAnchorPoint( ccp( 0, 0 ) );
    
    PanZoomLayer *pzLayer = PanZoomLayer::create();
    this->addChild( pzLayer );
    
	moveBackground = false;
    
    // Add the map layers !!!
    CCSprite *map_1 = CCSprite::create("WorldMap/WorldMapHD_1.png");
    map_1->setAnchorPoint(ccp(0,0));
    map_1->setPosition(ccp(0,0));
    map_base->addChild(map_1);
    
    float aSizeW = map_1->getContentSize().width;
    float aSizeH = 0;//map->getContentSize().height
    
    // Update the total height of map
    aSizeH+=map_1->getContentSize().height;
    
    CCSprite *map_2 = CCSprite::create("WorldMap/WorldMapHD_2.png");
    map_2->setAnchorPoint(ccp(0,0));
    map_2->setPosition(ccp(0,map_1->getContentSize().height));
    map_base->addChild(map_2);
    
    aSizeH+=map_2->getContentSize().height; // Update the total height of map
    
    // Set it correct in screen
    map_base->setPosition( ccp(aSizeW*-0.5,aSizeH*-0.5) );
    
    pzLayer->SetPanBoundsRect( CCRectMake(
                                          aSizeW * -0.5,
                                          aSizeH * -0.5,
                                          aSizeW * 1.0,
                                          aSizeH * 1.0) );
//    pzLayer->setScale( 1.0 );
    
//    pzLayer->SetMinScale(0.9375f);
    pzLayer->SetMinScale(0.46875f);
    pzLayer->SetMaxScale(1.0f);
    
    pzLayer->addChild( map_base, -1 );
    
    // Set it now at start
    pzLayer->setPosition(ccp(aSizeW,aSizeH));
    
    /*
    CCSprite *mark = CCSprite::create( "button_freez.png" );
    map_base->addChild( mark );
    mark->setPosition( map_base->getContentSize() * 0.5 );
    */
    
    // Values
    mCurrentMissionID = 0;//No mission
    
    AddMovingObjects();
    
    CreateLevels();
    
    CreatePlayer();
    
    PrepeareSmallMissionScreen();
    
    CreateHud();
    
    //ResetStats();
    
    return true;
}

// Some particles and stuff like that :)
void WorldMap::AddMovingObjects()
{
    // Some test particles
    CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/AutumnFly_01.plist");
    p->setPosition(976, 1920);
    p->setPositionType(kCCPositionTypeGrouped);
    p->setAutoRemoveOnFinish(false);
    map_base->addChild(p,1);
    
    p = CCParticleSystemQuad::create("Particles/AutumnFly_01.plist");
    p->setPosition(406, 2280);
    p->setPositionType(kCCPositionTypeGrouped);
    p->setAutoRemoveOnFinish(false);
    map_base->addChild(p,1);
    
    p = CCParticleSystemQuad::create("Particles/AutumnFly_01.plist");
    p->setPosition(1194, 2272);
    p->setPositionType(kCCPositionTypeGrouped);
    p->setAutoRemoveOnFinish(false);
    map_base->addChild(p,1);
    
    p = CCParticleSystemQuad::create("Particles/AutumnFly_01.plist");
    p->setPosition(1664, 1962);
    p->setPositionType(kCCPositionTypeGrouped);
    p->setAutoRemoveOnFinish(false);
    map_base->addChild(p,1);
    
    p = CCParticleSystemQuad::create("Particles/AutumnFly_01.plist");
    p->setPosition(1812, 2240);
    p->setPositionType(kCCPositionTypeGrouped);
    p->setAutoRemoveOnFinish(false);
    map_base->addChild(p,1);
    
    // The snow
    p = CCParticleSystemQuad::create("Particles/SnowEasy.plist");
    p->setPosition(1000, 900);
    p->cocos2d::CCNode::setScale(0.8f);
    p->setPositionType(kCCPositionTypeGrouped);
    p->setAutoRemoveOnFinish(false);
    map_base->addChild(p,1);
    
    // The autumn
    p = CCParticleSystemQuad::create("effects/wind/Wind_New.plist");
    p->setPosition(ccp(0,1520));
//    p->cocos2d::CCNode::setScale(0.6f);
    p->setPositionType(kCCPositionTypeGrouped);
    p->setAutoRemoveOnFinish(false);
    map_base->addChild(p,1);//The reain tag :)
    

}

void WorldMap::CreateLevels()
{
    CCMenuItemImage *dummyItem;
    CCArray*array = CCArray::create();
    
    // Create all the levels
    for(int i=0;i<10;i++)
    {
        //Add button above?
        dummyItem = CCMenuItemImage::create("WorldMap/level_spot.png",
                                              "WorldMap/level_spot.png",
                                              this,
                                              menu_selector(WorldMap::OnClickedMission));
        dummyItem->setTag(i+1);
        dummyItem->setOpacity(0);
        dummyItem->setColor(ccBLUE);
        dummyItem->setPosition(ccp(mWorldNodeCords[i*2],mWorldNodeCords[i*2+1]));
        array->addObject(dummyItem);
    }
    
    CCMenu*menu = CCMenu::createWithArray(array);
    menu->setPosition(ccp(0,0));
    map_base->addChild(menu,1);
}

void WorldMap::OnClickedPlayer(CCObject* sender)
{
    ShowMissionScreen(User::getInstance()->mCurrentMissionLevel);
}

void WorldMap::OnClickedMission(CCObject* sender)
{
    // Do not open other mission
    if(mSmallMissionScreen->isVisible()){
        return;
    }
    
    //get the button id !!!
    CCMenuItemImage* aButton =(CCMenuItemImage*)sender;
    int aID = aButton->getTag();
    CCLog("Clicked Mission:%i",aID);
    MissionTaskInditificator(aID);
    
    //Check if this mission is clickable !!!
//    if(aID > User::getInstance()->mCurrentMissionLevel){
//        return;
//    }
    
    ccColor3B m_from = aButton->getColor();
    
    if(m_from.r == ccRED.r && m_from.g == ccRED.b && m_from.b == ccRED.b){
        aButton->setColor(ccBLUE);
    }
    else{
        aButton->setColor(ccRED);
    }
    
    ShowMissionScreen(aID);
}

void WorldMap::MissionTaskInditificator(int theID)
{
	WhatMission = theID;
	
	mCall = User::getInstance()->getMissionManager().GetMissionByID(WhatMission-1);

	TESTdwarfCount = mCall.Mission_SaveDwarfs;
	
	CCSprite* aScreenBooster2 = CCSprite::create("WorldMap/NewPregame/BoosterCardDamage.png");
    aScreenBooster2->setPosition(ccp(mSmallMissionScreen->getContentSize().width/4+80,mSmallMissionScreen->getContentSize().height/3-43));
    aScreenBooster2->setTag(712);
    
    CCSprite* aScreenBooster3 = CCSprite::create("WorldMap/NewPregame/BoosterCardRange.png");
    aScreenBooster3->setPosition(ccp(mSmallMissionScreen->getContentSize().width/4+270,mSmallMissionScreen->getContentSize().height/3-43));
    aScreenBooster3->setTag(713);
    
    CCSprite* Tootem = CCSprite::create("WorldMap/NewPregame/Totem.png");
    Tootem->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0-375,mSmallMissionScreen->getContentSize().height/1.2-110));
    Tootem->setTag(717);
    
    CCSprite* TaskVariables = CCSprite::create("WorldMap/NewPregame/TXTtotem.png");
    TaskVariables->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0-220,mSmallMissionScreen->getContentSize().height/1.2-110));
    TaskVariables->setTag(718);
    
    CCMenuItemImage* plusOneDamage = CCMenuItemImage::create(
                                                        "WorldMap/NewPregame/ButtonPlusOne.png",
                                                        "WorldMap/NewPregame/ButtonPlusOne.png",
                                                        this,
                                                        menu_selector(WorldMap::HideMissionScreen));
    plusOneDamage->setTag(714);//Play the level
    plusOneDamage->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0-185,mSmallMissionScreen->getContentSize().height/2-200));
    
    CCMenuItemImage* plusOneRange = CCMenuItemImage::create(
                                                        "WorldMap/NewPregame/ButtonPlusOne.png",
                                                        "WorldMap/NewPregame/ButtonPlusOne.png",
                                                        this,
                                                        menu_selector(WorldMap::HideMissionScreen));
    plusOneRange->setTag(715);//Play the level
    plusOneRange->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+5,mSmallMissionScreen->getContentSize().height/2-200));
	
	if(mCall.Task_type==6 || mCall.Task_type==8)
	{
	mSmallMissionScreen->addChild(aScreenBooster2);
	mSmallMissionScreen->addChild(aScreenBooster3);
	CCMenu* aButtonMenuA = CCMenu::create(plusOneDamage,plusOneRange,NULL);
    aButtonMenuA->setAnchorPoint(ccp(0,0));
    aButtonMenuA->setPosition(ccp(0,0));
    aButtonMenuA->setTag(716);
    mSmallMissionScreen->addChild(aButtonMenuA);
	}
	if (mCall.Task_type==1)
	{
		taskInfo = CCLabelTTF::create("RESCUE DWARFS","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8, CCSize(350,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+110,mSmallMissionScreen->getContentSize().height/2.0+345));//160,700
    	taskInfo->setTag(30004);
    	taskInfo->setColor(ccc3(36,102,102));
    	mSmallMissionScreen->addChild(taskInfo);
		mSmallMissionScreen->removeChildByTag(712);	
		mSmallMissionScreen->removeChildByTag(713);	
	}else if(mCall.Task_type==11){
		taskInfo = CCLabelTTF::create("SURVIVE","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8, CCSize(350,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+110,mSmallMissionScreen->getContentSize().height/2.0+345));//160,700
    	taskInfo->setTag(30004);
    	taskInfo->setColor(ccc3(36,102,102));
    	mSmallMissionScreen->addChild(taskInfo);
	}else if(mCall.Task_type==6){
		taskInfo = CCLabelTTF::create("DEFEAT MASTER TROLL","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8, CCSize(550,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+215,mSmallMissionScreen->getContentSize().height/2.0+345));//160,700
    	taskInfo->setTag(30004);
    	taskInfo->setColor(ccc3(36,102,102));
    	mSmallMissionScreen->addChild(taskInfo);		
	}
	else if (mCall.Task_type==8){
		taskInfo = CCLabelTTF::create("DESTROY TOTEM","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8, CCSize(350,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+110,mSmallMissionScreen->getContentSize().height/2.0+345));//160,700
    	taskInfo->setTag(30004);
    	taskInfo->setColor(ccc3(36,102,102));
    	mSmallMissionScreen->addChild(taskInfo);
    	
    	_totemHP = CCLabelTTF::create("","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	_totemHP->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0-125,mSmallMissionScreen->getContentSize().height/1.2+15));//160,700
		_totemHP->setTag(30009);
    	_totemHP->setColor(ccc3(36,102,102));
    	mSmallMissionScreen->addChild(_totemHP);
		
    	std::stringstream _totemHPReal;
    	_totemHPReal<<int(mCall.TOTEM_HP);
    	_totemHP->setString(_totemHPReal.str().c_str());
    	
    	if(mCall.TOTEM_Event_Type1==1)
    	{
    	_totemShield = CCLabelTTF::create("Bubble Shield","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	_totemShield->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0-80,mSmallMissionScreen->getContentSize().height/1.2-20));//160,700
		_totemShield->setTag(30010);
    	_totemShield->setColor(ccc3(36,102,102));
    	mSmallMissionScreen->addChild(_totemShield);
    	}else if (mCall.TOTEM_Event_Type1==2){
    	_totemShield = CCLabelTTF::create("Spike Shield","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	_totemShield->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0-80,mSmallMissionScreen->getContentSize().height/1.2-20));//160,700
		_totemShield->setTag(30010);
    	_totemShield->setColor(ccc3(36,102,102));
    	mSmallMissionScreen->addChild(_totemShield);	
    	}
    	
    	if(mCall.TOTEM_Event_Type3==1)
    	{
    		_totemAttack = CCLabelTTF::create("Bullets","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    		_totemAttack->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0-75,mSmallMissionScreen->getContentSize().height/1.2-55));//160,700
			_totemAttack->setTag(30011);
    		_totemAttack->setColor(ccc3(36,102,102));
    		mSmallMissionScreen->addChild(_totemAttack);
    	}else if (mCall.TOTEM_Event_Type3==2){
    		_totemAttack = CCLabelTTF::create("Flamethrower","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    		_totemAttack->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0-75,mSmallMissionScreen->getContentSize().height/1.2-55));//160,700
			_totemAttack->setTag(30011);
    		_totemAttack->setColor(ccc3(36,102,102));
    		mSmallMissionScreen->addChild(_totemAttack);
    	}
    	
    	mSmallMissionScreen->addChild(Tootem);
    	mSmallMissionScreen->addChild(TaskVariables);		
	}else if (mCall.Task_type==10){
		taskInfo = CCLabelTTF::create("Gather Crystals","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8, CCSize(550,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+215,mSmallMissionScreen->getContentSize().height/2.0+355));//160,700
    	taskInfo->setTag(30004);
    	taskInfo->setColor(ccc3(36,102,102));
    	mSmallMissionScreen->addChild(taskInfo);	
		mSmallMissionScreen->removeChildByTag(712);		
		mSmallMissionScreen->removeChildByTag(713);	
	}
	
	if(mCall.Mission_SaveDwarfs)
	{
		dwarfCount = CCLabelTTF::create("","PowerButtons/PB_Font.fnt", TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	dwarfCount->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0-190,mSmallMissionScreen->getContentSize().height/2.0+37));//160,700
    	dwarfCount->setTag(30005);
    	dwarfCount->setColor(ccc3(36,102,102));
    	mSmallMissionScreen->addChild(dwarfCount);	
    	
    	std::stringstream dwarfCountReal;
    	dwarfCountReal<<int(mCall.Mission_SaveDwarfs);
    	dwarfCount->setString(dwarfCountReal.str().c_str());
    	
    	CCMenuItemImage* plusOne = CCMenuItemImage::create(
                                                        "WorldMap/NewPregame/ButtonPlusOne.png",
                                                        "WorldMap/NewPregame/ButtonPlusOne.png",
                                                        this,
                                                        menu_selector(WorldMap::BuyMoreDwarfs));
    	plusOne->setTag(1);//Play the level
    	plusOne->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0-377,mSmallMissionScreen->getContentSize().height/2-200));
    
    	CCMenu* aButtonMenu2 = CCMenu::create(plusOne,NULL);
    	aButtonMenu2->setAnchorPoint(ccp(0,0));
    	aButtonMenu2->setPosition(ccp(0,0));
    	mSmallMissionScreen->addChild(aButtonMenu2);
    
     	dwarfPrice =CCLabelBMFont::create("50", "PowerButtons/PB_Font.fnt",150, kCCTextAlignmentCenter);
   	 	dwarfPrice->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0-325,mSmallMissionScreen->getContentSize().height/2-200));
    	//dwarfPrice->setColor(ccc3(36,102,102));
    	dwarfPrice->setWidth(10);
    	dwarfPrice->setTag(30006);
    	mSmallMissionScreen->addChild(dwarfPrice);	
    	
    	std::stringstream dwarfPriceReal;
    	dwarfPriceReal<<int(mCall.STORE_Booster_DwarfPrice);
    	dwarfPrice->setString(dwarfPriceReal.str().c_str());
    
	}
	
		_diamondsLabel = CCLabelTTF::create("Game", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(90, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
		_diamondsLabel->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+234,mSmallMissionScreen->getContentSize().height/2+95));
		_diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
    	_diamondsLabel->setColor(ccc3(255,246,200));
    	_diamondsLabel->setTag(30007);
    	mSmallMissionScreen->addChild(_diamondsLabel);	
    	
    	_crystalsLabel = CCLabelTTF::create("Game", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5, CCSize(90, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
		_crystalsLabel->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+234,mSmallMissionScreen->getContentSize().height/2+175));
		_crystalsLabel->setString(toString(User::getInstance()->getCrystals()).c_str());
    	_crystalsLabel->setColor(ccc3(255,246,200));
    	_crystalsLabel->setTag(30008);
    	mSmallMissionScreen->addChild(_crystalsLabel);	
    	
	
	if(WhatMission == 1)//Identificate what mission number we need
	{
		//mCall = User::getInstance()->getMissionManager().GetMissionByID(0);
		//mCall.Task_type
		/*
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M1.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
    	
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(0);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
	
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);
			*/		
	}else if(WhatMission == 2)
	{
		/*
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M2.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);	
    	
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(1);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		//removeChildByTag(30004);
		
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);	
		*/			
	}else if(WhatMission == 3)
	{
		/*
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M3.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);	
    	
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(2);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);	
		*/			
	}else if(WhatMission == 4)
	{
		/*
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M4.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
    	
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(3);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		*/
		    	/*
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(2);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);			
		*/	
	}else if(WhatMission == 5)
	{
		/*
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M5.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);	
    	
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(4);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		*/	
		    	/*
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(2);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);
		*/			
	}else if(WhatMission == 6)
	{
			/*
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M6.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
	
		mCall = User::getInstance()->getMissionManager().GetMissionByID(5);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		*/
		    	/*
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(2);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);		
		*/			
    }else if(WhatMission == 7)
	{
		/*
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M7.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
		
		mCall = User::getInstance()->getMissionManager().GetMissionByID(5);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());	
		*/
		    	/*
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(2);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);	
		*/			
    }else if(WhatMission == 8)
	{
			/*
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M8.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
	
		mCall = User::getInstance()->getMissionManager().GetMissionByID(5);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		*/
		    	/*
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(2);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);	
		*/				
    }else if(WhatMission == 9)
	{
		/*
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M9.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
		
		mCall = User::getInstance()->getMissionManager().GetMissionByID(5);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		*/
		    	/*
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(2);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);	
		*/				
    }
}

void WorldMap::ResetStats()
{
	//mCall = User::getInstance()->getMissionManager().Reset();
}

void WorldMap::BuyMoreDwarfs()
{
	CCLog("Works so far!!!");
	
	int aPrice = mCall.STORE_Booster_DwarfPrice;
	
	int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }else{
        	if(aPrice <=0)
        	{
        	}else{
        	//mCall = User::getInstance()->getMissionManager().GetMissionByID(WhatMission+1);
			if(WhatMission == 1)//Identificate what mission number we need
			{
			mCall = User::getInstance()->getMissionManager().AddDwarfs(0);
			mCall.Mission_SaveDwarfs;
			}else if(WhatMission == 2)
			{
			mCall = User::getInstance()->getMissionManager().AddDwarfs(1);
			mCall.Mission_SaveDwarfs;	
			}else if(WhatMission == 3)
			{
			mCall = User::getInstance()->getMissionManager().AddDwarfs(2);
			mCall.Mission_SaveDwarfs;	
			}else if(WhatMission == 4)
			{
			mCall = User::getInstance()->getMissionManager().AddDwarfs(3);
			mCall.Mission_SaveDwarfs;	
			}else if(WhatMission == 5)
			{
			mCall = User::getInstance()->getMissionManager().AddDwarfs(4);
			mCall.Mission_SaveDwarfs;	
			}else if(WhatMission == 6)
			{
			mCall = User::getInstance()->getMissionManager().AddDwarfs(5);
			mCall.Mission_SaveDwarfs;	
			}else if(WhatMission == 7)
			{
			mCall = User::getInstance()->getMissionManager().AddDwarfs(6);
			mCall.Mission_SaveDwarfs;	
			}else if(WhatMission == 8)
			{
			mCall = User::getInstance()->getMissionManager().AddDwarfs(7);
			mCall.Mission_SaveDwarfs;	
			}else if(WhatMission == 9)
			{
			mCall = User::getInstance()->getMissionManager().AddDwarfs(8);
			mCall.Mission_SaveDwarfs;	
			}else if(WhatMission == 10)
			{
			mCall = User::getInstance()->getMissionManager().AddDwarfs(9);
			mCall.Mission_SaveDwarfs;	
			}
			
        	}
		}

	UpdateStats();
	
}

void WorldMap::UpdateStats()
{
	std::stringstream _diamondsLabelReal;
    _diamondsLabelReal<<int(User::getInstance()->getDiamonds());
    _diamondsLabel->setString(_diamondsLabelReal.str().c_str());
    
    std::stringstream dwarfCountReal;
    dwarfCountReal<<int(mCall.Mission_SaveDwarfs);
    dwarfCount->setString(dwarfCountReal.str().c_str());
}

void WorldMap::CreatePlayer()
{
    // Try to load player with the image what he has on fb !!!
    int _currentLevel = User::getInstance()->mCurrentMissionLevel-1;// Load from save
    
    CCSprite* base_1 = CCSprite::create("WorldMap/player_spot.png");
    CCSprite* base_2 = CCSprite::create("WorldMap/avatar_none.png");
    base_2->setPosition(ccp(base_1->getContentSize().width/2,base_1->getContentSize().height/2));
    base_1->addChild(base_2);
    
    CCMenuItemSprite *dummyItem = CCMenuItemSprite::create(base_1, base_1, this, menu_selector(WorldMap::OnClickedPlayer));
    mPlayer = CCMenu::createWithItem(dummyItem);
    mPlayer->setPosition(ccp(mWorldNodeCords[_currentLevel*2],mWorldNodeCords[_currentLevel*2+1]+30));
    
    map_base->addChild(mPlayer);
    
    //MissionTaskInditificator(_currentLevel);
    
    // Maybe now create other players with their progress ? TODO
    
}

void WorldMap::onEnterTransitionDidFinish()
{
    if(mLastMissionID == User::getInstance()->mCurrentMissionLevel){
        return;
    }
    
    mLastMissionID = User::getInstance()->mCurrentMissionLevel;
    int _currentLevel = mLastMissionID-1;// Load from save
    
    // Move that player avatar to new space
    CCMoveTo* aMove1 = CCMoveTo::create(1.0f,ccp(mWorldNodeCords[_currentLevel*2],mWorldNodeCords[_currentLevel*2+1]+20));
    CCEaseExponentialInOut* aEase1 = CCEaseExponentialInOut::create(aMove1);
    CCCallFuncN* aFuncDone = CCCallFuncN::create(this, callfuncN_selector(WorldMap::OnPlayerFinishedMove));
    CCSequence* aSeq1 = CCSequence::create(aEase1,aFuncDone,NULL);
    mPlayer->runAction(aSeq1);
}

void WorldMap::OnPlayerFinishedMove()
{
    // Create some particles and stuff like that !!!
    CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/bullet_explode.plist");
    p->setPosition(mPlayer->getPosition());
    p->setAutoRemoveOnFinish(true);
    map_base->addChild(p,1000);
}

/*
void WorldMap::onEnter()
{
    CheckPlayerMoveFuther();
}

void WorldMap::onExit()
{
    mLastMissionID = User::getInstance()->mCurrentMissionLevel;
}
*/

/*
void WorldMap::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    
}

bool WorldMap::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    return true;
}

void WorldMap::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    
}

void WorldMap::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void WorldMap::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
*/

std::vector<int> static &split(const std::string &s, char delim, std::vector<int> &elems){
    std::stringstream ss(s);
    std::string item;
    int resultInt;
    
    while (std::getline(ss, item, delim)) {
        resultInt = atoi(item.c_str());
        elems.push_back(resultInt);
    }
    return elems;
}

std::vector<int> WorldMap::SplitString(const std::string s,char delim){
    std::vector<int> elems;
    split(s, delim, elems);
    return elems;
}

//-------------------------------------------------------
// The small mission screen !!!

void WorldMap::PrepeareSmallMissionScreen()
{
    mSmallMissionScreen = CCLayerColor::create(ccc4(0,0,0,128),mScreenSize.width,mScreenSize.height);
    mSmallMissionScreen->setVisible(false);
    
    //Add the screen
    CCSprite* aScreenPlay = CCSprite::create("WorldMap/NewPregame/Background.png");
    aScreenPlay->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2,mSmallMissionScreen->getContentSize().height/2));
    mSmallMissionScreen->addChild(aScreenPlay);
    
    // Add 2 buttons clouse and play for now !!!
    
    // The play button
    CCMenuItemImage* playItem = CCMenuItemImage::create(
                                                        "WorldMap/NewPregame/ButtonPlay.png",
                                                        "WorldMap/NewPregame/ButtonPlay.png",
                                                        this,
                                                        menu_selector(WorldMap::HideMissionScreen));
    playItem->setTag(1);//Play the level
    playItem->setPosition(ccp(aScreenPlay->getContentSize().width/2.0+300,aScreenPlay->getContentSize().height/6));
    
    // The clouse button
    CCMenuItemImage* clouseItem = CCMenuItemImage::create(
                                                        "Interfeiss/before_quit/btn_cross.png",
                                                        "Interfeiss/before_quit/btn_cross.png",
                                                        this,
                                                        menu_selector(WorldMap::HideMissionScreen));
    clouseItem->setTag(2);//Clouse screen
    clouseItem->setPosition(ccp(aScreenPlay->getContentSize().width-30,aScreenPlay->getContentSize().height-30));
    
    CCMenuItemImage* upgradeItem = CCMenuItemImage::create(
                                                        "WorldMap/NewPregame/ButtonUpgrades.png",
                                                        "WorldMap/NewPregame/ButtonUpgrades.png",
                                                        this,
                                                        menu_selector(WorldMap::Hud_ShowStore));
    upgradeItem->setTag(1);//Play the level
    upgradeItem->setPosition(ccp(aScreenPlay->getContentSize().width/2.0+300,aScreenPlay->getContentSize().height/3));
    
    CCSprite* MissionTask = CCSprite::create("WorldMap/NewPregame/TXTMissionTask.png");
    MissionTask->setPosition(ccp(aScreenPlay->getContentSize().width/4,aScreenPlay->getContentSize().height+18));
    mSmallMissionScreen->addChild(MissionTask);
    
    CCSprite* aScreenPlay2 = CCSprite::create("WorldMap/NewPregame/Background2.png");
    aScreenPlay2->setPosition(ccp(aScreenPlay->getContentSize().width/2.0-140,aScreenPlay->getContentSize().height/1.2));
    mSmallMissionScreen->addChild(aScreenPlay2);
    
    CCSprite* aScreenBooster = CCSprite::create("WorldMap/NewPregame/BoosterCardDwarfs.png");
    aScreenBooster->setPosition(ccp(aScreenPlay->getContentSize().width/6.0-24,aScreenPlay->getContentSize().height/3));
    mSmallMissionScreen->addChild(aScreenBooster);
    
    CCSprite* aNeedle = CCSprite::create("WorldMap/NewPregame/boostersneedle.png");
    aNeedle->setFlipX(true);
    aNeedle->setPosition(ccp(aScreenPlay->getContentSize().width/6.0-10,aScreenPlay->getContentSize().height/2+60));
    mSmallMissionScreen->addChild(aNeedle);
    
    CCSprite* aNeedle2 = CCSprite::create("WorldMap/NewPregame/boostersneedle.png");
    aNeedle2->setFlipX(false);
    aNeedle2->setPosition(ccp(aScreenPlay->getContentSize().width/6.0+350,aScreenPlay->getContentSize().height/2+60));
    mSmallMissionScreen->addChild(aNeedle2);
    
    CCSprite* Boosters = CCSprite::create("WorldMap/NewPregame/TXTBoosters.png");
    Boosters->setPosition(ccp(aScreenPlay->getContentSize().width/6.0+170,aScreenPlay->getContentSize().height/2+60));
    mSmallMissionScreen->addChild(Boosters);
    
    CCSprite* CurrencyCrystals = CCSprite::create("WorldMap/NewPregame/CurrencyCrystals.png");
    CurrencyCrystals->setPosition(ccp(aScreenPlay->getContentSize().width/2+240,aScreenPlay->getContentSize().height/1.1));
    mSmallMissionScreen->addChild(CurrencyCrystals);
    
    CCSprite* CurrencyDiamonds = CCSprite::create("WorldMap/NewPregame/CurrencyDiamonds.png");
    CurrencyDiamonds->setPosition(ccp(aScreenPlay->getContentSize().width/2+231,aScreenPlay->getContentSize().height/1.3));
    mSmallMissionScreen->addChild(CurrencyDiamonds);
    
    CCMenu* aButtonMenu = CCMenu::create(playItem,clouseItem,upgradeItem,NULL);
    aButtonMenu->setAnchorPoint(ccp(0,0));
    aButtonMenu->setPosition(ccp(0,0));
    aScreenPlay->addChild(aButtonMenu, 1);
    
    addChild(mSmallMissionScreen,1);//Above all
}

void WorldMap::ShowMissionScreen(int theID)
{
	moveBackground = true;
	mSmallMissionScreen->removeChildByTag(30011);
	mSmallMissionScreen->removeChildByTag(30010);
	mSmallMissionScreen->removeChildByTag(30009);
	mSmallMissionScreen->removeChildByTag(30008);
	mSmallMissionScreen->removeChildByTag(30007);
	mSmallMissionScreen->removeChildByTag(30006);
	mSmallMissionScreen->removeChildByTag(30005);
	mSmallMissionScreen->removeChildByTag(30004);
    mSmallMissionScreen->removeChildByTag(30002);
    mSmallMissionScreen->removeChildByTag(712);
    mSmallMissionScreen->removeChildByTag(713);
    mSmallMissionScreen->removeChildByTag(714);
    mSmallMissionScreen->removeChildByTag(715);
    mSmallMissionScreen->removeChildByTag(716);
    mSmallMissionScreen->removeChildByTag(717);
    mSmallMissionScreen->removeChildByTag(718);
    //removeChildByTag(67881);
    mCurrentMissionID = theID;
    MissionTaskInditificator(mCurrentMissionID);
    mSmallMissionScreen->setVisible(true);
}

void WorldMap::HideMissionScreen(CCObject * pSender)
{
	moveBackground = false;
	//moveBackground = false;
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = (int)pMenuItem->getTag();
    
    mSmallMissionScreen->removeChildByTag(30011);
    mSmallMissionScreen->removeChildByTag(30010);
    mSmallMissionScreen->removeChildByTag(30009);
    mSmallMissionScreen->removeChildByTag(30008);
    mSmallMissionScreen->removeChildByTag(30007);
    mSmallMissionScreen->removeChildByTag(30006);
    mSmallMissionScreen->removeChildByTag(30005);
    mSmallMissionScreen->removeChildByTag(30004);
    mSmallMissionScreen->removeChildByTag(30002);
    mSmallMissionScreen->removeChildByTag(712);
    mSmallMissionScreen->removeChildByTag(713);
    mSmallMissionScreen->removeChildByTag(714);
    mSmallMissionScreen->removeChildByTag(715);
    mSmallMissionScreen->removeChildByTag(716);
    mSmallMissionScreen->removeChildByTag(717);
    mSmallMissionScreen->removeChildByTag(718);
    mSmallMissionScreen->setVisible(false);
    
    if(tag == 1){
        //Fade all black and load the needed level
        CCLOG("mCurrentMissionID:%i",mCurrentMissionID);
        
        // Set the value what level will be played now
        User::getInstance()->getMissionManager().mCurrentActiveMission = mCurrentMissionID-1;
        
        CCScene* scene;
        
        // If need to show some promo - do it
        scene = GameScene::scene();
        CCTransitionScene* transition = CCTransitionFade::create(0.5f, scene);
        CCDirector::sharedDirector()->replaceScene(transition);
    }
    else{
        CCLOG("Do nothing cloused screen");
    }
}

// The other stuff
void WorldMap::CreateHud()
{
    CCSprite * storeNormalSprite= CCSprite::create("Interfeiss/main_menu/new/store_btn_new0001.png");
    CCSprite * storeSelectedlSprite= CCSprite::create("Interfeiss/main_menu/new/store_btn_new0002.png");
    
    CCMenuItemSprite *storeItem = CCMenuItemSprite::create(storeNormalSprite, storeSelectedlSprite,
                                                           this,
                                                           menu_selector(WorldMap::Hud_ShowStore));
    
    CCSprite * challengesNormalSprite= CCSprite::create("Interfeiss/main_menu/new/challenges_btn_new0001.png");
    CCSprite * challengesSelectedlSprite= CCSprite::create("Interfeiss/main_menu/new/challenges_btn_new0002.png");
    
    CCMenuItemSprite *challengesItem = CCMenuItemSprite::create(challengesNormalSprite, challengesSelectedlSprite,
                                                                this,
                                                                menu_selector(WorldMap::Hud_ShowChallenges));
    
    CCMenuItemImage* backButtonReload = CCMenuItemImage::create(
                                                                "DebugStuff/load_mission_1.png",
                                                                "DebugStuff/load_mission_1.png",
                                                                this,
                                                                menu_selector(WorldMap::OnReloadStuff));
    backButtonReload->setAnchorPoint(ccp(0,0));
    
    CCMenu *mMainMenu = CCMenu::create(storeItem,
                               challengesItem,backButtonReload,
                               NULL);
    
    mMainMenu->alignItemsHorizontally();
    
    mMainMenu->setPosition(ccp(mScreenSize.width/2-27,44));
    
    mMainMenu->setTag(67881);
    
    map_base->addChild(mMainMenu);
    
}

void WorldMap::OnReloadStuff()
{
    User::getInstance()->getMissionManager().OnDownloadSpecialMissions();
}

void WorldMap::Hud_ShowStore(CCObject* sender)
{
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
    
//    if(mOptionsOpen)
//        return;
//    
//    mOptionsOpen = true;
    
    StoreScene* storeLayer = StoreScene::create();
    storeLayer->setTag(2223);
    this->addChild(storeLayer,100);
}

void WorldMap::OnRemoveStore()
{
//    AnalyticX::flurryLogEvent("Menu Hide: Store");
    
    removeChildByTag(2223,true);
//    mOptionsOpen = false;
}

void WorldMap::Hud_ShowChallenges(CCObject* sender)
{
    //Old stuff
//    mOptionsOpen = true;
    
//    mMainMenu->setEnabled(false);
    
//    AnalyticX::flurryLogEvent("Menu Show: Challenges");
    
    ChallengesScene* challengeLayer = ChallengesScene::create();
    this->addChild(challengeLayer,100);
    moveBackground = true;
//    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
}


