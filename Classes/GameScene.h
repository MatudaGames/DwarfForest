//
//  GameScene.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 18.02.2013.
//
//

#pragma once

#include <cocos2d.h>
#include <vector>
#include "Dwarf.h"
#include "Troll.h"
#include "Spider.h"
#include "Goblin.h"
#include "Hidra.h"
#include "Crystal.h"
#include "Diamond.h"
#include "GameStats.h"
#include "DailyChallengesPopUp.h"
#include "AdvancedAnimation.h"

#include "MissionManager.h"

#include "InGamePowers.h"

#include "Enemy_Bee.h"

#include "GameItem_PowerUp.h"

#include "Enemy_Totem.h"

#include "Universal_PowerItem.h"

#include "MasterTroll.h"

#include "MasterDwarf.h"

/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "fmod.hpp"
#include "fmod_errors.h"
#endif
*/

#include "TimedSpriteAnimation.h"

#define DWARF_SPELL_FREEZER 1
#define DWARF_SPELL_ELECTRIFY 0

#define DWARF_SPELL_SHOP_1 101
#define DWARF_SPELL_SHOP_2 102
#define DWARF_SPELL_SHOP_3 103
#define DWARF_SPELL_SHOP_4 104

#define MASTER_ACTION_CONFUSE 2
#define MASTER_ACTION_SPAWN_TRAP 0
#define MASTER_ACTION_TROLL 3
#define MASTER_ACTION_BULLET 11
#define MASTER_ACTION_BULLET_ICE 12
#define MASTER_ACTION_BULLET_POISON 13
#define MASTER_ACTION_BULLET_STRAIGHT 14
#define MASTER_ACTION_BULLET_SPLIT_MIDDLE 15
#define MASTER_ACTION_BULLET_ZIGZAG 16
#define MASTER_ACTION_BULLET_DECOMPOSE_BEGINNING 17
#define MASTER_ACTION_BULLET_ONE_LINE 18
#define MASTER_ACTION_BULLET_SPLIT_UP 19
#define MASTER_ACTION_BULLET_SPELL 20

// Mission types
#define MissionType_PointCount 0
#define MissionType_DwarfCount 1
#define MissionType_Combo 2

#define MissionType_Time 11
#define MissionType_Mushroom 4
#define MissionType_TrollEscape 5

#define MissionType_DwarfSave 6

#define MissionType_KillEnemy 7

#define MissionType_DestroyTotem 8

#define MissionType_CrystalCollect 10

#define MissionType_TEST 11

// Activate power up
#define MissionType_AP_Freezer 200
#define MissionType_AP_GhostDwarfs 201

//The special stuff collect
#define MissionType_Crystal_Red 100
#define MissionType_Crystal_Blue 101
#define MissionType_Crystal_Yellow 102
#define MissionType_Crystal_Green 103


// The MT event ids
#define MT_EVENT_DWARF 0
#define MT_EVENT_BULLET 1
#define MT_EVENT_TRAP 2
#define MT_EVENT_TROLL 3
#define MT_EVENT_MASS 4
#define MT_EVENT_ICEBLITZ 5
#define MT_EVENT_CAVEBLOCK 6

// Here are the points that are given by each action
//#define ATTACK_BAR_CRYSTAL_BLUE 20
//#define ATTACK_BAR_CRYSTAL_GREEN 10
//#define ATTACK_BAR_CRYSTAL_RED 30
//#define ATTACK_BAR_CRYSTAL_YELLOW 50

//#define ATTACK_BAR_DWARF_ENTER_CAVE 30


class IntroAnimation;
class Troll;
class Crystal;
class Effect;

class InGamePowers;

struct GeneratePoint
{
	int32_t x;
	int32_t y;
	float angle;
};

struct WindObject
{
    Dwarf *theDwarf;
    Troll *theTroll;
    int32_t x;
    int32_t y;
    
    int32_t haveDwarf;
    
    int32_t ID;
};

struct SpawnPoint
{
	int32_t x;
	int32_t y;
    float angle;
};

struct PopupShowInfo
{
    int32_t reward_type;
    int32_t reward_amount;
    const char* reward_text;
};

//For test of smaller objects
const float GLOBAL_SCALE = 0.75f;

class GameScene : public cocos2d::CCLayer
{
public:
	// implement the "static node()" method manually
    CREATE_FUNC(GameScene);
	
	GameScene();
	virtual ~GameScene();
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	
	virtual void update(float delta);
    
//    virtual void onEnter();
//    virtual void onExit();
    
//    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
//    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
//    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    cocos2d::CCLabelTTF* mDwarfSaveCounter;
    cocos2d::CCLabelTTF* mExtraCrystalCounter;
    
    bool mWaitForSaveMe;
    void GeneratePowerUp(int theType,int theTime);
    
    bool mIgnoreDwarfSave;
    
    void OnTryToShoot();
    
    //----------------------------------------
    // The new special power up spawn stuff
    void updatePowerUpSpawn(float delta);
    
