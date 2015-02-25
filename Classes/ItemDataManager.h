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

using namespace cocos2d;

struct SpellInfo
{
    std::string name;
    int charge;
    int price_crystals;
    int price_diamonds;
    int damage;
    int range;
    
    std::string icon_path;
    
    // If want in shop some order - use this
    int order_index;
};

struct PowerInfo
{
    std::string name;
    
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
    
    FILE *pFile;
    
    void OnDownloadData();
    void OnDownloadedData();
    void OnFailToLoad(CURLcode code);
    void CreateNoInternet();
    void menuCloseCallback_mission();
    
    // If want to redownload for debug
    bool mReDownload;
    
private:
    bool mAllFinished;
};