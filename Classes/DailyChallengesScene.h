#pragma once

#include <cocos2d.h>
#include <ctime>
#include "Crystal.h"

class SpinBar;

class DailyChallengesScene: public cocos2d::CCLayer
{
public:
	CREATE_FUNC(DailyChallengesScene);

    virtual bool init();

    static cocos2d::CCScene* scene();

	void menuBackCallback(CCObject* sender);
    void menuWeeklyCallback(CCObject* sender);
    
    void SpinTheBars();
    void EmptyHandlePull();
    void HideOneSpinBubble();
    
    void onEnter();
    
    // on open update
    void update();
    void keyBackClicked();
    
    // frame update
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

/*
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
*/