    cocos2d::CCArray* _powersOnMap;
    
    cocos2d::CCArray* _spellBulletsOnMap;
    
    float mPowerUpSpawnTime;
    
    // The Master Troll Attack stuff
    bool mAttackFunctionalActive;
    
    int mMasterTroll_HP;        // What is Master Troll HP?
    float mMasterTroll_CurrentHP; // For smooth progress bar fill
    
    int mMasterTroll_Damege;    // What is the attack damage
    
    int mMasterTroll_Attack;    // Whats the current value for attack
    float mMasterTroll_CurrentAttack;
    
    int mDwarfMachineHP;
    bool mDwarfMachineProtect; // Troll attacks machine
    
    // Even more new stuff
    bool mDwarfCollectMachine;
    
    int EnemyLineCount;
    
    //Mission_KillEnemys
    int mKillTrollsAmountLeft;
    
    int mDwarfKingItemSpawnID; // What will dwarf king spawn when bullet hits ground
    cocos2d::CCPoint mDwarfKingItemSpawnPos;
    void StartDwarKingItemSpawn();
    void OnDwarfKingSpawnHitGround(CCNode* sender);
    bool mDwarfKingSpawn_Active;
    bool mDwarfKingSpawnItems; // The new stuff when dwarf king spawns stuff after amount
    
    bool mWinGameOnMasterTrollKill; // Will we win game if master troll will fall?
    
    void CreateBattleArena();
    void UpdateBattleLabel();
    void UpdateSmoothBattleBars(float delta);
    void OnAttackHitTroll(CCNode* sender);
    
    // The dwarf machine attack
    float mAttackDwarfMachineTimer;
    void OnTryToShoot_ToDwarf();
    void OnAttackHitMachine(CCNode* sender);
    
    // The progress bars !!!
    cocos2d::CCSprite* mBattleBar_TrollBase;
    cocos2d::CCSprite* mBattleBar_MachineBase;
    
    // The actual progress bars
    cocos2d::CCSprite* mBattleBar_TrollHP;
    cocos2d::CCSprite* mBattleBar_MachinePower;
    
    //----------------------------------------
    // Totem stuff
    
    Enemy_Totem* mTotem;
    void OnAttackHitTotem(cocos2d::CCPoint position,SpellInfo theSpell);
    
    //------------
    
    void FreezeDwarfTotal(cocos2d::CCObject *sender);
    void StartDwarfFreeze();
    void StartTrollFreeze();
    void StartDwarfFreeze_All();
    void IceBlitz();
    void StartIceBlitz();
    void BulletDwarf();
    int mMasterTroll_Bullets;
    
    
    int _mission_escaped_trolls;
    int _mission_AP_Freezer;
    int _mission_AP_GhostDwarfs;
    
    int _mission_SaveDwarfs_Left;
    int _SaveDwarfsCounter;
    
    int _crystal_Point_counter;
    
    bool _mInstantBulletAction; // Will this be a instant bullet
    int mCurrentBulletType;// What type of bullet will fly now
    
    float mNoDwarfEneterCave;
    
    void CheckMissionByValue(int theType,float theValue);
    int _missionCurrentValue;
    
    int mTotalPointsNeeded;
    
    int mCombo_DwarfEnter;
    float mComboTimer_DwarfEnter;
    
    //Snipe stuff
    
    int mtSnipe;
    int mtSnipeDelay;
    
//    InGamePowers* mPowerMenu;
    InGamePowers* mPowerMenu;
    int mCurrentSpellCharge;
    int mCurrentActiveSpell;
    
    cocos2d::CCPointArray* _movePointsWind;
    
    cocos2d::CCPointArray* _movePointsSpiderCave_1;
    cocos2d::CCPointArray* _movePointsSpiderCave_2;
    cocos2d::CCPointArray* _movePointsSpiderCave_3;
    
    cocos2d::CCSprite* _SpiderCave_1;
    cocos2d::CCSprite* _SpiderCave_2;
    cocos2d::CCSprite* _SpiderCave_3;
    
    bool _SpawnBlueDwarf;
    bool _SpawnOrangeDwarf;
    
    //------------------------------------------
    //The new stuff
    
    void generatePowerItem(int theID);
    
    MissionSet mCurrentMission;
    
    cocos2d::CCSize visibleSize;
    cocos2d::CCPoint origin;
    
    void CreateGameByMission();
    void CreateMap();
    void CreateGameStartHUD();
    void CreateMissionStart();
    void ShowTheHUD();
    
    void CaveBlock(int theType);
    void CaveBlockRemover();
    
    void generateEnemyForMission(MissionTroll theEnemy);
    void generateTrollForMission(MissionTroll theTrollInfo);
    void generateDwarfMission(bool theInstant);
    
    void UpdateDwarfSpawn(float delta);
//    void UpdateTrapsSpawn(float delta);
    
    float _CrystalSpawnRecheckTimer;
    void UpdateCrystalSpawn(float delta);
    
