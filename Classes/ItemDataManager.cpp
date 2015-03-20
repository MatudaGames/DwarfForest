//
//  ItemDataManager.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 24/02/15.
//
//

#include "ItemDataManager.h"

#include "User.h"
#include "Utils.h"
#include <algorithm>

int CHARGE_DWARF_ENTER_CAVE = 30;
int CHARGE_CRYSTAL_BLUE = 20;
int CHARGE_CRYSTAL_GREEN = 10;
int CHARGE_CRYSTAL_RED = 30;
int CHARGE_CRYSTAL_YELLOW = 50;

ItemDataManager::ItemDataManager()
{
    mReDownload = false;
}

//....................................................................................................
// File download stuff & Helpers

size_t static pWriteCallback(void *pData, size_t n, size_t nDataSize, FILE *stream)
{
    size_t nWritten = fwrite(pData,n,nDataSize,(FILE *)stream);
    return nWritten;
}

int static DownProgresss(void* clientp,double fDownLoadTotal,double fDownLoaded,double fUpTotal,double fUpLoaded)
{
    if (fDownLoaded >= 0 && fDownLoadTotal != 0)
        CCLOG("%6.2f%%\n", 100*fDownLoaded/fDownLoadTotal);//Download percentage
    return 0;
}

static bool sortByID_Spells(SpellInfo a, SpellInfo b)
{
    return a.order_index < b.order_index;
}

static bool sortByID_Powers(PowerInfo a, PowerInfo b)
{
    return a.order_index < b.order_index;
}

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

std::vector<int> ItemDataManager::SplitString(const std::string s,char delim){
    std::vector<int> elems;
    split(s, delim, elems);
    return elems;
}

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

std::vector<std::string> ItemDataManager::SplitString_VecString(const std::string s,char delim){
    std::vector<std::string> elems;
    splitString(s, delim, elems);
    return elems;
}

//....................................................................................................

//....................................................................................................
// Get the data file from web

void ItemDataManager::OnDownloadData()
{
    CURL *pCurl;
    CURLcode nResCode;
    
    pCurl = curl_easy_init();//Initialize the CURL has initialized after the success of the CURL pointer
    if (pCurl != NULL)
    {
        std::string saveFileName;
        saveFileName = "DF_Data.plist";
        saveFileName = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + saveFileName;
        
        pFile = fopen(saveFileName.c_str(), "wb");
        
        // The file from web [in release, lets put this file on server private !!!]
        curl_easy_setopt(pCurl,CURLOPT_URL,"https://www.dropbox.com/s/u8x8e2o35rmpcxs/DF_Items.xml?dl=1");
        if(pFile != NULL)
        {
            curl_easy_setopt(pCurl,CURLOPT_FILE,pFile);                   //The specified file write
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, pWriteCallback);//Callback function to write data
            curl_easy_setopt(pCurl, CURLOPT_VERBOSE, true);                //Let CURL report every suddenness
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 30);                  //Setting the timeout
            curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS,0L);
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, DownProgresss);//Specify a callback function
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER,false);
            curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, true);
            nResCode = curl_easy_perform(pCurl);//Executing the above a set operation and return a status code
            curl_easy_cleanup(pCurl);           //Release the related resources
            fclose(pFile);
            OnFailToLoad(nResCode);
        }
    }
}

void ItemDataManager::OnFailToLoad(CURLcode code)
{
    //    code == CURLE_OK ? CCLOG("DownLoad Success") : CCLOG("CODE: %d",code);
    OnDownloadedData();
}

//....................................................................................................

// The data parser checker if valid stuff

