//
//  StoreBox.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 02/03/14.
//
//

#include "StoreBox.h"

#include "JniCalls.h"
#include "AppMacros.h"

#include "AnalyticX.h"

#include "User.h"
#include "StoreScene.h"

USING_NS_CC;

// The split to string stuff
std::vector<std::string> static &splitString(const std::string &s, char delim, std::vector<std::string> &elems){
    std::stringstream ss(s);
    std::string item;
    std::string resultInt;
    
    while (std::getline(ss, item, delim)) {
        resultInt = item;
        elems.push_back(resultInt);
    }
    return elems;
}

std::vector<std::string> StoreBox::SplitString_VecString(const std::string s,char delim){
    std::vector<std::string> elems;
    splitString(s, delim, elems);
    return elems;
}

CCScene* StoreBox::scene(CCRect size,CCRect zone)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StoreBox* layer = StoreBox::create(size,zone);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

StoreBox* StoreBox::create(CCRect size,CCRect zone)
{
    StoreBox *pRet = new StoreBox();
    if (pRet && pRet->init(size,zone))
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

// on "init" you need to initialize your instance
bool StoreBox::init(CCRect size,CCRect zone)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    mCurrentSelectedButtonIndex = 0;
    
    mLayer_2X = 0;
    
    mBoxCords = size;
    mZoneCords = zone;
    mZoneCords = CCRect(zone.origin.x+zone.size.width/2+100,zone.origin.y+zone.size.height/2+40,zone.size.width,zone.size.height);
    
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCLayerColor* aLayer = CCLayerColor::create(ccc4(0,0,0,128),zone.size.width,zone.size.height);
    aLayer->setPosition(ccp(zone.origin.x,zone.origin.y));
//    addChild(aLayer);
    
    mUpgradesCreated = false;
    
    mBase = CCSprite::create("Interfeiss/store/FRESH/df_shop.png");
    mBase->setOpacity(0);
    mBase->setPosition(ccp(zone.origin.x+zone.size.width/2,zone.origin.y+zone.size.height/2));
    addChild(mBase);
    
    mBaseShop = CCSprite::create("Interfeiss/store/FRESH/df_shop.png");
    mBaseShop->setOpacity(0);
    mBaseShop->setPosition(ccp(zone.origin.x+zone.size.width/2,zone.origin.y+zone.size.height/2));
    addChild(mBaseShop);
    
    mBaseFreeStuff = CCSprite::create("Interfeiss/store/FRESH/df_shop.png");
    mBaseFreeStuff->setOpacity(0);
    mBaseFreeStuff->setPosition(ccp(zone.origin.x+zone.size.width/2,zone.origin.y+zone.size.height/2));
    addChild(mBaseFreeStuff);
    
    
//    CCLog("mBase->getPositionY():%f",mBase->getPositionY());
    
    mMinScrollY = mBase->getPositionY();
    mMinScroll_Shop_Y = mBaseShop->getPositionY();
    mMinScroll_Free_Y = mBaseFreeStuff->getPositionY();
    
    return true;
}

void StoreBox::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void StoreBox::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool StoreBox::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    mDidMoveFinger = false;
    
//    CCLOG("Did touch started");
    if(!mZoneCords.containsPoint(touch->getLocation()))
    {
        return false;
    }
    
    if (_moveStarted == false)
    {
        _moveStarted = true;
        _dragStart = touch->getLocation();
        
        if(mBase->isVisible())
        {
            if(mBase->numberOfRunningActions()>0)
                mBase->stopAllActions();
            
            mLayer_2X = mBase->getPositionY();
        }
        else if(mBaseShop->isVisible())
        {
            if(mBaseShop->numberOfRunningActions()>0)
                mBaseShop->stopAllActions();
            
            mLayer_2X = mBaseShop->getPositionY();
        }
        else if(mBaseFreeStuff->isVisible())
        {
            if(mBaseFreeStuff->numberOfRunningActions()>0)
                mBaseFreeStuff->stopAllActions();
            
            mLayer_2X = mBaseFreeStuff->getPositionY();
        }
        
        mSlidingFree = false;
    }
    
    return true;
}

void StoreBox::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    _moveStarted = false;
    _dragEnd = touch->getLocation();
    
    //check if slider is not too much off limits
    int aPosY = 0;
    bool aNeedToMove = false;
    
    //Disable all button clicks if was active
    if(mBase->isVisible())
    {
        /*
        if(User::getInstance()->getMachine_EnemiesLvl()>0)
            ChangeButtonState(3,2);
        else
            ChangeButtonState(3,1);
        
        if(User::getInstance()->getMachine_PlantLvl()>0)
            ChangeButtonState(1,2);
        else
            ChangeButtonState(1,1);
        
        if(User::getInstance()->getMachine_GhostLvl()>0)
            ChangeButtonState(2,2);
        else
            ChangeButtonState(2,1);
        
        if(User::getInstance()->getMachine_StampLvl()>0)
            ChangeButtonState(4,2);
        else
            ChangeButtonState(4,1);
        */
        
//        CCLog("mBase->getPositionY():%f",mBase->getPositionY());
        
        if(mBase->getPositionY()<mMinScroll_Shop_Y)
        {
            aPosY = mMinScroll_Shop_Y;
            aNeedToMove = true;
        }
        else if(mBase->getPositionY()>mMaxScroll_Shop_Y)
        {
            aPosY = mMaxScroll_Shop_Y;
            aNeedToMove = true;
        }
        
        if(aNeedToMove)
        {
            CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(mBase->getPositionX(),aPosY));
            CCEaseBackOut* aEase1 = CCEaseBackOut::create(aMove1);
            mBase->runAction(aEase1);
        }
        
        /*
        if(mBase->getPositionY()<mMinScrollY)
        {
            aPosY = mMinScrollY;
            aNeedToMove = true;
        }
        else if(mBase->getPositionY()>mMaxScrollY)
        {
            aPosY = mMaxScrollY;
            aNeedToMove = true;
        }
        
        if(aNeedToMove)
        {
            CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(mBase->getPositionX(),aPosY));
            CCEaseBackOut* aEase1 = CCEaseBackOut::create(aMove1);
            mBase->runAction(aEase1);
        }
        */
    }
    else if(mBaseShop->isVisible())
    {
//        CCLog("mBaseShop->getPositionY():%f",mBaseShop->getPositionY());
        if(mBaseShop->getPositionY()<mMinScroll_Shop_Y)
        {
            aPosY = mMinScroll_Shop_Y;
            aNeedToMove = true;
        }
        else if(mBaseShop->getPositionY()>mMaxScroll_Shop_Y)
        {
            aPosY = mMaxScroll_Shop_Y;
            aNeedToMove = true;
        }
        
        if(aNeedToMove)
        {
            CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(mBaseShop->getPositionX(),aPosY));
            CCEaseBackOut* aEase1 = CCEaseBackOut::create(aMove1);
            mBaseShop->runAction(aEase1);
        }
    }
    else if(mBaseFreeStuff->isVisible())
    {
//        CCLog("mBaseFreeStuff->getPositionY():%f",mBaseFreeStuff->getPositionY());
        if(mBaseFreeStuff->getPositionY()<mMinScroll_Free_Y)
        {
            aPosY = mMinScroll_Free_Y;
            aNeedToMove = true;
        }
        else if(mBaseFreeStuff->getPositionY()>mMaxScroll_Free_Y)
        {
            aPosY = mMaxScroll_Free_Y;
            aNeedToMove = true;
        }
        
        if(aNeedToMove)
        {
            CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(mBaseFreeStuff->getPositionX(),aPosY));
            CCEaseBackOut* aEase1 = CCEaseBackOut::create(aMove1);
            mBaseFreeStuff->runAction(aEase1);
        }
    }
}

void StoreBox::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
//    CCLOG("Did touch moved");
    
    if(mDidMoveFinger!=true)
        mDidMoveFinger = true;
    
    if(mBase->isVisible())
        mBase->setPositionY(mLayer_2X+(touch->getLocation().y - _dragStart.y));
    else if(mBaseFreeStuff->isVisible())
        mBaseFreeStuff->setPositionY(mLayer_2X+(touch->getLocation().y - _dragStart.y));
    else if(mBaseShop->isVisible())
        mBaseShop->setPositionY(mLayer_2X+(touch->getLocation().y - _dragStart.y));
}

void StoreBox::visit()
{
//    CCPoint convertedLocationLowerLeft = this->getParent()->convertToWorldSpace(this->getPosition());
    glEnable(GL_SCISSOR_TEST);
    //    glScissor(convertedLocationLowerLeft.x, convertedLocationLowerLeft.y,
    //              this->getContentSize().width, this->getContentSize().height);
    glScissor(mBoxCords.origin.x,mBoxCords.origin.y,mBoxCords.size.width,mBoxCords.size.height);
    CCLayer::visit();
    glDisable(GL_SCISSOR_TEST);
}

//The header stuff
void StoreBox::UpdateStats()
{
//    _scoreLabel->setString(toString(User::getInstance()->getCrystals()).c_str());
//    _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
    
    StoreScene* mainMenu = (StoreScene*)this->getParent()->getParent();
    mainMenu->UpdateStats();
}