    std::vector<float> _dwarfSpawnArrTimers;
    float _SubDwarfSpawn;
    int _LeftNotSpawnDwatfs;
    
//    float _MasterTroll_TimeToAct;
    float mMasterTrollLastActionID;
    
    void CreateBulletByType(int theType,int theStartX,int theStartY);
    
    //------------------------------------------
    
    int mThrowObjecIndex;
    
    bool mIntroPlaying;
    
    bool _DailySpinVisible;
    
    int mLastShootDir;
    
    int mCombo_CollectCrystals;
    float mComboTimer_CollectCrystal;
    
    cocos2d::CCSprite* _debugSprite1;
    cocos2d::CCSprite* _debugSprite2;
    
    int mMaxBarPoints;
    int mTotalPointsInGame;
    double mTrollTimer;
    double mTrollFreeze;
    double mTrollFreezeDwarfTime;
    int mTrollDwarfCounter;
    
    double mTrollBulletTimer;
    
    void OnCompletedMission16();
    void SetMasterTrollAction(int theType);
	
    void createPoints(int amount,int theBonus,cocos2d::CCPoint thePos,cocos2d::ccColor3B theColor,bool crystalPoint=false);
	void addScore(int score);
    void addPickedUpCrystall(CrystalColor col);
	void lose(bool ignoreMissionSave = false);
    
    void pauseGame();
    void showWinScreen();
    
    double _gameTime;
    
    void CreateDwarfDebug();
    void CreateLevelEnd(CCNode* theSender);
    
    void playInGameSound(const char* theName,bool theForce=false);
    void stopInGameSound(const char* theName,bool theForce=false);
    
    void createTextFly(const char* txt,cocos2d::CCPoint thePos,int theFlyPos,int theComboValue = 0);
	
    Goblin* generateGoblin(int theX,int theY, float theRadius);
    Hidra* generateHidra(int theX,int theY,float theRadius);
    Spider* generateSpider(int theX,int theY,float theRadius);
    
	Dwarf* generateDwarf(int theType = -1,int theSpawnPoint = -1);
	cocos2d::CCArray* getDwarves() const { return _dwarves; }
    
    cocos2d::CCArray* getActiveTornado() const { return _tornadoActive; }
    
	
	Troll* generateTroll(bool theSkip=false);
	
//	Crystal* generateCrystal(bool theNearDwarf);
    Crystal* generateCrystal(bool theNearDwarf,int theCrystalID,int theTimeOnMap);
    
    void generateCrystalSpecial(int theX,int theY);
    void generateDiamondSpecial(int theX,int theY);
	void removeCrystal(Crystal* crystal);
    
    void removeDiamond(Diamond* diamond);
    
    void generateMushroom(float theTime);
    
    void CreateSpecialSpot();
	
	void generateEffect();
	void removeEffect(Effect* effect);
    void removeAllEffects(bool theMachine=false);
    void showRemovableEffects();
    void removeRemovableEffects();
    
    void OnEffectDelayFinish(CCNode* sender);
    
    void CreateBlitz(int theX,int theY,Troll* theEnemy=NULL,Effect* theEffect=NULL);
    
    void CreateBlitz_Goblin(int theX,int theY,Goblin* theEnemy=NULL);
    void CreateBlitz_Hidra(int theX,int theY,Hidra* theEnemy=NULL);
    void CreateBlitz_Spider(int theX,int theY,Spider* theEnemy=NULL);
	
	void generateDiamond(float theTimeOnMap);
	
	bool getMask(int32_t posX, int32_t posY);
    
    bool isInTrollSensor(Dwarf* theDwarf,Troll* theTroll);
    
    bool getTutorialMask(int32_t posX, int32_t posY);
    
    bool getSpawnObjectMask(int32_t posX, int32_t posY);
	
	void introFinished(IntroAnimation* introAnimation);
	
	const cocos2d::CCPoint& getCavePoint(DwarfType dwarfType);
    
    void removeNode(CCNode* sender);
    void removeBatchNode(CCNode* sender);
    
    void OnTrollDelayFinish(CCNode* sender);
    
    void menuSaveMeCallBack(Dwarf* dwarf_1,Dwarf* dwarf_2,Troll* troll);
    void nowCreateSaveMe();
    
    void pauseSchedulerAndActionsRecursive(cocos2d::CCNode* node,bool skipSaveMe);
    void resumeSchedulerAndActionsRecursive(cocos2d::CCNode* node);
    void onResumeFromPause();
    void onSaveFromPause();
    void onPauseFinished();
    void onPuseTimeCount();
    
    void highlightBigDoor(bool theOff);
    void highlightSmallDoor(bool theOff);
//    void dwarfEnterDoor(bool theFat);
    void dwarfEnterDoor(bool theFat, Dwarf* theDwarf);
    
    void CheckForDwarfCancel(Dwarf* theDwarf);
    
    void disableBooster(int theType);
    void createRandomBoos(cocos2d::CCPoint thePos);
    
    void UpdateDwarfSaveLabel();
    
