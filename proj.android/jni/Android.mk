LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/AnalyticX.cpp \
                   ../../Classes/AnalyticXStringUtilAndroid.cpp \
                   ../../Classes/Crystal.cpp \
                   ../../Classes/CrystalIntro.cpp \
                   ../../Classes/Diamond.cpp \
                   ../../Classes/DiamondIntro.cpp \
                   ../../Classes/Dwarf.cpp \
                   ../../Classes/DwarfIntro.cpp \
                   ../../Classes/Effect.cpp \
                   ../../Classes/EffectIntro.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/IceBarrage.cpp \
                   ../../Classes/IntroAnimation.cpp \
                   ../../Classes/LoseScene.cpp \
                   ../../Classes/MainMenuScene.cpp \
                   ../../Classes/OptionsScene.cpp \
                   ../../Classes/SpriteAnimation.cpp \
                   ../../Classes/Troll.cpp \
                   ../../Classes/TrollIntro.cpp \
                   ../../Classes/Utils.cpp \
                   ../../Classes/Web.cpp \
                   ../../Classes/TutorialScene.cpp \
                   ../../Classes/IntroScene.cpp \
                   ../../Classes/TimedSpriteAnimation.cpp \
                   ../../Classes/FreeDiamondsScene.cpp \
                   ../../Classes/StoreScene.cpp \
                   ../../Classes/PauseScene.cpp \
                   ../../Classes/Levels.cpp \
                   ../../Classes/User.cpp \
                   ../../Classes/JniCalls.cpp \
                   ../../Classes/Mushroom.cpp \
                   ../../Classes/SaveMeScene.cpp \
                   ../../Classes/WeeklyChallengesScene.cpp \
                   ../../Classes/DailyChallengesScene.cpp \
                   ../../Classes/WeeklyChallenges.cpp \
                   ../../Classes/DailyChallenges.cpp \
                   ../../Classes/DailyChallengesPopUp.cpp \
                   ../../Classes/WeeklyCompletePopUp.cpp \
                   ../../Classes/DailyDaysCompletePopup.cpp \
                   ../../Classes/PopUp.cpp \
                   ../../Classes/PlantFuzzBall.cpp \
                   ../../Classes/PlantCrystal.cpp \
                   ../../Classes/PlantIceFlower.cpp \
                   ../../Classes/PlantSunFlower.cpp \
                   ../../Classes/CCShake.cpp \
                   ../../Classes/DebugScene.cpp \
                   ../../Classes/LevelEndScene.cpp \
                   ../../Classes/ExperienceBar.cpp \
                   ../../Classes/HighScore.cpp \
                   ../../Classes/LevelEndMenu.cpp \
                   ../../Classes/LevelUpPopUp.cpp \
                   ../../Classes/StatsScene.cpp \
                   ../../Classes/UserStats.cpp \
                   ../../Classes/GameStats.cpp \
                   ../../Classes/FreeStuffPopup.cpp \
                   ../../Classes/StoreOfferPopup.cpp \
                   ../../Classes/RateGamePopup.cpp \
                   ../../Classes/BeforeQuitScene.cpp \
                   ../../Classes/Machine_Enemies.cpp \
                   ../../Classes/Machine_Ghost.cpp \
                   ../../Classes/Machine_Stamp.cpp \
                   ../../Classes/Plant_Crystal_Weed.cpp \
                   ../../Classes/StaticSceneManager.cpp \
                   ../../Classes/LoadingLayer.cpp \
                   ../../Classes/LoadingScene.cpp \
                   ../../Classes/UpgradeScene.cpp \
                   ../../Classes/MissionManager.cpp \
                   ../../Classes/LevelEndMissions.cpp \
                   ../../Classes/MissionScene.cpp \
                   ../../Classes/Rain.cpp \
                   ../../Classes/Wind.cpp \
                   ../../Classes/StoreBox.cpp \
                   ../../Classes/LoadingScreen.cpp \
                   ../../Classes/ChallengesScene.cpp \
                   ../../Classes/StatsBox.cpp \
                   ../../Classes/StatsScreen.cpp \
                   ../../Classes/TrollBullet.cpp \
                   ../../Classes/SupportPopup.cpp \
                   ../../Classes/Goblin.cpp \
                   ../../Classes/Hidra.cpp \
                   ../../Classes/HidraIntro.cpp \
                   ../../Classes/Spider.cpp \
                   ../../Classes/AdvancedAnimation.cpp \
                   ../../Classes/InGamePowers.cpp \
                   ../../Classes/Enemy_Bee.cpp \
                   ../../Classes/HelloWorldScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
