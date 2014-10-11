#pragma once

class JniCalls
{
private:

public:
	static void openLeaderboards();
	static void submitScore(double points, int level);
    
    static void showLeaderboards();
    static void showAchievements();
    static bool isSignedIn();
    static void signPlayerIn();
    static void signPlayerOut();
    
    static void completedAchievement(const char* theAchievement,int theAmount);
    static void submitScroeToLeaderboard(const char* theLeaderboard,int theAmount);
    
    static void shareScoreToFB(int theAmount);
    static bool isFacebookSigned();
    static void loginFacebook();
    static void logoutFacebook();
    
    static void showTwitterFollow();
    static void showFacebookFollow();
    
    static void openURLJNI(const char* url);
    
};

