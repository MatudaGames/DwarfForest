//
//  ItemDataManager.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 24/02/15.
//
//

#pragma once

#include "cocos2d.h"

#include "LoadingScreen.h"

#include "stdio.h"
#include "stdlib.h"
#include "curl/curl.h"

#define SHOP_SPELLS 1
#define SHOP_POWERS 2

using namespace cocos2d;

struct SpellInfo
{
    std::string name;
    int charge;
    int price_crystals;
    int price_diamonds;
    int damage;
    int range;
    int id;
    
    // The new stuff for possions
    int damage_extra;
    int damage_extra_time;
    int damage_extra_multiply;
    
    std::string icon_path;
    
    std::string unlock_info;
    
    // If want in shop some order - use this
    int order_index;
};

struct PowerInfo
{
    std::string name;
    int id;
    
    std::string icon_path;
    
    std::vector<int> level_cost;
    int level_unlock;
    
    // If want in shop some order - use this
    int order_index;
};

class ItemDataManager
{
public:
    ItemDataManager();
    
    std::vector<SpellInfo> mSpellDataVector;
    std::vector<PowerInfo> mPowerDataVector;
    
    std::vector<int> SplitString(const std::string s,char delim);
    std::vector<std::string> SplitString_VecString(const std::string s,char delim);
    
    FILE *pFile;
    
    void OnDownloadData();
    void OnDownloadedData();
    void OnFailToLoad(CURLcode code);
    void CreateNoInternet();
    void menuCloseCallback_mission();
    
    bool isItemUnlocked(int theID);
    bool isItemActive(int theID);
    void onPurchaseItem(int theType,int theID);
    void onSetSelectedItem(int theType, int theID);
    void onRemoveSelectedItem(int theType, int theID);
    
    std::vector<int> getActiveItems();
    SpellInfo getSpellByID(int theID);
    
    // If want to redownload for debug
    bool mReDownload;
    
private:
    bool mAllFinished;
};