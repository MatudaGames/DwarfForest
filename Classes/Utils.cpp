//
//  Utils.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 24.02.2013.
//
//

#include "Utils.h"
#include <sys/time.h>

#include <cocos2d.h>
#include "User.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <AudioToolbox/AudioServices.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
cocos2d::JniMethodInfo vibrateMethod;
#endif

void vibrate()
{
    bool vibrationEnabled = cocos2d::CCUserDefault::sharedUserDefault()->getBoolForKey("vibrationEnabled", true);
    
    if (vibrationEnabled)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	
        if (vibrateMethod.methodID == 0)
        {
            if (!cocos2d::JniHelper::getStaticMethodInfo(vibrateMethod,
                                                         "lv/gamevision/dwarfforest/DwarfForest",
                                                         "vibrate",
                                                         "()V"))
            {
                //TODO: throw error
            }
        }
        
        vibrateMethod.env->CallStaticVoidMethod(vibrateMethod.classID, vibrateMethod.methodID);
	
#endif
    }
}

std::string toString(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

std::string toString(unsigned int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

unsigned long Time::getTime()
{
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + time.tv_usec;
	return millisecs;
}

// t2 newer if result positive
int Time::dayDifference(time_t t1, time_t t2)
{
    struct tm laiks = *localtime(&t1);
    laiks.tm_sec = 0;
    laiks.tm_hour = 0;
    laiks.tm_min = 0;
    time_t t1day = mktime(&laiks);
    
    struct tm laiks2 = *localtime(&t2);
    laiks2.tm_sec = 0;
    laiks2.tm_hour = 0;
    laiks2.tm_min = 0;
    time_t t2day = mktime(&laiks2);
    
    time_t diff = abs(t2day - t1day);
    
    struct tm* dienas = localtime(&diff);
    
    // long years not handedled properly becouse of very little necessety for it
    return (dienas->tm_year-70)*365 + dienas->tm_yday;
}

bool Time::isToday(time_t day)
{
    if(User::getInstance()->FirstSessionDone==1)
        return true;
    
    Time::dayDifference(day);
    
    time_t now = time(0);
    struct tm laiks = *localtime(&now);
    struct tm laiks2 = *localtime(&day);

    if (laiks.tm_year == laiks2.tm_year && laiks.tm_yday == laiks2.tm_yday)
    {
        return true;
    }
    
    return false;
}

uint32_t Time::getYDay()
{
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    return t->tm_yday;
}

uint32_t Time::getYWeek()
{
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    return t->tm_yday / 7;
}

//Some sound for some sounds magic !!!
std::string getSoundFx(const char* theName)
{
    //For android use ogg | iphone special ios format
    char aSoundFX[32];
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    sprintf(aSoundFX,"Sound/%s.ogg",theName);
    sprintf(aSoundFX,"Sound/%s.wav",theName);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sprintf(aSoundFX,"Sound/%s.wav",theName);
#endif
    
    return aSoundFX;
}
