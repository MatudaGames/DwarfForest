//
//  InGamePowers.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 01/11/14.
//
//

#include "InGamePowers.h"

#include "User.h"

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
    
    mPowerCost_1 = 100;
    mPowerCost_2 = 50;
    mPowerCost_3 = 50;
    
    mPowerCoolDown_1 = 15;
    mPowerCoolDown_2 = 10;
    mPowerCoolDown_3 = 10;
    
    // Ask director the window size.
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // Create the score label.
    
//    __pScoreLabel = CCLabelTTF::create("Score 0", "Arial", 30);
    
    // Position the label on the right side of the screen.
//    __pScoreLabel->setPosition(ccp(winSize.width - 100, winSize.height - 20));
    
    // Add the label as a child to this layer.
//    addChild(__pScoreLabel, 1);
    
    
    // The base of button
    
    
    //------------------------------------------------------------------------------------------------------------
    // The new spell stuff !!!
    
    std::vector<int> theActiveSpells = User::getInstance()->getItemDataManager().getActiveItems();
    
    button_1 = NULL;
    button_2 = NULL;
    
    // Create the 1st button
    if(theActiveSpells.size()>0)
    {
        // Get the real data from spell
        mButtonSpell_1 = User::getInstance()->getItemDataManager().getSpellByID(theActiveSpells[0]);
        
        button_1 = CCMenuItemImage::create(
                                           mButtonSpell_1.icon_path.c_str(),
                                           mButtonSpell_1.icon_path.c_str(),
                                           this,
                                           menu_selector(InGamePowers::onButton_1));
        
        button_1->setOpacity(255);
    }
    
    if(theActiveSpells.size()>1)
    {
        mButtonSpell_2 = User::getInstance()->getItemDataManager().getSpellByID(theActiveSpells[1]);
        
        button_2 = CCMenuItemImage::create(
                                           mButtonSpell_2.icon_path.c_str(),
                                           mButtonSpell_2.icon_path.c_str(),
                                           this,
                                           menu_selector(InGamePowers::onButton_2));
        
        button_2->setOpacity(128);
    }
    
    // Lets create the menu for buttons
    CCMenu* mainMenu;
    
    if(button_1 != NULL && button_2 == NULL){
        mainMenu = CCMenu::create(button_1, NULL);
    }
    else{
        mainMenu = CCMenu::create(button_1,button_2, NULL);
    }
    
    mainMenu->alignItemsHorizontallyWithPadding(10);
    mainMenu->setPosition(ccp(0,20));