    void checkBoosterAvailability();
    
    void generatePlantCrystal();
    
    void generatePlantFuzz();
    
    void generatePlantSunFlower();
    void generatePlantIceFlower();
    
    void menuPowerupNoEnemiesCallback(CCObject* sender);
    void menuPowerupEyeCallback(CCObject* sender);
    void menuPowerupSlowPlayCallback(CCObject* sender);
    
    void CreateTornadoThrow(Dwarf* theDwarf,Troll* theTroll,int theStartX,int theStartY);
    void RemoveActiveFlyObject(CCNode* sender);
    
    void CreateMissionStuff();
    void CreateDebugPanel();
    void DebugCreateCrystal();
    
    void OnMachineStamp(cocos2d::CCObject *sender);
    void OnMachineGhost(cocos2d::CCObject *sender);
    void OnMachineEnemies(cocos2d::CCObject *sender);
    
    cocos2d::CCSprite* maskedSpriteWithSprite(cocos2d::CCSprite* textureSprite,cocos2d::CCSprite* maskSprite,int theX,int theY);
    
    void clearAllMap();
    
    int mChargesForBlitz;
    cocos2d::CCLabelTTF* mCharges_Electro;
    
    bool mPlant_FuzzFlower_First;
    bool mPlant_SunFlower_First;
    bool mPlant_IceFlower_First;
    
    cocos2d::CCLabelTTF* _pointsLabel;
    
    bool spellMoveTo;
    
    void onEnterTransitionDidFinish();
    void CreateMachines();
    float _SpawnSpecialTrolls;
    void OnCompleteMission();
    void SetMasterTrollIdleAfterTime(float theTime);
    void SetMasterTrollAnimIdle();
    void CreateTrollIndicator(int theType);
    
    //----------------------------------------------
    // The new mission sub stuff
    
    cocos2d::CCLabelTTF* _dwarfOnMap;
    cocos2d::CCSprite* _mission_star_1;
    cocos2d::CCSprite* _mission_star_2;
    cocos2d::CCSprite* _mission_star_3;
    
    void UpdateMissionStars();
    
    int _mission_star_points_1;
    int _mission_star_points_2;
    int _mission_star_points_3;
    
    bool _mission_count_dwarfs;
    int _mission_dwarfs_saved_counter;
    
    int _mission_allowed_effect;
    
    int _mission_dwarfs_spawned;
    int _mission_dwarfs_removed;
    int _mission_dwarfs_max;
    
    cocos2d::CCSprite* _mission_progress_bar_1;
    cocos2d::CCSprite* _mission_progress_bar_2;
    cocos2d::CCSprite* _mission_progress_bar_3;
    cocos2d::CCSprite* _mission_progress_bar_4;
    
    void NewMissionCompleted();
    void OnExitWithNewMission();
    
    //The new fellas
//    cocos2d::CCSprite* _MasterTrollBase;
//    cocos2d::CCSprite* _MasterDwarfBase;
    
    MasterTroll* _MasterTrollBase;
    MasterDwarf* _MasterDwarfBase;
    
    void UpdateTestStuff(float delta);
    
    cocos2d::CCSprite* mMasterTroll;
    SpriteAnimation* _MasterTroll_IdleAnimation;
	SpriteAnimation* _MasterTroll_WinAnimation;
    SpriteAnimation* _MasterTroll_animation;
    
//    void SetMasterTrollAnimation(SpriteAnimation* animation);
    void SetMasterTrollAnimation(const char* theAnimation);
    void UpdateMasterTroll(float delta);
    
    //Some params for master troll
    void SetDwarfKingAnimation(const char* theAnimation);
    float mMasterTrollActionTimer; // When should do some damage
    
//    void OnMasterTrollFinishEnter();
//    void CreateMasterTrollEnter();
    
    bool CanSpawnMasteTrollExtraEnemy();
    
    void MasterAction_Enemy(cocos2d::CCObject *sender);
    
    void MasterAction_Bullet(cocos2d::CCObject *sender);
    
    float mCallBackTime_BulletFire;
    int mCurrentBUlletFiresLeft; // How many bullets left for fire off
    void DoMultiBulletFire();
    
    float mCallBackTime_TrapSpawn;
    int mCurrentTrapSpawnLeft; // How many traps left for fire off
    void DoMultiTrapSpawn();
    
    bool IsObjectInMap(int theX,int theY);
    
    void MasterAction_Confusion(cocos2d::CCObject *sender);
    
    void CreateMasters();
    void OnMasterHitGround(CCNode* sender);
    void OnMasterHitGroundSFX(CCNode* sender);
    
    float _mission_dwarf_spawn_timer;
    float _mission_crystal_spawn_timer;
    float _mission_effect_spawn_timer;
    
    float MasterTroll_CheckForEnemy_Timer;
    bool MasterTroll_ForceEnemySpawn;
    
    //----------------------------------------------
    //The new map changer
    
    cocos2d::CCSpriteBatchNode * _batchNode;
    