void ItemDataManager::OnDownloadedData()
{
    // Start with local file check - then if we have local - try to check if downloaded file exists and is newer
    bool _doWeHaveLocalFile = false;
    std::string path_local = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+"DF_Data_Local.plist";
    if(cocos2d::CCFileUtils::sharedFileUtils()->isFileExist(path_local))
    {
        //Safe check
        cocos2d::CCDictionary* pRet = cocos2d::CCDictionary::createWithContentsOfFileThreadSafe(path_local.c_str());
        if(pRet != NULL){
            // We have local file !!!
            _doWeHaveLocalFile = true;
        }
    }
    
    // Now that we have local file - we can check futher whats with out downloaded file !!!
    bool _doWeHaveDownloadedFile = false;
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+"DF_Data.plist";
    if(cocos2d::CCFileUtils::sharedFileUtils()->isFileExist(path))
    {
        // We have donwloaded something !!!
        cocos2d::CCDictionary* pRet = cocos2d::CCDictionary::createWithContentsOfFileThreadSafe(path.c_str());
        if(pRet != NULL){
            _doWeHaveDownloadedFile = true;
            
            // Update the local file !!!
            FILE *pWriteDummy = fopen(path_local.c_str(), "w+");
            
            FILE *pFile = fopen(path.c_str(),"r+");
            fseek(pFile,0,SEEK_END);
            int size = ftell(pFile);
            
            fclose(pFile);
            
            //ReOpen it?
            pFile = fopen(path.c_str(), "r+");
            
            char buffer [size];
            
            while ( ! feof (pFile) )
            {
                fgets (buffer , size , pFile);
                fputs (buffer , pWriteDummy);
            }
            
            fclose(pFile);
            fclose(pWriteDummy);
            
            _doWeHaveLocalFile = true;
        }
    }
    
    // Get a internet connection for 1st time xml file downloads !!!
    if(_doWeHaveLocalFile == false){
        CCLOG("!!! We have no data files !!!");
        
        //Show popup that need internet connection to continue !!!
        CreateNoInternet();
        return;
    }
    
    cocos2d::CCDictionary* plistDictionary = cocos2d::CCDictionary::createWithContentsOfFile(path_local.c_str());
    
    if(plistDictionary == NULL)
    {
        // Problem - mission file down?
        CreateNoInternet();
        return;
    }
    
    // All cool - data file donwloaded or taked older one !!!
    CCLog("+++ Downloaded Data Stuff - Parse it now +++");
    
    // Let the magic begin !!!
    cocos2d::CCDictElement* pElement = NULL;
    cocos2d::CCDictElement* pSubElement = NULL;
    
    cocos2d::CCDICT_FOREACH(plistDictionary, pElement)
    {
        cocos2d::CCDictionary* missionDict = (cocos2d::CCDictionary*)pElement->getObject();
        std::string aPathValue = pElement->getStrKey();
        CCLog("aPathValue: %s",aPathValue.c_str());
        
        if(aPathValue.compare("Spells") == 0)
        {
            // Do all the Spell magic
            cocos2d::CCDICT_FOREACH(missionDict, pSubElement)
            {
                cocos2d::CCDictionary* subDict = (cocos2d::CCDictionary*)pSubElement->getObject();
                std::string aSubPathValue = pSubElement->getStrKey();
                CCLog("aSubPathValue: %s",aSubPathValue.c_str());
                
                // Add all the spells to the magic
                SpellInfo* aSpellInfo = new SpellInfo();
                
                // The item id
                aSpellInfo->id = subDict->valueForKey("ID")->intValue()+100;//All spells have id from 100
                
                aSpellInfo->name = aSubPathValue;
                aSpellInfo->charge = subDict->valueForKey("Charge")->intValue();
                aSpellInfo->damage = subDict->valueForKey("Damage")->intValue();
                aSpellInfo->order_index = subDict->valueForKey("Order_ID")->intValue();
                
                aSpellInfo->price_crystals = subDict->valueForKey("Price_Crystals")->intValue();
                aSpellInfo->price_diamonds = subDict->valueForKey("Price_Diamonds")->intValue();
                
                // The spell unocl stuff
                if(subDict->valueForKey("UnlockInfo")->compare("") != 0)
                {
                    // We have some unlock info
                    aSpellInfo->unlock_info = subDict->valueForKey("UnlockInfo")->m_sString;
                }
                
                if(subDict->valueForKey("Damage_Extra")->compare("") != 0)
                {
                    // We have posion or other spell
                    std::vector<int> extraDamage = SplitString(subDict->valueForKey("Damage_Extra")->getCString(),',');
                    
                    aSpellInfo->damage_extra = extraDamage[0];
                    aSpellInfo->damage_extra_multiply = extraDamage[1];
                    aSpellInfo->damage_extra_time = extraDamage[2];
                }
                
                aSpellInfo->range = subDict->valueForKey("Range")->intValue();
                
                aSpellInfo->icon_path = subDict->valueForKey("Icon_Path")->m_sString;
                
                mSpellDataVector.push_back(*aSpellInfo);
            }
            
            std::sort(mSpellDataVector.begin(), mSpellDataVector.end(),sortByID_Spells);
        }
        else if(aPathValue.compare("Powerups") == 0)
        {
            // Do all the Powerup magic
            cocos2d::CCDICT_FOREACH(missionDict, pSubElement)
            {
                cocos2d::CCDictionary* subDict = (cocos2d::CCDictionary*)pSubElement->getObject();
                std::string aSubPathValue = pSubElement->getStrKey();
                CCLog("aSubPathValue: %s",aSubPathValue.c_str());
                
                PowerInfo* aPowerInfo = new PowerInfo();
                
                // The item id
                aPowerInfo->id = subDict->valueForKey("ID")->intValue()+1000;
                
                aPowerInfo->upgrade_power = SplitString(subDict->valueForKey("Upgrade_Info")->getCString(),',');
                
                aPowerInfo->name = aSubPathValue;
                aPowerInfo->level_unlock = subDict->valueForKey("Level_Unlock")->intValue();
                aPowerInfo->level_cost = SplitString_VecString(subDict->valueForKey("Level_Cost")->getCString(),',');
                aPowerInfo->max_upgrades = aPowerInfo->level_cost.size();
                
                aPowerInfo->text_locked = subDict->valueForKey("Text_Locked")->m_sString;
                aPowerInfo->text_unlocked = subDict->valueForKey("Text_UnLocked")->m_sString;
                aPowerInfo->text_upgrade = subDict->valueForKey("Text_Upgrade")->m_sString;
                
                // The universal stuff
                aPowerInfo->icon_change = false;
                if(subDict->valueForKey("Icon_Change")->compare("") != 0) {
                    aPowerInfo->icon_change = subDict->valueForKey("Icon_Change")->boolValue();
                }
                
                aPowerInfo->icon_path = subDict->valueForKey("Icon_Path")->m_sString;
                
                mPowerDataVector.push_back(*aPowerInfo);
            }
            
            std::sort(mPowerDataVector.begin(), mPowerDataVector.end(),sortByID_Powers);
        }
        else if(aPathValue.compare("GlobalValues") == 0)
        {
            // Some default values that can be changed if needed !!!
            CCLog("CHARGE_DWARF_ENTER_CAVE Before: %i",CHARGE_DWARF_ENTER_CAVE);
            
            if(missionDict->valueForKey("CHARGE_DWARF_ENTER_CAVE")->compare("") != 0){
                CHARGE_DWARF_ENTER_CAVE = missionDict->valueForKey("CHARGE_DWARF_ENTER_CAVE")->intValue();
                CCLog("CHARGE_DWARF_ENTER_CAVE After: %i",CHARGE_DWARF_ENTER_CAVE);
            }
            if(missionDict->valueForKey("CHARGE_CRYSTAL_BLUE")->compare("") != 0){
                CHARGE_CRYSTAL_BLUE = missionDict->valueForKey("CHARGE_CRYSTAL_BLUE")->intValue();
            }
            if(missionDict->valueForKey("CHARGE_CRYSTAL_GREEN")->compare("") != 0){
                CHARGE_CRYSTAL_GREEN = missionDict->valueForKey("CHARGE_CRYSTAL_GREEN")->intValue();
            }
            if(missionDict->valueForKey("CHARGE_CRYSTAL_RED")->compare("") != 0){
                CHARGE_CRYSTAL_RED = missionDict->valueForKey("CHARGE_CRYSTAL_RED")->intValue();
            }
            if(missionDict->valueForKey("CHARGE_CRYSTAL_YELLOW")->compare("") != 0){
                CHARGE_CRYSTAL_YELLOW = missionDict->valueForKey("CHARGE_CRYSTAL_YELLOW")->intValue();
            }
        }
    }
    
//    CCLog("mPowerDataVector size %lu",mPowerDataVector.size());
//    CCLog("mSpellDataVector size %lu",mSpellDataVector.size());
    
    CCLog("All done !!!");
    
    mAllFinished = true;
    
    if(mReDownload)
    {
        cocos2d::CCMessageBox("Data Download Complete !!!","Continue");
    }
    else
    {
        mReDownload = true;
        
        cocos2d::CCScene* aScene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
        LoadingScreen* aLoading = static_cast<LoadingScreen*>(aScene->getChildByTag(888));
        aLoading->OnItemDataDownloaded();
    }
}