//    mainMenu->setOpacity(0);
    addChild(mainMenu, 1);
    
    
    OnResetAllVars();
    
    return true;
    
    //------------------------------------------------------------------------------------------------------------
    
    
    //------------------------------------------------------------------------------------------------------------
    //The button place in world
    
    /*
    mButton_1_Base = CCSprite::create("small_dot_red.png");
    mButton_1_Base->setPosition(ccp(-92,20));
    addChild(mButton_1_Base);
    
    // The bottom of button
    mButton_1_TopBack = CCSprite::create("PowerButtons/PB_TopBack.png");
    mButton_1_TopBack->setPosition(ccp(mButton_1_Base->getContentSize().width/2,
                                       mButton_1_Base->getContentSize().height/2));
    mButton_1_Base->addChild(mButton_1_TopBack,-1);
    
    // The progress bar
    CCSprite* aProgressSprite = CCSprite::create("PowerButtons/PB_Progress.png");
    mButton_1_Progress = CCProgressTimer::create(aProgressSprite);
    mButton_1_Progress->setType(kCCProgressTimerTypeRadial);
    mButton_1_Progress->setPercentage(100);
    mButton_1_Progress->setPosition(ccp(mButton_1_Base->getContentSize().width/2,
                                       mButton_1_Base->getContentSize().height/2));
    mButton_1_Base->addChild(mButton_1_Progress);
    
    // The above stuff
    mButton_1_Top = CCSprite::create("PowerButtons/PB_Top.png");
    mButton_1_Top->setPosition(ccp(mButton_1_Base->getContentSize().width/2,
                                       mButton_1_Base->getContentSize().height/2));
    mButton_1_Base->addChild(mButton_1_Top);
    
    
    // The icon - can be custom
    mButton_1_Icon = CCSprite::create("PowerButtons/PB_Ghost_Icon.png");
    mButton_1_Icon->setPosition(ccp(mButton_1_Top->getContentSize().width/2,
                                   mButton_1_Top->getContentSize().height/2));
    mButton_1_Icon->setColor(ccGRAY);
    mButton_1_Top->addChild(mButton_1_Icon);
    
    // The procent of load
    mButton_1_Load = CCLabelBMFont::create("100%", "PowerButtons/PB_Font.fnt", mButton_1_Top->getContentSize().width, kCCTextAlignmentCenter);
    mButton_1_Load->setPosition(ccp(mButton_1_Base->getContentSize().width/2+2,mButton_1_Base->getContentSize().height/2-20));
    mButton_1_Load->setScale(0.75f);
    mButton_1_Load->setVisible(false);
    mButton_1_Base->addChild(mButton_1_Load,1);
    
    
    // This is the cost bar
    mButton_1_Bottom = CCSprite::create("PowerButtons/PB_Bottom.png");
    mButton_1_Base->addChild(mButton_1_Bottom);
    mButton_1_Bottom->setPosition(ccp(mButton_1_Base->getContentSize().width/2,
                                    mButton_1_Base->getContentSize().height/2-45));
    
    // Add the text field for this button !!! Cost of crystals
    mButton_1_Cost =CCLabelBMFont::create("100", "PowerButtons/PB_Font.fnt", mButton_1_Bottom->getContentSize().width, kCCTextAlignmentCenter);
    mButton_1_Cost->setPosition(ccp(mButton_1_Bottom->getContentSize().width/2-12,mButton_1_Bottom->getContentSize().height/2-4));
    mButton_1_Cost->setColor(ccc3(183, 183, 183));
    mButton_1_Bottom->addChild(mButton_1_Cost);
    
    //------------------------------------------------------------------------------------------------------------
    
    // The next button !!!
    //------------------------------------------------------------------------------------------------------------
    //The button place in world
    
    mButton_2_Base = CCSprite::create("small_dot_red.png");
    mButton_2_Base->setPosition(ccp(0,20));
    addChild(mButton_2_Base);
    
    // The bottom of button
    mButton_2_TopBack = CCSprite::create("PowerButtons/PB_TopBack.png");
    mButton_2_TopBack->setPosition(ccp(mButton_2_Base->getContentSize().width/2,
                                       mButton_2_Base->getContentSize().height/2));
    mButton_2_Base->addChild(mButton_2_TopBack,-1);
    
    // The progress bar
    aProgressSprite = CCSprite::create("PowerButtons/PB_Progress.png");
    mButton_2_Progress = CCProgressTimer::create(aProgressSprite);
    mButton_2_Progress->setType(kCCProgressTimerTypeRadial);
    mButton_2_Progress->setPercentage(100);
    mButton_2_Progress->setPosition(ccp(mButton_2_Base->getContentSize().width/2,
                                        mButton_2_Base->getContentSize().height/2));
    mButton_2_Base->addChild(mButton_2_Progress);
    
    // The above stuff
    mButton_2_Top = CCSprite::create("PowerButtons/PB_Top.png");
    mButton_2_Top->setPosition(ccp(mButton_2_Base->getContentSize().width/2,
                                   mButton_2_Base->getContentSize().height/2));
    mButton_2_Base->addChild(mButton_2_Top);
    
    
    // The icon - can be custom
    mButton_2_Icon = CCSprite::create("PowerButtons/PB_Attack_Icon.png");
    mButton_2_Icon->setPosition(ccp(mButton_2_Top->getContentSize().width/2,
                                    mButton_2_Top->getContentSize().height/2));
    mButton_2_Icon->setColor(ccGRAY);
    mButton_2_Top->addChild(mButton_2_Icon);
    
    // The procent of load
    mButton_2_Load = CCLabelBMFont::create("100%", "PowerButtons/PB_Font.fnt", mButton_2_Top->getContentSize().width, kCCTextAlignmentCenter);
    mButton_2_Load->setPosition(ccp(mButton_2_Base->getContentSize().width/2+2,mButton_2_Base->getContentSize().height/2-20));
    mButton_2_Load->setScale(0.75f);
    mButton_2_Load->setVisible(false);
    mButton_2_Base->addChild(mButton_2_Load,1);
    
    
    // This is the cost bar
    mButton_2_Bottom = CCSprite::create("PowerButtons/PB_Bottom.png");
    mButton_2_Base->addChild(mButton_2_Bottom);
    mButton_2_Bottom->setPosition(ccp(mButton_2_Base->getContentSize().width/2,
                                      mButton_2_Base->getContentSize().height/2-45));
    
    // Add the text field for this button !!! Cost of crystals
    mButton_2_Cost =CCLabelBMFont::create("50", "PowerButtons/PB_Font.fnt", mButton_2_Bottom->getContentSize().width, kCCTextAlignmentCenter);
    mButton_2_Cost->setPosition(ccp(mButton_2_Bottom->getContentSize().width/2-12,mButton_2_Bottom->getContentSize().height/2-4));
    mButton_2_Cost->setColor(ccc3(183, 183, 183));
    mButton_2_Bottom->addChild(mButton_2_Cost);
    
    //------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------
    //The button place in world
    
    mButton_3_Base = CCSprite::create("small_dot_red.png");
    mButton_3_Base->setPosition(ccp(92,20));
    addChild(mButton_3_Base);
    
    // The bottom of button
    mButton_3_TopBack = CCSprite::create("PowerButtons/PB_TopBack.png");
    mButton_3_TopBack->setPosition(ccp(mButton_3_Base->getContentSize().width/2,
                                       mButton_3_Base->getContentSize().height/2));
    mButton_3_Base->addChild(mButton_3_TopBack,-1);
    
    // The progress bar
    aProgressSprite = CCSprite::create("PowerButtons/PB_Progress.png");
    mButton_3_Progress = CCProgressTimer::create(aProgressSprite);
    mButton_3_Progress->setType(kCCProgressTimerTypeRadial);
    mButton_3_Progress->setPercentage(100);
    mButton_3_Progress->setPosition(ccp(mButton_3_Base->getContentSize().width/2,
                                        mButton_3_Base->getContentSize().height/2));
    mButton_3_Base->addChild(mButton_3_Progress);
    
    // The above stuff
    mButton_3_Top = CCSprite::create("PowerButtons/PB_Top.png");
    mButton_3_Top->setPosition(ccp(mButton_3_Base->getContentSize().width/2,
                                   mButton_3_Base->getContentSize().height/2));
    mButton_3_Base->addChild(mButton_3_Top);
    
    
    // The icon - can be custom
    mButton_3_Icon = CCSprite::create("PowerButtons/PB_Freeze_Icon.png");
    mButton_3_Icon->setPosition(ccp(mButton_3_Top->getContentSize().width/2,
                                    mButton_3_Top->getContentSize().height/2));
    mButton_3_Icon->setColor(ccGRAY);
    mButton_3_Top->addChild(mButton_3_Icon);
    
    // The procent of load
    mButton_3_Load = CCLabelBMFont::create("100%", "PowerButtons/PB_Font.fnt", mButton_3_Top->getContentSize().width, kCCTextAlignmentCenter);
    mButton_3_Load->setPosition(ccp(mButton_3_Base->getContentSize().width/2+2,mButton_3_Base->getContentSize().height/2-20));
    mButton_3_Load->setScale(0.75f);
    mButton_3_Load->setVisible(false);
    mButton_3_Base->addChild(mButton_3_Load,1);
    
    
    // This is the cost bar
    mButton_3_Bottom = CCSprite::create("PowerButtons/PB_Bottom.png");
    mButton_3_Base->addChild(mButton_3_Bottom);
    mButton_3_Bottom->setPosition(ccp(mButton_3_Base->getContentSize().width/2,
                                      mButton_3_Base->getContentSize().height/2-45));
    
    // Add the text field for this button !!! Cost of crystals
    mButton_3_Cost =CCLabelBMFont::create("50", "PowerButtons/PB_Font.fnt", mButton_3_Bottom->getContentSize().width, kCCTextAlignmentCenter);
    mButton_3_Cost->setPosition(ccp(mButton_3_Bottom->getContentSize().width/2-12,mButton_3_Bottom->getContentSize().height/2-4));
    mButton_3_Cost->setColor(ccc3(183, 183, 183));
    mButton_3_Bottom->addChild(mButton_3_Cost);
    
    //------------------------------------------------------------------------------------------------------------
    
    //Add the 3 buttons
    button_1 = CCMenuItemImage::create(
                                                          "button_freez.png",
                                                          "button_freez.png",
                                                          this,
                                                          menu_selector(InGamePowers::onButton_1));
    
    
    button_2 = CCMenuItemImage::create(
                                                          "button_electro.png",
                                                          "button_electro.png",
                                                          this,
                                                          menu_selector(InGamePowers::onButton_2));
    
    
    button_3 = CCMenuItemImage::create(
                                                          "Charges_Machine.png",
                                                          "Charges_Machine.png",
                                                          this,
                                                          menu_selector(InGamePowers::onButton_3));
    
    
    CCMenu* mainMenu = CCMenu::create(button_1,button_2,button_3, NULL);
    mainMenu->alignItemsHorizontallyWithPadding(10);
    mainMenu->setPosition(ccp(0,20));
    mainMenu->setOpacity(0);
    addChild(mainMenu, 1);
    
    // The other stuff
//    mGameScene = dynamic_cast<GameScene*>(this->getParent());
    
    OnResetAllVars();
    */
    
    return true;
}