    int mCurrentMapID;
    
    void CreateStartMap(int theTypeID);
    void ChangeMap(int theTypeID);
    
    void OnChangeObject(CCNode* sender);
    void AddLoopAnimation(CCNode* sender);
    
    cocos2d::CCSprite* mMapBase;
    
    //----------------------------------------------
    
    bool mCheckSpecialSpotTouch;
    cocos2d::CCPoint mSpecialTouchSpot;
    
    //DEBUG
    int aStartSpeedEnabled;
    int aStartMin_1;
    int aStartMin_1_extra;
    int aStartMin_2;
    int aStartMin_2_extra;
    int aStartMin_3;
    int aStartMin_3_extra;
    bool mDebugInfoVisible;
    
    bool mDebugFormulas;
    
    bool mPaternFormulas;
    
    int mCrystalPoints;
    
    void activateNoEnemies(int theX,int theY);
    void activateGhoustDwarfs(int theX,int theY);
    void activateClearMap(int theX,int theY);
    void crossFadeBackground(bool theGodMode);
	
    void addDebugCrystalPoints(int amount);
    
    void CreateComboSound(int theType,int theLevel);
    
    float mTotalComboTimer;
    
    void ResetValues();
    
    //The new global counters
    int mTotalCombo;
    int mTotalPoints;
    int mTotalBlueDwarfs;
    int mTotalOrangeDwarfs;
    
    int mTotalGreen_Crystals;
    int mTotalBlue_Crystals;
    int mTotalRed_Crystals;
    int mTotalYellow_Crystals;
    
    int mTotalMushroom;
    int mTotalTimeInGame;
    int mTotalReverseTime;
    int mTotalMinutes;
    int mTotalSeconds;
    int _gameTimeReverse;
    int mTotalEscapesFromTroll;
    
    int mLastSpawnID;
    
    //The Dwarf Spawn stuff
    float _DSpawnCurrentTime;
    float _DSpawnTimer;
    int _DSpawnGameMinCurrent;
    
    std::vector<int> _dwarfSpawnArr;
    
    //The real values after changes
    int _DSpawn_Real_Zone;
    int _DSpawn_Real_Jump;
    int _DSpawn_Real_Min;
    int _DSpawn_Real_Max;
    
    void CreateSpawnLine();
    
    //The parametr time checkers
    float _DSpawn_change_zone;
    float _DSpawn_change_min;
    float _DSpawn_change_max;
    float _DSpawn_change_jump;
    
    int _CurrentSpawnCount;

	float getGameSpeed() const { return _gameSpeed; }

    // Old stuff - deprecated - all moving to item power data xml reading powa :D
//	float getGhostTimer() const {return _boostGhostTimer; }
    float getShieldTimer() const { return _boostShieldTimer; }
	
	int getBoostExtraPoints() const { return _boostExtraPoints; }
	float getBoostFutureSee() const { return _boostFutureSee; }
	
	cocos2d::CCLabelTTF* getDiamondsLabel() const { return _diamondsLabel; }
	
	void setGamePause(bool value) { _gamePause = value; }
	
	int getLastSavePrice() const { return _lastSavePrice; }
	void setLastSavePrice(int value) { _lastSavePrice = value; }
    
    float _boostNoEnemyTimer;
    
    //Tutorial stuff
    float mShowTutorialCompleted;
    
    bool mSplitCavesMode;
    
    bool mBlockFatCave;
    bool mBlockTallCave;
    bool mTutorialEnabled;
    int mTutorialStep;
    float mTutorialTimer;
    bool mTutorialTimerSet;
    
    bool mTutorialFlag_1;
    bool mTutorialFlag_2;
    
    void ShowTutorialButton();
    void OnTutorialStepCompleted(int theStep);
    void CreateDrawHand(int theStep);
    void RemoveDrawHand();
    
    cocos2d::CCPointArray* _movePoints;
	cocos2d::CCSpriteBatchNode* _line;
    
    bool addTutorialMovePoint(const cocos2d::CCPoint& point, const cocos2d::CCPoint& previousPoint);
    void UpdateTutorialStuff();
    void removeTutorialMovePoints();
    
    void CreateSmallTutorial(int theType,int theX,int theY);
    void RemoveSmallTutorial();
    
    void HideMissionText();
    void CreateSubMissionCompletedShow(int theMissionID,int theSubID);
    void CreateMissionCompletedShow(int theMissionID);
    
    void onSaveFromSmallTutorial();
    
    int mCurrentSmallTutorial;
    
    void OnTutorialHandFinish();
    void OnTutorialHandStart();
    void OnTutorialHandClick(CCNode* sender);
    
    void CreateCompleted();
    void RemoveCompletedTutorial();
    
    void FromOptionsEnableMusic();
    
    bool mTutorialDrawHand;
    bool mTutorialPointsAdded;
    cocos2d::CCPoint mTutorialHandStartCords;
    
    bool mCanSpawnExtraCrystal;
    float mSpecialCrystalSpawnTimer;
    int mSpeciaCrystallDwarfID;
    