//The mega ultra craete functional !!!
void StoreBox::CreateUpgrades()
{
    //Clear before created stuff??? or just hide it???
    mBase->setVisible(false);
    mBaseFreeStuff->setVisible(false);
    mBaseShop->setVisible(false);
    
    // Reset values
    mCurrentSelectedButtonIndex = -1;
    
    //Create the menu buttons too!!!
    if(mUpgradesCreated)
    {
        mBase->setVisible(true);
        CheckSpellButtons(true,true);
        return;
    }
    
    mBase->setVisible(true);
    
    mUpgradesCreated = true;
    
    // Add the new shop here !!!
    
    //Add some elements
    int aX = 20;
    int aY = 450;
    int aSpaceY = 10;
    
    // Now here comes the magic for rading from xml
    CCSprite* aHeader = CCSprite::create("Interfeiss/store/FRESH/shop/title_combo_deals.png");
    aHeader->setPosition(ccp(aX,aY+90));
    mBase->addChild(aHeader,1);
    
    int aTotalSpellTabItems = User::getInstance()->getItemDataManager().mSpellDataVector.size();
    CCLog("Total Spell Tab Itmes: %i",aTotalSpellTabItems);
    
    int aTotalPowerTabItems = User::getInstance()->getItemDataManager().mPowerDataVector.size();
    CCLog("Total Power Tab Itmes: %i",aTotalPowerTabItems);
    
    CCSprite* aBoxBase;
    CCLabelTTF* aTxtHeader = NULL;
    
    CCLabelTTF* aTxtPowerDesc = NULL;
    
    CCSprite* aIcon;
    CCSprite* aLockIcon;
    
    CCSprite* aHeaderIcon;
    
    // Special magic
    CCSprite* aIconDamage;
    CCSprite* aIconRange;
    CCLabelTTF* aTxt_Damage = NULL;
    CCLabelTTF* aTxt_Range = NULL;
    
    CCLabelTTF* aTxt_Price = NULL;
    CCLabelTTF* aTxt_PriceAmount = NULL;
    CCSprite* aPriceIcon;
    
    CCMenuItemImage* MenuButton;
    CCMenu* MenuButtons;
    
    CCSprite* aDummyStar;
    
    int aGlobalMidOffestX = 40;
    
    int aScrollOffsetY = 20;
    mMaxScroll_Shop_Y = 0;
    
    //........................................................................
    // Create the powerups before spells !!!
    
    aY = 420;
    
    for(int i = 0;i<aTotalPowerTabItems;i++)
    {
        aBoxBase = CCSprite::create("Shop/panel_upgrades.png");
        aBoxBase->setPosition(ccp(aX,aY));
        aBoxBase->setTag(i+1000);
        mBase->addChild(aBoxBase);
        aY-=aBoxBase->getContentSize().height+aSpaceY;
        
        // Update how far can scroll
        if(aY<=0) mMaxScroll_Shop_Y+=aBoxBase->getContentSize().height;
        
        //------------------------------------------------------------------
        // The Header
        
        aTxtHeader = CCLabelTTF::create(User::getInstance()->getItemDataManager().mPowerDataVector[i].name.c_str(),
                                        "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6);
        aTxtHeader->setHorizontalAlignment(kCCTextAlignmentLeft);
        aTxtHeader->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxtHeader->setPosition(ccp(aBoxBase->getContentSize().width/2,aBoxBase->getContentSize().height-24));
        aTxtHeader->setColor(ccc3(84, 71, 52));
        aBoxBase->addChild(aTxtHeader);
        
        // Add the side icons
        aHeaderIcon = CCSprite::create("Shop/text_deco.png");
        aHeaderIcon->setPosition(ccp(aTxtHeader->getPositionX()-aTxtHeader->getContentSize().width/2-50,aTxtHeader->getPositionY()));
        aBoxBase->addChild(aHeaderIcon);
        
        aHeaderIcon = CCSprite::create("Shop/text_deco.png");
        aHeaderIcon->setFlipX(true);
        aHeaderIcon->setPosition(ccp(aTxtHeader->getPositionX()+aTxtHeader->getContentSize().width/2+50,aTxtHeader->getPositionY()));
        aBoxBase->addChild(aHeaderIcon);
        
        //----
        // The Level
        bool aPowerItemUnLocked = User::getInstance()->getItemDataManager().isPowerItemUnlocked(User::getInstance()->getItemDataManager().mPowerDataVector[i].id);
        int aPowerItemLevel = User::getInstance()->getItemDataManager().getPowerItemLevel(User::getInstance()->getItemDataManager().mPowerDataVector[i].id);
        
        CCLog("aPowerItemUnLocked: %i",aPowerItemUnLocked);
        CCLog("aPowerItemLevel: %i",aPowerItemLevel);
        
        //------------------------------------------------------------------
        // The icon?
        
        // Get the correct icon by upgrade etc
        std::stringstream power_icon;
        if(User::getInstance()->getItemDataManager().mPowerDataVector[i].icon_change)
        {
            power_icon << User::getInstance()->getItemDataManager().mPowerDataVector[i].icon_path << aPowerItemLevel <<".png";
        }
        else
        {
            power_icon << User::getInstance()->getItemDataManager().mPowerDataVector[i].icon_path;
        }
        
        aIcon = CCSprite::create(power_icon.str().c_str());
        aIcon->setTag(BUTTON_ICON);
        aIcon->setPosition(ccp(50+aIcon->getContentSize().width/2,aBoxBase->getContentSize().height/2));
        aBoxBase->addChild(aIcon);
        
        // Do we need to add above icon?
        if(aPowerItemUnLocked == false)
        {
            aLockIcon = CCSprite::create("Shop/locked.png");
            aLockIcon->setTag(ICON_LOCKED);
            aLockIcon->setPosition(ccp(aIcon->getContentSize().width/2,aIcon->getContentSize().height/2));
            aIcon->addChild(aLockIcon);
        }
        
        //------------------------------------------------------------------
        // The text for power
        std::string theDesc;
        float fontSize;
        
        int aUpgradeIndexCheck = aPowerItemLevel;
        
        if(aPowerItemUnLocked == false)
        {
            aUpgradeIndexCheck -= 1; //Take 1st cost
            fontSize = TITLE_FONT_SIZE*0.7;
            theDesc = User::getInstance()->getItemDataManager().mPowerDataVector[i].text_locked;
        }
        else
        {
            fontSize = TITLE_FONT_SIZE*0.5;
            theDesc = User::getInstance()->getItemDataManager().mPowerDataVector[i].text_unlocked;
        }
        
        aTxtPowerDesc = CCLabelTTF::create(theDesc.c_str(),"fonts/Marker Felt.ttf",fontSize);
        aTxtPowerDesc->setTag(POWER_DESC);
        aTxtPowerDesc->setHorizontalAlignment(kCCTextAlignmentLeft);
        aTxtPowerDesc->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxtPowerDesc->setPosition(ccp(aBoxBase->getContentSize().width/2+50,aBoxBase->getContentSize().height/2));
        aTxtPowerDesc->setColor(ccc3(84, 71, 52));
        aBoxBase->addChild(aTxtPowerDesc);
        
        // TODO check if not max upgraded !!!
        
        // The Price for unlock/buy?
        int aMaxUpgrades = User::getInstance()->getItemDataManager().mPowerDataVector[i].max_upgrades;
        
        // The safe check
        if(aUpgradeIndexCheck>=aMaxUpgrades){
            aUpgradeIndexCheck = aMaxUpgrades-1;
        }
        
        std::string theCostInfo = User::getInstance()->getItemDataManager().mPowerDataVector[i].level_cost[aUpgradeIndexCheck];
        std::vector<std::string> theCostVector = SplitString_VecString(theCostInfo,'=');
        std::stringstream buy_text;
        
        if(aPowerItemUnLocked == false){
            buy_text << "Unlock now for:";
        }
        else{
            buy_text << "Upgrade cost:";
        }
        
        // Set the texts
        aTxt_PriceAmount = CCLabelTTF::create(theCostVector[1].c_str(),"fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
        aTxt_PriceAmount->setAnchorPoint(ccp(0,0.5f));
        aTxt_Price = CCLabelTTF::create(buy_text.str().c_str(),"fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
        aTxt_Price->setAnchorPoint(ccp(0,0.5f));
        
        // Set the cost icon
        if(theCostVector[0].compare("D") == 0){
            // The diamonds
            aPriceIcon = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
        }
        else{
            // Crystal cost
            aPriceIcon = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
        }
        
        aPriceIcon->setAnchorPoint(ccp(0,0.5f));
        
        aBoxBase->addChild(aTxt_PriceAmount);
        aBoxBase->addChild(aTxt_Price);
        aBoxBase->addChild(aPriceIcon);
        
        // Set some tags for thease action memebers
        aTxt_PriceAmount->setTag(ITEM_PRICE_AMOUNT);
        aTxt_Price->setTag(ITEM_PRICE_TEXT);
        aPriceIcon->setTag(ITEM_PRICE_ICON);
        
        // Now align the elements
        aTxt_Price->setPosition(ccp(160,40));
        aTxt_PriceAmount->setPosition(ccp(aTxt_Price->getPositionX()+aTxt_Price->getTextureRect().size.width+10,40));
        aPriceIcon->setPosition(ccp(aTxt_PriceAmount->getPositionX()+aTxt_PriceAmount->getTextureRect().size.width+10,40));
        
        //------------------------------------------------------------------
        // The stars !!!
        if(aPowerItemUnLocked == true)
        {
            int aStartY = 125;
            // Check how far is unlocked !!!
            for(int s=0;s<User::getInstance()->getItemDataManager().mPowerDataVector[i].max_upgrades;s++)
            {
                // Whats the current upgrade level?
                if(s>=aPowerItemLevel){
                    aDummyStar = CCSprite::create("Shop/star_empty.png");
                }
                else{
                    aDummyStar = CCSprite::create("Shop/star_full.png");
                }
                
                aDummyStar->setScale(1.25f);
                
                aDummyStar->setPosition(ccp(aBoxBase->getContentSize().width/2-(aDummyStar->getContentSize().width*User::getInstance()->getItemDataManager().mPowerDataVector[i].max_upgrades)/2+(s*(aDummyStar->getContentSize().width+10)),aStartY));
                aDummyStar->setTag(UPGRADE_STAR+s);
                aBoxBase->addChild(aDummyStar);
            }
        }
        
        //------------------------------------------------------------------
        // Buttons?
        std::stringstream button_image_off;
        std::stringstream button_image_on;
        std::stringstream button_state;
        
        if(aPowerItemUnLocked == false)
        {
            // It's locked
            button_image_off << "Shop/Button_Unlock_1.png";
            button_image_on << "Shop/Button_Unlock_2.png";
            button_state << "Locked";
        }
        else
        {
            // Check if can upgrade more !!!
            button_image_off << "Shop/Button_Upgrade_1.png";
            button_image_on << "Shop/Button_Upgrade_2.png";
            button_state << "Upgrade";
        }
        
        MenuButton = CCMenuItemImage::create(button_image_off.str().c_str(),
                                             button_image_on.str().c_str(),
                                             this,
                                             menu_selector(StoreBox::OnPowerUpClick));
        MenuButton->setAnchorPoint(ccp(1,0));
        MenuButton->setTag(User::getInstance()->getItemDataManager().mPowerDataVector[i].id);
        MenuButton->setUserObject(CCString::create(button_state.str()));
        
        MenuButtons = CCMenu::create(MenuButton, NULL);
        MenuButtons->setTag(BUTTON_ACTION);// The menu button tah
        MenuButtons->setPosition(aBoxBase->getContentSize().width-30,10);
        aBoxBase->addChild(MenuButtons, 10);
        
        // The special stuff check
        if(aPowerItemLevel >= aMaxUpgrades){
            MenuButtons->setVisible(false);// Stop it - hide this button !!!
            // Hide the price stuff
            aPriceIcon->setVisible(false);
            aTxt_Price->setVisible(false);
            aTxt_PriceAmount->setVisible(false);
        }
        
        //------------------------------------------------------------------
        
    }
    
    //........................................................................
    // The next header
    
    aHeader = CCSprite::create("Shop/title_spells.png");
    aHeader->setPosition(ccp(aX,aY+66));
    mBase->addChild(aHeader,1);
    
    aY-=20;
    //    mMaxScroll_Shop_Y-=160;
    
    //........................................................................
    
    for(int i = 0;i<aTotalSpellTabItems;i++)
    {
        //------------------------------------------------------------------
        // The Base
        aBoxBase = CCSprite::create("Shop/panel_shop.png");
        aBoxBase->setPosition(ccp(aX,aY));
        aBoxBase->setTag(i+100);//The spell Taging
        mBase->addChild(aBoxBase);
        aY-=aBoxBase->getContentSize().height+aSpaceY;
        
        // Update how far can scroll
        //        if(i>5)
        //        {
        //            mMaxScroll_Shop_Y+=aBoxBase->getContentSize().height;
        //        }
        if(aY<=0) mMaxScroll_Shop_Y+=aBoxBase->getContentSize().height;
        
        //------------------------------------------------------------------
        // The Header
        
        aTxtHeader = CCLabelTTF::create(User::getInstance()->getItemDataManager().mSpellDataVector[i].name.c_str(),
                                        "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6,CCSize(420,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
        aTxtHeader->setPosition(ccp(aBoxBase->getContentSize().width/2-aGlobalMidOffestX,aBoxBase->getContentSize().height-24));
        aTxtHeader->setColor(ccc3(84, 71, 52));
        aBoxBase->addChild(aTxtHeader);
        
        //------------------------------------------------------------------
        // The Icon
        
        aIcon = CCSprite::create(User::getInstance()->getItemDataManager().mSpellDataVector[i].icon_path.c_str());
        aIcon->setTag(BUTTON_ICON);
        aIcon->setPosition(ccp(50+aIcon->getContentSize().width/2,aBoxBase->getContentSize().height/2));
        aBoxBase->addChild(aIcon);
        
        //------------------------------------------------------------------
        // The Stats
        
        aIconDamage = CCSprite::create("Shop/Icon_0002_Sword.png");
        aIconDamage->setAnchorPoint(ccp(0,0.5f));
        aIconRange = CCSprite::create("Shop/Icon_0001_Range.png");
        aIconRange->setAnchorPoint(ccp(0,0.5f));
        
        std::stringstream aDamageTxt;
        aDamageTxt << User::getInstance()->getItemDataManager().mSpellDataVector[i].damage;
        if(User::getInstance()->getItemDataManager().mSpellDataVector[i].damage_extra>0)
        {
            aDamageTxt << " + " <<User::getInstance()->getItemDataManager().mSpellDataVector[i].damage_extra << " x " << User::getInstance()->getItemDataManager().mSpellDataVector[i].damage_extra_multiply;
        }
        
        aTxt_Damage = CCLabelTTF::create(aDamageTxt.str().c_str(),"fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6);
        aTxt_Damage->setHorizontalAlignment(kCCTextAlignmentLeft);
        aTxt_Damage->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxt_Damage->setAnchorPoint(ccp(0,0.5f));
        
        std::stringstream aRangeTxt;
        aRangeTxt << User::getInstance()->getItemDataManager().mSpellDataVector[i].range;
        
        aTxt_Range = CCLabelTTF::create(aRangeTxt.str().c_str(),"fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6);
        aTxt_Range->setHorizontalAlignment(kCCTextAlignmentLeft);
        aTxt_Range->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxt_Range->setAnchorPoint(ccp(0,0.5f));
        
        aBoxBase->addChild(aTxt_Damage);
        aBoxBase->addChild(aTxt_Range);
        aBoxBase->addChild(aIconDamage);
        aBoxBase->addChild(aIconRange);
        
        // Position all stuff dynamic by text and icon size !!!
        int iconText_Space = 10;
        int statsToStats_Space = 20;
        int statsY = 30;
        
        int aTotalStatsWidth = aIconDamage->getContentSize().width + iconText_Space + aTxt_Damage->getTextureRect().size.width + statsToStats_Space + aIconRange->getContentSize().width + iconText_Space + aTxt_Range->getTextureRect().size.width;
        
        int aStartStatsX = aBoxBase->getContentSize().width/2 - aTotalStatsWidth/2-aGlobalMidOffestX;
        
        aIconDamage->setPosition(ccp(aStartStatsX,statsY));
        aStartStatsX+=aIconDamage->getContentSize().width+iconText_Space;
        
        aTxt_Damage->setPosition(ccp(aStartStatsX,statsY));
        aStartStatsX+=aTxt_Damage->getTextureRect().size.width+statsToStats_Space;
        
        aIconRange->setPosition(ccp(aStartStatsX,statsY));
        aStartStatsX+=aIconRange->getContentSize().width+iconText_Space;
        
        aTxt_Range->setPosition(ccp(aStartStatsX,statsY));
        
        
        //------------------------------------------------------------------
        // Check if this item is bought/unlocked etc !!!
        
        std::stringstream button_image_off;
        std::stringstream button_image_on;
        std::stringstream button_state;
        
        if(User::getInstance()->getItemDataManager().isItemUnlocked(User::getInstance()->getItemDataManager().mSpellDataVector[i].id))
        {
            // Check if it is selected or not
            if(User::getInstance()->getItemDataManager().isItemActive(User::getInstance()->getItemDataManager().mSpellDataVector[i].id))
            {
                // Item is active
                button_image_off << "Shop/Button_Selected.png";
                button_image_on << "Shop/Button_Selected.png";
                button_state << "Selected";
            }
            else
            {
                button_image_off << "Shop/Button_Select.png";
                button_image_on << "Shop/Button_Select.png";
                button_state << "UnSelected";
            }
        }
        else
        {
            // The locked state buttons
            button_image_off << "Shop/Button_Unlock_1.png";
            button_image_on << "Shop/Button_Unlock_2.png";
            button_state << "Locked";
            
            // Do we need to add above icon?
            aLockIcon = CCSprite::create("Shop/locked.png");
            aLockIcon->setTag(20);
            aLockIcon->setPosition(ccp(aIcon->getContentSize().width/2,aIcon->getContentSize().height/2));
            aIcon->addChild(aLockIcon);
        }
        
        MenuButton = CCMenuItemImage::create(button_image_off.str().c_str(),
                                             button_image_on.str().c_str(),
                                             this,
                                             menu_selector(StoreBox::OnSpellClick));
        MenuButton->setAnchorPoint(ccp(1,1));
        MenuButton->setTag(User::getInstance()->getItemDataManager().mSpellDataVector[i].id);
        MenuButton->setUserObject(CCString::create(button_state.str()));
        
        MenuButtons = CCMenu::create(MenuButton, NULL);
        MenuButtons->setTag(BUTTON_ACTION);// The menu button tah
        MenuButtons->setPosition(aBoxBase->getContentSize().width-30,aBoxBase->getContentSize().height-10);
        aBoxBase->addChild(MenuButtons, 10);
        
        //------------------------------------------------------------------
        // The Price if not bought
        aTxt_Price = CCLabelTTF::create("Cost:","fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
        aTxt_Price->setHorizontalAlignment(kCCTextAlignmentRight);
        aTxt_Price->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxt_Price->setAnchorPoint(ccp(1,0.5f));
        aTxt_Price->setColor(ccc3(84, 71, 52));
        
        std::stringstream aPriceValue;
        // Check what price is this !!!
        if(User::getInstance()->getItemDataManager().mSpellDataVector[i].price_crystals>0)
        {
            aPriceValue << User::getInstance()->getItemDataManager().mSpellDataVector[i].price_crystals;
            
            aPriceIcon = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
            aPriceIcon->setAnchorPoint(ccp(1,0.5f));
        }
        else
        {
            aPriceValue << User::getInstance()->getItemDataManager().mSpellDataVector[i].price_diamonds;
            
            aPriceIcon = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
            aPriceIcon->setAnchorPoint(ccp(1,0.5f));
        }
        
        aTxt_PriceAmount = CCLabelTTF::create(aPriceValue.str().c_str(),"fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
        aTxt_PriceAmount->setHorizontalAlignment(kCCTextAlignmentRight);
        aTxt_PriceAmount->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxt_PriceAmount->setAnchorPoint(ccp(1,0.5f));
        
        aBoxBase->addChild(aTxt_PriceAmount);
        aBoxBase->addChild(aTxt_Price);
        aBoxBase->addChild(aPriceIcon);
        
        // Now align
        iconText_Space = 6;
        statsY = 30;
        
        aStartStatsX = aBoxBase->getContentSize().width - 30;
        
        aPriceIcon->setPosition(ccp(aStartStatsX,statsY));
        aStartStatsX-=aPriceIcon->getContentSize().width+iconText_Space;
        
        aTxt_PriceAmount->setPosition(ccp(aStartStatsX,statsY));
        aStartStatsX-=aTxt_PriceAmount->getTextureRect().size.width;
        
        aTxt_Price->setPosition(ccp(aStartStatsX,statsY));
        
        
    }
    
    // Set the max scroll down
    mMaxScroll_Shop_Y -= aScrollOffsetY;
    
    /*
    int aX = 20;
    int aY = 540;
    int aSpaceY = 10;
    int aIndex = 1;
    
    mMaxScrollY = 216;
    //------------------------------------------------------------
    
    CCSprite* aHeader = CCSprite::create("Interfeiss/store/FRESH/upgrades/title_machines.png");
    aHeader->setPosition(ccp(aX,aY));
    mBase->addChild(aHeader,1);
    
    aY-=aHeader->getContentSize().height+86;
    
    //Create the base with all the elements
    //1st is the electro stuff !!!
    
    CCSprite* aBoxBase = CCSprite::create("Interfeiss/store/FRESH/upgrades/panel_upgrades.png");
    aBoxBase->setPosition(ccp(aX,aY));
    aBoxBase->setTag(300);
    mBase->addChild(aBoxBase);
    
    //===============
    //The button
    mElectroButtonState = 1;//Unlock state
    if(User::getInstance()->getMachine_EnemiesLvl()>0)
        mElectroButtonState = 2;
    
    //The unlock
    CCMenuItemImage* electroUnlock = CCMenuItemImage::create(
                                                             "Interfeiss/upgrade_screen/unlocknow_btn0001.png",
                                                             "Interfeiss/upgrade_screen/unlocknow_btn0002.png",
                                                             this,
                                                             menu_selector(StoreBox::UnlockNow_Electro));
    CCMenu* electroMenuUnlock = CCMenu::create(electroUnlock, NULL);
    electroMenuUnlock->setPosition(aBoxBase->getContentSize().width-140,30);
    electroMenuUnlock->setVisible(false);
    electroMenuUnlock->setTag(311);
    aBoxBase->addChild(electroMenuUnlock, 10);
    
    //The upgrade
    CCMenuItemImage* electroUpgrade = CCMenuItemImage::create(
                                                              "Interfeiss/upgrade_screen/upgrade_btn0001.png",
                                                              "Interfeiss/upgrade_screen/upgrade_btn0002.png",
                                                              this,
                                                              menu_selector(StoreBox::UnlockNow_Electro));
    CCMenu* electroMenuUpgrade = CCMenu::create(electroUpgrade, NULL);
    electroMenuUpgrade->setPosition(aBoxBase->getContentSize().width-140,30);
    electroMenuUpgrade->setVisible(false);
    electroMenuUpgrade->setTag(312);
    aBoxBase->addChild(electroMenuUpgrade, 10);
    
    //The buy
    CCMenuItemImage* electroBuy = CCMenuItemImage::create(
                                                          "Interfeiss/upgrade_screen/buy_btn0001.png",
                                                          "Interfeiss/upgrade_screen/buy_btn0002.png",
                                                          this,
                                                          menu_selector(StoreBox::Buy_Electro));
    CCMenu* electroMenuBuy = CCMenu::create(electroBuy, NULL);
    electroMenuBuy->setPosition(aBoxBase->getContentSize().width-140,30);
    electroMenuBuy->setVisible(false);
    electroMenuBuy->setTag(313);
    aBoxBase->addChild(electroMenuBuy, 10);
    
    //===============
    
    //The star info
    //set some star info
    for(int i=0;i<5;i++)
    {
        CCSprite* aStartEmpty = CCSprite::create("Interfeiss/store/FRESH/upgrades/star_empty.png");
        aStartEmpty->setPosition(ccp(274+i*34,122));
        aStartEmpty->setTag(320+i);
        aStartEmpty->setVisible(false);
        aBoxBase->addChild(aStartEmpty,10);
        
        //Check what star amount is visible
        CCSprite* aStartFull = CCSprite::create("Interfeiss/store/FRESH/upgrades/star_full.png");
        aStartFull->setPosition(ccp(274+i*34,122));
        aStartFull->setTag(330+i);
        aStartFull->setVisible(false);
        aBoxBase->addChild(aStartFull,10);
    }
    
    //The text fields
    //The text part !!!
    CCLabelTTF* aTxtElectroHeader = CCLabelTTF::create("Elec-tree", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.65, CCSize(200, 120), kCCTextAlignmentCenter);
    aTxtElectroHeader->setPosition(ccp(aBoxBase->getContentSize().width/2,aBoxBase->getContentSize().height-72));
    aTxtElectroHeader->setColor(ccc3(84, 71, 52));
    aBoxBase->addChild(aTxtElectroHeader);
    
    //Add some sprites !!!
    CCSprite* aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aSideHeader->setPosition(ccp(aBoxBase->getContentSize().width/2-120,aBoxBase->getContentSize().height-30));
    aBoxBase->addChild(aSideHeader);
    
    aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aSideHeader->setFlipX(true);
    aSideHeader->setPosition(ccp(aBoxBase->getContentSize().width/2+120,aBoxBase->getContentSize().height-30));
    aBoxBase->addChild(aSideHeader);
    
    //It contains only important gameplay strategy secrets. It is full of thestg ea 3252
    CCLabelTTF* aTxtElectroDesc = CCLabelTTF::create("This text is ultra smart It contains only important gameplay strategy secrets. It is full of thestg ea 3252",
                                                     "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.42, CCSize(500, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
//    aTxtElectroDesc->setPosition(ccp(aBoxBase->getContentSize().width/2,aBoxBase->getContentSize().height-96));
    aTxtElectroDesc->setAnchorPoint(ccp(0,0));
    aTxtElectroDesc->setPosition(ccp(160,60));
    aTxtElectroDesc->setColor(ccc3(82, 44, 19));
//    aTxtElectroDesc->setVerticalAlignment(kCCVerticalTextAlignmentTop);//If locked alling to top
//    aTxtElectroDesc->setHorizontalAlignment(kCCTextAlignmentLeft);
    aTxtElectroDesc->setTag(363);
    aBoxBase->addChild(aTxtElectroDesc);
    
    //Checks by state
    CCLabelTTF* aTxtElectroCost = CCLabelTTF::create("Upgrade cost:","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.4, CCSize(180, 35), kCCTextAlignmentLeft);
    aTxtElectroCost->setPosition(ccp(aBoxBase->getContentSize().width/2-80,30));
    aTxtElectroCost->setColor(ccc3(82, 44, 19));
    aTxtElectroCost->setTag(364);
    aBoxBase->addChild(aTxtElectroCost);
    
    //The price
    CCLabelTTF* aTxtElectroCostPrice = CCLabelTTF::create("5000","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.45, CCSize(85, 50), kCCTextAlignmentCenter);
    aTxtElectroCostPrice->setPosition(ccp(aBoxBase->getContentSize().width/2-10,23));
    aTxtElectroCostPrice->setColor(ccWHITE);
    aTxtElectroCostPrice->enableShadow(CCSize(1,-1), 1.5f, 2,true);
    aTxtElectroCostPrice->setTag(365);
    aBoxBase->addChild(aTxtElectroCostPrice);
    
    //And the crystal and dimand for price
    CCSprite* aElectroCrystal = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
    aElectroCrystal->setPosition(ccp(aBoxBase->getContentSize().width/2+55,33));//This is the upgrade pos
    aElectroCrystal->setTag(350);
    aElectroCrystal->setVisible(false);
    aBoxBase->addChild(aElectroCrystal,10);
    
    CCSprite* aElectroDiamond = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
    aElectroDiamond->setPosition(ccp(aBoxBase->getContentSize().width/2+55,33));//This is the upgrade pos
    aElectroDiamond->setTag(351);
    aElectroDiamond->setVisible(true);
    aBoxBase->addChild(aElectroDiamond,10);
    
    ChangeButtonState(3,mElectroButtonState);
    CreateMachineByLevel(3,User::getInstance()->getMachine_EnemiesLvl(),true);//Get from user stuff
    
    aIndex++;
    aY-=aBoxBase->getContentSize().height+aSpaceY;
    
    //------------------------------------------------------------
    
    aBoxBase = CCSprite::create("Interfeiss/store/FRESH/upgrades/panel_upgrades.png");
    aBoxBase->setPosition(ccp(aX,aY));
    aBoxBase->setTag(200);
    mBase->addChild(aBoxBase);
    
    //The ghost
    mGhostButtonState = 1;//Unlock state
    if(User::getInstance()->getMachine_GhostLvl()>0)
        mGhostButtonState = 2;
    
    //The unlock
    CCMenuItemImage* ghostUnlock = CCMenuItemImage::create(
                                                           "Interfeiss/upgrade_screen/unlocknow_btn0001.png",
                                                           "Interfeiss/upgrade_screen/unlocknow_btn0002.png",
                                                           this,
                                                           menu_selector(StoreBox::UnlockNow_Ghost));
    CCMenu* ghostMenuUnlock = CCMenu::create(ghostUnlock, NULL);
    ghostMenuUnlock->setPosition(aBoxBase->getContentSize().width-140,30);
    ghostMenuUnlock->setVisible(false);
    ghostMenuUnlock->setTag(211);
    aBoxBase->addChild(ghostMenuUnlock, 10);
    
    //The upgrade
    CCMenuItemImage* ghostUpgrade = CCMenuItemImage::create(
                                                            "Interfeiss/upgrade_screen/upgrade_btn0001.png",
                                                            "Interfeiss/upgrade_screen/upgrade_btn0002.png",
                                                            this,
                                                            menu_selector(StoreBox::UnlockNow_Ghost));
    CCMenu* ghostMenuUpgrade = CCMenu::create(ghostUpgrade, NULL);
    ghostMenuUpgrade->setPosition(aBoxBase->getContentSize().width-140,30);
    ghostMenuUpgrade->setVisible(false);
    ghostMenuUpgrade->setTag(212);
    aBoxBase->addChild(ghostMenuUpgrade, 10);
    
    //The buy
    CCMenuItemImage* ghostBuy = CCMenuItemImage::create(
                                                        "Interfeiss/upgrade_screen/buy_btn0001.png",
                                                        "Interfeiss/upgrade_screen/buy_btn0002.png",
                                                        this,
                                                        menu_selector(StoreBox::Buy_Ghost));
    CCMenu* ghostMenuBuy = CCMenu::create(ghostBuy, NULL);
    ghostMenuBuy->setPosition(aBoxBase->getContentSize().width-140,30);
    ghostMenuBuy->setVisible(false);
    ghostMenuBuy->setTag(213);
    aBoxBase->addChild(ghostMenuBuy, 10);
    
    //set some star info
    for(int i=0;i<5;i++)
    {
        CCSprite* aStartEmpty = CCSprite::create("Interfeiss/upgrade_screen/star_empty.png");
        aStartEmpty->setPosition(ccp(274+i*34,122));
        aStartEmpty->setTag(220+i);
        aStartEmpty->setVisible(false);
        aBoxBase->addChild(aStartEmpty,10);
        
        //Check what star amount is visible
        CCSprite* aStartFull = CCSprite::create("Interfeiss/upgrade_screen/star_gold.png");
        aStartFull->setPosition(ccp(274+i*34,122));
        aStartFull->setTag(230+i);
        aStartFull->setVisible(false);
        aBoxBase->addChild(aStartFull,10);
    }
    
    //The text part !!!
    CCLabelTTF* aTxtGhostHeader = CCLabelTTF::create("Ghost Stump", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.65, CCSize(200, 120), kCCTextAlignmentCenter);
    aTxtGhostHeader->setPosition(ccp(aBoxBase->getContentSize().width/2,aBoxBase->getContentSize().height-72));
    aTxtGhostHeader->setColor(ccc3(84, 71, 52));
    aBoxBase->addChild(aTxtGhostHeader);
    
    //Add some sprites !!!
    aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aSideHeader->setPosition(ccp(aBoxBase->getContentSize().width/2-120,aBoxBase->getContentSize().height-30));
    aBoxBase->addChild(aSideHeader);
    
    aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aSideHeader->setFlipX(true);
    aSideHeader->setPosition(ccp(aBoxBase->getContentSize().width/2+120,aBoxBase->getContentSize().height-30));
    aBoxBase->addChild(aSideHeader);
    
    CCLabelTTF* aTxtGhostDesc = CCLabelTTF::create("This text is ultra smart. It contains only important gameplay strategy secrets.",
                                                   "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.42, CCSize(500, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
    aTxtGhostDesc->setAnchorPoint(ccp(0,0));
    aTxtGhostDesc->setPosition(ccp(160,60));
    aTxtGhostDesc->setColor(ccc3(82, 44, 19));
    aTxtGhostDesc->setTag(263);
    aBoxBase->addChild(aTxtGhostDesc);
    
    //Checks by state
    CCLabelTTF* aTxtGhostCost = CCLabelTTF::create("Upgrade cost:","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.4, CCSize(180, 35), kCCTextAlignmentLeft);
    aTxtGhostCost->setPosition(ccp(aBoxBase->getContentSize().width/2-80,30));
    aTxtGhostCost->setColor(ccc3(82, 44, 19));
    aTxtGhostCost->setTag(264);
    aBoxBase->addChild(aTxtGhostCost);
    
    //The price
    CCLabelTTF* aTxtGhostCostPrice = CCLabelTTF::create("50000","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.45, CCSize(85, 50), kCCTextAlignmentCenter);
    aTxtGhostCostPrice->setPosition(ccp(aBoxBase->getContentSize().width/2-10,23));
    aTxtGhostCostPrice->setColor(ccWHITE);
    aTxtGhostCostPrice->enableShadow(CCSize(1,-1), 1.5f, 2,true);
    aTxtGhostCostPrice->setTag(265);
    aBoxBase->addChild(aTxtGhostCostPrice);
    
    //And the crystal and dimand for price
    CCSprite* aGhostCrystal = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
    aGhostCrystal->setPosition(ccp(aBoxBase->getContentSize().width/2+55,33));//This is the upgrade pos
    aGhostCrystal->setTag(250);
    aGhostCrystal->setVisible(false);
    aBoxBase->addChild(aGhostCrystal,10);
    
    CCSprite* aGhostDiamond = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
    aGhostDiamond->setPosition(ccp(aBoxBase->getContentSize().width/2+55,33));//This is the upgrade pos
    aGhostDiamond->setTag(251);
    aBoxBase->addChild(aGhostDiamond,10);
    
    //Set the states
    ChangeButtonState(2,mGhostButtonState);
    CreateMachineByLevel(2,User::getInstance()->getMachine_GhostLvl(),true);//Get from user stuff
    
    aIndex++;
    aY-=aBoxBase->getContentSize().height+aSpaceY;
    
    //------------------------------------------------------------
    
    aBoxBase = CCSprite::create("Interfeiss/store/FRESH/upgrades/panel_upgrades.png");
    aBoxBase->setPosition(ccp(aX,aY));
    aBoxBase->setTag(400);
    mBase->addChild(aBoxBase);
    
    mStampButtonState = 1;//Unlock state
    if(User::getInstance()->getMachine_StampLvl()>0)
        mStampButtonState = 2;
    
    //The unlock
    CCMenuItemImage* stampUnlock = CCMenuItemImage::create(
                                                           "Interfeiss/upgrade_screen/unlocknow_btn0001.png",
                                                           "Interfeiss/upgrade_screen/unlocknow_btn0002.png",
                                                           this,
                                                           menu_selector(StoreBox::UnlockNow_Stamp));
    CCMenu* stampMenuUnlock = CCMenu::create(stampUnlock, NULL);
    stampMenuUnlock->setPosition(aBoxBase->getContentSize().width-140,30);
    stampMenuUnlock->setVisible(false);
    stampMenuUnlock->setTag(411);
    aBoxBase->addChild(stampMenuUnlock, 10);
    
    //The upgrade
    CCMenuItemImage* stampUpgrade = CCMenuItemImage::create(
                                                            "Interfeiss/upgrade_screen/upgrade_btn0001.png",
                                                            "Interfeiss/upgrade_screen/upgrade_btn0002.png",
                                                            this,
                                                            menu_selector(StoreBox::UnlockNow_Stamp));
    CCMenu* stampMenuUpgrade = CCMenu::create(stampUpgrade, NULL);
    stampMenuUpgrade->setPosition(aBoxBase->getContentSize().width-140,30);
    stampMenuUpgrade->setVisible(false);
    stampMenuUpgrade->setTag(412);
    aBoxBase->addChild(stampMenuUpgrade, 10);
    
    //The buy
    CCMenuItemImage* stampBuy = CCMenuItemImage::create(
                                                        "Interfeiss/upgrade_screen/buy_btn0001.png",
                                                        "Interfeiss/upgrade_screen/buy_btn0002.png",
                                                        this,
                                                        menu_selector(StoreBox::Buy_Stamp));
    CCMenu* stampMenuBuy = CCMenu::create(stampBuy, NULL);
    stampMenuBuy->setPosition(aBoxBase->getContentSize().width-140,30);
    stampMenuBuy->setVisible(false);
    stampMenuBuy->setTag(413);
    aBoxBase->addChild(stampMenuBuy, 10);
    
    //set some star info
    for(int i=0;i<5;i++)
    {
        CCSprite* aStartEmpty = CCSprite::create("Interfeiss/upgrade_screen/star_empty.png");
        aStartEmpty->setPosition(ccp(274+i*34,122));
        aStartEmpty->setTag(420+i);
        aStartEmpty->setVisible(false);
        aBoxBase->addChild(aStartEmpty,10);
        
        //Check what star amount is visible
        CCSprite* aStartFull = CCSprite::create("Interfeiss/upgrade_screen/star_gold.png");
        aStartFull->setPosition(ccp(274+i*34,122));
        aStartFull->setTag(430+i);
        aStartFull->setVisible(false);
        aBoxBase->addChild(aStartFull,10);
    }
    
    //The text part !!!
    CCLabelTTF* aTxtStampHeader = CCLabelTTF::create("Smasher", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.65, CCSize(200, 120), kCCTextAlignmentCenter);
    aTxtStampHeader->setPosition(ccp(aBoxBase->getContentSize().width/2,aBoxBase->getContentSize().height-72));
    aTxtStampHeader->setColor(ccc3(84, 71, 52));
    aBoxBase->addChild(aTxtStampHeader);
    
    //Add some sprites !!!
    aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aSideHeader->setPosition(ccp(aBoxBase->getContentSize().width/2-120,aBoxBase->getContentSize().height-30));
    aBoxBase->addChild(aSideHeader);
    
    aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aSideHeader->setFlipX(true);
    aSideHeader->setPosition(ccp(aBoxBase->getContentSize().width/2+120,aBoxBase->getContentSize().height-30));
    aBoxBase->addChild(aSideHeader);
    
    CCLabelTTF* aTxtStampDesc = CCLabelTTF::create("This text is ultra smart. It contains only important gameplay strategy secrets.",
                                                   "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.42, CCSize(500, 50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
    aTxtStampDesc->setAnchorPoint(ccp(0,0));
    aTxtStampDesc->setPosition(ccp(160,60));
    aTxtStampDesc->setColor(ccc3(82, 44, 19));
    aTxtStampDesc->setTag(463);
    aBoxBase->addChild(aTxtStampDesc);
    
    //Checks by state
    CCLabelTTF* aTxtStampCost = CCLabelTTF::create("Upgrade cost:","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.4, CCSize(180, 35), kCCTextAlignmentLeft);
    aTxtStampCost->setPosition(ccp(aBoxBase->getContentSize().width/2-80,30));
    aTxtStampCost->setColor(ccc3(82, 44, 19));
    aTxtStampCost->setTag(464);
    aBoxBase->addChild(aTxtStampCost);
    
    //The price
    CCLabelTTF* aTxtStampCostPrice = CCLabelTTF::create("50000","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.45, CCSize(85, 50), kCCTextAlignmentCenter);
    aTxtStampCostPrice->setPosition(ccp(aBoxBase->getContentSize().width/2-10,23));
    aTxtStampCostPrice->setColor(ccWHITE);
    aTxtStampCostPrice->enableShadow(CCSize(1,-1), 1.5f, 2,true);
    aTxtStampCostPrice->setTag(465);
    aBoxBase->addChild(aTxtStampCostPrice);
    
    //And the crystal and dimand for price
    CCSprite* aStampCrystal = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
    aStampCrystal->setPosition(ccp(aBoxBase->getContentSize().width/2+55,33));//This is the upgrade pos
    aStampCrystal->setTag(450);
    aStampCrystal->setVisible(false);
    aBoxBase->addChild(aStampCrystal,10);
    
    CCSprite* aStampDiamond = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
    aStampDiamond->setPosition(ccp(aBoxBase->getContentSize().width/2+55,33));//This is the upgrade pos
    aStampDiamond->setTag(451);
    aBoxBase->addChild(aStampDiamond,10);
    
    //Set the states
    ChangeButtonState(4,mStampButtonState);
    CreateMachineByLevel(4,User::getInstance()->getMachine_StampLvl(),true);//Get from user stuff
    
    aIndex++;
    aY-=aBoxBase->getContentSize().height+aSpaceY;
    
    //------------------------------------------------------------
    
    
    aY+=aBoxBase->getContentSize().height/2-10;
    
    aHeader = CCSprite::create("Interfeiss/store/FRESH/upgrades/title_plants.png");
    aHeader->setPosition(ccp(aX,aY));
    mBase->addChild(aHeader,1);
    
    aY-=aHeader->getContentSize().height+86;
    
    aBoxBase = CCSprite::create("Interfeiss/store/FRESH/upgrades/panel_upgrades.png");
    aBoxBase->setPosition(ccp(aX,aY));
    aBoxBase->setTag(100);
    mBase->addChild(aBoxBase);
    
    mPlantButtonState = 1;//Unlock state
    if(User::getInstance()->getMachine_PlantLvl()>0)
        mPlantButtonState = 2;
    
    //The unlock
    CCMenuItemImage* plantUnlock = CCMenuItemImage::create(
                                                           "Interfeiss/upgrade_screen/unlocknow_btn0001.png",
                                                           "Interfeiss/upgrade_screen/unlocknow_btn0002.png",
                                                           this,
                                                           menu_selector(StoreBox::UnlockNow_Plant));
    CCMenu* plantMenuUnlock = CCMenu::create(plantUnlock, NULL);
    plantMenuUnlock->setPosition(aBoxBase->getContentSize().width-140,30);
    plantMenuUnlock->setVisible(false);
    plantMenuUnlock->setTag(111);
    aBoxBase->addChild(plantMenuUnlock, 10);
    
    //The upgrade
    CCMenuItemImage* plantUpgrade = CCMenuItemImage::create(
                                                            "Interfeiss/upgrade_screen/upgrade_btn0001.png",
                                                            "Interfeiss/upgrade_screen/upgrade_btn0002.png",
                                                            this,
                                                            menu_selector(StoreBox::UnlockNow_Plant));
    CCMenu* plantMenuUpgrade = CCMenu::create(plantUpgrade, NULL);
    plantMenuUpgrade->setPosition(aBoxBase->getContentSize().width-140,30);
    plantMenuUpgrade->setVisible(false);
    plantMenuUpgrade->setTag(112);
    aBoxBase->addChild(plantMenuUpgrade, 10);
    
    //The buy
    CCMenuItemImage* plantBuy = CCMenuItemImage::create(
                                                        "Interfeiss/upgrade_screen/buy_btn0001.png",
                                                        "Interfeiss/upgrade_screen/buy_btn0002.png",
                                                        this,
                                                        menu_selector(StoreBox::Buy_Plant));
    CCMenu* plantMenuBuy = CCMenu::create(plantBuy, NULL);
    plantMenuBuy->setPosition(aBoxBase->getContentSize().width-140,30);
    plantMenuBuy->setVisible(false);
    plantMenuBuy->setTag(113);
    aBoxBase->addChild(plantMenuBuy, 10);
    
    //set some star info
    for(int i=0;i<5;i++)
    {
        CCSprite* aStartEmpty = CCSprite::create("Interfeiss/upgrade_screen/star_empty.png");
        aStartEmpty->setPosition(ccp(274+i*34,122));
        aStartEmpty->setTag(120+i);
        aStartEmpty->setVisible(false);
        aBoxBase->addChild(aStartEmpty,10);
        
        //Check what star amount is visible
        CCSprite* aStartFull = CCSprite::create("Interfeiss/upgrade_screen/star_gold.png");
        aStartFull->setPosition(ccp(274+i*34,122));
        aStartFull->setTag(130+i);
        aStartFull->setVisible(false);
        aBoxBase->addChild(aStartFull,10);
    }
    
    //The text part !!!
    CCLabelTTF* aTxtPlantHeader = CCLabelTTF::create("Crystal Plant", "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.65, CCSize(200, 120), kCCTextAlignmentCenter);
    aTxtPlantHeader->setPosition(ccp(aBoxBase->getContentSize().width/2,aBoxBase->getContentSize().height-72));
    aTxtPlantHeader->setColor(ccc3(84, 71, 52));
    aBoxBase->addChild(aTxtPlantHeader);
    
    //Add some sprites !!!
    aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aSideHeader->setPosition(ccp(aBoxBase->getContentSize().width/2-120,aBoxBase->getContentSize().height-30));
    aBoxBase->addChild(aSideHeader);
    
    aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
    aSideHeader->setFlipX(true);
    aSideHeader->setPosition(ccp(aBoxBase->getContentSize().width/2+120,aBoxBase->getContentSize().height-30));
    aBoxBase->addChild(aSideHeader);
    
    CCLabelTTF* aTxtPlantDesc = CCLabelTTF::create("This text is ultra smart. It contains only important gameplay strategy secrets.",
                                                   "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.42, CCSize(500, 50), kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
    aTxtPlantDesc->setAnchorPoint(ccp(0,0));
    aTxtPlantDesc->setPosition(ccp(160,60));
    aTxtPlantDesc->setColor(ccc3(82, 44, 19));
    aTxtPlantDesc->setTag(163);
    aBoxBase->addChild(aTxtPlantDesc);
    
    //Checks by state
    CCLabelTTF* aTxtPlantCost = CCLabelTTF::create("Upgrade cost:","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.4, CCSize(185, 30), kCCTextAlignmentLeft);
    aTxtPlantCost->setPosition(ccp(aBoxBase->getContentSize().width/2-80,30));
    aTxtPlantCost->setColor(ccc3(82, 44, 19));
    aTxtPlantCost->setTag(164);
    aBoxBase->addChild(aTxtPlantCost);
    
    //The price
    CCLabelTTF* aTxtPlantCostPrice = CCLabelTTF::create("50000","fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.45, CCSize(80, 50), kCCTextAlignmentCenter);
    aTxtPlantCostPrice->setPosition(ccp(aBoxBase->getContentSize().width/2-10,23));
    aTxtPlantCostPrice->setColor(ccWHITE);
    aTxtPlantCostPrice->enableShadow(CCSize(1,-1), 1.5f, 2,true);
    aTxtPlantCostPrice->setTag(165);
    aBoxBase->addChild(aTxtPlantCostPrice);
    
    //And the crystal and dimand for price
    CCSprite* aPlantCrystal = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
    aPlantCrystal->setPosition(ccp(aBoxBase->getContentSize().width/2+55,33));//This is the upgrade pos
    aPlantCrystal->setTag(150);
    aPlantCrystal->setVisible(false);
    aBoxBase->addChild(aPlantCrystal,10);
    
    CCSprite* aPlantDiamond = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
    aPlantDiamond->setPosition(ccp(aBoxBase->getContentSize().width/2+55,33));//This is the upgrade pos
    aPlantDiamond->setTag(151);
    aBoxBase->addChild(aPlantDiamond,10);
    
    //Set the states
    ChangeButtonState(1,mPlantButtonState);
    CreateMachineByLevel(1,User::getInstance()->getMachine_PlantLvl(),true);//Get from user stuff
    
    aIndex++;
    */
//    aY-=aBoxBase->getContentSize().height+aSpaceY;
    
    //------------------------------------------------------------
}

//The other stuff
void StoreBox::UnlockNow_Stamp(cocos2d::CCObject *sender)
{
    AnalyticX::flurryLogEvent("Menu Upgrades: Unlock Stamp");
    //Flips button to buy or unlock
    ChangeButtonState(4,3);//Buy to unlock
}

void StoreBox::Buy_Stamp(cocos2d::CCObject *sender)
{
    /*
    AnalyticX::flurryLogEvent("Menu Upgrades: Buy Stamp");
    
    //Try to buy it !!!
    int aLevel = User::getInstance()->getMachine_StampLvl();
    int aPrice = 50;
    
    if(aLevel == 0)
    {
        aPrice = 150;
        //We want to unlock it !!!
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        //TODO
        //        _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
        
        User::getInstance()->setMachine_StampLvl(1);
        CreateMachineByLevel(4,1);
    }
    else
    {
        if(aLevel == 1)
            aPrice = 2000;
        else if(aLevel == 2)
            aPrice = 8000;
        else if(aLevel == 3)
            aPrice = 20000;
        else if(aLevel == 4)
            aPrice = 50000;
        
        //We just want to upgrade it !!!
        //        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        int aDidUseDiamonds = User::getInstance()->canUseCrystals(aPrice);
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        if(aLevel<5)
        {
            CreateMachineByLevel(4,aLevel+1,false);//Get from user stuff
            User::getInstance()->addMachine_StampLvl(1);
        }
    }
    */
    
    UpdateStats();
    
    //Flips button to buy or unlock
    ChangeButtonState(4,2);//Upgrades enabled or shows next upgrade s
}

void StoreBox::UnlockNow_Ghost(cocos2d::CCObject *sender)
{
    AnalyticX::flurryLogEvent("Menu Upgrades: Unlock Ghost");
    //Flips button to buy or unlock
    ChangeButtonState(2,3);//Buy to unlock
}

void StoreBox::Buy_Ghost(cocos2d::CCObject *sender)
{
    /*
    AnalyticX::flurryLogEvent("Menu Upgrades: Buy Ghost");
    //Try to buy it !!!
    int aLevel = User::getInstance()->getMachine_GhostLvl();
    int aPrice = 50;
    
    if(aLevel == 0)
    {
        aPrice = 100;
        //We want to unlock it !!!
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        //TODO
        //        _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
        
        User::getInstance()->setMachine_GhostLvl(1);
        CreateMachineByLevel(2,1);
    }
    else
    {
        if(aLevel == 1)
            aPrice = 2000;
        else if(aLevel == 2)
            aPrice = 8000;
        else if(aLevel == 3)
            aPrice = 20000;
        else if(aLevel == 4)
            aPrice = 50000;
        
        //We just want to upgrade it !!!
        int aDidUseDiamonds = User::getInstance()->canUseCrystals(aPrice);
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        if(aLevel<5)
        {
            CreateMachineByLevel(2,aLevel+1,false);//Get from user stuff
            User::getInstance()->addMachine_GhostLvl(1);
        }
    }
    */
    
    UpdateStats();
    
    //Flips button to buy or unlock
    ChangeButtonState(2,2);//Upgrades enabled or shows next upgrade s
}


void StoreBox::UnlockNow_Electro(cocos2d::CCObject *sender)
{
    AnalyticX::flurryLogEvent("Menu Upgrades: Unlock Blitz");
    
    //Flips button to buy or unlock
    ChangeButtonState(3,3);//Buy to unlock
}

void StoreBox::Buy_Electro(cocos2d::CCObject *sender)
{
    AnalyticX::flurryLogEvent("Menu Upgrades: Buy Blitz");
    
    /*
    //Try to buy it !!!
    int aLevel = User::getInstance()->getMachine_EnemiesLvl();
    int aPrice = 35;
    
    if(aLevel == 0)
    {
        //We want to unlock it !!!
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        //TODO
        //        _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
        
        User::getInstance()->setMachine_EnemiesLvl(1);
        CreateMachineByLevel(3,1);
    }
    else
    {
        if(aLevel == 1)
            aPrice = 2000;
        else if(aLevel == 2)
            aPrice = 8000;
        else if(aLevel == 3)
            aPrice = 20000;
        else if(aLevel == 4)
            aPrice = 50000;
        
        //We just want to upgrade it !!!
        int aDidUseDiamonds = User::getInstance()->canUseCrystals(aPrice);
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        if(aLevel<5)
        {
            CreateMachineByLevel(3,aLevel+1,false);//Get from user stuff
            User::getInstance()->addMachine_EnemiesLvl(1);
        }
    }
    */
    
    UpdateStats();
    
    //Flips button to buy or unlock
    ChangeButtonState(3,2);//Upgrades enabled or shows next upgrade s
}

void StoreBox::UnlockNow_Plant(cocos2d::CCObject *sender)
{
    AnalyticX::flurryLogEvent("Menu Upgrades: Unlock Plants");
    
    //Flips button to buy or unlock
    ChangeButtonState(1,3);//Buy to unlock
}

void StoreBox::Buy_Plant(cocos2d::CCObject *sender)
{
    AnalyticX::flurryLogEvent("Menu Upgrades: Buy Plants");
    
    /*
    //Try to buy it !!!
    int aLevel = User::getInstance()->getMachine_PlantLvl();
    int aPrice = 35;
    
    if(aLevel == 0)
    {
        aPrice = 75;
        //We want to unlock it !!!
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        User::getInstance()->setMachine_PlantLvl(1);
        CreateMachineByLevel(1,1);
    }
    else
    {
        if(aLevel == 1)
            aPrice = 2000;
        else if(aLevel == 2)
            aPrice = 8000;
        else if(aLevel == 3)
            aPrice = 20000;
        else if(aLevel == 4)
            aPrice = 50000;
        
        //We just want to upgrade it !!!
        int aDidUseDiamonds = User::getInstance()->canUseCrystals(aPrice);
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        if(aLevel<5)
        {
            CreateMachineByLevel(1,aLevel+1,false);//Get from user stuff
            User::getInstance()->addMachine_PlantLvl(1);
        }
    }
    */
    
    UpdateStats();
    
    //Flips button to buy or unlock
    ChangeButtonState(1,2);//Upgrades enabled or shows next upgrade s
}

void StoreBox::ChangeButtonState(int theButton,int theState)
{
    /*
    CCMenu* aMenuButton = NULL;
    CCTextFieldTTF* aTextField = NULL;
    CCSprite* aBG = NULL;
    CCSprite* aDummySprite = NULL;
    
    bool aLocked = true;
    int aLevel = 0;
    int aPrice = 0;
    
    std::stringstream thePriceString;
    
    std::stringstream theDesc;
    std::stringstream theUnlockLevel;
    
    if(theButton == 1)
    {
        aBG = static_cast<CCSprite*>(mBase->getChildByTag(100));//Get the old one
        
        //static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(theType));
        aMenuButton = static_cast<CCMenu*>(aBG->getChildByTag(110+mPlantButtonState));//Get the old one
        aMenuButton->setVisible(false);
        
        aMenuButton = static_cast<CCMenu*>(aBG->getChildByTag(110+theState));
        aMenuButton->setVisible(true);
        
        //Update txt and other stuff
        aLevel = User::getInstance()->getMachine_PlantLvl();
        if(aLevel > 0)
        {
            aLocked = false;
            //GET THE CORRECT PRICE
            if(aLevel == 1)
                aPrice = 2000;
            else if(aLevel == 2)
                aPrice = 8000;
            else if(aLevel == 3)
                aPrice = 20000;
            else if(aLevel == 4)
                aPrice = 50000;
            else
                aPrice = -1;//Hide it ???
            
            theDesc << "Upgrading the plant will give you more shiny crystals";
            theUnlockLevel << "A plant that produces shiny crystals";
        }
        else
        {
            aPrice = 75;
            theUnlockLevel << "Unlock from Mission Set #5";
            theDesc << "A plant that produces shiny crystals";
        }
        
        thePriceString << aPrice;
        
        mPlantButtonState = theState;//set the new state
    }
    else if(theButton == 2)
    {
        aBG = static_cast<CCSprite*>(mBase->getChildByTag(200));//Get the old one
        
        //static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(theType));
        aMenuButton = static_cast<CCMenu*>(aBG->getChildByTag(210+mGhostButtonState));//Get the old one
        aMenuButton->setVisible(false);
        
        aMenuButton = static_cast<CCMenu*>(aBG->getChildByTag(210+theState));
        aMenuButton->setVisible(true);
        
        //Update txt and other stuff
        aLevel = User::getInstance()->getMachine_GhostLvl();
        if(aLevel > 0)
        {
            aLocked = false;
            //GET THE CORRECT PRICE
            if(aLevel == 1)
                aPrice = 2000;
            else if(aLevel == 2)
                aPrice = 8000;
            else if(aLevel == 3)
                aPrice = 20000;
            else if(aLevel == 4)
                aPrice = 50000;
            else
                aPrice = -1;//Hide it ???
            
            theDesc << "Improves effectiveness of the dwarf machine";
            theUnlockLevel << "With the help of forest magic dwarfs gain ability to go through one another";
        }
        else
        {
            aPrice = 100;
            theUnlockLevel << "Unlock from Mission Set #8";
            theDesc << "With the help of forest magic dwarfs gain ability to go through one another";
        }
        
        thePriceString << aPrice;
        
        mGhostButtonState = theState;//set the new state
    }
    else if(theButton == 3)
    {
        aBG = static_cast<CCSprite*>(mBase->getChildByTag(300));//Get the old one
        
        //static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(theType));
        aMenuButton = static_cast<CCMenu*>(aBG->getChildByTag(310+mElectroButtonState));//Get the old one
        aMenuButton->setVisible(false);
        
        aMenuButton = static_cast<CCMenu*>(aBG->getChildByTag(310+theState));
        aMenuButton->setVisible(true);
        
        //Update txt and other stuff
        aLevel = User::getInstance()->getMachine_EnemiesLvl();
        if(aLevel > 0)
        {
            aLocked = false;
            //GET THE CORRECT PRICE
            if(aLevel == 1)
                aPrice = 2000;
            else if(aLevel == 2)
                aPrice = 8000;
            else if(aLevel == 3)
                aPrice = 20000;
            else if(aLevel == 4)
                aPrice = 50000;
            else
                aPrice = -1;//Hide it ???
            
//            theDesc << "Upgrade how long machine will work";
//            theUnlockLevel << "For a limited time zap all enemies out of the existence";
            theDesc << "Improves effectiveness of the dwarf machine";
            theUnlockLevel << "For a limited time zap all enemies out of existence";
        }
        else
        {
            aPrice = 35;
            theUnlockLevel << "Unlock from Mission Set #2";
//            theDesc << "For a limited time zap all enemies out of the existence";
            theDesc << "For a limited time zap all enemies out of existence";
        }
        
        thePriceString << aPrice;
        
        mElectroButtonState = theState;//set the new state
    }
    else
    {
        aBG = static_cast<CCSprite*>(mBase->getChildByTag(400));//Get the old one
        
        //static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(theType));
        aMenuButton = static_cast<CCMenu*>(aBG->getChildByTag(410+mStampButtonState));//Get the old one
        aMenuButton->setVisible(false);
        
        aMenuButton = static_cast<CCMenu*>(aBG->getChildByTag(410+theState));
        aMenuButton->setVisible(true);
        
        //Update txt and other stuff
        aLevel = User::getInstance()->getMachine_StampLvl();
        if(aLevel > 0)
        {
            aLocked = false;
            //GET THE CORRECT PRICE
            if(aLevel == 1)
                aPrice = 2000;
            else if(aLevel == 2)
                aPrice = 8000;
            else if(aLevel == 3)
                aPrice = 20000;
            else if(aLevel == 4)
                aPrice = 50000;
            else
                aPrice = -1;//Hide it ???
            
            theDesc << "Improves effectiveness of the dwarf machine";
            theUnlockLevel << "Smashes earth clearing the forest of all trolls, traps and dwarfs alike. A fresh start!";
        }
        else
        {
            aPrice = 150;
            theUnlockLevel << "Unlock from Mission Set #12";
            theDesc << "Smashes earth clearing the forest of all trolls, traps and dwarfs alike. A fresh start!";
        }
        
        thePriceString << aPrice;
        
        mStampButtonState = theState;//set the new state
    }
    
    //The universal mega functional
    //Checks if is locked
    if(aLocked)
    {
        //The unlock steps
        if(theState == 1)
        {
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+63));//Get the old one
            aTextField->setString(theUnlockLevel.str().c_str());//Get the correct level req
            aTextField->setFontSize(TITLE_FONT_SIZE*0.6);
            
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+64));//Get the old one
            aTextField->setString("Unlock now for:");//Get the correct level req
//            aTextField->setHorizontalAlignment(kCCTextAlignmentCenter);
            
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+65));//Get the old one
            aTextField->setString(thePriceString.str().c_str());//Get the correct level req
//            aTextField->setFontSize(TITLE_FONT_SIZE*0.8);
//            aTextField->setHorizontalAlignment(kCCTextAlignmentCenter);
//            aTextField->setPosition(ccp(aBG->getContentSize().width/2,aBG->getContentSize().height-198));
            
            //Position of the crystal
            aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+51));
            float aLength = strlen(aTextField->getString())*0.5f;
            aDummySprite->setPosition(ccp(aTextField->getPositionX()+aLength*25+aDummySprite->getContentSize().width/2,aDummySprite->getPositionY()));
        }
        else if(theState == 3)//want to unlock for money
        {
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+63));//Get the old one
            aTextField->setString(theDesc.str().c_str());//Get the correct level req
            aTextField->setFontSize(TITLE_FONT_SIZE*0.4);
            
            
        }
    }
    else
    {
        if(theState == 3)
        {
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+63));//Get the old one
            aTextField->setString(theDesc.str().c_str());//Get the correct level req
//            aTextField->setHorizontalAlignment(kCCTextAlignmentLeft);
            aTextField->setHorizontalAlignment(kCCTextAlignmentCenter);
            aTextField->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
            aTextField->setFontSize(TITLE_FONT_SIZE*0.4);
        }
        else
        {
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+63));//Get the old one
            aTextField->setString(theUnlockLevel.str().c_str());//Get the correct level req
            aTextField->setHorizontalAlignment(kCCTextAlignmentCenter);
            aTextField->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
            aTextField->setFontSize(TITLE_FONT_SIZE*0.4);
        }
        
        //The upgrade steps
        if(theState == 1 || theState == 2)
        {
            //Check if button buy is visible !!!
            //            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+63));//Get the old one
            //            aTextField->setString(theDesc.str().c_str());//Get the correct level req
            //            aTextField->setFontSize(TITLE_FONT_SIZE*0.4);
            
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+64));//Get the old one
            aTextField->setString("Upgrade cost:");//Get the correct level req
//            aTextField->setHorizontalAlignment(kCCTextAlignmentLeft);
            
            aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+65));//Get the old one
            aTextField->setString(thePriceString.str().c_str());//Get the correct level req
            aTextField->setFontSize(TITLE_FONT_SIZE*0.45);
//            aTextField->setHorizontalAlignment(kCCTextAlignmentCenter);
//            aTextField->setPosition(ccp(aBG->getContentSize().width/2+60,aBG->getContentSize().height-173));
            
            aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+51));
            aDummySprite->setVisible(false);
            
            aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+50));
            aDummySprite->setVisible(true);
            
            //Set the stars amount !!!
            for(int i=0;i<5;i++)
            {
                aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+20+i));
                if(aLevel<=i)
                    aDummySprite->setVisible(true);
                else
                    aDummySprite->setVisible(false);
                
                aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+30+i));
                if(aLevel>i)
                    aDummySprite->setVisible(true);
                else
                    aDummySprite->setVisible(false);
            }
            
            if(aLevel == 5)//Max lvl
            {
                aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+64));//Get the old one
                aTextField->setString("Fully upgraded");//Get the correct level req
                aTextField->setPositionX(340);
                aTextField->setHorizontalAlignment(kCCTextAlignmentCenter);
                
                aTextField = static_cast<CCTextFieldTTF*>(aBG->getChildByTag(theButton*100+65));//Get the old one
                aTextField->setVisible(false);
                
                aDummySprite = static_cast<CCSprite*>(aBG->getChildByTag(theButton*100+50));
                aDummySprite->setVisible(false);
                
                aMenuButton->setVisible(false);
            }
        }
    }
    */
}

void StoreBox::CreateMachineByLevel(int theMachine,int theLevel,bool theCreate)
{
    bool aLocked = false;
    if(theLevel<1)
    {
        aLocked = true;
        theLevel = 1;
    }
    
    CCSprite* aIcon = NULL;
    
    char theFileName[64];
    //The electro
    if(theMachine == 1)
    {
        sprintf(theFileName, "Interfeiss/upgrade_screen/plant_%i.png", theLevel);
        
        if(!theCreate)
        {
            //Create some fx
            mBase->getChildByTag(100)->removeChildByTag(101,true);
            
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(101);
//            aIcon->setScale(0.8);
            aIcon->setPosition(ccp(100,90));
            mBase->getChildByTag(100)->addChild(aIcon);
            
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/GhoustActivate.plist");
            p->setPosition(mBase->getChildByTag(100)->getPositionX()-250, mBase->getChildByTag(100)->getPositionY()-30);
            p->setAutoRemoveOnFinish(true);
            mBase->addChild(p,10);
        }
        else
        {
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(101);
//            aIcon->setScale(0.8);
            aIcon->setPosition(ccp(100,90));
            mBase->getChildByTag(100)->addChild(aIcon);
        }
    }
    else if(theMachine == 2)
    {
        sprintf(theFileName, "Interfeiss/upgrade_screen/ghost_%i.png", theLevel);
        
        if(!theCreate)
        {
            mBase->getChildByTag(200)->removeChildByTag(201,true);
            
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(201);
            aIcon->setScale(0.8);
            aIcon->setPosition(ccp(100,90));
            mBase->getChildByTag(200)->addChild(aIcon);
            
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/GhoustActivate.plist");
            p->setPosition(mBase->getChildByTag(200)->getPositionX()-250, mBase->getChildByTag(200)->getPositionY()-30);
            p->setAutoRemoveOnFinish(true);
            mBase->addChild(p,10);
        }
        else
        {
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(201);
            aIcon->setScale(0.8);
            aIcon->setPosition(ccp(100,90));
            mBase->getChildByTag(200)->addChild(aIcon);
        }
    }
    else if(theMachine == 3)
    {
        sprintf(theFileName, "Interfeiss/upgrade_screen/elektro_%i.png", theLevel);
        
        if(!theCreate)
        {
            mBase->getChildByTag(300)->removeChildByTag(301,true);
            
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(301);
            aIcon->setScale(0.8);
            aIcon->setPosition(ccp(100,90));
            mBase->getChildByTag(300)->addChild(aIcon);
            
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/GhoustActivate.plist");
            p->setPosition(mBase->getChildByTag(300)->getPositionX()-250, mBase->getChildByTag(300)->getPositionY()-30);
            p->setAutoRemoveOnFinish(true);
            mBase->addChild(p,10);
        }
        else
        {
            aIcon = CCSprite::create(theFileName);
            aIcon->setScale(0.8);
            aIcon->setTag(301);
            aIcon->setPosition(ccp(100,90));
//            getChildByTag(300)->addChild(aIcon);
            mBase->getChildByTag(300)->addChild(aIcon);
        }
    }
    else if(theMachine == 4)
    {
        sprintf(theFileName, "Interfeiss/upgrade_screen/stomp_%i.png", theLevel);
        
        if(!theCreate)
        {
            mBase->getChildByTag(400)->removeChildByTag(401,true);
            
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(401);
            aIcon->setScale(0.8);
            aIcon->setPosition(ccp(100,90));
            mBase->getChildByTag(400)->addChild(aIcon);
            
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/GhoustActivate.plist");
            p->setPosition(mBase->getChildByTag(400)->getPositionX()-250, mBase->getChildByTag(400)->getPositionY()-30);
            p->setAutoRemoveOnFinish(true);
            mBase->addChild(p,10);
        }
        else
        {
            aIcon = CCSprite::create(theFileName);
            aIcon->setTag(401);
            aIcon->setScale(0.8);
            aIcon->setPosition(ccp(100,90));
            mBase->getChildByTag(400)->addChild(aIcon);
        }
    }
    
    //Check if is not locked
    if(aLocked && aIcon!=NULL)
    {
        CCSprite* aLock = CCSprite::create("Interfeiss/upgrade_screen/locked.png");
        aLock->setTag(1000);
        aLock->setPosition(ccp(aIcon->getContentSize().width/2,aIcon->getContentSize().height/2));
        aIcon->addChild(aLock);
    }
}

//================================================================
//================================================================

void StoreBox::OnPowerUpClick(CCObject* sender)
{
    CCMenuItemImage* aButton =(CCMenuItemImage*)sender;
    int aID = aButton->getTag();
    
    mCurrentSelectedButtonIndex = aID;
    
    // Check if this button was locked or selected/deselected
    CCString* state = (CCString*)aButton->getUserObject();
    
    // Clicked 1st time and want to unlock
    if(state->compare("Locked") == 0)
    {
        // Show popup about buy !!!
        aButton->setUserObject(CCString::create("Buy_Unlock"));
    }
    else if(state->compare("Buy_Unlock") == 0)
    {
        // Check if has the monet for it :)
        // TODO
        User::getInstance()->getItemDataManager().onPurchaseItem(SHOP_POWERS,aID);
        
        // Bought? show that can upgrade
        aButton->setUserObject(CCString::create("Upgrade"));
    }
    else if(state->compare("Upgrade") == 0)
    {
        // Buy this upgrade
        aButton->setUserObject(CCString::create("Buy_Upgrade"));
    }
    else if(state->compare("Buy_Upgrade") == 0)
    {
        // Check if has the monet for it :)
        // TODO
        
        User::getInstance()->getItemDataManager().upgradePowerItem(aID);
        
        // Set back to normal
        aButton->setUserObject(CCString::create("Upgrade"));
    }
    
    CheckSpellButtons(false,true);
}

void StoreBox::OnSpellClick(CCObject* sender)
{
    // Deselect power item if had selected
    mCurrentSelectedButtonIndex = -1;
    
    CCMenuItemImage* aButton =(CCMenuItemImage*)sender;
    int aID = aButton->getTag();
    
    // Check if this button was locked or selected/deselected
    CCString* state = (CCString*)aButton->getUserObject();
    
    if(state->compare("Locked") == 0)
    {
        // Show popup about buy !!!
        User::getInstance()->getItemDataManager().onPurchaseItem(SHOP_SPELLS,aID);
        aButton->setUserObject(CCString::create("UnSelected"));
    }
    else
    {
        // It's unlocked
        if(state->compare("Selected") == 0)
        {
            User::getInstance()->getItemDataManager().onRemoveSelectedItem(SHOP_SPELLS, aID);
        }
        else if(state->compare("UnSelected") == 0)
        {
            User::getInstance()->getItemDataManager().onSetSelectedItem(SHOP_SPELLS, aID);
        }
    }
    
    CheckSpellButtons(true,false);
}

// The ultimate function to check states of buttons
void StoreBox::CheckSpellButtons(bool OnlySpells, bool OnlyPowers)
{
    CCSprite* aButtonBase;
    CCSprite* aButtonIcon;
    CCLabelTTF* aDescText;
    
    std::stringstream button_image_off;
    std::stringstream button_image_on;
    std::stringstream button_state;
    std::stringstream power_desc;
    
    CCSprite* aItemPriceIcon;
    CCLabelTTF* aItemPriceText;
    CCLabelTTF* aItemPriceAmount;
    
    float fontSize;
    
    for(int i=0;i<mBase->getChildrenCount();i++)
    {
        //..................................................................
        // The powerup part !!!
            
        aButtonBase = (CCSprite*)mBase->getChildByTag(i+1000); // The powerup tag 1000
        
        if(aButtonBase != NULL && OnlyPowers == true)
        {
            CCLog("We have powerup to update");
            
            button_image_off.str("");button_image_off.clear();
            button_image_on.str("");button_image_on.clear();
            button_state.str("");button_state.clear();
            power_desc.str(""),power_desc.clear();
            
            aButtonIcon = (CCSprite*)aButtonBase->getChildByTag(BUTTON_ICON);
            
            CCMenu* aMenuOfBase = (CCMenu*)aButtonBase->getChildByTag(BUTTON_ACTION);
            CCMenuItemImage* aButton = (CCMenuItemImage*)aMenuOfBase->getChildren()->objectAtIndex(0);
            
            aItemPriceIcon = (CCSprite*)aButtonBase->getChildByTag(ITEM_PRICE_ICON);
            aItemPriceText = (CCLabelTTF*)aButtonBase->getChildByTag(ITEM_PRICE_TEXT);
            aItemPriceAmount = (CCLabelTTF*)aButtonBase->getChildByTag(ITEM_PRICE_AMOUNT);
            
            aDescText = (CCLabelTTF*)aButtonBase->getChildByTag(POWER_DESC);
            // Set the text by button
            
            // Do something by state
            CCString* state = (CCString*)aButton->getUserObject();
            
            CCLog("mCurrentSelectedButtonIndex: %i | aButton->getTag(): %i",mCurrentSelectedButtonIndex,aButton->getTag());
            CCLog("state: %s",state->m_sString.c_str());
            
            fontSize = TITLE_FONT_SIZE*0.6;
            
            bool checkStars = false;
            
            int theItemLevel = User::getInstance()->getItemDataManager().getPowerItemLevel(aButton->getTag());
            int theMaxUpgrades = User::getInstance()->getItemDataManager().getPowerByID(aButton->getTag()).max_upgrades;
            
            if(mCurrentSelectedButtonIndex != aButton->getTag())
            {
                // if this button had some state - move to prev state
                if(state->compare("Buy_Unlock") == 0){
                    
                    button_image_off << "Shop/Button_Unlock_1.png";
                    button_image_on << "Shop/Button_Unlock_2.png";
                    button_state << "Locked";
                    
                    fontSize = TITLE_FONT_SIZE*0.6;
                    power_desc << User::getInstance()->getItemDataManager().getPowerByID(aButton->getTag()).text_locked;
                }
                else if(state->compare("Buy_Upgrade") == 0){
                    
                    button_image_off << "Shop/Button_Upgrade_1.png";
                    button_image_on << "Shop/Button_Upgrade_2.png";
                    button_state << "Upgrade";
                    
                    checkStars = true;
                    fontSize = TITLE_FONT_SIZE*0.4;
                    power_desc << User::getInstance()->getItemDataManager().getPowerByID(aButton->getTag()).text_upgrade;
                }
                else
                {
                    // Default stuff
                    if(state->compare("Locked") == 0)
                    {
                        // It's locked
                        button_image_off << "Shop/Button_Unlock_1.png";
                        button_image_on << "Shop/Button_Unlock_2.png";
                        
                        fontSize = TITLE_FONT_SIZE*0.6;
                        power_desc << User::getInstance()->getItemDataManager().getPowerByID(aButton->getTag()).text_locked;
                    }
                    else if(state->compare("Upgrade") == 0)
                    {
                        // Check if can upgrade more !!!
                        if(theItemLevel >= theMaxUpgrades){
                            aButton->setVisible(false);// Stop it - hide this button !!!
                            // Hide the price stuff
                            aItemPriceIcon->setVisible(false);
                            aItemPriceText->setVisible(false);
                            aItemPriceAmount->setVisible(false);
                        }
                        
                        button_image_off << "Shop/Button_Upgrade_1.png";
                        button_image_on << "Shop/Button_Upgrade_2.png";
                        
                        checkStars = true;
                        fontSize = TITLE_FONT_SIZE*0.4;
                        power_desc << User::getInstance()->getItemDataManager().getPowerByID(aButton->getTag()).text_unlocked;
                    }
                    
                    button_state << state->m_sString;
                }
            }
            else
            {
                if(state->compare("Buy_Unlock") == 0 || state->compare("Buy_Upgrade") == 0)
                {
                    // Show the buy button !!!
                    button_image_off << "Shop/Button_Buy_1.png";
                    button_image_on << "Shop/Button_Buy_2.png";
                    
                    fontSize = TITLE_FONT_SIZE*0.4;
                    if(state->compare("Buy_Upgrade") == 0){
                        power_desc << User::getInstance()->getItemDataManager().getPowerByID(aButton->getTag()).text_upgrade;
                    }
                    else{
                        power_desc << User::getInstance()->getItemDataManager().getPowerByID(aButton->getTag()).text_unlocked;
                    }
                }
                else if(state->compare("Locked") == 0)
                {
                    button_image_off << "Shop/Button_Unlock_1.png";
                    button_image_on << "Shop/Button_Unlock_2.png";
                    
                    fontSize = TITLE_FONT_SIZE*0.6;
                    power_desc << User::getInstance()->getItemDataManager().getPowerByID(aButton->getTag()).text_locked;
                }
                else if(state->compare("Upgrade") == 0)
                {
                    // Remove the lock icon if had
                    if(aButtonIcon->getChildByTag(ICON_LOCKED) != NULL){
                        aButtonIcon->removeChildByTag(ICON_LOCKED);
                    }
                    
                    button_image_off << "Shop/Button_Upgrade_1.png";
                    button_image_on << "Shop/Button_Upgrade_2.png";
                    
                    checkStars = true;
                    fontSize = TITLE_FONT_SIZE*0.4;
                    power_desc << User::getInstance()->getItemDataManager().getPowerByID(aButton->getTag()).text_unlocked;
                    
                    // Hide all if max upgrade !!!
                    if(theItemLevel >= theMaxUpgrades){
                        aButton->setVisible(false);// Stop it - hide this button !!!
                        // Hide the price stuff
                        aItemPriceIcon->setVisible(false);
                        aItemPriceText->setVisible(false);
                        aItemPriceAmount->setVisible(false);
                    }
                }
                
                button_state << state->m_sString;
            }
            
            if(checkStars)
            {
                CCSprite* aDummyStar;
                
                for(int s=0;s<theMaxUpgrades;s++)
                {
                    aDummyStar = (CCSprite*)aButton->getChildByTag(UPGRADE_STAR+s);
                    if(aDummyStar != NULL)
                    {
                        // Change to needed?
                        if(theItemLevel>=s){
                            aDummyStar->setTexture(CCSprite::create("Shop/star_empty.png")->getTexture());
                        }
                        else{
                            aDummyStar->setTexture(CCSprite::create("Shop/star_full.png")->getTexture());
                        }
                    }
                    else
                    {
                        int aStartY = 125;
                        // Create the star missing
                        if(s>=theItemLevel){
                            aDummyStar = CCSprite::create("Shop/star_empty.png");
                        }
                        else{
                            aDummyStar = CCSprite::create("Shop/star_full.png");
                        }
                        
                        aDummyStar->setScale(1.25f);
                        
                        aDummyStar->setPosition(ccp(aButtonBase->getContentSize().width/2-(aDummyStar->getContentSize().width*theMaxUpgrades)/2+(s*(aDummyStar->getContentSize().width+10)),aStartY));
                        aDummyStar->setTag(UPGRADE_STAR+s);
                        aButtonBase->addChild(aDummyStar);
                    }
                }
                
                // Check the icon too
                std::stringstream power_icon;
                if(User::getInstance()->getItemDataManager().getPowerByID(aButton->getTag()).icon_change){
                    power_icon << User::getInstance()->getItemDataManager().mPowerDataVector[i].icon_path << theItemLevel <<".png";
                }
                else{
                    power_icon << User::getInstance()->getItemDataManager().mPowerDataVector[i].icon_path;
                }
                
                aButtonIcon->setTexture(CCSprite::create(power_icon.str().c_str())->getTexture());
                
                //--------
                
                // The Price for unlock/buy?
                if(theItemLevel<theMaxUpgrades)
                {
                    std::string theCostInfo = User::getInstance()->getItemDataManager().getPowerByID(aButton->getTag()).level_cost[theItemLevel];
                    std::vector<std::string> theCostVector = SplitString_VecString(theCostInfo,'=');
                    std::stringstream buy_text;
                    
                    buy_text << "Upgrade cost:";
                    
                    // Set the cost icon
                    if(theCostVector[0].compare("D") == 0){
                        // The diamonds
                        aItemPriceIcon->setTexture(CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png")->getTexture());
                    }
                    else{
                        // Crystal cost
                        aItemPriceIcon->setTexture(CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png")->getTexture());
                    }
                    
                    aItemPriceAmount->setString(theCostVector[1].c_str());
                    aItemPriceText->setString(buy_text.str().c_str());
                    
                    // Now align the elements
                    aItemPriceText->setPosition(ccp(160,40));
                    aItemPriceAmount->setPosition(ccp(aItemPriceText->getPositionX()+aItemPriceText->getTextureRect().size.width+10,40));
                    aItemPriceIcon->setPosition(ccp(aItemPriceAmount->getPositionX()+aItemPriceAmount->getTextureRect().size.width+10,40));
                }
                
                //........
            }
            
            aDescText->setString(power_desc.str().c_str());
            aDescText->setFontSize(fontSize);
            
            aButton->setNormalImage(CCSprite::create(button_image_off.str().c_str()));
            aButton->setSelectedImage(CCSprite::create(button_image_on.str().c_str()));
            aButton->setUserObject(CCString::create(button_state.str()));
        }
        
        //..................................................................
        // The spell part !!!
        
        aButtonBase = (CCSprite*)mBase->getChildByTag(i+100); // The spell tag 100
        
        if(aButtonBase != NULL && OnlySpells == true)
        {
            aButtonIcon = (CCSprite*)aButtonBase->getChildByTag(BUTTON_ICON);
            
            CCMenu* aMenuOfBase = (CCMenu*)aButtonBase->getChildByTag(BUTTON_ACTION);
            CCMenuItemImage* aButton = (CCMenuItemImage*)aMenuOfBase->getChildren()->objectAtIndex(0);
            
            button_image_off.str("");button_image_off.clear();
            button_image_on.str("");button_image_on.clear();
            button_state.str("");button_state.clear();
            
            if(User::getInstance()->getItemDataManager().isItemUnlocked(aButton->getTag()))
            {
                // Remove the lock icon if had
                if(aButtonIcon->getChildByTag(ICON_LOCKED) != NULL){
                    aButtonIcon->removeChildByTag(ICON_LOCKED);
                }
                
                // Check if it is selected or not
                if(User::getInstance()->getItemDataManager().isItemActive(aButton->getTag()))
                {
                    // Item is active
                    button_image_off << "Shop/Button_Selected.png";
                    button_image_on << "Shop/Button_Selected.png";
                    button_state << "Selected";
                }
                else
                {
                    button_image_off << "Shop/Button_Select.png";
                    button_image_on << "Shop/Button_Select.png";
                    button_state << "UnSelected";
                }
            }
            else
            {
                // The locked state buttons
                button_image_off << "Shop/Button_Unlock_1.png";
                button_image_on << "Shop/Button_Unlock_2.png";
                button_state << "Locked";
            }
            
            aButton->setNormalImage(CCSprite::create(button_image_off.str().c_str()));
            aButton->setSelectedImage(CCSprite::create(button_image_on.str().c_str()));
            aButton->setUserObject(CCString::create(button_state.str()));
        }
        
        //..................................................................
    }
}

void StoreBox::CreateShop()
{
    mBase->setVisible(false);
    mBaseFreeStuff->setVisible(false);
    mBaseShop->setVisible(false);
    
    // Reset values
    mCurrentSelectedButtonIndex = -1;
    
    if(mShopCreated)
    {
        mBaseShop->setVisible(true);
        CheckSpellButtons(true,true);
        return;
    }
    
    mShopCreated = true;
    mBaseShop->setVisible(true);
    
    /*
    //Add some elements
    int aX = 20;
    int aY = 450;
    int aSpaceY = 10;
    
    // Now here comes the magic for rading from xml
    CCSprite* aHeader = CCSprite::create("Interfeiss/store/FRESH/shop/title_combo_deals.png");
    aHeader->setPosition(ccp(aX,aY+90));
    mBaseShop->addChild(aHeader,1);
    
    int aTotalSpellTabItems = User::getInstance()->getItemDataManager().mSpellDataVector.size();
    CCLog("Total Spell Tab Itmes: %i",aTotalSpellTabItems);
    
    int aTotalPowerTabItems = User::getInstance()->getItemDataManager().mPowerDataVector.size();
    CCLog("Total Power Tab Itmes: %i",aTotalPowerTabItems);
    
    CCSprite* aBoxBase;
    CCLabelTTF* aTxtHeader = NULL;
    
    CCLabelTTF* aTxtPowerDesc = NULL;
    
    CCSprite* aIcon;
    CCSprite* aLockIcon;
    
    CCSprite* aHeaderIcon;
    
    // Special magic
    CCSprite* aIconDamage;
    CCSprite* aIconRange;
    CCLabelTTF* aTxt_Damage = NULL;
    CCLabelTTF* aTxt_Range = NULL;
    
    CCLabelTTF* aTxt_Price = NULL;
    CCLabelTTF* aTxt_PriceAmount = NULL;
    CCSprite* aPriceIcon;
    
    CCMenuItemImage* MenuButton;
    CCMenu* MenuButtons;
    
    CCSprite* aDummyStar;
    
    int aGlobalMidOffestX = 40;
    
    int aScrollOffsetY = 20;
    mMaxScroll_Shop_Y = 0;
    
    //........................................................................
    // Create the powerups before spells !!!
    
    aY = 420;
    
    for(int i = 0;i<aTotalPowerTabItems;i++)
    {
        aBoxBase = CCSprite::create("Shop/panel_upgrades.png");
        aBoxBase->setPosition(ccp(aX,aY));
        aBoxBase->setTag(i+1000);
        mBaseShop->addChild(aBoxBase);
        aY-=aBoxBase->getContentSize().height+aSpaceY;
        
        // Update how far can scroll
        if(aY<=0) mMaxScroll_Shop_Y+=aBoxBase->getContentSize().height;
        
        //------------------------------------------------------------------
        // The Header
        
        aTxtHeader = CCLabelTTF::create(User::getInstance()->getItemDataManager().mPowerDataVector[i].name.c_str(),
                                        "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6);
        aTxtHeader->setHorizontalAlignment(kCCTextAlignmentLeft);
        aTxtHeader->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxtHeader->setPosition(ccp(aBoxBase->getContentSize().width/2,aBoxBase->getContentSize().height-24));
        aTxtHeader->setColor(ccc3(84, 71, 52));
        aBoxBase->addChild(aTxtHeader);
        
        // Add the side icons
        aHeaderIcon = CCSprite::create("Shop/text_deco.png");
        aHeaderIcon->setPosition(ccp(aTxtHeader->getPositionX()-aTxtHeader->getContentSize().width/2-50,aTxtHeader->getPositionY()));
        aBoxBase->addChild(aHeaderIcon);
        
        aHeaderIcon = CCSprite::create("Shop/text_deco.png");
        aHeaderIcon->setFlipX(true);
        aHeaderIcon->setPosition(ccp(aTxtHeader->getPositionX()+aTxtHeader->getContentSize().width/2+50,aTxtHeader->getPositionY()));
        aBoxBase->addChild(aHeaderIcon);
        
        //----
        // The Level
        bool aPowerItemUnLocked = User::getInstance()->getItemDataManager().isPowerItemUnlocked(User::getInstance()->getItemDataManager().mPowerDataVector[i].id);
        int aPowerItemLevel = User::getInstance()->getItemDataManager().getPowerItemLevel(User::getInstance()->getItemDataManager().mPowerDataVector[i].id);
        
        CCLog("aPowerItemUnLocked: %i",aPowerItemUnLocked);
        CCLog("aPowerItemLevel: %i",aPowerItemLevel);
        
        //------------------------------------------------------------------
        // The icon?
        
        // Get the correct icon by upgrade etc
        std::stringstream power_icon;
        if(User::getInstance()->getItemDataManager().mPowerDataVector[i].icon_change)
        {
            power_icon << User::getInstance()->getItemDataManager().mPowerDataVector[i].icon_path << aPowerItemLevel <<".png";
        }
        else
        {
            power_icon << User::getInstance()->getItemDataManager().mPowerDataVector[i].icon_path;
        }
        
        aIcon = CCSprite::create(power_icon.str().c_str());
        aIcon->setTag(BUTTON_ICON);
        aIcon->setPosition(ccp(50+aIcon->getContentSize().width/2,aBoxBase->getContentSize().height/2));
        aBoxBase->addChild(aIcon);
        
        // Do we need to add above icon?
        if(aPowerItemUnLocked == false)
        {
            aLockIcon = CCSprite::create("Shop/locked.png");
            aLockIcon->setTag(ICON_LOCKED);
            aLockIcon->setPosition(ccp(aIcon->getContentSize().width/2,aIcon->getContentSize().height/2));
            aIcon->addChild(aLockIcon);
        }
        
        //------------------------------------------------------------------
        // The text for power
        std::string theDesc;
        float fontSize;
        
        int aUpgradeIndexCheck = aPowerItemLevel;
        
        if(aPowerItemUnLocked == false)
        {
            aUpgradeIndexCheck -= 1; //Take 1st cost
            fontSize = TITLE_FONT_SIZE*0.7;
            theDesc = User::getInstance()->getItemDataManager().mPowerDataVector[i].text_locked;
        }
        else
        {
            fontSize = TITLE_FONT_SIZE*0.5;
            theDesc = User::getInstance()->getItemDataManager().mPowerDataVector[i].text_unlocked;
        }
        
        aTxtPowerDesc = CCLabelTTF::create(theDesc.c_str(),"fonts/Marker Felt.ttf",fontSize);
        aTxtPowerDesc->setTag(POWER_DESC);
        aTxtPowerDesc->setHorizontalAlignment(kCCTextAlignmentLeft);
        aTxtPowerDesc->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxtPowerDesc->setPosition(ccp(aBoxBase->getContentSize().width/2+50,aBoxBase->getContentSize().height/2));
        aTxtPowerDesc->setColor(ccc3(84, 71, 52));
        aBoxBase->addChild(aTxtPowerDesc);
        
        // TODO check if not max upgraded !!!
        
        // The Price for unlock/buy?
        int aMaxUpgrades = User::getInstance()->getItemDataManager().mPowerDataVector[i].max_upgrades;
        
        // The safe check
        if(aUpgradeIndexCheck>=aMaxUpgrades){
            aUpgradeIndexCheck = aMaxUpgrades-1;
        }
        
        std::string theCostInfo = User::getInstance()->getItemDataManager().mPowerDataVector[i].level_cost[aUpgradeIndexCheck];
        std::vector<std::string> theCostVector = SplitString_VecString(theCostInfo,'=');
        std::stringstream buy_text;
        
        if(aPowerItemUnLocked == false){
            buy_text << "Unlock now for:";
        }
        else{
            buy_text << "Upgrade cost:";
        }
        
        // Set the texts
        aTxt_PriceAmount = CCLabelTTF::create(theCostVector[1].c_str(),"fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
        aTxt_PriceAmount->setAnchorPoint(ccp(0,0.5f));
        aTxt_Price = CCLabelTTF::create(buy_text.str().c_str(),"fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
        aTxt_Price->setAnchorPoint(ccp(0,0.5f));
        
        // Set the cost icon
        if(theCostVector[0].compare("D") == 0){
            // The diamonds
            aPriceIcon = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
        }
        else{
            // Crystal cost
            aPriceIcon = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
        }
        
        aPriceIcon->setAnchorPoint(ccp(0,0.5f));
        
        aBoxBase->addChild(aTxt_PriceAmount);
        aBoxBase->addChild(aTxt_Price);
        aBoxBase->addChild(aPriceIcon);
        
        // Set some tags for thease action memebers
        aTxt_PriceAmount->setTag(ITEM_PRICE_AMOUNT);
        aTxt_Price->setTag(ITEM_PRICE_TEXT);
        aPriceIcon->setTag(ITEM_PRICE_ICON);
        
        // Now align the elements
        aTxt_Price->setPosition(ccp(160,40));
        aTxt_PriceAmount->setPosition(ccp(aTxt_Price->getPositionX()+aTxt_Price->getTextureRect().size.width+10,40));
        aPriceIcon->setPosition(ccp(aTxt_PriceAmount->getPositionX()+aTxt_PriceAmount->getTextureRect().size.width+10,40));
        
        //------------------------------------------------------------------
        // The stars !!!
        if(aPowerItemUnLocked == true)
        {
            int aStartY = 125;
            // Check how far is unlocked !!!
            for(int s=0;s<User::getInstance()->getItemDataManager().mPowerDataVector[i].max_upgrades;s++)
            {
                // Whats the current upgrade level?
                if(s>=aPowerItemLevel){
                    aDummyStar = CCSprite::create("Shop/star_empty.png");
                }
                else{
                    aDummyStar = CCSprite::create("Shop/star_full.png");
                }
                
                aDummyStar->setScale(1.25f);
                
                aDummyStar->setPosition(ccp(aBoxBase->getContentSize().width/2-(aDummyStar->getContentSize().width*User::getInstance()->getItemDataManager().mPowerDataVector[i].max_upgrades)/2+(s*(aDummyStar->getContentSize().width+10)),aStartY));
                aDummyStar->setTag(UPGRADE_STAR+s);
                aBoxBase->addChild(aDummyStar);
            }
        }
        
        //------------------------------------------------------------------
        // Buttons?
        std::stringstream button_image_off;
        std::stringstream button_image_on;
        std::stringstream button_state;
        
        if(aPowerItemUnLocked == false)
        {
            // It's locked
            button_image_off << "Shop/Button_Unlock_1.png";
            button_image_on << "Shop/Button_Unlock_2.png";
            button_state << "Locked";
        }
        else
        {
            // Check if can upgrade more !!!
            button_image_off << "Shop/Button_Upgrade_1.png";
            button_image_on << "Shop/Button_Upgrade_2.png";
            button_state << "Upgrade";
        }
        
        MenuButton = CCMenuItemImage::create(button_image_off.str().c_str(),
                                             button_image_on.str().c_str(),
                                             this,
                                             menu_selector(StoreBox::OnPowerUpClick));
        MenuButton->setAnchorPoint(ccp(1,0));
        MenuButton->setTag(User::getInstance()->getItemDataManager().mPowerDataVector[i].id);
        MenuButton->setUserObject(CCString::create(button_state.str()));
        
        MenuButtons = CCMenu::create(MenuButton, NULL);
        MenuButtons->setTag(BUTTON_ACTION);// The menu button tah
        MenuButtons->setPosition(aBoxBase->getContentSize().width-30,10);
        aBoxBase->addChild(MenuButtons, 10);
        
        // The special stuff check
        if(aPowerItemLevel >= aMaxUpgrades){
            MenuButtons->setVisible(false);// Stop it - hide this button !!!
            // Hide the price stuff
            aPriceIcon->setVisible(false);
            aTxt_Price->setVisible(false);
            aTxt_PriceAmount->setVisible(false);
        }
        
        //------------------------------------------------------------------
        
    }
    
    //........................................................................
    // The next header
    
    aHeader = CCSprite::create("Shop/title_spells.png");
    aHeader->setPosition(ccp(aX,aY+66));
    mBaseShop->addChild(aHeader,1);
    
    aY-=20;
//    mMaxScroll_Shop_Y-=160;
    
    //........................................................................
    
    for(int i = 0;i<aTotalSpellTabItems;i++)
    {
        //------------------------------------------------------------------
        // The Base
        aBoxBase = CCSprite::create("Shop/panel_shop.png");
        aBoxBase->setPosition(ccp(aX,aY));
        aBoxBase->setTag(i+100);//The spell Taging
        mBaseShop->addChild(aBoxBase);
        aY-=aBoxBase->getContentSize().height+aSpaceY;
        
        // Update how far can scroll
//        if(i>5)
//        {
//            mMaxScroll_Shop_Y+=aBoxBase->getContentSize().height;
//        }
        if(aY<=0) mMaxScroll_Shop_Y+=aBoxBase->getContentSize().height;
        
        //------------------------------------------------------------------
        // The Header
        
        aTxtHeader = CCLabelTTF::create(User::getInstance()->getItemDataManager().mSpellDataVector[i].name.c_str(),
                                        "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6,CCSize(420,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
        aTxtHeader->setPosition(ccp(aBoxBase->getContentSize().width/2-aGlobalMidOffestX,aBoxBase->getContentSize().height-24));
        aTxtHeader->setColor(ccc3(84, 71, 52));
        aBoxBase->addChild(aTxtHeader);
        
        //------------------------------------------------------------------
        // The Icon
        
        aIcon = CCSprite::create(User::getInstance()->getItemDataManager().mSpellDataVector[i].icon_path.c_str());
        aIcon->setTag(BUTTON_ICON);
        aIcon->setPosition(ccp(50+aIcon->getContentSize().width/2,aBoxBase->getContentSize().height/2));
        aBoxBase->addChild(aIcon);
        
        //------------------------------------------------------------------
        // The Stats
        
        aIconDamage = CCSprite::create("Shop/Icon_0002_Sword.png");
        aIconDamage->setAnchorPoint(ccp(0,0.5f));
        aIconRange = CCSprite::create("Shop/Icon_0001_Range.png");
        aIconRange->setAnchorPoint(ccp(0,0.5f));
        
        std::stringstream aDamageTxt;
        aDamageTxt << User::getInstance()->getItemDataManager().mSpellDataVector[i].damage;
        if(User::getInstance()->getItemDataManager().mSpellDataVector[i].damage_extra>0)
        {
            aDamageTxt << " + " <<User::getInstance()->getItemDataManager().mSpellDataVector[i].damage_extra << " x " << User::getInstance()->getItemDataManager().mSpellDataVector[i].damage_extra_multiply;
        }
        
        aTxt_Damage = CCLabelTTF::create(aDamageTxt.str().c_str(),"fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6);
        aTxt_Damage->setHorizontalAlignment(kCCTextAlignmentLeft);
        aTxt_Damage->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxt_Damage->setAnchorPoint(ccp(0,0.5f));
        
        std::stringstream aRangeTxt;
        aRangeTxt << User::getInstance()->getItemDataManager().mSpellDataVector[i].range;
        
        aTxt_Range = CCLabelTTF::create(aRangeTxt.str().c_str(),"fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6);
        aTxt_Range->setHorizontalAlignment(kCCTextAlignmentLeft);
        aTxt_Range->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxt_Range->setAnchorPoint(ccp(0,0.5f));
        
        aBoxBase->addChild(aTxt_Damage);
        aBoxBase->addChild(aTxt_Range);
        aBoxBase->addChild(aIconDamage);
        aBoxBase->addChild(aIconRange);
        
        // Position all stuff dynamic by text and icon size !!!
        int iconText_Space = 10;
        int statsToStats_Space = 20;
        int statsY = 30;
        
        int aTotalStatsWidth = aIconDamage->getContentSize().width + iconText_Space + aTxt_Damage->getTextureRect().size.width + statsToStats_Space + aIconRange->getContentSize().width + iconText_Space + aTxt_Range->getTextureRect().size.width;
        
        int aStartStatsX = aBoxBase->getContentSize().width/2 - aTotalStatsWidth/2-aGlobalMidOffestX;
        
        aIconDamage->setPosition(ccp(aStartStatsX,statsY));
        aStartStatsX+=aIconDamage->getContentSize().width+iconText_Space;
        
        aTxt_Damage->setPosition(ccp(aStartStatsX,statsY));
        aStartStatsX+=aTxt_Damage->getTextureRect().size.width+statsToStats_Space;
        
        aIconRange->setPosition(ccp(aStartStatsX,statsY));
        aStartStatsX+=aIconRange->getContentSize().width+iconText_Space;
        
        aTxt_Range->setPosition(ccp(aStartStatsX,statsY));
        
        
        //------------------------------------------------------------------
        // Check if this item is bought/unlocked etc !!!
        
        std::stringstream button_image_off;
        std::stringstream button_image_on;
        std::stringstream button_state;
        
        if(User::getInstance()->getItemDataManager().isItemUnlocked(User::getInstance()->getItemDataManager().mSpellDataVector[i].id))
        {
            // Check if it is selected or not
            if(User::getInstance()->getItemDataManager().isItemActive(User::getInstance()->getItemDataManager().mSpellDataVector[i].id))
            {
                // Item is active
                button_image_off << "Shop/Button_Selected.png";
                button_image_on << "Shop/Button_Selected.png";
                button_state << "Selected";
            }
            else
            {
                button_image_off << "Shop/Button_Select.png";
                button_image_on << "Shop/Button_Select.png";
                button_state << "UnSelected";
            }
        }
        else
        {
            // The locked state buttons
            button_image_off << "Shop/Button_Unlock_1.png";
            button_image_on << "Shop/Button_Unlock_2.png";
            button_state << "Locked";
            
            // Do we need to add above icon?
            aLockIcon = CCSprite::create("Shop/locked.png");
            aLockIcon->setTag(20);
            aLockIcon->setPosition(ccp(aIcon->getContentSize().width/2,aIcon->getContentSize().height/2));
            aIcon->addChild(aLockIcon);
        }
        
        MenuButton = CCMenuItemImage::create(button_image_off.str().c_str(),
                                             button_image_on.str().c_str(),
                                             this,
                                             menu_selector(StoreBox::OnSpellClick));
        MenuButton->setAnchorPoint(ccp(1,1));
        MenuButton->setTag(User::getInstance()->getItemDataManager().mSpellDataVector[i].id);
        MenuButton->setUserObject(CCString::create(button_state.str()));
        
        MenuButtons = CCMenu::create(MenuButton, NULL);
        MenuButtons->setTag(BUTTON_ACTION);// The menu button tah
        MenuButtons->setPosition(aBoxBase->getContentSize().width-30,aBoxBase->getContentSize().height-10);
        aBoxBase->addChild(MenuButtons, 10);
        
        //------------------------------------------------------------------
        // The Price if not bought
        aTxt_Price = CCLabelTTF::create("Cost:","fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
        aTxt_Price->setHorizontalAlignment(kCCTextAlignmentRight);
        aTxt_Price->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxt_Price->setAnchorPoint(ccp(1,0.5f));
        aTxt_Price->setColor(ccc3(84, 71, 52));
        
        std::stringstream aPriceValue;
        // Check what price is this !!!
        if(User::getInstance()->getItemDataManager().mSpellDataVector[i].price_crystals>0)
        {
            aPriceValue << User::getInstance()->getItemDataManager().mSpellDataVector[i].price_crystals;
            
            aPriceIcon = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
            aPriceIcon->setAnchorPoint(ccp(1,0.5f));
        }
        else
        {
            aPriceValue << User::getInstance()->getItemDataManager().mSpellDataVector[i].price_diamonds;
            
            aPriceIcon = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
            aPriceIcon->setAnchorPoint(ccp(1,0.5f));
        }
        
        aTxt_PriceAmount = CCLabelTTF::create(aPriceValue.str().c_str(),"fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
        aTxt_PriceAmount->setHorizontalAlignment(kCCTextAlignmentRight);
        aTxt_PriceAmount->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        aTxt_PriceAmount->setAnchorPoint(ccp(1,0.5f));
        
        aBoxBase->addChild(aTxt_PriceAmount);
        aBoxBase->addChild(aTxt_Price);
        aBoxBase->addChild(aPriceIcon);
        
        // Now align
        iconText_Space = 6;
        statsY = 30;
        
        aStartStatsX = aBoxBase->getContentSize().width - 30;
        
        aPriceIcon->setPosition(ccp(aStartStatsX,statsY));
        aStartStatsX-=aPriceIcon->getContentSize().width+iconText_Space;
        
        aTxt_PriceAmount->setPosition(ccp(aStartStatsX,statsY));
        aStartStatsX-=aTxt_PriceAmount->getTextureRect().size.width;
        
        aTxt_Price->setPosition(ccp(aStartStatsX,statsY));

        
    }
    
    // Set the max scroll down
    mMaxScroll_Shop_Y -= aScrollOffsetY;
    */
    
    //Add some elements
    int aX = 20;
    int aY = 450;
    int aSpaceY = 10;
    
    CCSprite* aBoxBase;
    
    CCSprite* aHeader = CCSprite::create("Interfeiss/store/FRESH/shop/title_combo_deals.png");
    aHeader->setPosition(ccp(aX,aY+90));
    mBaseShop->addChild(aHeader,1);
    
    CCSprite* aIcon;
    CCPoint aIconOffset;
    
    CCLabelTTF* aTxtHeader = NULL;
    CCLabelTTF* aTxtLine = NULL;
    CCLabelTTF* aTxtPrice = NULL;
    
    CCSprite* aIcon2 = NULL;
    
    CCMenu* MenuButtons;
    
    CCSprite* aSideHeader;
    CCMenuItemImage* MenuButton;
    
    mMaxScroll_Shop_Y = 970;
    
    for(int i=1;i<13;i++)
    {
        if(i==5)
        {
            aHeader = CCSprite::create("Interfeiss/store/FRESH/shop/title_crystalpacks.png");
            aHeader->setPosition(ccp(aX,aY+30));
            mBaseShop->addChild(aHeader,1);
            aY-=55;
        }
        
        if(i==9)
        {
            aHeader = CCSprite::create("Interfeiss/store/FRESH/shop/title_diamondpacks.png");
            aHeader->setPosition(ccp(aX,aY+30));
            mBaseShop->addChild(aHeader,1);
            aY-=55;
        }
        
        aBoxBase = CCSprite::create("Interfeiss/store/FRESH/shop/panel_shop.png");
        aBoxBase->setPosition(ccp(aX,aY));
        aBoxBase->setTag(i);
        mBaseShop->addChild(aBoxBase);
        aY-=aBoxBase->getContentSize().height+aSpaceY;
        
        aTxtHeader = CCLabelTTF::create("","fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6,CCSize(420,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
        aTxtHeader->setPosition(ccp(aBoxBase->getContentSize().width/2,aBoxBase->getContentSize().height-24));
        aTxtHeader->setColor(ccc3(84, 71, 52));
        
        aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
        aSideHeader->setTag(111);
        aBoxBase->addChild(aSideHeader);
        
        aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
        aSideHeader->setTag(222);
        aSideHeader->setFlipX(true);
        aBoxBase->addChild(aSideHeader);
        
        if(i==1)//No Ads!
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/combodeal1.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("No ads!");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-100,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+100,aBoxBase->getContentSize().height-23));
            
            aTxtLine = CCLabelTTF::create("Permanently remove ads! Thats sweet!",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.45,CCSize(420,80),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(84, 71, 52));
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2,48));
            aBoxBase->addChild(aTxtLine);
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$0.69",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        else if(i==2)//Some stuff
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/combodeal2.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("First buyer pack");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-140,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+140,aBoxBase->getContentSize().height-23));
            
            //Crystal amount
            CCLabelTTF* aText1 = CCLabelTTF::create("10 000",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aText1->setColor(ccc3(255,255,255));
            aText1->enableShadow(CCSize(1,-1),2.0f,true);
            aText1->setPosition(ccp(aBoxBase->getContentSize().width/2-125,48));
            aBoxBase->addChild(aText1);
            //The crystal icon
            
            //the + text
            aText1 = CCLabelTTF::create("+",
                                                    "fonts/Marker Felt.ttf",TITLE_FONT_SIZE,CCSize(60,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aText1->setColor(ccc3(255,255,255));
            aText1->enableShadow(CCSize(1,-1),2.0f,true);
            aText1->setPosition(ccp(aBoxBase->getContentSize().width/2,43));
            aBoxBase->addChild(aText1);
            
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2-60,48));
            aBoxBase->addChild(aIcon2);
            
            //the diamond
            aText1 = CCLabelTTF::create("150",
                                        "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(60,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aText1->setColor(ccc3(255,255,255));
            aText1->enableShadow(CCSize(1,-1),2.0f,true);
            aText1->setPosition(ccp(aBoxBase->getContentSize().width/2+70,48));
            aBoxBase->addChild(aText1);
            
            //Add the icon2
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2+120,48));
            aBoxBase->addChild(aIcon2);
            
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$1.99",
                                           "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        else if(i==3)
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/combodeal3.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("Crysal Doubler");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-130,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+130,aBoxBase->getContentSize().height-23));
            
            aTxtLine = CCLabelTTF::create("Double the crystal value!",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.45,CCSize(420,80),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(84, 71, 52));
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2,44));
            aBoxBase->addChild(aTxtLine);
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$2.69",
                                           "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        else if(i==4)//Some stuff
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/combodeal4.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("Combo Deal!");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-120,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+120,aBoxBase->getContentSize().height-23));
            
            //Crystal amount
            CCLabelTTF* aText1 = CCLabelTTF::create("80 000",
                                                    "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aText1->setColor(ccc3(255,255,255));
            aText1->enableShadow(CCSize(1,-1),2.0f,true);
            aText1->setPosition(ccp(aBoxBase->getContentSize().width/2-150,48));
            aBoxBase->addChild(aText1);
            //The crystal icon
            
            //the + text
            aText1 = CCLabelTTF::create("+",
                                        "fonts/Marker Felt.ttf",TITLE_FONT_SIZE,CCSize(60,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aText1->setColor(ccc3(255,255,255));
            aText1->enableShadow(CCSize(1,-1),2.0f,true);
            aText1->setPosition(ccp(aBoxBase->getContentSize().width/2-50,43));
            aBoxBase->addChild(aText1);
            
            //the diamond
            aText1 = CCLabelTTF::create("200",
                                        "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(60,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aText1->setColor(ccc3(255,255,255));
            aText1->enableShadow(CCSize(1,-1),2.0f,true);
            aText1->setPosition(ccp(aBoxBase->getContentSize().width/2-10,48));
            aBoxBase->addChild(aText1);
            
            aText1 = CCLabelTTF::create("+",
                                        "fonts/Marker Felt.ttf",TITLE_FONT_SIZE,CCSize(60,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aText1->setColor(ccc3(255,255,255));
            aText1->enableShadow(CCSize(1,-1),2.0f,true);
            aText1->setPosition(ccp(aBoxBase->getContentSize().width/2+85,43));
            aBoxBase->addChild(aText1);
            
            //Add the icon2
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2-90,48));
            aBoxBase->addChild(aIcon2);
            
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2+40,48));
            aBoxBase->addChild(aIcon2);
            
            aIcon2 = CCSprite::create("Interfeiss/store/FRESH/shop/noads.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2+160,48));
            aBoxBase->addChild(aIcon2);
            
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$7.99",
                                           "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        else if(i==5)//Crystal pack
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/crystalpack1.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("Tiny crystal pack");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-140,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+140,aBoxBase->getContentSize().height-23));
            
            //Crystal amount
            aTxtLine = CCLabelTTF::create("5 000",
                                                    "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(400,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(255,255,255));
            aTxtLine->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2-20,36));
            aBoxBase->addChild(aTxtLine);
            
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2,20));
            aBoxBase->addChild(aIcon2);
            
            float aLength = strlen(aTxtLine->getString())*0.4f;
            aIcon2->setPosition(ccp(aTxtLine->getPositionX()+aLength*25+aIcon2->getContentSize().width/2,36));
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$0.69",
                                           "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        else if(i==6)//Crystal pack
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/crystalpack2.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("Bigger crystal pack");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-150,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+150,aBoxBase->getContentSize().height-23));
            
            //Crystal amount
            aTxtLine = CCLabelTTF::create("30 000",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(400,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(255,255,255));
            aTxtLine->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2-20,36));
            aBoxBase->addChild(aTxtLine);
            
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2,20));
            aBoxBase->addChild(aIcon2);
            
            float aLength = strlen(aTxtLine->getString())*0.4f;
            aIcon2->setPosition(ccp(aTxtLine->getPositionX()+aLength*25+aIcon2->getContentSize().width/2,36));
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$1.99",
                                           "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        else if(i==7)//Crystal pack
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/crystalpack3.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("Quite huge crystal pack");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-180,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+180,aBoxBase->getContentSize().height-23));
            
            //Crystal amount
            aTxtLine = CCLabelTTF::create("100 000",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(400,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(255,255,255));
            aTxtLine->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2-20,36));
            aBoxBase->addChild(aTxtLine);
            
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2,20));
            aBoxBase->addChild(aIcon2);
            
            float aLength = strlen(aTxtLine->getString())*0.4f;
            aIcon2->setPosition(ccp(aTxtLine->getPositionX()+aLength*25+aIcon2->getContentSize().width/2,36));
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$6.99",
                                           "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        else if(i==8)//Crystal pack
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/crystalpack4.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("Dwarfload of crystals!");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-180,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+180,aBoxBase->getContentSize().height-23));
            
            //Crystal amount
            aTxtLine = CCLabelTTF::create("300 000",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(400,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(255,255,255));
            aTxtLine->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2-20,36));
            aBoxBase->addChild(aTxtLine);
            
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/crystal_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2,20));
            aBoxBase->addChild(aIcon2);
            
            float aLength = strlen(aTxtLine->getString())*0.4f;
            aIcon2->setPosition(ccp(aTxtLine->getPositionX()+aLength*25+aIcon2->getContentSize().width/2,36));
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$13.99",
                                           "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        else if(i==9)//Diamond pack
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/diamondpack1.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("Few diamonds");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-130,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+130,aBoxBase->getContentSize().height-23));
            
            //Crystal amount
            aTxtLine = CCLabelTTF::create("50",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(400,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(255,255,255));
            aTxtLine->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2-20,36));
            aBoxBase->addChild(aTxtLine);
            
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2,20));
            aBoxBase->addChild(aIcon2);
            
            float aLength = strlen(aTxtLine->getString())*0.4f;
            aIcon2->setPosition(ccp(aTxtLine->getPositionX()+aLength*25+aIcon2->getContentSize().width/2,36));
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$0.69",
                                           "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        else if(i==10)//Crystal pack
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/diamondpack2.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("More diamonds");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-155,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+155,aBoxBase->getContentSize().height-23));
            
            //Crystal amount
            aTxtLine = CCLabelTTF::create("300",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(400,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(255,255,255));
            aTxtLine->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2-20,36));
            aBoxBase->addChild(aTxtLine);
            
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2,20));
            aBoxBase->addChild(aIcon2);
            
            float aLength = strlen(aTxtLine->getString())*0.4f;
            aIcon2->setPosition(ccp(aTxtLine->getPositionX()+aLength*25+aIcon2->getContentSize().width/2,36));
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$2.99",
                                           "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        else if(i==11)//Crystal pack
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/diamondpack3.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("Even more diamonds");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-170,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+170,aBoxBase->getContentSize().height-23));
            
            //Crystal amount
            aTxtLine = CCLabelTTF::create("800",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(400,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(255,255,255));
            aTxtLine->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2-20,36));
            aBoxBase->addChild(aTxtLine);
            
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2,20));
            aBoxBase->addChild(aIcon2);
            
            float aLength = strlen(aTxtLine->getString())*0.4f;
            aIcon2->setPosition(ccp(aTxtLine->getPositionX()+aLength*25+aIcon2->getContentSize().width/2,36));
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$6.99",
                                           "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        else if(i==12)//Crystal pack
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/shop/diamondpack4.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("Epic pile of diamonds!");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(aBoxBase->getContentSize().width/2-170,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(aBoxBase->getContentSize().width/2+170,aBoxBase->getContentSize().height-23));
            
            //Crystal amount
            aTxtLine = CCLabelTTF::create("2200",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5,CCSize(400,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(255,255,255));
            aTxtLine->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2-20,36));
            aBoxBase->addChild(aTxtLine);
            
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2,20));
            aBoxBase->addChild(aIcon2);
            
            float aLength = strlen(aTxtLine->getString())*0.4f;
            aIcon2->setPosition(ccp(aTxtLine->getPositionX()+aLength*25+aIcon2->getContentSize().width/2,36));
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/shop/btn_buy_shop0001.png",
                                                 "Interfeiss/store/FRESH/shop/btn_buy_shop0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
            
            //Add some price point !!!
            aTxtPrice = CCLabelTTF::create("$13.99",
                                           "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.40,CCSize(100,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtPrice->setColor(ccc3(255, 255, 255));
            aTxtPrice->enableShadow(CCSize(1,-1),2.0f,true);
            aTxtPrice->setPosition(ccp(aBoxBase->getContentSize().width-74,18));
            aBoxBase->addChild(aTxtPrice);
        }
        
        if(aIcon)
        {
            aIcon->setPosition(aIconOffset);
            aBoxBase->addChild(aIcon);
        }
        
        if(MenuButton)
        {
            MenuButtons = CCMenu::create(MenuButton, NULL);
            MenuButtons->setPosition(aBoxBase->getContentSize().width-74,64);
            MenuButtons->setTag(311);
            aBoxBase->addChild(MenuButtons, 10);
        }
        
        aBoxBase->addChild(aTxtHeader);
        
        aIcon = NULL;
        MenuButton = NULL;
    }
}

