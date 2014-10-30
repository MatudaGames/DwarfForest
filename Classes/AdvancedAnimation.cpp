//
//  AdvancedAnimation.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 30/10/14.
//
//

#include "AdvancedAnimation.h"
#include <algorithm>

USING_NS_CC;
using namespace std;

AdvancedAnimation* AdvancedAnimation::create(const char* plist, bool repeat)
{
    AdvancedAnimation *pRet = new AdvancedAnimation();
    if (pRet && pRet->init(plist, repeat))
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

AdvancedAnimation::AdvancedAnimation():
animFrames(NULL)
{
}

AdvancedAnimation::~AdvancedAnimation()
{
    if (animFrames) animFrames->release();
}

static bool compare(const CCObject* a, const CCObject* b)
{
    return (((CCString*)a)->m_sString < ((CCString*)b)->m_sString);
}

bool AdvancedAnimation::init(const char* plist, bool repeat)
{
    if (!CCSprite::init())
    {
        return false;
    }
    
    _paused = false;
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile(plist);
    
    CCDictionary *dict = CCDictionary::createWithContentsOfFile(plist);
    CCDictionary *framesDict = (CCDictionary*)dict->objectForKey("frames");
    
    CCArray* frameNames = framesDict->allKeys();
    std::sort(frameNames->data->arr, frameNames->data->arr + frameNames->data->num, compare);
    
    animFrames = CCArray::createWithCapacity(framesDict->count());
    animFrames->retain();
    
    CCObject* frameName;
    CCARRAY_FOREACH(frameNames, frameName)
    {
        CCSpriteFrame* frame = cache->spriteFrameByName(((CCString*)frameName)->getCString());
        animFrames->addObject(frame);
    }
    
    animation = CCAnimation::createWithSpriteFrames(animFrames, 0.05f);
    animation->setLoops(1);
    
    //Set the fps
    frameFPS_Current = 0;
    frameFPS_Change = 0.03;
    
    setDisplayFrame((CCSpriteFrame *)animFrames->objectAtIndex(0));
    
    currentFrame = 0;
    totalFrames = animation->getFrames()->count();
    
    mClouseAnim = false;
    mOpenAnim = false;
    
    return true;
}

void AdvancedAnimation::pause()
{
    _paused = true;
    pauseSchedulerAndActions();
}

void AdvancedAnimation::resume()
{
    _paused = false;
    resumeSchedulerAndActions();
}

void AdvancedAnimation::update(float delta)
{
    if(animFrames == NULL || animFrames->count()==0){
        return;
    }
    
    if(mClouseAnim){
        if(currentFrame>0){
            frameFPS_Current+=delta;
            if(frameFPS_Current>=frameFPS_Change){
                currentFrame-=1;
                setDisplayFrame((CCSpriteFrame *)animFrames->objectAtIndex(currentFrame));
                if(currentFrame<=0){
                    mClouseAnim = false;
                }
            }
        }
    }
    else if(mOpenAnim){
        if(currentFrame<totalFrames/2){
            frameFPS_Current+=delta;
            if(frameFPS_Current>=frameFPS_Change){
                currentFrame+=1;
                setDisplayFrame((CCSpriteFrame *)animFrames->objectAtIndex(currentFrame));
                if(currentFrame>=totalFrames/2){
                    mOpenAnim = false;
                }
            }
        }
    }
}

void AdvancedAnimation::onEnter()
{
    CCSprite::onEnter();
    
    if (_paused)
    {
        pauseSchedulerAndActions();
    }
}

void AdvancedAnimation::onExit()
{
    CCSprite::onExit();
    
    stopAllActions();
}