void InGamePowers::UpdateButtons()
{
    return;
    // Checks for crystal cost and button enable state !!!
    
    // Check if this button is not in progress
    if(mButton_1_Progress->getPercentage()==100)
    {
        // mCrystalPoints = the real crystal count
        // mMasterTroll_Attack = the current point taker
        
        if(mGameScene->mMasterTroll_Attack>=mPowerCost_1){
            mButton_1_Icon->setColor(ccc3(255,255,255));
        }
        else{
            mButton_1_Icon->setColor(ccGRAY);
        }
    }
    
    if(mButton_2_Progress->getPercentage()==100)
    {
        // mCrystalPoints = the real crystal count
        // mMasterTroll_Attack = the current point taker
        
        if(mGameScene->mMasterTroll_Attack>=mPowerCost_2){
            mButton_2_Icon->setColor(ccc3(255,255,255));
        }
        else{
            mButton_2_Icon->setColor(ccGRAY);
        }
    }
    
    if(mButton_3_Progress->getPercentage()==100)
    {
        // mCrystalPoints = the real crystal count
        // mMasterTroll_Attack = the current point taker
        
        if(mGameScene->mMasterTroll_Attack>=mPowerCost_3){
            mButton_3_Icon->setColor(ccc3(255,255,255));
        }
        else{
            mButton_3_Icon->setColor(ccGRAY);
        }
    }
}

