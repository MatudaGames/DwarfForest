//
//  SpriteAnimation.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 30.03.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "SpriteAnimation.h"
#include <algorithm>

USING_NS_CC;
using namespace std;

SpriteAnimation* SpriteAnimation::create(const char* plist, bool repeat)
{
    SpriteAnimation *pRet = new SpriteAnimation();
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

SpriteAnimation::SpriteAnimation():
	_action(NULL)
{
}

SpriteAnimation::~SpriteAnimation()
{
	if (_action) _action->release();
}

static bool compare(const CCObject* a, const CCObject* b)
{
	return (((CCString*)a)->m_sString < ((CCString*)b)->m_sString);
}

bool SpriteAnimation::init(const char* plist, bool repeat)
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
	
	CCArray* animFrames = CCArray::createWithCapacity(framesDict->count());
	
	CCObject* frameName;
	CCARRAY_FOREACH(frameNames, frameName)
	{
		CCSpriteFrame* frame = cache->spriteFrameByName(((CCString*)frameName)->getCString());
		animFrames->addObject(frame);
	}
	
//	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.05f);
    animation = CCAnimation::createWithSpriteFrames(animFrames, 0.05f);//0.05f
    
    if (repeat)
    {
//        _action = CCRepeatForever::create(CCAnimate::create(animation)->reverse());
        _action = CCRepeatForever::create(CCAnimate::create(animation));
        _action_reverse = CCRepeatForever::create(CCAnimate::create(animation));
    }
    else
    {
//        _action = CCAnimate::create(animation);
        
        _action = CCAnimate::create(animation);
    }
    
    
    
	_action->retain();
	
	setDisplayFrame((CCSpriteFrame *)animFrames->objectAtIndex(0));
	
	return true;
}

void SpriteAnimation::pause()
{
    _paused = true;
    pauseSchedulerAndActions();
}

void SpriteAnimation::resume()
{
    _paused = false;
    resumeSchedulerAndActions();
}

void SpriteAnimation::onEnter()
{
	CCSprite::onEnter();
	
    runAction(_action);
    if (_paused)
    {
        pauseSchedulerAndActions();
    }
}

void SpriteAnimation::onExit()
{
	CCSprite::onExit();
	
	stopAllActions();
}