    DailyChallengesInGameInfo* dailyChallengeInfo;
    
    bool CanZipTroll();

    void generateTrollMission(int theX,int theY,float theRadius,bool theCircle,
                              bool theControlPoint,cocos2d::CCPoint theControlPoint_1,cocos2d::CCPoint theControlPoint_2,int theDrawLine);
    
    void generateTrollSpecial(int theX,int theY, float theRadius=-1);
    void generateEffectSpecial(int theX,int theY, int theType);
    
    int getSpriteOrderZ(int posY);
    void PlaceInDifferentSpotWind(CCNode* sender);
    
    float mTimeNotCollectedCrystal;
    float mTimeToCheckAgainNoCrystal;
    
    float mCurretnEffectDistance;
    cocos2d::CCPoint mRealEffectDistance;
    cocos2d::CCPoint mRealDwarfDistance;
    
    cocos2d::CCSprite* mPopUpPanel;
    cocos2d::CCLabelTTF* mPopUpText;
    void CreateInGamePopup();
    void OnGotItButton();
    bool mPopUpShowActive;
    
    void OnResumeFromDaily();
    
    void AddPopUpToShow(int theRewardType,int theRewardAmount,const char* theRewardText);
    void PopUpFinishedShow(CCNode* sender);
    void PopUpShowNext();
    void AddCheckDoneOnPopUp(CCNode* sender);
    std::vector<PopupShowInfo> popups;
    
    cocos2d::CCMenu* _tutorialButtons;
    cocos2d::CCMenuItemImage* tutorial_button_gotit;
    
    void CraeteHUD();
    
    TimedSpriteAnimation* _blueArrowAnim;
    TimedSpriteAnimation* _orangeArrowAnim;
    
    //Fmod stuff
    bool mFmodEnabled;
    
    /*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FMOD::System     *system;
    FMOD::Sound      *sound1, *sound2, *sound3, *sound4;
    FMOD::Channel    *channel = 0;
    FMOD::Channel    *channel1 = 0;
    FMOD::Channel    *channel2 = 0;
    FMOD::Channel    *channel3 = 0;
    FMOD_RESULT       result;
    
    void updateSound(float delta);
    
    
#endif
    */
    
    
    
    // The new MT spawn stuff
    
    
    // Debug stuff
    bool debugLabelsCreated;
    cocos2d::CCLabelTTF* _MT_Label_Timer;
    cocos2d::CCLabelTTF* _MT_Label_Procent;
    cocos2d::CCLabelTTF* _MT_Label_Value;
    cocos2d::CCLabelTTF* _MT_Label_CurrentValue;
    cocos2d::CCLabelTTF* _MT_Label_ValueTimer;
    cocos2d::CCLabelTTF* _MT_Label_Action;
    
    
    int GetPointsForEvent(int theEvent);
    int GetCurrentMT_Value();
    bool DoWeHaveEnemyOnMap();
    
    float mMT_LastBulletTimer;
    int mMT_LastBulletCount;
    
    float mMT_SpawnTimer;
    float mMT_ValueTimer; // When the value number will increase
    int mMT_ValueAdd;
    int mMT_SpawnValue; // Whats the min stuff
    int mMT_ValueLimit; // Whats the sky limit
    
    bool mMusic1_On;
    bool mMusic2_On;
    bool mMusic3_On;
    
    bool mMusic3_Done;
    bool mMusic2_Done;
    bool mMusic1_Done;
    
    void updateDwarfMusic();
    
    cocos2d::CCArray* _dwarves;
    cocos2d::CCArray* _goblins;
    cocos2d::CCArray* _hidras;
    cocos2d::CCArray* _spiders;
    
    cocos2d::CCArray* _otherEnemy;
    
    void addSpellBullet();
    void finishSpellBullet();
    
    Dwarf * addSpellDwarf;
    
    bool _gamePause;
    
    //----------
    
    //Special 10 mission stuff
    
    void StartSpecialMission();
    void OnExitWithSpecialMission();
    void SpecialMissionCompleted();
    cocos2d::CCLabelTTF* _DwarfCounter;
    int _DwarfsEnteredCave;
    int _DwarfsCollectedObject;
    
    std::vector<int> _blockedGeneratePoints;
    int _DwarfsSpawned;
    int mLostDwarfCount;
    
    // New parametrs for game [used by missions]
    int mTask_SurviveLives;     // The new parametr for missions with lives !!!
    float mTask_SurviveTime;    // Task_SurviveTime
    
    float _gameSpeed;//For slow down and speed up
    
    void OnIncreaseDwarfSpeed();
    void OnDecreaseDwarfSpeed();
    void OnIncreaseTrollSpeed();
    void OnDecreaseTrollSpeed();
    cocos2d::CCLabelTTF* _DwarfSpeed;
    cocos2d::CCLabelTTF* _TrollSpeed;
    
    void GiveGoblinSnatch(Crystal* theCrystal);
    cocos2d::CCSize _screenSize;
    