void InGamePowers::onButton_1(CCObject* sender)
{
    CCLOG("CLick 1 !!!");
    
//    mButtonSpell_1
    mGameScene->mCurrentActiveSpell = 0;
    
    button_1->setOpacity(255);
    
    if(button_2 != NULL){
        button_2->setOpacity(128);
    }
    
    mGameScene->mCurrentSpellCharge = mButtonSpell_1.charge;
    
    float aTotalValue = float(mGameScene->mMasterTroll_Attack) / float(mGameScene->mCurrentSpellCharge);
//    CCLog("mMasterTroll_Attack:%i / mCurrentSpellCharge:%i = aTotalValue: %f",mGameScene->mMasterTroll_Attack, mGameScene->mCurrentSpellCharge,aTotalValue);
    if(aTotalValue>1)aTotalValue = 1;
    mGameScene->mBattleBar_MachinePower->setTextureRect(CCRect(0, 0,
                                                               mGameScene-> mBattleBar_MachinePower->getTexture()->getContentSize().width*(aTotalValue),
                                                               mGameScene->mBattleBar_MachinePower->getTexture()->getContentSize().height));
    
    mGameScene->UpdateBattleLabel();
    
    /*
    if(mButton_1_Progress->getPercentage()!=100){
        return;
    }
    if(mGameScene->mMasterTroll_Attack<mPowerCost_1){
        return; // Not enough crystals
    }
    OnStartRecharge(0);
    */
}

