//
//  DailyChallengesPopUp.h
//  DwarfForest
//
//  Created by Agris Dumins on 9/17/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>
#include "SpriteAnimation.h"
#include "DailyChallengesScene.h"
#include "GameStats.h"

class SmallSpinBar;

class DailyChallengesPopUp: public cocos2d::CCLayer
{
public:
	CREATE_FUNC(DailyChallengesPopUp);
    
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    
    void SpinTheBars();
    void EmptyHandlePull();
    void ShowPlay();
    void Play();
    void Spin();
    
    void RealRemove();
    
private:
    
    cocos2d::CCSprite* diamondBox;
    
    SmallSpinBar* bar1;
    SmallSpinBar* bar2;
    SmallSpinBar* bar3;
    
    cocos2d::CCMenu* spinBt;
    SpriteAnimation* spinAnim;
    
    void dropDiamonds();
};

class SmallSpinBar: public cocos2d::CCLayer
{
public:
    
    SmallSpinBar();
    
	struct SpinPosition
	{
		SpinPosition(): type(CRYSTAL_COLOR_BLUE), count(0) {}
		SpinPosition(CrystalColor col, unsigned int count): type(col), count(count)
		{}
		CrystalColor type;
		unsigned int count;
	};
    
	CREATE_FUNC(SmallSpinBar);
    
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

class SmallHandleBar: public cocos2d::CCLayer
{
public:
    CREATE_FUNC(SmallHandleBar);
    
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

class SmallFallingDiamond: public cocos2d::CCLayer
{
public:
    
    CREATE_FUNC(SmallFallingDiamond);
    
    cocos2d::CCSprite* diamond;
    
    virtual bool init();
};

class DailyChallengesInGamePopUp: public cocos2d::CCLayer
{
public:
    static DailyChallengesInGamePopUp* create(uint32_t reward);
    
    bool init(uint32_t reward);
    
    void onEnter();
    void onExit();
    
    void showCheck();
    void showSpin();
    void showReward();
    void remove();
    
private:
    cocos2d::CCSprite* back;
    SpriteAnimation* check;
    SpriteAnimation* spin;
    cocos2d::CCLayer* diamonds;
    
    uint32_t reward;
};

class DailyChallengesInGameInfo: public cocos2d::CCLayer
{
public:
    static DailyChallengesInGameInfo* create(const DF::Stats::GameStats& stats);
    
    bool init(const DF::Stats::GameStats& stats);
    
    void updateNumbers(const DF::Stats::GameStats& stats);
    
    void onEnter();
    void onExit();
    
    bool isActive();
    
    void afterShow();
    void beforeShow();
    
    void remove();
    void destroy();
    
private:
    
    uint32_t waitCount;
    bool active;
    
    cocos2d::CCLayer* popup;
    cocos2d::CCLabelTTF* b1;
    cocos2d::CCLabelTTF* b2;
    cocos2d::CCLabelTTF* b3;
};
