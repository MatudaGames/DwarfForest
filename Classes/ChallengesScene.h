//
//  ChallengesScene.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 09/03/14.
//
//

#pragma once

#include <cocos2d.h>
#include <string.h>
#include "WeeklyChallenges.h"

class SpinBar;

class ChallengeProgressBar: public cocos2d::CCLayer
{
public:
    static ChallengeProgressBar* create(std::string text, unsigned int max, unsigned int value, int rewardAmount, WeeklyTask::RewardType rewardType);
    
    virtual bool init(std::string text, unsigned int max, unsigned int value, int rewardAmount, WeeklyTask::RewardType rewardType);
    
    void update(std::string text, unsigned int max, unsigned int value, int rewardAmount, WeeklyTask::RewardType rewardType);
    
private:
    
    cocos2d::CCSprite* progress;
    cocos2d::CCLabelTTF* titleSh;
    cocos2d::CCLabelTTF* title;
    cocos2d::CCLabelTTF* pointsSh;
    cocos2d::CCLabelTTF* points;
    cocos2d::CCLabelTTF* rewardSh;
    cocos2d::CCLabelTTF* reward;
    cocos2d::CCSprite* icon;
};

class ChallengesScene: public cocos2d::CCLayer
{
public:
	// implement the "static node()" method manually
    CREATE_FUNC(ChallengesScene);
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    void menuBackCallback(CCObject* sender);
    void menuWeeklyCallback(CCObject* sender);
    
    void SpinTheBars();
    void EmptyHandlePull();
    void HideOneSpinBubble();
    
    void OnFinishedEnter();
    
    //----------------------------------
    //Quick easter stuff
    
    cocos2d::CCLabelTTF* _EasterSpeach;
    cocos2d::CCLabelTTF* _EasterProgress;
    cocos2d::CCLabelTTF* _EasterReward;
    cocos2d::CCLabelTTF* _EasterTimeLeft;
    
    cocos2d::CCLabelTTF* _EasterTime_Days_1;
    cocos2d::CCLabelTTF* _EasterTime_Days_2;
    
    cocos2d::CCLabelTTF* _EasterTime_Hours_1;
    cocos2d::CCLabelTTF* _EasterTime_Hours_2;
    
    cocos2d::CCLabelTTF* _EasterTime_Minutes_1;
    cocos2d::CCLabelTTF* _EasterTime_Minutes_2;
    
    cocos2d::CCLabelTTF* _EasterTime_Seconds_1;
    cocos2d::CCLabelTTF* _EasterTime_Seconds_2;
    
    //----------------------------------
    
//    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
//    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
//    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    bool mDidFinishEnter;
    
    virtual void onEnter();
    virtual void onExit();
    
    void RemovePopup();
    void RealRemove();
    
    void update();
    
    cocos2d::CCSprite* aBG2;
    
    void OnTab1(bool aFirstTime);
    void OnTab2();
    
    void SetActiveTab(int theType);
    int mActiveTab;
    
    cocos2d::CCSprite* mWeeklyBase;
    cocos2d::CCSprite* mDailyBase;
    
    void ShowWeekly();
    void ShowDaily();
    
    virtual void update(float delta);
    
    
private:
    cocos2d::CCSprite* spinBubble;
    cocos2d::CCSprite* collectBubble;
    cocos2d::CCSprite* comebackBubble;
    cocos2d::CCSprite* oneSpinBubble;
    
    cocos2d::CCSprite* diamondBox;
    
    SpinBar* bar1;
    SpinBar* bar2;
    SpinBar* bar3;
    
    cocos2d::CCLabelTTF* daysN1;
    cocos2d::CCLabelTTF* daysN2;
    
    cocos2d::CCLabelTTF* footerText;
    cocos2d::CCLabelTTF* footerText6;
    
    void dropDiamonds();
};

class SpinBar: public cocos2d::CCLayer
{
public:
    
    SpinBar();
    ~SpinBar();
    
	struct SpinPosition
	{
		SpinPosition(): type(CRYSTAL_COLOR_BLUE), count(0) {}
		SpinPosition(CrystalColor col, unsigned int count): type(col), count(count)
		{}
		CrystalColor type;
		unsigned int count;
	};
    
	CREATE_FUNC(SpinBar);
    
	virtual bool init();
    
	// after spin, there is set result
	void spin(int order);
    void resestSlide();
    void finalizeSpin();
    
    void setSpinPosition(CrystalColor col, uint32_t count);
    
	CrystalColor getCrystalColor();
	unsigned int getCrystalCount();
    
	cocos2d::CCLayer* slide;
    cocos2d::CCGLProgram* boxShader1;
    cocos2d::CCGLProgram* boxShader2;
    
    virtual void update(float delta);
    
    int sh1_minyLoc;
    int sh1_maxyLoc;
    int sh2_minyLoc;
    int sh2_maxyLoc;
    
    float sh1_miny;
    float sh1_maxy;
    float sh2_miny;
    float sh2_maxy;
    
private:
	SpinPosition positions[14];
    
	void setSpinPosition(unsigned int p);
    int getYFromPos(unsigned int p);
    
    int resultingPosition;
    
    
};

class HandleBar: public cocos2d::CCLayer
{
public:
    CREATE_FUNC(HandleBar);
    
    virtual bool init();
private:
    cocos2d::CCSprite* handle;
    
    bool moveStarted;
    cocos2d::CCPoint dragStart;
    cocos2d::CCSprite* shadow;
    
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void onEnter();
    void onExit();
    
    void onAnimationEnded();
};

class FallingDiamond: public cocos2d::CCLayer
{
public:
    
    CREATE_FUNC(FallingDiamond);
    
    cocos2d::CCSprite* diamond;
    
    virtual bool init();
};