void InGamePowers::onButton_2(CCObject* sender)
{
    CCLOG("CLick 2 !!!");
    
    mGameScene->mCurrentActiveSpell = 1;
    
    button_1->setOpacity(128);
    
    if(button_2 != NULL){
        button_2->setOpacity(255);
    }
    
    // Update the battle bar forced !!!
    mGameScene->mCurrentSpellCharge = mButtonSpell_2.charge;
    
    float aTotalValue = float(mGameScene->mMasterTroll_Attack) / float(mGameScene->mCurrentSpellCharge);
//    CCLog("mMasterTroll_Attack:%i / mCurrentSpellCharge:%i = aTotalValue: %f",mGameScene->mMasterTroll_Attack, mGameScene->mCurrentSpellCharge,aTotalValue);
    if(aTotalValue>1)aTotalValue = 1;
    mGameScene->mBattleBar_MachinePower->setTextureRect(CCRect(0, 0,
                                                               mGameScene-> mBattleBar_MachinePower->getTexture()->getContentSize().width*(aTotalValue),
                                                               mGameScene->mBattleBar_MachinePower->getTexture()->getContentSize().height));
    
    
    mGameScene->UpdateBattleLabel();
    
    /*
    if(mButton_2_Progress->getPercentage()!=100){
        return;
    }
    if(mGameScene->mMasterTroll_Attack<mPowerCost_2){
        return; // Not enough crystals
    }
    OnStartRecharge(1);
    */
}

void InGamePowers::onButton_3(CCObject* sender)
{
    CCLOG("CLick 3 !!!");
    
    /*
    mGameScene->mCurrentMission.MT_Bullet_Speed_Min = 150;
    mGameScene->mCurrentMission.MT_Bullet_Speed_Max = 260;
    
    mGameScene->mCurrentBulletType = 11;
    mGameScene->MasterAction_Bullet(NULL);
    */
    
    if(mButton_3_Progress->getPercentage()!=100){
        return;
    }
    if(mGameScene->mMasterTroll_Attack<mPowerCost_3){
        return; // Not enough crystals
    }
    OnStartRecharge(2);
}