// The no file popup - TODO - Add retry button !!!
void ItemDataManager::CreateNoInternet()
{
    cocos2d::CCLayerColor* aBG = cocos2d::CCLayerColor::create(cocos2d::ccc4(0,0,0,128),
                                                               cocos2d::CCDirector::sharedDirector()->getVisibleSize().width,
                                                               cocos2d::CCDirector::sharedDirector()->getVisibleSize().height);
    
    // Create some popup about this error
    //Add some button
    cocos2d::CCSprite* popup = cocos2d::CCSprite::create("Interfeiss/before_quit/dont_leave.png");
    popup->setPosition(ccp(aBG->getContentSize().width/2,aBG->getContentSize().height/2));
    aBG->addChild(popup);
    
    cocos2d::CCMenuItemImage* okBtn = cocos2d::CCMenuItemImage::create("Interfeiss/before_quit/check_btn0001.png",
                                                                       "Interfeiss/before_quit/check_btn0002.png",
                                                                       aBG,
                                                                       menu_selector(ItemDataManager::menuCloseCallback_mission));
    
    cocos2d::CCMenu* menu = cocos2d::CCMenu::create(okBtn, NULL);
    menu->alignItemsHorizontally();
    menu->setPosition(ccp(465, 262));
    popup->addChild(menu);
    
    cocos2d::CCScene* aScene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
    aScene->addChild(aBG);
}