    bool mZipZapActive;
    
    std::vector<GeneratePoint> _genearetPoints;
    
    std::vector<int> _lastSpawnPoints;
    
    cocos2d::CCArray* _bullets;
    cocos2d::CCArray* _trolls;
    cocos2d::CCArray* _crystals;
    cocos2d::CCArray* _effects;
    cocos2d::CCArray* _diamonds;
//    cocos2d::CCArray* _mushrooms;
    
    void OnCaveBlueOpen();
    void OnCaveBlueClouse();
    
    void OnCaveOrangeOpen();
    void OnCaveOrangeClouse();
    
    AdvancedAnimation* _caveFat;
    AdvancedAnimation* _caveTall;
    
    void CreateTheRealTrollAction(cocos2d::CCObject *sender);
    
//    float _boostGhostTimer;
    
    //................................................
    // The new stuff for Items With Powers !!!
    
    cocos2d::CCArray* mUniversalItems;
    
    float mPowerItem_GhostsActive;
    float mPowerItem_CrystalDoublerActive;
    float mPowerItem_CrystalDoublerValue; // If not active = 1, when active 2
    
    float mPowerItem_CrystalRefiner; // How much gives each crystal more in procent
    
    void updateActiveInGamePowers(float delta);
    
    //................................................
    
private:
    
    //===========  Mission stuff  ===============
    //
    int mDwarfsInCaveOneGame;
    int mFatDwarfsInRow;
    
    //-------------------------------------------
    
    void CreatePossibleObjectSpawns();
    
	cocos2d::CCPoint getRandomPointFromBlock(int theID);
    cocos2d::CCPoint getBlockCenterPos(int theID);
	void menuPowerupPointsCallback(CCObject* sender);
	void menuPauseCallback(CCObject* sender);
    
    void StartTornado(bool theDisable);
    void StartRain(bool theDisable);
    bool mRainActive;
    bool mTornadoActive;
    
    //int aPowerUpNum;
    
    bool mSnowActive;
    
    float mTimeToChangeSeason;
    
    void StartRainDebug();
    void StartWindDebug();
    
    void menuFastModeCallback(CCObject* sender);
    void menuNormalModeCallback(CCObject* sender);
	
	void updateDwarfs(float delta);
	void updateTrolls(float delta);
	void updateCrystals(float delta);
	void updateIntroAnimations(float delta);
	
	void updatePlants(float delta);
	
    int _currentMinForSpawn;
    float _currentMinSpwanedTime;
    int _effectActiveSpawnTime;
    
    //Trolls
    std::vector<int> _trollSpawnTimes;
    int _trollCurrentSpawnMinute;
    int _trollCurrentActiveTime;
    float _trollCurrentTime;
    
    //Crystals
    std::vector<int> _crystalSpawnTimes;
    int _crystalCurrentSpawnMinute;
    int _crystalCurrentActiveTime;
    float _crystalCurrentTime;
    
    //Dwarf
    std::vector<int> _dwarfSpawnTimes;
    
    int _dwarfCurrentSpawnMinute;
    int _dwarfCurrentActiveTime;
    float _dwarfCurrentTime;
    
    int _effectLastSpawnBlockID;
    int _windLastSpawnBlockID;
    
    int _lastSpawnPoint;
    
    bool mNewSplitCaves;
    
    
    cocos2d::CCArray* _flyObjects;
//    std::vector<WindObject> _flyObjects;
    
    int mCurrentMusicID;
    void PlaySpecialMusic(int theDwarfAmount);
    void OnFinishedMusicFadeOut();
    bool mFadeOutMusic;
    bool mFadeInMusic;
    float mBackMusicVolume;
    
    //The updated combo system
    float mComboPlayDelayTimer;
    int mComboLastSoundID;
    std::stringstream mComboSound_1;
    std::stringstream mComboSound_2;
    
    std::stringstream mComboVisual;
    std::stringstream mFileFormat;
    
    int mDebugComboValue;
    
    
    int mTutorialSubStep;
    
    void UpdateBullets(float delta);
    
    cocos2d::CCArray* _tornadoActive;
	
	cocos2d::CCArray* _introAnimations;
	
	std::map<DwarfType, cocos2d::CCPoint> _cavePoints;
	cocos2d::CCSprite* _cave;
	cocos2d::CCImage* _caveMask;
//    cocos2d::CCSprite* _caveMask;
    
//    cocos2d::CCSprite* _caveFat;
	cocos2d::CCImage* _caveMaskFat;
//    cocos2d::CCSprite* _caveTall;
	cocos2d::CCImage* _caveMaskTall;
    
	cocos2d::CCImage* _mask;
    
    cocos2d::CCImage* _trollSensor;
    
    cocos2d::CCImage* _objectMask;
    
    cocos2d::CCImage* _tutMask_1;
    cocos2d::CCImage* _tutMask_2;
    cocos2d::CCImage* _tutMask_3;
    
    
    cocos2d::CCSprite* _caveDoorBig;
    cocos2d::CCSprite* _caveDoorSmall;
    
