//
//  InGamePowers.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 01/11/14.
//
//

#include "InGamePowers.h"

USING_NS_CC;

InGamePowers::InGamePowers() :
__pScoreLabel(NULL)
{
}

bool InGamePowers::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    // Ask director the window size.
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // Create the score label.
    
//    __pScoreLabel = CCLabelTTF::create("Score 0", "Arial", 30);
    
    // Position the label on the right side of the screen.
//    __pScoreLabel->setPosition(ccp(winSize.width - 100, winSize.height - 20));
    
    // Add the label as a child to this layer.
//    addChild(__pScoreLabel, 1);
    
    
    //Add the 3 buttons
    button_1 = CCMenuItemImage::create(
                                                          "Charges_Machine.png",
                                                          "Charges_Machine.png",
                                                          this,
                                                          menu_selector(InGamePowers::onButton_1));
    
    button_2 = CCMenuItemImage::create(
                                                          "Charges_Machine.png",
                                                          "Charges_Machine.png",
                                                          this,
                                                          menu_selector(InGamePowers::onButton_2));
    
    button_3 = CCMenuItemImage::create(
                                                          "Charges_Machine.png",
                                                          "Charges_Machine.png",
                                                          this,
                                                          menu_selector(InGamePowers::onButton_3));
    
    CCMenu* mainMenu = CCMenu::create(button_1, button_2, button_3, NULL);
    mainMenu->alignItemsHorizontally();
    mainMenu->setPosition(ccp(0,0));
    addChild(mainMenu, 1);
    
    // The other stuff
//    mGameScene = dynamic_cast<GameScene*>(this->getParent());
    
    OnResetAllVars();
    
    return true;
}

void InGamePowers::onButton_1(CCObject* sender)
{
    CCLOG("CLick 1 !!!");
    
    OnGhoustDwarfs();
}

void InGamePowers::onButton_2(CCObject* sender)
{
    CCLOG("CLick 2 !!!");
    
    OnFreezeTroll();
}

void InGamePowers::onButton_3(CCObject* sender)
{
    CCLOG("CLick 3 !!!");
}

// The power upssss
void InGamePowers::OnFreezeTroll()
{
    // Activates the freeze troll stuff !!!
    button_2->setEnabled(false);
    button_2->setOpacity(128);
    
    // Activate master dwarf to freeze some troll
    mFreezeActive = true;
    
    // Show what trolls can be freezed
    for (int effectIndex = mGameScene->_trolls->count() - 1; effectIndex >= 0; --effectIndex)
    {
        Troll* trollIn = static_cast<Troll*>(mGameScene->_trolls->objectAtIndex(effectIndex));
        if (trollIn != NULL)
        {
            if(trollIn->getChildByTag(TROLL_SELECT_INDICATOR)!=NULL){
                trollIn->removeChildByTag(TROLL_SELECT_INDICATOR);
            }
            
            // Check if this troll can be troubled !!! If has no other carp on him
            
            //Create new target sprite
            CCSprite* aSprite = CCSprite::create("Troll_Target.png");
            aSprite->setTag(TROLL_SELECT_INDICATOR);
            aSprite->setPosition(ccp(0,60));
            trollIn->addChild(aSprite);
        }
    }
}

void InGamePowers::OnPlayerClickTroll(Troll* theTroll)
{
    mCurrentActiveTroll = theTroll;
    
    //What was the current power ??
    if(mFreezeActive){
        mFreezeActive = false;
        
        mGameScene->_mission_AP_Freezer+=1;
        mGameScene->CheckMissionByValue(MissionType_AP_Freezer,mGameScene->_mission_AP_Freezer);
        
        // Do some freeze FX - Like Master Dwarf fires his ball or stike
        CCSprite* aFlyBall = CCSprite::create("DebugDot.png");
        int aSpawnX = mGameScene->_MasterDwarfBase->getPositionX();
        int aSpawnY = mGameScene->_MasterDwarfBase->getPositionY()+mGameScene->_MasterDwarfBase->getContentSize().height;
        
        aFlyBall->setPosition(ccp(aSpawnX,aSpawnY)); // Where is that master troll powa stick?
        aFlyBall->setTag(FLY_BALL_MASTER_TROLL);// What is
        mGameScene->addChild(aFlyBall);
        
        // move it from master dwarf to the troll
        CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(theTroll->getPositionX(),theTroll->getPositionY()));
        CCCallFunc* aCall1 = CCCallFuncN::create(this, callfuncN_selector(InGamePowers::OnFreezeBallHit));
        CCSequence* aSeq1 = CCSequence::create(aMove1,aCall1,NULL);
        
        // Stop the troll !!! Otherwise it runs away from our ball :D
        mCurrentActiveTroll->mFreezedTime = 5;
        
        // Maybe add some ??? above him !!!
        CCSprite* aWtfMsg = CCSprite::create("troll_warn.png");
        aWtfMsg->setTag(TROLL_WTF_INDICATOR);// What is
        theTroll->addChild(aWtfMsg);
        
        //Run the action
        aFlyBall->runAction(aSeq1);
    }
    
    // Remove the indicator ?
    if(theTroll->getChildByTag(TROLL_SELECT_INDICATOR)!=NULL){
        theTroll->removeChildByTag(TROLL_SELECT_INDICATOR);
    }
    
}

// The kaboom stuff
void InGamePowers::OnFreezeBallHit(CCNode* sender)
{
    // Do all the magic here
    mCurrentActiveTroll->mFreezedTime = 10;//Get from missions some param?
    
    // Remove the indicator ?
    if(mCurrentActiveTroll->getChildByTag(TROLL_WTF_INDICATOR)!=NULL){
        mCurrentActiveTroll->removeChildByTag(TROLL_WTF_INDICATOR);
    }
    
    // Add for now blue troll FX
    mCurrentActiveTroll->_animation->setColor(ccc3(0, 164, 255));
    
    // Remove the ball
    if(mGameScene->getChildByTag(FLY_BALL_MASTER_TROLL)!=NULL){
        mGameScene->removeChildByTag(FLY_BALL_MASTER_TROLL);
    }
    
    // Create some particles and sound ???
    CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/bullet_explode.plist");
    p->setPosition(mCurrentActiveTroll->getPositionX(), mCurrentActiveTroll->getPositionY());
    p->setAutoRemoveOnFinish(true);
    mGameScene->addChild(p,5002);
    
    mGameScene->playInGameSound("dwarf_crash");
    
    // Clear it
    mCurrentActiveTroll = NULL;
    
    // Reset the correct button?
    button_2->setEnabled(true);
    button_2->setOpacity(255);
}

void InGamePowers::OnRemoveTrollIndicators()
{
    for (int effectIndex = mGameScene->_trolls->count() - 1; effectIndex >= 0; --effectIndex)
    {
        Troll* trollIn = static_cast<Troll*>(mGameScene->_trolls->objectAtIndex(effectIndex));
        if (trollIn != NULL)
        {
            if(trollIn->getChildByTag(TROLL_SELECT_INDICATOR)!=NULL){
                trollIn->removeChildByTag(TROLL_SELECT_INDICATOR);
            }
        }
    }
}

void InGamePowers::OnGhoustDwarfs()
{
    // Make all dwarfs tansparent
    mGameScene->_boostGhostTimer = 10;//Just enable it
    
    mGameScene->_mission_AP_GhostDwarfs+=1;
    mGameScene->CheckMissionByValue(MissionType_AP_GhostDwarfs,mGameScene->_mission_AP_GhostDwarfs);
}

void InGamePowers::OnResetAllVars()
{
    mCurrentActiveTroll = NULL;
    
    mFreezeActive = false;
}