void StoreBox::CreateFreeStuff()
{
    mBase->setVisible(false);
    mBaseFreeStuff->setVisible(false);
    mBaseShop->setVisible(false);
    
    if(mFreeStuffCreated)
    {
        mBaseFreeStuff->setVisible(true);
        return;
    }
    
    mFreeStuffCreated = true;
    mBaseFreeStuff->setVisible(true);
    
    //Add some elements
    int aX = 20;
    int aY = 440;
    int aSpaceY = 10;
    int aIndex = 1;
    
    CCSprite* aBoxBase;
    
    CCSprite* aHeader = CCSprite::create("Interfeiss/store/FRESH/free_stuff/title_freestuff.png");
    aHeader->setPosition(ccp(aX,aY+90));
    mBaseFreeStuff->addChild(aHeader,1);
    
    CCSprite* aIcon;
    CCPoint aIconOffset;
    
    CCLabelTTF* aTxtHeader = NULL;
    CCLabelTTF* aTxtLine = NULL;
    
    CCSprite* aIcon2 = NULL;
    
    CCMenu* MenuButtons;
    
    CCSprite* aSideHeader;
    CCMenuItemImage* MenuButton;
    
    mMaxScroll_Free_Y = mMinScroll_Free_Y;
    
    for(int i=1;i<5;i++)
    {
        aBoxBase = CCSprite::create("Interfeiss/store/FRESH/free_stuff/panel_freestuff.png");
        aBoxBase->setPosition(ccp(aX,aY));
        aBoxBase->setTag(i);
        mBaseFreeStuff->addChild(aBoxBase);
        aY-=aBoxBase->getContentSize().height+aSpaceY;
        
        aTxtHeader = CCLabelTTF::create("","fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6,CCSize(620,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
        aTxtHeader->setPosition(ccp(aBoxBase->getContentSize().width/2,aBoxBase->getContentSize().height-24));
        aTxtHeader->setColor(ccc3(84, 71, 52));
        
        aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
        aSideHeader->setTag(111);
        aBoxBase->addChild(aSideHeader);
        
        aSideHeader = CCSprite::create("Interfeiss/store/FRESH/text_deco.png");
        aSideHeader->setTag(222);
        aSideHeader->setFlipX(true);
        aBoxBase->addChild(aSideHeader);
        
        //Lets add some info
        if(i==1)//FB
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/free_stuff/icon_fb.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("Like Dwarf Forest on Facebook");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(100,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(585,aBoxBase->getContentSize().height-23));
            
            //Add the diamond with number
            aTxtLine = CCLabelTTF::create("+20","fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6,CCSize(360,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(255,255,255));
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2-26,48));
            aTxtLine->enableShadow(CCSize(1,-1),2.0f,true);
            aBoxBase->addChild(aTxtLine);
            
            //Add the icon2
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2+26,50));
            aBoxBase->addChild(aIcon2);
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/free_stuff/btn_like0001.png",
                                                 "Interfeiss/store/FRESH/free_stuff/btn_like0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
        }
        else if(i==2)//TWITTER
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/free_stuff/icon_twitter.png");
            aIconOffset = ccp(80,54);
            
            aTxtHeader->setString("Follow Dwarf Forest on Twitter");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(100,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(585,aBoxBase->getContentSize().height-23));
            
            //Add the diamond with number
            aTxtLine = CCLabelTTF::create("+20","fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6,CCSize(360,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(255,255,255));
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2-26,48));
            aTxtLine->enableShadow(CCSize(1,-1),2.0f,true);
            aBoxBase->addChild(aTxtLine);
            
            //Add the icon2
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2+26,50));
            aBoxBase->addChild(aIcon2);
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/free_stuff/btn_follow0001.png",
                                                 "Interfeiss/store/FRESH/free_stuff/btn_follow0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
        }
        else if(i==3)//TV
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/free_stuff/icon_tv.png");
            aIconOffset = ccp(80,64);
            
            aTxtHeader->setString("Watch awesome videos");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(160,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(525,aBoxBase->getContentSize().height-23));
            
            //Add the diamond with number
            aTxtLine = CCLabelTTF::create("+1","fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.6,CCSize(360,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(255,255,255));
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2-26,48));
            aTxtLine->enableShadow(CCSize(1,-1),2.0f,true);
            aBoxBase->addChild(aTxtLine);
            
            //Add the icon2
            aIcon2 = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
            aIcon2->setPosition(ccp(aBoxBase->getContentSize().width/2+26,50));
            aBoxBase->addChild(aIcon2);
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/free_stuff/btn_watch0001.png",
                                                 "Interfeiss/store/FRESH/free_stuff/btn_watch0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
        }
        else if(i==4)//TV
        {
            aIcon = CCSprite::create("Interfeiss/store/FRESH/free_stuff/icon_thumb.png");
            aIconOffset = ccp(80,62);
            
            aTxtHeader->setString("Check out Offerwall");
            
            aBoxBase->getChildByTag(111)->setPosition(ccp(170,aBoxBase->getContentSize().height-23));
            aBoxBase->getChildByTag(222)->setPosition(ccp(515,aBoxBase->getContentSize().height-23));
            
            aTxtLine = CCLabelTTF::create("Free stuff is waiting for you!",
                                          "fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.45,CCSize(360,40),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            aTxtLine->setColor(ccc3(84, 71, 52));
            aTxtLine->setPosition(ccp(aBoxBase->getContentSize().width/2,48));
            aBoxBase->addChild(aTxtLine);
            
            MenuButton = CCMenuItemImage::create("Interfeiss/store/FRESH/free_stuff/btn_yeeah0001.png",
                                                 "Interfeiss/store/FRESH/free_stuff/btn_yeeah0002.png",
                                                 this,
                                                 menu_selector(StoreBox::OnFreeStuff));
        }
        
        if(aIcon)
        {
            aIcon->setPosition(aIconOffset);
            aBoxBase->addChild(aIcon);
        }
        
        MenuButtons = CCMenu::create(MenuButton, NULL);
        MenuButtons->setPosition(aBoxBase->getContentSize().width-100,45);
        MenuButtons->setTag(311);
        aBoxBase->addChild(MenuButtons, 10);
        
        aBoxBase->addChild(aTxtHeader);
        
        aIcon = NULL;
    }
}


void StoreBox::OnFreeStuff(CCObject* sender)
{
    //Add some diamonds
    User::getInstance()->addDiamonds(100);
    User::getInstance()->addCrystals(1000);
}