    cocos2d::CCLayerColor* mGeneralScreen;
    cocos2d::CCSize mScreenSize;
    
    std::vector<int> _possibleGeneratePoints;
    
    
    
    std::vector<int> _possiblePowerMushroom;
    
    std::vector<int> _effectSpawnTimes;
    
    std::vector<int> _windSpawnPositions;
    std::vector<int> _effectSpawnPositions;
    std::vector<int> _mapSpawnTypes;
    
//    int _mushroomLastSpawnBlockID;
//    std::vector<int> _mushroomSpawnPositions;
    
    int _crystalLastSpawnBlockID;
    std::vector<int> _crystalSpawnPositions;
    
    int _diamondLastSpawnBlockID;
    std::vector<int> _diamondSpawnPositions;
    
    //---------------------------------------
    // Map generate stuff !!!
    
    bool mNewMapRandomEnabled;
    SpawnPoint mCaveFinalCords;
    
    std::vector<SpawnPoint> _CaveSpawnPoints;
    std::vector<SpawnPoint> _DwarfSpawnPoints;
    
    void CreatePossibleRandomDwarfSpawns(int theMapID);
    void CreateRandomMap();
    
    
    //---------------------------------------
    
    //Funny stuff !!!
    std::vector<SpawnPoint> _BlockPositions_1;
    std::vector<SpawnPoint> _BlockPositions_2;
    std::vector<SpawnPoint> _BlockPositions_3;
    std::vector<SpawnPoint> _BlockPositions_4;
    std::vector<SpawnPoint> _BlockPositions_5;
    std::vector<SpawnPoint> _BlockPositions_6;
    std::vector<SpawnPoint> _BlockPositions_7;
    std::vector<SpawnPoint> _BlockPositions_8;
    
    //    std::vector<std::string> _soundPlaying;
    
    int _dwarfWalkID;
    int _monsterWalkID;
    int _saveMeID;
    int _webID;
    
	unsigned int _score;
    
    void CreateParticleOnShow(CCNode* sender);
    
	cocos2d::CCLabelTTF* _scoreLabel;
	cocos2d::CCLabelTTF* _dwarfsLeftLabel;
    cocos2d::CCLabelTTF* _dwarfsLostLabel;
    cocos2d::CCLabelTTF* _dwarfsSavedLabel;
    cocos2d::CCLabelTTF* _crystalsGathered;
    cocos2d::CCLabelTTF* _dwarfsCrystalsLabel;
    cocos2d::CCLabelTTF* _timeLabel;
    cocos2d::CCLabelTTF* _lifesLabel;
    cocos2d::CCLabelTTF* _pointLabel;
    
    cocos2d::CCSprite* mMissionStatus;
    
    cocos2d::CCLabelTTF* _tutorialLabel;
    cocos2d::CCLabelTTF* _tutorialLabel2;
    
    // game stats
    DF::Stats::GameStats _stats;
    
    int mScoreMulti_Global;
    
    //Some flurry data
    int mFlurry_MachinesActivated;
    int mFlurry_CrystalPoints;
    
    int mFlurry_StampActivated;
    int mFlurry_GhostActivated;
    int mFlurry_BlitzActivated;
    
    void RemoveEffectByChild(Crystal* theCrystal);
    
    
	
    
    double _rainTime;
    double _whenToRain;
    bool rainTimeSet;
    
    double _windTime;
    double _whenToWind;
    
    int mMapDangerType;
    int mLastMapDangerType;
    double mMapDangerTimer;
    double mMapDangerTimerReq;
    bool mFirstTimeMap;
    
    double mMapDangerCooldown;
    
    bool _generatedExtraStartDwarf;
    
    bool _foundWarning;
    
    double _dwarfTimer;
	
	bool _dwarfWalkOn;
    bool _monsterWalkOn;
	
	
    cocos2d::CCPoint _origin;
    int _totalStepsZ;
    
    int _totalActiveBoosters;
    bool _boosterMushroom;
    
    int _lastSavePrice;
    
    bool _willUseMushroom;
	
	
    
    Dwarf* _saveDwarf1;
    Dwarf* _saveDwarf2;
    Troll* _saveTroll;
	
	
    float _gameSlowTimer;
    
    float _dwarfPointMulty;
    
    
    float _boostCrystalsTimer;
    float _boostShieldTimer;//Not in use now
    
    
    float _actionTrollSpawn;
    float _actionTrollSpawnTime;
    
    float _mushroomTimer;
    
    float _boostFutureSee;
    
    int _boostExtraPoints;
	
	int _minDwarfCountInTime;
    float _diamondTimer;
    
    //Plants
    float _plantCrystalTime;
	float _plantFuzzBall;
	
	bool _plantFuzzFlowerFirst;
    bool _plantSunFlowerFirst;
    bool _plantIceFlowerFirst;
	
	cocos2d::CCLabelTTF* _diamondsLabel;
};
