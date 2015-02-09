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
	
	if(WhatMission == 1)//Identificate what mission number we need
	{
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M1.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
    	/*
    	mCall = User::getInstance()->getMissionManager().GetMissionByID(0);
 		int minCrystalCount = mCall.Star_1;
		
		taskInfo = CCLabelTTF::create("",FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(250,250), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
    	taskInfo->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+140,mSmallMissionScreen->getContentSize().height/2.0+220));//160,700
    	taskInfo->setTag(30004);
    	mSmallMissionScreen->addChild(taskInfo);	
    
    	std::stringstream taskInfoCount;
    	taskInfoCount<<"Collect "<<int(minCrystalCount)<<" green Crystals";
    	taskInfo->setString(taskInfoCount.str().c_str());
		*/
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);		
	}else if(WhatMission == 2)
	{
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M2.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);	
    	/*
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
		*/
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);				
	}else if(WhatMission == 3)
	{
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M3.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);	
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
		*/
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);				
	}else if(WhatMission == 4)
	{
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M4.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
    	/*
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
		*/
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);				
	}else if(WhatMission == 5)
	{
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M5.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);	
    	/*
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
		*/
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);			
	}else if(WhatMission == 6)
	{
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M6.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
		/*
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
		*/
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);					
    }else if(WhatMission == 7)
	{
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M7.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
		/*
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
		*/
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);				
    }else if(WhatMission == 8)
	{
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M8.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
		/*
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
		*/
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);					
    }else if(WhatMission == 9)
	{
		CCSprite* missionNumber = CCSprite::create("WorldMap/mission_number/M9.png");
    	missionNumber->setPosition(ccp(595,570));//550,480
    	missionNumber->setTag(30002);
    	missionNumber->setScale(0.5);
    	mSmallMissionScreen->addChild(missionNumber);
		/*
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
		*/
		CCSprite* missionTask = CCSprite::create("WorldMap/mission_number/defeat_master_troll.png");
    	missionTask->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2.0+120,mSmallMissionScreen->getContentSize().height/2.0+120));//550,480
    	missionTask->setTag(30004);
    	mSmallMissionScreen->addChild(missionTask);					
    }
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
    CCSprite* aScreenPlay = CCSprite::create("WorldMap/pre_screen.png");
    aScreenPlay->setPosition(ccp(mSmallMissionScreen->getContentSize().width/2,mSmallMissionScreen->getContentSize().height/2));
    mSmallMissionScreen->addChild(aScreenPlay);
    
    // Add 2 buttons clouse and play for now !!!
    
    // The play button
    CCMenuItemImage* playItem = CCMenuItemImage::create(
                                                        "Interfeiss/main_menu/play_btn0001.png",
                                                        "Interfeiss/main_menu/play_btn0002.png",
                                                        this,
                                                        menu_selector(WorldMap::HideMissionScreen));
    playItem->setTag(1);//Play the level
    playItem->setPosition(ccp(aScreenPlay->getContentSize().width/2,0));
    
    // The clouse button
    CCMenuItemImage* clouseItem = CCMenuItemImage::create(
                                                        "Interfeiss/before_quit/btn_cross.png",
                                                        "Interfeiss/before_quit/btn_cross.png",
                                                        this,
                                                        menu_selector(WorldMap::HideMissionScreen));
    clouseItem->setTag(2);//Clouse screen
    clouseItem->setPosition(ccp(aScreenPlay->getContentSize().width,aScreenPlay->getContentSize().height));
    
    CCMenu* aButtonMenu = CCMenu::create(playItem,clouseItem,NULL);
    aButtonMenu->setAnchorPoint(ccp(0,0));
    aButtonMenu->setPosition(ccp(0,0));
    aScreenPlay->addChild(aButtonMenu);
    
    
    addChild(mSmallMissionScreen,1);//Above all
}

void WorldMap::ShowMissionScreen(int theID)
{
	mSmallMissionScreen->removeChildByTag(30004);
    mSmallMissionScreen->removeChildByTag(30002);
    mCurrentMissionID = theID;
    MissionTaskInditificator(mCurrentMissionID);
    mSmallMissionScreen->setVisible(true);
}

void WorldMap::HideMissionScreen(CCObject * pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = (int)pMenuItem->getTag();
    
    mSmallMissionScreen->removeChildByTag(30004);
    mSmallMissionScreen->removeChildByTag(30002);
    
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
    
    CCMenu *mMainMenu = CCMenu::create(storeItem,
                               challengesItem,
                               NULL);
    
    mMainMenu->alignItemsHorizontally();
    
    mMainMenu->setPosition(ccp(mScreenSize.width/2-27,44));
    
    this->addChild(mMainMenu, 1);
    
    
    
    
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
    
//    SimpleAudioEngine::sharedEngine()->playEffect(getSoundFx("button_click").c_str());
}


