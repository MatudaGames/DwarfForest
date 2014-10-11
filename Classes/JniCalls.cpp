#include "JniCalls.h"

#include <cocos2d.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

void JniCalls::openLeaderboards()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // JNI call
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t,
			"lv.gamevision.dwarfforest.DwarfForest",
			"getInstance",
			"()Llv/gamevision/dwarfforest/DwarfForest;"))
	{
		jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

		JniMethodInfo openLeaderBoard;

		if (JniHelper::getMethodInfo(openLeaderBoard,
				"lv.gamevision.dwarfforest.DwarfForest",
				"openLeaderboard",
				"()V"))
		{
			openLeaderBoard.env->CallVoidMethod(obj, openLeaderBoard.methodID);
		}

	}
#endif
}

void JniCalls::submitScore(double points, int level)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // JNI call
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t,
			"lv.gamevision.dwarfforest.DwarfForest",
			"getInstance",
			"()Llv/gamevision/dwarfforest/DwarfForest;"))
	{
		jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

		JniMethodInfo submitScoreMethod;

		if (JniHelper::getMethodInfo(submitScoreMethod,
				"lv.gamevision.dwarfforest.DwarfForest",
				"submitScore",
				"(DI)V"))
		{
			jdouble jpoints = points;
			jint jlvl = level;
			submitScoreMethod.env->CallVoidMethod(obj, submitScoreMethod.methodID, points, jlvl);
		}

	}
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Google play stuff by Kristaps :')
//Check if is signed in on first time game start !!! ask to sign in for cool features

bool JniCalls::isSignedIn()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "gameServiceIsSigned", "()I")) {
        jint version =  t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        if (version>0)
            return true;
        return false;
    }
#endif
    
    return false;
}

void JniCalls::openURLJNI(const char* url)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t
                    , "lv/gamevision/dwarfforest/DwarfForest"
                    , "openURL"
                    , "(Ljava/lang/String;)V"))
    {
        jstring StringArg1 = t.env->NewStringUTF(url);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, StringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void JniCalls::signPlayerIn()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "gameServicesSignIn", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void JniCalls::signPlayerOut()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "gameServicesSignOut", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}



void JniCalls::showLeaderboards()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "showLeaderboards", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void JniCalls::showAchievements()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "showAchievements", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

//Set amount to 0 if this is simple achievement
void JniCalls::completedAchievement(const char* theAchievement,int theAmount)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "completeAchievment", "(Ljava/lang/String;I)V")) {
        jstring name = t.env->NewStringUTF(theAchievement);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID,name,theAmount);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

//If we will have more then one leaderboard, just give the correct leaderboard name
void JniCalls::submitScroeToLeaderboard(const char* theLeaderboard,int theAmount)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "submitScoreToLeaderboard", "(Ljava/lang/String;I)V")) {
        jstring name = t.env->NewStringUTF(theLeaderboard);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID,name,theAmount);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

//Share the score to fb
void JniCalls::shareScoreToFB(int theAmount)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "ShareScoreToFB", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID,theAmount);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

bool JniCalls::isFacebookSigned()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "isFacebookSigned", "()I")) {
        jint version =  t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        if (version>0)
            return true;
        return false;
    }
#endif
    
    return false;
}

void JniCalls::loginFacebook()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "loginFacebook", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void JniCalls::logoutFacebook()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "logoutFacebook", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

//new stuff to follow users
void JniCalls::showTwitterFollow()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "showTwitterFollow", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void JniCalls::showFacebookFollow()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "lv/gamevision/dwarfforest/DwarfForest", "showFacebookFollow", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}





