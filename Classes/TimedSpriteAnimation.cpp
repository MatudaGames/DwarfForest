//
//  TimedSpriteAnimation.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 7/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "TimedSpriteAnimation.h"

#include <algorithm>
#include <cmath>

USING_NS_CC;
using namespace std;

TimedSpriteAnimation* TimedSpriteAnimation::create(const char* plist, float delay, int limit)
{
    TimedSpriteAnimation *pRet = new TimedSpriteAnimation(delay,limit);
    if (pRet && pRet->init(plist))
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

TimedSpriteAnimation::TimedSpriteAnimation(float delay,int limit):
_action(NULL), _delay(delay), _frameLimit(limit)
{
}

TimedSpriteAnimation::~TimedSpriteAnimation()
{
	if (_action) _action->release();
}

static bool compare(const CCObject* a, const CCObject* b)
{
	return (((CCString*)a)->m_sString < ((CCString*)b)->m_sString);
}

bool TimedSpriteAnimation::init(const char* plist)
{
	if (!CCSprite::init())
	{
		return false;
	}
	
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile(plist);
	
	CCDictionary *dict = CCDictionary::createWithContentsOfFile(plist);
	CCDictionary *framesDict = (CCDictionary*)dict->objectForKey("frames");
	
	CCArray* frameNames = framesDict->allKeys();
	std::sort(frameNames->data->arr, frameNames->data->arr + frameNames->data->num, compare);
	
	CCArray* animFrames = CCArray::createWithCapacity(framesDict->count());
	
	CCObject* frameName;
    int aCurrentFrames = 0;
	CCARRAY_FOREACH(frameNames, frameName)
	{
        //Break if needed
        aCurrentFrames++;
        if(_frameLimit>0 && aCurrentFrames>=_frameLimit)
            break;
        
		CCSpriteFrame* frame = cache->spriteFrameByName(((CCString*)frameName)->getCString());
		animFrames->addObject(frame);
	}
	
//	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.05f);
    CCAnimation* animation = NULL;
    if (_delay>0)
        animation = CCAnimation::createWithSpriteFrames(animFrames, 0.05f);
    else
        animation = CCAnimation::createWithSpriteFrames(animFrames, abs(_delay));
	
	_action = CCSequence::create(CCAnimate::create(animation),
                                 CCCallFuncN::create(this, callfuncN_selector(TimedSpriteAnimation::onAnimFinsh)),
                                 NULL);
	_action->retain();
	
	setDisplayFrame((CCSpriteFrame *)animFrames->objectAtIndex(0));
	
	return true;
}

void TimedSpriteAnimation::onEnter()
{
	CCSprite::onEnter();
	
	runAction(_action);
}

void TimedSpriteAnimation::onExit()
{
	CCSprite::onExit();
	
	stopAllActions();
}

void TimedSpriteAnimation::onAnimFinsh(cocos2d::CCObject *selector)
{
    runAction(CCSequence::create(CCDelayTime::create(_delay), _action, NULL));
}
