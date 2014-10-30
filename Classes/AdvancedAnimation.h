//
//  AdvancedAnimation.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 30/10/14.
//
//

#pragma once

#include <cocos2d.h>

class AdvancedAnimation: public cocos2d::CCSprite
{
public:
    static AdvancedAnimation* create(const char* plist, bool repeat = true);
    
    AdvancedAnimation();
    virtual ~AdvancedAnimation();
    
    virtual bool init(const char* plist, bool repeat);
    
    virtual void update(float delta);
    virtual void onEnter();
    virtual void onExit();
    
    int currentFrame;
    float frameFPS_Current;
    float frameFPS_Change;
    int totalFrames;
    
    bool mClouseAnim;
    bool mOpenAnim;
    
    cocos2d::CCArray* animFrames;
    
    cocos2d::CCAnimation* animation;
    
    void pause();
    void resume();
    
    bool _paused;
    
private:
    
    
};