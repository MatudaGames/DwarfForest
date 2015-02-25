//
//  LoadingScreen.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 07/02/14.
//
//

#pragma once

#include <cocos2d.h>

/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "fmod.hpp"
#include "fmod_errors.h"
#endif
*/

class LoadingScreen: public cocos2d::CCLayer
{
public:
	// implement the "static node()" method manually
    CREATE_FUNC(LoadingScreen);
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void update(float delta);
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    cocos2d::CCSprite* mLoadingBar;
    
    void onPreloadImages();
    void onPreloadSounds();
    
    void AllCompleted();
    
    void StartLoading();
    
    //Debug stuff
    
    void AddExtraDwarf();
    void RemoveDwarf();
    
    
    virtual void onEnter();
    virtual void onExit();
    
    bool mMissionMiscLoopFix;
    bool mResourceLoadingCompleted;
    bool mMissionLoadingCompleted;
    
    bool mItemDataCompleted;
    
    void OnMissionsLoaded();
    void OnContinueGameInit();
    void OnItemDataDownloaded();
    
    /*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FMOD::System     *system;
    FMOD::Sound      *sound1, *sound2, *sound3, *sound4;
    FMOD::Channel    *channel = 0;
    FMOD::Channel    *channel1 = 0;
    FMOD::Channel    *channel2 = 0;
    FMOD::Channel    *channel3 = 0;
    FMOD_RESULT       result;
#endif
    */
    
    bool mMusic1_On;
    bool mMusic2_On;
    
    bool mMusic2_Done;
    bool mMusic1_Done;
    
    void CreateRandomFX();
    
private:
	//FB STUFF - FOR NOW
};