// The power upssss
void InGamePowers::OnFreezeTroll()
{
    // Activates the freeze troll stuff !!!
    button_1->setEnabled(false);
    button_1->setOpacity(128);
    
    // Activate master dwarf to freeze some troll
    mFreezeActive = true;
    mElectroActive = false;
    
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

void InGamePowers::OnElectroTroll()
{
    button_2->setEnabled(false);
    button_2->setOpacity(128);
    
    mElectroActive = true;
    mFreezeActive = false;
    
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
        
        //Whats the cooldown for it?
        button_1->setEnabled(true);
        button_1->setOpacity(255);
        
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
    else if(mElectroActive){
        mElectroActive = false;
        
        button_2->setEnabled(true);
        button_2->setOpacity(255);
        
        // Maybe add some ??? above him !!!
        CCSprite* aWtfMsg = CCSprite::create("troll_warn.png");
        aWtfMsg->setTag(TROLL_WTF_INDICATOR);// What is
        theTroll->addChild(aWtfMsg);
        
        mGameScene->CreateBlitz(theTroll->getPositionX(),theTroll->getPositionY()-80,theTroll);
    }
    
    // Remove the indicator ?
    if(theTroll->getChildByTag(TROLL_SELECT_INDICATOR)!=NULL){
        theTroll->removeChildByTag(TROLL_SELECT_INDICATOR);
    }
    
}

void InGamePowers::OnStartRecharge(int theID)
{
    // Clear old stuff
    if(theID == 0)
    {
        mGameScene->OnTryToShoot();
        
        mLoad_1_StringStream.str("");
        mLoad_1_StringStream.clear();
        
        mButton_1_Load->setString("0%");
        mButton_1_Load->setVisible(true);
        
        mButton_1_Icon->setColor(ccGRAY);
        
        // Run the magic
        CCProgressFromTo* aAction = CCProgressFromTo::create(mPowerCoolDown_1, 0, 100);
        mButton_1_Progress->runAction(aAction);
        
        // Start the progress bar stuff
        schedule(schedule_selector(InGamePowers::OnUpdateProgress_1));
    }
    else if(theID == 1)
    {
        mGameScene->mMasterTroll_Attack-=mPowerCost_2;
        OnGhoustDwarfs();
        
        mLoad_2_StringStream.str("");
        mLoad_2_StringStream.clear();
        
        mButton_2_Load->setString("0%");
        mButton_2_Load->setVisible(true);
        
        mButton_2_Icon->setColor(ccGRAY);
        
        // Run the magic
        CCProgressFromTo* aAction = CCProgressFromTo::create(mPowerCoolDown_2, 0, 100);
        mButton_2_Progress->runAction(aAction);
        
        // Start the progress bar stuff
        schedule(schedule_selector(InGamePowers::OnUpdateProgress_2));
    }
    else if(theID == 2)
    {
        mGameScene->mMasterTroll_Attack-=mPowerCost_3;
        mGameScene->StartDwarfFreeze_All();
        
        mLoad_3_StringStream.str("");
        mLoad_3_StringStream.clear();
        
        mButton_3_Load->setString("0%");
        mButton_3_Load->setVisible(true);
        
        mButton_3_Icon->setColor(ccGRAY);
        
        // Run the magic
        CCProgressFromTo* aAction = CCProgressFromTo::create(mPowerCoolDown_3, 0, 100);
        mButton_3_Progress->runAction(aAction);
        
        // Start the progress bar stuff
        schedule(schedule_selector(InGamePowers::OnUpdateProgress_3));
    }
    
    mGameScene->UpdateBattleLabel();
}

void InGamePowers::OnUpdateProgress_1(float Dt)
{
    if(mButton_1_Progress->getPercentage()>=100)
    {
        // Finished it - play sound etc
        unschedule(schedule_selector(InGamePowers::OnUpdateProgress_1));
        mButton_1_Load->setVisible(false);
        
        // Checks for crystal amount
        UpdateButtons();
    }
    else
    {
        mLoad_1_StringStream.str("");
        mLoad_1_StringStream.clear();
        mLoad_1_StringStream<<(int)mButton_1_Progress->getPercentage()<<"%";
        mButton_1_Load->setString(mLoad_1_StringStream.str().c_str());
    }
}

void InGamePowers::OnUpdateProgress_2(float Dt)
{
    if(mButton_2_Progress->getPercentage()>=100)
    {
        // Finished it - play sound etc
        unschedule(schedule_selector(InGamePowers::OnUpdateProgress_2));
        mButton_2_Load->setVisible(false);
        
        // Checks for crystal amount
        UpdateButtons();
    }
    else
    {
        mLoad_2_StringStream.str("");
        mLoad_2_StringStream.clear();
        mLoad_2_StringStream<<(int)mButton_2_Progress->getPercentage()<<"%";
        mButton_2_Load->setString(mLoad_2_StringStream.str().c_str());
    }
}

void InGamePowers::OnUpdateProgress_3(float Dt)
{
    if(mButton_3_Progress->getPercentage()>=100)
    {
        // Finished it - play sound etc
        unschedule(schedule_selector(InGamePowers::OnUpdateProgress_3));
        mButton_3_Load->setVisible(false);
        
        // Checks for crystal amount
        UpdateButtons();
    }
    else
    {
        mLoad_3_StringStream.str("");
        mLoad_3_StringStream.clear();
        mLoad_3_StringStream<<(int)mButton_3_Progress->getPercentage()<<"%";
        mButton_3_Load->setString(mLoad_3_StringStream.str().c_str());
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
//    mGameScene->_boostGhostTimer = 10;//Just enable it
    
    mGameScene->_mission_AP_GhostDwarfs+=1;
    mGameScene->CheckMissionByValue(MissionType_AP_GhostDwarfs,mGameScene->_mission_AP_GhostDwarfs);
}

void InGamePowers::OnResetAllVars()
{
    mCurrentActiveTroll = NULL;
    
    mFreezeActive = false;
}