// Show the popup and exit game - because need to be connected at least once !!!
void ItemDataManager::menuCloseCallback_mission()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    cocos2d::CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
}

// The power info/finfo
bool ItemDataManager::isPowerItemUnlocked(int theID)
{
    CCLog("mBoghtPowers: %s",User::getInstance()->mBoghtPowers.c_str());
    
    std::vector<int> boughtStuff = SplitString(User::getInstance()->mBoghtPowers,',');
    if(std::find(boughtStuff.begin(), boughtStuff.end(), theID) != boughtStuff.end()){
        // We found it
        return true;
    }
    
    // TODO - check levels
    
    return false;
}

int ItemDataManager::getPowerItemLevel(int theID)
{
    std::vector<std::string> boughtStuff = SplitString_VecString(User::getInstance()->mPowerInfo,',');
    
    for(int i=0;i<boughtStuff.size();i++)
    {
        //Sub split it more
        std::vector<int> subPowa = SplitString(boughtStuff[i],'=');
        if(subPowa.size()>0)
        {
            if(subPowa[0] == theID){
                return subPowa[1];
            }
        }
    }
    
    return 1;// As default
}

void ItemDataManager::upgradePowerItem(int theID)
{
    std::stringstream theSaveData;
    std::vector<std::string> boughtStuff = SplitString_VecString(User::getInstance()->mPowerInfo,',');
    
    CCLog("Save data before power upgrade: %s",User::getInstance()->mPowerInfo.c_str());
    
    bool didFindItemForUpgrade = false;
    
    for(int i=0;i<boughtStuff.size();i++)
    {
        //Sub split it more
        std::vector<int> subPowa = SplitString(boughtStuff[i],'=');
        if(subPowa.size()>0)
        {
            if(subPowa[0] == theID){
                subPowa[1] += 1;
                didFindItemForUpgrade = true;
            }
            
            theSaveData << subPowa[0] << "=" << subPowa[1] << ",";
        }
    }
    
    // Add new stuff
    if(didFindItemForUpgrade == false)
    {
        theSaveData<<theID<<"=2,";
        User::getInstance()->mPowerInfo.append(theSaveData.str().c_str());
    }
    else
    {
        User::getInstance()->mPowerInfo = theSaveData.str().c_str();
    }
    
    // Save data to local stuff
    CCLog("Will save data about power: %s",theSaveData.str().c_str());
    
    cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("Powers_Info", theSaveData.str().c_str());
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

// The chekers and other nice stuff !!!
bool ItemDataManager::isItemUnlocked(int theID)
{
    // Check if user has bought it or earned
//    mBoghtSpells
    
    std::vector<int> boughtStuff = SplitString(User::getInstance()->mBoghtSpells,',');
    if(std::find(boughtStuff.begin(), boughtStuff.end(), theID) != boughtStuff.end()){
        // We found it
        return true;
    }
    
    // Lets check the level !!!
    for(int i=0;i<mSpellDataVector.size();i++)
    {
        if(mSpellDataVector[i].id == theID)
        {
            // Check if user level/mission has unlocked it !!!
            // TODO
            
            /*
            std::string theUnlockInfo = subDict->valueForKey("UnlockInfo")->m_sString;
            std::vector<std::string> theSplitInfo = SplitString_VecString(theUnlockInfo,'=');
            if(theSplitInfo[0] == "mission")
            {
                // The mission unlock stuff !!!
            }
            
            return true;
            */
        }
    }
    
    return false;
}

// For item buy functional goes here?
bool ItemDataManager::isItemActive(int theID)
{
    std::vector<int> activeSpells = SplitString(User::getInstance()->mActiveSpells,',');
    if(std::find(activeSpells.begin(), activeSpells.end(), theID) != activeSpells.end()){
        return true;
    }
    
    return false;
}

void ItemDataManager::onSetSelectedItem(int theType, int theID)
{
    std::vector<int> activeSpells = SplitString(User::getInstance()->mActiveSpells,',');
    // Remove the 1st and add the last
    std::stringstream theSaveData;
    // If want to select 2 items
//    theSaveData << activeSpells[1] << "," << theID;
    // For now only 1 possible to select
    theSaveData << theID;
    
    // Replace current stuff
    User::getInstance()->mActiveSpells = theSaveData.str();
    
    cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("Spells_Active", theSaveData.str().c_str());
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
    CCLog("CurrentData of spells active %s",User::getInstance()->mActiveSpells.c_str());
}

void ItemDataManager::onRemoveSelectedItem(int theType, int theID)
{
    std::vector<int> activeSpells = SplitString(User::getInstance()->mActiveSpells,',');
    
    // For now only 2 possible to activate :)
    std::stringstream theSaveData;
    
    /*
    if(activeSpells[0] == theID)
    {
        theSaveData << "0" << "," << activeSpells[1];
    }
    else
    {
        theSaveData << "0" << "," << activeSpells[0];
    }
    */
    // For 1 possible active
    theSaveData << "0";
    
    // Replace current stuff
    User::getInstance()->mActiveSpells = theSaveData.str();
    
    cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("Spells_Active", theSaveData.str().c_str());
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
    CCLog("CurrentData of spells active %s",User::getInstance()->mActiveSpells.c_str());
}

// theType - what items [spell or powers]
void ItemDataManager::onPurchaseItem(int theType,int theID)
{
    std::stringstream theNewData;
    theNewData << "," << theID;
    
    if(theType == SHOP_SPELLS)
    {
        // The spells
        User::getInstance()->mBoghtSpells.append(theNewData.str());
        CCLog("CurrentData of spells bought %s",User::getInstance()->mBoghtSpells.c_str());
        
        cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("Spells_Bought", User::getInstance()->mBoghtSpells.c_str());
    }
    else if(theType == SHOP_POWERS)
    {
        // The powers
        User::getInstance()->mBoghtPowers.append(theNewData.str());
        CCLog("CurrentData of powers bought %s",User::getInstance()->mBoghtPowers.c_str());
        
        cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("Powers_Bought", User::getInstance()->mBoghtPowers.c_str());
    }
    
    // Save it now on device
//    cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("Spells_Bought", User::getInstance()->mBoghtSpells.c_str());
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

SpellInfo ItemDataManager::getSpellByID(int theID)
{
    SpellInfo *aSpell = NULL;
    
    for(int i=0;i<mSpellDataVector.size();i++)
    {
        if(mSpellDataVector[i].id == theID)
        {
            aSpell = &mSpellDataVector[i];
            break;
        }
    }
    
    return *aSpell;
}

PowerInfo ItemDataManager::getPowerByID(int theID)
{
    PowerInfo *aPower = NULL;
    
    for(int i=0;i<mPowerDataVector.size();i++)
    {
        if(mPowerDataVector[i].id == theID)
        {
            aPower = &mPowerDataVector[i];
            break;
        }
    }
    
    return *aPower;
}

std::vector<int> ItemDataManager::getActiveItems()
{
    std::vector<int> theReturn = SplitString(User::getInstance()->mActiveSpells,',');
    std::vector<int> theRealReturn;
    
    // Do not add 0
    for(int i=0;i<theReturn.size();i++)
    {
        if(theReturn[i] != 0){
            theRealReturn.push_back(theReturn[i]);
        }
    }
    
    return theRealReturn;
}




