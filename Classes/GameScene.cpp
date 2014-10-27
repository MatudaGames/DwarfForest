//
//  GameScene.cpp
//  DwarfForest
//
//  Created by Elviss Strazdins on 18.02.2013.
//
//

//#include "FlurryX.h"

#include "AnalyticX.h"

#include <SimpleAudioEngine.h>
#include "GameScene.h"
#include "MainMenuScene.h"
#include "PauseScene.h"
#include "SaveMeScene.h"
#include "AppMacros.h"
#include "Troll.h"
#include "Crystal.h"
#include "Effect.h"
#include "IceBarrage.h"
#include "Web.h"
#include "Diamond.h"
#include "Utils.h"
#include "TrollIntro.h"
#include "DwarfIntro.h"
#include "CrystalIntro.h"
#include "EffectIntro.h"
#include "DiamondIntro.h"
#include "Levels.h"
#include "User.h"
#include "PopUp.h"
#include "Mushroom.h"
#include "DailyChallengesPopUp.h"
#include "LevelEndScene.h"
#include "TutorialScene.h"
#include "CCShake.h"

#include "HidraIntro.h"

#include "MissionScene.h"

#include "PlantCrystal.h"
#include "PlantFuzzBall.h"
#include "PlantIceFlower.h"
#include "PlantSunFlower.h"

#include "StaticSceneManager.h"

#include "JniCalls.h"

#include "Machine_Stamp.h"
#include "Machine_Ghost.h"
#include "Machine_Enemies.h"

#include "Plant_Crystal_Weed.h"

#include "Wind.h"
#include "Rain.h"

#include "FreeStuffPopup.h"
#include "TrollBullet.h"

USING_NS_CC;
using namespace CocosDenshion;

const float GAME_SPEED_NORMAL = 1.0f;// default 1.5
const float GAME_SPEED_FAST = 2.0f;// default 2.2

const float CAVE_DISTANCE = 50.0f;
const float CAVE_DISTANCE_TALL = 60.0f;

const float TROLL_DISTANCE = 30.0f;
const float CRYSTAL_DISTANCE = 50.0f;//30
const float EFFECT_DISTANCE = 50.0f;

const float EFFECT_DISTANCE_TORNADO = 1400.0f;
const float EFFECT_DISTANCE_RAIN = 2000.0f;

const float DIAMOND_DISTANCE = 50.0f;//30

const float MUSHROOM_DISTANCE = 50.0f;
const float MUSHROOM_WAIT = 120.0f;//120

const float EXTRA_CRYSTAL_SPAWN = 60.0f;//120


const float DWARF_GENERATE_INTERVAL = 3.0f;
const float TROLL_GENERATE_INTERVAL = 4.0f;
const float CRYSTAL_GENERATE_INTERVAL = 10.0f;
const unsigned int MAX_DWARFS = 30;//10
const unsigned int MAX_TROLLS = 10;//5
const unsigned int MAX_CRYSTALS = 30;//5
const unsigned int MAX_EFFECTS = 3;//5
const unsigned int MAX_DIAMONDS = 5;//5

const unsigned int CAVE_SCORE = 10;

const unsigned int DIAMOND_SPAWN_TIME = 300;

//Whats the price of boosters
const unsigned int BOOSTER_1_PRICE = 5;
const unsigned int BOOSTER_2_PRICE = 5;
const unsigned int BOOSTER_3_PRICE = 5;
const unsigned int BOOSTER_4_PRICE = 5;

#define MissionType_PointCount 0
#define MissionType_DwarfCount 1
#define MissionType_Combo 2

#define MissionType_Time 3
#define MissionType_Mushroom 4
#define MissionType_TrollEscape 5

//The special stuff collect
#define MissionType_Crystal_Red 100
#define MissionType_Crystal_Blue 101
#define MissionType_Crystal_Yellow 102
#define MissionType_Crystal_Green 103


#define kBoosters 1000
#define kBooster_NoEnemy 1003
#define kBooster_Ghost 1002
//#define kBooster_Shield 1003
#define kBooster_Crystals 1004
#define kBooster_FutureSee 1001

#define kMushroom 1100

#define kPoints_Z_Order 100
#define kHUD_Z_Order 200

#define kFreezeSprite 111
#define kNoPause 112

#define kBoostMenu 89

#define kPlant_Crystal 2001
#define kPlant_FuzzBall 2002
#define kPlant_SunFlower 2003
#define kPlant_IceFlower 2004

#define kMap_Summer 1
#define kMap_Autumn 2
#define kMap_Winter 3
#define kMap_Spring 4


CCScene* GameScene::scene()
{
//    if (Time::isToday(User::getInstance()->getDailyChallenges().getLastSpinTime()) == false)
//    {
//        return DailyChallengesPopUp::scene();
//    }
//    else
//    {
//        // 'scene' is an autorelease object
//        CCScene *scene = CCScene::create();
//        
//        // 'layer' is an autorelease object
//        GameScene *layer = GameScene::create();
//        
//        // add layer as a child to scene
//        scene->addChild(layer);
//        
//        // return the scene
//        return scene;
//    }
    
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

GameScene::GameScene():
	dailyChallengeInfo(NULL), _bullets(NULL),_dwarves(NULL), _trolls(NULL), _crystals(NULL), _effects(NULL), _diamonds(NULL),
	_introAnimations(NULL), _scoreLabel(NULL), _cave(NULL), _caveMask(NULL), _mask(NULL),_MasterTroll_IdleAnimation(NULL),_MasterTroll_WinAnimation(NULL),
	_gameTime(0)
{
	
    
    User::getInstance()->getDailyChallenges().checkDayCounter();
}

GameScene::~GameScene()
{
    if(_MasterTroll_IdleAnimation)_MasterTroll_IdleAnimation->release();
    if(_MasterTroll_WinAnimation)_MasterTroll_WinAnimation->release();
    
    if (_bullets) _bullets->release();
	if (_dwarves) _dwarves->release();
    if (_goblins) _goblins->release();
    if (_hidras) _hidras->release();
    if (_spiders) _spiders->release();
	if (_trolls) _trolls->release();
	if (_crystals) _crystals->release();
	if (_effects) _effects->release();
	if (_diamonds) _diamonds->release();
	if (_introAnimations) _introAnimations->release();
	if (_caveMask) _caveMask->release();
	if (_mask) _mask->release();
    if(_flyObjects)_flyObjects->release();
}

/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        CCLog("FMOD error %d - %s", result, FMOD_ErrorString(result));
    }
}

const char *Common_MediaPath(const char *fileName)
{
    return CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName).c_str();
}

void GameScene::updateSound(float delta)
{
    if(!mFmodEnabled)
        return;
    
    float aVolume = 0;
    
    if(!mMusic1_Done)
    {
        if(mMusic1_On)
        {
            //Check the volume
            channel1->getVolume(&aVolume);
            if(aVolume<1.0f)
            {
                aVolume+=delta;
                if(aVolume>=1.0f)
                {
                    aVolume = 1.0f;
                    mMusic1_Done = true;
                }
                channel1->setVolume(aVolume);
            }
        }
        else
        {
            channel1->getVolume(&aVolume);
            if(aVolume>0.0f)
            {
                aVolume-=delta;
                if(aVolume<=0.0f)
                {
                    aVolume = 0.0f;
                    mMusic1_Done = true;
                }
                channel1->setVolume(aVolume);
            }
        }
    }
    
    if(!mMusic2_Done)
    {
        if(mMusic2_On)
        {
            //Check the volume
            channel2->getVolume(&aVolume);
            if(aVolume<1.0f)
            {
                aVolume+=delta;
                if(aVolume>=1.0f)
                {
                    aVolume = 1.0f;
                    mMusic2_Done = true;
                }
                channel2->setVolume(aVolume);
            }
        }
        else
        {
            channel2->getVolume(&aVolume);
            if(aVolume>0.0f)
            {
                aVolume-=delta;
                if(aVolume<=0.0f)
                {
                    aVolume = 0.0f;
                    mMusic2_Done = true;
                }
                channel2->setVolume(aVolume);
            }
        }
    }
    
    if(!mMusic3_Done)
    {
        if(mMusic3_On)
        {
            //Check the volume
            channel3->getVolume(&aVolume);
            if(aVolume<1.0f)
            {
                aVolume+=delta;
                if(aVolume>=1.0f)
                {
                    aVolume = 1.0f;
                    mMusic3_Done = true;
                }
                channel3->setVolume(aVolume);
            }
        }
        else
        {
            channel3->getVolume(&aVolume);
            if(aVolume>0.0f)
            {
                aVolume-=delta;
                if(aVolume<=0.0f)
                {
                    aVolume = 0.0f;
                    mMusic3_Done = true;
                }
                channel3->setVolume(aVolume);
            }
        }
    }
    
    if(system)
        system->update();
}
#endif
*/

void GameScene::updateDwarfMusic()
{
    if(!mFmodEnabled)
        return;
    
    //Check how many dwarfs are on the map !!!
    if(_dwarves->count() <3)
    {
        if(mMusic1_On!=false)
            mMusic1_Done = false;
        if(mMusic2_On!=false)
            mMusic2_Done = false;
        if(mMusic3_On!=false)
            mMusic3_Done = false;
        
        mMusic1_On = false;
        mMusic2_On = false;
        mMusic3_On = false;
    }
    else if(_dwarves->count()<5)
    {
        if(mMusic1_On!=true)
            mMusic1_Done = false;
        if(mMusic2_On!=false)
            mMusic2_Done = false;
        if(mMusic3_On!=false)
            mMusic3_Done = false;
        
        mMusic1_On = true;
        mMusic2_On = false;
        mMusic3_On = false;
    }
    else if(_dwarves->count()<7)
    {
        if(mMusic1_On!=true)
            mMusic1_Done = false;
        if(mMusic2_On!=true)
            mMusic2_Done = false;
        if(mMusic3_On!=false)
            mMusic3_Done = false;
        
        mMusic1_On = true;
        mMusic2_On = true;
        mMusic3_On = false;
    }
    else
    {
        if(mMusic1_On!=true)
            mMusic1_Done = false;
        if(mMusic2_On!=true)
            mMusic2_Done = false;
        if(mMusic3_On!=true)
            mMusic3_Done = false;
        
        mMusic1_On = true;
        mMusic2_On = true;
        mMusic3_On = true;
    }
}

//================================================================
// The MAP GENERATOR THE BASE STUFF THE MAGIC THE POWA :D !!!

void GameScene::CreateRandomMap()
{
    mNewMapRandomEnabled = true;
    
    //Start with place search for cave !!!
    _CaveSpawnPoints.push_back((SpawnPoint){190, 553});
    _CaveSpawnPoints.push_back((SpawnPoint){190, 404});
    _CaveSpawnPoints.push_back((SpawnPoint){190, 245});
    
    _CaveSpawnPoints.push_back((SpawnPoint){327, 605});
    _CaveSpawnPoints.push_back((SpawnPoint){327, 458});
    _CaveSpawnPoints.push_back((SpawnPoint){327, 310});
    
    _CaveSpawnPoints.push_back((SpawnPoint){500, 634});
    _CaveSpawnPoints.push_back((SpawnPoint){500, 475});
    _CaveSpawnPoints.push_back((SpawnPoint){500, 330});
    _CaveSpawnPoints.push_back((SpawnPoint){500, 179});
    
    _CaveSpawnPoints.push_back((SpawnPoint){327, 614});
    _CaveSpawnPoints.push_back((SpawnPoint){327, 454});
    _CaveSpawnPoints.push_back((SpawnPoint){327, 310});
    
    _CaveSpawnPoints.push_back((SpawnPoint){784, 553});
    _CaveSpawnPoints.push_back((SpawnPoint){784, 404});
    _CaveSpawnPoints.push_back((SpawnPoint){784, 245});
    
    //Take the random cave
    int aCaveSpawnID = rand() % _CaveSpawnPoints.size();
    SpawnPoint aCavePos = _CaveSpawnPoints[aCaveSpawnID];
    
    //Create the cave here !!!
    mCaveFinalCords.x = aCavePos.x;
    mCaveFinalCords.y = aCavePos.y;
    
    
    //By the cave ID know where points can be made !!!
    CreatePossibleRandomDwarfSpawns(aCaveSpawnID);
    
    //debug check what did we get
    for (int x=0; x<_DwarfSpawnPoints.size();x++){
        SpawnPoint aDwarfSpawn2 = _DwarfSpawnPoints[x];
        std::cout << aDwarfSpawn2.x << "\n";
    }
    
    //Now check how much spawn points will use !!!
    int aMaxSpawns = (rand() % 4)+4;// from 4 to 8 spawn points
    
    //Now get the spawn spots !!!
    SpawnPoint aDwarfSpawn;
    int aRandomID = 0;
    
    for(int i=0;i<aMaxSpawns;i++)
    {
        aRandomID = rand()%_DwarfSpawnPoints.size();
        aDwarfSpawn = _DwarfSpawnPoints[aRandomID];
        _DwarfSpawnPoints.erase(_DwarfSpawnPoints.begin()+aRandomID);
        
        //Create the spots for spawn !!!
        
        //Add it to the list
        _genearetPoints.push_back((GeneratePoint){aDwarfSpawn.x, aDwarfSpawn.y, aDwarfSpawn.angle});//M_PI / 2
        
        //Add debug sprite for now !!!
        CCSprite* aDebug = CCSprite::create("small_dot_blue.png");
        aDebug->setPosition(ccp(aDwarfSpawn.x, aDwarfSpawn.y));
        addChild(aDebug,1000);
    }
}

//Some reference
////bottom
//_genearetPoints.push_back((GeneratePoint){252, 719 - 0, 4.7});//M_PI / 2
//_genearetPoints.push_back((GeneratePoint){756, 719 - 0, 4.7});//M_PI / 2
//
////right
//_genearetPoints.push_back((GeneratePoint){959, 719 - 240, M_PI});//240
//_genearetPoints.push_back((GeneratePoint){959, 719 - 580, M_PI});//580  540
//
////top
//_genearetPoints.push_back((GeneratePoint){756, 719 - 719, 1.65});//3.0 * M_PI / 2
//_genearetPoints.push_back((GeneratePoint){252, 719 - 719, 1.65});//3.0 * M_PI / 2
//
////left
//_genearetPoints.push_back((GeneratePoint){0, 719 - 540, 0});
//_genearetPoints.push_back((GeneratePoint){0, 719 - 240, 0});

void GameScene::CreatePossibleRandomDwarfSpawns(int theMapID)
{
    //Check what points can be spawned
    
    //Start from spawn point #1
    if(theMapID != 1 && theMapID != 4){
        _DwarfSpawnPoints.push_back((SpawnPoint){266,719,4.7});//Spawn point #1
    }
    //And so go on to next spots !!!
    if(theMapID != 4 && theMapID != 7){
        _DwarfSpawnPoints.push_back((SpawnPoint){422,719,4.7});//Spawn point #2
    }
    if(theMapID != 7 && theMapID != 11){
        _DwarfSpawnPoints.push_back((SpawnPoint){575,719,4.7});//Spawn point #3
    }
    if(theMapID != 11 && theMapID != 14){
        _DwarfSpawnPoints.push_back((SpawnPoint){730,719,4.7});//Spawn point #4
    }
    if(theMapID != 14 && theMapID != 15){
        _DwarfSpawnPoints.push_back((SpawnPoint){959,476,M_PI});//Spawn point #5
    }
    if(theMapID != 15 && theMapID != 16){
        _DwarfSpawnPoints.push_back((SpawnPoint){959,319,M_PI});//Spawn point #6
    }
    if(theMapID != 16){
        _DwarfSpawnPoints.push_back((SpawnPoint){959,178,M_PI});//Spawn point #7
    }
    
    //The #8,9,10,11 spot always available
    _DwarfSpawnPoints.push_back((SpawnPoint){730,0,1.65});//Spawn point #8
    _DwarfSpawnPoints.push_back((SpawnPoint){575,0,1.65});//Spawn point #9
    _DwarfSpawnPoints.push_back((SpawnPoint){414,0,1.65});//Spawn point #10
    _DwarfSpawnPoints.push_back((SpawnPoint){262,0,1.65});//Spawn point #11
    
    if(theMapID != 3){
        _DwarfSpawnPoints.push_back((SpawnPoint){0,184,0});//Spawn point #12
    }
    if(theMapID != 3 && theMapID != 2){
        _DwarfSpawnPoints.push_back((SpawnPoint){0,329,0});//Spawn point #13
    }
    if(theMapID != 1 && theMapID != 2){
        _DwarfSpawnPoints.push_back((SpawnPoint){0,470,0});//Spawn point #14
    }
}


//================================================================

void GameScene::CreateMap()
{
    _SpawnBlueDwarf = true;
    _SpawnOrangeDwarf = true;
    
    int aPossibleSpawnPoints = mCurrentMission.Dwarf_spawn_points;
    if(aPossibleSpawnPoints<=0) aPossibleSpawnPoints = 8;//Max dwarf spawn points
    
    std::vector<int> _dwarfSpawnData;//The spawn points
    
    //Get some random values or show correct spots if so for spawn points !!!
    if(mCurrentMission.Dwarf_paths.size()>0)
    {
        //We will have forced stuff
        for(int i=0;i<mCurrentMission.Dwarf_paths.size();i++)
        {
            _dwarfSpawnData.push_back(mCurrentMission.Dwarf_paths[i]);
        }
    }
    else
    {
        //lets do the random stuff
        //TODO::
    }
    
    //Now run trough and set the correct places for spawn
    for(int i = 0;i<_dwarfSpawnData.size();i++)
    {
        if(_dwarfSpawnData[i] == 0){_genearetPoints.push_back((GeneratePoint){0,180,0});}
        else if(_dwarfSpawnData[i] == 1){_genearetPoints.push_back((GeneratePoint){0,470,0});}
        else if(_dwarfSpawnData[i] == 2){_genearetPoints.push_back((GeneratePoint){266,719,4.7});}
        else if(_dwarfSpawnData[i] == 3){_genearetPoints.push_back((GeneratePoint){730,719,4.7});}
        else if(_dwarfSpawnData[i] == 4){_genearetPoints.push_back((GeneratePoint){959,476,M_PI});}
        else if(_dwarfSpawnData[i] == 5){_genearetPoints.push_back((GeneratePoint){959,178,M_PI});}
        else if(_dwarfSpawnData[i] == 6){_genearetPoints.push_back((GeneratePoint){730,0,1.65});}
        else if(_dwarfSpawnData[i] == 7){_genearetPoints.push_back((GeneratePoint){262,0,1.65});}
    }
    
//    _genearetPoints.push_back((GeneratePoint){266,719,4.7}); // 2
//    _genearetPoints.push_back((GeneratePoint){730,719,4.7}); // 3
//    _genearetPoints.push_back((GeneratePoint){959,476,M_PI}); // 4
//    _genearetPoints.push_back((GeneratePoint){959,178,M_PI}); // 5
//    _genearetPoints.push_back((GeneratePoint){730,0,1.65}); // 6
//    _genearetPoints.push_back((GeneratePoint){262,0,1.65}); // 7
    
//    _genearetPoints.push_back((GeneratePoint){0,470,0}); // 1
//    _genearetPoints.push_back((GeneratePoint){0,180,0}); // 0
    
    //now do the other nice stuff
    
    // The tall orange dwarf !!!
    _caveTall = CCSprite::create("cave/trapdoor-orange.png");
    _caveTall->setPosition(ccp(mCurrentMission.OrangeCave_x,mCurrentMission.OrangeCave_y));
    if(mCurrentMission.OrangeCave_x==0 && mCurrentMission.OrangeCave_y==0){
        _SpawnOrangeDwarf = false;
    }
    else{
        addChild(_caveTall,0);
    }
    
    _caveFat = CCSprite::create("cave/trapdoor-blue.png");
    _caveFat->setPosition(ccp(mCurrentMission.BlueCave_x,mCurrentMission.BlueCave_y));
    if(mCurrentMission.BlueCave_x==0 && mCurrentMission.BlueCave_y==0){
        _SpawnBlueDwarf = false;
    }
    else{
        addChild(_caveFat,0);
    }
    
    //Where dwarfs should enter to finish the game !!!
    _cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_TALL, ccp(_caveTall->getPositionX(), _caveTall->getPositionY())));
    _cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_FAT, ccp(_caveFat->getPositionX(), _caveFat->getPositionY())));
    
    //The cave arrows
    _blueArrowAnim = TimedSpriteAnimation::create("cave/blue.plist", 0.1);
    _blueArrowAnim->retain();
    _blueArrowAnim->setTag(79);
    _blueArrowAnim->setPosition(ccp(_caveFat->getPositionX()-40,
                                    _caveFat->getPositionY()-50));
    
    _orangeArrowAnim = TimedSpriteAnimation::create("cave/orange.plist", 0.1);
    _orangeArrowAnim->retain();
    _orangeArrowAnim->setTag(78);
    _orangeArrowAnim->setPosition(ccp(_caveTall->getPositionX()+40,
                                      _caveTall->getPositionY()-50));
    
    //Wind move points - should check map layout :)
    _movePointsWind = CCPointArray::create(0);
	_movePointsWind->retain();
    
    float precision = .25f;
    float cir = 2 * M_PI;
    int mRadius = 280;
    for (float a = 0.1f; a < cir; a += precision)
    {
        float x = visibleSize.width/2 + mRadius * cos(a);
        float y = visibleSize.height/2 + mRadius/1.5f * sin(a);
        _movePointsWind->addControlPoint(ccp(x,y-50));
    }
    
    //Set the troll stuff
    for(int i = 0;i<mCurrentMission.Enemy_info.size();i++)
    {
        MissionTroll theTroll = mCurrentMission.Enemy_info[i];
        generateTrollForMission(theTroll);
    }
    
    //Check how much dwarfs should instantly come in!!!
    for(int i = 0;i<mCurrentMission.StartDwarfCount;i++){
        //just generate all possible dwarfs !!!
        generateDwarfMission(false);
    }
}

//The new stuff where all is clean and understandable
void GameScene::CreateGameByMission()
{
    ResetValues();
    
    setTag(11);//The game tag !!! ???
    
    //=================================================================
    //Clear default values
    
    visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // Do we need to show tutorial
    if(User::getInstance()->_tutorial==0){
        mTutorialEnabled = true;
        
        //Some diamond check
        if(User::getInstance()->getDiamonds()>=26)
            User::getInstance()->setDiamonds(25);
    }
    else{
        mTutorialEnabled = false;
        User::getInstance()->addTimesPlayed();
    }
    
    //Disabled for now!!!
    mTutorialEnabled = false;
    
    _mission_star_points_1 = mCurrentMission.Star_1;
    _mission_star_points_2 = mCurrentMission.Star_2;
    _mission_star_points_3 = mCurrentMission.Star_3;
    
    _missionCurrentValue = -1;//So that it does update at 1st
    
    //Set the spawn stuff for start !!
    _DSpawnCurrentTime = 0;//When to change the zone
    
    if(mCurrentMission.DSpawn_change_jump_time>0) _DSpawn_change_jump = mCurrentMission.DSpawn_change_jump_time;
    else _DSpawn_change_jump = -1;//Do not check it
    _DSpawn_Real_Jump = mCurrentMission.DSpawn_jump;
    
    if(mCurrentMission.DSpawn_change_max_time>0) _DSpawn_change_max = mCurrentMission.DSpawn_change_max_time;
    else _DSpawn_change_max = -1;//Do not check it
    _DSpawn_Real_Max = mCurrentMission.DSpawn_max;
    
    if(mCurrentMission.DSpawn_change_min_time>0) _DSpawn_change_min = mCurrentMission.DSpawn_change_min_time;
    else _DSpawn_change_min = -1;//Do not check it
    _DSpawn_Real_Min = mCurrentMission.DSpawn_min;
    
    if(mCurrentMission.DSpawn_change_zone_time>0) _DSpawn_change_zone = mCurrentMission.DSpawn_change_zone_time;
    else _DSpawn_change_zone = -1;//Do not check it
    _DSpawn_Real_Zone = mCurrentMission.DSpawn_zone;//TWhats the current zone length for spawn?
    
    _CurrentSpawnCount = mCurrentMission.DSpawn_min;//We dont know
    
    //+create the basic stuff
    
    _DSpawnGameMinCurrent = 0;
    _DSpawnTimer = mCurrentMission.DSpawn_zone_step;
    
    _CrystalSpawnRecheckTimer = 0;//For now
    
    
    //=================================================================
    // The mess section ?!@$
    
    mNewMapRandomEnabled = false;
    
    _DwarfsCollectedObject = 0;
    _DwarfsEnteredCave = 0;
    
    mFlurry_GhostActivated = 0;
    mFlurry_CrystalPoints = 0;
    mFlurry_BlitzActivated = 0;
    mFlurry_StampActivated = 0;
    mFlurry_MachinesActivated = 0;
    
    mCurrentSmallTutorial = -1;
    
    _mission_dwarfs_saved_counter = 0;
    _mission_count_dwarfs = false;
    
    mFlurry_MachinesActivated = 0;
    
    mTrollTimer = 0;
    
    mTotalPointsInGame = 0;
    _SpawnSpecialTrolls = 0;
    
    _mission_dwarfs_spawned = 0;
    _mission_dwarfs_max = 0;
    
    mFadeOutMusic = false;
    mFadeInMusic = false;
    mBackMusicVolume = 1.0f;
    
    mSpeciaCrystallDwarfID = -1;
    
    mTotalCombo = 1;
    
    mDebugComboValue = 1;
    
    //For now disabled
    mPaternFormulas = false;
    
    _mission_dwarfs_removed = 0;
    
    mTimeNotCollectedCrystal = 0.0f;
    mTimeToCheckAgainNoCrystal = 0.0f;
    
    mCurretnEffectDistance = 0.0f;
    mNoDwarfEneterCave = 0.0f;
    
    _rainTime = 0;
    _whenToRain = 0;
    rainTimeSet = false;
    
    _windTime = 0;
    _whenToWind = (rand()%45)+45;
    
    _windLastSpawnBlockID = -1;
    
    mRainActive = false;
    mTornadoActive = false;
    
    mMapDangerType = 1;//Basic danger only
    mMapDangerTimer = 0;
    mMapDangerTimerReq = (rand()%45)+45;
    
    mMapDangerCooldown = 0;
    
    mIntroPlaying = false;
    
    mCurrentSmallTutorial = -1;
    
    mCombo_DwarfEnter = 1;
    mCombo_CollectCrystals = 1;
    mComboTimer_CollectCrystal = 0;
    mComboTimer_DwarfEnter = 0;
    
    _mission_dwarfs_saved_counter = 0;
    
    mSplitCavesMode = false;
    
    mScoreMulti_Global = User::getInstance()->getScore_Multi();
    
    mShowTutorialCompleted = -1;
    
    mCurrentMusicID = -1;
    
    _generatedExtraStartDwarf = false;
    
    mTutorialStep = 0;//should be 0
    
    mTutorialFlag_1 = false;
    mTutorialFlag_2 = false;
    
    mTutorialTimer = -1;
    mTutorialTimerSet = false;
    
    mTutorialPointsAdded = false;
    mTutorialDrawHand = false;
    
    _DwarfsSpawned = 0;
    
    //For 1st dwarf too
    mSpecialCrystalSpawnTimer = 4;
    mCanSpawnExtraCrystal = false;
    
    mCrystalPoints = 0;
    
    _plantIceFlowerFirst = false;
    _plantSunFlowerFirst = false;
    _plantFuzzFlowerFirst = false;
    
    _gameTime = 0;
    _actionTrollSpawnTime = -1;
    
    _currentMinSpwanedTime = 0;//For the first meteorite - some random value above 20sec?
    _currentMinForSpawn = -1;
    _effectActiveSpawnTime = rand() % 15 + 40;//For the first meteorite - some random value above 20sec?
    
    _crystalCurrentSpawnMinute = -1;
    _crystalCurrentActiveTime = rand()%10+30;
    _crystalCurrentTime = 0;
    
    _trollCurrentSpawnMinute = -1;
    _trollCurrentActiveTime = rand()%10+20;
    _trollCurrentTime = 0;
    
    _dwarfCurrentTime = 0;
    _dwarfCurrentSpawnMinute = -1;
    _dwarfCurrentActiveTime = 0;
    
    mTrollFreezeDwarfTime = 0;
    mTrollFreeze = 0;
    
    _dwarfOnMap = NULL;
    
    _plantCrystalTime = 0;
    _plantFuzzBall = 0;
    
    _dwarfCurrentSpawnMinute = 0;
    _trollCurrentSpawnMinute = 0;
    _crystalCurrentSpawnMinute = 0;
    _currentMinForSpawn = 0;
    
    _lastSavePrice = 1;//Start with 2 crystals
    
    _effectLastSpawnBlockID = -1;
    
    //Some flags
    _boosterMushroom = false;//If true - will allow extra booster to enable
    _totalActiveBoosters = 0;
    
    _diamondTimer = 0;
    
    _willUseMushroom = false;
    
    _lastSpawnPoint = -1;
    
    _screenSize = visibleSize;
    _origin = origin;
    _totalStepsZ = _screenSize.height/2/10;
    
    mDebugInfoVisible = false;
    
    mDebugFormulas = false;
    
    _DailySpinVisible = false;
    _saveTroll = NULL;
    _saveDwarf2 = NULL;
    _saveDwarf1 = NULL;
    
    _gamePause = false;
    _dwarfPointMulty = 1.0f;
    
    
    _dwarfTimer = 3;//We can spawn dwarf
    _gameSpeed = GAME_SPEED_NORMAL;//Normal // was 1.5
    _gameSlowTimer = 0.0f;
    
    _boostShieldTimer = 0.0f;
    _boostCrystalsTimer = 0.0f;
    _boostExtraPoints = 0;
    _boostGhostTimer = 0.0f;
    _boostNoEnemyTimer = 0.0f;
    _boostFutureSee = 0.0f;
    
    //What is this
    _blockedGeneratePoints.clear();
    
    _bullets = CCArray::create();
	_bullets->retain();
    
	_dwarves = CCArray::create();
	_dwarves->retain();
    
    _goblins = CCArray::create();
	_goblins->retain();
    
    _hidras = CCArray::create();
    _hidras->retain();
    
    _spiders = CCArray::create();
    _spiders->retain();
    
    _flyObjects = CCArray::create();
    _flyObjects->retain();
	
	_trolls = CCArray::create();
	_trolls->retain();
	
	_crystals = CCArray::create();
	_crystals->retain();
	
	_effects = CCArray::create();
	_effects->retain();
    
    _tornadoActive = CCArray::create();
    _tornadoActive->retain();
	
	_diamonds = CCArray::create();
	_diamonds->retain();
	
	_introAnimations = CCArray::create();
	_introAnimations->retain();
    
    
    //=================================================================
    
    //Create the map by the map id
    CreateStartMap(mCurrentMission.MapType);
    
    CreatePossibleObjectSpawns();
    
    //Create the map?
    //Start with map layout and all the main stuff !!!
    CreateMap();
    
    //+Create the HUD stuff
    CreateGameStartHUD();
    
    mTotalCombo = 0;
    mTotalPoints = 0;
    mTotalBlueDwarfs = 0;
    mTotalOrangeDwarfs = 0;
    CheckMissionByValue(mCurrentMission.Task_type,0);
    
    //For now - use the old map mask stuff
    _mask = new CCImage();
    _mask->initWithImageFile("kartes_maska.png");
    _mask->retain();
    
    //-------------------------------------------------------
    //The clear cool values - that we know what they do !!!
    
    mNewSplitCaves = true;//Always use the new caves
    mZipZapActive = false;//Flag for showing which target can be zipped
    
    mFmodEnabled = false; //Fmod tests !!!
    
    //Flags that possible are not needed anymore - but we will clear them by def
    mCheckSpecialSpotTouch = false;//Flag for old mission 11 [some special touch spot start]
    
    //-----------------------------------------------------------------
    // Some extra stuff
    
    //Can we change it???
    mTimeToChangeSeason = -1;//For now disabled
    
    //The magic mushroom
    if(User::getInstance()->getMissionManager().GetActiveMissionID()>1)
        _mushroomTimer = MUSHROOM_WAIT;
    else
        _mushroomTimer = -1;
    
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true) == false){
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
    else{
        PlaySpecialMusic(0);
    }
    
    // Play startup sound when all done
    playInGameSound("button_start_game");
    
    //Start the updater
    this->scheduleUpdate();
    
    //Check if need to show some stuff at start???
    
    //Check what is this mission and what need to do !!!
    if(mCurrentMission.Story_show>0)
    {
        //We need to create some intro scene
        CreateMissionStart();//For now just start the game with all the mission stuff
        return;
    }
    else{
        CreateMissionStart();
    }
}

void GameScene::ShowTheHUD()
{
    //Create all the tweens !!!
    CCMenu* aDummyMenu = (CCMenu*)getChildByTag(100);
    CCMoveTo* aActionMove1;
    CCEaseSineOut* aEaseActionMove1;
    
    aActionMove1 = CCMoveTo::create(0.5f,ccp(5, 5));
    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
    aDummyMenu->runAction(aEaseActionMove1);
    
    aDummyMenu = (CCMenu*)getChildByTag(90002);
    aActionMove1 = CCMoveTo::create(0.5f,ccp(_screenSize.width - 5, 5));
    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
    aDummyMenu->runAction(aEaseActionMove1);
    
    aDummyMenu = (CCMenu*)getChildByTag(90003);
    aActionMove1 = CCMoveTo::create(0.5f,ccp(0, _screenSize.height-54));
    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
    aDummyMenu->runAction(aEaseActionMove1);
    
    aDummyMenu = (CCMenu*)getChildByTag(90004);
    aActionMove1 = CCMoveTo::create(0.5f,ccp((_screenSize.width-aDummyMenu->getContentSize().width/2)+21, _screenSize.height-55));
    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
    aDummyMenu->runAction(aEaseActionMove1);
    
    aDummyMenu = (CCMenu*)getChildByTag(90005);
    aActionMove1 = CCMoveTo::create(0.5f,ccp(_screenSize.width-aDummyMenu->getContentSize().width/2,_screenSize.height-10));
    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
    aDummyMenu->runAction(aEaseActionMove1);
}

//Creates the mission intro
void GameScene::CreateMissionStart()
{
    //Create some popup and explain what need to do !!!
    
    
    //Prepeare the mission intro !!!
    if(mCurrentMission.Task_type == 0){
        //Crystal count
        
    }
    else if(mCurrentMission.Task_type == 1){
        //Dwarf count
        
    }
    else if(mCurrentMission.Task_type == 2){
        //Activate power up
        
    }
    else if(mCurrentMission.Task_type == 3){
        // Collect green crystals
        
    }
    
    //After this do it
    ShowTheHUD();
}

void GameScene::CreateGameStartHUD()
{
    _tutorialLabel = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(700,240), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	_tutorialLabel->setString("Welcome to DF");
    _tutorialLabel->setColor(ccc3(255,246,200));
    _tutorialLabel->setAnchorPoint(ccp(0.5f,0.5f));
	_tutorialLabel->setPosition(ccp(visibleSize.width/2,visibleSize.height-100));
    _tutorialLabel->setVisible(false);
    _tutorialLabel->setTag(kNoPause);
	this->addChild(_tutorialLabel, kHUD_Z_Order+1);
    
    _tutorialLabel2 = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.42, CCSize(700,240), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	_tutorialLabel2->setString("Welcome to DF");
    _tutorialLabel2->setColor(ccc3(255,246,200));
    _tutorialLabel2->setAnchorPoint(ccp(0.5f,0.5f));
	_tutorialLabel2->setPosition(ccp(visibleSize.width/2,visibleSize.height-140));
    _tutorialLabel2->setVisible(false);
	this->addChild(_tutorialLabel2, kHUD_Z_Order+1);
    
    _scoreLabel = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(115, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
	_scoreLabel->setString("0");
    _scoreLabel->setColor(ccc3(255,246,200));
    
    _diamondsLabel = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(90, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
	_diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
    _diamondsLabel->setColor(ccc3(255,246,200));
    
    _pointsLabel = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(120, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	_pointsLabel->setString("0");
    _pointsLabel->setColor(ccc3(255,246,200));
    
    
    // The HUD back
    CCSprite* pointsBack = CCSprite::create("Interfeiss/in_game/crystal_count.png");
    pointsBack->setAnchorPoint(ccp(0.5, 1));
    pointsBack->setPosition(ccp(visibleSize.width+pointsBack->getContentSize().width,visibleSize.height-10));
    addChild(pointsBack, kHUD_Z_Order-1);
    
    _scoreLabel->setAnchorPoint(ccp(0.5,0.5));
	_scoreLabel->setPosition(ccp(pointsBack->getContentSize().width/2-40,pointsBack->getContentSize().height/2+1));
	pointsBack->addChild(_scoreLabel);
    pointsBack->setTag(90005);
    
    CCSprite* diamondsBack = CCSprite::create("Interfeiss/in_game/diamond_count.png");
    diamondsBack->setAnchorPoint(ccp(0.5, 1));
    diamondsBack->cocos2d::CCNode::setPosition(ccp(visibleSize.width+diamondsBack->getContentSize().width+21, visibleSize.height-55));
    addChild(diamondsBack, kHUD_Z_Order-1);
    
    _diamondsLabel->setAnchorPoint(ccp(0.5,0.5));
	_diamondsLabel->setPosition(ccp(diamondsBack->getContentSize().width/2-45,diamondsBack->getContentSize().height/2+2));//27
	diamondsBack->addChild(_diamondsLabel);
    diamondsBack->setTag(90004);
    
    CCSprite* pointsTotalBack = CCSprite::create("Interfeiss/in_game/score.png");
    pointsTotalBack->setAnchorPoint(ccp(0, 0.5));
    pointsTotalBack->cocos2d::CCNode::setPosition(-pointsTotalBack->getContentSize().width, visibleSize.height-54);
    addChild(pointsTotalBack, kHUD_Z_Order-1);
    
    _pointsLabel->setAnchorPoint(ccp(0.5,0.5));
    _pointsLabel->setPosition(ccp(pointsTotalBack->getContentSize().width/2-8,pointsTotalBack->getContentSize().height/2+9));//27
    pointsTotalBack->addChild(_pointsLabel);
    pointsTotalBack->setTag(90003);
    
    //---------------------------------------------------------------------------------------------------
    //The pause and speed up button
    
    CCMenuItemImage* pauseItem = CCMenuItemImage::create(
                                                         "Interfeiss/in_game/pause.png",
                                                         "Interfeiss/in_game/pause_pressed.png",
                                                         this,
                                                         menu_selector(GameScene::menuPauseCallback));
    pauseItem->setAnchorPoint(ccp(1,0));
    
    CCMenu* pauseMenu = CCMenu::create(pauseItem, NULL);
    this->addChild(pauseMenu, kHUD_Z_Order-1);
    pauseMenu->setPosition(visibleSize.width - 5, -100);
    pauseMenu->setTag(90002);
    
    //===============================================================
    //Cheat for mission skip //This is debug stuff
    /*
    pauseItem = CCMenuItemImage::create(
                                        "Interfeiss/in_game/pause.png",
                                        "Interfeiss/in_game/pause_pressed.png",
                                        this,
                                        menu_selector(GameScene::OnCompleteMission));
    pauseItem->setAnchorPoint(ccp(1,0));
    pauseItem->setOpacity(20);
    
    pauseMenu = CCMenu::create(pauseItem, NULL);
    this->addChild(pauseMenu, kHUD_Z_Order-1);
    pauseMenu->setPosition(50,visibleSize.height-50);
    pauseMenu->setTag(60002);
    */
    //===============================================================
    
    CCMenuItemImage* fastModeItem = CCMenuItemImage::create(
                                                            "Interfeiss/in_game/fast_speed.png",
                                                            "Interfeiss/in_game/fast_forward_pressed.png",
                                                            this,
                                                            menu_selector(GameScene::menuFastModeCallback));
    fastModeItem->setAnchorPoint(ccp(0,0));
    fastModeItem->setTag(102);
    
    CCMenuItemImage* normalModeItem = CCMenuItemImage::create(
                                                              "Interfeiss/in_game/normal_speed.png",
                                                              "Interfeiss/in_game/normal_speed.png",
                                                              this,
                                                              menu_selector(GameScene::menuNormalModeCallback));
    normalModeItem->setTag(101);
    normalModeItem->setAnchorPoint(ccp(0,0));
    normalModeItem->setVisible(false);
    
    CCMenu* modeMenu = CCMenu::create(fastModeItem, normalModeItem, NULL);
    modeMenu->setTag(100);
    this->addChild(modeMenu, kHUD_Z_Order-1);
    modeMenu->setPosition(5, -100);
    
    //The star stuff
    std::stringstream theComboParticle;
    if(mCurrentMission.Task_type==1)//Dwarf need to collect for stars
    {
        theComboParticle << _mission_dwarfs_saved_counter <<"/"<<_mission_star_points_3;
    }
    else if(mCurrentMission.Task_type==0)//Points need to collect for stars
    {
        theComboParticle <<"0/"<<_mission_star_points_3;
    }
    else if(mCurrentMission.Task_type == 2)//Combo level reach for stars
    {
        theComboParticle <<"0/"<<_mission_star_points_3;
    }
    
    _pointsLabel->setString(theComboParticle.str().c_str());
    
    //-----------------------------------------------------------------
    //Create the ultra combo bar !!!
    
    CCSprite* aBaseProgress = CCSprite::create("small_dot_red.png");
    addChild(aBaseProgress);
    aBaseProgress->setPosition(ccp(94,_screenSize.height-86));
    aBaseProgress->setScaleX(0.25);
    aBaseProgress->setScaleY(0.6);
    
    CCSprite* aDummy = CCSprite::create("Interfeiss/challenges/daily/progress_days.png");
    aDummy->setPosition(ccp(-10,0));
    aBaseProgress->addChild(aDummy);
    
    int aOneStep = aDummy->getContentSize().width/4;
    
    _mission_progress_bar_1 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
    _mission_progress_bar_1->setScaleX(0.25);
    _mission_progress_bar_1->setAnchorPoint(ccp(0,0.5f));
    _mission_progress_bar_1->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6,aDummy->getPositionY()));
    aBaseProgress->addChild(_mission_progress_bar_1);
    
    _mission_progress_bar_2 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
    _mission_progress_bar_2->setAnchorPoint(ccp(0,0.5f));
    _mission_progress_bar_2->setScaleX(0.25);
    _mission_progress_bar_2->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep,aDummy->getPositionY()));
    aBaseProgress->addChild(_mission_progress_bar_2);
    
    _mission_progress_bar_3 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
    _mission_progress_bar_3->setAnchorPoint(ccp(0,0.5f));
    _mission_progress_bar_3->setScaleX(0.25);
    _mission_progress_bar_3->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep*2,aDummy->getPositionY()));
    aBaseProgress->addChild(_mission_progress_bar_3);
    
    _mission_progress_bar_4 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
    _mission_progress_bar_4->setAnchorPoint(ccp(0,0.5f));
    _mission_progress_bar_4->setScaleX(0.23);
    _mission_progress_bar_4->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep*3,aDummy->getPositionY()));
    aBaseProgress->addChild(_mission_progress_bar_4);
    
    
    
    CCSprite* aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
    aDummySpe->setPosition(ccp(-aOneStep,0));
    aBaseProgress->addChild(aDummySpe);
    
    aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
    aDummySpe->setPosition(ccp(0,0));
    aBaseProgress->addChild(aDummySpe);
    
    aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
    aDummySpe->setPosition(ccp(aOneStep,0));
    aBaseProgress->addChild(aDummySpe);
    
    
    //-----------------------------------------------------------------
    
    //Create the 3 stars
    _mission_star_1 = CCSprite::create("new_mission/zvaigzne_final.png");
    _mission_star_1->setPosition(ccp(-aOneStep,-45));
    _mission_star_1->setOpacity(80);
    _mission_star_1->setScaleX(2.2);
    _mission_star_1->setScaleY(1.0);
    aBaseProgress->addChild(_mission_star_1,kHUD_Z_Order+1);
    
    _mission_star_2 = CCSprite::create("new_mission/zvaigzne_final.png");
    _mission_star_2->setPosition(ccp(0,-45));
    _mission_star_2->setOpacity(80);
    _mission_star_2->setScaleX(2.2);
    _mission_star_2->setScaleY(1.0);
    aBaseProgress->addChild(_mission_star_2,kHUD_Z_Order+1);
    
    _mission_star_3 = CCSprite::create("new_mission/zvaigzne_final.png");
    _mission_star_3->setPosition(ccp(aOneStep,-45));
    _mission_star_3->setOpacity(80);
    _mission_star_3->setScaleX(2.2);
    _mission_star_3->setScaleY(1.0);
    aBaseProgress->addChild(_mission_star_3,kHUD_Z_Order+1);
    
    
    mMaxBarPoints = _mission_star_points_3;
    
    UpdateMissionStars();
}

bool GameScene::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    //How much bullets does have maseter troll !!!
    mMasterTroll_Bullets = 10;
    
    //Get settings from mission !!!
    mCurrentMission = User::getInstance()->getMissionManager().GetActiveMission();
    
    //Check what map need to create !!!
    CreateGameByMission();
    
    return true;
    
    //All the old crap and stuff that is not needed anymore !!! [maybe something, then add it to the new stuff]
    //========================================================================
    
    //Check if need to create new random maps !!!
    if(User::getInstance()->mNewMaps){
        CreateRandomMap();
    }
    else{
        mNewMapRandomEnabled = false;
    }
//    return true;
    
    mZipZapActive = false;
    
    mCheckSpecialSpotTouch = false;
    _DwarfsCollectedObject = 0;
    _DwarfsEnteredCave = 0;
    
    mFlurry_GhostActivated = 0;
    mFlurry_CrystalPoints = 0;
    mFlurry_BlitzActivated = 0;
    mFlurry_StampActivated = 0;
    mFlurry_MachinesActivated = 0;
    
    mCurrentSmallTutorial = -1;
    
    _mission_dwarfs_saved_counter = 0;
    _mission_count_dwarfs = false;
    
    mFlurry_MachinesActivated = 0;
    
    mNewSplitCaves = User::getInstance()->mNewSplitCaves;
    
    mTrollTimer = 0;
    
    //Fmod tests !!!
    
    mFmodEnabled = false;
    
    mTotalPointsInGame = 0;
    _SpawnSpecialTrolls = 0;
    
    /*
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(mFmodEnabled)
    {
        unsigned int      version;
        void             *extradriverdata = 0;
        
        result = FMOD::System_Create(&system);
        ERRCHECK(result);
        
        result = system->getVersion(&version);
        ERRCHECK(result);
        
        if (version < FMOD_VERSION)
        {
            CCLog("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
        }
        
        mMusic1_On = false;
        mMusic2_On = false;
        mMusic3_On = false;
        
        mMusic3_Done = false;
        mMusic2_Done = false;
        mMusic1_Done = false;
        
        result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
        ERRCHECK(result);
        
        result = system->createSound(Common_MediaPath("music/music_1s.wav"), FMOD_LOOP_NORMAL, 0, &sound1);
        ERRCHECK(result);
        
        result = system->createSound(Common_MediaPath("music/music_2s.wav"), FMOD_LOOP_NORMAL, 0, &sound2);
        ERRCHECK(result);
        
        result = system->createSound(Common_MediaPath("music/music_3s.wav"), FMOD_LOOP_NORMAL, 0, &sound3);
        ERRCHECK(result);
        
        result = system->createSound(Common_MediaPath("music/music_4s.wav"), FMOD_LOOP_NORMAL, 0, &sound4);
        ERRCHECK(result);
        
        result = system->playSound(sound1, 0, false, &channel);
        ERRCHECK(result);
        
        result = system->playSound(sound2, 0, false, &channel1);
        channel1->setVolume(0);
        ERRCHECK(result);
        
        result = system->playSound(sound3, 0, false, &channel2);
        channel2->setVolume(0);
        ERRCHECK(result);
        
        result = system->playSound(sound4, 0, false, &channel3);
        channel3->setVolume(0);
        ERRCHECK(result);
        
        system->update();
    }
#endif
    */
    
    _mission_dwarfs_spawned = 0;
    _mission_dwarfs_max = 0;
    
    //---------------------------
    
    mFadeOutMusic = false;
    mFadeInMusic = false;
    mBackMusicVolume = 1.0f;
    
    mSpeciaCrystallDwarfID = -1;
    
    mTotalCombo = 1;
    
    mDebugComboValue = 1;
    
    //For now disabled
    mPaternFormulas = false;
    
    _mission_dwarfs_removed = 0;
    
    mTimeNotCollectedCrystal = 0.0f;
    mTimeToCheckAgainNoCrystal = 0.0f;
    
    mCurretnEffectDistance = 0.0f;
    mNoDwarfEneterCave = 0.0f;
    
    _rainTime = 0;
    _whenToRain = 0;
    rainTimeSet = false;
    
    _windTime = 0;
    _whenToWind = (rand()%45)+45;
    
    _windLastSpawnBlockID = -1;
    
    mRainActive = false;
    mTornadoActive = false;
    
    mMapDangerType = 1;//Basic danger only
    mMapDangerTimer = 0;
    mMapDangerTimerReq = (rand()%45)+45;
    
    mMapDangerCooldown = 0;
    
    mIntroPlaying = false;
    
    //Particle test :)
//    CCParticleSystemQuad* p = CCParticleSystemQuad::create("whatever_particle.plist");
//    this->addChild(p);
    
//    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    mCurrentSmallTutorial = -1;
    
    mCombo_DwarfEnter = 1;
    mCombo_CollectCrystals = 1;
    mComboTimer_CollectCrystal = 0;
    mComboTimer_DwarfEnter = 0;
    
//    PlaySpecialMusic(0);
    
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true) == false)
    {
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
    else
    {
        PlaySpecialMusic(0);
    }
    
    mSplitCavesMode = false;
    
    mScoreMulti_Global = User::getInstance()->getScore_Multi();
//    mScoreMulti_Global = 2;
    
    mShowTutorialCompleted = -1;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    //Lets start tutorial
    mCurrentMusicID = -1;
    
    if(User::getInstance()->_tutorial==0)
        mTutorialEnabled = true;
    else
        mTutorialEnabled = false;
    
    //for now disabled always
//    mTutorialEnabled = true;
    
    if(mTutorialEnabled)
    {
        //Check diamonds count - todo
        if(User::getInstance()->getDiamonds()==26)
            User::getInstance()->setDiamonds(25);
    }
    else
    {
        //Count how many times have played !!!
        User::getInstance()->addTimesPlayed();
    }
    
    _blockedGeneratePoints.clear();
    
    _generatedExtraStartDwarf = false;
    
//    mTutorialEnabled = true;
    mTutorialStep = 0;//should be 0
    
    mTutorialFlag_1 = false;
    mTutorialFlag_2 = false;
    
    mTutorialTimer = -1;
    mTutorialTimerSet = false;
    
    mTutorialPointsAdded = false;
    mTutorialDrawHand = false;
    
//    _debugSprite1 = CCSprite::create("small_dot_red.png");
//    addChild(_debugSprite1,kHUD_Z_Order);
//    _debugSprite2 = CCSprite::create("small_dot_red.png");
//    addChild(_debugSprite2,kHUD_Z_Order);
    
    
    _tutorialLabel = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(700,240), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	_tutorialLabel->setString("Welcome to DF");
    _tutorialLabel->setColor(ccc3(255,246,200));
    _tutorialLabel->setAnchorPoint(ccp(0.5f,0.5f));
	_tutorialLabel->setPosition(ccp(visibleSize.width/2,visibleSize.height-100));
    _tutorialLabel->setVisible(false);
    _tutorialLabel->setTag(kNoPause);
	this->addChild(_tutorialLabel, kHUD_Z_Order+1);
    
    _tutorialLabel2 = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.42, CCSize(700,240), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	_tutorialLabel2->setString("Welcome to DF");
    _tutorialLabel2->setColor(ccc3(255,246,200));
    _tutorialLabel2->setAnchorPoint(ccp(0.5f,0.5f));
	_tutorialLabel2->setPosition(ccp(visibleSize.width/2,visibleSize.height-140));
    _tutorialLabel2->setVisible(false);
	this->addChild(_tutorialLabel2, kHUD_Z_Order+1);
    
    //-------------------------------
    
    _DwarfsSpawned = 0;
    
    //For 1st dwarf too
    mSpecialCrystalSpawnTimer = 4;
    mCanSpawnExtraCrystal = false;
    
    mCrystalPoints = 0;
    
    _plantIceFlowerFirst = false;
    _plantSunFlowerFirst = false;
    _plantFuzzFlowerFirst = false;
    
    _gameTime = 0;
    _actionTrollSpawnTime = -1;
    
    _currentMinSpwanedTime = 0;//For the first meteorite - some random value above 20sec?
    _currentMinForSpawn = -1;
    _effectActiveSpawnTime = rand() % 15 + 40;//For the first meteorite - some random value above 20sec?
    
    _crystalCurrentSpawnMinute = -1;
    _crystalCurrentActiveTime = rand()%10+30;
    _crystalCurrentTime = 0;
    
    _trollCurrentSpawnMinute = -1;
    _trollCurrentActiveTime = rand()%10+20;
    _trollCurrentTime = 0;
    
    _dwarfCurrentTime = 0;
    _dwarfCurrentSpawnMinute = -1;
    _dwarfCurrentActiveTime = 0;
    
    mTrollFreezeDwarfTime = 0;
    mTrollFreeze = 0;
    
    _dwarfOnMap = NULL;
    
    if(User::getInstance()->mSpecial_17_Mission){
        mTrollFreezeDwarfTime = (rand()%10)+10;
    }
    
    _plantCrystalTime = 0;
    _plantFuzzBall = 0;
    
    if(!mPaternFormulas)
    {
        _dwarfCurrentSpawnMinute = 0;
        _trollCurrentSpawnMinute = 0;
        _crystalCurrentSpawnMinute = 0;
        _currentMinForSpawn = 0;
    }
    
    setTag(11);//The game tag !!! ???
    
    _lastSavePrice = 1;//Start with 2 crystals
    
    _effectLastSpawnBlockID = -1;
    
    //Some flags
    _boosterMushroom = false;//If true - will allow extra booster to enable
    _totalActiveBoosters = 0;
    
    _diamondTimer = 0;
    
    _willUseMushroom = false;
    
//    this->setTouchEnabled(true);
    
	
	
	//preload images
	//CCTextureCache::sharedTextureCache()->addImage("map_960_720.png");
    
    if(mNewMapRandomEnabled){
        //All points already set at random generator !!!
    }
    else{
        //bottom
        _genearetPoints.push_back((GeneratePoint){252, 719 - 0, 4.7});//M_PI / 2
        _genearetPoints.push_back((GeneratePoint){756, 719 - 0, 4.7});//M_PI / 2
        
        //right
        _genearetPoints.push_back((GeneratePoint){959, 719 - 240, M_PI});//240
        _genearetPoints.push_back((GeneratePoint){959, 719 - 580, M_PI});//580  540
        
        //top
        _genearetPoints.push_back((GeneratePoint){756, 719 - 719, 1.65});//3.0 * M_PI / 2
        _genearetPoints.push_back((GeneratePoint){252, 719 - 719, 1.65});//3.0 * M_PI / 2
        
        //left
        _genearetPoints.push_back((GeneratePoint){0, 719 - 540, 0});
        _genearetPoints.push_back((GeneratePoint){0, 719 - 240, 0});
    }
    
    _bullets = CCArray::create();
	_bullets->retain();
    
	_dwarves = CCArray::create();
	_dwarves->retain();
    
    _goblins = CCArray::create();
	_goblins->retain();
    
    _hidras = CCArray::create();
    _hidras->retain();
    
    _spiders = CCArray::create();
    _spiders->retain();
    
    _flyObjects = CCArray::create();
    _flyObjects->retain();
	
	_trolls = CCArray::create();
	_trolls->retain();
	
	_crystals = CCArray::create();
	_crystals->retain();
	
	_effects = CCArray::create();
	_effects->retain();
    
    _tornadoActive = CCArray::create();
    _tornadoActive->retain();
	
	_diamonds = CCArray::create();
	_diamonds->retain();
	
	_introAnimations = CCArray::create();
	_introAnimations->retain();
    
    _lastSpawnPoint = -1;
    
    _screenSize = visibleSize;
    _origin = origin;
    _totalStepsZ = _screenSize.height/2/10;
	
	_scoreLabel = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(115, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
	_scoreLabel->setString("0");
    _scoreLabel->setColor(ccc3(255,246,200));
//    _scoreLabel->setAnchorPoint(ccp(0,1));
//	_scoreLabel->setPosition(ccp(origin.x+10,
//							   origin.y + visibleSize.height - 24));
//	this->addChild(_scoreLabel, kHUD_Z_Order+1);
    
    _diamondsLabel = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(90, 55), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
	_diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
    _diamondsLabel->setColor(ccc3(255,246,200));
//    _diamondsLabel->setAnchorPoint(ccp(0,1));
//	_diamondsLabel->setPosition(ccp(visibleSize.width - 110,
//                                 origin.y + visibleSize.height - 28));//27
//	this->addChild(_diamondsLabel, kHUD_Z_Order+1);
    
    _pointsLabel = CCLabelTTF::create("Game", FONT_SKRANJI, TITLE_FONT_SIZE*0.5, CCSize(120, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	_pointsLabel->setString("0");
    _pointsLabel->setColor(ccc3(255,246,200));
//    _pointsLabel->setAnchorPoint(ccp(0.5,0.5));
//	_pointsLabel->setPosition(ccp(visibleSize.width/2,visibleSize.height - 29));//27
//	this->addChild(_pointsLabel, kHUD_Z_Order+1);
    
    //-----------------------------------
    //Some debug stuff !!!
    
    mDebugInfoVisible = false;
    
    mDebugFormulas = false;
    
    if(mDebugFormulas)
    {
        CCLabelTTF* aDebugLabel_1 = CCLabelTTF::create("Game1", "fonts/skranji_regular.ttf", TITLE_FONT_SIZE*0.3, CCSize(300, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
        aDebugLabel_1->setString("Dwarf spawn times");
        aDebugLabel_1->setTag(20001);
        aDebugLabel_1->setAnchorPoint(ccp(0,1));
        aDebugLabel_1->setPosition(ccp((visibleSize.width-200)/2,140));
        this->addChild(aDebugLabel_1,kHUD_Z_Order+1);
        
        CCLabelTTF* aDebugLabel_2 = CCLabelTTF::create("Game2", "fonts/skranji_regular.ttf", TITLE_FONT_SIZE*0.3, CCSize(300, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
        aDebugLabel_2->setString("Troll spawn times");
        aDebugLabel_2->setTag(20002);
        aDebugLabel_2->setAnchorPoint(ccp(0,1));
        aDebugLabel_2->setPosition(ccp((visibleSize.width-200)/2,120));
        this->addChild(aDebugLabel_2,kHUD_Z_Order+1);
        
        CCLabelTTF* aDebugLabel_3 = CCLabelTTF::create("Game3", "fonts/skranji_regular.ttf", TITLE_FONT_SIZE*0.3, CCSize(300, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
        aDebugLabel_3->setString("Meteorite spawn times");
        aDebugLabel_3->setTag(20003);
        aDebugLabel_3->setAnchorPoint(ccp(0,1));
        aDebugLabel_3->setPosition(ccp((visibleSize.width-200)/2,100));
        this->addChild(aDebugLabel_3,kHUD_Z_Order+1);
        
        CCLabelTTF* aDebugLabel_4 = CCLabelTTF::create("Game4", "fonts/skranji_regular.ttf", TITLE_FONT_SIZE*0.3, CCSize(300, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
        aDebugLabel_4->setString("Crystal spawn times");
        aDebugLabel_4->setTag(20004);
        aDebugLabel_4->setAnchorPoint(ccp(0,1));
        aDebugLabel_4->setPosition(ccp((visibleSize.width-200)/2,80));
        this->addChild(aDebugLabel_4,kHUD_Z_Order+1);
        
        CCLabelTTF* aDebugLabel_5 = CCLabelTTF::create("Game5", "fonts/skranji_regular.ttf", TITLE_FONT_SIZE*0.3, CCSize(300, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
        aDebugLabel_5->setString("Time in level: 00:00");
        aDebugLabel_5->setTag(20005);
        aDebugLabel_5->setAnchorPoint(ccp(0,1));
        aDebugLabel_5->setPosition(ccp((visibleSize.width-200)/2,160));
        this->addChild(aDebugLabel_5,kHUD_Z_Order+1);
    }
    
    /*
    CCLabelTTF* aDebugLabel_1 = CCLabelTTF::create("Game1", FONT_SKRANJI, TITLE_FONT_SIZE*0.4, CCSize(200, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	aDebugLabel_1->setString("Dwarf Points:");
    aDebugLabel_1->setTag(10001);
    aDebugLabel_1->setAnchorPoint(ccp(0,1));
    aDebugLabel_1->setPosition(ccp((visibleSize.width-200)/2,150));
    this->addChild(aDebugLabel_1,kHUD_Z_Order+1);
    
    CCLabelTTF* aDebugLabel_2 = CCLabelTTF::create("Game2", FONT_SKRANJI, TITLE_FONT_SIZE*0.4, CCSize(200, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	aDebugLabel_2->setString("0");
    aDebugLabel_2->setTag(10002);
    aDebugLabel_2->setAnchorPoint(ccp(0,1));
    aDebugLabel_2->setPosition(ccp((visibleSize.width-200)/2,120));
    this->addChild(aDebugLabel_2,kHUD_Z_Order+1);
    
    CCLabelTTF* aDebugLabel_3 = CCLabelTTF::create("Game3", FONT_SKRANJI, TITLE_FONT_SIZE*0.4, CCSize(200, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	aDebugLabel_3->setString("Other Points:");
    aDebugLabel_3->setTag(10003);
    aDebugLabel_3->setAnchorPoint(ccp(0,1));
    aDebugLabel_3->setPosition(ccp((visibleSize.width-200)/2,90));
    this->addChild(aDebugLabel_3,kHUD_Z_Order+1);
    
    CCLabelTTF* aDebugLabel_4 = CCLabelTTF::create("Game4", FONT_SKRANJI, TITLE_FONT_SIZE*0.4, CCSize(200, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	aDebugLabel_4->setString("0");
    aDebugLabel_4->setTag(10004);
    aDebugLabel_4->setAnchorPoint(ccp(0,1));
    aDebugLabel_4->setPosition(ccp((visibleSize.width-200)/2,60));
    this->addChild(aDebugLabel_4,kHUD_Z_Order+1);
    
    CCLabelTTF* aDebugLabel_5 = CCLabelTTF::create("Game5", FONT_SKRANJI, TITLE_FONT_SIZE*0.3, CCSize(200, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	aDebugLabel_5->setString("1");
    aDebugLabel_5->setTag(10005);
    aDebugLabel_5->setAnchorPoint(ccp(0,1));
    aDebugLabel_5->setPosition(ccp((visibleSize.width-300)/2,50));
    this->addChild(aDebugLabel_5,kHUD_Z_Order+1);
    
    CCLabelTTF* aDebugLabel_6 = CCLabelTTF::create("Game6", FONT_SKRANJI, TITLE_FONT_SIZE*0.3, CCSize(200, 55), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
	aDebugLabel_6->setString("1");
    aDebugLabel_6->setTag(10006);
    aDebugLabel_6->setAnchorPoint(ccp(0,1));
    aDebugLabel_6->setPosition(ccp((visibleSize.width-100)/2,50));
    this->addChild(aDebugLabel_6,kHUD_Z_Order+1);
    */
    
    
    //-----------------------------------
//	window.rootViewController
    
    if(User::getInstance()->mNewMissionBuild)
    {
        if(User::getInstance()->mSpecial_17_Mission){
            CreateStartMap(kMap_Winter);
        }
        else
        {
            //        User::getInstance()->mNewMissionProgress = 5;
            //Check what map do we need
            if(User::getInstance()->mNewMissionProgress == 1){
                CreateStartMap(kMap_Summer);
            }
            else if(User::getInstance()->mNewMissionProgress == 2){
                CreateStartMap(kMap_Autumn);
            }
            else if(User::getInstance()->mNewMissionProgress == 3){
                CreateStartMap(kMap_Winter);
            }
            else if(User::getInstance()->mNewMissionProgress == 4){
                CreateStartMap(kMap_Spring);
            }
            else if(User::getInstance()->mNewMissionProgress == 5){
                CreateStartMap(kMap_Summer);
            }
            else{
                CreateStartMap(kMap_Summer);
            }
        }
    }
    else
    {
        CreateStartMap(1);
    }
    
	// background
//    CCSprite* level = CCSprite::create("large_map2.png");
//    level->setTag(332);
//    
//    CCSprite* levelGod = CCSprite::create("powerup/Sun_Flower/map_sunshine.png");
//    levelGod->setVisible(false);
//    levelGod->setOpacity(0);
//    levelGod->setTag(333);//The feature debug
//    levelGod->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//	
//    // position the sprite on the center of the screen
//    level->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//	this->addChild(level, 0);
//    this->addChild(levelGod,0);
    // NOW WE WILL USE DYNAMIC MAP !!!
	
//	_cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_TALL, ccp(545.0f, 719 - 400.0f)));
//	_cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_FAT, ccp(440.0f, 719 - 400.0f)));
    
    //Where are the cave points
    CCSprite* aDebug1 = CCSprite::create("cave/cave_new.png");
    aDebug1->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y-10));
    aDebug1->setOpacity(128);
//    addChild(aDebug1, 1000);
//
//    CCSprite* aDebug2 = CCSprite::create("DebugDot.png");
//    aDebug2->setPosition(ccp(440,319));
//    addChild(aDebug2, 1000);
    
	_caveMask = new CCImage();
//	_caveMask->initWithImageFile("cave/cave_mask.png");
    _caveMask->initWithImageFile("cave/DwarfCaveMask_85.png");
	_caveMask->retain();
    
    _caveMaskFat = new CCImage();
	_caveMaskFat->initWithImageFile("cave/CaveFatMask.png");
	_caveMaskFat->retain();
    
    _caveMaskTall = new CCImage();
	_caveMaskTall->initWithImageFile("cave/CaveTallMask.png");
	_caveMaskTall->retain();
    
    //For each level own mask
    _objectMask = new CCImage();
    _objectMask->initWithImageFile("MapSpawnObjectMask.png");
    _objectMask->retain();
    
    if(mNewSplitCaves)
    {
        _trollSensor = new CCImage();
        _trollSensor->initWithImageFile("troll_sensor_vertical.png");
        _trollSensor->retain();
    }
    else
    {
        _trollSensor = new CCImage();
        _trollSensor->initWithImageFile("troll_sensor.png");
        _trollSensor->retain();
    }
	
	_mask = new CCImage();
//    _mask->initWithImageFile("mask_new_15_12_2013.png");
    if(mNewMapRandomEnabled){
        _mask->initWithImageFile("cave_mask_custom.png");
    }
    else{
        _mask->initWithImageFile("kartes_maska.png");
    }
    
//	_mask->initWithImageFile("kartes_maska.png");
//    _mask->initWithImageFile("kartes_maska_Split.png");
	_mask->retain();
    
    _tutMask_1 = new CCImage();
    _tutMask_1->initWithImageFile("Tutorial_CP_1.png");
    _tutMask_1->retain();
    
    _tutMask_2 = new CCImage();
    _tutMask_2->initWithImageFile("Tutorial_CP_2.png");
    _tutMask_2->retain();
    
    _tutMask_3 = new CCImage();
    _tutMask_3->initWithImageFile("Tutorial_CP_3.png");
    _tutMask_3->retain();
    
    //DEbug
//    CCSprite* aDebugMask_1 = CCSprite::create("Tutorial_CP_3.png");
//    aDebugMask_1->setPosition(ccp(designResolutionSize.width / 2 + _tutMask_1->getWidth()/2+200,
//                                  designResolutionSize.height / 2 + _tutMask_1->getHeight()/2 - 160));
//    addChild(aDebugMask_1,10000);
    
//    int cavePosX = posX - designResolutionSize.width / 2 + _tutMask_1->getWidth() / 2 - 100;
//    int cavePosY = posY - designResolutionSize.height / 2 + _tutMask_1->getHeight() / 2 - 100;
    
	
//	_cave = CCSprite::create("cave/cave.png");
//    _cave = CCSprite::create("cave/cave_new.png");
    
    
    _cave = CCSprite::create("Interfeiss/main_menu/new/cave_big.png");
    _cave->setScale(0.40f);
    if(mNewMapRandomEnabled){
        //Take the cords
        _cave->setPosition(ccp(mCaveFinalCords.x, mCaveFinalCords.y));
    }
    else{
        _cave->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y-10));
    }
//    _cave->setScale(0.85);
    if(mSplitCavesMode || mNewSplitCaves)
        _cave->setVisible(false);
	addChild(_cave,getSpriteOrderZ(_cave->getPositionY()));// mTotalStepsZ-floorf(_cave->getPositionY()/42));
    
    //The split caves
    if(mNewSplitCaves)
    {
        // The tall orange dwarf !!!
        _caveTall = CCSprite::create("cave/trapdoor-orange.png");
        _caveTall->setPosition(ccp((_screenSize.width/4-_caveTall->getContentSize().width/2)+90,
                                   (_screenSize.height/4-_caveTall->getContentSize().height/2)+50));
        addChild(_caveTall,0);
        
        _caveFat = CCSprite::create("cave/trapdoor-blue.png");
        _caveFat->setPosition(ccp(_screenSize.width - ((_screenSize.width/4-_caveFat->getContentSize().width/2))-60,
                                  (_screenSize.height - (_screenSize.height/4-_caveFat->getContentSize().height/2))-90));
        addChild(_caveFat,0);
    }
    else
    {
        _caveTall = CCSprite::create("cave/CaveTall.png");
        _caveTall->setPosition(ccp(origin.x+_caveTall->getContentSize().width/2, origin.y-10+_caveTall->getContentSize().height/2));
        if(!mSplitCavesMode)
            _caveTall->setVisible(false);
        _caveTall->setScale(0.85);
        addChild(_caveTall,getSpriteOrderZ(_caveTall->getPositionY()));
        
        _caveFat = CCSprite::create("cave/CaveFat.png");
        _caveFat->setPosition(ccp(visibleSize.width - origin.x-_caveTall->getContentSize().width/2, origin.y-10+_caveTall->getContentSize().height/2));
        if(!mSplitCavesMode)
            _caveFat->setVisible(false);
        _caveFat->setScale(0.85);
        addChild(_caveFat,getSpriteOrderZ(_caveTall->getPositionY()));
    }
    
    //The split stuff
//	_cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_TALL, ccp(_caveTall->getContentSize().width/2, _caveTall->getContentSize().height/2)));
//	_cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_FAT, ccp(_screenSize.width-_caveFat->getContentSize().width/2, _caveFat->getContentSize().height/2)));
    
    //Add the debug points with width !!!
    cocos2d::CCSprite* _caveFat_debug_Dot;
    _caveFat_debug_Dot = CCSprite::create("small_dot_red.png");
    _caveFat_debug_Dot->setPosition(ccp(440.0f, 719 - 400.0f));
//    addChild(_caveFat_debug_Dot,10000);
    
    cocos2d::CCSprite* _caveThin_debug_Dot;
    _caveThin_debug_Dot = CCSprite::create("small_dot_red.png");
    _caveThin_debug_Dot->setPosition(ccp(545.0f, 730 - 400.0f));
//    addChild(_caveThin_debug_Dot,10000);
    
    cocos2d::CCSprite* _caveThin_debug_Radius;
    _caveThin_debug_Radius = CCSprite::create("small_dot_blue.png");
    _caveThin_debug_Radius->setPosition(ccp(545.0f, 719 - 400.0f));
//    addChild(_caveThin_debug_Radius,10000);
    
//    CCSprite* aDebugMask1 = CCSprite::create("cave/cave_mask.png");
//	aDebugMask1->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//    aDebugMask1->setScale(0.85);
//    this->addChild(aDebugMask1,GetSpriteOrderZ(aDebugMask1->getPositionY()));
    
    //Cave lights
    _caveDoorBig = CCSprite::create("cave/fat_cave_light.png");
    _caveDoorBig->setPosition(ccp(47,35));
    _caveDoorBig->setOpacity(0);
    if(!mSplitCavesMode)
    {
        if(mNewMapRandomEnabled){
            _caveDoorBig->setPosition(ccp(mCaveFinalCords.x-70,mCaveFinalCords.y-45));
        }
        else{
            _caveDoorBig->setPosition(ccp(visibleSize.width/2-70,visibleSize.height/2-45));
        }
        
        addChild(_caveDoorBig,getSpriteOrderZ(_cave->getPositionY()));
    }
    else
    {
        _caveFat->addChild(_caveDoorBig);
    }
    
    _caveDoorSmall = CCSprite::create("cave/tall_cave_light.png");
    _caveDoorSmall->setOpacity(0);
    if(!mSplitCavesMode)
    {
        if(mNewMapRandomEnabled){
            _caveDoorSmall->setPosition(ccp(mCaveFinalCords.x+100,mCaveFinalCords.y-38));
        }
        else{
            _caveDoorSmall->setPosition(ccp(visibleSize.width/2+100,visibleSize.height/2-38));
        }
        addChild(_caveDoorSmall,getSpriteOrderZ(_cave->getPositionY()));
    }
    else
    {
        _caveDoorSmall->setPosition(ccp(100,40));
        _caveTall->addChild(_caveDoorSmall);
    }
    
    if(mNewSplitCaves)
    {
        _cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_TALL, ccp(_caveTall->getPositionX(), _caveTall->getPositionY())));
        _cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_FAT, ccp(_caveFat->getPositionX(), _caveFat->getPositionY())));
    }
    else
    {
        //use the door stuff !!!
        if(!mSplitCavesMode)
        {
            if(mNewMapRandomEnabled){
                _cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_TALL,
                                                                 ccp(_caveDoorSmall->getPositionX()-40,
                                                                     _caveDoorSmall->getPositionY()+20)));
                _cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_FAT,
                                                                 ccp(_caveDoorBig->getPositionX()+40,
                                                                     _caveDoorBig->getPositionY()+20)));
            }
            else{
                _cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_TALL, ccp(545.0f, 730 - 400.0f)));
                _cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_FAT, ccp(440.0f, 719 - 400.0f)));
            }
        }
        else
        {
            _cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_TALL,
                                                             ccp(_caveTall->getContentSize().width/2, _caveTall->getContentSize().height/2)));
            _cavePoints.insert(std::pair<DwarfType, CCPoint>(DWARF_TYPE_FAT,
                                                             ccp(_screenSize.width-_caveFat->getContentSize().width/2, _caveFat->getContentSize().height/2)));
        }
    }
    
	//play startup sound
    playInGameSound("button_start_game");
    
    //------------------------------------------------------------------------
    //Game crystals score
    
//    CCSprite* pointsBack = CCSprite::create("Interfeiss/in_game/point_count.png");
    CCSprite* pointsBack = CCSprite::create("Interfeiss/in_game/crystal_count.png");
    pointsBack->setAnchorPoint(ccp(0.5, 1));
//    pointsBack->setPositionY(visibleSize.height-25);
    pointsBack->setPosition(ccp(visibleSize.width+pointsBack->getContentSize().width,visibleSize.height-10));
    //cocos2d::CCNode::setPosition((visibleSize.width-pointsBack->getContentSize().width/2), visibleSize.height-10);
    addChild(pointsBack, kHUD_Z_Order-1);
    
    _scoreLabel->setAnchorPoint(ccp(0.5,0.5));
	_scoreLabel->setPosition(ccp(pointsBack->getContentSize().width/2-40,pointsBack->getContentSize().height/2+1));
	pointsBack->addChild(_scoreLabel);
    pointsBack->setTag(90005);
    
//    CCMoveTo* aActionMove1 = CCMoveTo::create(0.5f,ccp(visibleSize.width-pointsBack->getContentSize().width/2,visibleSize.height-10));
//    CCEaseSineOut* aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
//    pointsBack->runAction(aEaseActionMove1);
    
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //Game diamonds score
    
    CCSprite* diamondsBack = CCSprite::create("Interfeiss/in_game/diamond_count.png");
    diamondsBack->setAnchorPoint(ccp(0.5, 1));
    diamondsBack->cocos2d::CCNode::setPosition(ccp(visibleSize.width+diamondsBack->getContentSize().width+21, visibleSize.height-55));
    addChild(diamondsBack, kHUD_Z_Order-1);
    
    _diamondsLabel->setAnchorPoint(ccp(0.5,0.5));
	_diamondsLabel->setPosition(ccp(diamondsBack->getContentSize().width/2-45,diamondsBack->getContentSize().height/2+2));//27
	diamondsBack->addChild(_diamondsLabel);
    diamondsBack->setTag(90004);
    
//    aActionMove1 = CCMoveTo::create(0.5f,ccp((visibleSize.width-diamondsBack->getContentSize().width/2)+21, visibleSize.height-55));
//    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
//    diamondsBack->runAction(aEaseActionMove1);
    
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //Game points score
    
    CCSprite* pointsTotalBack = CCSprite::create("Interfeiss/in_game/score.png");
    pointsTotalBack->setAnchorPoint(ccp(0, 0.5));
//    pointsTotalBack->cocos2d::CCNode::setPosition(visibleSize.width/2, visibleSize.height-40);
    pointsTotalBack->cocos2d::CCNode::setPosition(-pointsTotalBack->getContentSize().width, visibleSize.height-54);
    addChild(pointsTotalBack, kHUD_Z_Order-1);
    
    _pointsLabel->setAnchorPoint(ccp(0.5,0.5));
    
//    if(User::getInstance()->mNewMissionBuild){
//        _pointsLabel->setPosition(ccp(_screenSize.width/2,_screenSize.height/2+50));//27
//        
//        addChild(_pointsLabel,kHUD_Z_Order+1);
//    }
//    else{
//        _pointsLabel->setPosition(ccp(pointsTotalBack->getContentSize().width/2-8,pointsTotalBack->getContentSize().height/2+9));//27
//        pointsTotalBack->addChild(_pointsLabel);
//    }
    
    _pointsLabel->setPosition(ccp(pointsTotalBack->getContentSize().width/2-8,pointsTotalBack->getContentSize().height/2+9));//27
    pointsTotalBack->addChild(_pointsLabel);
    
//	_pointsLabel->setPosition(ccp(pointsTotalBack->getContentSize().width/2-8,pointsTotalBack->getContentSize().height/2+9));//27
//	pointsTotalBack->addChild(_pointsLabel);
    
    pointsTotalBack->setTag(90003);
//    aActionMove1 = CCMoveTo::create(0.5f,ccp(0, visibleSize.height-54));
//    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
//    pointsTotalBack->runAction(aEaseActionMove1);
    
    //------------------------------------------------------------------------
    
    CCMenuItemImage* crystallEyeItem = CCMenuItemImage::create(
                                                           "Interfeiss/in_game/powerup_crystal_eye.png",
                                                           "Interfeiss/in_game/powerup_crystal_eye.png",
                                                           this,
                                                           menu_selector(GameScene::menuPowerupEyeCallback));
    //Check if has enough money !!!
    if (User::getInstance()->getDiamonds()<BOOSTER_1_PRICE)
        crystallEyeItem->setOpacity(120);//When inactive (update when not enough money !!!)
    crystallEyeItem->setTag(kBooster_FutureSee);
    
    CCMenuItemImage* extraPointsItem = CCMenuItemImage::create(
                                                               "Interfeiss/in_game/powerup_extra_points.png",
                                                               "Interfeiss/in_game/powerup_extra_points.png",
                                                               this,
                                                               menu_selector(GameScene::menuPowerupPointsCallback));
    if (User::getInstance()->getDiamonds()<BOOSTER_2_PRICE)
        extraPointsItem->setOpacity(120);//When inactive
    extraPointsItem->setTag(kBooster_Crystals);
    
    CCMenuItemImage* noEnemiesItem = CCMenuItemImage::create(
                                                               "Interfeiss/in_game/powerup_no_enemies.png",
                                                               "Interfeiss/in_game/powerup_no_enemies.png",
                                                               this,
                                                               menu_selector(GameScene::menuPowerupNoEnemiesCallback));
    if (User::getInstance()->getDiamonds()<BOOSTER_3_PRICE)
        noEnemiesItem->setOpacity(120);//When inactive
    noEnemiesItem->setTag(kBooster_NoEnemy);
    
    CCMenuItemImage* slowPlayItem = CCMenuItemImage::create(
                                                               "Interfeiss/in_game/powerup_slow_play.png",
                                                               "Interfeiss/in_game/powerup_slow_play.png",
                                                               this,
                                                               menu_selector(GameScene::menuPowerupSlowPlayCallback));
    if (User::getInstance()->getDiamonds()<BOOSTER_4_PRICE)
        slowPlayItem->setOpacity(120);//When inactive
    slowPlayItem->setTag(kBooster_Ghost);
    
    CCMenu* powerupMenu = CCMenu::create(crystallEyeItem, slowPlayItem, noEnemiesItem, extraPointsItem, NULL);
    powerupMenu->alignItemsHorizontallyWithPadding(10);
    this->addChild(powerupMenu, kHUD_Z_Order-1,kBoostMenu);
    powerupMenu->setPositionY(visibleSize.height-50);
    
    powerupMenu->setTag(kBoosters);
    powerupMenu->setVisible(false);//For now disabled
    
    CCMenuItemImage* pauseItem = CCMenuItemImage::create(
                                                               "Interfeiss/in_game/pause.png",
                                                               "Interfeiss/in_game/pause_pressed.png",
                                                               this,
                                                               menu_selector(GameScene::menuPauseCallback));
    pauseItem->setAnchorPoint(ccp(1,0));
    
    CCMenu* pauseMenu = CCMenu::create(pauseItem, NULL);
    this->addChild(pauseMenu, kHUD_Z_Order-1);
    if(!mSplitCavesMode)
        pauseMenu->setPosition(visibleSize.width - 5, -100);
    else
        pauseMenu->setPosition(visibleSize.width/2 + 140, 5);
    pauseMenu->setTag(90002);
    
    //Cheat for mission skip
    pauseItem = CCMenuItemImage::create(
                                                         "Interfeiss/in_game/pause.png",
                                                         "Interfeiss/in_game/pause_pressed.png",
                                                         this,
                                                         menu_selector(GameScene::OnCompleteMission));
    pauseItem->setAnchorPoint(ccp(1,0));
    pauseItem->setOpacity(20);
    
    pauseMenu = CCMenu::create(pauseItem, NULL);
    this->addChild(pauseMenu, kHUD_Z_Order-1);
    pauseMenu->setPosition(50,visibleSize.height-50);
    pauseMenu->setTag(60002);
    
//    aActionMove1 = CCMoveTo::create(0.5f,ccp(visibleSize.width - 5, 5));
//    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
//    pauseMenu->runAction(aEaseActionMove1);
    
    
    CCMenuItemImage* fastModeItem = CCMenuItemImage::create(
                                                         "Interfeiss/in_game/fast_speed.png",
                                                         "Interfeiss/in_game/fast_forward_pressed.png",
                                                         this,
                                                         menu_selector(GameScene::menuFastModeCallback));
    fastModeItem->setAnchorPoint(ccp(0,0));
    fastModeItem->setTag(102);
    
    CCMenuItemImage* normalModeItem = CCMenuItemImage::create(
                                                            "Interfeiss/in_game/normal_speed.png",
                                                            "Interfeiss/in_game/normal_speed.png",
                                                            this,
                                                            menu_selector(GameScene::menuNormalModeCallback));
    normalModeItem->setTag(101);
    normalModeItem->setAnchorPoint(ccp(0,0));
    normalModeItem->setVisible(false);
    
    
    CCMenu* modeMenu = CCMenu::create(fastModeItem, normalModeItem, NULL);
    modeMenu->setTag(100);
    
    if(User::getInstance()->mNewMissionBuild){
        if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission
           || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
           || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission){
            //Show the speed up
        }
        else{
            modeMenu->setVisible(false);
        }
    }
    
    this->addChild(modeMenu, kHUD_Z_Order-1);
    if(!mSplitCavesMode)
        modeMenu->setPosition(5, -100);
    else
        modeMenu->setPosition(visibleSize.width/2-60, 5);
//    modeMenu->setTag(90001);
    
//    aActionMove1 = CCMoveTo::create(0.5f,ccp(5, 5));
//    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
//    modeMenu->runAction(aEaseActionMove1);
    
    //Machine stuff !!!
    CCMenuItemImage* machineStamp = CCMenuItemImage::create("powerup/MachineClick.png", "powerup/MachineClick.png",this,menu_selector(GameScene::OnMachineStamp));
//    machineStamp->setPosition(ccp(100, 350));
    machineStamp->setPosition(ccp(_screenSize.width/2+42,_screenSize.height-100));
    machineStamp->setOpacity(0);
    
    CCMenuItemImage* machineGhost = CCMenuItemImage::create("powerup/MachineClick.png", "powerup/MachineClick.png",this,menu_selector(GameScene::OnMachineGhost));
//    machineGhost->setPosition(ccp(mScreenSize.width/2,mScreenSize.height-150));
    machineGhost->setPosition(ccp(95, 345));
    machineGhost->setOpacity(0);
    
    CCMenuItemImage* machineEnemies = CCMenuItemImage::create("powerup/MachineClick.png", "powerup/MachineClick.png",this,menu_selector(GameScene::OnMachineEnemies));
    machineEnemies->setPosition(ccp(_screenSize.width-95,320));
    machineEnemies->setOpacity(0);
    
    CCMenu* machineMenu = CCMenu::create(machineStamp,machineGhost,machineEnemies, NULL);
//    machineMenu->setTag(95);
    this->addChild(machineMenu, kHUD_Z_Order-1);
    machineMenu->setPosition(ccp(0,0));
    
    //debug
    aStartSpeedEnabled = User::getInstance()->getSpeedMode();
    
    aStartMin_1 =User::getInstance()->getSpeedStartMin_1();
    aStartMin_1_extra = User::getInstance()->getSpeedExtraMin_1_extra();
    
    aStartMin_2 =User::getInstance()->getSpeedStartMin_2();
    aStartMin_2_extra = User::getInstance()->getSpeedExtraMin_2_extra();
    
    aStartMin_3 =User::getInstance()->getSpeedStartMin_3();
    aStartMin_3_extra = User::getInstance()->getSpeedExtraMin_3_extra();
    
    _DailySpinVisible = false;
    
    //Some stuff
    _saveTroll = NULL;
    _saveDwarf2 = NULL;
    _saveDwarf1 = NULL;
    
    _gamePause = false;
    
    //The point multiplyer for dwarfs entering the cave
    _dwarfPointMulty = 1.0f;

    //For now disabled
//    _dwarfPointMulty = Levels::getInstance()->getModifier();
    //Check if mission 3 completed - then set x3 active !!!
//    MissionSet aCheck = User::getInstance()->getMissionManager().GetMissionByID(2);
//    if(aCheck.completed == 1)
//        _dwarfPointMulty = 2.0f;
    
    
    
    _dwarfTimer = 3;//We can spawn dwarf
    _gameSpeed = GAME_SPEED_NORMAL;//Normal // was 1.5
    _gameSlowTimer = 0.0f;
    
    _boostShieldTimer = 0.0f;
    _boostCrystalsTimer = 0.0f;
    _boostExtraPoints = 0;
    _boostGhostTimer = 0.0f;
    _boostNoEnemyTimer = 0.0f;
    _boostFutureSee = 0.0f;
    
    //Check if mission set 2 is completed
    if(User::getInstance()->getMissionManager().GetActiveMissionID()>1)
        _mushroomTimer = MUSHROOM_WAIT;
    else
        _mushroomTimer = -1;
    
    mTimeToChangeSeason = 60;
    
//    _gameSpeed = 10;
    
//    _dailyChallengeInfo = DailyChallengesInGameInfo::create(_stats);
//    _dailyChallengeInfo->setAnchorPoint(ccp(0.5,0));
//    _dailyChallengeInfo->setPosition(ccp((_screenSize.width - _dailyChallengeInfo->getContentSize().width) / 2 , 0));
//    addChild(_dailyChallengeInfo, 1000);
    
    //Crete spawn points
    CreatePossibleObjectSpawns();
    
	this->scheduleUpdate();
    
    CreateMissionStuff();
    CreateInGamePopup();
    
    _movePointsWind = CCPointArray::create(0);
	_movePointsWind->retain();
    
    float precision = .25f;
    float cir = 2 * M_PI;
    int mRadius = 280;
    for (float a = 0.1f; a < cir; a += precision)
    {
        float x = visibleSize.width/2 + mRadius * cos(a);
        float y = visibleSize.height/2 + mRadius/1.5f * sin(a);
        //        vertex.push_back(ccp(x,y));
        
        _movePointsWind->addControlPoint(ccp(x,y-50));
        
//        CCSprite* aDummyDot = CCSprite::create("small_dot_red.png");
//        aDummyDot->setPosition(ccp(x,y-50));
//        addChild(aDummyDot);
    }
    
    
    //The tutorial button - got it !!!
    //Debug buttons
    tutorial_button_gotit = CCMenuItemImage::create(
                                                              "Interfeiss/tutorial/got_it_btn0001.png",
                                                              "Interfeiss/tutorial/got_it_btn0002.png",
                                                              this,
                                                              menu_selector(GameScene::OnGotItButton));
    tutorial_button_gotit->setAnchorPoint(ccp(0.5,0.5));
    
    _tutorialButtons = CCMenu::create(tutorial_button_gotit,NULL);
    _tutorialButtons->setVisible(false);
    
//    tutorialButtons->alignItemsHorizontallyWithPadding(10);
    _tutorialButtons->setPosition(ccp(_screenSize.width/2,60));
    this->addChild(_tutorialButtons, kHUD_Z_Order+1);
    
    //Check if need to show the daily challange popup
    if(!mTutorialEnabled && Time::isToday(User::getInstance()->getDailyChallenges().getLastSpinTime()) == false)
    {
        //Add above the popup !!!
//        DailyChallengesPopUp::scene();
        _gamePause = true;
        
        _DailySpinVisible = true;
        
//        pauseSchedulerAndActionsRecursive(this,false);
        
        //Add some black bg
        CCLayerColor* aBlackBG = CCLayerColor::create(ccc4(0,0,0,64),visibleSize.width,visibleSize.height);
        aBlackBG->setTag(7777);
        aBlackBG->setOpacity(0);
        addChild(aBlackBG,kHUD_Z_Order+99);
        
        CCFadeTo* aFadeBG = CCFadeTo::create(0.5f,64);
        aBlackBG->runAction(aFadeBG);
        
        DailyChallengesPopUp* optionsLayer = DailyChallengesPopUp::create();
        optionsLayer->setAnchorPoint(ccp(0,0));
        optionsLayer->setPosition(ccp(0,visibleSize.height));
        addChild(optionsLayer,kHUD_Z_Order+100);
        
        CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(0,0));
        CCEaseElasticOut* aEase1 = CCEaseElasticOut::create(aMove1,0.5f);
        optionsLayer->runAction(aEase1);
    }
    else
    {
        CraeteHUD();
    }
    
    //------------------
    //Cave arrows
    _blueArrowAnim = TimedSpriteAnimation::create("cave/blue.plist", 0.1);
    _blueArrowAnim->retain();
    _blueArrowAnim->setTag(79);
//    _blueArrowAnim->setVisible(true);
    
    _blueArrowAnim->setPosition(ccp(visibleSize.width/2-80,visibleSize.height/2-80));
//    addChild(_blueArrowAnim,getSpriteOrderZ(_cave->getPositionY()));
    
    _orangeArrowAnim = TimedSpriteAnimation::create("cave/orange.plist", 0.1);
    _orangeArrowAnim->retain();
    _orangeArrowAnim->setTag(78);
//    _orangeArrowAnim->setVisible(false);
    
    _orangeArrowAnim->setPosition(ccp(visibleSize.width/2+110,visibleSize.height/2-80));
//    addChild(_orangeArrowAnim,getSpriteOrderZ(_cave->getPositionY()));
    
    if(mNewSplitCaves)
    {
        _blueArrowAnim->setPosition(ccp(_caveFat->getPositionX()-40,
                                        _caveFat->getPositionY()-50));
        _orangeArrowAnim->setPosition(ccp(_caveTall->getPositionX()+40,
                                          _caveTall->getPositionY()-50));
    }
    else if(mNewMapRandomEnabled){
        //Update the cords
        _blueArrowAnim->setPosition(ccp(_caveDoorSmall->getPositionX()-170,
                                        _caveDoorSmall->getPositionY()-30));
        _orangeArrowAnim->setPosition(ccp(_caveDoorBig->getPositionX()+170,
                                          _caveDoorBig->getPositionY()-30));
    }
    
    
    //The new caves
    if(mNewSplitCaves)
    {
        
    }
    
    //------------------
    
//    CreateDebugPanel()
    
    return true;
}

//----------------------------
//Some special 10 missio stuff

void GameScene::StartSpecialMission()
{
    //Hide all older popups? Don't think so !!!
    
    //Remove the pop - spawn trolls etc
    removeChildByTag(2014);
    
    _gamePause = false;
    resumeSchedulerAndActionsRecursive(this);
    
    _gameTime = 120;
    
    _DwarfsSpawned = 0;
    
    //Show some new hud !!!
    //Add the text field !!!
    std::stringstream value;
    if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission
       || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
       || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission){
        //Some special stuff here !!!
        CreateMasterTrollEnter();
        
        
    }
    else if(User::getInstance()->mSpecial_10_Mission){
        //Spawn trolls
        generateTrollMission(240,_screenSize.height/2,1.6,false,false,CCPointZero,CCPointZero,-1);
        generateTrollMission(_screenSize.width-240,_screenSize.height/2,4.7,false,false,CCPointZero,CCPointZero,-1);
        generateTrollMission(_screenSize.width/2,200,0,true,false,CCPointZero,CCPointZero,-1);
        
        value<<"0/20";
    }
    else if(User::getInstance()->mSpecial_11_Mission){
        //Spanw that magic crystal which need to be touched for action dwarf
        CCSprite* aCrystalYellow = CCSprite::create("Crystals/yellow_crystal.png");
        aCrystalYellow->setPosition(ccp(mMapBase->getContentSize().width-130,mMapBase->getContentSize().height-130));
        aCrystalYellow->setTag(20001);//The yellow cystal
        mMapBase->addChild(aCrystalYellow);
        
        CCSprite* aCrystalDummy = CCSprite::create("Crystals/yellow_under.png");
        aCrystalDummy->setPosition(ccp(aCrystalDummy->getContentSize().width/2-20,20));
        aCrystalYellow->addChild(aCrystalDummy);
        
        //And now add the blink for action
        aCrystalDummy = CCSprite::create("Crystals/yellow_glow.png");
        aCrystalDummy->setPosition(aCrystalYellow->getPosition());
        aCrystalDummy->setTag(20002);//This could blink fora action
        mMapBase->addChild(aCrystalDummy);
        
        CCFadeTo* aAction1 = CCFadeTo::create(0.5f,128);
        CCFadeTo* aAction2 = CCFadeTo::create(0.25f,255);
        
        CCSequence* aSeq1 = CCSequence::create(aAction1,aAction2,NULL);
        CCRepeatForever* aRep1 = CCRepeatForever::create(aSeq1);
        aCrystalDummy->runAction(aRep1);
        
        mCheckSpecialSpotTouch = true;
        mSpecialTouchSpot = ccp(mMapBase->getContentSize().width-130,mMapBase->getContentSize().height-130);
        
        
        value<<"0/15";
    }
    else if(User::getInstance()->mSpecial_12_Mission){
        value<<"0/25";
        
        //Hide the cave !!!
        _cave->setOpacity(0);
        
        _DwarfsEnteredCave = 0;
        
        generateTrollMission(_screenSize.width/2,200,0,true,false,CCPointZero,CCPointZero,-1);
        
        generateTrollMission(_screenSize.width/2,500,2,true,false,CCPointZero,CCPointZero,-1);
        
        //Change it !!!
        Machine_Enemies* aMachine3 = Machine_Enemies::create(this);
        aMachine3->setPosition(ccp(_screenSize.width/2,_screenSize.height/2));
        aMachine3->setTag(10030);
        addChild(aMachine3,1);
        
        mCheckSpecialSpotTouch = true;
        mSpecialTouchSpot = ccp(mMapBase->getContentSize().width/2,mMapBase->getContentSize().height/2);
    }
    else if(User::getInstance()->mSpecial_13_Mission){
        //Spawn trolls
        generateTrollMission(240,_screenSize.height/2-160,M_PI,false,false,CCPointZero,CCPointZero,-1);
        generateTrollMission(_screenSize.width-240,_screenSize.height/2+160,0,false,false,CCPointZero,CCPointZero,-1);
//        generateTrollMission(_screenSize.width/2,200,0,true);
        
        value<<"0/25";
    }
    else if(User::getInstance()->mSpecial_14_Mission){
        //Spawn trolls
        generateTrollMission(240,_screenSize.height/2-160,M_PI,false,false,CCPointZero,CCPointZero,-1);
        generateTrollMission(_screenSize.width-240,_screenSize.height/2+160,0,false,false,CCPointZero,CCPointZero,-1);
        //        generateTrollMission(_screenSize.width/2,200,0,true);
        
        value<<"0/32";
    }
    
    if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission
       || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
       || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission){
        
    }
    else{
        _DwarfCounter = CCLabelTTF::create(value.str().c_str(),
                                           "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.8,
                                           CCSize(400, 80), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        _DwarfCounter->setPosition(ccp(_screenSize.width/2,_screenSize.height-60));
        addChild(_DwarfCounter,kHUD_Z_Order+1);
    }
    
    //Set more dwarfs spawn !!!
}

void GameScene::SpecialMissionCompleted()
{
    _gamePause = true;
    pauseSchedulerAndActionsRecursive(this,false);
    
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    
    //Create some fake pop dont_leave.png
    /*
    CCLayerColor* aLayerFake = CCLayerColor::create(ccc4(0,0,0,64),_screenSize.width,_screenSize.height);
    aLayerFake->setTag(2014);
    addChild(aLayerFake,kHUD_Z_Order+100);
    
    CCSprite* aSpriteDum = CCSprite::create("Interfeiss/before_quit/dont_leave.png");
    aLayerFake->addChild(aSpriteDum);
    aSpriteDum->setPosition(ccp(aLayerFake->getContentSize().width/2,aLayerFake->getContentSize().height/2));
    
    //Add some button for clouse
    //Debug buttons
    CCMenuItemImage* debug_button_1 = CCMenuItemImage::create(
                                                              "backbtn0001.png",
                                                              "backbtn0002.png",
                                                              this,
                                                              menu_selector(GameScene::OnExitWithSpecialMission));
    debug_button_1->setAnchorPoint(ccp(0,0));
    
    CCMenu* debugMenu = CCMenu::create(debug_button_1,NULL);
    
    debugMenu->alignItemsHorizontallyWithPadding(10);
    debugMenu->setPosition(ccp(aSpriteDum->getContentSize().width/2,aSpriteDum->getContentSize().height/2));
    aSpriteDum->addChild(debugMenu);
    */
    
    CCLayerColor* aLayerFake = CCLayerColor::create(ccc4(0,0,0,64),_screenSize.width,_screenSize.height);
    aLayerFake->setTag(2014);
    addChild(aLayerFake,kHUD_Z_Order+100);
    
    CCSprite* aSpriteDum = CCSprite::create("Interfeiss/challenges/complete_popup/dont_leave.png");
    aLayerFake->addChild(aSpriteDum);
    aSpriteDum->setPosition(ccp(aLayerFake->getContentSize().width/2,aLayerFake->getContentSize().height/2));
    
    //Add some text !!!
    cocos2d::CCLabelTTF* _PopText;
    //Add the text field !!!
    _PopText = CCLabelTTF::create("",
                                  "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                  CCSize(400, 280), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    _PopText->setAnchorPoint(ccp(0.5,0.5f));
    _PopText->setColor(ccc3(79, 65, 33));
    _PopText->setPositionX(350);//This can change by reward type
    _PopText->setPositionY(300);
    aSpriteDum->addChild(_PopText);
    
    std::stringstream theTextField;
//    if(User::getInstance()->mSpecial_13_Mission){
//        theTextField << "Send safely 25\nDwarfs to the cave";
//    }
//    else if(User::getInstance()->mSpecial_12_Mission){
//        theTextField << "Send 25 dwarfs to\nthe tesla machine\nTo complete it";
//    }
//    else if(User::getInstance()->mSpecial_11_Mission){
//        theTextField << "Collect all crystals\nFrom crystal heap";
//    }
//    else if(User::getInstance()->mSpecial_10_Mission){
//        theTextField << "Send safely 20\nDwarfs to the cave";
//    }
//    else if(User::getInstance()->mSpecial_14_Mission){
//        theTextField << "Send safely 32\nDwarfs to the cave";
//    }
    
    if(User::getInstance()->mSpecialMissionBuild){
        theTextField<<"Congrats! Well done!\nYou find important\npart for Tesla!";
        
        //Add some part image !!!
        CCSprite* aDebugPic = CCSprite::create("MissionCompleted.png");
        CCSprite* aIcon = NULL;
        if(User::getInstance()->mSpecial_10_Mission){
            aIcon = CCSprite::create("powerup/base_big.png");
        }
        else if(User::getInstance()->mSpecial_11_Mission){
            aIcon = CCSprite::create("powerup/top_big.png");
        }
        else if(User::getInstance()->mSpecial_12_Mission){
            aIcon = CCSprite::create("powerup/rats_big.png");
        }
        else if(User::getInstance()->mSpecial_13_Mission){
            aIcon = CCSprite::create("powerup/progres_big.png");
        }
        else if(User::getInstance()->mSpecial_14_Mission){
//            aIcon = CCSprite::create("powerup/base_big.png");
            theTextField.str("");
            theTextField.clear();
            theTextField<<"Congrats! Well done!";
        }
        
        if(aIcon != NULL){
            aIcon->setPosition(ccp(300,100));
        }
        aDebugPic->setPosition(ccp(300,-40));
        
        if(aIcon!=NULL){
            aSpriteDum->addChild(aIcon);
        }
        
        aSpriteDum->addChild(aDebugPic);
    }
    
    _PopText->setString(theTextField.str().c_str());
    
    //Add some button for clouse
    CCMenuItemImage* debug_button_1 = CCMenuItemImage::create(
                                                              "Interfeiss/before_quit/check_btn0001.png",
                                                              "Interfeiss/before_quit/check_btn0002.png",
                                                              this,
                                                              menu_selector(GameScene::OnExitWithSpecialMission));
    
    CCMenu* debugMenu = CCMenu::create(debug_button_1,NULL);
    debugMenu->setPosition(ccp(500,100));
    aSpriteDum->addChild(debugMenu);
}

//Gets back to mission scene !!!
void GameScene::OnExitWithSpecialMission()
{
    removeChildByTag(2014);
    
    //This time shows mission screen
    if(User::getInstance()->mSpecialMissionBuild)
    {
        if(User::getInstance()->mSpecialMissionProgress<4){
            User::getInstance()->mSpecialMissionProgress+=1;
            User::getInstance()->mShowNewPart = true;
        }
//        CCScene* options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MISSIONS);
        //        CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, options);
//        CCTransitionScene* transition = CCTransitionFade::create(0.5f,options,ccBLACK);
//        CCDirector::sharedDirector()->replaceScene(transition);
        
        CCScene* options = MissionScene::scene();
        CCTransitionScene* transition = CCTransitionFade::create(0.5f,options,ccBLACK);
        CCDirector::sharedDirector()->replaceScene(transition);
    }
    else
    {
        CCScene* options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
        //        CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, options);
        CCTransitionScene* transition = CCTransitionFade::create(0.5f,options,ccBLACK);
        CCDirector::sharedDirector()->replaceScene(transition);
    }
}

//----------------------------

void GameScene::CraeteHUD()
{
    if(User::getInstance()->mSpecial_10_Mission || User::getInstance()->mSpecial_11_Mission || User::getInstance()->mSpecial_12_Mission
       || User::getInstance()->mSpecial_13_Mission || User::getInstance()->mSpecial_14_Mission)
    {
        _DwarfsEnteredCave = 0;
        
        _gamePause = true;
        pauseSchedulerAndActionsRecursive(this,false);
        
        //Hide some stuff and show some extra stuff !!! + Set some special flags for test !!!
        
        //Create some fake pop dont_leave.png
        CCLayerColor* aLayerFake = CCLayerColor::create(ccc4(0,0,0,64),_screenSize.width,_screenSize.height);
        aLayerFake->setTag(2014);
        addChild(aLayerFake,kHUD_Z_Order+100);
        
        CCSprite* aSpriteDum = CCSprite::create("Interfeiss/challenges/complete_popup/dont_leave.png");
        aLayerFake->addChild(aSpriteDum);
        aSpriteDum->setPosition(ccp(aLayerFake->getContentSize().width/2,aLayerFake->getContentSize().height/2));
        
        //Add some text !!!
        cocos2d::CCLabelTTF* _PopText;
        //Add the text field !!!
        _PopText = CCLabelTTF::create("",
                                        "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                        CCSize(400, 280), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        _PopText->setAnchorPoint(ccp(0.5,0.5f));
        _PopText->setColor(ccc3(79, 65, 33));
        _PopText->setPositionX(350);//This can change by reward type
        _PopText->setPositionY(300);
        aSpriteDum->addChild(_PopText);
        
        std::stringstream theTextField;
        if(User::getInstance()->mSpecial_13_Mission){
            theTextField << "Send safely 25\nDwarfs to the cave";
        }
        else if(User::getInstance()->mSpecial_12_Mission){
            theTextField << "Send 25 dwarfs to\nthe tesla machine\nTo complete it";
        }
        else if(User::getInstance()->mSpecial_11_Mission){
            theTextField << "Collect all crystals\nFrom crystal heap";
        }
        else if(User::getInstance()->mSpecial_10_Mission){
            theTextField << "Send safely 20\nDwarfs to the cave";
        }
        else if(User::getInstance()->mSpecial_14_Mission){
            theTextField << "Send safely 32\nDwarfs to the cave";
        }

        
        _PopText->setString(theTextField.str().c_str());
        
        //Add some button for clouse
        //Debug buttons
        CCMenuItemImage* debug_button_1 = CCMenuItemImage::create(
                                                                  "Interfeiss/before_quit/check_btn0001.png",
                                                                  "Interfeiss/before_quit/check_btn0002.png",
                                                                  this,
                                                                  menu_selector(GameScene::StartSpecialMission));
        //        debug_button_1->setAnchorPoint(ccp(0,0));
        
        CCMenu* debugMenu = CCMenu::create(debug_button_1,NULL);
        debugMenu->setPosition(ccp(270,100));
        
        //        debugMenu->alignItemsHorizontallyWithPadding(10);
        //        debugMenu->setPosition(ccp(aSpriteDum->getContentSize().width/2,aSpriteDum->getContentSize().height/2));
        aSpriteDum->addChild(debugMenu);
        
        
        //Add to it all the stuff
    }
    else if(User::getInstance()->mDynamicTrolls)
    {
        mChargesForBlitz = 2;
        
        _gameTime = 60;
        
        _mission_allowed_effect = EFFECT_TYPE_WEB;
        
        //------------------------------------
        //We will show some popup here too !!!
        
        _gamePause = true;
        pauseSchedulerAndActionsRecursive(this,false);
        
        CCLayerColor* aLayerFake = CCLayerColor::create(ccc4(0,0,0,64),_screenSize.width,_screenSize.height);
        aLayerFake->setTag(2014);
        addChild(aLayerFake,kHUD_Z_Order+100);
        
        CCSprite* aSpriteDum = CCSprite::create("Interfeiss/challenges/complete_popup/dont_leave.png");
        aLayerFake->addChild(aSpriteDum);
        aSpriteDum->setPosition(ccp(aLayerFake->getContentSize().width/2,aLayerFake->getContentSize().height/2));
        
        cocos2d::CCLabelTTF* _PopText;
        _PopText = CCLabelTTF::create("",
                                      "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                      CCSize(400, 280), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        _PopText->setAnchorPoint(ccp(0.5,0.5f));
        _PopText->setColor(ccc3(79, 65, 33));
        _PopText->setPositionX(350);//This can change by reward type
        _PopText->setPositionY(300);
        aSpriteDum->addChild(_PopText);
        
        _mission_count_dwarfs = false;
        
        std::stringstream theTextField;
        //User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission
        if(User::getInstance()->mSpecial_23_Mission){
            _mission_star_points_1 = 20;
            _mission_star_points_2 = 30;
            _mission_star_points_3 = 50;
            
            theTextField << "Lead dwarfs\nin to cave!";
        }
        else if(User::getInstance()->mSpecial_22_Mission){
            _mission_star_points_1 = 20;
            _mission_star_points_2 = 30;
            _mission_star_points_3 = 50;
            
            theTextField << "Lead dwarfs\nin to cave!";
        }
        else if(User::getInstance()->mSpecial_21_Mission){
            _mission_star_points_1 = 500;
            _mission_star_points_2 = 600;
            _mission_star_points_3 = 700;
            
            _mission_count_dwarfs = true;
            
            theTextField << "Collect crystals\nto complete mission!";
        }
        else if(User::getInstance()->mSpecial_20_Mission){
            _mission_star_points_1 = 500;
            _mission_star_points_2 = 600;
            _mission_star_points_3 = 700;
            
            _mission_count_dwarfs = true;
            
            theTextField << "Collect crystals\nto complete mission!";
        }
        else if(User::getInstance()->mSpecial_19_Mission){
            _mission_star_points_1 = 20;
            _mission_star_points_2 = 35;
            _mission_star_points_3 = 40;
            
            theTextField << "Lead dwarfs\nin to cave!";
        }
        else if(User::getInstance()->mSpecial_18_Mission){
            _mission_star_points_1 = 1000;
            _mission_star_points_2 = 1600;
            _mission_star_points_3 = 2000;
            
            theTextField << "Collect 1000 points\nand You've got Unlimited Dwarfs\nto work with!";
        }
        else if(User::getInstance()->mSpecial_17_Mission){
            theTextField << "Collect 3000 points\nand You've got 50 Dwarfs\nto work with!";
        }
        else{
            theTextField << "Collect 1500 points\nand You've got 50 Dwarfs\nto work with!";
        }
        
        _PopText->setString(theTextField.str().c_str());
        CCMenuItemImage* debug_button_1 = CCMenuItemImage::create(
                                                                  "Interfeiss/before_quit/check_btn0001.png",
                                                                  "Interfeiss/before_quit/check_btn0002.png",
                                                                  this,
                                                                  menu_selector(GameScene::StartSpecialMission));
        CCMenu* debugMenu = CCMenu::create(debug_button_1,NULL);
        debugMenu->setPosition(ccp(270,100));
        aSpriteDum->addChild(debugMenu);
        
        //------------------------------------
        
        if(User::getInstance()->mSpecial_18_Mission){
            //=========================
            
            CCSprite* aBaseProgress = CCSprite::create("small_dot_red.png");
            addChild(aBaseProgress);
            aBaseProgress->setPosition(ccp(94,_screenSize.height-86));
            aBaseProgress->setScaleX(0.25);
            aBaseProgress->setScaleY(0.6);
            
            CCSprite* aDummy = CCSprite::create("Interfeiss/challenges/daily/progress_days.png");
            aDummy->setPosition(ccp(-10,0));
            aBaseProgress->addChild(aDummy);
            
            int aOneStep = aDummy->getContentSize().width/4;
            
            
            
            _mission_progress_bar_1 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
            _mission_progress_bar_1->setScaleX(0.25);
            _mission_progress_bar_1->setAnchorPoint(ccp(0,0.5f));
            _mission_progress_bar_1->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6,aDummy->getPositionY()));
            aBaseProgress->addChild(_mission_progress_bar_1);
            
            _mission_dwarfs_spawned = 0;
            _mission_dwarfs_removed = 0;
            
            _mission_crystal_spawn_timer = 1;
            _mission_dwarf_spawn_timer = 12;
            _mission_effect_spawn_timer = 10;
            
            UpdateMissionStars();
            
            std::stringstream theComboParticle;
            if(User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission){
                theComboParticle << "0";
            }
            else{
                theComboParticle << _stats.points <<"/"<<_mission_star_points_1;
            }
            _pointsLabel->setString(theComboParticle.str().c_str());
        }
        else if(User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
                || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission)
        {
            _mission_dwarfs_spawned = 0;
            _mission_dwarfs_removed = 0;
            
            _mission_crystal_spawn_timer = 1;
            _mission_dwarf_spawn_timer = 12;
            _mission_effect_spawn_timer = 10;
            
            std::stringstream theComboParticle;
            if(User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission
               || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission){
                theComboParticle << "0";
            }
            else{
                theComboParticle << _stats.points <<"/"<<_mission_star_points_1;
            }
            _pointsLabel->setString(theComboParticle.str().c_str());
            
            //-----------------------------------------------------------------
            //Create the ultra combo bar !!!
            
            CCSprite* aBaseProgress = CCSprite::create("small_dot_red.png");
            addChild(aBaseProgress);
            aBaseProgress->setPosition(ccp(94,_screenSize.height-86));
            aBaseProgress->setScaleX(0.25);
            aBaseProgress->setScaleY(0.6);
            
            CCSprite* aDummy = CCSprite::create("Interfeiss/challenges/daily/progress_days.png");
            aDummy->setPosition(ccp(-10,0));
            aBaseProgress->addChild(aDummy);
            
            int aOneStep = aDummy->getContentSize().width/4;
            
            _mission_progress_bar_1 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
            _mission_progress_bar_1->setScaleX(0.25);
            _mission_progress_bar_1->setAnchorPoint(ccp(0,0.5f));
            _mission_progress_bar_1->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6,aDummy->getPositionY()));
            aBaseProgress->addChild(_mission_progress_bar_1);
            
            _mission_progress_bar_2 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
            _mission_progress_bar_2->setAnchorPoint(ccp(0,0.5f));
            _mission_progress_bar_2->setScaleX(0.25);
            _mission_progress_bar_2->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep,aDummy->getPositionY()));
            aBaseProgress->addChild(_mission_progress_bar_2);
            
            _mission_progress_bar_3 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
            _mission_progress_bar_3->setAnchorPoint(ccp(0,0.5f));
            _mission_progress_bar_3->setScaleX(0.25);
            _mission_progress_bar_3->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep*2,aDummy->getPositionY()));
            aBaseProgress->addChild(_mission_progress_bar_3);
            
            _mission_progress_bar_4 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
            _mission_progress_bar_4->setAnchorPoint(ccp(0,0.5f));
            _mission_progress_bar_4->setScaleX(0.23);
            _mission_progress_bar_4->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep*3,aDummy->getPositionY()));
            aBaseProgress->addChild(_mission_progress_bar_4);
            
            
            
            CCSprite* aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
            aDummySpe->setPosition(ccp(-aOneStep,0));
            aBaseProgress->addChild(aDummySpe);
            
            aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
            aDummySpe->setPosition(ccp(0,0));
            aBaseProgress->addChild(aDummySpe);
            
            aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
            aDummySpe->setPosition(ccp(aOneStep,0));
            aBaseProgress->addChild(aDummySpe);
            
            
            //-----------------------------------------------------------------
            
            //Create the 3 stars
            _mission_star_1 = CCSprite::create("new_mission/zvaigzne_final.png");
            _mission_star_1->setPosition(ccp(-aOneStep,-45));
            _mission_star_1->setOpacity(80);
            _mission_star_1->setScaleX(2.2);
            _mission_star_1->setScaleY(1.0);
            aBaseProgress->addChild(_mission_star_1,kHUD_Z_Order+1);
            
            _mission_star_2 = CCSprite::create("new_mission/zvaigzne_final.png");
            _mission_star_2->setPosition(ccp(0,-45));
            _mission_star_2->setOpacity(80);
            _mission_star_2->setScaleX(2.2);
            _mission_star_2->setScaleY(1.0);
            aBaseProgress->addChild(_mission_star_2,kHUD_Z_Order+1);
            
            _mission_star_3 = CCSprite::create("new_mission/zvaigzne_final.png");
            _mission_star_3->setPosition(ccp(aOneStep,-45));
            _mission_star_3->setOpacity(80);
            _mission_star_3->setScaleX(2.2);
            _mission_star_3->setScaleY(1.0);
            aBaseProgress->addChild(_mission_star_3,kHUD_Z_Order+1);
            
            
            mMaxBarPoints = _mission_star_points_3;
            
            UpdateMissionStars();
        }
        
        if(mNewSplitCaves && User::getInstance()->mSpecial_20_Mission){
            generateTrollMission(_screenSize.width/2,_screenSize.height/2,1.6,false,true,
                                 ccp(_screenSize.width/2,_screenSize.height-100),ccp(_screenSize.width/2,100),3);
        }
        else{
            generateTrollMission(_screenSize.width/2,_screenSize.height-180,1.6,false,true,
                                 ccp(240,_screenSize.height-180),ccp(_screenSize.width-240,_screenSize.height-180),1);
        }

        
        //Add some debug info for users !!!
//        mDebugSpeed_dwarf
        
        
        CCMenuItemImage* debug_button_T1 = CCMenuItemImage::create(
                                                                  "Interfeiss/buttons/arrow_l_btn0001.png",
                                                                  "Interfeiss/buttons/arrow_l_btn0002.png",
                                                                  this,
                                                                  menu_selector(GameScene::OnDecreaseTrollSpeed));
        CCMenuItemImage* debug_button_T2 = CCMenuItemImage::create(
                                                                  "Interfeiss/buttons/arrow_r_btn0001.png",
                                                                  "Interfeiss/buttons/arrow_r_btn0002.png",
                                                                  this,
                                                                  menu_selector(GameScene::OnIncreaseTrollSpeed));
        
        CCMenu* debugMenu_T = CCMenu::create(debug_button_T1,debug_button_T2,NULL);
        debugMenu_T->alignItemsHorizontally();
        debugMenu_T->setPosition(ccp(370,40));
        addChild(debugMenu_T,100);
        
        _TrollSpeed = CCLabelTTF::create("Troll Speed: 1.0",
                                        "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                        CCSize(400, 280), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        _TrollSpeed->setPosition(ccp(370,70));
        addChild(_TrollSpeed,kHUD_Z_Order+1);
        
        CCMenuItemImage* debug_button_D1 = CCMenuItemImage::create(
                                                                   "Interfeiss/buttons/arrow_l_btn0001.png",
                                                                   "Interfeiss/buttons/arrow_l_btn0002.png",
                                                                   this,
                                                                   menu_selector(GameScene::OnDecreaseDwarfSpeed));
        CCMenuItemImage* debug_button_D2 = CCMenuItemImage::create(
                                                                   "Interfeiss/buttons/arrow_r_btn0001.png",
                                                                   "Interfeiss/buttons/arrow_r_btn0002.png",
                                                                   this,
                                                                   menu_selector(GameScene::OnIncreaseDwarfSpeed));
        
        CCMenu* debugMenu_D = CCMenu::create(debug_button_D1,debug_button_D2,NULL);
        debugMenu_D->alignItemsHorizontally();
        debugMenu_D->setPosition(ccp(570,40));
        addChild(debugMenu_D,100);
        
        _DwarfSpeed = CCLabelTTF::create("Dwarf Speed: 1.0",
                                         "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                         CCSize(400, 280), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        _DwarfSpeed->setPosition(ccp(570,70));
        addChild(_DwarfSpeed,kHUD_Z_Order+1);
    }
    else if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission)
    {
        mChargesForBlitz = 2;
        
        
        //Lets create some extra stuff here
        _dwarfOnMap = CCLabelTTF::create("10 Dwarfs Left",
                                         "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                         CCSize(400, 280), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        _dwarfOnMap->setPosition(ccp(_screenSize.width/2,_screenSize.height-50));
        addChild(_dwarfOnMap,kHUD_Z_Order+1);
        
        _mission_star_points_1 = 1500;
        _mission_star_points_2 = 1600;
        _mission_star_points_3 = 2000;
        
        if(User::getInstance()->mSpecial_17_Mission){
            _mission_star_points_1 = 3000;
            _mission_star_points_2 = 3200;
            _mission_star_points_3 = 4000;
        }
        
        
        
        _gameTime = 120;
        
        //Only troll can spawn stuff
//        _mission_allowed_effect = -1;//No effects allowed in this game mode
        _mission_allowed_effect = EFFECT_TYPE_WEB;//No effects allowed in this game mode
        
        _mission_dwarfs_max = 50;
        
        //------------------------------------
        //We will show some popup here too !!!
        
        _gamePause = true;
        pauseSchedulerAndActionsRecursive(this,false);
        
        CCLayerColor* aLayerFake = CCLayerColor::create(ccc4(0,0,0,64),_screenSize.width,_screenSize.height);
        aLayerFake->setTag(2014);
        addChild(aLayerFake,kHUD_Z_Order+100);
        
        CCSprite* aSpriteDum = CCSprite::create("Interfeiss/challenges/complete_popup/dont_leave.png");
        aLayerFake->addChild(aSpriteDum);
        aSpriteDum->setPosition(ccp(aLayerFake->getContentSize().width/2,aLayerFake->getContentSize().height/2));
        
        cocos2d::CCLabelTTF* _PopText;
        _PopText = CCLabelTTF::create("",
                                      "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                      CCSize(400, 280), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        _PopText->setAnchorPoint(ccp(0.5,0.5f));
        _PopText->setColor(ccc3(79, 65, 33));
        _PopText->setPositionX(350);//This can change by reward type
        _PopText->setPositionY(300);
        aSpriteDum->addChild(_PopText);
        
        std::stringstream theTextField;
        if(User::getInstance()->mSpecial_17_Mission){
            theTextField << "Collect 3000 points\nand You've got 50 Dwarfs\nto work with!";
        }
        else{
            theTextField << "Collect 1500 points\nand You've got 50 Dwarfs\nto work with!";
        }
        
        _PopText->setString(theTextField.str().c_str());
        CCMenuItemImage* debug_button_1 = CCMenuItemImage::create(
                                                                  "Interfeiss/before_quit/check_btn0001.png",
                                                                  "Interfeiss/before_quit/check_btn0002.png",
                                                                  this,
                                                                  menu_selector(GameScene::StartSpecialMission));
        CCMenu* debugMenu = CCMenu::create(debug_button_1,NULL);
        debugMenu->setPosition(ccp(270,100));
        aSpriteDum->addChild(debugMenu);
        
        //------------------------------------
        
        std::stringstream aDwarfOnMap;
        aDwarfOnMap<<_mission_dwarfs_max<<" Dwarfs Left";
        
        
        _dwarfOnMap->setString(aDwarfOnMap.str().c_str());
        
        //=========================
        
        CCSprite* aBaseProgress = CCSprite::create("small_dot_red.png");
        addChild(aBaseProgress);
        aBaseProgress->setPosition(ccp(94,_screenSize.height-86));
        aBaseProgress->setScaleX(0.25);
        aBaseProgress->setScaleY(0.6);
        
        CCSprite* aDummy = CCSprite::create("Interfeiss/challenges/daily/progress_days.png");
        aDummy->setPosition(ccp(-10,0));
        aBaseProgress->addChild(aDummy);
        
        int aOneStep = aDummy->getContentSize().width/4;
        
        _mission_progress_bar_1 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
        _mission_progress_bar_1->setScaleX(0.25);
        _mission_progress_bar_1->setAnchorPoint(ccp(0,0.5f));
        _mission_progress_bar_1->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6,aDummy->getPositionY()));
        aBaseProgress->addChild(_mission_progress_bar_1);
        
        _mission_dwarfs_spawned = 0;
        _mission_dwarfs_removed = 0;
        
        _mission_crystal_spawn_timer = 1;
        _mission_dwarf_spawn_timer = 12;
        _mission_effect_spawn_timer = 10;
        
        UpdateMissionStars();
        
        std::stringstream theComboParticle;
        theComboParticle << _stats.points <<"/"<<_mission_star_points_1;
        _pointsLabel->setString(theComboParticle.str().c_str());
    }
    else if(User::getInstance()->mNewMissionBuild)
    {
        //Lets create some extra stuff here
        _dwarfOnMap = CCLabelTTF::create("10 Dwarfs Left",
                                        "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                        CCSize(400, 280), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        _dwarfOnMap->setPosition(ccp(_screenSize.width/2,_screenSize.height-50));
        addChild(_dwarfOnMap,kHUD_Z_Order+1);
        
        //Check what mission is now active
        if(User::getInstance()->getMissionManager().GetActiveMissionID() == 1)// || User::getInstance()->getMissionManager().GetActiveMissionID() >= 2)
        {
            _mission_star_points_1 = 50;
            _mission_star_points_2 = 60;
            _mission_star_points_3 = 70;
            
            _mission_allowed_effect = -1;//No effects allowed in this game mode
            
            _mission_dwarfs_max = 5;
            
            //Start the tutorial !!!
            
        }
        else if(User::getInstance()->getMissionManager().GetActiveMissionID() == 2)
        {
            _mission_star_points_1 = 130;
            _mission_star_points_2 = 160;
            _mission_star_points_3 = 220;
            
            _mission_allowed_effect = EFFECT_TYPE_RAIN;//No effects allowed in this game mode
            
            _mission_dwarfs_max = 7;
            
            generateTrollMission(_screenSize.width/2,200,0,true,false,CCPointZero,CCPointZero,-1);
        }
        else if(User::getInstance()->getMissionManager().GetActiveMissionID() == 3)
        {
            _mission_star_points_1 = 150;
            _mission_star_points_2 = 200;
            _mission_star_points_3 = 250;
            
            _mission_dwarfs_max = 10;
            
            _mission_allowed_effect = EFFECT_TYPE_ICE_BARRAGE;//No effects allowed in this game mode
            
            generateTrollMission(240,_screenSize.height/2,1.6,false,true,ccp(240,_screenSize.height-100),ccp(240,100),-1);
            generateTrollMission(_screenSize.width-240,_screenSize.height/2,4.7,false,true,
                                 ccp(_screenSize.width-240,100),ccp(_screenSize.width-240,_screenSize.height-100),-1);
        }
        else if(User::getInstance()->getMissionManager().GetActiveMissionID() == 4)
        {
            _mission_star_points_1 = 200;
            _mission_star_points_2 = 250;
            _mission_star_points_3 = 300;
            
            _mission_dwarfs_max = 10;
            
            _mission_allowed_effect = EFFECT_TYPE_WIND;//No effects allowed in this game mode
            
            generateTrollMission(240,_screenSize.height/2,1.6,false,true,ccp(240,_screenSize.height-100),ccp(240,100),-1);
            generateTrollMission(_screenSize.width-240,_screenSize.height/2,4.7,false,true,
                                 ccp(_screenSize.width-240,100),ccp(_screenSize.width-240,_screenSize.height-100),-1);
            generateTrollMission(_screenSize.width/2,200,0,true,false,CCPointZero,CCPointZero,-1);
            
        }
        else if(User::getInstance()->getMissionManager().GetActiveMissionID() >= 5)
        {
            _mission_star_points_1 = 200;
            _mission_star_points_2 = 300;
            _mission_star_points_3 = 400;
            
            _mission_dwarfs_max = 15;
            
            _mission_allowed_effect = EFFECT_TYPE_WEB;//No effects allowed in this game mode
            
            generateTrollMission(240,_screenSize.height/2,1.6,false,true,ccp(240,_screenSize.height-100),ccp(240,100),-1);
            generateTrollMission(_screenSize.width-240,_screenSize.height/2,4.7,false,true,
                                 ccp(_screenSize.width-240,100),ccp(_screenSize.width-240,_screenSize.height-100),-1);
            generateTrollMission(_screenSize.width/2,200,0,true,false,CCPointZero,CCPointZero,-1);
        }
        
        std::stringstream aDwarfOnMap;
        aDwarfOnMap<<_mission_dwarfs_max<<" Dwarfs Left";
        
        
        _dwarfOnMap->setString(aDwarfOnMap.str().c_str());
        
        //-----------------------------------------------------------------
        //Create the ultra combo bar !!!
        
        CCSprite* aBaseProgress = CCSprite::create("small_dot_red.png");
        addChild(aBaseProgress);
        aBaseProgress->setPosition(ccp(94,_screenSize.height-86));
        aBaseProgress->setScaleX(0.25);
        aBaseProgress->setScaleY(0.6);
        
        CCSprite* aDummy = CCSprite::create("Interfeiss/challenges/daily/progress_days.png");
        aDummy->setPosition(ccp(-10,0));
        aBaseProgress->addChild(aDummy);
        
        int aOneStep = aDummy->getContentSize().width/4;
        
        _mission_progress_bar_1 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
        _mission_progress_bar_1->setScaleX(0.25);
        _mission_progress_bar_1->setAnchorPoint(ccp(0,0.5f));
        _mission_progress_bar_1->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6,aDummy->getPositionY()));
        aBaseProgress->addChild(_mission_progress_bar_1);
        
        _mission_progress_bar_2 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
        _mission_progress_bar_2->setAnchorPoint(ccp(0,0.5f));
        _mission_progress_bar_2->setScaleX(0.25);
        _mission_progress_bar_2->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep,aDummy->getPositionY()));
        aBaseProgress->addChild(_mission_progress_bar_2);
        
        _mission_progress_bar_3 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
        _mission_progress_bar_3->setAnchorPoint(ccp(0,0.5f));
        _mission_progress_bar_3->setScaleX(0.25);
        _mission_progress_bar_3->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep*2,aDummy->getPositionY()));
        aBaseProgress->addChild(_mission_progress_bar_3);
        
        _mission_progress_bar_4 = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
        _mission_progress_bar_4->setAnchorPoint(ccp(0,0.5f));
        _mission_progress_bar_4->setScaleX(0.23);
        _mission_progress_bar_4->setPosition(ccp(aDummy->getPositionX()-aDummy->getContentSize().width/2+6+aOneStep*3,aDummy->getPositionY()));
        aBaseProgress->addChild(_mission_progress_bar_4);

        
        
        CCSprite* aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
        aDummySpe->setPosition(ccp(-aOneStep,0));
        aBaseProgress->addChild(aDummySpe);
        
        aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
        aDummySpe->setPosition(ccp(0,0));
        aBaseProgress->addChild(aDummySpe);
        
        aDummySpe = CCSprite::create("Interfeiss/challenges/daily/seperator_days.png");
        aDummySpe->setPosition(ccp(aOneStep,0));
        aBaseProgress->addChild(aDummySpe);
        
        
        //-----------------------------------------------------------------
        
        //Create the 3 stars
        _mission_star_1 = CCSprite::create("new_mission/zvaigzne_final.png");
        _mission_star_1->setPosition(ccp(-aOneStep,-45));
        _mission_star_1->setOpacity(80);
        _mission_star_1->setScaleX(2.2);
        _mission_star_1->setScaleY(1.0);
        aBaseProgress->addChild(_mission_star_1,kHUD_Z_Order+1);
        
        _mission_star_2 = CCSprite::create("new_mission/zvaigzne_final.png");
        _mission_star_2->setPosition(ccp(0,-45));
        _mission_star_2->setOpacity(80);
        _mission_star_2->setScaleX(2.2);
        _mission_star_2->setScaleY(1.0);
        aBaseProgress->addChild(_mission_star_2,kHUD_Z_Order+1);
        
        _mission_star_3 = CCSprite::create("new_mission/zvaigzne_final.png");
        _mission_star_3->setPosition(ccp(aOneStep,-45));
        _mission_star_3->setOpacity(80);
        _mission_star_3->setScaleX(2.2);
        _mission_star_3->setScaleY(1.0);
        aBaseProgress->addChild(_mission_star_3,kHUD_Z_Order+1);
        
        
        mMaxBarPoints = _mission_star_points_3;
        
        UpdateMissionStars();
        
        //Set the progress bar
//        _mission_progress_bar->setTextureRect(CCRect(0, 0,
//                                                     _mission_progress_bar->getTexture()->getContentSize().width*((float)50 / mMaxBarPoints),
//                                                     _mission_progress_bar->getTexture()->getContentSize().height));
        
//        mNewMissionProgress
        
        
        //Create the 3 trolls !!!
//        generateTrollMission(240,_screenSize.height/2,1.6,false,true,ccp(240,_screenSize.height-100),ccp(240,100));
//        generateTrollMission(_screenSize.width-240,_screenSize.height/2,4.7,false,true,
//                             ccp(_screenSize.width-240,100),ccp(_screenSize.width-240,_screenSize.height-100));
//        generateTrollMission(_screenSize.width/2,200,0,true,false,CCPointZero,CCPointZero);
        
        
        _mission_dwarfs_spawned = 0;
        _mission_dwarfs_removed = 0;
        
        _mission_crystal_spawn_timer = 1;
        _mission_dwarf_spawn_timer = 12;
        _mission_effect_spawn_timer = 10;
    }
    
    //Create all the tweens !!!
    CCMenu* aDummyMenu = (CCMenu*)getChildByTag(100);
    CCMoveTo* aActionMove1;
    CCEaseSineOut* aEaseActionMove1;
    
    aActionMove1 = CCMoveTo::create(0.5f,ccp(5, 5));
    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
    aDummyMenu->runAction(aEaseActionMove1);
    
    aDummyMenu = (CCMenu*)getChildByTag(90002);
    aActionMove1 = CCMoveTo::create(0.5f,ccp(_screenSize.width - 5, 5));
    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
    aDummyMenu->runAction(aEaseActionMove1);
    
    aDummyMenu = (CCMenu*)getChildByTag(90003);
    aActionMove1 = CCMoveTo::create(0.5f,ccp(0, _screenSize.height-54));
    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
    aDummyMenu->runAction(aEaseActionMove1);
    
    aDummyMenu = (CCMenu*)getChildByTag(90004);
    aActionMove1 = CCMoveTo::create(0.5f,ccp((_screenSize.width-aDummyMenu->getContentSize().width/2)+21, _screenSize.height-55));
    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
    aDummyMenu->runAction(aEaseActionMove1);
    
    aDummyMenu = (CCMenu*)getChildByTag(90005);
    aActionMove1 = CCMoveTo::create(0.5f,ccp(_screenSize.width-aDummyMenu->getContentSize().width/2,_screenSize.height-10));
    aEaseActionMove1 = CCEaseSineOut::create(aActionMove1);
    aDummyMenu->runAction(aEaseActionMove1);
    
    
}

void GameScene::OnIncreaseDwarfSpeed()
{
    User::getInstance()->mDebugSpeed_dwarf+=0.1;
    
    std::stringstream aStringValue;
    aStringValue<<"Dwarf Speed: "<<User::getInstance()->mDebugSpeed_dwarf;
    _DwarfSpeed->setString(aStringValue.str().c_str());
}

void GameScene::OnDecreaseDwarfSpeed()
{
    if(User::getInstance()->mDebugSpeed_dwarf>0.1){
        User::getInstance()->mDebugSpeed_dwarf-=0.1;
    }
    else{
        User::getInstance()->mDebugSpeed_dwarf=0.1;
    }
    
    std::stringstream aStringValue;
    aStringValue<<"Dwarf Speed: "<<User::getInstance()->mDebugSpeed_dwarf;
    _DwarfSpeed->setString(aStringValue.str().c_str());
}

void GameScene::OnIncreaseTrollSpeed()
{
    User::getInstance()->mDebugSpeed_troll+=0.1;
    
    std::stringstream aStringValue;
    aStringValue<<"Troll Speed: "<<User::getInstance()->mDebugSpeed_troll;
    _TrollSpeed->setString(aStringValue.str().c_str());
}

void GameScene::OnDecreaseTrollSpeed()
{
    if(User::getInstance()->mDebugSpeed_troll>0.1){
        User::getInstance()->mDebugSpeed_troll-=0.1;
    }
    else{
        User::getInstance()->mDebugSpeed_troll=0.1;
    }
    
    std::stringstream aStringValue;
    aStringValue<<"Troll Speed: "<<User::getInstance()->mDebugSpeed_troll;
    _TrollSpeed->setString(aStringValue.str().c_str());
}


void GameScene::UpdateMissionStars()
{
    return;
    
    //Count dwarfs as points
    if(mCurrentMission.Task_type == 1){
        mTotalPointsInGame = _mission_dwarfs_saved_counter;
    }
    else if(mCurrentMission.Task_type == 0){
        //Crystal points
    }
    
    /*
    if(User::getInstance()->mDynamicTrolls && User::getInstance()->mSpecial_18_Mission==false
       && User::getInstance()->mSpecial_19_Mission==false && User::getInstance()->mSpecial_20_Mission==false
       && User::getInstance()->mSpecial_21_Mission==false && User::getInstance()->mSpecial_22_Mission==false
       && User::getInstance()->mSpecial_23_Mission==false){
        return;
    }
    
    //maybe latter add some tween stuff !!!
    if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission){
        
        int aDummyPoints = mTotalPointsInGame;
        if(aDummyPoints>_mission_star_points_1){
            aDummyPoints = _mission_star_points_1;
        }
        
        
        _mission_progress_bar_1->setTextureRect(CCRect(0, 0,
                                                       _mission_progress_bar_1->getTexture()->getContentSize().width*((float)aDummyPoints / _mission_star_points_1),
                                                       _mission_progress_bar_1->getTexture()->getContentSize().height));
        
        return;
    }
    
    if(User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission){
        mTotalPointsInGame = _mission_dwarfs_saved_counter;
    }
    */
    
    //Check by current mission and points
    if(mTotalPointsInGame>=_mission_star_points_1){
        _mission_star_1->setOpacity(255);
    }
    else{
        _mission_star_1->setOpacity(80);
    }
    
    if(mTotalPointsInGame>=_mission_star_points_2){
        _mission_star_2->setOpacity(255);
    }
    else{
        _mission_star_2->setOpacity(80);
    }
    
    if(mTotalPointsInGame>=_mission_star_points_3){
        _mission_star_3->setOpacity(255);
    }
    else{
        _mission_star_3->setOpacity(80);
    }
    
    //Check the points for each bar
    int aDummyPoints = mTotalPointsInGame;
    if(aDummyPoints>_mission_star_points_1){
        aDummyPoints = _mission_star_points_1;
    }
    _mission_progress_bar_1->setTextureRect(CCRect(0, 0,
                                                   _mission_progress_bar_1->getTexture()->getContentSize().width*((float)aDummyPoints / _mission_star_points_1),
                                                   _mission_progress_bar_1->getTexture()->getContentSize().height));
    
    aDummyPoints = mTotalPointsInGame-_mission_star_points_1;
    if(aDummyPoints<0){
        aDummyPoints = 0;
    }
    else if(aDummyPoints>_mission_star_points_2-_mission_star_points_1){
        aDummyPoints = _mission_star_points_2-_mission_star_points_1;
    }
    
    _mission_progress_bar_2->setTextureRect(CCRect(0, 0,
                                                   _mission_progress_bar_2->getTexture()->getContentSize().width*
                                                   ((float)aDummyPoints / (_mission_star_points_2-_mission_star_points_1)),
                                                   _mission_progress_bar_2->getTexture()->getContentSize().height));
    
    aDummyPoints = mTotalPointsInGame-_mission_star_points_2;
    if(aDummyPoints<0){
        aDummyPoints = 0;
    }
    else if(aDummyPoints>_mission_star_points_3-_mission_star_points_2){
        aDummyPoints = _mission_star_points_3-_mission_star_points_2;
    }
    
    _mission_progress_bar_3->setTextureRect(CCRect(0, 0,
                                                   _mission_progress_bar_3->getTexture()->getContentSize().width*
                                                   ((float)aDummyPoints / (_mission_star_points_3-_mission_star_points_2)),
                                                   _mission_progress_bar_3->getTexture()->getContentSize().height));
    
    aDummyPoints = mTotalPointsInGame-_mission_star_points_3;
    if(aDummyPoints<0){
        aDummyPoints = 0;
    }
    else if(aDummyPoints>(_mission_star_points_3+100)-_mission_star_points_3){
        aDummyPoints = (_mission_star_points_3+100)-_mission_star_points_3;
    }
    
    _mission_progress_bar_4->setTextureRect(CCRect(0, 0,
                                                   _mission_progress_bar_4->getTexture()->getContentSize().width*
                                                   ((float)aDummyPoints / ((_mission_star_points_3+100)-_mission_star_points_3)),
                                                   _mission_progress_bar_4->getTexture()->getContentSize().height));
    
    
    
    
    //Set the progress bar
//    _mission_progress_bar->setTextureRect(CCRect(0, 0,
//                                                 _mission_progress_bar->getTexture()->getContentSize().width*((float)mTotalPointsInGame / mMaxBarPoints),
//                                                 _mission_progress_bar->getTexture()->getContentSize().height));
}

void GameScene::OnResumeFromDaily()
{
//    resumeSchedulerAndActionsRecursive(this);
    
    _DailySpinVisible = false;
    CCLayerColor* aBlackBG = (CCLayerColor*)getChildByTag(7777);
    
    CCFadeTo* aFade1 = CCFadeTo::create(0.5f,0);
    CCCallFuncN* aF1_func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCSequence* aSe1 = CCSequence::create(aFade1,aF1_func,NULL);
    aBlackBG->runAction(aSe1);
    
    CraeteHUD();
    
    _gamePause = false;
}

//This will jump around when someyhing done !!!
void GameScene::CreateInGamePopup()
{
    mPopUpShowActive = false;
    mPopUpPanel = CCSprite::create("Interfeiss/panel_pop_new.png");
    mPopUpPanel->setPosition(ccp(_screenSize.width/2,-100));
    
    //Add the text field !!!
    mPopUpText = CCLabelTTF::create("Test",
                                           "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.4,
                                           CCSize(400, 80), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//    mPopUpText->enableStroke(ccBLACK, 0.5f,true);
    
    mPopUpText->setAnchorPoint(ccp(0.5,0.5f));
    mPopUpText->setColor(ccc3(79, 65, 33));
    mPopUpText->setPositionX(mPopUpPanel->getContentSize().width/2);//This can change by reward type
    mPopUpText->setPositionY(mPopUpPanel->getContentSize().height/2);
    mPopUpPanel->addChild(mPopUpText);
    
    addChild(mPopUpPanel,kHUD_Z_Order-1);
}

void GameScene::PlaySpecialMusic(int theDwarfAmount)
{
    //Check what slide to fade in !!!
//    SimpleAudioEngine::sharedEngine()->get
    
    mIntroPlaying = true;
    mFadeOutMusic = true;//!!!
    
    return;
    
    //The old functional !!!
//    music_intro
    
    int aCurrentMusicID = 1;
    if(theDwarfAmount<4)
        aCurrentMusicID = 1;
    else if(theDwarfAmount<7)
        aCurrentMusicID = 2;
    else if(theDwarfAmount<9)
        aCurrentMusicID = 3;
    else
        aCurrentMusicID = 4;
    
    CCLog("Play music [aCurrentMusicID]:%i",aCurrentMusicID);
    
    //Check if the current music is not the current needed already!!!
    if(mCurrentMusicID == aCurrentMusicID)
        return;//No need
    
    if(CCUserDefault::sharedUserDefault()->getBoolForKey("musicEnabled", true) == false)
        return;
    
    //Lets add some cross fade !!!
    mFadeOutMusic = true;//!!!
    
    mCurrentMusicID = aCurrentMusicID;
    CCLog("Play music +++[mCurrentMusicID]:%i",mCurrentMusicID);
    
}

void GameScene::OnFinishedMusicFadeOut()
{
    //For now disabled - fmod stuff check
//    return;
    if(mFmodEnabled)
        return;
    
    mBackMusicVolume = 0;
    
    std::stringstream theMusicPath;
    
    //Lets use mp3 for all !!! :D
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    theMusicPath << "music/music_GriegLoop.ogg";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    theMusicPath << "music/music_GriegLoop.aifc";
#endif
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(theMusicPath.str().c_str(), true);
    
    mFadeOutMusic = false;
    mFadeInMusic = true;
}

void GameScene::AddPopUpToShow(int theRewardType,int theRewardAmount,const char* theRewardText)
{
    PopupShowInfo aInfo = *new PopupShowInfo();
    aInfo.reward_type = theRewardType;
    aInfo.reward_amount = theRewardAmount;
    aInfo.reward_text = theRewardText;
    
    popups.push_back(aInfo);
    
    if(!mPopUpShowActive)
        PopUpShowNext();
}

void GameScene::PopUpFinishedShow(CCNode* sender)
{
    //Get the next one to show
    mPopUpShowActive = false;
    
    if(popups.size()>0)
        PopUpShowNext();
}

void GameScene::PopUpShowNext()
{
    if(popups.size()==0)
        return;//Nothing here !!!
    
    //We are starting to show something new !!!
    mPopUpShowActive = true;
    PopupShowInfo aWind = popups.back();
    popups.pop_back();
    
    std::stringstream theTextField;
    theTextField << aWind.reward_text;
    mPopUpText->setString(theTextField.str().c_str());
    
    //Remove not needed crap !!!
    if(mPopUpPanel->getChildByTag(100))
        mPopUpPanel->removeChildByTag(100);
    
    if(mPopUpPanel->getChildByTag(101))
        mPopUpPanel->removeChildByTag(101);
    
    //Check text positions
    if(aWind.reward_type>0)
    {
        //Create the reward icon too !!!
        mPopUpText->setPosition(ccp(mPopUpPanel->getContentSize().width/2+10,mPopUpPanel->getContentSize().height/2));
        
        CCSprite* aReward = CCSprite::create("Interfeiss/upgrade_screen/diamond_upgrade.png");
        aReward->setTag(100);
        aReward->setPosition(ccp(70,mPopUpPanel->getContentSize().height/2));
        mPopUpPanel->addChild(aReward);
        
        //The text field
        std::stringstream theRewardAmount;
        theRewardAmount <<"+"<< aWind.reward_amount;
        CCLabelTTF* aRewardAmount = CCLabelTTF::create(theRewardAmount.str().c_str(),
                                        "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.6,
                                        CCSize(70, 80), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
        //    aRewardAmount->enableStroke(ccBLACK, 0.5f,true);
        
        aRewardAmount->setAnchorPoint(ccp(1.0f,0.5f));
        aRewardAmount->setColor(ccc3(255, 255, 255));
        aRewardAmount->setPositionX(-4);//This can change by reward type
        aRewardAmount->setPositionY(aReward->getContentSize().height/2);
        aReward->addChild(aRewardAmount);
    }
    else
    {
        //Nothing just place in center !!!
        mPopUpText->setPosition(ccp(mPopUpPanel->getContentSize().width/2-20,mPopUpPanel->getContentSize().height/2));
    }
    
    //Add the animation frame !!!
    CCDelayTime* aStartDelay = CCDelayTime::create(0.1f);
    CCMoveTo* aMoveUp;
    if(dailyChallengeInfo!=NULL)
    {
        aMoveUp = CCMoveTo::create(0.5f,ccp(_screenSize.width/2,100));
    }
    else
    {
        aMoveUp = CCMoveTo::create(0.5f,ccp(_screenSize.width/2,50));
    }
    
    CCEaseBackOut* aEaseUp = CCEaseBackOut::create(aMoveUp);
    
    CCCallFuncN* aFuncDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::AddCheckDoneOnPopUp));
    
    CCDelayTime* aShowDelay = CCDelayTime::create(1.5f);
    
    CCMoveTo* aMoveDown = CCMoveTo::create(0.5f,ccp(_screenSize.width/2,-100));
    CCEaseBackIn* aEaseDown = CCEaseBackIn::create(aMoveDown);
    CCCallFuncN* aFunc = CCCallFuncN::create(this, callfuncN_selector(GameScene::PopUpFinishedShow));
    
    CCSequence* aSeq = CCSequence::create(aStartDelay,aEaseUp,aFuncDone,aShowDelay,aEaseDown,aFunc,NULL);
//    CCSequence* aSeq = CCSequence::create(aStartDelay,aEaseUp,aFuncDone,aShowDelay,NULL);
    mPopUpPanel->runAction(aSeq);
    
    //Check if we need to start some animations or reward fly !!!
    
}

void GameScene::AddCheckDoneOnPopUp(CCNode* sender)
{
    SpriteAnimation* _crashAnimation = SpriteAnimation::create("Interfeiss/challenges/ingame_pop_up/check.plist",false);
    _crashAnimation->retain();
    _crashAnimation->setTag(101);
    
    _crashAnimation->setPosition(ccp(sender->getContentSize().width-20,sender->getContentSize().height/2+16));
    sender->addChild(_crashAnimation);
}

void GameScene::CreateDebugPanel()
{
//    if(User::getInstance()->getDebugButtonsState()==0)
//        return;
    
    
    //Debug buttons
    CCMenuItemImage* debug_button_1 = CCMenuItemImage::create(
                                                              "DebugStuff/Debug_Rain.png",
                                                              "DebugStuff/Debug_Rain.png",
                                                              this,
                                                              menu_selector(GameScene::StartRainDebug));
    debug_button_1->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* debug_button_2 = CCMenuItemImage::create(
                                                              "DebugStuff/Debug_Wind.png",
                                                              "DebugStuff/Debug_Wind.png",
                                                              this,
                                                              menu_selector(GameScene::StartWindDebug));
    debug_button_2->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* debug_button_3 = CCMenuItemImage::create(
                                                              "DebugStuff/Debug_Crystal.png",
                                                              "DebugStuff/Debug_Crystal.png",
                                                              this,
                                                              menu_selector(GameScene::DebugCreateCrystal));
    debug_button_3->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* debug_button_4 = CCMenuItemImage::create(
                                                              "DebugStuff/Debug_Diamond.png",
                                                              "DebugStuff/Debug_Diamond.png",
                                                              this,
                                                              menu_selector(GameScene::generateDiamond));
    debug_button_4->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* debug_button_5 = CCMenuItemImage::create(
                                                              "DebugStuff/Debug_Dwarf.png",
                                                              "DebugStuff/Debug_Dwarf.png",
                                                              this,
                                                              menu_selector(GameScene::CreateDwarfDebug));
    debug_button_5->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* debug_button_6 = CCMenuItemImage::create(
                                                              "DebugStuff/Debug_Mushroom.png",
                                                              "DebugStuff/Debug_Mushroom.png",
                                                              this,
                                                              menu_selector(GameScene::generateMushroom));
    debug_button_6->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* debug_button_7 = CCMenuItemImage::create(
                                                              "DebugStuff/Debug_Troll.png",
                                                              "DebugStuff/Debug_Troll.png",
                                                              this,
                                                              menu_selector(GameScene::generateTroll));
    debug_button_7->setAnchorPoint(ccp(0,0));
    
    CCMenuItemImage* debug_button_8 = CCMenuItemImage::create(
                                                              "DebugStuff/Debug_Effect.png",
                                                              "DebugStuff/Debug_Effect.png",
                                                              this,
                                                              menu_selector(GameScene::CreateSpecialSpot));
    debug_button_8->setAnchorPoint(ccp(0,0));
    
    CCMenu* debugMenu = CCMenu::create(debug_button_1,debug_button_2,debug_button_3,debug_button_4,debug_button_5,debug_button_6,
                                       debug_button_7,debug_button_8,NULL);
    
    debugMenu->alignItemsHorizontallyWithPadding(10);
    debugMenu->setPosition(ccp(_screenSize.width/2,20));
    this->addChild(debugMenu, kHUD_Z_Order-1);
}

void GameScene::DebugCreateCrystal()
{
//    generateCrystal(false);
}

//The tornado stuff !!!
void GameScene::CreateTornadoThrow(Dwarf* theDwarf,Troll* theTroll,int theStartX,int theStartY)
{
    WindObject *theObject = new WindObject();
    CCSprite* aThrow = CCSprite::create("small_dot_red.png");;
    CCSprite* aLandAnim;
    CCPoint aOffset;
    
    if(theDwarf!=NULL)
    {
        SpriteAnimation* _crashAnimation = SpriteAnimation::create("effects/crash_stars.plist");
        _crashAnimation->retain();
        _crashAnimation->setOpacity(0);
        
        CCDelayTime* aAnimation = CCDelayTime::create(0.5f);
        CCFadeTo* aFadeIn = CCFadeTo::create(0.1f,255);
        CCSequence* aSeq1 = CCSequence::create(aAnimation,aFadeIn,NULL);
        _crashAnimation->runAction(aSeq1);
        
        theObject->haveDwarf = 1;
        theObject->theDwarf = theDwarf;
        theDwarf->setVisible(false);
        
        if(theDwarf->getType() == DWARF_TYPE_FAT)
        {
            aThrow = CCSprite::create("effects/wind/fatass_fly.png");
            aLandAnim = CCSprite::create("Characters/fat_dwarf/fat_dwarf_dead.png");
            aOffset = ccp(-26,-24);//-24
            _crashAnimation->setPosition(ccp(aLandAnim->getContentSize().width/2+4,aLandAnim->getContentSize().height/2+35));
        }
        else
        {
            aThrow = CCSprite::create("effects/wind/tallass_fly.png");
            aLandAnim = CCSprite::create("Characters/tall_dwarf/tall_dwarf_dead.png");
            aOffset = ccp(-5,-14);//-14
            _crashAnimation->setPosition(ccp(aLandAnim->getContentSize().width/2+4,aLandAnim->getContentSize().height/2+35));
        }
        
        aLandAnim->addChild(_crashAnimation);
        
        theDwarf->_knockOut = true;
        theDwarf->setDisabled(false);
    }
    if(theTroll!=NULL)
    {
        theObject->haveDwarf = 0;
        theObject->theTroll = theTroll;
    }
    
    theObject->x = theStartX;
    theObject->y = theStartY;
    
    mThrowObjecIndex++;
    theObject->ID = mThrowObjecIndex;
    
    
    aThrow->setTag(mThrowObjecIndex);
    aThrow->setScale(GLOBAL_SCALE);
    aThrow->setOpacity(0);
    aThrow->setPosition(ccp(theStartX,theStartY));
    addChild(aThrow,getSpriteOrderZ(theStartY));
    
    
    aLandAnim->setTag(2);
    aLandAnim->setPosition(aOffset);
    aLandAnim->setAnchorPoint(ccp(0,0));
//    aLandAnim->setScale(GLOBAL_SCALE);
    aLandAnim->setOpacity(0);
    aThrow->addChild(aLandAnim);
    
//    return;
    
    CCDelayTime* aFinDelay = CCDelayTime::create(0.75f);
    CCFadeTo* aFinFade = CCFadeTo::create(0.0f,255);
    //Maybe add stars
    CCSequence* aFinSeq = CCSequence::create(aFinDelay,aFinFade,NULL);
    aLandAnim->runAction(aFinSeq);
    
    //now create the action when he fly
//    CCDelayTime* aTime = CCDelayTime::create(1.6f);
    CCCallFuncN* aFunc = CCCallFuncN::create(this, callfuncN_selector(GameScene::RemoveActiveFlyObject));
    
    CCRotateBy* aRotate = CCRotateBy::create(0.75f,720.0f);
//    CCMoveTo* aMove = CCMoveTo::create(1.0f,ccp(300,300));
    
    ccBezierConfig bezier;
    if(theStartX>_screenSize.width/2)
    {
        bezier.controlPoint_1 = ccp(theStartX, theStartY+100);//1096,168
        bezier.controlPoint_2 = ccp(theStartX-50, theStartY+100);//635,105
        bezier.endPosition = ccp(theStartX-50, theStartY-50);
    }
    else
    {
//        bezier.controlPoint_1 = ccp(200, 300);//1096,168
//        bezier.controlPoint_2 = ccp(300, 300);//635,105
//        bezier.endPosition = ccp(300, 100);
        bezier.controlPoint_1 = ccp(theStartX, theStartY+100);//1096,168
        bezier.controlPoint_2 = ccp(theStartX+50, theStartY+100);//635,105
        bezier.endPosition = ccp(theStartX+50, theStartY-50);
    }
    
    CCBezierTo* aToPos_1a = CCBezierTo::create(0.75f, bezier);
    
//    CCEaseBounceOut* aBounce = CCEaseBounceOut::create(aToPos_1a); //Fail
//    CCEaseSineOut* aBounce = CCEaseSineOut::create(aToPos_1a); //Fail
    
    CCFadeIn* aFadIn = CCFadeIn::create(0.25f);
    CCDelayTime* aDelay = CCDelayTime::create(1.5f);
    CCFadeTo* aFade = CCFadeTo::create(0.0f,0);
    CCSpawn* aSpawn = CCSpawn::create(aFadIn,aRotate,aToPos_1a,NULL);
    
    CCSequence* aSeq = CCSequence::create(aSpawn,aFade,aDelay,aFunc,NULL);
    aThrow->runAction(aSeq);
    
    //Calculate where will it drop and to what side !!!
    _flyObjects->addObject((CCObject*)theObject);
//    _flyObjects.push_back(theObject);
}



void GameScene::RemoveActiveFlyObject(CCNode* sender)
{
//    return;
    WindObject *aWind;
    //Check where was this dwarf
//    for(int i;i>_flyObjects->count();i--)
    for (int i = _flyObjects->count() - 1; i >= 0; --i)
    {
        aWind = (WindObject*)(_flyObjects->objectAtIndex(i));
//        aWind = &_flyObjects[i];
        if(aWind->ID == sender->getTag())
        {
            //We found it - enable dwarf back !!!
            if(aWind->haveDwarf>0)
            {
                aWind->theDwarf->setAnimationVisibility(true);
                aWind->theDwarf->setVisible(true);
                aWind->theDwarf->setDisabled(false);
                aWind->theDwarf->_knockOut = false;
                
                aWind->theDwarf->setPosition(ccp(sender->getPositionX(),sender->getPositionY()));
            }
//            else
            {
                
            }
            removeChild(sender);
            //Clear it !!!
            _flyObjects->removeObjectAtIndex(i);
//            _flyObjects.erase(_flyObjects.begin()+i);
            break;
        }
    }
}

void GameScene::CreateMissionStuff()
{
    User::getInstance()->getMissionManager()._gameScene = this;
    
    //Set default values or load some?
    mDwarfsInCaveOneGame = 0;
    mFatDwarfsInRow = 0;
    
    //Reset some mission current amount !!!
    User::getInstance()->getMissionManager().ResetCurrentStates();
    
    User::getInstance()->getMissionManager().CheckForStuckMission();
}

//Some actions for machines !!!
void GameScene::OnMachineStamp(cocos2d::CCObject *sender)
{
//    CCLog("Clicked Machine stamp");
    
    if(mTutorialEnabled || mCurrentSmallTutorial!=-1)
        return;//for now disabled
    
    //Check if this machine can be clicked !!!
    Machine_Stamp* aMachine = static_cast<Machine_Stamp*>(getChildByTag(1010));
    
    if(aMachine->mUpgradeLevel==0)
    {
        return;//Machine is not ready - do you want to enable it???
    }
    
    if(aMachine->mCanBeActivated)
    {
        //Check if has money - if not - ask to buy? how will this work in game?
        int aPrice = BOOSTER_1_PRICE;
        if(_willUseMushroom)
            aPrice = 0;
        
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            return;
        }
        
        AnalyticX::flurryLogEvent("Game Play: Activate Stamp");
        mFlurry_StampActivated+=1;
        
        if(aPrice>0)
            mFlurry_MachinesActivated++;
        
        _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
        
        aMachine->SetAnimation_Use();
        
        playInGameSound("machine_stamp");
        
        //
        //Update subtask
        if(!_willUseMushroom)
        {
            User::getInstance()->getMissionManager().CheckSubMission(SUB_ACTIVATE_POWER_ANY,1);
            User::getInstance()->getMissionManager().CheckSubMission(SUB_ACTIVATE_POWER_BOOM,1);
        }
    }
}

//Some actions for machines !!!
void GameScene::OnMachineGhost(cocos2d::CCObject *sender)
{
    if(mTutorialEnabled)
        return;//for now disabled
    
    if(mCurrentSmallTutorial!=-1)
        return;
    
    //Check if this machine can be clicked !!!
    Machine_Ghost* aMachine = static_cast<Machine_Ghost*>(getChildByTag(1020));
    
    if(aMachine->mUpgradeLevel==0)
    {
        //Maybe create some popup?
        return;//Machine is not ready - do you want to enable it???
    }
    
    if(aMachine->mCanBeActivated)
    {
        //Check if has money - if not - ask to buy? how will this work in game?
        int aPrice = BOOSTER_1_PRICE;
        if(_willUseMushroom)
            aPrice = 0;
        
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            
            return;
        }
        
        AnalyticX::flurryLogEvent("Game Play: Activate Ghost");
        mFlurry_GhostActivated+=1;
        
        if(aPrice>0)
            mFlurry_MachinesActivated++;
        
        _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
        
        switch (aMachine->mUpgradeLevel) {
            case 1:_boostGhostTimer = 10;break;
            case 2:_boostGhostTimer = 15;break;
            case 3:_boostGhostTimer = 20;break;
            case 4:_boostGhostTimer = 25;break;
            case 5:_boostGhostTimer = 30;break;
            default:
                break;
        }
        
        aMachine->SetAnimation_Use(_boostGhostTimer);
        
        CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/GhoustActivate.plist");
        p->setPosition(aMachine->getPositionX(), aMachine->getPositionY());
        p->setAutoRemoveOnFinish(true);
        addChild(p,kPoints_Z_Order-1);
        
        removeAllEffects();
        
        playInGameSound("machine_ghost");
        
        //Update subtask
        if(!_willUseMushroom)
        {
            User::getInstance()->getMissionManager().CheckSubMission(SUB_ACTIVATE_POWER_ANY,1);
            User::getInstance()->getMissionManager().CheckSubMission(SUB_ACTIVATE_POWER_GHOST,1);
        }
    }
}

//Troll* GameScene::generateTroll(bool theSkip)
//CCSprite* maskedSpriteWithSprite:(CCSprite *)textureSprite maskSprite:(CCSprite *)maskSprite
CCSprite* GameScene::maskedSpriteWithSprite(CCSprite* textureSprite,CCSprite* maskSprite,int theX,int theY)
{
    // 1
//    CCRenderTexture * rt = [CCRenderTexture renderTextureWithWidth:maskSprite.contentSizeInPixels.width height:maskSprite.contentSizeInPixels.height];
    CCRenderTexture* rt = CCRenderTexture::create(maskSprite->getContentSize().width,maskSprite->getContentSize().height);
    
    // 2
//    maskSprite.position = ccp(maskSprite.contentSize.width/2, maskSprite.contentSize.height/2);
//    textureSprite.position = ccp(textureSprite.contentSize.width/2, textureSprite.contentSize.height/2);
    
    maskSprite->setPosition(ccp(maskSprite->getContentSize().width/2,maskSprite->getContentSize().height/2));
//    textureSprite->setPosition(ccp(textureSprite->getContentSize().width/2,textureSprite->getContentSize().height/2));
    textureSprite->setPosition(ccp(theX,theY));
    
    // 3
//    [maskSprite setBlendFunc:(ccBlendFunc){GL_ONE, GL_ZERO}];
//    [textureSprite setBlendFunc:(ccBlendFunc){GL_DST_ALPHA, GL_ZERO}];
    
    ccBlendFunc* someBlend = new ccBlendFunc();
    someBlend->src = GL_ONE;
    someBlend->dst = GL_ZERO;
    maskSprite->setBlendFunc(*someBlend);
    
    ccBlendFunc* someBlend2 = new ccBlendFunc();
    someBlend2->src = GL_DST_ALPHA;
    someBlend2->dst = GL_ZERO;
    textureSprite->setBlendFunc(*someBlend2);
    
    // 4
//    [rt begin];
//    [maskSprite visit];
//    [textureSprite visit];
//    [rt end];
    
    rt->begin();
    maskSprite->visit();
    textureSprite->visit();
    rt->end();
    
    // 5
//    CCSprite *retval = [CCSprite spriteWithTexture:rt.sprite.texture];
//    retval.flipY = YES;
    
    CCSprite* retval = CCSprite::createWithTexture(rt->getSprite()->getTexture());
    retval->setFlipY(true);
    return retval;
    
//    ccBlendFunc* someBlend = new ccBlendFunc();
//    someBlend->src = GL_ONE;//OGLES.GL_ONE;
//    someBlend->dst = GL_ONE;//OGLES.GL_ONE;
//    _animation->setBlendFunc(*someBlend);// .BlendFunc = someBlend;
    
}

bool GameScene::CanZipTroll()
{
    if(User::getInstance()->mSpecial_16_Mission==false && User::getInstance()->mSpecial_17_Mission==false
       && User::getInstance()->mSpecial_18_Mission==false && User::getInstance()->mSpecial_19_Mission==false
       && User::getInstance()->mSpecial_20_Mission==false && User::getInstance()->mSpecial_21_Mission==false
       && User::getInstance()->mSpecial_22_Mission==false && User::getInstance()->mSpecial_23_Mission==false){
        return false;
    }
    
    Machine_Enemies* aMachine = static_cast<Machine_Enemies*>(getChildByTag(1030));
    //Is it active ???
    if(aMachine->mCanBeActivated == false){
        return true;
    }
    
    return false;
}

//Some actions for machines !!!
void GameScene::OnMachineEnemies(cocos2d::CCObject *sender)
{
    if(mCurrentSmallTutorial!=-1 && mCurrentSmallTutorial!=6)
        return;
    
    //Check if this machine can be clicked !!!
    Machine_Enemies* aMachine = static_cast<Machine_Enemies*>(getChildByTag(1030));
    
    if(mTutorialEnabled)
    {
        //Allow only when needed
        if(mTutorialStep!=33)
            return;
        
        OnTutorialStepCompleted(33);
        
        //This will be for free !!!
        _boostNoEnemyTimer = 10;
        
        _actionTrollSpawnTime = -1;
        aMachine->SetAnimation_Use(_boostNoEnemyTimer);
        removeAllEffects();
        
        return;
    }
    
    if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission
       || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
       || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission)
    {
        //Check how much charges do you have !!!
        if(mChargesForBlitz>0)
        {
            if(aMachine->mCanBeActivated == false){
                return;
            }
            
            //All cool
            mChargesForBlitz-=1;
            
//            _boostNoEnemyTimer = 10;
            
            std::stringstream aStringValue;
            aStringValue<<mChargesForBlitz;
            
            mCharges_Electro->setString(aStringValue.str().c_str());
            
            //The charhe amount !!!
            _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
            
            aMachine->SetAnimation_Use(10);
            //Create lightning here !!!
//            removeAllEffects();
            showRemovableEffects();
        }
        
        return;
    }
    
    if(aMachine->mUpgradeLevel==0)
    {
        return;//Machine is not ready - do you want to enable it???
    }
    
    if(aMachine->mCanBeActivated)
    {
        //Check if has money - if not - ask to buy? how will this work in game?
        int aPrice = BOOSTER_1_PRICE;
        if(_willUseMushroom)
            aPrice = 0;
        
        int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
        if(aDidUseDiamonds<0)
        {
            //Show popup that no money
            
            return;
        }
        
        AnalyticX::flurryLogEvent("Game Play: Activate No Enemy");
        mFlurry_BlitzActivated+=1;
        
        if(aPrice>0)
            mFlurry_MachinesActivated++;
        
        _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
        
        switch (aMachine->mUpgradeLevel) {
            case 1:_boostNoEnemyTimer = 10;break;
            case 2:_boostNoEnemyTimer = 15;break;
            case 3:_boostNoEnemyTimer = 20;break;
            case 4:_boostNoEnemyTimer = 25;break;
            case 5:_boostNoEnemyTimer = 30;break;
            default:
                break;
        }
        
        _actionTrollSpawnTime = -1;
        
        aMachine->SetAnimation_Use(_boostNoEnemyTimer);
        
        //Create lightning here !!!
        removeAllEffects();
        
        if(!_willUseMushroom)
        {
            User::getInstance()->getMissionManager().CheckSubMission(SUB_ACTIVATE_POWER_ANY,1);
            User::getInstance()->getMissionManager().CheckSubMission(SUB_ACTIVATE_POWER_BLITZ,1);
        }
        
        if(mCurrentSmallTutorial == 6)
        {
            RemoveSmallTutorial();
        }
    }
}

void GameScene::CreateTrollIndicator(int theType)
{
    if(mMasterTroll->getChildByTag(12345)!=NULL){
        mMasterTroll->removeChildByTag(12345);
    }
    
    //Add some indicator above troll what will he do now !!!
    CCSprite* aDummyIcon;//CCSprite::create("");
    
    CCSprite* aBaseForIcon = CCSprite::create("effects/Troll_Info.png");
    aBaseForIcon->setPosition(ccp(0,90));
    aBaseForIcon->setTag(12345);
    
    if(theType == 1)
    {
        aDummyIcon = CCSprite::create("effects/troll.png");
        aDummyIcon->setScale(0.4f);
        aDummyIcon->setPosition(ccp(aBaseForIcon->getContentSize().width/2,aBaseForIcon->getContentSize().height/2+6));
    }
    else if(theType == 2)
    {
        aDummyIcon = CCSprite::create("effects/dwarf_crash_boom.png");
        aDummyIcon->setScale(0.4f);
        aDummyIcon->setPosition(ccp(aBaseForIcon->getContentSize().width/2,aBaseForIcon->getContentSize().height/2+6));
    }
    else if(theType == 3)
    {
        aDummyIcon = CCSprite::create("ice_barrage/state_2.png");
        aDummyIcon->setScale(0.4f);
        aDummyIcon->setPosition(ccp(aBaseForIcon->getContentSize().width/2,aBaseForIcon->getContentSize().height/2+6));
    }
    
    aBaseForIcon->addChild(aDummyIcon);
    aBaseForIcon->setScale(0);
    
    CCScaleTo* aScale = CCScaleTo::create(0.5f,1.0f);
    CCEaseElasticOut* aEase = CCEaseElasticOut::create(aScale);
    aBaseForIcon->runAction(aEase);
    
    mMasterTroll->addChild(aBaseForIcon);
}

void GameScene::SetMasterTrollAction(int theType)
{
    SetMasterTrollAnimation(_MasterTroll_WinAnimation);
    
    //Troll incoming
    if(theType == 6){
        CreateTrollIndicator(2);
    }
    else if(theType == 7){
        CreateTrollIndicator(3);
    }
    else{
        CreateTrollIndicator(1);
    }
    
    
    //The troll spawn
    if(theType == 1)
    {
        generateTrollMission(_screenSize.width/2,_screenSize.height-180,1.6,false,true,
                             ccp(240,_screenSize.height-180),ccp(_screenSize.width-240,_screenSize.height-180),1);
    }
    else if(theType == 2)
    {
        generateTrollMission(_screenSize.width/2,160,1.6,false,true,
                             ccp(240,160),ccp(_screenSize.width-240,160),2);
    }
    else if(theType == 3)
    {
        generateTrollMission(240,_screenSize.height/2,1.6,false,true,ccp(240,_screenSize.height-100),ccp(240,100),3);
    }
    else if(theType == 4)
    {
        generateTrollMission(_screenSize.width-240,_screenSize.height/2,1.6,false,true,
                             ccp(_screenSize.width-240,_screenSize.height-100),ccp(_screenSize.width-240,100),4);
    }
    else if(theType == 5)
    {
        generateTrollMission(_screenSize.width/2,200,0,true,false,CCPointZero,CCPointZero,5);
    }
    else if(theType == 7)
    {
        //First create indicator which dwarf will freeze
        StartDwarfFreeze();
    }
    else if(theType == 8)
    {
        //Get some dwarf and fire bullet to it
        BulletDwarf();
    }
    else
    {
        //Generate meteorite !!!
        generateEffect();
    }
    
    //Call idle after some time !!!
    SetMasterTrollIdleAfterTime(3.0f);
}

void GameScene::BulletDwarf()
{
    mMasterTroll_Bullets-=1;
    
    Dwarf* dwarf;
    cocos2d::CCArray* _dwarvesToAttack = CCArray::create();
	_dwarvesToAttack->retain();
    
    //Remove all dwarfs
    for (int dwarfIndex = _dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
	{
        dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(dwarfIndex));
		
		if (!dwarf->mBulletActive && dwarf->getEffect()==NULL && !dwarf->_knockOut && dwarf->getDisabled()==false)
        {
            //This can be choosen to attack !!!
            _dwarvesToAttack->addObject(dwarf);
        }
    }
    
    if(_dwarvesToAttack->count() == 0){
        return;//No luck
    }
    
    //Now choose !!!
    int aRanodmDwarf = rand()%_dwarvesToAttack->count();
//    CCLOG("Bullet aRanodmDwarf 1: %f",aRanodmDwarf);
    dwarf = static_cast<Dwarf*>(_dwarvesToAttack->objectAtIndex(aRanodmDwarf));
    
    dwarf->mBulletActive = true;
    
    TrollBullet* aBullet = TrollBullet::create(this);
    aBullet->setPosition(mMasterTroll->getPositionX(),mMasterTroll->getPositionY());
    aBullet->_dwarf = dwarf;
    
    this->addChild(aBullet, 1000);
    _bullets->addObject(aBullet);
    
//    CCLOG("Bullet amount 1: %f",_bullets->count());
    
    //clear the arr !!!
    _dwarvesToAttack->release();
}

void GameScene::UpdateBullets(float delta)
{
//    CCLOG("Bullet amount 2: %f",_bullets->count());
    
    // update trolls
	for (int trollIndex = _bullets->count() - 1; trollIndex >= 0; --trollIndex)
	{
		TrollBullet* troll = static_cast<TrollBullet*>(_bullets->objectAtIndex(trollIndex));
		
        if(!troll->_isDisabled)
        {
            troll->update(delta);
            
//            if (getSpriteOrderZ(troll->getPositionY())!=troll->getZOrder())
//                reorderChild(troll, getSpriteOrderZ(troll->getPositionY()));
        }
        else
        {
            //Create some particles and sound !!!
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/bullet_explode.plist");
            p->setPosition(troll->getPositionX(), troll->getPositionY());
            p->setAutoRemoveOnFinish(true);
            addChild(p,1000);
            
            //Remove it !!!
            this->removeChild(troll);
            _bullets->removeObjectAtIndex(trollIndex);
            CCLOG("Removed Bullet");
            troll = NULL;
        }
        
        //Check if does not hit other dwarf !!!
        if(troll)
        {
            for (int dwarfIndex = _dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
            {
                Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(dwarfIndex));
                
                //**********************************
                //Simple z-sorting =D
                
                if (dwarf)
                {
                    if (ccpDistanceSQ(dwarf->getPosition(), troll->getPosition()) <= 1000)
                    {
                        //gameover for other dwarf !!!
                        troll->_isDisabled = true;
                        dwarf->_knockOutTime = 3;
                        dwarf->_knockOut = true;
                        dwarf->createCrash();
                    }
                }
            }
        }
	}
}

void GameScene::StartDwarfFreeze()
{
    //Choose any dwarf !!!
    int aRanodmDwarf = rand()%_dwarves->count();
    
    //need to check what dwarf can bee frozen !!! - todo !!!
    Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(aRanodmDwarf));
    
    //Craete the freeze target stuff
    dwarf->_TargetIcon->setVisible(true);
    
    CCDelayTime* aDelay = CCDelayTime::create(1.0f);
    CCCallFuncN* aFunc1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::FreezeDwarfTotal));
    CCSequence* aTrollSeq = CCSequence::create(aDelay,aFunc1,NULL);
    
    dwarf->runAction(aTrollSeq);
}

void GameScene::FreezeDwarfTotal(cocos2d::CCObject *sender)
{
    Dwarf* dwarf = static_cast<Dwarf*>(sender);
    dwarf->_TargetIcon->setVisible(false);
    
    //Freeze
    Effect* effect = NULL;
    effect = IceBarrage::create(this);
    effect->setPosition(ccp(dwarf->getPositionX(),dwarf->getPositionY()));
//    this->addChild(effect,0);
//    _effects->addObject(effect);
    
    effect->touch(dwarf,NULL);
    effect->setVisible(true);
    
//    dwarf->setEffect(effect);
    dwarf->pauseAnimation();
}

void GameScene::SetMasterTrollIdleAfterTime(float theTime)
{
    CCDelayTime* aDelay = CCDelayTime::create(theTime);
    CCCallFuncN* aFunc1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::SetMasterTrollAnimIdle));
    CCSequence* aTrollSeq = CCSequence::create(aDelay,aFunc1,NULL);
    mMasterTroll->runAction(aTrollSeq);
}

void GameScene::SetMasterTrollAnimIdle()
{
    SetMasterTrollAnimation(_MasterTroll_IdleAnimation);
    _MasterTroll_animation->pause();
    
    if(mMasterTroll->getChildByTag(12345)!=NULL){
        mMasterTroll->removeChildByTag(12345);
    }
}

//void Troll::setAnimation(SpriteAnimation* animation)
void GameScene::SetMasterTrollAnimation(SpriteAnimation* animation)
{
	if (_MasterTroll_animation != animation)
	{
		if (_MasterTroll_animation)
		{
//            mMasterTroll->removeChild(_MasterTroll_animation);
            mMasterTroll->cocos2d::CCNode::removeChild(_MasterTroll_animation);
		}
		
		_MasterTroll_animation = animation;
		
		if (_MasterTroll_animation)
		{
			mMasterTroll->addChild(_MasterTroll_animation);
		}
	}
}

void GameScene::OnMasterTrollFinishEnter()
{
    
    //Set to pause anim and start some counter for effect spawn
//    mMasterTroll->stopAllActions();
    _MasterTroll_animation->pause();
}

void GameScene::CreateMasterTrollEnter()
{
    CCMoveTo* aWalkTo = CCMoveTo::create(1.0f,ccp(100,_screenSize.height/2));
    CCCallFuncN* aFunc1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnMasterTrollFinishEnter));
    CCSequence* aSeq = CCSequence::create(aWalkTo,aFunc1,NULL);
    mMasterTroll->runAction(aSeq);
}

void GameScene::CreateMachines()
{
    if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission
       || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
       || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission)
    {
        //Spawns troll that comes from forest !!!
        mMasterTroll = CCSprite::create("DebugDot.png");
        addChild(mMasterTroll);
        
        _MasterTroll_IdleAnimation = SpriteAnimation::create("Characters/troll/troll_diagonal_down.plist");
        _MasterTroll_IdleAnimation->retain();
        _MasterTroll_IdleAnimation->setFlipX(true);
        
        _MasterTroll_WinAnimation = SpriteAnimation::create("Characters/troll/troll_victory_dance.plist");
        _MasterTroll_WinAnimation->retain();
        _MasterTroll_WinAnimation->setFlipX(true);
        
        SetMasterTrollAnimation(_MasterTroll_IdleAnimation);
        
        //Craete some small animation move !!!
        mMasterTroll->setPosition(ccp(-100,_screenSize.height/2));
        
        //When to spawn bullet
        mTrollBulletTimer = (rand()%6)+6;
        
        if(User::getInstance()->mSpecial_23_Mission){
            //Spawn 3 spiders !!! and the caves !!!
            
            //Create the 3 caves and the circles for them !!!
            _SpiderCave_1 = CCSprite::create("cave/trapdoor-blue.png");
            _SpiderCave_1->setPosition(ccp(300,200));
            addChild(_SpiderCave_1);
            
            //1st cave
            float CavePos_X = 200;
            float CavePos_Y = 200;
            
            float precision = .5f;
            float cir = 2 * M_PI;
            int mRadius = 130;
            
            int aOffY = 10;
            
            _movePointsSpiderCave_1 = CCPointArray::create(0);
            _movePointsSpiderCave_1->retain();
            
            for (float a = 0.1f; a < cir; a += precision)
            {
                float x = _SpiderCave_1->getPositionX() + mRadius * cos(a);
                float y = _SpiderCave_1->getPositionY() + mRadius/1.5f * sin(a);
                
                _movePointsSpiderCave_1->addControlPoint(ccp(x,y+aOffY));
                
//                CCSprite* aDummyDot = CCSprite::create("small_dot_red.png");
//                aDummyDot->setPosition(ccp(x,y+aOffY));
//                addChild(aDummyDot);
            }
            
            //Next
            _SpiderCave_2 = CCSprite::create("cave/trapdoor-blue.png");
            _SpiderCave_2->setPosition(ccp(680,300));
            addChild(_SpiderCave_2);
            
            _movePointsSpiderCave_2 = CCPointArray::create(0);
            _movePointsSpiderCave_2->retain();
            
            for (float a = 0.1f; a < cir; a += precision)
            {
                float x = _SpiderCave_2->getPositionX() + mRadius * cos(a);
                float y = _SpiderCave_2->getPositionY() + mRadius/1.5f * sin(a);
                
                _movePointsSpiderCave_2->addControlPoint(ccp(x,y+aOffY));
                
//                CCSprite* aDummyDot = CCSprite::create("small_dot_red.png");
//                aDummyDot->setPosition(ccp(x,y+aOffY));
//                addChild(aDummyDot);
            }
            
            //Next
            _SpiderCave_3 = CCSprite::create("cave/trapdoor-blue.png");
            _SpiderCave_3->setPosition(ccp(300,500));
            addChild(_SpiderCave_3);
            
            _movePointsSpiderCave_3 = CCPointArray::create(0);
            _movePointsSpiderCave_3->retain();
            
            for (float a = 0.1f; a < cir; a += precision)
            {
                float x = _SpiderCave_3->getPositionX() + mRadius * cos(a);
                float y = _SpiderCave_3->getPositionY() + mRadius/1.5f * sin(a);
                
                _movePointsSpiderCave_3->addControlPoint(ccp(x,y+aOffY));
                
//                CCSprite* aDummyDot = CCSprite::create("small_dot_red.png");
//                aDummyDot->setPosition(ccp(x,y+aOffY));
//                addChild(aDummyDot);
            }
            
            generateSpider(200,200,0);
        }
    }
    else
    {
        Machine_Stamp* aMachine1 = Machine_Stamp::create(this);
        aMachine1->setPosition(ccp(_screenSize.width/2+50,_screenSize.height-100));
        aMachine1->setTag(1010);
        addChild(aMachine1,1);
        
        //The Ghost
        Machine_Ghost* aMachine2 = Machine_Ghost::create(this);
        aMachine2->setPosition(ccp(100,350));
        aMachine2->setTag(1020);
        addChild(aMachine2,1);
    }
    
    //The Enemies
    Machine_Enemies* aMachine3 = Machine_Enemies::create(this);
    aMachine3->setPosition(ccp(_screenSize.width-90,315));
    aMachine3->setTag(1030);
    addChild(aMachine3,1);
    
    //Add to this machine some counter !!!
    if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission
       || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
       || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission)
    {
        CCSprite* aCharges = CCSprite::create("Charges_Machine.png");
        aCharges->setOpacity(130);
        aCharges->setScale(0.7);
        aCharges->setPosition(ccp(0,100));
        aMachine3->addChild(aCharges);
        
        mCharges_Electro = CCLabelTTF::create("2", FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*0.7, CCSize(100, 120), kCCTextAlignmentCenter);
        mChargesForBlitz = 2;
        
        aCharges->addChild(mCharges_Electro);
        mCharges_Electro->setPosition(ccp(40,10));
    }
}

int GameScene::getSpriteOrderZ(int posY)
{
    return _totalStepsZ-floorf(posY/42);
}

void GameScene::highlightBigDoor(bool theOff)
{
    int aFadeValue = 255;
    float aFadeSpeed = 0.5f;
    
    if (theOff){
        aFadeSpeed = 0.25f;
        aFadeValue = 90;
        
        if(getChildByTag(79))
            removeChildByTag(79);
    }
    else{
        if(getChildByTag(79)==NULL)
            addChild(_blueArrowAnim,getSpriteOrderZ(_caveFat->getPositionY()));
    }
    
    /*
    if(User::getInstance()->mSpecial_12_Mission)
    {
        return;
    }
    
    _caveDoorBig->stopAllActions();
    
    int aFadeValue = 255;
    float aFadeSpeed = 0.5f;
    if (theOff)
    {
        aFadeSpeed = 0.25f;
        aFadeValue = 90;
//        getChildByTag(79)->setVisible(false);
        if(getChildByTag(79))
            removeChildByTag(79);
    }
    else
    {
//        getChildByTag(79)->setVisible(true);
        if(getChildByTag(79)==NULL)
            addChild(_blueArrowAnim,getSpriteOrderZ(_cave->getPositionY()));
    }
    
    if(mNewSplitCaves){
        return;
    }
    
    CCActionInterval* aFadeTo = CCFadeTo::create(aFadeSpeed, aFadeValue);
    _caveDoorBig->runAction(aFadeTo);
    */
}

void GameScene::highlightSmallDoor(bool theOff)
{
    int aFadeValue = 255;
    float aFadeSpeed = 0.5f;
    
    if (theOff){
        aFadeSpeed = 0.25f;
        aFadeValue = 90;
        
        if(getChildByTag(78))
            removeChildByTag(78);
    }
    else{
        if(getChildByTag(78)==NULL)
            addChild(_orangeArrowAnim,getSpriteOrderZ(_caveTall->getPositionY()));
    }
    
    /*
    if(User::getInstance()->mSpecial_12_Mission)
    {
        return;
    }
    
    _caveDoorSmall->stopAllActions();
    
    int aFadeValue = 255;
    float aFadeSpeed = 0.5f;
    if (theOff)
    {
        aFadeSpeed = 0.25f;
        aFadeValue = 90;
//        getChildByTag(78)->setVisible(false);
        if(getChildByTag(78))
            removeChildByTag(78);
    }
    else
    {
//        getChildByTag(78)->setVisible(true);
        if(getChildByTag(78)==NULL)
            addChild(_orangeArrowAnim,getSpriteOrderZ(_cave->getPositionY()));
    }
    
    if(mNewSplitCaves){
        return;
    }
    
    CCActionInterval* aFadeTo = CCFadeTo::create(aFadeSpeed, aFadeValue);
    _caveDoorSmall->runAction(aFadeTo);
    */
}

void GameScene::dwarfEnterDoor(bool theFat, Dwarf* theDwarf)
{
    
    
    //Check if this dwarf is not on some bullet adictive
    if(theDwarf->mBulletActive){
        //Disable at bullet dwarf
        for (int trollIndex = _bullets->count() - 1; trollIndex >= 0; --trollIndex)
        {
            TrollBullet* bullet = static_cast<TrollBullet*>(_bullets->objectAtIndex(trollIndex));
            if(bullet->_dwarf == theDwarf){
                bullet->_dwarf = NULL;//NO MORE DWARF
                break;
            }
        }
    }
    
    //infrom all trolls that catch is over for this dwarf !!!
    for (int trollIndex = _trolls->count() - 1; trollIndex >= 0; --trollIndex)
    {
        Troll* troll = static_cast<Troll*>(_trolls->objectAtIndex(trollIndex));
        troll->CancelDwarfCatch(theDwarf);
    }
    
    CCActionInterval* aFadeIn = CCFadeTo::create(0.25,255);
    CCActionInterval* aDelay = CCDelayTime::create(0.25);
    CCActionInterval* aFadeOut = CCFadeTo::create(0.25,90);
    CCSequence* aFadeSeq = CCSequence::create(aFadeIn,aDelay,aFadeOut,NULL);
    
    /*
    if(User::getInstance()->mSpecial_12_Mission || mNewSplitCaves)
    {
        
    }
    else
    {
        if (theFat)
        {
            _caveDoorBig->stopAllActions();
            _caveDoorBig->runAction(aFadeSeq);
            _stats.rescuedDwarfs[DWARF_TYPE_FAT] += 1;
        }
        else
        {
            _caveDoorSmall->stopAllActions();
            _caveDoorSmall->runAction(aFadeSeq);
            _stats.rescuedDwarfs[DWARF_TYPE_TALL] += 1;
        }
    }
    */
    
    if(User::getInstance()->mSpecial_10_Mission)
    {
        _DwarfsEnteredCave+=1;
        
        std::stringstream aStringValue;
        aStringValue<<_DwarfsEnteredCave<<"/20";
        
        _DwarfCounter->setString(aStringValue.str().c_str());
        
        if(_DwarfsEnteredCave>=20)
        {
            //We completed mission !!!
            SpecialMissionCompleted();
        }
    }
    
    if(User::getInstance()->mSpecial_13_Mission)
    {
        _DwarfsEnteredCave+=1;
        
        std::stringstream aStringValue;
        aStringValue<<_DwarfsEnteredCave<<"/25";
        
        _DwarfCounter->setString(aStringValue.str().c_str());
        
        if(_DwarfsEnteredCave>=25)
        {
            //We completed mission !!!
            SpecialMissionCompleted();
        }
    }
    
    if(User::getInstance()->mSpecial_14_Mission)
    {
        _DwarfsEnteredCave+=1;
        
        std::stringstream aStringValue;
        aStringValue<<_DwarfsEnteredCave<<"/32";
        
        _DwarfCounter->setString(aStringValue.str().c_str());
        
        if(_DwarfsEnteredCave>=32)
        {
            //We completed mission !!!
            SpecialMissionCompleted();
        }
        else
        {
            
        }
    }
    
    if(User::getInstance()->mSpecial_11_Mission)
    {
        //Check if dwarf collected crystal - if 10 collected - finish it
        if(theDwarf->mSpecialObjectPicked)
        {
            _DwarfsEnteredCave+=1;
            
            std::stringstream aStringValue;
            aStringValue<<_DwarfsEnteredCave<<"/15";
            
            _DwarfCounter->setString(aStringValue.str().c_str());
            
            if(_DwarfsEnteredCave>=15)
            {
                //We completed mission !!!
                SpecialMissionCompleted();
            }
        }
    }
    
    if(User::getInstance()->mSpecial_12_Mission)
    {
        _DwarfsEnteredCave+=1;
        
        std::stringstream aStringValue;
        aStringValue<<_DwarfsEnteredCave<<"/25";
        
        _DwarfCounter->setString(aStringValue.str().c_str());
        
        if(_DwarfsEnteredCave>=25)
        {
            //We completed mission !!!
            SpecialMissionCompleted();
        }
        else
        {
            //Update tesla machine visual by level amount of dwarfs in
            
            if(_DwarfsEnteredCave == 5 || _DwarfsEnteredCave == 10 || _DwarfsEnteredCave == 15 || _DwarfsEnteredCave == 20 || _DwarfsEnteredCave == 25)
            {
                //maybe create some blitx here !!!
                SpriteAnimation* aBlitz = SpriteAnimation::create("powerup/Machine_Enemies/elektro_strike.plist",false);
                aBlitz->retain();
                aBlitz->setAnchorPoint(ccp(0.5,0));
                aBlitz->setPosition(ccp(mMapBase->getContentSize().width/2,mMapBase->getContentSize().height/2-100));
                addChild(aBlitz,kPoints_Z_Order);
                
                CCDelayTime* aDelay = CCDelayTime::create(0.5f);
                CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                CCSequence* aSeq1 = CCSequence::create(aDelay,func,NULL);
                aBlitz->runAction(aSeq1);
                
                playInGameSound("ET_Zap");
                
                if(getChildByTag(10030))
                {
                    removeChildByTag(10030);
                }
                
                //Change it !!!
                Machine_Enemies* aMachine3 = Machine_Enemies::create(this);
                aMachine3->setPosition(ccp(_screenSize.width/2,_screenSize.height/2));
                aMachine3->setTag(10030);
                addChild(aMachine3,1);
            }

        }
    }
    
}

void GameScene::disableBooster(int theType)
{
    _totalActiveBoosters--;
//    CCMenuItemImage* aButton = static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(theType));
//    aButton->setOpacity(120);
    checkBoosterAvailability();
}

void GameScene::checkBoosterAvailability()
{
    return;
    
    CCMenuItemImage* aButton = static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(kBooster_FutureSee));
    if (User::getInstance()->getDiamonds()<BOOSTER_1_PRICE)
        aButton->setOpacity(120);
    
    aButton = static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(kBooster_Crystals));
    if (User::getInstance()->getDiamonds()<BOOSTER_2_PRICE)
        aButton->setOpacity(120);
    
    aButton = static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(kBooster_NoEnemy));
    if (User::getInstance()->getDiamonds()<BOOSTER_3_PRICE)
        aButton->setOpacity(120);
    
    aButton = static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(kBooster_Ghost));
    if (User::getInstance()->getDiamonds()<BOOSTER_4_PRICE)
        aButton->setOpacity(120);
}

void GameScene::menuPowerupEyeCallback(cocos2d::CCObject *sender)
{
//    generateEffect();
//    return;
//    createTextFly("Foresee\nCrystals", ccp(100,100), 1);
//    return;
//    generateTroll();
//    return;
//    PopUp::create(this, "Daily challenge finished", (CallBack)(&MainMenuScene::testCall),true);
//    return;
//    menuSaveMeCallBack(NULL,NULL,NULL);
//    nowCreateSaveMe();
//    return;
//    generateDwarf();
//    return;
//    generateDiamond();
//    User::getInstance()->addDiamonds(10);
//    return;
    
    //Check how much boosters are active now
    if (_totalActiveBoosters>0 && !_boosterMushroom)
    {
        //Text fly out of button?
        
        return;//Too much active boosters
    }
    
    //Check if has money - if not - ask to buy? how will this work in game?
    int aPrice = BOOSTER_1_PRICE;
    if (_willUseMushroom)
        aPrice = 0;
    
    int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);//BOOSTER_1_PRICE
    if (aDidUseDiamonds<0)
    {
        //Show popup that no money
        
        return;
    }
    
    _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
    
    //All ok we payed for it !!!
    _totalActiveBoosters++;
    
    //When selected then what to doo ??? Maybe add some pulse??
    CCMenuItemImage* aButton = static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(kBooster_FutureSee));
//    if (aButton->getOpacity()>120)
//        return;//It is active already !!! maybe add more to the active
    
    createTextFly("Foresee\nCrystals",
				  ccp(getChildByTag(kBoosters)->getPositionX()+getChildByTag(kBoosters)->getChildByTag(kBooster_FutureSee)->getPositionX(),
                                           _screenSize.height-114), 1);
    
    //Lets activate it !!!
    aButton->setOpacity(255);//It is active
    
    if (getChildByTag(2))
    {
        //remove it and add new timer !!!
        removeChildByTag(2);
    }
    
    //Where is our button !!!
    int aUserLvl = User::getInstance()->getLevel();
    _boostFutureSee = 30+(floorf(aUserLvl*0.2)*15);//This should be callculated from level
    
    //Now create the animation circle
    float aTimerTime = _boostFutureSee/64;
    TimedSpriteAnimation* _animation = TimedSpriteAnimation::create("Interfeiss/in_game/power_timer.plist", -aTimerTime);
    _animation->setTag(2);
    _animation->setPosition(ccp(aButton->getPositionX()+aButton->getParent()->getPositionX()-2,
                                aButton->getPositionY()+aButton->getParent()->getPositionY()+3));
    //Set the progress bar !!!
    addChild(_animation,kHUD_Z_Order+1);
}

void GameScene::menuPowerupPointsCallback(cocos2d::CCObject *sender)
{
    //Check how much boosters are active now
    if (_totalActiveBoosters>0 && !_boosterMushroom)
    {
        //Text fly out of button?
        
        return;//Too much active boosters
    }
    
    int aPrice = BOOSTER_2_PRICE;
    if (_willUseMushroom)
        aPrice = 0;
    
    //Check if has money - if not - ask to buy? how will this work in game?
    int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);
    if (aDidUseDiamonds<0)
    {
        //Show popup that no money
        
        return;
    }
    
    _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
    
    //All ok we payed for it !!!
    _totalActiveBoosters++;
    
    //When selected then what to doo ??? Maybe add some pulse??
    CCMenuItemImage* aButton = static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(kBooster_Crystals));
//    if (aButton->getOpacity()>120)
//        return;//It is active already !!! maybe add more to the active
    
//    createTextFly("Crystal\nBoost", ccp(aButton->getPositionX(),_screenSize.height-100), 1);
    
    createTextFly("Crystal\nBoost", ccp(getChildByTag(kBoosters)->getPositionX()+getChildByTag(kBoosters)->getChildByTag(kBooster_Crystals)->getPositionX(),
                                           _screenSize.height-114), 1);
    
    //Lets activate it !!!
    aButton->setOpacity(255);//It is active
    
    //////////////////////////
    
    if (getChildByTag(3))
    {
        //remove it and add new timer !!!
        removeChildByTag(3);
    }
    
    //Where is our button !!!
    int aUserLvl = User::getInstance()->getLevel();
    _boostCrystalsTimer = 30+(floorf(aUserLvl*0.2)*15);//This should be callculated from level
    
    //Now create the animation circle
    float aTimerTime = _boostCrystalsTimer/64;
    TimedSpriteAnimation* _animation = TimedSpriteAnimation::create("Interfeiss/in_game/power_timer.plist", -aTimerTime);
    _animation->setTag(3);
    _animation->setPosition(ccp(aButton->getPositionX()+aButton->getParent()->getPositionX()-2,
                                aButton->getPositionY()+aButton->getParent()->getPositionY()+3));
    //Set the progress bar !!!
    addChild(_animation,kHUD_Z_Order+1);
    
    //////////////////////////
    
    _boostExtraPoints = 10;
//    mBoost_Crystals_Timer = 30;
}

void GameScene::activateNoEnemies(int theX,int theY)
{
    return;//For now disabled
    //create somewhere this powerup stuff!!!
    createTextFly("No\nEnemies", ccp(theX,theY), 1);
    
    _boostNoEnemyTimer = 30;//This should be callculated from level
    
    removeAllEffects();
    
    //Play the good mode !!!
//    crossFadeBackground(true);
}

void GameScene::crossFadeBackground(bool theGodMode)
{
    return;//Not needed - but who knows what will be later :)
    CCSprite* aGodLevel = static_cast<CCSprite*>(this->getChildByTag(333));//levelGod
    CCSprite* aSimpleLevel = static_cast<CCSprite*>(this->getChildByTag(332));//levelGod
    
    CCFadeIn* aFadeIn = CCFadeIn::create(0.5f);
    CCFadeOut* aFadeOut = CCFadeOut::create(0.5f);
    
    if (theGodMode)
    {
        aGodLevel->cocos2d::CCNode::setVisible(true);
        aGodLevel->runAction(aFadeIn);
        aSimpleLevel->runAction(aFadeOut);
    }
    else
    {
        aSimpleLevel->runAction(aFadeIn);
        aGodLevel->runAction(aFadeOut);
    }
}

void GameScene::activateGhoustDwarfs(int theX,int theY)
{
    //create somewhere this powerup stuff!!!
    createTextFly("Ghost\ndwarfs", ccp(theX,theY), 1);
    
    _boostGhostTimer = 30;//This should be callculated from level
    
    removeAllEffects();
}

void GameScene::activateClearMap(int theX,int theY)
{
    //create somewhere this powerup stuff!!!
    createTextFly("Clear\nAll Map", ccp(theX,theY), 1);
    
    clearAllMap();
}

void GameScene::menuPowerupNoEnemiesCallback(cocos2d::CCObject *sender)
{
    //Check how much boosters are active now
    if (_totalActiveBoosters>0 && !_boosterMushroom)
    {
        //Text fly out of button?
        
        return;//Too much active boosters
    }
    
    int aPrice = BOOSTER_3_PRICE;
    if (_willUseMushroom)
        aPrice = 0;
    
    //Check if has money - if not - ask to buy? how will this work in game?
    int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);
    if (aDidUseDiamonds<0)
    {
        //Show popup that no money
        
        return;
    }
    
    _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
    
    //All ok we payed for it !!!
    _totalActiveBoosters++;
    
    //When selected then what to doo ??? Maybe add some pulse??
    CCMenuItemImage* aButton = static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(kBooster_NoEnemy));
//    if (aButton->getOpacity()>120)
//        return;//It is active already !!! maybe add more to the active
    
    createTextFly("No\nEnemies",
				  ccp(getChildByTag(kBoosters)->getPositionX()+getChildByTag(kBoosters)->getChildByTag(kBooster_NoEnemy)->getPositionX(),
                                        _screenSize.height-114), 1);
    
    //Lets activate it !!!
    aButton->setOpacity(255);//It is active
    
    //////////////////////////
    
    if (getChildByTag(4))
    {
        //remove it and add new timer !!!
        removeChildByTag(4);
    }
    
    //Where is our button !!!
    int aUserLvl = User::getInstance()->getLevel();
    _boostNoEnemyTimer = 30+(floorf(aUserLvl*0.2)*15);//This should be callculated from level
    
    //Now create the animation circle
    float aTimerTime = _boostNoEnemyTimer/64;
    TimedSpriteAnimation* _animation = TimedSpriteAnimation::create("Interfeiss/in_game/power_timer.plist", -aTimerTime);
    _animation->setTag(4);
    _animation->setPosition(ccp(aButton->getPositionX()+aButton->getParent()->getPositionX()-2,
                                aButton->getPositionY()+aButton->getParent()->getPositionY()+3));
    //Set the progress bar !!!
    addChild(_animation,kHUD_Z_Order+1);
    
    //////////////////////////
    
//    mBoost_NoEnemy_Timer = 30;//This should be callculated from level
    
    removeAllEffects();
}

void GameScene::menuPowerupSlowPlayCallback(cocos2d::CCObject *sender)
{
    //Check how much boosters are active now
    if (_totalActiveBoosters>0 && !_boosterMushroom)
    {
        //Text fly out of button?
        
        return;//Too much active boosters
    }
    
    int aPrice = BOOSTER_4_PRICE;
    if (_willUseMushroom)
        aPrice = 0;
    
    //Check if has money - if not - ask to buy? how will this work in game?
    int aDidUseDiamonds = User::getInstance()->canUseDiamonds(aPrice);
    if (aDidUseDiamonds<0)
    {
        //Show popup that no money
        
        return;
    }
    
    _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
    
    //All ok we payed for it !!!
    _totalActiveBoosters++;
    
    //When selected then what to doo ??? Maybe add some pulse??
    CCMenuItemImage* aButton = static_cast<CCMenuItemImage*>(getChildByTag(kBoosters)->getChildByTag(kBooster_Ghost));
//    if (aButton->getOpacity()>120)
//        return;//It is active already !!! maybe add more to the active
    
    createTextFly("Ghost\ndwarfs",
				  ccp(getChildByTag(kBoosters)->getPositionX()+getChildByTag(kBoosters)->getChildByTag(kBooster_Ghost)->getPositionX(),
                                     _screenSize.height-114), 1);
    
    //Lets activate it !!!
    aButton->setOpacity(255);//It is active
    
    //////////////////////////
    
    if (getChildByTag(5))
    {
        //remove it and add new timer !!!
        removeChildByTag(5);
    }
    
    //Where is our button !!!
    int aUserLvl = User::getInstance()->getLevel();
    _boostGhostTimer = 30+(floorf(aUserLvl*0.2)*15);//This should be callculated from level
    _boostGhostTimer = 5;
    
    //Now create the animation circle
    float aTimerTime = _boostGhostTimer/64;
    TimedSpriteAnimation* _animation = TimedSpriteAnimation::create("Interfeiss/in_game/power_timer.plist", -aTimerTime);
    _animation->setTag(5);
    _animation->setPosition(ccp(aButton->getPositionX()+aButton->getParent()->getPositionX()-2,
                                aButton->getPositionY()+aButton->getParent()->getPositionY()+3));
    //Set the progress bar !!!
    addChild(_animation,kHUD_Z_Order+1);
    
    //////////////////////////
    
//    mBoost_Ghost_Timer = 30;//This should be callculated from level
}

void GameScene::StartTornado(bool theDisable)
{
    if(mTornadoActive || theDisable)
    {
        mTornadoActive = false;
        
        SpriteAnimation* p = (SpriteAnimation*)getChildByTag(9878);
        if(p!=NULL)
            removeChild(p,true);
        
//        CCParticleSystemQuad* p = (CCParticleSystemQuad*)getChildByTag(9878);
//        if(p!=NULL)
//            p->stopSystem();
    }
    else
    {
        mTornadoActive = true;
        
//        CCParticleSystemQuad* p = CCParticleSystemQuad::create("effects/wind/Wind_1.plist");
//        p->setPosition(ccp(0,_screenSize.height/2));
//        p->setAutoRemoveOnFinish(true);
//        addChild(p,kHUD_Z_Order-10,9878);//The reain tag :)
        
//        generateEffectSpecial(-100,400,EFFECT_TYPE_WIND);
        
        SpriteAnimation* p = SpriteAnimation::create("effects/wind/wind_small.plist");
        p->setPosition(ccp(100,100));
        addChild(p,getSpriteOrderZ(p->getPositionY()),9878);
        
        CCDelayTime* aTime = CCDelayTime::create(1.6f);
        CCCallFuncN* aFunc = CCCallFuncN::create(this, callfuncN_selector(GameScene::PlaceInDifferentSpotWind));
        CCSequence* aSeq = CCSequence::create(aTime,aFunc,NULL);
        CCRepeatForever* aRep = CCRepeatForever::create(aSeq);
        p->runAction(aRep);
        
        //maybe create the 2nd too?
        
    }
}

void GameScene::PlaceInDifferentSpotWind(CCNode* sender)
{
    int aRanodmID = rand()%4;
    if(aRanodmID==0)
        sender->setPosition(ccp(200,_screenSize.height/2-150));
    else if(aRanodmID==1)
        sender->setPosition(ccp(_screenSize.width/2,200));
    else if(aRanodmID==2)
        sender->setPosition(ccp(_screenSize.width/2,_screenSize.height-200));
    else if(aRanodmID==3)
        sender->setPosition(ccp(_screenSize.width-200,_screenSize.height/2+150));
    
    reorderChild(sender,getSpriteOrderZ(sender->getPositionY()));
}

void GameScene::StartRainDebug()
{
    StartRain(false);
}

void GameScene::StartWindDebug()
{
    StartTornado(false);
}

void GameScene::StartRain(bool theDisable)
{
    if(mRainActive || theDisable)
    {
        mRainActive = false;
        
        CCParticleSystemQuad* p = (CCParticleSystemQuad*)getChildByTag(9876);
        if(p!=NULL)
            p->stopSystem();
        
        p = (CCParticleSystemQuad*)getChildByTag(9877);
        if(p!=NULL)
            p->stopSystem();
    }
    else
    {
        mRainActive = true;
        
        CCParticleSystemQuad* p = CCParticleSystemQuad::create("effects/rain/Lietus_2.plist");
        p->setPosition(ccp(_screenSize.width/2,_screenSize.height+100));
        p->setAutoRemoveOnFinish(true);
        addChild(p,kHUD_Z_Order-10,9876);//The reain tag :)
        
        //Add the 2nd particles - rain drops under all
        CCParticleSystemQuad* p2 = CCParticleSystemQuad::create("effects/rain/Lietus_Pelkes_2.plist");
        p2->setPosition(ccp(_screenSize.width/2,_screenSize.height/2));
        p2->setAutoRemoveOnFinish(true);
        addChild(p2,1,9877);//The reain tag :)
        
//        generateEffect();
//        generateEffectSpecial(200,200,EFFECT_TYPE_RAIN);
    }
}

void GameScene::CreateSpecialSpot()
{
    generateEffect();
}

//void GameScene::StartTornado()
//{
//    
//}

void GameScene::OnCompleteMission()
{
//    SpecialMissionCompleted();
    NewMissionCompleted();
}

void GameScene::menuPauseCallback(cocos2d::CCObject *sender)//menuPauseCallback
{
    
//    NewMissionCompleted();
//    return;
    
//    ChangeMap(1);
//    return;
    
//    if(!mTornadoActive)
//        StartTornado(false);
//    generateEffect();
//    return;
//
//    if(_trolls->count()<1)
//        generateTroll();
//    
//    return;
    
//    CreateCompleted();
//    return;
    
//    CreateTornadoThrow((Dwarf*)_dwarves->objectAtIndex(0),NULL,200,200);
//    return;
    
//    CreateComboSound(2, mDebugComboValue);
//    mDebugComboValue++;
//    if(mDebugComboValue>6)
//        mDebugComboValue = 1;
//    
//    return;
    
//    generatePlantCrystal();
    
//    CreateSubMissionCompletedShow(0,0);
//    CreateMissionCompletedShow(0);
//    return;
    
//    std::stringstream theComboAmount;
//    theComboAmount << "x" << mCombo_DwarfEnter;
//    createTextFly(theComboAmount.str().c_str(), ccp(300,300), 3,mCombo_DwarfEnter);
//    CreateComboSound(1,mCombo_DwarfEnter);
//    
//    mCombo_DwarfEnter+=1;
//    mComboTimer_DwarfEnter=3.0f;
////
//    StartRain();
//    StartTornado();
//    return;
    
//    if(!mTornadoActive)
//        StartTornado(false);
    
//    generateEffect();
//    Machine_Enemies* aMachine = static_cast<Machine_Enemies*>(getChildByTag(1030));
////    aMachine->ShowNumber();
//    aMachine->SetAnimation_Use(10);
//    
//    Machine_Ghost* aMachineG = static_cast<Machine_Ghost*>(getChildByTag(1020));
//    aMachineG->SetAnimation_Use(10);
//    
//    return;
    
    if(mTutorialEnabled)
        return;//!!!
    
//    std::stringstream theTextField;
//    theTextField <<"Daily Challange";
////    theTextField<<User::getInstance()->getMissionManager().GetSubMissionText(0,0,false,true);
//    
//    
//    AddPopUpToShow(1,5,theTextField.str().c_str());
//    return;
    
//    lose();
//    return;
    
//    generateCrystal();
//    return;
    
//    generateCrystal();
//    generateMushroom();
//    generateEffect();
//    generateDwarf();
//    generateTroll();
//    return;
//    if (gamePause)
//        return;
    
//    std::stringstream theComboAmount;
//    theComboAmount << "x" << 2;
//    createTextFly(theComboAmount.str().c_str(), ccp(300,300), 2);
//    return;
    
//    CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/KaboomFx.plist");
//    p->setPosition(ccp(200,200));
//    addChild(p);
    
//    nowCreateSaveMe();
//    return;
    
    if(_gamePause)
        return;//It's done already
    
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    
    _gamePause = true;
    pauseSchedulerAndActionsRecursive(this,false);
    
    CCLayerColor* aBlackBG = CCLayerColor::create(ccc4(0,0,0,64),_screenSize.width,_screenSize.height);
    aBlackBG->setTag(7777);
    aBlackBG->setOpacity(0);
    addChild(aBlackBG,kHUD_Z_Order);
    
    CCFadeTo* aFadeBG = CCFadeTo::create(0.5f,128);
    aBlackBG->runAction(aFadeBG);
    
    PauseScene* pauseLayer = PauseScene::create();
    pauseLayer->setAnchorPoint(ccp(0,0));
    pauseLayer->setPosition(ccp(0,_screenSize.height));
    this->addChild(pauseLayer, kHUD_Z_Order+10);
    
    pauseLayer->UpdateMissionStarsPause();
    
    CCMoveTo* aMove1 = CCMoveTo::create(0.5f,ccp(0,0));
    CCEaseElasticOut* aEase1 = CCEaseElasticOut::create(aMove1,0.5f);
    pauseLayer->runAction(aEase1);
}

void GameScene::onResumeFromPause()
{
    CCLayerColor* aBlackBG = (CCLayerColor*)getChildByTag(7777);
    
    CCFadeTo* aFade1 = CCFadeTo::create(0.5f,0);
    CCCallFuncN* aF1_func2 = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCSequence* aSe1 = CCSequence::create(aFade1,aF1_func2,NULL);
    aBlackBG->runAction(aSe1);
    
//    onPauseFinished();/Direct from pause - no timers
    
    //Show timer with 3,2,1 - action !!!
    CCSprite* frame_1 = CCSprite::create("Interfeiss/save_me/1.png");
    frame_1->setOpacity(0);
//    frame_1->setPosition(ccp((_screenSize.width-frame_1->boundingBox().size.width)/2,(_screenSize.height-frame_1->boundingBox().size.height)/2+_screenSize.height/3));
    frame_1->setPosition(ccp((_screenSize.width-frame_1->boundingBox().size.width)/2,(_screenSize.height-frame_1->boundingBox().size.height)/2));
    
    CCSprite* frame_2 = CCSprite::create("Interfeiss/save_me/2.png");
    frame_2->setOpacity(0);
//    frame_2->setPosition(ccp((_screenSize.width-frame_2->boundingBox().size.width)/2,(_screenSize.height-frame_2->boundingBox().size.height)/2+_screenSize.height/3));
    frame_2->setPosition(ccp((_screenSize.width-frame_2->boundingBox().size.width)/2,(_screenSize.height-frame_2->boundingBox().size.height)/2));
    
    CCSprite* frame_3 = CCSprite::create("Interfeiss/save_me/3.png");
//    frame_3->setPosition(ccp((_screenSize.width-frame_3->boundingBox().size.width)/2,(_screenSize.height-frame_3->boundingBox().size.height)/2+_screenSize.height/3));
    frame_3->setPosition(ccp((_screenSize.width-frame_3->boundingBox().size.width)/2,(_screenSize.height-frame_3->boundingBox().size.height)/2));
    
//    playInGameSound("pause_count");
    
    addChild(frame_1, kHUD_Z_Order+1);
    addChild(frame_2, kHUD_Z_Order+1);
    addChild(frame_3, kHUD_Z_Order+1);
    
    //Create the actions
    CCDelayTime* aF1_delay = CCDelayTime::create(2.25f);
    CCCallFuncN* aF1_sound1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::onPuseTimeCount));
    
    CCFadeIn* aF1_fadeIn = CCFadeIn::create(0.0f);
    CCFadeOut* aF1_fadeOut = CCFadeOut::create(1.0f);
    CCCallFuncN* aF1_func1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::onPauseFinished));
    CCCallFuncN* aF1_func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCSequence* aF1_seq = CCSequence::create(aF1_delay,aF1_sound1,aF1_fadeIn,aF1_fadeOut,aF1_func1,aF1_func,NULL);
    frame_1->runAction(aF1_seq);
    
    CCDelayTime* aF2_delay = CCDelayTime::create(1.25f);
    CCCallFuncN* aF2_sound1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::onPuseTimeCount));
    
    CCFadeIn* aF2_fadeIn = CCFadeIn::create(0.0f);
    CCFadeOut* aF2_fadeOut = CCFadeOut::create(1.0f);
    CCCallFuncN* aF2_func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCSequence* aF2_seq = CCSequence::create(aF2_delay,aF2_sound1,aF2_fadeIn,aF2_fadeOut,aF2_func,NULL);
    frame_2->runAction(aF2_seq);
    
    CCDelayTime* aF3_delay = CCDelayTime::create(0.25f);
    CCCallFuncN* aF3_sound1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::onPuseTimeCount));
    
    CCFadeOut* aF3_fadeOut = CCFadeOut::create(1.0f);
    CCCallFuncN* aF3_func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCSequence* aF3_seq = CCSequence::create(aF3_delay,aF3_sound1,aF3_fadeOut,aF3_func,NULL);
    frame_3->runAction(aF3_seq);
}

void GameScene::onPuseTimeCount()
{
    playInGameSound("pause_count");
}

void GameScene::onPauseFinished()
{
    _gamePause = false;
    resumeSchedulerAndActionsRecursive(this);
    
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void GameScene::menuSaveMeCallBack(Dwarf* dwarf_1,Dwarf* dwarf_2,Troll* troll)
{
    if(User::getInstance()->mSpecial_10_Mission || User::getInstance()->mSpecial_11_Mission || User::getInstance()->mSpecial_12_Mission
       || User::getInstance()->mSpecial_13_Mission || User::getInstance()->mSpecial_14_Mission)
    {
        //End the game
        lose();
        return;
    }
    
    //do something with the call back callers
    if (dwarf_1)
        _saveDwarf1 = dwarf_1;
    if (dwarf_2)
        _saveDwarf2 = dwarf_2;
    if (troll)
        _saveTroll = troll;
    
//    if(User::getInstance()->_tutorial_small_3==0)
//    {
//        //Wait for 2 sec and only then show up the menu with save me !!!
//        stopInGameSound("dwarf_walk",true);
//        stopInGameSound("troll_walk",true);
//        stopInGameSound("dwarf_web_stuck",true);
//        
//        CreateSmallTutorial(3,dwarf_1->getPositionX(),dwarf_1->getPositionY());
//    }
//    else
//    {
//        _gamePause = true;
//        pauseSchedulerAndActionsRecursive(this,true);
//        
//        stopInGameSound("dwarf_walk",true);
//        stopInGameSound("troll_walk",true);
//        stopInGameSound("dwarf_web_stuck",true);
//        
//        RemoveCompletedTutorial();
//        
//        schedule(schedule_selector(GameScene::nowCreateSaveMe), 2.0f);
//    }
    
    _gamePause = true;
    pauseSchedulerAndActionsRecursive(this,true);
    
    stopInGameSound("Footsteps",true);
    stopInGameSound("troll_walk",true);
    stopInGameSound("dwarf_web_stuck",true);
    
    RemoveCompletedTutorial();
    
    schedule(schedule_selector(GameScene::nowCreateSaveMe), 2.0f);
    
//    _gamePause = true;
//    pauseSchedulerAndActionsRecursive(this,true);
    
    //Wait for 2 sec and only then show up the menu with save me !!!
//    stopInGameSound("dwarf_walk",true);
//    stopInGameSound("troll_walk",true);
//    stopInGameSound("dwarf_web_stuck",true);
    
    //Play troll victory sound?
    
//    schedule(schedule_selector(GameScene::nowCreateSaveMe), 2.0f);
}

void GameScene::nowCreateSaveMe()
{
    //Wait for 2 sec and only then show up the menu with save me !!!
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    
    unschedule(schedule_selector(GameScene::nowCreateSaveMe));
    
    SaveMeScene* saveLayer = SaveMeScene::create();
    saveLayer->setAnchorPoint(ccp(0,0));
    this->addChild(saveLayer,kHUD_Z_Order);
}

void GameScene::onSaveFromPause()
{
    _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
    
    /*
    if(User::getInstance()->mSpecial_18_Mission || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission
       || User::getInstance()->mSpecial_21_Mission || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission){
        
        _gamePause = false;
        resumeSchedulerAndActionsRecursive(this);
        
        //Play quick clear sound
        playInGameSound("save_me_clear");
        
        return;
    }
    
    if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission)
    {
        if(_dwarfOnMap!=NULL){
            _dwarfOnMap->setString("5 Dwarfs Left");
        }
        
        _mission_dwarfs_max+=5;
        
        _gamePause = false;
        resumeSchedulerAndActionsRecursive(this);
        
        //Play quick clear sound
        playInGameSound("save_me_clear");
        
        return;
    }
    
    if(User::getInstance()->mNewMissionBuild)
    {
        //Update the header
        if(_dwarfOnMap!=NULL){
            _dwarfOnMap->setString("2 Dwarfs Left");
        }
        
        //Just add 2 more dwarfs to spawn !!!
        _mission_dwarfs_max+=2;
        
        _gamePause = false;
        resumeSchedulerAndActionsRecursive(this);
        
        //Play quick clear sound
        playInGameSound("save_me_clear");
        
        return;
    }
    */
    
    //Remove the crashed trolls and other stuff that made this save me call
//    if (_saveTroll)
//    {
//        _saveTroll->setTag(0);
//        //Fade him out and let dwarf move forward
//        _saveTroll->removeFromSave();
//    }
    
    //Small update - remove all objects from map !!!
    if (_saveDwarf1)
    {
        if (_saveDwarf2)
        {
            _saveDwarf2->setTag(0);
            _saveDwarf1->setTag(0);
            //Each goes to other direction
            _saveDwarf1->clearCrash(false);
            _saveDwarf2->clearCrash(false);
            
            _saveDwarf1->setDisabled(true);
            _saveDwarf1->removeFromSave();
            
            _saveDwarf2->setDisabled(true);
            _saveDwarf2->removeFromSave();
        }
        else
        {
            _saveDwarf1->setTag(0);
            //Just allow him to move futher !!!
            _saveDwarf1->clearCrash(true);
            
            _saveDwarf1->setDisabled(true);
            _saveDwarf1->removeFromSave();
        }
    }
    
    /*
    gameSpeed = 0.5f;
    gameSlowTimer = 10.0f;
    */
    
    //Remove all trolls
//    for (int trollIndex = _trolls->count() - 1; trollIndex >= 0; --trollIndex)
//	{
//		Troll* troll = static_cast<Troll*>(_trolls->objectAtIndex(trollIndex));
//		
//		if (troll->isVisible())
//            troll->removeFromSave();
//    }
    
    //Remove all dwarfs
//    for (int dwarfIndex = _dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
//	{
//		Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(dwarfIndex));
//		
//		if (dwarf->isVisible())
//        {
//            dwarf->setDisabled(true);
//            dwarf->removeFromSave();
//        }
//    }
    
    removeAllEffects();
    
    stopInGameSound("Footsteps");
    stopInGameSound("troll_walk");
    stopInGameSound("dwarf_web_stuck",true);
    
    _gameSpeed = GAME_SPEED_NORMAL;
    
    this->getChildByTag(100)->getChildByTag(101)->setVisible(false);
    this->getChildByTag(100)->getChildByTag(102)->setVisible(true);
    
    //Clear all
    _saveTroll = NULL;
    _saveDwarf1 = NULL;
    _saveDwarf2 = NULL;
    
    _gamePause = false;
    resumeSchedulerAndActionsRecursive(this);
    
    //Play quick clear sound
    playInGameSound("save_me_clear");
    
    //Update subtask
    User::getInstance()->getMissionManager().CheckSubMission(SUB_USE_SAVE_ME,1);
    
//    PlaySpecialMusic(_dwarves->count());
}

void GameScene::clearAllMap()
{
    //Remove all trolls
    for (int trollIndex = _trolls->count() - 1; trollIndex >= 0; --trollIndex)
	{
		Troll* troll = static_cast<Troll*>(_trolls->objectAtIndex(trollIndex));
		
		if (troll->isVisible())
            troll->removeFromSave();
    }
    
    //Remove all dwarfs
    for (int dwarfIndex = _dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
	{
		Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(dwarfIndex));
		
		if (dwarf->isVisible())
        {
            dwarf->setDisabled(true);
            dwarf->removeFromSave();
        }
    }
    
    stopInGameSound("Footsteps");
    stopInGameSound("troll_walk");
    stopInGameSound("dwarf_web_stuck",true);
    
    //Remove all effects
    removeAllEffects();
    
    playInGameSound("save_me_clear");
}

void GameScene::menuFastModeCallback(cocos2d::CCObject *sender)
{
    if (_gameSpeed<GAME_SPEED_NORMAL)
    {
        _gameSpeed = GAME_SPEED_NORMAL;
        return;
    }
    
//    SetMasterTrollAction(8);
//    CreateTrollIndicator(2);
//    SetMasterTrollAction(7);
//    return;
    
    if(mTutorialEnabled)
        return;//for now disabled
    
    //Maybe disable speed button when saved self???
    _gameSlowTimer = 0;//Remove it if was active
    _gameSpeed = GAME_SPEED_FAST;
    
    this->getChildByTag(100)->getChildByTag(101)->setVisible(true);
    this->getChildByTag(100)->getChildByTag(102)->setVisible(false);
    
    AnalyticX::flurryLogEvent("Game Menu: Turbo On");
    
    playInGameSound("button_speed_on");
}

void GameScene::menuNormalModeCallback(cocos2d::CCObject *sender)
{
    if(mTutorialEnabled)
        return;//for now disabled
    
    _gameSlowTimer = 0;//Remove it if was active
    _gameSpeed = GAME_SPEED_NORMAL;
    
     AnalyticX::flurryLogEvent("Game Menu: Turbo Off");
    
    this->getChildByTag(100)->getChildByTag(101)->setVisible(false);
    this->getChildByTag(100)->getChildByTag(102)->setVisible(true);
    
    playInGameSound("button_speed_off");
}

//------------------------------------------------------------------------
//Pause functional for all game

void GameScene::pauseSchedulerAndActionsRecursive(cocos2d::CCNode* node,bool skipSaveMe)
{
    node->pauseSchedulerAndActions();
    
    if (node->getChildren())
    {
        for(int i=0;i<node->getChildren()->count();i++)
        {
            CCNode* nodeIn = static_cast<CCNode*>(node->getChildren()->objectAtIndex(i));
            if (!nodeIn)
                continue;
            
            cocos2d::CCNode *projectile = static_cast<CCSprite*>(nodeIn);
            if (!projectile)
                continue;
            
            if (skipSaveMe && projectile->getTag()==999)
                continue;//Skip this one !!!
            
            if(mTutorialEnabled)
            {
                if(projectile->getTag() == 121212122)
                    continue;
            }
            
            //The freeze anim skip
            if (projectile->getTag()==kFreezeSprite)
                continue;
            
            if (projectile->getTag()==kNoPause)
                continue;
            
            pauseSchedulerAndActionsRecursive(projectile,skipSaveMe);
        }
    }
}

void GameScene::resumeSchedulerAndActionsRecursive(cocos2d::CCNode* node)
{
    node->resumeSchedulerAndActions();
    
    if (node->getChildren())
    {
        for(int i=0;i<node->getChildren()->count();i++)
        {
            CCNode* nodeIn = static_cast<CCNode*>(node->getChildren()->objectAtIndex(i));
            if (!nodeIn)
                continue;
            
            cocos2d::CCNode *projectile = static_cast<CCSprite*>(nodeIn);
            if (!projectile)
                continue;
            
            if (projectile->getTag()==kFreezeSprite)
                continue;
            
            resumeSchedulerAndActionsRecursive(projectile);
        }
    }
}

//------------------------------------------------------------------------

////Resumes paused game if needed
//- (void)resumeSchedulerAndActionsRecursive:(CCNode *)node
//{
//    [node resumeSchedulerAndActions];
//    for (CCNode *child in [node children]) {
//        [self resumeSchedulerAndActionsRecursive:child];
//    }
//}

void GameScene::UpdateTutorialStuff()
{
    if(getChildByTag(301))
    {
        //Check if can do this
        if(mTutorialDrawHand)
        {
            if(mTutorialHandStartCords.x == 0 && mTutorialHandStartCords.y == 0)
            {
                mTutorialHandStartCords = getChildByTag(301)->getPosition();
                mTutorialHandStartCords.x-=10;
                mTutorialHandStartCords.y+=20;
            }
            CCPoint previousPoint = mTutorialHandStartCords;
            
            if (_movePoints && mTutorialPointsAdded &&  _movePoints->count() > 0)
                previousPoint = _movePoints->getControlPointAtIndex(_movePoints->count() - 1);
            
            CCPoint position = getChildByTag(301)->getPosition();
            position.x-=25;
            position.y+=52;
            
            addTutorialMovePoint(position, previousPoint);
        }
    }
}

void GameScene::CreateDrawHand(int theStep)
{
    RemoveDrawHand();
    
    if(theStep == 1)
    {
        //The anim !!!
        Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(0));
        
        //Fat ass to cave
        CCSprite* aHand = CCSprite::create("Interfeiss/tutorial/hand_cursor.png");
        aHand->setPosition(ccp(dwarf->getPositionX()+20,dwarf->getPositionY()-50));
        aHand->setTag(301);
        addChild(aHand,kHUD_Z_Order+1);
        
        //Tap effect
        CCScaleTo* aScale_1 = CCScaleTo::create(0.25f, 0.8f);
        CCCallFunc* aRipleCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandClick));
        CCSequence* aSeqHand_1 = CCSequence::create(aScale_1,aRipleCall_S1,NULL);
        CCDelayTime* aTimeDelay1 = CCDelayTime::create(0.5f);
        
        CCMoveTo* aToPos_1 = CCMoveTo::create(1.5f,ccp(_screenSize.width/2-10,_screenSize.height/2-100));
        CCDelayTime* aTime1 = CCDelayTime::create(0.5f);
        CCMoveTo* aToPos_2 = CCMoveTo::create(0.0f, ccp(aHand->getPositionX(),aHand->getPositionY()));
        
        //On finish clear all points drawed !!!
        CCCallFunc* aRemoveCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandStart));
        CCCallFunc* aRemoveCall_F1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandFinish));
        
        CCScaleTo* aScale_2 = CCScaleTo::create(0.0f, 1.0f);
        
//        CCSequence* aSeq1 = CCSequence::create(aRemoveCall_S1,aToPos_1,aTime1,aToPos_2,aRemoveCall_F1,NULL);
        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        CCRepeatForever* aRep1 = CCRepeatForever::create(aSeq1);
        
        aHand->runAction(aRep1);
    }
    else if(theStep == 2)
    {
        //Thin ass to cave
        //The anim !!!
        Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(1));
        
        //Fat ass to cave
        CCSprite* aHand = CCSprite::create("Interfeiss/tutorial/hand_cursor.png");
        aHand->setPosition(ccp(dwarf->getPositionX()+10,dwarf->getPositionY()-50));
        aHand->setTag(301);
        addChild(aHand,kHUD_Z_Order+1);
        
        
        //Tap effect
        CCScaleTo* aScale_1 = CCScaleTo::create(0.25f, 0.8f);
        CCCallFunc* aRipleCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandClick));
        CCSequence* aSeqHand_1 = CCSequence::create(aScale_1,aRipleCall_S1,NULL);
        CCDelayTime* aTimeDelay1 = CCDelayTime::create(0.5f);
        
        CCMoveTo* aToPos_1 = CCMoveTo::create(1.5f,ccp(_screenSize.width/2+80,_screenSize.height/2-100));
        CCDelayTime* aTime1 = CCDelayTime::create(0.5f);
        CCMoveTo* aToPos_2 = CCMoveTo::create(0.0f, ccp(aHand->getPositionX(),aHand->getPositionY()));
        
        CCCallFunc* aRemoveCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandStart));
        CCCallFunc* aRemoveCall_F1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandFinish));
        
        CCScaleTo* aScale_2 = CCScaleTo::create(0.0f, 1.0f);
        
        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        CCRepeatForever* aRep1 = CCRepeatForever::create(aSeq1);
        
        aHand->runAction(aRep1);
    }
    else if(theStep == 3)
    {
        //Thin ass to cave
        //The anim !!!
        Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(0));
        
        //Fat ass to cave
        CCSprite* aHand = CCSprite::create("Interfeiss/tutorial/hand_cursor.png");
        aHand->setPosition(ccp(dwarf->getPositionX()+50,dwarf->getPositionY()-50));
        aHand->setTag(301);
        addChild(aHand,kHUD_Z_Order+1);
        
        
        //Tap effect
        CCScaleTo* aScale_1 = CCScaleTo::create(0.25f, 0.8f);
        CCCallFunc* aRipleCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandClick));
        CCSequence* aSeqHand_1 = CCSequence::create(aScale_1,aRipleCall_S1,NULL);
        CCDelayTime* aTimeDelay1 = CCDelayTime::create(0.5f);
        
        ccBezierConfig bezier;
        bezier.controlPoint_1 = ccp(_screenSize.width/2+200, _screenSize.height/2+70);//1096,168
        bezier.controlPoint_2 = ccp(_screenSize.width/2+350, _screenSize.height/2-100);//635,105
        bezier.endPosition = ccp(_screenSize.width/2+80, _screenSize.height/2-100);
        
        CCBezierTo* aToPos_1a = CCBezierTo::create(1.5f, bezier);
        
//        CCMoveTo* aToPos_1 = CCMoveTo::create(1.5f,ccp(_screenSize.width/2+80,_screenSize.height/2-100));
        CCDelayTime* aTime1 = CCDelayTime::create(0.5f);
        CCMoveTo* aToPos_2 = CCMoveTo::create(0.0f, ccp(aHand->getPositionX(),aHand->getPositionY()));
        
        CCCallFunc* aRemoveCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandStart));
        CCCallFunc* aRemoveCall_F1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandFinish));
        
        CCScaleTo* aScale_2 = CCScaleTo::create(0.0f, 1.0f);
        
//        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1a,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        
        CCRepeatForever* aRep1 = CCRepeatForever::create(aSeq1);
        
        aHand->runAction(aRep1);
    }
    else if(theStep == 4)
    {
        //Thin ass to cave
        //The anim !!!
        Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(0));
        
        //Fat ass to cave
        CCSprite* aHand = CCSprite::create("Interfeiss/tutorial/hand_cursor.png");
        aHand->setPosition(ccp(dwarf->getPositionX()+50,dwarf->getPositionY()-50));
        aHand->setTag(301);
        addChild(aHand,kHUD_Z_Order+1);
        
        
        //Tap effect
        CCScaleTo* aScale_1 = CCScaleTo::create(0.25f, 0.8f);
        CCCallFunc* aRipleCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandClick));
        CCSequence* aSeqHand_1 = CCSequence::create(aScale_1,aRipleCall_S1,NULL);
        CCDelayTime* aTimeDelay1 = CCDelayTime::create(0.5f);
        
        ccBezierConfig bezier;
        bezier.controlPoint_1 = ccp(_screenSize.width/2-200, _screenSize.height/2-220);//1096,168
        bezier.controlPoint_2 = ccp(_screenSize.width/2+50, _screenSize.height/2-260);//635,105
        bezier.endPosition = ccp(_screenSize.width/2+80, _screenSize.height/2-100);
        
        CCBezierTo* aToPos_1a = CCBezierTo::create(1.5f, bezier);
        
//        CCMoveTo* aToPos_1 = CCMoveTo::create(1.5f,ccp(_screenSize.width/2+80,_screenSize.height/2-100));
        CCDelayTime* aTime1 = CCDelayTime::create(0.5f);
        CCMoveTo* aToPos_2 = CCMoveTo::create(0.0f, ccp(aHand->getPositionX(),aHand->getPositionY()));
        
        CCCallFunc* aRemoveCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandStart));
        CCCallFunc* aRemoveCall_F1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandFinish));
        
        CCScaleTo* aScale_2 = CCScaleTo::create(0.0f, 1.0f);
        
        //        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1a,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        
        CCRepeatForever* aRep1 = CCRepeatForever::create(aSeq1);
        
        aHand->runAction(aRep1);
    }
    else if(theStep == 5)
    {
        //Thin ass to cave
        //The anim !!!
        Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(0));
        
        //Fat ass to cave
        CCSprite* aHand = CCSprite::create("Interfeiss/tutorial/hand_cursor.png");
        aHand->setPosition(ccp(dwarf->getPositionX()+30,dwarf->getPositionY()-50));
        aHand->setTag(301);
        addChild(aHand,kHUD_Z_Order+1);
        
        
        //Tap effect
        CCScaleTo* aScale_1 = CCScaleTo::create(0.25f, 0.8f);
        CCCallFunc* aRipleCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandClick));
        CCSequence* aSeqHand_1 = CCSequence::create(aScale_1,aRipleCall_S1,NULL);
        CCDelayTime* aTimeDelay1 = CCDelayTime::create(0.5f);
        
        ccBezierConfig bezier;
        bezier.controlPoint_1 = ccp(aHand->getPositionX()-160,aHand->getPositionY()-120);//1096,168
        bezier.controlPoint_2 = ccp(_screenSize.width/2+140, aHand->getPositionY()+120);//635,105
        bezier.endPosition = ccp(_screenSize.width/2+140, _screenSize.height/2-180);//160
//        CCBezierTo* aToPos_1a = CCBezierTo::create(1.0f, bezier);
        
        //Create another braizer combo
        ccBezierConfig bezier2;
        bezier2.controlPoint_1 = ccp(_screenSize.width/2+100, _screenSize.height/2-120);//1096,168
        bezier2.controlPoint_2 = ccp(_screenSize.width/2+20, _screenSize.height/2-200);//635,105
        bezier2.endPosition = ccp(_screenSize.width/2-20, _screenSize.height/2-100);
        
        CCBezierTo* aToPos_1a = CCBezierTo::create(1.4f, bezier);
        CCBezierTo* aToPos_1b = CCBezierTo::create(1.1f, bezier2);
        
        
//        CCMoveTo* aToPos_1 = CCMoveTo::create(1.5f,ccp(_screenSize.width/2+80,_screenSize.height/2-100));
        CCDelayTime* aTime1 = CCDelayTime::create(0.5f);
        CCMoveTo* aToPos_2 = CCMoveTo::create(0.0f, ccp(aHand->getPositionX(),aHand->getPositionY()));
        
        CCCallFunc* aRemoveCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandStart));
        CCCallFunc* aRemoveCall_F1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandFinish));
        
        CCScaleTo* aScale_2 = CCScaleTo::create(0.0f, 1.0f);
        
        //        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1a,aToPos_1b,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        
        CCRepeatForever* aRep1 = CCRepeatForever::create(aSeq1);
        
        aHand->runAction(aRep1);
    }
    if(theStep == 6)
    {
        //The anim !!!
//        Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(0));
        
        //Fat ass to cave
        CCSprite* aHand = CCSprite::create("Interfeiss/tutorial/hand_cursor.png");
        aHand->setPosition(ccp(_screenSize.width-70,_screenSize.height/2-80));
        aHand->setTag(301);
        addChild(aHand,kHUD_Z_Order+1);
        
        //Tap effect
        CCScaleTo* aScale_1 = CCScaleTo::create(0.25f, 0.8f);
        CCCallFunc* aRipleCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandClick));
        CCSequence* aSeqHand_1 = CCSequence::create(aScale_1,aRipleCall_S1,NULL);
        CCDelayTime* aTimeDelay1 = CCDelayTime::create(1.0f);
        
        
        
//        CCMoveTo* aToPos_1 = CCMoveTo::create(1.5f,ccp(_screenSize.width/2-10,_screenSize.height/2-100));
//        CCDelayTime* aTime1 = CCDelayTime::create(0.5f);
//        CCMoveTo* aToPos_2 = CCMoveTo::create(0.0f, ccp(aHand->getPositionX(),aHand->getPositionY()));
        
        //On finish clear all points drawed !!!
//        CCCallFunc* aRemoveCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandStart));
//        CCCallFunc* aRemoveCall_F1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandFinish));
        
        CCScaleTo* aScale_2 = CCScaleTo::create(0.1f, 1.0f);
        CCDelayTime* aDelay2 = CCDelayTime::create(0.2f);
        
        //        CCSequence* aSeq1 = CCSequence::create(aRemoveCall_S1,aToPos_1,aTime1,aToPos_2,aRemoveCall_F1,NULL);
        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aScale_2,aDelay2,NULL);
        CCRepeatForever* aRep1 = CCRepeatForever::create(aSeq1);
        
        aHand->runAction(aRep1);
    }
    else
    {
        //Wtf
    }
    
    //Special new steps
    if(theStep == 10)
    {
        //Thin ass to cave
        //The anim !!!
        Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(1));
        
        //Fat ass to cave
        CCSprite* aHand = CCSprite::create("Interfeiss/tutorial/hand_cursor.png");
        aHand->setPosition(ccp(dwarf->getPositionX(),dwarf->getPositionY()-50));
        aHand->setTag(301);
        addChild(aHand,kHUD_Z_Order+1);
        
        
        //Tap effect
        CCScaleTo* aScale_1 = CCScaleTo::create(0.25f, 0.8f);
        CCCallFunc* aRipleCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandClick));
        CCSequence* aSeqHand_1 = CCSequence::create(aScale_1,aRipleCall_S1,NULL);
        CCDelayTime* aTimeDelay1 = CCDelayTime::create(0.5f);
        
        ccBezierConfig bezier;
//        bezier.controlPoint_1 = ccp(dwarf->getPositionX()+50,dwarf->getPositionY()-50);//1096,168
//        bezier.controlPoint_2 = ccp(dwarf->getPositionX()+50, _screenSize.height/2+100);//635,105
        //Under troll
        bezier.controlPoint_1 = ccp(dwarf->getPositionX()-200,dwarf->getPositionY()-100);//1096,168
        bezier.controlPoint_2 = ccp(dwarf->getPositionX()-200, _screenSize.height/2-260);//635,105
        bezier.endPosition = ccp(_screenSize.width/2+80, _screenSize.height/2-100);
        
        CCBezierTo* aToPos_1a = CCBezierTo::create(1.5f, bezier);
        
//        CCMoveTo* aToPos_1 = CCMoveTo::create(1.5f,ccp(_screenSize.width/2+80,_screenSize.height/2-100));
        CCDelayTime* aTime1 = CCDelayTime::create(0.5f);
        CCMoveTo* aToPos_2 = CCMoveTo::create(0.0f, ccp(aHand->getPositionX(),aHand->getPositionY()));
        
        CCCallFunc* aRemoveCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandStart));
        CCCallFunc* aRemoveCall_F1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandFinish));
        
        CCScaleTo* aScale_2 = CCScaleTo::create(0.0f, 1.0f);
        
        //        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1a,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        
        CCRepeatForever* aRep1 = CCRepeatForever::create(aSeq1);
        
        aHand->runAction(aRep1);
    }
    
    if(theStep == 11)
    {
        //Thin ass to cave
        //The anim !!!
        Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(0));
        
        //Fat ass to cave
        CCSprite* aHand = CCSprite::create("Interfeiss/tutorial/hand_cursor.png");
        aHand->setPosition(ccp(dwarf->getPositionX(),dwarf->getPositionY()-50));
        aHand->setTag(301);
        addChild(aHand,kHUD_Z_Order+1);
        
        
        //Tap effect
        CCScaleTo* aScale_1 = CCScaleTo::create(0.25f, 0.8f);
        CCCallFunc* aRipleCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandClick));
        CCSequence* aSeqHand_1 = CCSequence::create(aScale_1,aRipleCall_S1,NULL);
        CCDelayTime* aTimeDelay1 = CCDelayTime::create(0.5f);
        
        ccBezierConfig bezier;
        bezier.controlPoint_1 = ccp(dwarf->getPositionX(),dwarf->getPositionY()-50);//1096,168
        bezier.controlPoint_2 = ccp(_screenSize.width/2+20, dwarf->getPositionY()-170);//635,105
        bezier.endPosition = ccp(_screenSize.width/2-10, _screenSize.height/2-100);
        
        CCBezierTo* aToPos_1a = CCBezierTo::create(1.5f, bezier);
        
//        CCMoveTo* aToPos_1 = CCMoveTo::create(1.5f,ccp(_screenSize.width/2-10,_screenSize.height/2-100));
        CCDelayTime* aTime1 = CCDelayTime::create(0.5f);
        CCMoveTo* aToPos_2 = CCMoveTo::create(0.0f, ccp(aHand->getPositionX(),aHand->getPositionY()));
        
        CCCallFunc* aRemoveCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandStart));
        CCCallFunc* aRemoveCall_F1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTutorialHandFinish));
        
        CCScaleTo* aScale_2 = CCScaleTo::create(0.0f, 1.0f);
        
        //        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        CCSequence* aSeq1 = CCSequence::create(aSeqHand_1,aTimeDelay1,aRemoveCall_S1,aToPos_1a,aTime1,aToPos_2,aRemoveCall_F1,aScale_2,NULL);
        
        CCRepeatForever* aRep1 = CCRepeatForever::create(aSeq1);
        
        aHand->runAction(aRep1);
    }
}

void GameScene::OnTutorialHandClick(CCNode* sender)
{
    if(getChildByTag(334))
        removeChildByTag(334);
    
    CCSprite* aHand = CCSprite::create("Interfeiss/tutorial/ripple_effect.png");
    aHand->setPosition(ccp(sender->getPositionX()-30,sender->getPositionY()+40));
    aHand->setTag(334);
    aHand->setScale(0);
    addChild(aHand,kHUD_Z_Order-1);
    
    CCScaleTo* aScale_1 = CCScaleTo::create(0.5f, 1.0f);
    
    CCDelayTime* aDelay1 = CCDelayTime::create(0.75f);
    CCFadeTo* aFade_1 = CCFadeTo::create(0.5f,0);
    CCSpawn* aSpawn1 = CCSpawn::create(aDelay1,aFade_1,NULL);
    
    CCSpawn* aSpawn2 = CCSpawn::create(aScale_1,aSpawn1,NULL);
    
    CCCallFunc* aRemoveCall_S1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCSequence* aSeq1 = CCSequence::create(aSpawn2,aRemoveCall_S1,NULL);
    aHand->runAction(aSeq1);
}

void GameScene::OnTutorialHandFinish()
{
    removeTutorialMovePoints();
}

void GameScene::OnTutorialHandStart()
{
    mTutorialDrawHand = true;
}

void GameScene::RemoveDrawHand()
{
    removeTutorialMovePoints();
    
    if(getChildByTag(301) != NULL)
        removeChildByTag(301);
}

bool GameScene::addTutorialMovePoint(const cocos2d::CCPoint& point, const cocos2d::CCPoint& previousPoint)
{
	CCPoint diff = point - previousPoint;
    
	float angle = atan2f(diff.y, diff.x);
	
	float dx = 20.0f * cosf(angle);
	float dy = 20.0f * sinf(angle);
	
	float x = previousPoint.x + dx;
	float y = previousPoint.y + dy;
	
	while (true)
	{
		if (dx >= 0.0f && x >= point.x) return true;
		if (dy >= 0.0f && y >= point.y) return true;
		if (dx < 0.0f && x <= point.x) return true;
		if (dy < 0.0f && y <= point.y) return true;
		
		_movePoints->addControlPoint(ccp(x, y));
        mTutorialPointsAdded = true;
		
		CCSprite* dot = CCSprite::create("trajectory_dot_white.png");
		dot->setPosition(ccp(x, y));
		_line->addChild(dot);
		
		x += dx;
		y += dy;
	}
	
	return true;
}

void GameScene::removeTutorialMovePoints()
{
    mTutorialHandStartCords = ccp(0,0);
	// remove all control points
    if(_movePoints && _movePoints!=NULL && mTutorialPointsAdded)
    {
        while (_movePoints->count() != 0)
        {
            _movePoints->removeControlPointAtIndex(0);
        }
        _line->removeAllChildrenWithCleanup(false);
        
        mTutorialPointsAdded = false;
    }
}

void GameScene::OnGotItButton()
{
    if(mTutorialEnabled)
    {
        if(tutorial_button_gotit->numberOfRunningActions()>0)
            return;
        
        _tutorialButtons->setVisible(false);
        
        if(mTutorialSubStep<4)
        {
            OnTutorialStepCompleted(2);
        }
    }
    else
    {
        RemoveSmallTutorial();
    }
}

void GameScene::CreateSmallTutorial(int theType,int theX,int theY)
{
    return;//For now
    
    if(_gamePause)
        return;//Do not come twice
    
    if(mCurrentSmallTutorial!=-1)
    {
        //Try to remove prev stuff !!!
        if(theType == 6){
            if(getChildByTag(322))
                removeChildByTag(322,true);
        }
        else{
            return;//Others can wait
        }
    }
    
    _gamePause = true;
    pauseSchedulerAndActionsRecursive(this,true);
    
    CCSprite* aBG01 = CCSprite::create("TutorialBG2.png");
    CCSprite* aBG02;
    if(theType==3)
        aBG02 = CCSprite::create("TutorialSpot3.png");
    else
        aBG02 = CCSprite::create("TutorialSpot2.png");
    
    //Safe check !!!
    if(getChildByTag(322))
        removeChildByTag(322,true);
    
    CCSprite* aTest = maskedSpriteWithSprite(aBG02, aBG01,theX,theY);
    aTest->setAnchorPoint(ccp(0,0));
    aTest->setOpacity(0);//200
    addChild(aTest,kHUD_Z_Order,322);
    CCFadeTo* aFade1 = CCFadeTo::create(0.5f, 200);
    aTest->runAction(aFade1);
    
    mCurrentSmallTutorial = theType;
    
    if(theType == 1)
    {
        //The ice barge effect explain
        stopInGameSound("Footsteps");
        
        //Show text that we need to do this and this !!!
        //            _tutorialLabel->setString("Tap on dwarf and drag it to the cave, dwarfs like caves");
        _tutorialLabel->setString("Tap on Ice Barrage 4 times to free Dwarf");
        _tutorialLabel->setScale(0.0f);
        if(theY<_screenSize.height/2)
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,_screenSize.height-20));
        else
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,200));
        //Check the label position where can it be placed !!!
        
        CCDelayTime* aDelay = CCDelayTime::create(0.25f);
        CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
        CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
        CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
        _tutorialLabel->runAction(aSeq1);
        
        _tutorialLabel->setVisible(true);
    }
    else if(theType == 2)
    {
        //The ice barge effect explain
        stopInGameSound("Footsteps");
        
        //Show text that we need to do this and this !!!
        //            _tutorialLabel->setString("Tap on dwarf and drag it to the cave, dwarfs like caves");
        _tutorialLabel->setString("Wait 5 seconds and Dwarf will free himself");
        _tutorialLabel->setScale(0.0f);
        if(theY<_screenSize.height/2)
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,_screenSize.height-20));
        else
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,200));
        //Check the label position where can it be placed !!!
        
        CCDelayTime* aDelay = CCDelayTime::create(0.25f);
        CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
        CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
        CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
        _tutorialLabel->runAction(aSeq1);
        
        _tutorialLabel->setVisible(true);
        
        schedule(schedule_selector(GameScene::RemoveSmallTutorial), 1, 0, 5.0f);
    }
    else if(theType == 3)
    {
        //The ice barge effect explain
        stopInGameSound("Footsteps");
        
        //Show text that we need to do this and this !!!
        //            _tutorialLabel->setString("Tap on dwarf and drag it to the cave, dwarfs like caves");
        _tutorialLabel->setString("Don`t let the dwarfs run into each other \notherwise bad things will happen");
        _tutorialLabel->setScale(0.5f);
//        if(theY<_screenSize.height/2)
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,_screenSize.height-20));
//        else
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,230));
        //Check the label position where can it be placed !!!
        
        _tutorialLabel->setVisible(true);
        
        CCDelayTime* aDelay = CCDelayTime::create(0.25f);
        CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
        CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
        CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
        _tutorialLabel->runAction(aSeq1);
        
        
        
        //after 5 sec free save me - or when clicks !!!
        //Add some continue button???
        _tutorialButtons->setVisible(true);
//        CCMenuItemImage* resumeButton = CCMenuItemImage::create(
//                                                                "Interfeiss/pause_menu/resume_game_btn.png",
//                                                                "Interfeiss/pause_menu/resume_game_btn_pressed.png",
//                                                                this,
//                                                                menu_selector(GameScene::onSaveFromSmallTutorial));
//        resumeButton->setScale(0.6f);
//        
//        CCMenu* mainMenu = CCMenu::create(resumeButton, NULL);
//        mainMenu->alignItemsVerticallyWithPadding(-40);
//        mainMenu->setPositionX(_screenSize.width/2);
//        mainMenu->setPositionY(60);
//        this->addChild(mainMenu, kHUD_Z_Order+1,12345);
    }
    else if(theType == 4)
    {
        //The power sene
        stopInGameSound("Footsteps");
        
        //Show text that we need to do this and this !!!
        _tutorialLabel->setString("Pick up magic mushrooms to activate \nacient dwarf machinery");
        _tutorialLabel->setScale(0.0f);
        if(theY<_screenSize.height/2)
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,_screenSize.height-20));
        else
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,230));
        //Check the label position where can it be placed !!!
        
        CCDelayTime* aDelay = CCDelayTime::create(0.25f);
        CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
        CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
        CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
        _tutorialLabel->runAction(aSeq1);
        
        _tutorialLabel->setVisible(true);
        
        _tutorialButtons->setVisible(true);
    }
    else if(theType == 5)
    {
        //The power sene
        stopInGameSound("Footsteps");
        
        //Show text that we need to do this and this !!!
        _tutorialLabel->setString("Gather shiny diamonds to activate awesome powerups");
        _tutorialLabel->setScale(0.0f);
        if(theY<_screenSize.height/2)
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,_screenSize.height-20));
        else
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,230));
        //Check the label position where can it be placed !!!
        
        CCDelayTime* aDelay = CCDelayTime::create(0.25f);
        CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
        CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
        CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
        _tutorialLabel->runAction(aSeq1);
        
        _tutorialLabel->setVisible(true);
        
        _tutorialButtons->setVisible(true);
    }
    else if(theType == 6)
    {
        //The power sene
        stopInGameSound("Footsteps");
        
        //Show text that we need to do this and this !!!
        _tutorialLabel->setString("Hey! Did you know that you can activate dwarf machinery to gain awesome powerups?");
        _tutorialLabel->setScale(0.0f);
        if(theY<_screenSize.height/2)
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,_screenSize.height-60));
        else
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,230));
        //Check the label position where can it be placed !!!
        
        CCDelayTime* aDelay = CCDelayTime::create(0.25f);
        CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
        CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
        CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
        _tutorialLabel->runAction(aSeq1);
        
        _tutorialLabel->setVisible(true);
        
//        _tutorialButtons->setVisible(true);
        
        //Add some more special stuff !!!
        CCSprite* aArrow1 = CCSprite::create("Interfeiss/tutorial/arrow.png");
        aArrow1->setFlipX(true);
        aArrow1->setPosition(ccp(getChildByTag(1030)->getPositionX()-140,getChildByTag(1030)->getPositionY()+65));
        aArrow1->setTag(90110);
        
        CCScaleTo* aArrowScale1 = CCScaleTo::create(0.25f,0.9f);
        CCEaseExponentialInOut* aArrowEase1 = CCEaseExponentialInOut::create(aArrowScale1);
        CCScaleTo* aArrowScale2 = CCScaleTo::create(0.5f,1.0f);
        CCEaseExponentialInOut* aArrowEase2 = CCEaseExponentialInOut::create(aArrowScale2);
        CCSequence* aArrowSeq = CCSequence::create(aArrowEase1,aArrowEase2,NULL);
        CCRepeatForever* aArrowRepeat = CCRepeatForever::create(aArrowSeq);
        aArrow1->runAction(aArrowRepeat);
        
        Machine_Enemies* aMachineE = static_cast<Machine_Enemies*>(getChildByTag(1030));
        aMachineE->ForceSetReady();
        
        addChild(aArrow1,kHUD_Z_Order);
        
    }
    else if(theType == 7)
    {
        //The power sene
        stopInGameSound("Footsteps");
        
        //Show text that we need to do this and this !!!
        _tutorialLabel->setString("Avoid tornados. They bring trouble.");
        _tutorialLabel->setScale(0.0f);
        if(theY<_screenSize.height/2)
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,_screenSize.height-20));
        else
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,230));
        //Check the label position where can it be placed !!!
        
        CCDelayTime* aDelay = CCDelayTime::create(0.25f);
        CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
        CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
        CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
        _tutorialLabel->runAction(aSeq1);
        
        _tutorialLabel->setVisible(true);
        
        _tutorialButtons->setVisible(true);
    }
    else if(theType == 8)
    {
        //The power sene
        stopInGameSound("Footsteps");
        
        //Show text that we need to do this and this !!!
        _tutorialLabel->setString("Avoid rain puddles if you want to stay on your feet");
        _tutorialLabel->setScale(0.0f);
        if(theY<_screenSize.height/2)
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,_screenSize.height-20));
        else
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,230));
        //Check the label position where can it be placed !!!
        
        CCDelayTime* aDelay = CCDelayTime::create(0.25f);
        CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
        CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
        CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
        _tutorialLabel->runAction(aSeq1);
        
        _tutorialLabel->setVisible(true);
        
        _tutorialButtons->setVisible(true);
    }
    else if(theType == 9)
    {
        //The power sene
        stopInGameSound("Footsteps");
        
        //Show text that we need to do this and this !!!
        _tutorialLabel->setString("Avoid meteorite craters at all costs!");
        _tutorialLabel->setScale(0.0f);
        if(theY<_screenSize.height/2)
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,_screenSize.height-20));
        else
            _tutorialLabel->setPosition(ccp(_screenSize.width/2,230));
        //Check the label position where can it be placed !!!
        
        CCDelayTime* aDelay = CCDelayTime::create(0.25f);
        CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
        CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
        CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
        _tutorialLabel->runAction(aSeq1);
        
        _tutorialLabel->setVisible(true);
        
        _tutorialButtons->setVisible(true);
    }
}

void GameScene::onSaveFromSmallTutorial()
{
    removeChildByTag(12345,true);
//    _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
    
    //Remove the crashed trolls and other stuff that made this save me call
    if (_saveTroll)
    {
        _saveTroll->setTag(0);
        //Fade him out and let dwarf move forward
        _saveTroll->removeFromSave();
    }
    
    //Small update - remove all objects from map !!!
    if (_saveDwarf1)
    {
        if (_saveDwarf2)
        {
            _saveDwarf2->setTag(0);
            _saveDwarf1->setTag(0);
            //Each goes to other direction
            _saveDwarf1->clearCrash(false);
            _saveDwarf2->clearCrash(false);
        }
        else
        {
            _saveDwarf1->setTag(0);
            //Just allow him to move futher !!!
            _saveDwarf1->clearCrash(true);
        }
    }
    
    /*
     gameSpeed = 0.5f;
     gameSlowTimer = 10.0f;
     */
    
    //Remove all trolls
    for (int trollIndex = _trolls->count() - 1; trollIndex >= 0; --trollIndex)
	{
		Troll* troll = static_cast<Troll*>(_trolls->objectAtIndex(trollIndex));
		
		if (troll->isVisible())
            troll->removeFromSave();
    }
    
    //Remove all dwarfs
    for (int dwarfIndex = _dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
	{
		Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(dwarfIndex));
		
		if (dwarf->isVisible())
        {
            dwarf->setDisabled(true);
            dwarf->removeFromSave();
        }
    }
    
    removeAllEffects();
    
    stopInGameSound("Footsteps");
    stopInGameSound("troll_walk");
    stopInGameSound("dwarf_web_stuck",true);
    
    _gameSpeed = GAME_SPEED_NORMAL;
    
    this->getChildByTag(100)->getChildByTag(101)->setVisible(false);
    this->getChildByTag(100)->getChildByTag(102)->setVisible(true);
    
    //Clear all
    _saveTroll = NULL;
    _saveDwarf1 = NULL;
    _saveDwarf2 = NULL;
    
//    _gamePause = false;
//    resumeSchedulerAndActionsRecursive(this);
    
    //Play quick clear sound
    playInGameSound("save_me_clear");
    
    RemoveSmallTutorial();
}

//For now
void GameScene::CreateSubMissionCompletedShow(int theMissionID,int theSubID)
{
    
    
    //        std::stringstream ss;
    //        ss << minutes << _gameTime;
    std::stringstream theFull;
//    theFull<<"Completed Submission:\n"<<User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,theSubID,false,true);
    theFull<<User::getInstance()->getMissionManager().GetSubMissionText(theMissionID,theSubID,false,true);
    AddPopUpToShow(0,0,theFull.str().c_str());
    return;
    
    _tutorialLabel->setString(theFull.str().c_str());
    
    _tutorialLabel->setPositionY(_screenSize.height-20);
    
//    _tutorialLabel->setString("Tap on Powerup Machine to activate it and Blitz Trolls");
    _tutorialLabel->setScale(0.0f);
    
    CCDelayTime* aDelay = CCDelayTime::create(0.25f);
    CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
    CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
    
    CCDelayTime* aHide = CCDelayTime::create(3.0f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::HideMissionText));
    
    CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,aHide,func,NULL);
    _tutorialLabel->runAction(aSeq1);
    
    _tutorialLabel->setVisible(true);
}

void GameScene::CreateMissionCompletedShow(int theMissionID)
{
    //        std::stringstream ss;
    //        ss << minutes << _gameTime;
    std::stringstream theFull;
//    theFull<<"Congratulations\n"<<"Mission set #"<<(theMissionID+1)<<" completed";
    theFull<<"Mission set #"<<(theMissionID+1)<<" completed";
    
    AddPopUpToShow(0,0,theFull.str().c_str());
    return;
    
    //Some special stuff
//    mMissionStatus =
    
    _tutorialLabel->setString(theFull.str().c_str());
    
    _tutorialLabel->setPositionY(_screenSize.height-20);
    
    //    _tutorialLabel->setString("Tap on Powerup Machine to activate it and Blitz Trolls");
    _tutorialLabel->setScale(0.0f);
    
    CCDelayTime* aDelay = CCDelayTime::create(0.25f);
    CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
    CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
    
    CCDelayTime* aHide = CCDelayTime::create(3.0f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::HideMissionText));
    
    CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,aHide,func,NULL);
    _tutorialLabel->runAction(aSeq1);
    
    _tutorialLabel->setVisible(true);
    
    //Give the reward to the player???
    
}

void GameScene::HideMissionText()
{
    CCScaleTo* aScale1 = CCScaleTo::create(1.0f,0.0f);
    CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
    _tutorialLabel->runAction(aBounceScale1);
}

void GameScene::RemoveSmallTutorial()
{
    unschedule(schedule_selector(GameScene::RemoveSmallTutorial));
    
    if(getChildByTag(322))
    {
        //Fade it out while other fades in !!!
        CCFadeTo* aFadeOut = CCFadeTo::create(0.5f,0);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
        CCSequence* aSeqFade = CCSequence::create(aFadeOut,func,NULL);
        getChildByTag(322)->runAction(aSeqFade);
    }
    
    playInGameSound("Footsteps");
    
    CCScaleTo* aScale1 = CCScaleTo::create(1.0f,0.0f);
    CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
    _tutorialLabel->runAction(aBounceScale1);
    
    _gamePause = false;
    resumeSchedulerAndActionsRecursive(this);
    
    if(mCurrentSmallTutorial == 1)
        User::getInstance()->finishTutorial_Small_1();
    else if(mCurrentSmallTutorial == 2)
        User::getInstance()->finishTutorial_Small_2();
    else if(mCurrentSmallTutorial == 3)
    {
        User::getInstance()->finishTutorial_Small_3();
    }
    else if(mCurrentSmallTutorial == 4)
        User::getInstance()->finishTutorial_Small_4();
    else if(mCurrentSmallTutorial == 5)
        User::getInstance()->finishTutorial_Small_5();
    else if(mCurrentSmallTutorial == 6)
    {
        //Remove extra stuff
        User::getInstance()->finishTutorial_Small_6();
        if(getChildByTag(90110))
        {
            CCScaleTo* aOldPopup = CCScaleTo::create(0.25f,0.0f);
            CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
            CCSequence* aSeqFade = CCSequence::create(aOldPopup,func,NULL);
            getChildByTag(90110)->runAction(aSeqFade);
        }
    }
    else if(mCurrentSmallTutorial == 7)
        User::getInstance()->finishTutorial_Small_7();
    else if(mCurrentSmallTutorial == 8)
        User::getInstance()->finishTutorial_Small_8();
    else if(mCurrentSmallTutorial == 9)
        User::getInstance()->finishTutorial_Small_9();
    
    mCurrentSmallTutorial = -1;
    
    _tutorialButtons->setVisible(false);
}

void GameScene::ShowTutorialButton()
{
    _tutorialButtons->setVisible(true);
}

void GameScene::OnTutorialStepCompleted(int theStep)
{
    switch (theStep)
    {
        case 1:
        {
            mTutorialSubStep = 1;
            //Spawn dwarfs
            generateDwarf(1,6);
            generateDwarf(0,3);
            
            _line = CCSpriteBatchNode::create("trajectory_dot_white.png");
            addChild(_line);
            
            _movePoints = CCPointArray::create(0);
            _movePoints->retain();
            
            break;
        }
            
        case 2:
        {
            stopInGameSound("Footsteps",true);
            
            //Sub step here !!!
            if(mTutorialSubStep<4)
            {
                if(mTutorialSubStep == 1)
                {
                    //Show text that we need to do this and this !!!
//                    _tutorialLabel->setString("Tap on dwarf and drag it to the cave,\ndwarfs like caves");
                    _tutorialLabel->setString("Blue dwarf goes into the blue cave entrance.\nGot it?");
                    
                    _tutorialLabel->setScale(0.0f);
                    
                    CCDelayTime* aDelay = CCDelayTime::create(0.25f);
                    CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
                    CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
                    CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
                    _tutorialLabel->runAction(aSeq1);
                    
                    _tutorialLabel->setVisible(true);
                    
                    //Make the screen dark and highlight dwarfs
//                    CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_1.png");
                    CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_2.png");
                    aBG1->setAnchorPoint(ccp(0,0));
                    aBG1->setOpacity(0);
                    addChild(aBG1,kHUD_Z_Order,322);
                    
                    CCFadeTo* aFade1 = CCFadeTo::create(0.5f, 200);
                    aBG1->runAction(aFade1);
                    
                    //Add the dwarf bubble 1
                    Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(0));
                    CCSprite* aDwarfPop = CCSprite::create("Interfeiss/tutorial/blue_bubble.png");
                    aDwarfPop->setAnchorPoint(ccp(0,0));
                    aDwarfPop->setScale(0.0f);
                    aDwarfPop->setPosition(ccp(dwarf->getPositionX()-20,dwarf->getPositionY()+40));
                    aDwarfPop->setTag(90100);
                    addChild(aDwarfPop,kHUD_Z_Order);
                    
                    CCScaleTo* aDwarfBop = CCScaleTo::create(0.25f,1.0f);
                    CCEaseBackOut* aDwarfBopEase = CCEaseBackOut::create(aDwarfBop);
                    aDwarfPop->runAction(aDwarfBopEase);
                    
                    //Add arrows to cave !!!
                    CCSprite* aArrow1 = CCSprite::create("Interfeiss/tutorial/arrow_blue.png");
                    aArrow1->setFlipX(false);
                    aArrow1->setPosition(ccp(_screenSize.width/2-aArrow1->getContentSize().width-10,_screenSize.height/2+26));
                    aArrow1->setTag(90101);
                    
                    CCScaleTo* aArrowScale1 = CCScaleTo::create(0.25f,0.9f);
                    CCEaseExponentialInOut* aArrowEase1 = CCEaseExponentialInOut::create(aArrowScale1);
                    CCScaleTo* aArrowScale2 = CCScaleTo::create(0.5f,1.0f);
                    CCEaseExponentialInOut* aArrowEase2 = CCEaseExponentialInOut::create(aArrowScale2);
                    CCSequence* aArrowSeq = CCSequence::create(aArrowEase1,aArrowEase2,NULL);
                    CCRepeatForever* aArrowRepeat = CCRepeatForever::create(aArrowSeq);
                    aArrow1->runAction(aArrowRepeat);
                    
                    addChild(aArrow1,kHUD_Z_Order);
                    
                    //Add the I got it button
                    mTutorialSubStep++;
                    
                    tutorial_button_gotit->setScale(0.0f);
                    _tutorialButtons->setVisible(true);
                    CCDelayTime* aShowButton = CCDelayTime::create(0.5f);
                    CCScaleTo* aButtonScale = CCScaleTo::create(0.25f,1.0f);
                    CCEaseBackOut* aButtonEase = CCEaseBackOut::create(aButtonScale);
                    CCSequence* aSeqButtons = CCSequence::create(aShowButton,aButtonEase,NULL);
                    tutorial_button_gotit->runAction(aSeqButtons);
//                    _tutorialButtons->setVisible(true);
                }
                else if(mTutorialSubStep == 2)
                {
                    if(getChildByTag(322))
                    {
                        //Fade it out while other fades in !!!
                        CCFadeTo* aFadeOut = CCFadeTo::create(0.5f,0);
                        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                        CCSequence* aSeqFade = CCSequence::create(aFadeOut,func,NULL);
                        getChildByTag(322)->runAction(aSeqFade);
                        
                        CCScaleTo* aOldPopup = CCScaleTo::create(0.25f,0.0f);
                        func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                        aSeqFade = CCSequence::create(aOldPopup,func,NULL);
                        getChildByTag(90100)->runAction(aSeqFade);
                        
                        aOldPopup = CCScaleTo::create(0.25f,0.0f);
                        func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                        aSeqFade = CCSequence::create(aOldPopup,func,NULL);
                        getChildByTag(90101)->runAction(aSeqFade);
                    }
                    
                    //Show text that we need to do this and this !!!
                    //                    _tutorialLabel->setString("Tap on dwarf and drag it to the cave,\ndwarfs like caves");
                    _tutorialLabel->setString("Orange dwarf goes into the orange cave entrance.\nGot it?");
                    
                    _tutorialLabel->setScale(0.0f);
                    
                    CCDelayTime* aDelay = CCDelayTime::create(0.25f);
                    CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
                    CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
                    CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
                    _tutorialLabel->runAction(aSeq1);
                    
                    _tutorialLabel->setVisible(true);
                    
                    //Make the screen dark and highlight dwarfs
                    //                    CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_1.png");
                    CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_1.png");
                    aBG1->setAnchorPoint(ccp(0,0));
                    aBG1->setOpacity(0);
                    addChild(aBG1,kHUD_Z_Order,323);
                    
                    CCFadeTo* aFade1 = CCFadeTo::create(0.5f, 200);
                    aBG1->runAction(aFade1);
                    
                    //Add the dwarf bubble 1
                    Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(1));
                    CCSprite* aDwarfPop = CCSprite::create("Interfeiss/tutorial/orange_bubble.png");
                    aDwarfPop->setAnchorPoint(ccp(0,0));
                    aDwarfPop->setScale(0.0f);
                    aDwarfPop->setPosition(ccp(dwarf->getPositionX()-200,dwarf->getPositionY()+40));
                    aDwarfPop->setTag(90100);
                    addChild(aDwarfPop,kHUD_Z_Order);
                    
                    CCScaleTo* aDwarfBop = CCScaleTo::create(0.25f,1.0f);
                    CCEaseBackOut* aDwarfBopEase = CCEaseBackOut::create(aDwarfBop);
                    aDwarfPop->runAction(aDwarfBopEase);
                    
                    //Add arrows to cave !!!
                    CCSprite* aArrow1 = CCSprite::create("Interfeiss/tutorial/arrow.png");
                    aArrow1->setFlipX(false);
                    aArrow1->setPosition(ccp(_screenSize.width/2+aArrow1->getContentSize().width+10,_screenSize.height/2+26));
                    aArrow1->setTag(90101);
                    
                    CCScaleTo* aArrowScale1 = CCScaleTo::create(0.25f,0.9f);
                    CCEaseExponentialInOut* aArrowEase1 = CCEaseExponentialInOut::create(aArrowScale1);
                    CCScaleTo* aArrowScale2 = CCScaleTo::create(0.5f,1.0f);
                    CCEaseExponentialInOut* aArrowEase2 = CCEaseExponentialInOut::create(aArrowScale2);
                    CCSequence* aArrowSeq = CCSequence::create(aArrowEase1,aArrowEase2,NULL);
                    CCRepeatForever* aArrowRepeat = CCRepeatForever::create(aArrowSeq);
                    aArrow1->runAction(aArrowRepeat);
                    
                    addChild(aArrow1,kHUD_Z_Order);
                    
                    //Add the I got it button
                    mTutorialSubStep++;
                    
                    //Show this button only afer some time !!!
                    tutorial_button_gotit->setScale(0.0f);
                    _tutorialButtons->setVisible(true);
                    CCDelayTime* aShowButton = CCDelayTime::create(0.5f);
                    CCScaleTo* aButtonScale = CCScaleTo::create(0.25f,1.0f);
                    CCEaseBackOut* aButtonEase = CCEaseBackOut::create(aButtonScale);
                    CCSequence* aSeqButtons = CCSequence::create(aShowButton,aButtonEase,NULL);
                    tutorial_button_gotit->runAction(aSeqButtons);
//                    _tutorialButtons->setVisible(true);
                }
                else if(mTutorialSubStep == 3)
                {
                    if(getChildByTag(323))
                    {
                        //Fade it out while other fades in !!!
                        CCFadeTo* aFadeOut = CCFadeTo::create(0.5f,0);
                        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                        CCSequence* aSeqFade = CCSequence::create(aFadeOut,func,NULL);
                        getChildByTag(323)->runAction(aSeqFade);
                        
                        CCScaleTo* aOldPopup = CCScaleTo::create(0.25f,0.0f);
                        func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                        aSeqFade = CCSequence::create(aOldPopup,func,NULL);
                        getChildByTag(90100)->runAction(aSeqFade);
                        
                        aOldPopup = CCScaleTo::create(0.25f,0.0f);
                        func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                        aSeqFade = CCSequence::create(aOldPopup,func,NULL);
                        getChildByTag(90101)->runAction(aSeqFade);
                    }
                    _tutorialButtons->setVisible(false);
                    
                    mTutorialSubStep=5;
                    OnTutorialStepCompleted(2);
                }
                return;
            }
            else
            {
                _tutorialButtons->setVisible(false);
                
                //Show text that we need to do this and this !!!
                _tutorialLabel->setString("Tap on dwarf and drag it to the cave,\ndwarfs like caves");
//                _tutorialLabel->setString("Blue dwarf goes into the blue cave entrance.\nGot it?");
                
                _tutorialLabel->setScale(0.0f);
                
                CCDelayTime* aDelay = CCDelayTime::create(0.25f);
                CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
                CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
                CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
                _tutorialLabel->runAction(aSeq1);
                
                _tutorialLabel->setVisible(true);
                
                //Make the screen dark and highlight dwarfs
                CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_1.png");
//                CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_2.png");
                aBG1->setAnchorPoint(ccp(0,0));
                aBG1->setOpacity(0);
                addChild(aBG1,kHUD_Z_Order,322);
                
                CCFadeTo* aFade1 = CCFadeTo::create(0.5f, 200);
                aBG1->runAction(aFade1);
                
                //Create the hand which will draw road to cave !!!
                CreateDrawHand(2);
//                CreateDrawHand(1);
                
                this->schedule( schedule_selector(GameScene::UpdateTutorialStuff) );
            }
            break;
        }
            
        case 3:
        {
            if(getChildByTag(322))
            {
                //Fade it out while other fades in !!!
                CCFadeTo* aFadeOut = CCFadeTo::create(0.5f,0);
                CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                CCSequence* aSeqFade = CCSequence::create(aFadeOut,func,NULL);
                getChildByTag(322)->runAction(aSeqFade);
            }
            
//            _tutorialLabel->setString("Tap the dwarf and drag it to the cave\n Short dwarf can only enter this cave entrance");
            _tutorialLabel->setString("Tap on dwarf and drag it to the cave,\ndwarfs like caves");
            
            CCSprite* aBG2 = CCSprite::create("TutorialFakeMap_2.png");
//            CCSprite* aBG2 = CCSprite::create("TutorialFakeMap_1.png");
            aBG2->setAnchorPoint(ccp(0,0));
            aBG2->setOpacity(0);
            addChild(aBG2,kHUD_Z_Order,323);
            
            CCFadeTo* aFade1 = CCFadeTo::create(0.5f, 200);
            aBG2->runAction(aFade1);
            
            CreateDrawHand(1);
//            CreateDrawHand(2);
            break;
        }
            
        case 4:
        {
            if(getChildByTag(323))
            {
                //Fade it out while other fades in !!!
                CCFadeTo* aFadeOut = CCFadeTo::create(0.5f,0);
                CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                CCSequence* aSeqFade = CCSequence::create(aFadeOut,func,NULL);
                getChildByTag(323)->runAction(aSeqFade);
            }
            RemoveDrawHand();
            
            playInGameSound("Footsteps");
            
            CCScaleTo* aScale1 = CCScaleTo::create(1.0f,0.0f);
            CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
            _tutorialLabel->runAction(aBounceScale1);
            
            _gamePause = false;
            resumeSchedulerAndActionsRecursive(this);
            
            //Reset the game timer !!! and count 2 sec, then spawn trolls with crystals !!!
            _gameTime = 0.0f;
            
            break;
        }
            
        case 5:
        {
            unschedule( schedule_selector(GameScene::UpdateTutorialStuff) );
            
            //The crystal
            generateCrystalSpecial(_screenSize.width/2-100,_screenSize.height/2-160);
            stopInGameSound("Footsteps",true);
            
            //The troll
            generateTrollSpecial(_screenSize.width/2+200,_screenSize.height/2-120,5.6);//5.3
            stopInGameSound("troll_walk",true);
            
//            Troll* trollala = static_cast<Troll*>(_trolls->objectAtIndex(0));
//            trollala->pauseSchedulerAndActions();
//            trollala->_animation->pauseSchedulerAndActions();
//            pauseSchedulerAndActionsRecursive(trollala,false);
            
            _gamePause = true;
            
            _tutorialLabel->setString("Avoid nasty trolls they are dangerous");
            _tutorialLabel->setScale(0.0f);
            
            CCDelayTime* aDelay = CCDelayTime::create(0.25f);
            CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
            CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
            CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
            _tutorialLabel->runAction(aSeq1);
            
            _tutorialLabel->setVisible(true);
            
            //Make the screen dark and highlight dwarfs
            CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_New_1.png");
//            CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_5.png");
            aBG1->setAnchorPoint(ccp(0,0));
            aBG1->setOpacity(0);
            addChild(aBG1,kHUD_Z_Order,322);
            
            CCFadeTo* aFade1 = CCFadeTo::create(0.5f, 200);
            aBG1->runAction(aFade1);
            
            //Create the hand which will draw road to cave !!!
            CreateDrawHand(10);
            
            this->schedule( schedule_selector(GameScene::UpdateTutorialStuff) );
            
//            CreateCompleted();
            
            break;
        }
            
        case 6:
        {
            if(getChildByTag(322))
            {
                //Fade it out while other fades in !!!
                CCFadeTo* aFadeOut = CCFadeTo::create(0.5f,0);
                CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                CCSequence* aSeqFade = CCSequence::create(aFadeOut,func,NULL);
                getChildByTag(322)->runAction(aSeqFade);
            }
            
            _tutorialLabel->setString("Gather crystals to unlock new adventures and dwarf machinery!");
            
            CCSprite* aBG2 = CCSprite::create("TutorialFakeMap_New_2.png");
            aBG2->setAnchorPoint(ccp(0,0));
            aBG2->setOpacity(0);
            addChild(aBG2,kHUD_Z_Order,323);
            
            CCFadeTo* aFade1 = CCFadeTo::create(0.5f, 200);
            aBG2->runAction(aFade1);
            
            CreateDrawHand(11);
            
            break;
        }
            
        case 7:
        {
            if(getChildByTag(323))
            {
                //Fade it out while other fades in !!!
                CCFadeTo* aFadeOut = CCFadeTo::create(0.5f,0);
                CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                CCSequence* aSeqFade = CCSequence::create(aFadeOut,func,NULL);
                getChildByTag(323)->runAction(aSeqFade);
            }
            RemoveDrawHand();
            
            playInGameSound("Footsteps");
            playInGameSound("troll_walk");
            
            CCScaleTo* aScale1 = CCScaleTo::create(1.0f,0.0f);
            CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
            _tutorialLabel->runAction(aBounceScale1);
            
            _gamePause = false;
            resumeSchedulerAndActionsRecursive(this);
            
            //Reset the game timer !!! and count 2 sec, then spawn trolls with crystals !!!
//            _gameTime = 0.0f;
            
//            unschedule( schedule_selector(GameScene::UpdateTutorialStuff) );
            
            break;
        }
            
        //Part II
        case 10:
        {
            unschedule( schedule_selector(GameScene::UpdateTutorialStuff) );
            
            //-----------------------
            //Remove on final
            _line = CCSpriteBatchNode::create("trajectory_dot_white.png");
            addChild(_line);
            
            _movePoints = CCPointArray::create(0);
            _movePoints->retain();
            //-----------------------
            mTutorialStep = 10;
            generateDwarf(0,7);
            
            break;
        }
        case 11:
        {
            _gamePause = true;
            pauseSchedulerAndActionsRecursive(this,false);
            
            generateCrystalSpecial(_screenSize.width/2-100,_screenSize.height-250);
            
            stopInGameSound("Footsteps",true);
            
            //The standart stuff
            //Show text that we need to do this and this !!!
            _tutorialLabel->setString("Gather crystals to unlock new adventures and dwarf machinery!");
            _tutorialLabel->setScale(0.0f);
            _tutorialLabel->setPositionY(_screenSize.height-20);
            
            CCDelayTime* aDelay = CCDelayTime::create(0.25f);
            CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
            CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
            CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
            _tutorialLabel->runAction(aSeq1);
            
            _tutorialLabel->setVisible(true);
            
            //Make the screen dark and highlight dwarfs
            CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_3.png");
            aBG1->setAnchorPoint(ccp(0,0));
            aBG1->setOpacity(0);
            addChild(aBG1,kHUD_Z_Order,322);
            
            CCFadeTo* aFade1 = CCFadeTo::create(0.5f, 200);
            aBG1->runAction(aFade1);
            
            //Create the hand which will draw road to cave !!!
            CreateDrawHand(3);
            
            this->schedule( schedule_selector(GameScene::UpdateTutorialStuff) );
            break;
        }
        
        case 12:
        {
            if(getChildByTag(322))
            {
                //Fade it out while other fades in !!!
                CCFadeTo* aFadeOut = CCFadeTo::create(0.5f,0);
                CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                CCSequence* aSeqFade = CCSequence::create(aFadeOut,func,NULL);
                getChildByTag(322)->runAction(aSeqFade);
            }
            RemoveDrawHand();
            
            unschedule(schedule_selector(GameScene::UpdateTutorialStuff));
            
            playInGameSound("Footsteps");
            
            CCScaleTo* aScale1 = CCScaleTo::create(1.0f,0.0f);
            CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
            _tutorialLabel->runAction(aBounceScale1);
            
            _gamePause = false;
            resumeSchedulerAndActionsRecursive(this);
            
            mTutorialFlag_1 = false;
            mTutorialFlag_2 = false;
            
            break;
        }
            
        case 13:
        {
            //Collected crystal spawn diamond now
            _gamePause = true;
            pauseSchedulerAndActionsRecursive(this,false);
            //Let the dwarf create new line !!!
            stopInGameSound("Footsteps",true);
            
//            generateCrystalSpecial(_screenSize.width/2-100,_screenSize.height-250);
            generateDiamondSpecial(_screenSize.width/2-60,_screenSize.height/2-120);
            
            //The standart stuff
            //Show text that we need to do this and this !!!
            _tutorialLabel->setString("Gather shiny diamonds to activate awesome powerups");
            _tutorialLabel->setScale(0.0f);
            
            CCDelayTime* aDelay = CCDelayTime::create(0.25f);
            CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
            CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
            CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
            _tutorialLabel->runAction(aSeq1);
            
            _tutorialLabel->setVisible(true);
            
            //Make the screen dark and highlight dwarfs
            CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_4.png");
            aBG1->setAnchorPoint(ccp(0,0));
            aBG1->setOpacity(0);
            addChild(aBG1,kHUD_Z_Order,322);
            
            CCFadeTo* aFade1 = CCFadeTo::create(0.5f, 200);
            aBG1->runAction(aFade1);
            
            //Create the hand which will draw road to cave !!!
            CreateDrawHand(4);
            
            this->schedule( schedule_selector(GameScene::UpdateTutorialStuff) );
            break;
        }
            
        case 14:
        {
            if(getChildByTag(322))
            {
                //Fade it out while other fades in !!!
                CCFadeTo* aFadeOut = CCFadeTo::create(0.5f,0);
                CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                CCSequence* aSeqFade = CCSequence::create(aFadeOut,func,NULL);
                getChildByTag(322)->runAction(aSeqFade);
            }
            RemoveDrawHand();
            
            unschedule(schedule_selector(GameScene::UpdateTutorialStuff));
            
            playInGameSound("Footsteps");
            
            CCScaleTo* aScale1 = CCScaleTo::create(1.0f,0.0f);
            CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
            _tutorialLabel->runAction(aBounceScale1);
            
            _gamePause = false;
            resumeSchedulerAndActionsRecursive(this);
            
            break;
        }
            
        //Part III
        case 20:
        {
            //Spawn dwarfs
            generateDwarf(1,3);
//            generateDwarf(0,3);
            
//            _line = CCSpriteBatchNode::create("trajectory_dot_white.png");
//            addChild(_line);
//            
//            _movePoints = CCPointArray::create(0);
//            _movePoints->retain();
            
            break;
        }
        case 21:
        {
            //Spawn troll
            generateTrollSpecial(_screenSize.width/2+150,_screenSize.height/2-170);
            
            //Spaw
            mTutorialTimerSet = true;
            mTutorialTimer = 1.0f;
            
            break;
        }
        case 22:
        {
            generateEffectSpecial(_screenSize.width/2+28, _screenSize.height/2-170, 1);//For now
            
            
            break;
        }
        case 23:
        {
            //Freeze the game and show how to avoid
            //Pause the sounds
            stopInGameSound("Footsteps",true);
            stopInGameSound("troll_walk",true);
            
            //Collected crystal spawn diamond now
            _gamePause = true;
            pauseSchedulerAndActionsRecursive(this,false);
            //Let the dwarf create new line !!!
            
            //The standart stuff
            //Show text that we need to do this and this !!!
            _tutorialLabel->setString("Avoid stinky trolls and sticky meteorite craters");
            _tutorialLabel->setScale(0.0f);
            
            CCDelayTime* aDelay = CCDelayTime::create(0.25f);
            CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
            CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
            CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
            _tutorialLabel->runAction(aSeq1);
            
            _tutorialLabel->setVisible(true);
            
            //Make the screen dark and highlight dwarfs
            CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_5.png");
            aBG1->setAnchorPoint(ccp(0,0));
            aBG1->setOpacity(0);
            addChild(aBG1,kHUD_Z_Order,322);
            
            CCFadeTo* aFade1 = CCFadeTo::create(0.5f, 200);
            aBG1->runAction(aFade1);
            
            //Create the hand which will draw road to cave !!!
            CreateDrawHand(5);
            
            this->schedule( schedule_selector(GameScene::UpdateTutorialStuff) );
            
            break;
        }
            
        case 24:
        {
            if(getChildByTag(322))
            {
                //Fade it out while other fades in !!!
                CCFadeTo* aFadeOut = CCFadeTo::create(0.5f,0);
                CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                CCSequence* aSeqFade = CCSequence::create(aFadeOut,func,NULL);
                getChildByTag(322)->runAction(aSeqFade);
            }
            RemoveDrawHand();
            
            unschedule(schedule_selector(GameScene::UpdateTutorialStuff));
            
            playInGameSound("Footsteps");
            playInGameSound("troll_walk");
            
            CCScaleTo* aScale1 = CCScaleTo::create(1.0f,0.0f);
            CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
            _tutorialLabel->runAction(aBounceScale1);
            
            _gamePause = false;
            resumeSchedulerAndActionsRecursive(this);
            
            break;
        }
        
        case 25:
        {
//            mTutorialEnabled = false;
            
//            _gameTime = 0;
            
            //Save this !!!
            //            User::sh
//            User::getInstance()->finishTutorial();
            mTutorialStep = 29;//so that moves to next step
            break;
        }
            
        //PART IV
        case 30:
        {
            generateDwarf(0,6);
            
            _line = CCSpriteBatchNode::create("trajectory_dot_white.png");
            addChild(_line);
            
            _movePoints = CCPointArray::create(0);
            _movePoints->retain();
            
            mTutorialTimerSet = true;
            mTutorialTimer = 1.0f;
            
            //Check if power machine is ready !!!
            
            
            break;
        }
        case 31:
        {
            //Generate 3 monsters !!!
            generateTrollSpecial(_screenSize.width/2-230,_screenSize.height/2-20,4.35);
            generateTrollSpecial(_screenSize.width/2-80,_screenSize.height/2-100,3.65);
            generateTrollSpecial(_screenSize.width/2-90,_screenSize.height/2-220,3.2);
            
            //Watch out boys and girls
            mTutorialTimerSet = true;
            mTutorialTimer = 4.5f;
            
            break;
        }
        case 32:
        {
            //Freeze the game and show how to avoid
            //Pause the sounds
            stopInGameSound("Footsteps",true);
            stopInGameSound("troll_walk",true);
            
            //Collected crystal spawn diamond now
            _gamePause = true;
            pauseSchedulerAndActionsRecursive(this,false);
            //Let the dwarf create new line !!!
            
            //The standart stuff
            //Show text that we need to do this and this !!!
            _tutorialLabel->setString("Tap on Powerup Machine to activate it and Blitz Trolls");
            _tutorialLabel->setScale(0.0f);
            
            CCDelayTime* aDelay = CCDelayTime::create(0.25f);
            CCScaleTo* aScale1 = CCScaleTo::create(0.5f,1.0f);
            CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
            CCSequence* aSeq1 = CCSequence::create(aDelay,aBounceScale1,NULL);
            _tutorialLabel->runAction(aSeq1);
            
            _tutorialLabel->setVisible(true);
            
            _tutorialLabel2->setString("Activating powerups cost 5");
            _tutorialLabel2->setScale(0.0f);
            _tutorialLabel2->setPositionY(_screenSize.height-62);
            CCDelayTime* aDelay2 = CCDelayTime::create(0.25f);
            CCScaleTo* aScale2 = CCScaleTo::create(0.5f,1.0f);
            CCActionInterval* aBounceScale2 = CCEaseElasticOut::create(aScale2,0.5f);
            CCSequence* aSeq2 = CCSequence::create(aDelay2,aBounceScale2,NULL);
            _tutorialLabel2->runAction(aSeq2);
            _tutorialLabel2->setVisible(true);
            
            CCSprite* diamond_sprtie = CCSprite::create("diamond/diamond.png");
            diamond_sprtie->setScale(0.7f);
            diamond_sprtie->setTag(12345);
            diamond_sprtie->setPosition(ccp(_tutorialLabel2->getContentSize().width-183,16.5f));
            _tutorialLabel2->addChild(diamond_sprtie);
            
            
            //Make the screen dark and highlight dwarfs
            CCSprite* aBG1 = CCSprite::create("TutorialFakeMap_6.png");
            aBG1->setAnchorPoint(ccp(0,0));
            aBG1->setOpacity(0);
            addChild(aBG1,kHUD_Z_Order,322);
            
            CCFadeTo* aFade1 = CCFadeTo::create(0.5f, 200);
            aBG1->runAction(aFade1);
            
            //Create the hand which will draw road to cave !!!
            CreateDrawHand(6);
            
            this->schedule( schedule_selector(GameScene::UpdateTutorialStuff) );
            
            break;
        }
            
        case 33:
        {
            if(getChildByTag(322))
            {
                //Fade it out while other fades in !!!
                CCFadeTo* aFadeOut = CCFadeTo::create(0.5f,0);
                CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
                CCSequence* aSeqFade = CCSequence::create(aFadeOut,func,NULL);
                getChildByTag(322)->runAction(aSeqFade);
            }
            RemoveDrawHand();
            
            unschedule(schedule_selector(GameScene::UpdateTutorialStuff));
            
            playInGameSound("Footsteps");
            
            CCScaleTo* aScale1 = CCScaleTo::create(1.0f,0.0f);
            CCActionInterval* aBounceScale1 = CCEaseElasticOut::create(aScale1,0.5f);
            _tutorialLabel->runAction(aBounceScale1);
            
            CCScaleTo* aScale2 = CCScaleTo::create(1.0f,0.0f);
            CCActionInterval* aBounceScale2 = CCEaseElasticOut::create(aScale2,0.5f);
            _tutorialLabel2->runAction(aBounceScale2);
            
            _gamePause = false;
            resumeSchedulerAndActionsRecursive(this);
            
            
            mTutorialEnabled = false;
            
            _gameTime = 0;
            
            //Save this !!!
            //            User::sh
            User::getInstance()->finishTutorial();
            
            mShowTutorialCompleted = 3.0f;
            break;
        }
            
            //All finished
        default:
        {
            unschedule( schedule_selector(GameScene::UpdateTutorialStuff));
            
            if(getChildByTag(323))
                removeChildByTag(323);
            RemoveDrawHand();
            
            mTutorialEnabled = false;
            
            _gameTime = 0;
            
            //Save this !!!
//            User::sh
            User::getInstance()->finishTutorial();
            
            break;
        }
    }
    
    //Qucik update :)
    if(theStep-1 != mTutorialStep)
        mTutorialStep = theStep+1;
    else
        mTutorialStep++;
}

void GameScene::CreateCompleted()
{
//    _tutorialLabel2->setPosition(ccp(_screenSize.width/2,_screenSize.height-140));
    
    //Show text that tutorial is completed - and hide after 5sec?
    //            Congratulations, you have completed the tutorial!
    _tutorialLabel2->setString("Congratulations!\n You have mastered the basics of Dwarf Forest");
    _tutorialLabel2->setFontSize(TITLE_FONT_SIZE*0.5);
    _tutorialLabel2->setPositionY(_screenSize.height-130);
//    _tutorialLabel2->setPositionY(150);
    
    _tutorialLabel2->removeChildByTag(12345);
    _tutorialLabel2->setScale(0.0f);
    _tutorialLabel2->setVisible(true);
    
    CCScaleTo* aScale2 = CCScaleTo::create(1.0f,1.0f);
    CCActionInterval* aBounceScale2 = CCEaseElasticOut::create(aScale2,0.5f);
    _tutorialLabel2->runAction(aBounceScale2);
    
    schedule(schedule_selector(GameScene::RemoveCompletedTutorial), 1, 0, 4.0f);
}

void GameScene::RemoveCompletedTutorial()
{
    unschedule(schedule_selector(GameScene::RemoveCompletedTutorial));
    
    CCScaleTo* aScale2 = CCScaleTo::create(1.0f,0.0f);
    CCActionInterval* aBounceScale2 = CCEaseElasticOut::create(aScale2,0.5f);
    _tutorialLabel2->runAction(aBounceScale2);
}

void GameScene::FromOptionsEnableMusic()
{
    mFadeOutMusic = true;
    mBackMusicVolume = 0;
}

//The magic where all parametrs are taken in action and made a spawn plan
void GameScene::CreateSpawnLine()
{
    //Create the line spawn params
    _dwarfSpawnArr.clear();
    
    //Get the max spawn steps
    int aRealStepValue = _DSpawn_Real_Max-_DSpawn_Real_Min;
    int aMinStepJump = _DSpawn_Real_Jump;
    
    //Create the spawn zones
    int aCurrentPoint = _CurrentSpawnCount-1;//Take the last line spawn count
    
    //Min value for the same spawn value
    int aMinStall = mCurrentMission.Dwarf_paths.size();
    int aCurrentStallCount = 0;
    bool aDidHitMax = false;
    
//    CCLog("mCurrentMission.min %i",mCurrentMission.DSpawn_min);
//    CCLog("mCurrentMission.max %i",mCurrentMission.DSpawn_max);
//    CCLog("mCurrentMission.jump %i",mCurrentMission.DSpawn_jump);
    
    int aDummyVal = 0;
    int aJumpTimes = _DSpawn_Real_Zone/mCurrentMission.DSpawn_zone_step;
    
    bool aAlowOtherValue = true;
    
    for(int i=0;i<aJumpTimes;i++)
    {
//        CCLOG("-------------  i = %i --------",i);
        
        aDummyVal = rand()%2;//Up or down
//        CCLog("aDummyVal %i",aDummyVal);
        
        //now check if will get to the max
        if(aDidHitMax==false)
        {
            if(aCurrentPoint + (_DSpawn_Real_Jump*((aJumpTimes-i)-1)) < _DSpawn_Real_Max){
//                CCLOG("Did force spawn");
                aCurrentPoint+=_DSpawn_Real_Jump;
                aAlowOtherValue = false;
                
                if(aCurrentPoint>=_DSpawn_Real_Max) {aDidHitMax = true;}
            }
        }
        else if(aAlowOtherValue!=true){
            aAlowOtherValue = true;
        }
        
        if(aAlowOtherValue)
        {
            if(aDummyVal==0){
                if(aCurrentPoint==_DSpawn_Real_Min){
                    //Check if can have stall
                    aCurrentStallCount+=1;
                    if(aCurrentStallCount == aMinStall){
                        //Can be the same?
                        aCurrentStallCount = 0;//The value will be the same
                        if(aCurrentPoint-_DSpawn_Real_Jump>_DSpawn_Real_Min) aCurrentPoint-=_DSpawn_Real_Jump;
                    }
                    else{
                        //Move up or let it be the same if cant
                        if(aCurrentPoint+_DSpawn_Real_Jump<_DSpawn_Real_Max) aCurrentPoint+=_DSpawn_Real_Jump;
                    }
                }
                else{
                    aCurrentPoint-=_DSpawn_Real_Jump;
                }
            }
            else{
                if(aCurrentPoint==_DSpawn_Real_Max){
                    aCurrentStallCount+=1;
                    if(aCurrentStallCount == aMinStall){
                        //Can be the same?
                        aCurrentStallCount = 0;//The value will be the same
                        if(aCurrentPoint+_DSpawn_Real_Jump<_DSpawn_Real_Max) aCurrentPoint+=_DSpawn_Real_Jump;
                    }
                    else{
                        //Move up or let it be the same if cant
                        if(aCurrentPoint-_DSpawn_Real_Jump>_DSpawn_Real_Min) aCurrentPoint-=_DSpawn_Real_Jump;
                    }
                }
                else{
                    aCurrentPoint=aCurrentPoint;
                    aCurrentPoint+=_DSpawn_Real_Jump;
                    if(aCurrentPoint>=_DSpawn_Real_Max) aDidHitMax = true;
                }
            }
        }
        
        //Safe check
        if(aCurrentPoint<_DSpawn_Real_Min)aCurrentPoint=_DSpawn_Real_Min;
        else if(aCurrentPoint>_DSpawn_Real_Max)aCurrentPoint=_DSpawn_Real_Max;
        
        //Lets take for each second dwarf spawn
        _dwarfSpawnArr.push_back(aCurrentPoint);
    }
    
    
    //Print all spawns for debug
    CCLog("--------------------");
    
    for(int i=0;i<_dwarfSpawnArr.size();i++)
    {
        CCLog("Dwarf Min:%i",_dwarfSpawnArr[i]);
    }
    
    CCLog("====================");
    
    _DSpawnTimer = -1;//Force take this
    
    
    //Debug - show current spawn line
//    CCLayerColor* aLayer = CCLayerColor::create(ccc4(128,128,128,128),300,300);
//    aLayer->setAnchorPoint(ccp(1,1));
//    
//    addChild(aLayer,1000);
//    
//    CCSprite* aSprite = CCSprite::create("");
//    aSprite->draw();
    
//    aLayer->draw()->ccDrawLine(ccp(0,0),ccp(200,200));
//    ccDrawLine(ccp(0,0),ccp(200,200));
    
}

//The magic !!! [for now]
void GameScene::UpdateDwarfSpawn(float delta)
{
    //For now !!!
//    if(_dwarves->count()<mCurrentMission.DwarfCount_Min){
//        generateDwarfMission();
//    }
    
    //----------------------------------------------------------------------------
    //Check if does not need to change any parametr by time and value !!!
    
    if(_DSpawn_change_zone!=-1){
        //Check if time did not pass the req margin
        _DSpawn_change_zone-=delta*_gameSpeed;
//        CCLog("_DSpawn_change_zone: %f",_DSpawn_change_zone);
        if(_DSpawn_change_zone<=0){
            //Time for changes
            _DSpawn_change_zone = mCurrentMission.DSpawn_change_zone_time;//Reset to next time change if needed
            _DSpawn_Real_Zone += mCurrentMission.DSpawn_change_zone_value;
            _DSpawnCurrentTime = 0;//Force to change the spawn zone
        }
    }
    
    if(_DSpawn_change_jump!=-1){
        _DSpawn_change_jump-=delta*_gameSpeed;
        if(_DSpawn_change_jump<=0){
            //Time for changes
            _DSpawn_change_jump = mCurrentMission.DSpawn_change_jump_time;//Reset to next time change if needed
            _DSpawn_Real_Jump += mCurrentMission.DSpawn_change_jump_value;
//            _DSpawnCurrentTime = 0;//Force to change the spawn zone
        }
    }
    
    if(_DSpawn_change_max!=-1){
        _DSpawn_change_max-=delta*_gameSpeed;
//        CCLog("_DSpawn_change_max: %f",_DSpawn_change_max);
        if(_DSpawn_change_max<=0){
            //Time for changes
            _DSpawn_change_max = mCurrentMission.DSpawn_change_max_time;//Reset to next time change if needed
            _DSpawn_Real_Max += mCurrentMission.DSpawn_change_max_value;
//            CCLog("_DSpawn_Real_Max: %i",_DSpawn_Real_Max);
//            _DSpawnCurrentTime = 0;//Force to change the spawn zone
        }
    }
    
    if(_DSpawn_change_min!=-1){
        _DSpawn_change_min-=delta*_gameSpeed;
        if(_DSpawn_change_min<=0){
            //Time for changes
            _DSpawn_change_min = mCurrentMission.DSpawn_change_min_time;//Reset to next time change if needed
            _DSpawn_Real_Min += mCurrentMission.DSpawn_change_min_value;
//            _DSpawnCurrentTime = 0;//Force to change the spawn zone
        }
    }
    
    //----------------------------------------------------------------------------
    
    
    //Update the line of spawns
    _DSpawnCurrentTime-=delta*_gameSpeed;
//    CCLOG("_DSpawnCurrentTime: %f",_DSpawnCurrentTime);
    //Create the current zone wave
    if(_DSpawnCurrentTime<=0){
        //Time to create new line by mission parametrs
        _DSpawnCurrentTime = _DSpawn_Real_Zone;
        //By this zone make the spawns and other great stuff
        CreateSpawnLine();
    }
    
    //Get whats the min dwarfs in map now !!!
    _DSpawnTimer-=delta*_gameSpeed;
    if(_DSpawnTimer<=0){
        //Get current spawn stuff
        _DSpawnTimer = mCurrentMission.DSpawn_zone_step;//Set to 1sec
        if(_dwarfSpawnArr.size()>0){
            _DSpawnGameMinCurrent = _dwarfSpawnArr[0];
            
            if(mCurrentMission.DSpawn_formula_type == 1){
                //Check what are the diff
                _DSpawnGameMinCurrent = _DSpawnGameMinCurrent-_dwarves->count();
                if(_DSpawnGameMinCurrent<=0){
                    _DSpawnGameMinCurrent = 0;
                }
                else{
                    _dwarfSpawnArrTimers.clear();
                    
                    float aPossibleSpawnTImes = mCurrentMission.DSpawn_zone_step/_DSpawnGameMinCurrent;//The min random time
                    float aRandomTIme = 0;
                    for(int t = 0;t<_DSpawnGameMinCurrent;t++)
                    {
                        aRandomTIme = (rand() % int(aPossibleSpawnTImes*100))/100+(t*aPossibleSpawnTImes);
//                        CCLOG("Random time for spawn: %f",aRandomTIme);
                        _dwarfSpawnArrTimers.push_back(aRandomTIme);
                    }
                }
            }
            
            _CurrentSpawnCount = _DSpawnGameMinCurrent;
            _dwarfSpawnArr.erase(_dwarfSpawnArr.begin());
//            CCLOG("Current min dwarfs:%i",_DSpawnGameMinCurrent);
        }
    }
    
    if(mCurrentMission.DSpawn_formula_type == 0){
        if(_dwarves->count()<_DSpawnGameMinCurrent){
            generateDwarfMission(false);
        }
    }
    else if(mCurrentMission.DSpawn_formula_type == 1){
        //Create some random steps for spawn !!!
        if(_SubDwarfSpawn>0){
            _SubDwarfSpawn-=delta*_gameSpeed;
        }
        else{
            if(_dwarfSpawnArrTimers.size()>0){
                _SubDwarfSpawn = _dwarfSpawnArrTimers[0];
                _dwarfSpawnArrTimers.erase(_dwarfSpawnArrTimers.begin());
                generateDwarfMission(false);
            }
            else if(_LeftNotSpawnDwatfs>0){
                _LeftNotSpawnDwatfs-=1;
                generateDwarfMission(false);
            }
        }
    }
    else if(mCurrentMission.DSpawn_formula_type == 2){
        if(_DSpawnGameMinCurrent>0){
             generateDwarfMission(false);
        }
    }
    
    //Safe check
    if(_dwarves->count() == 0){
        generateDwarfMission(true);
    }
}

void GameScene::UpdateTrapsSpawn(float delta)
{
    //This is the master troll brain :D
    if(_MasterTroll_TimeToAct>0){
        _MasterTroll_TimeToAct-=delta*_gameSpeed;
    }
    else{
        //Do some magic now !!!
        _MasterTroll_TimeToAct = (rand()%20)+20;
        
        //What will he do?
        generateEffect();
        
        //2 other stuff
//        StartDwarfFreeze();
//        BulletDwarf();
    }
    
}

void GameScene::UpdateCrystalSpawn(float delta)
{
    _CrystalSpawnRecheckTimer-=delta*_gameSpeed;
    if(_CrystalSpawnRecheckTimer<=0)
    {
        //Lets get new timer value with min max value
        int aSpaceForRandom = mCurrentMission.CrystalInterval_Max - mCurrentMission.CrystalInterval_Min;
        if(aSpaceForRandom>0){
            _CrystalSpawnRecheckTimer = (rand()%aSpaceForRandom)+mCurrentMission.CrystalInterval_Min;
        }
        else{
            _CrystalSpawnRecheckTimer = rand()%mCurrentMission.CrystalInterval_Max+mCurrentMission.CrystalInterval_Min;
        }
        
        //Lets check futher !!!
        int aProbToSpawn = 100 - (_crystals->count() * mCurrentMission.CrystalProbMultiplier);
        int aRandomResult = rand()%100;
        if(aRandomResult<=aProbToSpawn){
            //Spawn
            //Lets check how much can we spawn
            int aRandomMaxSpawn = rand()%100;
            
            //Get the value how much can we
            int aSpawnCrystals = 0;
            for(int i=0;i<mCurrentMission.CrystalNumProbs.size();i++)
            {
                if(aRandomMaxSpawn<=mCurrentMission.CrystalNumProbs[i]){
                    aSpawnCrystals = i+1;
                }
            }
            
            //Now check what can we spawn for each crystal
            int aRandomColorFin = 0;//What type of crystal should spawn
            for(int a=0;a<aSpawnCrystals;a++)
            {
                int aRadomColor = rand()%100;
                
                for(int c=0;c<mCurrentMission.CrystalColProbs.size();c++)
                {
                    if(aRadomColor<=mCurrentMission.CrystalColProbs[c]){
                        aRandomColorFin = c;
                    }
                }
                
                //The green?
                generateCrystal(true,aRandomColorFin,mCurrentMission.CrystalTimeOnMap);
            }
        }
        
    }
}

void GameScene::update(float delta)
{
    _gameTime += delta * _gameSpeed;
    
    //Clean page for this !!!
    updateTrolls(delta);
    updateDwarfs(delta);
    updateIntroAnimations(delta);
    
    //The spawn contorl
    UpdateDwarfSpawn(delta);
    UpdateCrystalSpawn(delta);
    UpdateTrapsSpawn(delta);
    
    //Update the combo timer
    if(mTotalComboTimer>0)
    {
        mTotalComboTimer-=delta*_gameSpeed;
        if(mTotalComboTimer<=0)
        {
            mTotalComboTimer = 0;
            mTotalCombo = 1;
        }
    }
    
    if(mCurrentMission.Task_type){
        //Update by seconds
        if(mTotalTimeInGame != roundf(_gameTime)){
            mTotalTimeInGame = roundf(_gameTime);
            CheckMissionByValue(MissionType_Time,mTotalTimeInGame);
        }
    }
    
    return;
    
    
    
    /*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(mFmodEnabled)
        updateSound(delta);
#endif
    */
    
    if (_gamePause)
        return;//Wait a bit
//    CCLog("delta: %f",delta);
    
    if(mFadeOutMusic)
    {
        //Wtf !!!
        if(mFadeInMusic)
            mFadeInMusic = false;
        
        mBackMusicVolume-=delta;
        
        if(mBackMusicVolume<0)
            mBackMusicVolume = 0;
        
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(mBackMusicVolume);
        
        if(mBackMusicVolume==0)
            OnFinishedMusicFadeOut();
    }
    
    if(mFadeInMusic)
    {
        mBackMusicVolume+=delta;
        if(mBackMusicVolume>=0.8)
        {
            mBackMusicVolume = 0.8f;
            mFadeInMusic = false;
        }
        
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(mBackMusicVolume);
    }
    
    //-------------------------------
    // Check the combo sound play !!!
    
    if(mComboPlayDelayTimer>0)
    {
        mComboPlayDelayTimer-=delta;
        if(mComboPlayDelayTimer<=0)
        {
            //Play now the sound !!!
            mComboPlayDelayTimer = 0;
            
//            if(mComboSound_1.str().length()>0)
//                SimpleAudioEngine::sharedEngine()->playEffect(mComboSound_1.str().c_str(),false);
            if(mComboSound_2.str().length()>0)
                SimpleAudioEngine::sharedEngine()->playEffect(mComboSound_2.str().c_str(),false);
        }
    }
    
    //-------------------------------
    
	//update game time
	_gameTime += delta * _gameSpeed;
    
    mNoDwarfEneterCave+=delta;
    
    mTimeNotCollectedCrystal += delta;
    mTimeToCheckAgainNoCrystal += delta;
    
    if(mTimeToCheckAgainNoCrystal>=30)
    {
        mTimeToCheckAgainNoCrystal = 0.0f;
        User::getInstance()->getMissionManager().CheckSubMission(SUB_NO_CRYSTAL_COLLECT,int(mTimeNotCollectedCrystal));
    }
        
    
    if (_gameSlowTimer>0)
    {
        _gameSlowTimer-=delta;
    }
    else if (_gameSpeed<GAME_SPEED_NORMAL)
    {
        _gameSlowTimer = 0;
        _gameSpeed = GAME_SPEED_NORMAL;
    }
    
    //Debug safe checker
    _dwarfTimer+=delta;
    
    updateDwarfs(delta);
	updateTrolls(delta);
	updateCrystals(delta);
	updateIntroAnimations(delta);
    
    UpdateBullets(delta);
	
	//generate new objects
    double minutes = _gameTime / 60.0;
    
    //For now disabled as we will use different system !!!
    if(!mTutorialEnabled)
    {
//        ChangeMap
        mTimeToChangeSeason-=delta;
        if(mTimeToChangeSeason<=0)
        {
            //Change map after 1min
            mTimeToChangeSeason = 60;
            ChangeMap(0);//Goes in cycle
        }
    }
    /*
    if(!mTutorialEnabled)
    {
        if(mMapDangerCooldown>0)
        {
            //Wait for co
            mMapDangerCooldown-=delta;
            if(mMapDangerCooldown<=0)
                mMapDangerCooldown = 0;
        }
        else
        {
            mMapDangerTimer+=delta*_gameSpeed;
            if(mMapDangerTimer>=mMapDangerTimerReq)
            {
                mMapDangerTimerReq = (rand()%45)+45;
                mMapDangerTimer = 0;
                
                if(mRainActive || mTornadoActive || _effects->count()>0)
                {
                    //Cooldown
                    mMapDangerCooldown = 15;
                    
                    //Stop all other effects
                    StartRain(true);
                    StartTornado(true);
                }
                else
                {
                    //Stop all other effects
                    StartRain(true);
                    StartTornado(true);
                    
                    //What will be active now? 1st time all pec kartas :D
                    if(mFirstTimeMap && mMapDangerType<4)
                    {
                        mMapDangerType+=1;
                        if(mMapDangerType>=4)
                        {
                            mFirstTimeMap = false;
                        }
                    }
                    else
                    {
                        mMapDangerType = (rand()%3)+1;
                        
                        _mapSpawnTypes.clear();
                        for (int i = 1; i<4; i++)
                        {
                            if (i != mLastMapDangerType)
                                _mapSpawnTypes.push_back(i);
                        }
                        mMapDangerType = _mapSpawnTypes[rand()%_mapSpawnTypes.size()];
                    }
                    
                    if(mMapDangerType == 2)
                        StartRain(false);
                    else if(mMapDangerType == 3)
                        StartTornado(false);
                    
                    generateEffect();
                    
                    mLastMapDangerType = mMapDangerType;
                }
            }
        }
    }
    */
    
    //===============================
    //COMBO
    
//    if(mComboTimer_DwarfEnter>0)
//    {
//        mComboTimer_DwarfEnter-=delta;
//        if(mComboTimer_DwarfEnter<=0)
//        {
//            mComboTimer_DwarfEnter = 0;
//            mCombo_DwarfEnter = 1;
//        }
//    }
//    
//    if(mComboTimer_CollectCrystal>0)
//    {
//        mComboTimer_CollectCrystal-=delta;
//        if(mComboTimer_CollectCrystal<=0)
//        {
//            mComboTimer_CollectCrystal = 0;
//            mCombo_CollectCrystals = 1;
//        }
//    }
    
    //Disabled
    if(mTotalComboTimer>0)
    {
        mTotalComboTimer-=delta*_gameSpeed;
        if(mTotalComboTimer<=0)
        {
            mTotalComboTimer = 0;
            mTotalCombo = 1;
        }
    }
    
    //===============================
    
    if(mTutorialEnabled)
    {
        if(mTutorialStep<1)
            OnTutorialStepCompleted(1);
        
        if(mTutorialStep == 1)
        {
            //Spawn 2 dwarfs in correct positions
            
            //Freeze the game after 2sec - make screen dark
            if(_gameTime>=7.0f)
            {
                _gamePause = true;
                pauseSchedulerAndActionsRecursive(this,false);
                
                _gameTime = 0.0f;//Reset time
                //Make screen black and show draw hand !!!
                OnTutorialStepCompleted(2);
            }
        }
        else if(mTutorialStep<10)
        {
            if(mTutorialStep == 4)
            {
                //Start to count 2sec and spawn something new !!!
                if(_gameTime>=2.0f)
                {
                    _gamePause = true;
                    pauseSchedulerAndActionsRecursive(this,false);
                    
                    _gameTime = 0.0f;//Reset time
                    //Make screen black and show draw hand !!!
                    OnTutorialStepCompleted(5);//Spawn some crystal and troll
                }
            }
        }
        else if(mTutorialStep == 10)
        {
            //Genere
            OnTutorialStepCompleted(10);
        }
        else if(mTutorialStep == 11)
        {
            if(mTutorialTimerSet)
                 mTutorialTimer-=delta;
            
            if(mTutorialTimer<0 && mTutorialTimer!=-1)
            {
                OnTutorialStepCompleted(11);
                mTutorialTimer = -1;
                mTutorialTimerSet = false;
            }
        }
        else if(mTutorialStep == 15)
        {
            if(_dwarves->count()==0)
            {
                OnTutorialStepCompleted(20);//finish for now
            }
            
        }
        else if(mTutorialStep == 20)
        {
            //Create the dwartf
            OnTutorialStepCompleted(20);
        }
        else if(mTutorialStep == 22)
        {
            if(mTutorialTimerSet)
                mTutorialTimer-=delta;
            
            if(mTutorialTimer<0 && mTutorialTimer!=-1)
            {
                OnTutorialStepCompleted(22);
                mTutorialTimer = -1;
                mTutorialTimerSet = false;
            }
        }
        else if(mTutorialStep == 23)
        {
            if(mTutorialTimerSet)
                mTutorialTimer-=delta;
            
            if(mTutorialTimer<0 && mTutorialTimer!=-1)
            {
                OnTutorialStepCompleted(23);
                mTutorialTimer = -1;
                mTutorialTimerSet = false;
            }
        }
        else if(mTutorialStep == 30)
        {
            //Start the 3 part
            OnTutorialStepCompleted(30);
        }
        else if(mTutorialStep == 31)
        {
            if(mTutorialTimerSet)
                mTutorialTimer-=delta;
            
            if(mTutorialTimer<0 && mTutorialTimer!=-1)
            {
                mTutorialTimer = -1;
                mTutorialTimerSet = false;
                OnTutorialStepCompleted(31);
            }
        }
        else if(mTutorialStep == 32)
        {
            if(mTutorialTimerSet)
                mTutorialTimer-=delta;
            
            if(mTutorialTimer<0 && mTutorialTimer!=-1)
            {
                OnTutorialStepCompleted(32);
                mTutorialTimer = -1;
                mTutorialTimerSet = false;
            }
        }
        else
        {
            if(_dwarves->count()==0 && mTutorialStep<10)
            {
                OnTutorialStepCompleted(10);//finish for now
            }
            
            else if(_dwarves->count()==0 && mTutorialStep<20)
            {
                OnTutorialStepCompleted(20);//finish for now
            }
            else if(_dwarves->count()==0 && mTutorialStep == 25)
            {
                //Remove the effect what is on map
                Effect* effect = static_cast<Effect*>(_effects->objectAtIndex(0));
                //                removeEffect(effect);
                
                this->removeChild(effect);
                _effects->removeObjectAtIndex(0);
                
                OnTutorialStepCompleted(30);//25
            }
//            else if(_dwarves->count()==0 && mTutorialStep<30)
//            {
//                OnTutorialStepCompleted(30);//finish for now
//            }
            
//            if(_dwarves->count()==0 && mTutorialStep == 25)
//            {
//                //Remove the effect what is on map
//                Effect* effect = static_cast<Effect*>(_effects->objectAtIndex(0));
////                removeEffect(effect);
//                
//                this->removeChild(effect);
//                _effects->removeObjectAtIndex(0);
//                
//                OnTutorialStepCompleted(25);
//            }
        }
        
        UpdateTutorialStuff();
        
        return;//Do the tutorial stuff
    }
    
    if(mShowTutorialCompleted>0)
    {
        mShowTutorialCompleted-=delta;
        if(mShowTutorialCompleted<1)
        {
            mShowTutorialCompleted = -1;
            CreateCompleted();
        }
    }
    
//    if(mDebugFormulas)
//    {
//        CCLabelTTF* aLabelDebug = static_cast<CCLabelTTF*>(getChildByTag(20005));
//        
//        std::stringstream ss;
//        ss << minutes << _gameTime;
//        
//        aLabelDebug->setString(ss.c_str());
//    }
	
	// generate dwarfs
	//3x^0.5
    
    //For now disabled
//    mMinDwarfCountInTime = _gameTime/18;//How much dwarfs should be in game !!!
    
//	double dwarfProbability = 3.0 * pow(minutes, 0.5);
//	
//	double dwarfRandom = mod(rand(), 60.0 * 1000.0) / 1000.0;
//	
//	if (dwarfRandom < dwarfProbability * delta)
//	{
//		generateDwarf();
//	}
    
    
    if(User::getInstance()->mNewMissionBuild)
    {
        
    }
    else
    {
        /*
        if(mSpecialCrystalSpawnTimer>0)
        {
            mSpecialCrystalSpawnTimer-=delta;
            if(mSpecialCrystalSpawnTimer<=0)
            {
                if(!mTutorialEnabled)
                {
                    if(_gameTime<EXTRA_CRYSTAL_SPAWN)
                    {
                        if(_dwarves->count()>_crystals->count())
                        {
                            mSpecialCrystalSpawnTimer = 1.0f;
                            generateCrystal(true);
                        }
                    }
                    else
                    {
                        generateCrystal(true);
                    }
                }
                else
                {
                    mSpecialCrystalSpawnTimer = -1;
                    generateCrystal(true);
                }
            }
        }
        */
        
        //---------------------------------------------------------------
        // A bit different generate formula
        
        if(!_generatedExtraStartDwarf)
        {
            if(_gameTime>=6.0f)
            {
                _generatedExtraStartDwarf = true;
                generateDwarf();
            }
        }
        
        //Special stuff!!!
        if(floor(minutes)>2000)
        {
            int aDwarfCount = 1;
            //Special stuff
            if(_gameTime<30)
            {
                //Spawn like normal by one !!!
                aDwarfCount = 1;
            }
            else
            {
                if(_gameTime<60)
                    aDwarfCount = 2;
                else if(_gameTime<100)
                    aDwarfCount = 3;
                else
                    aDwarfCount = 2;
            }
            
            if (_dwarves->count()<aDwarfCount)
            {
                generateDwarf();
            }
        }
        else
        {
            if(_dwarfCurrentSpawnMinute != floor(minutes))
            {
                _dwarfCurrentSpawnMinute = floor(minutes);
                _dwarfCurrentActiveTime = -1;
                _dwarfCurrentTime = 0;
                
                _dwarfSpawnTimes.clear();
                
                //Check for patern stuff
                std::vector<int> _dwarfSpawnData1;
                std::vector<int> _dwarfSpawnData2;
                
                if(_dwarfCurrentSpawnMinute == 0)
                {
                    _dwarfSpawnData1.push_back(4);
                    _dwarfSpawnData1.push_back(3);
                    
                    _dwarfSpawnData2.push_back(45);
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_dwarfCurrentSpawnMinute == 1)
                {
                    _dwarfSpawnData1.push_back(2);
                    _dwarfSpawnData1.push_back(4);
                    _dwarfSpawnData1.push_back(2);
                    
                    _dwarfSpawnData2.push_back(15);
                    _dwarfSpawnData2.push_back(45);
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_dwarfCurrentSpawnMinute == 2)
                {
                    _dwarfSpawnData1.push_back(4);
                    _dwarfSpawnData1.push_back(4);
                    
                    _dwarfSpawnData2.push_back(30);
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_dwarfCurrentSpawnMinute == 3)
                {
                    _dwarfSpawnData1.push_back(3);
                    _dwarfSpawnData1.push_back(0);
                    _dwarfSpawnData1.push_back(3);
                    _dwarfSpawnData1.push_back(3);
                    
                    _dwarfSpawnData2.push_back(15);
                    _dwarfSpawnData2.push_back(30);
                    _dwarfSpawnData2.push_back(45);
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_dwarfCurrentSpawnMinute == 4)
                {
                    _dwarfSpawnData1.push_back(3);
                    _dwarfSpawnData1.push_back(3);
                    _dwarfSpawnData1.push_back(2);
                    _dwarfSpawnData1.push_back(1);
                    
                    _dwarfSpawnData2.push_back(15);
                    _dwarfSpawnData2.push_back(30);
                    _dwarfSpawnData2.push_back(45);
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_dwarfCurrentSpawnMinute >= 5)
                {
                    _dwarfSpawnData1.push_back(3);
                    _dwarfSpawnData1.push_back(1);
                    _dwarfSpawnData1.push_back(3);
                    _dwarfSpawnData1.push_back(3);
                    
                    _dwarfSpawnData2.push_back(15);
                    _dwarfSpawnData2.push_back(30);
                    _dwarfSpawnData2.push_back(45);
                    _dwarfSpawnData2.push_back(60);
                }
                
                if(!mPaternFormulas)
                {
                    float aRandomTime = 0;
                    float aLastTime = 0;
                    int aPossibleTimes = floor(7.0 * pow(minutes, 0.25));
                    if(mDebugInfoVisible)
                    {
                        CCLabelTTF* aLabelDebug = static_cast<CCLabelTTF*>(getChildByTag(10005));
                        aLabelDebug->setString(toString(aPossibleTimes).c_str());
                        
                        int aMin = minutes;
                        aLabelDebug = static_cast<CCLabelTTF*>(getChildByTag(10006));
                        aLabelDebug->setString(toString(aMin).c_str());
                    }
                    //--------------------------------
                    
                    if(aPossibleTimes>0)
                    {
                        int aStartTime = floor(60/aPossibleTimes);
                        for(int i=0;i<aPossibleTimes;i++)
                        {
                            if(aStartTime<=0)
                                aRandomTime = rand()%aStartTime+aStartTime;
                            else
                                aRandomTime = (rand()%1)*0.1+aStartTime;
                            
                            aLastTime+=aRandomTime;
                            if(_dwarfCurrentActiveTime==-1)
                                _dwarfCurrentActiveTime = aLastTime;
                            else
                                _dwarfSpawnTimes.push_back(aLastTime);
                        }
                    }
                }
                else
                {
                    int aSteps = _dwarfSpawnData1.size();
                    
                    int aTimeInterval = 45;
                    int aTotalDwarfs = 8;
                    int aStartTime = 0;//floor(aTimeInterval/aTotalDwarfs);
                    int aTotalTimeAdd = 0;
                    
                    //                int aExtraParam = 0;
                    
                    for(int x=0;x<aSteps;x++)
                    {
                        //Get the detailed info
                        if(x>0)
                            aTimeInterval = _dwarfSpawnData2[x]-_dwarfSpawnData2[x-1];
                        else
                            aTimeInterval = _dwarfSpawnData2[x];
                        
                        aTotalDwarfs = _dwarfSpawnData1[x];
                        if(aTotalDwarfs==0)
                            continue;
                        
                        aStartTime = floor(aTimeInterval/aTotalDwarfs);
                        if(aStartTime<=0)
                            continue;
                        
                        if(x>0)
                            aTotalTimeAdd=_dwarfSpawnData2[x]-aTimeInterval;
                        
                        for(int i=0;i<aTotalDwarfs;i++)
                        {
                            if(aStartTime*0.5>0)
                                aTotalTimeAdd+=rand()%aStartTime*0.5+aStartTime*0.5+1;
                            else
                                aTotalTimeAdd+=rand()%aStartTime;
                            
                            if(_dwarfCurrentActiveTime==-1)
                                _dwarfCurrentActiveTime = aTotalTimeAdd;
                            else
                                _dwarfSpawnTimes.push_back(aTotalTimeAdd);
                        }
                    }
                }
            }
            else
            {
                _dwarfCurrentTime += delta * _gameSpeed;
                
                if(_dwarfCurrentTime>=_dwarfCurrentActiveTime)
                {
                    //Spawn effect and get the next !!!
                    if(_dwarfSpawnTimes.size()>0)
                    {
                        _dwarfCurrentActiveTime = _dwarfSpawnTimes[0];
                        _dwarfSpawnTimes.erase(_dwarfSpawnTimes.begin(),_dwarfSpawnTimes.begin()+1);
                    }
                    else
                    {
                        _dwarfCurrentActiveTime = 1000;//Never come here again
                    }
                    //Create the troll long awaited
                    generateDwarf();
                }
            }
        }
        
        //---------------------------------------------------------------
    }
    
    
	// generate trolls
	//2x^0.2
//	double trollProbability = 2.0 * pow(minutes, 0.2);
//	
//	double trollRandom = mod(rand(), 60.0 * 1000.0) / 1000.0;
//	
//	if (trollRandom < trollProbability * delta)
//	{
//		generateTroll();
//	}
    
    //---------------------------------------------------------------
    // A bit different generate formula
    
    if (_trollCurrentSpawnMinute != floor(minutes))
    {
        _trollCurrentSpawnMinute = floor(minutes);
        _trollCurrentActiveTime = -1;
        _trollCurrentTime = 0;
        
        _trollSpawnTimes.clear();
        
//        bool aDisableFormula = true;
        if(mPaternFormulas)
        {
            std::vector<int> _dwarfSpawnData1;
            std::vector<int> _dwarfSpawnData2;
            
            if(_trollCurrentSpawnMinute == 0)
            {
                _dwarfSpawnData1.push_back(2);
                _dwarfSpawnData1.push_back(0);
                
                _dwarfSpawnData2.push_back(45);
                _dwarfSpawnData2.push_back(60);
            }
            else if(_trollCurrentSpawnMinute == 1)
            {
                _dwarfSpawnData1.push_back(0);
                _dwarfSpawnData1.push_back(1);
                _dwarfSpawnData1.push_back(1);
                
                _dwarfSpawnData2.push_back(15);
                _dwarfSpawnData2.push_back(45);
                _dwarfSpawnData2.push_back(60);
            }
            else if(_trollCurrentSpawnMinute == 2)
            {
                _dwarfSpawnData1.push_back(2);
                
                _dwarfSpawnData2.push_back(60);
            }
            else if(_trollCurrentSpawnMinute == 3)
            {
                _dwarfSpawnData1.push_back(2);
                
                _dwarfSpawnData2.push_back(60);
            }
            else if(_trollCurrentSpawnMinute == 4)
            {
                _dwarfSpawnData1.push_back(3);
                
                _dwarfSpawnData2.push_back(60);
            }
            else if(_trollCurrentSpawnMinute >= 5)
            {
                _dwarfSpawnData1.push_back(3);
                _dwarfSpawnData1.push_back(0);
                _dwarfSpawnData1.push_back(0);
                
                _dwarfSpawnData2.push_back(30);
                _dwarfSpawnData2.push_back(45);
                _dwarfSpawnData2.push_back(60);
            }
            
            int aSteps = _dwarfSpawnData1.size();
            
            int aTimeInterval = 45;
            int aTotalDwarfs = 8;
            int aStartTime = 0;//floor(aTimeInterval/aTotalDwarfs);
            int aTotalTimeAdd = 0;
            
            for(int x=0;x<aSteps;x++)
            {
                //Get the detailed info
                if(x>0)
                    aTimeInterval = _dwarfSpawnData2[x]-_dwarfSpawnData2[x-1];
                else
                    aTimeInterval = _dwarfSpawnData2[x];
                
                aTotalDwarfs = _dwarfSpawnData1[x];
                if(aTotalDwarfs==0)
                    continue;
                
                aStartTime = floor(aTimeInterval/aTotalDwarfs);
                if(aStartTime<=0)
                    continue;
                
                if(x>0)
                    aTotalTimeAdd=_dwarfSpawnData2[x]-aTimeInterval;
                
                for(int i=0;i<aTotalDwarfs;i++)
                {
                    if(aStartTime*0.5>0)
                        aTotalTimeAdd+=rand()%aStartTime*0.5+aStartTime*0.5+1;
                    else
                        aTotalTimeAdd+=rand()%aStartTime;
                    
                    if(_trollCurrentActiveTime==-1)
                        _trollCurrentActiveTime = aTotalTimeAdd;
                    else
                        _trollSpawnTimes.push_back(aTotalTimeAdd);
                }
            }
        }
        else
        {
            int aRandomTime = 0;
            int aLastTime = 0;
            int aPossibleTimes = floor(2 * pow(minutes, 0.45));
            //        if (aPossibleTimes>MAX_TROLLS)
            //            aPossibleTimes = MAX_TROLLS;
            
            if (aPossibleTimes>0)
            {
                int aStartTime = floor(60/aPossibleTimes);
                for(int i=0;i<aPossibleTimes;i++)
                {
                    aRandomTime = rand()%aStartTime+aStartTime;
                    aLastTime+=aRandomTime;
                    if (_trollCurrentActiveTime==-1)
                        _trollCurrentActiveTime = aLastTime;
                    else
                        _trollSpawnTimes.push_back(aLastTime);
                }
            }
        }
    }
    else
    {
        _trollCurrentTime += delta * _gameSpeed;
        
        if (_trollCurrentTime>=_trollCurrentActiveTime)
        {
            //Spawn effect and get the next !!!
            if (_trollSpawnTimes.size()>0)
            {
                _trollCurrentActiveTime = _trollSpawnTimes[0];
                _trollSpawnTimes.erase(_trollSpawnTimes.begin(),_trollSpawnTimes.begin()+1);
            }
            else
            {
                _trollCurrentActiveTime = 1000;//Never come here again
            }
            //Create the troll long awaited
            
            if(User::getInstance()->mSpecial_10_Mission || User::getInstance()->mSpecial_13_Mission || User::getInstance()->mSpecial_14_Mission
               || User::getInstance()->mNewMissionBuild)
            {
                //Nope
                
            }
            else
            {
                generateTroll();
            }
        }
    }
    
    if(_SpawnSpecialTrolls>0)
    {
        _SpawnSpecialTrolls-=delta*_gameSpeed;
        if(_SpawnSpecialTrolls<=0)
        {
            if(User::getInstance()->mSpecial_18_Mission || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission
               || User::getInstance()->mSpecial_21_Mission || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission)
            {
                //Check if this troll is active
                bool aFoundTroll1 = false;
                bool aFoundTroll2 = false;
                
                for (int effectIndex = _trolls->count() - 1; effectIndex >= 0; --effectIndex)
                {
                    Troll* trollIn = static_cast<Troll*>(_trolls->objectAtIndex(effectIndex));
                    if (trollIn != NULL)
                    {
                        if(trollIn->mDrawLineID==33131){
                            aFoundTroll1 = true;
                        }
                        else if(trollIn->mDrawLineID==33132){
                            aFoundTroll2 = true;
                        }
                    }
                }
                
                if(aFoundTroll1==false){
                    generateTrollMission(_screenSize.width/2,_screenSize.height-180,1.6,false,true,
                                         ccp(240,_screenSize.height-180),ccp(_screenSize.width-240,_screenSize.height-180),1);
                }
                if(aFoundTroll2==false){
                    //Check if can do that
                    if(User::getInstance()->mSpecial_21_Mission){
                        //Do nothing - only 1 troll allowed
                    }
                    else{
                        generateTrollMission(_screenSize.width/2,160,1.6,false,true,
                                             ccp(240,160),ccp(_screenSize.width-240,160),2);
                    }
                }
            }
            //Check what need to spawn
            else if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission)
            {
                SetMasterTrollAction(1);
                
                //The troll spawn
                if(_DwarfsSpawned>=10)
                {
                    generateTrollMission(_screenSize.width/2,160,1.6,false,true,
                                         ccp(240,160),ccp(_screenSize.width-240,160),2);
                }
                else if(_DwarfsSpawned>=20)
                {
                    generateTrollMission(240,_screenSize.height/2,1.6,false,true,ccp(240,_screenSize.height-100),ccp(240,100),3);
                }
                else if(_DwarfsSpawned>=30)
                {
                    generateTrollMission(_screenSize.width-240,_screenSize.height/2,1.6,false,true,
                                         ccp(_screenSize.width-240,_screenSize.height-100),ccp(_screenSize.width-240,100),4);
                }
                else if(_DwarfsSpawned>=40)
                {
                    generateTrollMission(_screenSize.width/2,200,0,true,false,CCPointZero,CCPointZero,5);
                }
            }
            else
            {
                //The special stuff
                generateTrollMission(240,_screenSize.height/2,1.6,false,true,ccp(240,_screenSize.height-100),ccp(240,100),-1);
                generateTrollMission(_screenSize.width-240,_screenSize.height/2,4.7,false,true,
                                     ccp(_screenSize.width-240,100),ccp(_screenSize.width-240,_screenSize.height-100),-1);
                generateTrollMission(_screenSize.width/2,200,0,true,false,CCPointZero,CCPointZero,-1);
            }
            
//            generateTrollMission(240,_screenSize.height/2-160,M_PI,false,false,CCPointZero,CCPointZero);
//            generateTrollMission(_screenSize.width-240,_screenSize.height/2+160,0,false,false,CCPointZero,CCPointZero);
        }
    }
    
    if(_boostNoEnemyTimer>0)
    {
        //Some special effect !!!
        _actionTrollSpawn+=delta*_gameSpeed;
        if(_actionTrollSpawnTime == -1)
        {
            _actionTrollSpawnTime = _boostNoEnemyTimer/2;
        }
        
        if(_actionTrollSpawn>=_actionTrollSpawnTime)
        {
            _actionTrollSpawn = 0;
            
            if(_SpawnSpecialTrolls>0){
                
            }
            else
            {
                if(User::getInstance()->mNewMissionBuild == false){
                    generateTroll(true);
                }
            }
        }
    }
    
    //---------------------------------------------------------------
	
	// generate chrystals
	//2x^0.5
//	double chrystalProbability = 2.0 * pow(minutes, 0.5);
//	
//	double chrystalRandom = mod(rand(), 60.0 * 1000.0) / 1000.0;
//	
//	if (chrystalRandom < chrystalProbability * delta)
//	{
//		generateCrystal();
//	}
	
    //---------------------------------------------------------------
    // A bit different crystal generate formula
    
    if(User::getInstance()->mNewMissionBuild)
    {
        //Different crystal spawn mode
        
    }
    else
    {
        if (_crystalCurrentSpawnMinute != floor(minutes))
        {
            _crystalCurrentSpawnMinute = floor(minutes);
            _crystalCurrentActiveTime = -1;
            _crystalCurrentTime = 0;
            
            _crystalSpawnTimes.clear();
            
            //        bool aDisableFormula = true;
            if(mPaternFormulas)
            {
                std::vector<int> _dwarfSpawnData1;
                std::vector<int> _dwarfSpawnData2;
                
                if(_crystalCurrentSpawnMinute == 0)
                {
                    _dwarfSpawnData1.push_back(4);
                    
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_crystalCurrentSpawnMinute == 1)
                {
                    _dwarfSpawnData1.push_back(3);
                    _dwarfSpawnData1.push_back(2);
                    
                    _dwarfSpawnData2.push_back(15);
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_crystalCurrentSpawnMinute == 2)
                {
                    _dwarfSpawnData1.push_back(5);
                    
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_crystalCurrentSpawnMinute == 3)
                {
                    _dwarfSpawnData1.push_back(0);
                    _dwarfSpawnData1.push_back(0);
                    _dwarfSpawnData1.push_back(2);
                    _dwarfSpawnData1.push_back(2);
                    
                    _dwarfSpawnData2.push_back(15);
                    _dwarfSpawnData2.push_back(30);
                    _dwarfSpawnData2.push_back(45);
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_crystalCurrentSpawnMinute == 4)
                {
                    _dwarfSpawnData1.push_back(0);
                    _dwarfSpawnData1.push_back(1);
                    _dwarfSpawnData1.push_back(3);
                    _dwarfSpawnData1.push_back(3);
                    
                    _dwarfSpawnData2.push_back(15);
                    _dwarfSpawnData2.push_back(30);
                    _dwarfSpawnData2.push_back(45);
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_crystalCurrentSpawnMinute >= 5)
                {
                    _dwarfSpawnData1.push_back(5);
                    
                    _dwarfSpawnData2.push_back(60);
                }
                
                int aSteps = _dwarfSpawnData1.size();
                
                int aTimeInterval = 45;
                int aTotalDwarfs = 8;
                int aStartTime = 0;//floor(aTimeInterval/aTotalDwarfs);
                int aTotalTimeAdd = 0;
                
                for(int x=0;x<aSteps;x++)
                {
                    //Get the detailed info
                    if(x>0)
                        aTimeInterval = _dwarfSpawnData2[x]-_dwarfSpawnData2[x-1];
                    else
                        aTimeInterval = _dwarfSpawnData2[x];
                    
                    aTotalDwarfs = _dwarfSpawnData1[x];
                    if(aTotalDwarfs==0)
                        continue;
                    
                    aStartTime = floor(aTimeInterval/aTotalDwarfs);
                    if(aStartTime<=0)
                        continue;
                    
                    if(x>0)
                        aTotalTimeAdd=_dwarfSpawnData2[x]-aTimeInterval;
                    
                    for(int i=0;i<aTotalDwarfs;i++)
                    {
                        if(aStartTime*0.5>0)
                            aTotalTimeAdd+=rand()%aStartTime*0.5+aStartTime*0.5+1;
                        else
                            aTotalTimeAdd+=rand()%aStartTime;
                        
                        if(_crystalCurrentActiveTime==-1)
                            _crystalCurrentActiveTime = aTotalTimeAdd;
                        else
                            _crystalSpawnTimes.push_back(aTotalTimeAdd);
                    }
                }
            }
            else
            {
                int aRandomTime = 0;
                int aLastTime = 0;
                int aPossibleTimes = floor(4.0 * pow(minutes, 0.25));
                //        if (aPossibleTimes>MAX_CRYSTALS)
                //            aPossibleTimes = MAX_CRYSTALS;
                if (aPossibleTimes>0)
                {
                    int aStartTime = floor(60/aPossibleTimes);
                    for(int i=0;i<aPossibleTimes;i++)
                    {
                        aRandomTime = rand()%aStartTime+aStartTime;
                        aLastTime+=aRandomTime;
                        if (_crystalCurrentActiveTime==-1)
                            _crystalCurrentActiveTime = aLastTime;
                        else
                            _crystalSpawnTimes.push_back(aLastTime);
                    }
                }
            }
        }
        else
        {
            _crystalCurrentTime += delta * _gameSpeed;
            
            if (_crystalCurrentTime>=_crystalCurrentActiveTime)
            {
                //Spawn effect and get the next !!!
                if (_crystalSpawnTimes.size()>0)
                {
                    _crystalCurrentActiveTime = _crystalSpawnTimes[0];
                    _crystalSpawnTimes.erase(_crystalSpawnTimes.begin(),_crystalSpawnTimes.begin()+1);
                }
                else
                {
                    _crystalCurrentActiveTime = 1000;//Never come here again
                }
                //Create the troll long awaited
                
                /*
                if(_gameTime>=EXTRA_CRYSTAL_SPAWN)
                    generateCrystal(false);
                */
            }
        }
        
        //---------------------------------------------------------------
    }
    
	// generate diamonds
	//1/5
//	double diamondProbability = 1 / 5;
//	
//	double diamondRandom = mod(rand(), 60.0 * 1000.0) / 1000.0;
//	
//	if (diamondRandom < diamondProbability * delta)
//	{
//		generateDiamond();
//	}
    // updated diamonds - 1 diamond in 5 min
    _diamondTimer+=delta * _gameSpeed;
    if (_diamondTimer>=DIAMOND_SPAWN_TIME)
    {
        _diamondTimer = 0;//Start again
        generateDiamond();
    }
	
	// generate effect
	//2x^0.2
//	double effectProbability = 2.0 * pow(minutes, 0.2);//Current min max times can spawn
//    CCLog("effectProbability:%f || time in game%f",effectProbability,minutes);
    
    if(User::getInstance()->mNewMissionBuild == false)
    {
        if (_currentMinForSpawn != floor(minutes))
        {
            _currentMinSpwanedTime = 0;//Start again from 0
            //Reupdate the spawn timers !!!
            _currentMinForSpawn = floor(minutes);
            //Set the spawn times in this minute - so that does all the needed stuff in this minute !!!
            _effectSpawnTimes.clear();
            
            //How to make nice random !!!
            _effectActiveSpawnTime = -1;
            
            //        bool aDisableFormula = true;
            if(mPaternFormulas)
            {
                std::vector<int> _dwarfSpawnData1;
                std::vector<int> _dwarfSpawnData2;
                
                if(_currentMinForSpawn == 0)
                {
                    _dwarfSpawnData1.push_back(1);
                    _dwarfSpawnData1.push_back(0);
                    
                    _dwarfSpawnData2.push_back(45);
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_currentMinForSpawn == 1)
                {
                    _dwarfSpawnData1.push_back(1);
                    _dwarfSpawnData1.push_back(1);
                    
                    _dwarfSpawnData2.push_back(15);
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_currentMinForSpawn == 2)
                {
                    _dwarfSpawnData1.push_back(1);
                    _dwarfSpawnData1.push_back(0);
                    _dwarfSpawnData1.push_back(1);
                    
                    _dwarfSpawnData2.push_back(30);
                    _dwarfSpawnData2.push_back(45);
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_currentMinForSpawn == 3)
                {
                    _dwarfSpawnData1.push_back(2);
                    
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_currentMinForSpawn == 4)
                {
                    _dwarfSpawnData1.push_back(2);
                    
                    _dwarfSpawnData2.push_back(60);
                }
                else if(_currentMinForSpawn >= 5)
                {
                    _dwarfSpawnData1.push_back(0);
                    _dwarfSpawnData1.push_back(1);
                    _dwarfSpawnData1.push_back(2);
                    _dwarfSpawnData1.push_back(0);
                    
                    _dwarfSpawnData2.push_back(15);
                    _dwarfSpawnData2.push_back(30);
                    _dwarfSpawnData2.push_back(45);
                    _dwarfSpawnData2.push_back(60);
                }
                
                int aSteps = _dwarfSpawnData1.size();
                
                int aTimeInterval = 45;
                int aTotalDwarfs = 8;
                int aStartTime = 0;//floor(aTimeInterval/aTotalDwarfs);
                int aTotalTimeAdd = 0;
                
                for(int x=0;x<aSteps;x++)
                {
                    //Get the detailed info
                    if(x>0)
                        aTimeInterval = _dwarfSpawnData2[x]-_dwarfSpawnData2[x-1];
                    else
                        aTimeInterval = _dwarfSpawnData2[x];
                    
                    aTotalDwarfs = _dwarfSpawnData1[x];
                    if(aTotalDwarfs==0)
                        continue;
                    
                    aStartTime = floor(aTimeInterval/aTotalDwarfs);
                    if(aStartTime<=0)
                        continue;
                    
                    if(x>0)
                        aTotalTimeAdd=_dwarfSpawnData2[x]-aTimeInterval;
                    
                    for(int i=0;i<aTotalDwarfs;i++)
                    {
                        if(aStartTime*0.5>0)
                            aTotalTimeAdd+=rand()%aStartTime*0.5+aStartTime*0.5+1;
                        else
                            aTotalTimeAdd+=rand()%aStartTime;
                        
                        if(_effectActiveSpawnTime==-1)
                            _effectActiveSpawnTime = aTotalTimeAdd;
                        else
                            _effectSpawnTimes.push_back(aTotalTimeAdd);
                        
                        //                    CCLog("Effect will spawn at %d",aTotalTimeAdd);
                    }
                }
            }
            else
            {
                int aRandomTime = 0;
                int aLastTime = 0;
                int aPossibleTimes = floor(2.0 * pow(minutes, 0.45));//How much in this minute will we spawn!
                //        if (aPossibleTimes>MAX_EFFECTS)
                //            aPossibleTimes = MAX_EFFECTS;
                if (aPossibleTimes>0)
                {
                    int aStartTime = floor(60/aPossibleTimes);
                    for(int i=0;i<aPossibleTimes;i++)
                    {
                        aRandomTime = rand()%aStartTime+aStartTime;
                        aLastTime+=aRandomTime;
                        if (_effectActiveSpawnTime==-1)
                            _effectActiveSpawnTime = aLastTime;
                        else
                            _effectSpawnTimes.push_back(aLastTime);
                    }
                }
            }
        }
        else
        {
            _currentMinSpwanedTime += delta * _gameSpeed;
            
            if (_currentMinSpwanedTime>=_effectActiveSpawnTime)
            {
                //            CCLog("_currentMinSpwanedTime %f _effectActiveSpawnTime:%d",_currentMinSpwanedTime,_effectActiveSpawnTime);
                //Spawn effect and get the next !!!
                if (_effectSpawnTimes.size()>0)
                {
                    _effectActiveSpawnTime = _effectSpawnTimes[0];
                    _effectSpawnTimes.erase(_effectSpawnTimes.begin(),_effectSpawnTimes.begin()+1);
                    
                    
                }
                else
                {
                    _effectActiveSpawnTime = 1000;//Never come here again
                }
                
                
                
                //Create the effect long awaited
                generateEffect();
            }
        }
    }
    
    //Check when will spawn the effect in this minute !!!
    
    //Callculate how many times can spawn in this min !!
    
//	double effectRandom = mod(rand(), 60.0 * 1000.0) / 1000.0;
//	
//	if (effectRandom < effectProbability * delta)
//	{
//        generateEffect();
//	}
    //////////////////////
    // New meteorite formula
    
//    mEffectTime+=delta * gameSpeed;
    
    
    //////////////////////
	
	// if there are no dwarves on scene
//	if (_dwarves->count() == 0 || _dwarves->count() < mMinDwarfCountInTime)
    
    //Use the old functional
//    if (_dwarves->count() == 0 && floor(minutes)>=2)
//    if (_dwarves->count() == 0)
//	{
//        generateDwarf();
//        //Generate some crystal too !!!
//        
////        if(_crystals->count()<2)
////            generateCrystal();
//	}
    if(User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission){
        if (_dwarves->count() < 3){
            generateDwarf();
        }
    }
    else if(User::getInstance()->mSpecial_21_Mission){
        if(_dwarves->count()<2){
            generateDwarf();
        }
        
        if(_goblins->count()<1){
            //Choose from what spot will come in !!!
            int aRandomSpot = rand()%_genearetPoints.size();
            GeneratePoint gobSpawn = _genearetPoints[aRandomSpot];
            
            generateGoblin(gobSpawn.x,gobSpawn.y,gobSpawn.angle);
        }
    }
    else if(User::getInstance()->mSpecial_22_Mission){
        if(_gameTime>180 && _hidras->count()<2 && _hidras->count()>0){
            //Spawn other hidra
            generateHidra(_screenSize.width/2+200,450,0);
        }
        else if(_hidras->count()<1)
        {
            //Generate the 1st hidra
            generateHidra(_screenSize.width/2+200,200,0);
        }
    }
    else if(User::getInstance()->mSpecial_23_Mission){
        //3 spawn points
        
    }
    else{
        if (_dwarves->count() == 0){
            generateDwarf();
        }
    }
	
	// TODO: remove
//	if (_trolls->count() == 0)
//	{
//		generateTroll();
//	}
//	
//	if (_crystals->count() == 0)
//	{
//		generateCrystal();
//	}
//	
//	if (_effects->count() == 0)
//	{
//		generateEffect();
//	}
//	
//	if (_diamonds->count() == 0)
//	{
//		generateDiamond();
//	}
    
    //========================================
    //Booster update
    
    if (_boostGhostTimer>0)
    {
        _boostGhostTimer-=delta;
        if (_boostGhostTimer<=0)
        {
            disableBooster(kBooster_Ghost);
            _plantIceFlowerFirst = false;
        }
    }
        
//    if (mBoost_Shield_Timer>0)
//    {
//        mBoost_Shield_Timer-=delta;
//        if (mBoost_Shield_Timer<=0)
//            disableBooster(kBooster_Shield);
//    }
    
    if (_boostFutureSee>0)
    {
        _boostFutureSee-=delta;
        
        if (_boostFutureSee<=0)
        {
            _boostFutureSee = 0;
            disableBooster(kBooster_FutureSee);
        }
    }
    
    if (_boostCrystalsTimer>0)
    {
        _boostCrystalsTimer-=delta;
        if (_boostCrystalsTimer<=0)
        {
            _boostExtraPoints = 0;
            disableBooster(kBooster_Crystals);
        }
    }
    
    if (_boostNoEnemyTimer>0)
    {
        _boostNoEnemyTimer-=delta;
        if (_boostNoEnemyTimer<=0)
        {
            _plantSunFlowerFirst = false;//Create again the plant
            disableBooster(kBooster_NoEnemy);
//          crossFadeBackground(false);
            
            _SpawnSpecialTrolls = 10;
        }
    }
    
    if (_mushroomTimer>0)
    {
        _mushroomTimer-=delta;
        if (_mushroomTimer<=0)
        {
            _mushroomTimer = MUSHROOM_WAIT;
            generateMushroom();
        }
    }
    
    //========================================
    
    
    //New stuff - plants stuff !!!
    //Check if enabled
    updatePlants(delta);
    
    if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission
       || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
       || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission)
    {
        //We have mission 16 active !!!
        /*
        _mission_dwarf_spawn_timer-=delta;
        if(_mission_dwarf_spawn_timer<=0)
        {
            //new stuff
            int aWillSpawn = 0;
            bool spawnDwarf = true;
            
            if(_dwarves->count()<3)
            {
                _mission_dwarf_spawn_timer = (rand()%2)+2;
            }
            else if(_dwarves->count() == 3)
            {
                aWillSpawn = rand()%10;
                
                if(aWillSpawn<4){
                    _mission_dwarf_spawn_timer = (rand()%3)+8;
                }
                else{
                    spawnDwarf = false;
                    _mission_dwarf_spawn_timer = (rand()%2)+2;
                }
            }
            else
            {
                aWillSpawn = rand()%10;
                
                if(aWillSpawn<2){
                    _mission_dwarf_spawn_timer = (rand()%3)+8;
                }
                else{
                    spawnDwarf = false;
                    _mission_dwarf_spawn_timer = (rand()%2)+2;
                }
            }
            
            if(spawnDwarf){
                generateDwarf();
            }
        }
        */
        
        //---------------------------------
        
        if(_dwarfCurrentSpawnMinute != floor(minutes))
        {
            _dwarfCurrentSpawnMinute = floor(minutes);
            _dwarfCurrentActiveTime = -1;
            _dwarfCurrentTime = 0;
            
            _dwarfSpawnTimes.clear();
            
            float aRandomTime = 0;
            float aLastTime = 0;
            int aPossibleTimes = floor(7.0 * pow(minutes, 0.25));
            if(mDebugInfoVisible)
            {
                CCLabelTTF* aLabelDebug = static_cast<CCLabelTTF*>(getChildByTag(10005));
                aLabelDebug->setString(toString(aPossibleTimes).c_str());
                
                int aMin = minutes;
                aLabelDebug = static_cast<CCLabelTTF*>(getChildByTag(10006));
                aLabelDebug->setString(toString(aMin).c_str());
            }
            //--------------------------------
            
            if(aPossibleTimes>0)
            {
                int aStartTime = floor(60/aPossibleTimes);
                for(int i=0;i<aPossibleTimes;i++)
                {
                    if(aStartTime<=0)
                        aRandomTime = rand()%aStartTime+aStartTime;
                    else
                        aRandomTime = (rand()%1)*0.1+aStartTime;
                    
                    aLastTime+=aRandomTime;
                    if(_dwarfCurrentActiveTime==-1)
                        _dwarfCurrentActiveTime = aLastTime;
                    else
                        _dwarfSpawnTimes.push_back(aLastTime);
                }
            }
        }
        else
        {
            _dwarfCurrentTime += delta * _gameSpeed;
            
            if(_dwarfCurrentTime>=_dwarfCurrentActiveTime)
            {
                //Spawn effect and get the next !!!
                if(_dwarfSpawnTimes.size()>0)
                {
                    _dwarfCurrentActiveTime = _dwarfSpawnTimes[0];
                    _dwarfSpawnTimes.erase(_dwarfSpawnTimes.begin(),_dwarfSpawnTimes.begin()+1);
                }
                else
                {
                    _dwarfCurrentActiveTime = 1000;//Never come here again
                }
                //Create the troll long awaited
                generateDwarf();
            }
        }
        
        //=================================
        
        /*
        _mission_crystal_spawn_timer-=delta;
        if(_mission_crystal_spawn_timer<=0)
        {
            _mission_crystal_spawn_timer = 4;
            
            generateCrystal(false);
        }
        */
        
        _mission_effect_spawn_timer-=delta;
        if(_mission_effect_spawn_timer<=0)
        {
            _mission_effect_spawn_timer = (rand()%4)+10;
            
            if(_mission_allowed_effect == -1){
                //Nothing to spawn
            }
            else{
                //Check how many traps we have in map
//                generateEffect();
                SetMasterTrollAction(6);
            }
        }
        
        //Update the troll actions
        mTrollTimer+=delta;
        
        if(mTrollTimer>180){
            //Spawn some trap !!!
            mTrollTimer = 0;
            SetMasterTrollAction(6);
        }
        
        //How much bullets will fly around
        mTrollBulletTimer-=delta;
        if(mTrollBulletTimer<=0)
        {
            if(_gameTime<60){
                mTrollBulletTimer = (rand()%6)+6;
            }
            else{
                mTrollBulletTimer = (rand()%10)+10;
            }
            
            SetMasterTrollAction(8);
        }
        
        if(User::getInstance()->mSpecial_17_Mission){
            
            mTrollFreeze+=delta;
            
            if(mTrollFreeze>=mTrollFreezeDwarfTime){
                
                mTrollFreezeDwarfTime = (rand()%16)+14;
                mTrollFreeze = 0;
                
                SetMasterTrollAction(7);
            }
        }
        
        
    }
    else if(User::getInstance()->mNewMissionBuild)
    {
        //Update the dwarf and crystal spawn here !!!
        _mission_dwarf_spawn_timer-=delta;
        if(_mission_dwarf_spawn_timer<=0)
        {
            //new stuff
            int aWillSpawn = 0;
            bool spawnDwarf = true;
            
            //The special stuff
            if(User::getInstance()->mNewMissionProgress<3)
            {
                if(_dwarves->count()<3)
                {
                    _mission_dwarf_spawn_timer = (rand()%3)+8;
                }
                else if(_dwarves->count() == 3)
                {
                    aWillSpawn = rand()%10;
                    
                    if(aWillSpawn<4){
                        _mission_dwarf_spawn_timer = (rand()%3)+8;
                    }
                    else{
                        spawnDwarf = false;
                        _mission_dwarf_spawn_timer = (rand()%2)+2;
                    }
                }
                else
                {
                    aWillSpawn = rand()%10;
                    
                    if(aWillSpawn<2){
                        _mission_dwarf_spawn_timer = (rand()%3)+8;
                    }
                    else{
                        spawnDwarf = false;
                        _mission_dwarf_spawn_timer = (rand()%2)+2;
                    }
                }
            }
            else if(User::getInstance()->mNewMissionProgress<5)
            {
                if(_dwarves->count()<4)
                {
                    _mission_dwarf_spawn_timer = (rand()%4)+5;
                }
                else if(_dwarves->count() == 4)
                {
                    aWillSpawn = rand()%10;
                    
                    if(aWillSpawn<4){
                        _mission_dwarf_spawn_timer = (rand()%4)+5;
                    }
                    else{
                        spawnDwarf = false;
                        _mission_dwarf_spawn_timer = (rand()%2)+2;
                    }
                }
                else
                {
                    aWillSpawn = rand()%10;
                    
                    if(aWillSpawn<2){
                        _mission_dwarf_spawn_timer = (rand()%4)+5;
                    }
                    else{
                        spawnDwarf = false;
                        _mission_dwarf_spawn_timer = (rand()%2)+2;
                    }
                }
            }
            else
            {
                if(_dwarves->count()<4)
                {
                    _mission_dwarf_spawn_timer = (rand()%3)+3;
                }
                else if(_dwarves->count() == 4)
                {
                    aWillSpawn = rand()%10;
                    
                    if(aWillSpawn<4){
                        _mission_dwarf_spawn_timer = (rand()%3)+3;
                    }
                    else{
                        spawnDwarf = false;
                        _mission_dwarf_spawn_timer = (rand()%2)+2;
                    }
                }
                else
                {
                    aWillSpawn = rand()%10;
                    
                    if(aWillSpawn<2){
                        _mission_dwarf_spawn_timer = (rand()%3)+3;
                    }
                    else{
                        spawnDwarf = false;
                        _mission_dwarf_spawn_timer = (rand()%2)+2;
                    }
                }
            }
            

            
            if(spawnDwarf){
                generateDwarf();
            }
        }
        
        /*
        _mission_crystal_spawn_timer-=delta;
        if(_mission_crystal_spawn_timer<=0)
        {
            _mission_crystal_spawn_timer = 4;
            
            generateCrystal(false);
        }
        */
        
        _mission_effect_spawn_timer-=delta;
        if(_mission_effect_spawn_timer<=0)
        {
            _mission_effect_spawn_timer = (rand()%3)+8;
            
            if(_mission_allowed_effect == -1){
                //Nothing to spawn
            }
            else{
                //Check how many traps we have in map
                generateEffect();
            }
        }
    }
}

void GameScene::updatePlants(float delta)
{
//    CCLog("_plantCrystalTime:%f",_plantCrystalTime);
    _plantCrystalTime+=delta*_gameSpeed;
    //Grow faster !!! by 20%
    if(mRainActive)
    {
        _plantCrystalTime+=(delta*_gameSpeed)*1.25;
    }
    else
    {
        _plantCrystalTime+=delta*_gameSpeed;
    }
    
    if (_plantCrystalTime>=135)//90
    {
        _plantCrystalTime = 0;
        //Start to grow up a plant !!!
        generatePlantCrystal();
    }
    
    //For now disabled - we will have power machines
    //For first time start to plant somwehere some special plants
//    if (!_plantFuzzFlowerFirst)
//    {
//        _plantFuzzFlowerFirst = true;
//        generatePlantFuzz();
//    }
//    
//    if (!_plantSunFlowerFirst)
//    {
//        _plantSunFlowerFirst = true;
//        generatePlantSunFlower();
//    }
//    
//    if (!_plantIceFlowerFirst)
//    {
//        _plantIceFlowerFirst = true;
//        generatePlantIceFlower();
//    }
}

void GameScene::generatePlantSunFlower()
{
    if (this->getChildByTag(kPlant_SunFlower))
        return;//Wait a bit
    
    PlantSunFlower* plant = PlantSunFlower::create(this);
    plant->setTag(kPlant_SunFlower);
    
//    CCPoint aPosition = getRandomPointFromBlock(rand()%8);
    CCPoint aPosition = ccp(_screenSize.width-50,_screenSize.height/2-60);
    
    plant->setPosition(aPosition.x, aPosition.y);
    this->addChild(plant,getSpriteOrderZ(plant->getPositionY()));
}

void GameScene::generatePlantIceFlower()
{
    if (this->getChildByTag(kPlant_IceFlower))
        return;//Wait a bit
    
    PlantIceFlower* plant = PlantIceFlower::create(this);
    plant->setTag(kPlant_IceFlower);
    
//    CCPoint aPosition = getRandomPointFromBlock(rand()%8);
    CCPoint aPosition = ccp(50,_screenSize.height/2-60);
    
    plant->setPosition(aPosition.x, aPosition.y);
    this->addChild(plant,getSpriteOrderZ(plant->getPositionY()));
}

void GameScene::generatePlantFuzz()
{
    if (this->getChildByTag(kPlant_FuzzBall))
        return;//Wait a bit
    
    PlantFuzzBall* plant = PlantFuzzBall::create(this);
    plant->setTag(kPlant_FuzzBall);
    
//    CCPoint aPosition = getRandomPointFromBlock(rand()%8);
    CCPoint aPosition = ccp(_screenSize.width/2,_screenSize.height-200);
    
    plant->setPosition(aPosition.x, aPosition.y);
    this->addChild(plant,getSpriteOrderZ(plant->getPositionY()));
}

void GameScene::generatePlantCrystal()
{
    if (this->getChildByTag(kPlant_Crystal))
        return;//Wait a bit
    
    //The new stuff
    if(User::getInstance()->mNewMissionBuild){
        if(User::getInstance()->mNewMissionProgress >= 4){
            //All ok
            
        }
        else{
            return;
        }
    }
    else
    {
        if(User::getInstance()->getMachine_PlantLvl()<1)
            return;
    }
    
    Plant_Crystal_Weed* plant = Plant_Crystal_Weed::create(this);
    CCPoint aPosition = getRandomPointFromBlock(rand()%8);
    plant->setPosition(aPosition.x, aPosition.y);
    plant->setTag(kPlant_Crystal);
    addChild(plant,getSpriteOrderZ(plant->getPositionY()));
    
    //===========
    
//    PlantCrystal* plant = PlantCrystal::create(this);
//    plant->setTag(kPlant_Crystal);
//    
//    CCPoint aPosition = getRandomPointFromBlock(rand()%8);
////    CCPoint aPosition = ccp(200,200);
//    plant->setPosition(aPosition.x, aPosition.y);
    
    //////////////////////////////////////////////////////////////////////
    //Create the possible spawn points,except last one
    
//    _mushroomSpawnPositions.clear();
//    for(int i = 0;i<8;i++)
//    {
//        if (i!=mMushroomLastSpawnBlockID)
//            _mushroomSpawnPositions.push_back(i);
//    }
//    mMushroomLastSpawnBlockID = _mushroomSpawnPositions[rand()%_mushroomSpawnPositions.size()];
//    
//    CCPoint aPosition = getRandomPointFromBlock(mMushroomLastSpawnBlockID);
//    
//    mushroom->setPosition(aPosition);
    
    //////////////////////////////////////////////////////////////////////
    
//    this->addChild(plant, getSpriteOrderZ(plant->getPositionY()));
}

void GameScene::updateDwarfs(float delta)
{
	// update dwarves
	for (int dwarfIndex = _dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
	{
		Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(dwarfIndex));
        
        //**********************************
        //Simple z-sorting =D
        
        if (dwarf){
            if (getSpriteOrderZ(dwarf->getPositionY())!=dwarf->getZOrder()){
                reorderChild(dwarf,getSpriteOrderZ(dwarf->getPositionY()));
            }
        }
        
        //**********************************
		
		if (dwarf && dwarf->isVisible())
		{
			dwarf->update(delta);
			
			CCPoint cavePosition = getCavePoint(dwarf->getType());
            
            _foundWarning = false;
            
            if(mTutorialEnabled)
            {
                //Set the timer to 1sec to spawn the crystal
                if(mTutorialStep == 11 && !mTutorialTimerSet)
                {
                    mTutorialTimerSet = true;
                    mTutorialTimer = 1.0f;
                }
            }
            
            /*
            if(mCheckSpecialSpotTouch)
            {
                if (ccpDistanceSQ(dwarf->getPosition(), mSpecialTouchSpot) <= (CAVE_DISTANCE * CAVE_DISTANCE) * GLOBAL_SCALE)
                {
                    //We are clouse to this point
                    if(User::getInstance()->mSpecial_11_Mission)
                    {
                        //Check if dwarf is not already taken the crystal
                        if(dwarf->mSpecialObjectPicked==false)
                        {
                            if(_DwarfsCollectedObject>=15)
                            {
                                //Remove it !!!
                                mMapBase->removeChildByTag(20001);
                                mMapBase->removeChildByTag(20002);
                            }
                            else
                            {
                                //Move dwarf now to cave to finish mission
                                dwarf->PickedSpecialObject(1);
                            }
                            
                            _DwarfsCollectedObject+=1;
                        }
                    }
                    else if(User::getInstance()->mSpecial_12_Mission)
                    {
                        if(mTotalComboTimer>0)
                            mTotalCombo+=1;
                        mTotalComboTimer = 4.5f;
                        
                        if(mTotalCombo>1)
                        {
                            std::stringstream theComboAmount;
                            theComboAmount << "x" << mTotalCombo;
                            createTextFly(theComboAmount.str().c_str(), ccp(dwarf->getPosition().x+60,dwarf->getPosition().y), 2,mTotalCombo);
                            CreateComboSound(1, mTotalCombo);
                        }
                        
                        createPoints((CAVE_SCORE*_dwarfPointMulty)*mCombo_DwarfEnter, 0, dwarf->getPosition(),ccc3(0, 232, 225));
                        
                        dwarfEnterDoor(true,dwarf);
                        
                        CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/KaboomFx.plist");
                        p->setPosition(cavePosition.x, cavePosition.y+20);
                        p->setAutoRemoveOnFinish(true);
                        addChild(p,kPoints_Z_Order-1);
                        
                        this->removeChild(dwarf);
                        _dwarves->removeObjectAtIndex(dwarfIndex);
                        dwarf = NULL;
                        
                        stopInGameSound("Footsteps");
                        
                        playInGameSound("dwarf_enter_cave");
                        
                        JniCalls::completedAchievement("achievement_know_how_to_play",0);
                        
                        mCanSpawnExtraCrystal = true;
                        
                        updateDwarfMusic();
                        
                        //Special new tutorial case
                        if(mTutorialEnabled)
                        {
                            OnTutorialStepCompleted(8);
                            mShowTutorialCompleted = 1;
                        }
                        else
                        {
                            mDwarfsInCaveOneGame++;
                            User::getInstance()->getMissionManager().CheckSubMission(SUB_LEAD_DWARFS_ONE_GAME,mDwarfsInCaveOneGame);
                            
                            mFatDwarfsInRow++;
                            User::getInstance()->getMissionManager().CheckSubMission(SUB_LEAD_DWARF_FAT_ROW,mFatDwarfsInRow);
                            
                            //Update submissions
                            User::getInstance()->getMissionManager().CheckSubMission(SUB_DWARF_AWAY_ANY,mNoDwarfEneterCave);
                            mNoDwarfEneterCave = 0.0f;
                        }
                        
                    }
                }
            }
            */
			
			//check if cave is reached
            if (dwarf && dwarf->getType() == DWARF_TYPE_FAT)
            {
                //Can enter only when connected to cave (maybe need to check if last point for enter?)
                if (ccpDistanceSQ(dwarf->getPosition(), cavePosition) <= (CAVE_DISTANCE * CAVE_DISTANCE) * GLOBAL_SCALE &&
					dwarf->getIsConnectedToCave())
                {
                    //--------------------------------
                    if(mDebugInfoVisible)
                    {
                        CCLabelTTF* aLabelDebug = static_cast<CCLabelTTF*>(getChildByTag(10002));
                        int valueTotal = atoi(aLabelDebug->getString());
                        valueTotal+=CAVE_SCORE*_dwarfPointMulty;
                        aLabelDebug->setString(toString(valueTotal).c_str());
                    }
                    //--------------------------------
                    
                    
                    //========================================
                    
                    /*
                    if(User::getInstance()->mNewMissionBuild){
                        
                    }
                    else{
                        if(mTotalComboTimer>0)
                            mTotalCombo+=1;
                        mTotalComboTimer = 4.5f;
                        
                        if(mTotalCombo>1)
                        {
                            std::stringstream theComboAmount;
                            theComboAmount << "x" << mTotalCombo;
                            createTextFly(theComboAmount.str().c_str(), ccp(dwarf->getPosition().x+60,dwarf->getPosition().y), 2,mTotalCombo);
                            CreateComboSound(1, mTotalCombo);
                        }
                    }
                    */
                    
                    //========================================
                    
//                    if(mComboTimer_DwarfEnter>0)
//                        mCombo_DwarfEnter+=1;
//                    mComboTimer_DwarfEnter = 3.0f;
                    
                    //Create some stuff
//                    createPoints(mCombo_DwarfEnter, 0, ccp(dwarf->getPosition().x,dwarf->getPosition().y-50),ccc3(0, 232, 225));
//                    if(mCombo_DwarfEnter>1)
//                    {
//                        std::stringstream theComboAmount;
//                        theComboAmount << "x" << mCombo_DwarfEnter;
//                        createTextFly(theComboAmount.str().c_str(), ccp(dwarf->getPosition().x+60,dwarf->getPosition().y), 2,mCombo_DwarfEnter);
//                        CreateComboSound(1, mCombo_DwarfEnter);
//                    }
                    
                    /*
                    if(User::getInstance()->mNewMissionBuild){
                        //Do not count points when this mode???
                        //update the dwarf counter
                        if(_dwarfOnMap!=NULL){
                            std::stringstream aResult;
                            _mission_dwarfs_removed+=1;
                            aResult<<(_mission_dwarfs_max-_mission_dwarfs_removed)<<" Dwarfs Left";
                            _dwarfOnMap->setString(aResult.str().c_str());
                        }
                        
//                        createPoints(dwarf->mCrystalPoints,0,dwarf->getPosition(),ccc3(0, 232, 225));
//                        addScore(dwarf->mCrystalPoints);
                        _mission_dwarfs_saved_counter+=1;
                        createPoints(CAVE_SCORE, 0, dwarf->getPosition(),ccc3(0, 232, 225));
                        
                        //Give extra 10 points for cave
//                        if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission ||
//                           User::getInstance()->mSpecial_18_Mission || User::getInstance()->mSpecial_19_Mission ||
//                           User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission ||
//                           User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission){
//                            createPoints(CAVE_SCORE, 0, dwarf->getPosition(),ccc3(0, 232, 225));
//                        }
                    }
                    else{
                        createPoints((CAVE_SCORE*_dwarfPointMulty)*mCombo_DwarfEnter, 0, dwarf->getPosition(),ccc3(0, 232, 225));
                    }
                    */
                    
                    
                    
                    //--------------------------------
                    //The dwarf points and other cool stuff
                    //================================
                    // Combo
                    
                    if(mTotalComboTimer>0){
                        mTotalCombo+=1;
                        CheckMissionByValue(MissionType_Combo,mTotalCombo);
                    }
                    
                    mTotalComboTimer = 4.5f;
                    
                    if(mTotalCombo>1)
                    {
                        std::stringstream theComboAmount;
                        theComboAmount << "x" << mTotalCombo;
                        createTextFly(theComboAmount.str().c_str(), ccp(dwarf->getPosition().x+60,dwarf->getPosition().y), 2,mTotalCombo);
                        CreateComboSound(1, mTotalCombo);
                    }
                    
                    //--------------------------------
                    // Points for cave enter
                    
                    createPoints((CAVE_SCORE*_dwarfPointMulty)*mCombo_DwarfEnter, 0, dwarf->getPosition(),ccc3(0, 232, 225));
                    mTotalPoints+=(CAVE_SCORE*_dwarfPointMulty)*mCombo_DwarfEnter;
                    
                    CheckMissionByValue(MissionType_PointCount,mTotalPoints);
                    
                    //--------------------------------
                    
                    
                    //Fade in/out cave
//                    if (dwarf->getType() == DWARF_TYPE_FAT)
//                        dwarfEnterDoor(true);
//                    else if (dwarf->getType() == DWARF_TYPE_TALL)
//                        dwarfEnterDoor(false);
                    
                    dwarfEnterDoor(true,dwarf);
                    mTotalBlueDwarfs+=1;
                    CheckMissionByValue(MissionType_DwarfCount,mTotalBlueDwarfs+mTotalOrangeDwarfs);
                    
                    CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/KaboomFx.plist");
                    p->setPosition(cavePosition.x, cavePosition.y+20);
                    p->setAutoRemoveOnFinish(true);
                    addChild(p,kPoints_Z_Order-1);
                    
                    this->removeChild(dwarf);
                    _dwarves->removeObjectAtIndex(dwarfIndex);
                    dwarf = NULL;
                    
                    stopInGameSound("Footsteps");
                    
                    playInGameSound("dwarf_enter_cave");
//                    playInGameSound("Cave_Enter");//dwarf_enter_cave
                    
                    JniCalls::completedAchievement("achievement_know_how_to_play",0);
                    
                    mCanSpawnExtraCrystal = true;
                    
                    updateDwarfMusic();
                    
                    //Check missions!!!
//                    User::getInstance()->getMissionManager().CheckSubMission(SUB_LEAD_DWARF_FAT,1);
                    
                    //Special new tutorial case
                    if(mTutorialEnabled)
                    {
                        OnTutorialStepCompleted(8);
                        mShowTutorialCompleted = 1;
                    }
                    else
                    {
                        mDwarfsInCaveOneGame++;
                        User::getInstance()->getMissionManager().CheckSubMission(SUB_LEAD_DWARFS_ONE_GAME,mDwarfsInCaveOneGame);
                        
                        mFatDwarfsInRow++;
                        User::getInstance()->getMissionManager().CheckSubMission(SUB_LEAD_DWARF_FAT_ROW,mFatDwarfsInRow);
                        
                        //Update submissions
                        User::getInstance()->getMissionManager().CheckSubMission(SUB_DWARF_AWAY_ANY,mNoDwarfEneterCave);
                        mNoDwarfEneterCave = 0.0f;
                    }
                    
//                    PlaySpecialMusic(_dwarves->count());
                }
            }
            else if (dwarf && dwarf->getType() == DWARF_TYPE_TALL)
            {
                if (ccpDistanceSQ(dwarf->getPosition(), cavePosition) <= (CAVE_DISTANCE_TALL * CAVE_DISTANCE_TALL)*GLOBAL_SCALE &&
					dwarf->getIsConnectedToCave())
                {
                    //--------------------------------
                    if(mDebugInfoVisible)
                    {
                        CCLabelTTF* aLabelDebug = static_cast<CCLabelTTF*>(getChildByTag(10002));
                        int valueTotal = atoi(aLabelDebug->getString());
                        valueTotal+=CAVE_SCORE*_dwarfPointMulty;
                        aLabelDebug->setString(toString(valueTotal).c_str());
                    }
                    
                    //--------------------------------
                    //The dwarf points and other cool stuff
                    //================================
                    // Combo
                    
                    if(mTotalComboTimer>0){
                        mTotalCombo+=1;
                        CheckMissionByValue(MissionType_Combo,mTotalCombo);
                    }
                    
                    mTotalComboTimer = 4.5f;
                    
                    if(mTotalCombo>1)
                    {
                        std::stringstream theComboAmount;
                        theComboAmount << "x" << mTotalCombo;
                        createTextFly(theComboAmount.str().c_str(), ccp(dwarf->getPosition().x+60,dwarf->getPosition().y), 2,mTotalCombo);
                        CreateComboSound(1, mTotalCombo);
                    }
                    
                    //--------------------------------
                    // Points for cave enter
                    
                    createPoints((CAVE_SCORE*_dwarfPointMulty)*mCombo_DwarfEnter, 0, dwarf->getPosition(),ccc3(0, 232, 225));
                    mTotalPoints+=(CAVE_SCORE*_dwarfPointMulty)*mCombo_DwarfEnter;
                    
                    CheckMissionByValue(MissionType_PointCount,mTotalPoints);
                    
                    //--------------------------------
                    
                    
                    
//                    if(mComboTimer_DwarfEnter>0)
//                        mCombo_DwarfEnter+=1;
//                    mComboTimer_DwarfEnter = 3.0f;
//                    
//                    //Create combo x2 text, un blakus istais punktu skaits
//                    if(mCombo_DwarfEnter>1)
//                    {
//                        std::stringstream theComboAmount;
//                        theComboAmount << "x" << mCombo_DwarfEnter;
//                        createTextFly(theComboAmount.str().c_str(), ccp(dwarf->getPosition().x+60,dwarf->getPosition().y), 2,mCombo_DwarfEnter);
//                        CreateComboSound(1,mCombo_DwarfEnter);
//                    }
                    
                    /*
                    if(User::getInstance()->mNewMissionBuild){
                        //Do not count points when this mode???
                        //update the dwarf counter
                        if(_dwarfOnMap!=NULL){
                            std::stringstream aResult;
                            _mission_dwarfs_removed+=1;
                            aResult<<(_mission_dwarfs_max-_mission_dwarfs_removed)<<" Dwarfs Left";
                            _dwarfOnMap->setString(aResult.str().c_str());
                        }
                        
//                        createPoints(dwarf->mCrystalPoints,0,dwarf->getPosition(),ccc3(0, 232, 225));
//                        addScore(dwarf->mCrystalPoints);
                        
                        _mission_dwarfs_saved_counter+=1;
                        
                        //Give extra 10 points for cave
                        if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission
                           || User::getInstance()->mSpecial_18_Mission
                           || User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission
                           || User::getInstance()->mSpecial_21_Mission || User::getInstance()->mSpecial_22_Mission
                           || User::getInstance()->mSpecial_23_Mission){
                            createPoints(CAVE_SCORE, 0, dwarf->getPosition(),ccc3(0, 232, 225));
                        }
                    }
                    else{
                        createPoints((CAVE_SCORE*_dwarfPointMulty)*mCombo_DwarfEnter, 0, dwarf->getPosition(),ccc3(0, 232, 225));
                    }
                    */
                    
                    dwarfEnterDoor(false,dwarf);
                    mTotalOrangeDwarfs+=1;
                    CheckMissionByValue(MissionType_DwarfCount,mTotalBlueDwarfs+mTotalOrangeDwarfs);
                    
                    CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/KaboomFx.plist");
                    p->setPosition(cavePosition.x, cavePosition.y+20);
                    p->setAutoRemoveOnFinish(true);
                    addChild(p,kPoints_Z_Order-1);
                    
                    
                    this->removeChild(dwarf);
                    _dwarves->removeObjectAtIndex(dwarfIndex);
                    dwarf = NULL;
                    
                    updateDwarfMusic();
                    
                    stopInGameSound("Footsteps");
                    
                    playInGameSound("dwarf_enter_cave");
//                    playInGameSound("Cave_Enter");
                    
                    JniCalls::completedAchievement("achievement_know_how_to_play",0);
                    
                    mCanSpawnExtraCrystal = true;
                    
                    //Check missions!!!
//                    User::getInstance()->getMissionManager().CheckSubMission(SUB_LEAD_DWARFS_TALL,1);
                    
                    if(mFatDwarfsInRow>0)
                    {
                        User::getInstance()->getMissionManager().ResetSpecialMissions(SUB_LEAD_DWARF_FAT_ROW);
                        mFatDwarfsInRow = 0;//Reset !!!
                    }
                    
                    if(!mTutorialEnabled)
                    {
                        mDwarfsInCaveOneGame++;
                        User::getInstance()->getMissionManager().CheckSubMission(SUB_LEAD_DWARFS_ONE_GAME,mDwarfsInCaveOneGame);
                        
                        //Update submissions
                        User::getInstance()->getMissionManager().CheckSubMission(SUB_DWARF_AWAY_ANY,mNoDwarfEneterCave);
                        mNoDwarfEneterCave = 0.0f;
                    }
                    
//                    PlaySpecialMusic(_dwarves->count());
                }
            }
			
			if (dwarf && _boostShieldTimer<=0)//If has shield then nothing can scare our dwarf =D
			{
                //Check gobs
                for(int gobIndex = _goblins->count()-1;gobIndex>=0;--gobIndex)
                {
                    Goblin* gob = static_cast<Goblin*>(_goblins->objectAtIndex(gobIndex));
                    if(gob->isVisible() && gob->_touchable && gob->_canMove)
                    {
                        //Check for crash now !!!
                        if (ccpDistanceSQ(dwarf->getPosition(), gob->getPosition())<= powf(TROLL_DISTANCE, 2)*GLOBAL_SCALE)
                        {
                            stopInGameSound("Footsteps");
                            stopInGameSound("troll_walk");
                            
                            stopInGameSound("dwarf_web_stuck",true);
//                            dwarf->createCrash();
                            
                            dwarf->setDisabled(true);
                            dwarf->createCrash();
                            dwarf->doDwarfBang(dwarf->_angle);
                            dwarf->setTag(999);//Will skip his pause !!!
                            
//                            dwarf->createTrollCrash();
                            gob->HitGoblin(false);
                        }
                    }
                }
                
                //Check hidras
                for(int hidrasIndex = _hidras->count()-1;hidrasIndex>=0;--hidrasIndex)
                {
                    Hidra* hidra = static_cast<Hidra*>(_hidras->objectAtIndex(hidrasIndex));
                    if (ccpDistanceSQ(dwarf->getPosition(), hidra->getPosition())<= powf(TROLL_DISTANCE, 2)*GLOBAL_SCALE)
                    {
                        stopInGameSound("Footsteps");
                        stopInGameSound("troll_walk");
                        
                        stopInGameSound("dwarf_web_stuck",true);
                        
                        dwarf->createTrollCrash();
                        
                        hidra->setTag(999);
                        dwarf->setTag(999);
                        
                        lose();
                    }
                    else if(ccpDistanceSQ(dwarf->getPosition(), hidra->getPosition())<= powf(164, 2)*GLOBAL_SCALE)
                    {
                        //Check if is under hidra radar
                        //Total fake - but gets the point :)
                        if(dwarf->getPositionY()<hidra->getPositionY())
                        {
                            bool didTouch = false;
                            if(hidra->mCatchRadar->getRotation()<230){
                                if(dwarf->getPositionX()>hidra->getPositionX()+75 && dwarf->getPositionX()<hidra->getPositionX()+150){
                                    didTouch = true;
                                }
                            }
                            else if(hidra->mCatchRadar->getRotation()<270){
                                if(dwarf->getPositionX()>hidra->getPositionX() && dwarf->getPositionX()<hidra->getPositionX()+75){
                                    didTouch = true;
                                }
                            }
                            else if(hidra->mCatchRadar->getRotation()<310){
                                if(dwarf->getPositionX()>hidra->getPositionX()-75 && dwarf->getPositionX()<hidra->getPositionX()){
                                    didTouch = true;
                                }
                            }
                            if(hidra->mCatchRadar->getRotation()<350){
                                if(dwarf->getPositionX()>hidra->getPositionX()-150 && dwarf->getPositionX()<hidra->getPositionX()-75){
                                    didTouch = true;
                                }
                            }
                            
                            if(didTouch){
                                stopInGameSound("Footsteps");
                                stopInGameSound("troll_walk");
                                
                                stopInGameSound("dwarf_web_stuck",true);
                                
                                dwarf->createTrollCrash();
                                
                                hidra->setTag(999);
                                dwarf->setTag(999);
                                
                                lose();
                            }
                        }
                    }
                    
                }
                
				//check collisions with all trolls
				for (int trollIndex = _trolls->count() - 1; trollIndex >= 0; --trollIndex)
				{
					Troll* troll = static_cast<Troll*>(_trolls->objectAtIndex(trollIndex));
					
                    //Little update - warning light on gnome !!!
                    if (troll->isVisible() && troll->getTouchable() && troll->getCanMove() && _boostNoEnemyTimer<=0)
                    {
                        //Check the warning distance
                        if (ccpDistanceSQ(dwarf->getPosition(), troll->getPosition())<=powf(30, 3)*GLOBAL_SCALE)
                            _foundWarning = true;
                        
                        //Check for crash now !!!
                        if (ccpDistanceSQ(dwarf->getPosition(), troll->getPosition())<= powf(TROLL_DISTANCE, 2)*GLOBAL_SCALE && !mTutorialEnabled)
                        {
                            troll->setVictory();
                            
                            //Use this for now !!!
                            stopInGameSound("Footsteps");
                            stopInGameSound("troll_walk");
                            
                            stopInGameSound("dwarf_web_stuck",true);
                            
                            dwarf->createTrollCrash();
                            
                            //------------------------
                            
                            troll->setTag(999);
                            dwarf->setTag(999);
                            
                            menuSaveMeCallBack(dwarf,NULL,troll);
                            
                            /*
                            if(User::getInstance()->mDynamicTrolls && (User::getInstance()->mSpecial_19_Mission
                                                                       || User::getInstance()->mSpecial_20_Mission
                                                                       || User::getInstance()->mSpecial_21_Mission
                                                                       || User::getInstance()->mSpecial_22_Mission
                                                                       || User::getInstance()->mSpecial_23_Mission))
                            {
                                stopInGameSound("Footsteps");
                                stopInGameSound("troll_walk");
                                
                                stopInGameSound("dwarf_web_stuck",true);
                                
                                dwarf->createTrollCrash();
                                
                                //------------------------
                                
                                troll->setTag(999);
                                dwarf->setTag(999);
                                
                                //Check if did not complete mission!!!
                                if(User::getInstance()->mSpecial_19_Mission){
                                    if(_mission_dwarfs_saved_counter>=_mission_star_points_1){
                                        //All ok win
                                        NewMissionCompleted();
                                    }
                                    else{
                                        lose();
                                    }
                                }
                                else{
                                    lose();
                                }
                            }
                            else if(User::getInstance()->mDynamicTrolls && User::getInstance()->mSpecial_18_Mission==false)
                            {
                                stopInGameSound("Footsteps");
                                stopInGameSound("troll_walk");
                                
                                stopInGameSound("dwarf_web_stuck",true);
                                
                                dwarf->createTrollCrash();
                                
                                //------------------------
                                
                                troll->setTag(999);
                                dwarf->setTag(999);
                                
                                lose();
                            }
                            else if(User::getInstance()->mNewMissionBuild)
                            {
                                troll->setTag(999);//Will continue path after 2sec
                                
                                if(_dwarfOnMap!=NULL){
                                    std::stringstream aResult;
                                    _mission_dwarfs_removed+=1;
                                    aResult<<(_mission_dwarfs_max-_mission_dwarfs_removed)<<" Dwarfs Left";
                                    _dwarfOnMap->setString(aResult.str().c_str());
                                }
                                
                                
                                createPoints(-dwarf->mCrystalPoints, 0, dwarf->getPosition(), ccc3(0, 232, 225));
                                dwarf->mCollectedCrystals = 0;
                                dwarf->mCrystalPoints = 0;
                                dwarf->UpdateBagIcon();
                                
                                //Remove dwarf
                                CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/KaboomFx.plist");
                                p->setPosition(cavePosition.x, cavePosition.y+20);
                                p->setAutoRemoveOnFinish(true);
                                addChild(p,kPoints_Z_Order-1);
                                
//                                this->removeChild(dwarf);//Do not remove now !!!
                                dwarf->removeFromTroll();
                                
                                _dwarves->removeObjectAtIndex(dwarfIndex);
                                dwarf = NULL;
                            }
                            else
                            {
                                stopInGameSound("Footsteps");
                                stopInGameSound("troll_walk");
                                
                                stopInGameSound("dwarf_web_stuck",true);
                                
                                dwarf->createTrollCrash();
                                
                                //------------------------
                                
                                troll->setTag(999);
                                dwarf->setTag(999);
                                
                                menuSaveMeCallBack(dwarf,NULL,troll);
                            }
                            */
                            
                            //------------------------
                            break;
                        }
                        else if(isInTrollSensor(dwarf,troll))
                        {
                            //Alarm - activate troll to run !!!
                            troll->CatchDwarf(dwarf);
                        }
                    }
                    
                    dwarf->setCanPlayAlarm(true);
                    dwarf->setAlarmTroll(_foundWarning);
                    
				}
			}
			
			if (dwarf)
			{
				//check collisions with all chrystals
				for (int crystalIndex = _crystals->count() - 1; crystalIndex >= 0; --crystalIndex)
				{
					Crystal* crystal = static_cast<Crystal*>(_crystals->objectAtIndex(crystalIndex));
					
                    int aCollectDist = CRYSTAL_DISTANCE;
                    if(mTutorialEnabled)
                    {
                        aCollectDist = 70;
                    }
                    
                    if(!crystal->isVisible()){
                        continue;
                    }
                    
//					if (crystal->isVisible() &&
//						ccpDistanceSQ(dwarf->getPosition(), crystal->getPosition()) <= (aCollectDist * aCollectDist) * GLOBAL_SCALE)
                    
                    if (ccpDistanceSQ(dwarf->getPosition(), crystal->getPosition()) <= (aCollectDist * aCollectDist) * GLOBAL_SCALE)
					{
                        /*
                        //Check if this dwarf can add more to it !!!
                        if(User::getInstance()->mNewMissionBuild)
                        {
                            if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission
                               || User::getInstance()->mSpecial_18_Mission || User::getInstance()->mSpecial_19_Mission
                               || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
                               || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission)
                            {
                                //Allow to over collect crystals
                                
                            }
                            else
                            {
                                if(dwarf->mCollectedCrystals>=dwarf->mCollectedCrystalsMax)
                                {
                                    //Cant collect any more !!! - full house !!!
                                    
                                    break;
                                }
                            }
                        }
                        */
                        
                        
                        RemoveEffectByChild(crystal);
                        
//                        if(mComboTimer_CollectCrystal>0)
//                            mCombo_CollectCrystals+=1;
//                        mComboTimer_CollectCrystal = 3.0f;
//                        
//                        bool aSoundCreated = false;
//                        
//                        if(mCombo_CollectCrystals>1)
//                        {
//                            std::stringstream theComboAmount;
//                            theComboAmount << "x" << mCombo_CollectCrystals;
//                            createTextFly(theComboAmount.str().c_str(), ccp(dwarf->getPosition().x-60,dwarf->getPosition().y), 3,mCombo_CollectCrystals);
//                            
//                            CreateComboSound(2, mCombo_CollectCrystals);
//                            
//                            aSoundCreated = true;
//                        }
                        
                        //========================================
                        
                        if(crystal->_color != CRYSTAL_EGG)
                        {
                            if(mTotalComboTimer>0){
                                mTotalCombo+=1;
                                CheckMissionByValue(MissionType_Combo,mTotalCombo);
                            }
                            
                            mTotalComboTimer = 4.5f;
                            
                            if(mTotalCombo>1)
                            {
                                std::stringstream theComboAmount;
                                theComboAmount << "x" << mTotalCombo;
                                createTextFly(theComboAmount.str().c_str(), ccp(dwarf->getPosition().x-60,dwarf->getPosition().y), 3,mTotalCombo);
                                CreateComboSound(2, mTotalCombo);
                            }
                            
                            //========================================
                            
                            dwarf->mCollectedCrystals++;
                            
                            if(User::getInstance()->mNewMissionBuild){
                                dwarf->UpdateBagIcon();
                            }
                            
                            User::getInstance()->getMissionManager().CheckSubMission(SUB_COLLECT_CRYSTALS_SAME_DWARF,dwarf->mCollectedCrystals);
                        }
                        
                        //Crystal check for missions
                        if(mCurrentMission.Task_type == MissionType_Crystal_Blue && crystal->_color == CRYSTAL_COLOR_BLUE){
                            mTotalBlue_Crystals+=1;
                            CheckMissionByValue(MissionType_Crystal_Blue,mTotalBlue_Crystals);
                        }
                        else if(mCurrentMission.Task_type == MissionType_Crystal_Green && crystal->_color == CRYSTAL_COLOR_GREEN){
                            mTotalGreen_Crystals+=1;
                            CheckMissionByValue(MissionType_Crystal_Green,mTotalGreen_Crystals);
                        }
                        else if(mCurrentMission.Task_type == MissionType_Crystal_Red && crystal->_color == CRYSTAL_COLOR_RED){
                            mTotalRed_Crystals+=1;
                            CheckMissionByValue(MissionType_Crystal_Red,mTotalRed_Crystals);
                        }
                        else if(mCurrentMission.Task_type == MissionType_Crystal_Yellow && crystal->_color == CRYSTAL_COLOR_YELLOW){
                            mTotalYellow_Crystals+=1;
                            CheckMissionByValue(MissionType_Crystal_Yellow,mTotalYellow_Crystals);
                        }
                        
                        
                        
                        
                        
                        //Slow down it after 2 collected crystals!!!
                        int aExtraForSimpleCrystal = 1;
                        
                        //Make the dwarf slower + combo ||| Now disabled !!!
//                        if(dwarf->mCollectedCrystals>1)
//                        {
//                            if(dwarf->mCrystalSlowDown>0.5f)
//                                dwarf->mCrystalSlowDown -= (dwarf->mCrystalSlowDown*10)/100;
//                            
//                            //multiply the crystal
//                            aExtraForSimpleCrystal = dwarf->mCollectedCrystals;
//                            
//                            std::stringstream theComboAmount;
//                            theComboAmount << "x" << aExtraForSimpleCrystal;
//                            createTextFly(theComboAmount.str().c_str(), ccp(dwarf->getPosition().x-70,dwarf->getPosition().y+30), 3,aExtraForSimpleCrystal);
//                            
//                            if(!aSoundCreated)
//                                CreateComboSound(1, aExtraForSimpleCrystal);
//                        }
                        
                        //Remove the boss
                        if(crystal->mStealer!=NULL){
                            if(crystal->mStealer->mCrystalCatch!=NULL){
                                crystal->mStealer->mCrystalCatch = NULL;
                            }
                            crystal->mStealer = NULL;
                        }
                        
                        playInGameSound("crystal_pick_up");
                        
                        //Reset the no crystal timer
                        if(crystal->_color != CRYSTAL_EGG)
                        {
                            mFlurry_CrystalPoints+=mCombo_CollectCrystals*aExtraForSimpleCrystal;
                            
                            
                            User::getInstance()->getMissionManager().CheckSubMission(SUB_NO_CRYSTAL_COLLECT,int(mTimeNotCollectedCrystal));
                            mTimeNotCollectedCrystal = 0.0f;
                            mTimeToCheckAgainNoCrystal = 0.0f;
                        }
                        
                        crystal->pickUp(dwarf,mCombo_CollectCrystals*aExtraForSimpleCrystal);//For now disabled
                        
                        if(mTutorialEnabled)
                        {
                            if(mTutorialStep == 13)
                                OnTutorialStepCompleted(13);
                        }
                        else
                        {
                            //Check some stuff if game time <40 spawn extra crystals
                            if(_gameTime<EXTRA_CRYSTAL_SPAWN)
                            {
                                mSpeciaCrystallDwarfID = dwarfIndex;
                                mSpecialCrystalSpawnTimer = 1.0f;
                            }
                            
                            if(!User::getInstance()->getDailyChallenges().isTodaysCompleted())
                            {
                                // check if daily challenge is completed
                                if (!_stats.dailyChallengeCompleted &&
                                    User::getInstance()->getDailyChallenges().isChallengeRequirementsMet(_stats.gatheredCrystals))
                                {
//                                    DailyChallengesInGamePopUp* p =
//                                    DailyChallengesInGamePopUp::create(User::getInstance()->getDailyChallenges().getReward());
//                                    p->setAnchorPoint(ccp(0,0));
//                                    p->setPosition(ccp(_screenSize.width / 2 , 0));
//                                    addChild(p, 1000);
                                    
                                    AddPopUpToShow(1,User::getInstance()->getDailyChallenges().getReward(),"Daily Challange");
                                    
                                    User::getInstance()->addDiamonds(User::getInstance()->getDailyChallenges().getReward());
                                    
                                    _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
                                    _stats.dailyChallengeCompleted = true;
                                }
                                else if (!_stats.dailyChallengeCompleted
                                         && User::getInstance()->getDailyChallenges().isRequiredCrystal(crystal->getColor()))
                                {
                                    if (dailyChallengeInfo != NULL
                                        && dailyChallengeInfo->isActive())
                                    {
                                        // should update info
                                        dailyChallengeInfo->updateNumbers(_stats);
                                    }
                                    else
                                    {
                                        // should create new object
                                        dailyChallengeInfo = DailyChallengesInGameInfo::create(_stats);
                                        dailyChallengeInfo->setAnchorPoint(ccp(0.5,0));
                                        dailyChallengeInfo->setPosition(ccp((_screenSize.width - dailyChallengeInfo->getContentSize().width) / 2 , 0));
                                        addChild(dailyChallengeInfo, 1000);
                                    }
                                }
                            }
                        }
                        
                        this->removeChild(crystal);
						_crystals->removeObjectAtIndex(crystalIndex);
						crystal = NULL;
                        
						break;
					}
				}
			}
			
			if (dwarf && _boostShieldTimer<=0)
			{
				for (int effectIndex = _effects->count() - 1; effectIndex >= 0; --effectIndex)
				{
					Effect* effect = static_cast<Effect*>(_effects->objectAtIndex(effectIndex));
					
                    mCurretnEffectDistance = 0;
                    if(effect->getType() == EFFECT_TYPE_RAIN)
                    {
                        if(effect->getPositionY()>dwarf->getPositionY())
                            mRealEffectDistance = ccp(effect->getPositionX(),effect->getPositionY()+30);
                        else
                            mRealEffectDistance = ccp(effect->getPositionX(),effect->getPositionY()-10);
                        mCurretnEffectDistance = EFFECT_DISTANCE_RAIN;
                    }
                    else if(effect->getType() == EFFECT_TYPE_WIND)
                    {
                        mRealEffectDistance = effect->getPosition();
                        mCurretnEffectDistance = EFFECT_DISTANCE_TORNADO;
                    }
                    else
                    {
                        mRealEffectDistance = effect->getPosition();
                        mCurretnEffectDistance = (EFFECT_DISTANCE*EFFECT_DISTANCE)*GLOBAL_SCALE;
                    }
                    
//                    mCurretnEffectDistance = -10000;//hack
                    
//                    CCLog("Effect dist:%f",ccpDistanceSQ(dwarf->getPosition(), mRealEffectDistance));
                    
//                    _debugSprite1->setPosition(dwarf->getPosition());
//                    _debugSprite2->setPosition(mRealEffectDistance);
                    
                    //check warning
                    if(effect->isVisible() &&
                       ccpDistanceSQ(dwarf->getPosition(), mRealEffectDistance) <= mCurretnEffectDistance*1.5f)
                    {
                        _foundWarning = true;
                    }
                    
					if (effect->isVisible() &&
						ccpDistanceSQ(dwarf->getPosition(), mRealEffectDistance) <= mCurretnEffectDistance)
					{
                        if(effect->getType() == EFFECT_TYPE_WIND)
                        {
                            //Remove it later
                            _tornadoActive->addObject(effect);
                        }
                        else
                        {
                            this->removeChild(effect);
                        }
						
						effect->touch(dwarf,NULL);
						
						_effects->removeObjectAtIndex(effectIndex);
                        
                        //Check what was this for a effect
                        if(effect->getType() == EFFECT_TYPE_ICE_BARRAGE)
                        {
                            if(User::getInstance()->_tutorial_small_1==0)
                            {
                                dwarf->setTag(999);
                                CreateSmallTutorial(1,dwarf->getPositionX(),dwarf->getPositionY());
                            }
                        }
                        else if(effect->getType() == EFFECT_TYPE_WEB)
                        {
                            if(User::getInstance()->_tutorial_small_2==0)
                            {
                                dwarf->setTag(999);
                                CreateSmallTutorial(2,dwarf->getPositionX(),dwarf->getPositionY());
                            }
                        }
                        
                        if(User::getInstance()->mNewMissionBuild)
                        {
                            //Remove all crystals collected
                            //Create score off
                            createPoints(-dwarf->mCrystalPoints, 0, dwarf->getPosition(), ccc3(0, 232, 225));
                            dwarf->mCollectedCrystals = 0;
                            dwarf->mCrystalPoints = 0;
                            dwarf->UpdateBagIcon();
                        }
                        
                        //Freeze the dwarf
                        if (effect->getType()==EFFECT_TYPE_ICE_BARRAGE)
                            dwarf->pauseAnimation();
					}
                    else if(effect->isVisible() && effect->getType() == EFFECT_TYPE_WIND)
                    {
                        //check if does not need to remove it !!!
                        if(effect->_needToRemove)
                        {
                            this->removeChild(effect);
                            _effects->removeObjectAtIndex(effectIndex);
                        }
                    }
                    
                    dwarf->setCanPlayAlarm(true);
                    dwarf->setAlarmTroll(_foundWarning);
				}
                
                if(dwarf->isVisible())
                {
                    for (int effectIndex = _tornadoActive->count() - 1; effectIndex >= 0; --effectIndex)
                    {
                        Effect* effect = static_cast<Effect*>(_tornadoActive->objectAtIndex(effectIndex));
                        
                        if(effect->_needToRemove)
                        {
                            _tornadoActive->removeObjectAtIndex(effectIndex);
                        }
                        else
                        {
                            //Check for dwarf actions !!!
                            if (ccpDistanceSQ(dwarf->getPosition(), effect->getPosition()) <= EFFECT_DISTANCE_TORNADO)
                            {
                                if(User::getInstance()->mNewMissionBuild)
                                {
                                    //Remove all crystals collected
                                    //Create score off
                                    
                                    //What happens here???
//                                    createPoints(-dwarf->mCrystalPoints, 0, dwarf->getPosition(), ccc3(0, 232, 225));
//                                    dwarf->mCollectedCrystals = 0;
//                                    dwarf->UpdateBagIcon();
                                    
                                    
                                }
                                else
                                {
                                    //Suck in this dwarf and throw bouth dwarfs out as gameover !!!
                                    //                            troll->setVictory();
                                    
                                    stopInGameSound("Footsteps");
                                    stopInGameSound("troll_walk");
                                    
                                    stopInGameSound("dwarf_web_stuck",true);
                                    
                                    dwarf->createTrollCrash();
                                    
                                    //------------------------
                                    
                                    //                            troll->setTag(999);
                                    dwarf->setTag(999);
                                    
                                    menuSaveMeCallBack(dwarf,NULL,NULL);
                                    
                                    //------------------------
                                    break;
                                }
                            }
                            else if(ccpDistanceSQ(dwarf->getPosition(), effect->getPosition())<=EFFECT_DISTANCE_TORNADO*10.0f)
                            {
                                _foundWarning = true;
                                
                            }
                        }
                        
                        dwarf->setCanPlayAlarm(true);
                        dwarf->setAlarmTroll(_foundWarning);
                    }
                }
			}
			
			if (dwarf)
			{
				// checks if diamond is picked up
                for (int diamondIndex = _diamonds->count() - 1; diamondIndex >= 0; --diamondIndex)
				{
					Diamond* diamond = static_cast<Diamond*>(_diamonds->objectAtIndex(diamondIndex));
					
					if (diamond->isVisible() &&
						ccpDistanceSQ(dwarf->getPosition(), diamond->getPosition()) <= (DIAMOND_DISTANCE * DIAMOND_DISTANCE) * GLOBAL_SCALE)
					{
                        //--------------------------------
//                        CCLabelTTF* aLabelDebug = static_cast<CCLabelTTF*>(getChildByTag(10004));
//                        int valueTotal = atoi(aLabelDebug->getString());
//                        valueTotal+=10;
//                        aLabelDebug->setString(toString(valueTotal).c_str());
                        
                        //Show that collected it !!!
//                        createPoints(10, 0, dwarf->getPosition(),ccc3(0, 232, 225));
                        
						this->removeChild(diamond);
						_diamonds->removeObjectAtIndex(diamondIndex);
                        
                        playInGameSound("crystal_pick_up");
                        
                        _stats.gatheredDiamonds++;
                        User::getInstance()->addDiamonds(1);
                        _diamondsLabel->setString(toString(User::getInstance()->getDiamonds()).c_str());
                        
                        //Update the boosters if has enough money !!!
                        checkBoosterAvailability();
					}
				}
			}
            
            if (dwarf)
            {
                Mushroom* mushroom = static_cast<Mushroom*>(getChildByTag(kMushroom));
                
                if (mushroom!=NULL &&
                   ccpDistanceSQ(dwarf->getPosition(), mushroom->getPosition()) <= (MUSHROOM_DISTANCE * MUSHROOM_DISTANCE) * GLOBAL_SCALE)
                {
                    this->removeChild(mushroom);
                    //Get some random boost !!!
                    createRandomBoos(dwarf->getPosition());
                    
                    playInGameSound("crystal_pick_up");
                    
                    User::getInstance()->getMissionManager().CheckSubMission(SUB_COLLECT_MUSHROOM,1);
                    
                    mTotalMushroom+=1;
                    CheckMissionByValue(MissionType_Mushroom,mTotalMushroom);
                }
            }
            
            if (dwarf && dwarf->getForceRemove())
            {
                this->removeChild(dwarf);
				_dwarves->removeObjectAtIndex(dwarfIndex);
				dwarf = NULL;
                
                stopInGameSound("Footsteps");
            }
		}
        else
        {
            if (dwarf && dwarf->getForceRemove())
            {
                this->removeChild(dwarf);
				_dwarves->removeObjectAtIndex(dwarfIndex);
				dwarf = NULL;
            }
        }
	}
    
    if(_dwarves->count()<2 && _crystals->count()<2 && mCanSpawnExtraCrystal)
    {
        mCanSpawnExtraCrystal = false;
        mSpecialCrystalSpawnTimer = 2.0f;
//        generateCrystal();
    }
}

void GameScene::createRandomBoos(cocos2d::CCPoint thePos)
{
    //What booster could we get???
    int aBooster = 0;
    char scoreString[32];
    
    //check if this bosster is possible to use
    _possiblePowerMushroom.clear();
    
    Machine_Ghost* aMachineG = static_cast<Machine_Ghost*>(getChildByTag(1020));
    if(aMachineG->mCanBeActivated)
        _possiblePowerMushroom.push_back(2);
    
    Machine_Enemies* aMachineE = static_cast<Machine_Enemies*>(getChildByTag(1030));
    if(aMachineE->mCanBeActivated)
        _possiblePowerMushroom.push_back(3);
    
    if(_possiblePowerMushroom.size()==0)
        return;//No luck???
    if(_possiblePowerMushroom.size()==1)
        aBooster = _possiblePowerMushroom[0];
    else
        aBooster = _possiblePowerMushroom[rand()%_possiblePowerMushroom.size()];
    
    _willUseMushroom = true;
    
    switch (aBooster) {
        case 1:
            menuPowerupEyeCallback(NULL);
            sprintf(scoreString,"Foresee Crystals");
            break;
        case 2:
//            menuPowerupSlowPlayCallback(NULL);
            OnMachineGhost(NULL);
            sprintf(scoreString,"Ghost dwarfs");
            break;
        case 3:
//            menuPowerupNoEnemiesCallback(NULL);
            sprintf(scoreString,"No Enemies");
            OnMachineEnemies(NULL);
            break;
        case 4:
            menuPowerupPointsCallback(NULL);
            sprintf(scoreString,"Crystal boost");
            break;
            
        default:
            break;
    }
    
    _willUseMushroom = false;
    
//    //Create text what booster was activated !!!
//    CCLabelTTF* aBasePoints = CCLabelTTF::create(scoreString, FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*0.7, CCSize(115, 55), kCCTextAlignmentCenter);
//    
//    aBasePoints->setColor(ccc3(255, 255, 255));
//    aBasePoints->setOpacity(0);
//	aBasePoints->setPosition(ccp(thePos.x,thePos.y+50));
//	
//	this->addChild(aBasePoints, kHUD_Z_Order+2);
//    
//    //Add some action to this
//    CCFadeIn* aFadeIn = CCFadeIn::create(0.2f);
//    CCMoveBy* aMove = CCMoveBy::create(0.5f, ccp(0,50));
//    CCDelayTime* aDelay = CCDelayTime::create(0.2f);
//    CCFadeOut* aFadeOut = CCFadeOut::create(0.2f);
//    
//    CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
//    
//    CCSpawn* aSpawnAction = CCSpawn::create(aFadeIn,aMove,NULL);
//    CCSequence* aAllActions = CCSequence::create(aSpawnAction,aDelay,aFadeOut,func2,NULL);
//    
//    aBasePoints->runAction(aAllActions);
}

void GameScene::updateTrolls(float delta)
{
    for(int spiderIndex = _spiders->count()-1;spiderIndex>=0;--spiderIndex)
    {
        Spider* gob = static_cast<Spider*>(_spiders->objectAtIndex(spiderIndex));
        
        if (getSpriteOrderZ(gob->getPositionY())!=gob->getZOrder())
        {
            reorderChild(gob,getSpriteOrderZ(gob->getPositionY()));
        }
        
        gob->update(delta);
    }
    
    for(int hidraIndex = _hidras->count()-1;hidraIndex>=0;--hidraIndex)
    {
        Hidra* gob = static_cast<Hidra*>(_hidras->objectAtIndex(hidraIndex));
        if (gob->isVisible())
		{
            if (getSpriteOrderZ(gob->getPositionY())!=gob->getZOrder())
            {
                reorderChild(gob,getSpriteOrderZ(gob->getPositionY()));
            }
            
			gob->update(delta);
        }
    }
    
    for(int goblinIndex = _goblins->count()-1;goblinIndex>=0;--goblinIndex)
    {
        Goblin* gob = static_cast<Goblin*>(_goblins->objectAtIndex(goblinIndex));
        if (gob->isVisible())
		{
            if (getSpriteOrderZ(gob->getPositionY())!=gob->getZOrder())
            {
                reorderChild(gob,getSpriteOrderZ(gob->getPositionY()));
            }
            
			gob->update(delta);
        }
        else
        {
            this->removeChild(gob);
            _goblins->removeObjectAtIndex(goblinIndex);
        }
    }
//    mCrystalCatch
    
	// update trolls
	for (int trollIndex = _trolls->count() - 1; trollIndex >= 0; --trollIndex)
	{
		Troll* troll = static_cast<Troll*>(_trolls->objectAtIndex(trollIndex));
		
		if (troll->isVisible())
		{
			troll->update(delta);
			
			CCPoint trollPosition = troll->getPosition();
            
            if (_boostNoEnemyTimer>0)
            {
                //Set blitz to him !!!
//                troll->pauseSchedulerAndActions();
//                CreateBlitz(troll->getPositionX(),troll->getPositionY()-80,troll);
                
                if(User::getInstance()->mSpecialMissionBuild){
                    _SpawnSpecialTrolls = 30;
                }
                
                //When blitz finishes - remove it
                float aRandomDelay = (rand()%6)*0.1+0.2;
                CCDelayTime* aBeforeBlitz = CCDelayTime::create(aRandomDelay);
                CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnTrollDelayFinish));
                CCSequence* aSeq1 = CCSequence::create(aBeforeBlitz,func,NULL);
                troll->runAction(aSeq1);
                
                //Remove this bastard !!!
//                this->removeChild(troll);
				_trolls->removeObjectAtIndex(trollIndex);
                CCLOG("Removed troll 1");
				troll = NULL;
            }
			else if (trollPosition.x < 0 || trollPosition.y < 0 ||
				trollPosition.x >= designResolutionSize.width ||
				trollPosition.y >= designResolutionSize.height)
			{
				this->removeChild(troll);
				_trolls->removeObjectAtIndex(trollIndex);
                CCLOG("Removed troll 2");
				troll = NULL;
			}
            else if (troll->getForceRemove())
            {
                this->removeChild(troll);
				_trolls->removeObjectAtIndex(trollIndex);
                CCLOG("Removed troll 3");
				troll = NULL;
            }
            
            // it has been avoided
            if (troll == NULL)
            {
                _stats.avoidedTrolls++;
                stopInGameSound("troll_walk");
            }
		}
        
//        if (troll)
//        {
//            if (getSpriteOrderZ(troll->getPositionY())!=troll->getZOrder())
//                reorderChild(troll, getSpriteOrderZ(troll->getPositionY()));
//        }
        
        if(troll)
        {
            if(User::getInstance()->mNewMissionBuild)
            {
                //Skip for now !!!
                
            }
            else
            {
                for (int effectIndex = _effects->count() - 1; effectIndex >= 0; --effectIndex)
                {
                    Effect* effect = static_cast<Effect*>(_effects->objectAtIndex(effectIndex));
                    
                    mCurretnEffectDistance = 0;
                    if(effect->getType() == EFFECT_TYPE_WIND)
                    {
                        mRealEffectDistance = effect->getPosition();
                        mCurretnEffectDistance = EFFECT_DISTANCE_TORNADO;
                    }
                    
                    if (effect->isVisible() &&
                        ccpDistanceSQ(troll->getPosition(), mRealEffectDistance) <= mCurretnEffectDistance)
                    {
                        if(effect->getType() == EFFECT_TYPE_WIND)
                        {
                            _tornadoActive->addObject(effect);
                        }
                        
                        effect->touch(NULL,troll);
                        
                        _effects->removeObjectAtIndex(effectIndex);
                    }
                    else if(effect->isVisible() && effect->getType() == EFFECT_TYPE_WIND)
                    {
                        //check if does not need to remove it !!!
                        if(effect->_needToRemove)
                        {
                            this->removeChild(effect);
                            _effects->removeObjectAtIndex(effectIndex);
                        }
                    }
                }
            }
        }
	}
}

void GameScene::updateCrystals(float delta)
{
	
}

void GameScene::updateIntroAnimations(float delta)
{
	for (int introAnimationIndex = _introAnimations->count() - 1; introAnimationIndex >= 0; --introAnimationIndex)
	{
		IntroAnimation* introAnimation = static_cast<IntroAnimation*>(_introAnimations->objectAtIndex(introAnimationIndex));
		
		introAnimation->update(delta);
	}
}

void GameScene::addScore(int score)
{
    if(score>1000 || score<-100){
        CCLog("what 1!!");
    }
    
    /*
    
    if(User::getInstance()->mNewMissionBuild)
    {
        _stats.points += score;
        if(User::getInstance()->mSpecial_19_Mission){
            mTotalPointsInGame = _mission_dwarfs_saved_counter;
        }
        else{
            mTotalPointsInGame+=score;
        }
        
        if(mTotalPointsInGame<0)
        {
            mTotalPointsInGame = 0;
            _stats.points = 0;
        }
        
        //Update only when dwarf enter cave !!!
        if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission)
        {
            std::stringstream theComboParticle;
            theComboParticle << _stats.points <<"/"<<_mission_star_points_1;
            _pointsLabel->setString(theComboParticle.str().c_str());
        }
        else if(User::getInstance()->mSpecial_19_Mission)
        {
            _pointsLabel->setString(toString(_mission_dwarfs_saved_counter).c_str());
        }
        else
        {
            _pointsLabel->setString(toString(_stats.points).c_str());
        }
        
        UpdateMissionStars();
        
        return;
    }
//	_stats.points += Levels::getInstance()->getPoints(score);
    _stats.points += score;
    
    mTotalPointsInGame+=score;
    
    _pointsLabel->setString(toString(_stats.points).c_str());
    
    //Check achievments (do we need to check if it's completed?)
    //debug for now
    if (_stats.points>=100)
    {
        JniCalls::completedAchievement("achievement_100_points",0);
    }
    if (_stats.points>=1000)
    {
        JniCalls::completedAchievement("achievement_1K_points",0);
    }
    
    //Special stuff for mission
    if(score>0 && !mTutorialEnabled)
        User::getInstance()->getMissionManager().CheckSubMission(SUB_EARN_POINTS,score);
    */
}

//The powerup fly
void GameScene::createTextFly(const char* txt,cocos2d::CCPoint thePos,int theFlyPos,int theComboValue)
{
    CCLabelTTF* aTxt = CCLabelTTF::create(txt, FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*0.7, CCSize(strlen(txt)*40, 120), kCCTextAlignmentCenter);
    
    aTxt->setOpacity(0);
	aTxt->setPosition(ccp(thePos.x,thePos.y));
    
    this->addChild(aTxt, kPoints_Z_Order);
    
    //Fly from up to down !!!
    CCFadeIn* aFadeIn = CCFadeIn::create(0.2f);
    CCMoveBy* aMove = NULL;//CCMoveBy::create(0.5f, ccp(0,50));
    CCDelayTime* aDelay = CCDelayTime::create(1.3f);
    CCFadeOut* aFadeOut = CCFadeOut::create(0.2f);
    CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    
    //The down
//    theFlyPos = 1;//Debug
    if(theFlyPos<1)
        theFlyPos = 1;
    
    if (theFlyPos == 1)
    {
        aMove = CCMoveBy::create(1.5f, ccp(0,-60));
        
        CCSequence* aSeqAction2 = CCSequence::create(aDelay,aFadeOut,NULL);
        CCSpawn* aSpawnAction = CCSpawn::create(aFadeIn,aMove,aSeqAction2,NULL);
        CCSequence* aAllActions = CCSequence::create(aSpawnAction,func2,NULL);
        
        aTxt->runAction(aAllActions);
    }
    else if(theFlyPos >= 2)
    {
        //This is some combo stuff !!!
        CCRotateBy* aRotate1 = NULL;
        
        if(theFlyPos == 2)
        {
            aMove = CCMoveBy::create(0.7f, ccp(10,70));
            aRotate1 = CCRotateBy::create(0.6f, 24);
        }
        else if(theFlyPos == 3)
        {
            aMove = CCMoveBy::create(0.7f, ccp(-20,70));
            aRotate1 = CCRotateBy::create(0.6f, -24);
        }
        
//        CCRotateBy* aRotate1 = CCRotateBy::create(0.6f, 24);
        CCScaleTo* aScale = CCScaleTo::create(0.6f,1.5f);
        CCFadeTo* aFade2 = CCFadeTo::create(0.25f,0);
        
        CCDelayTime* aDelay1 = CCDelayTime::create(0.5f);
        CCSequence* aSeq1 = CCSequence::create(aDelay1,aFade2,func2,NULL);
        CCSpawn* aCombo1 = CCSpawn::create(aFadeIn,aRotate1,aMove,aScale,aSeq1,NULL);
        
        //Set the color
//        if(theFlyPos == 3)
//            aTxt->setColor(ccc3(60,160,255));
//        else
//            aTxt->setColor(ccGREEN);
        
        if(theComboValue>7)
            theComboValue = 7;
        
        std::stringstream theComboParticle;
        theComboParticle << "Particles/Combo_" << theComboValue << ".plist";
        
        if(theComboValue == 2)
        {
            aTxt->setColor(ccc3(128,255,0));
        }
        else if(theComboValue == 3)
        {
//            aTxt->setColor(ccc3(0,128,255));
            aTxt->setColor(ccc3(255,200,0));
        }
        else if(theComboValue == 4)
        {
            aTxt->setColor(ccc3(255,255,255));
        }
        else if(theComboValue == 5)
        {
            aTxt->setColor(ccc3(150,0,255));
        }
        else if(theComboValue == 6)
        {
            aTxt->setColor(ccc3(0,180,255));
        }
        else if(theComboValue == 7)
        {
            aTxt->setColor(ccc3(255,200,0));
        }
        else
        {
            aTxt->setColor(ccc3(255,150,0));
        }
        
        //Set some particles !!!
        CCParticleSystemQuad* p = CCParticleSystemQuad::create(theComboParticle.str().c_str());
        p->setPosition(thePos.x,thePos.y);
        p->setAutoRemoveOnFinish(true);
        addChild(p,1);
        
        aTxt->runAction(aCombo1);
    }
}

void GameScene::CreateComboSound(int theType,int theLevel)
{
    std::stringstream ss;
    
    mComboPlayDelayTimer = 0.01f;
    
    mComboSound_1.str("");
    mComboSound_1.clear();
    
    mComboSound_2.str("");
    mComboSound_2.clear();
    
    mComboVisual.str("");
    mComboVisual.clear();
    
    mFileFormat.str("");
    mFileFormat.clear();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    mFileFormat<<".ogg";
    mFileFormat<<".aifc";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    mFileFormat<<".aifc";
#endif
    
    //The combo combo !!!
    switch (theLevel)
    {
        case 1:
        {
            mComboSound_1 << "NewSounds/combo_1"<<mFileFormat.str();
            break;
        }
        case 2:
        {
            mComboSound_1 << "NewSounds/combo_2"<<mFileFormat.str();
            break;
        }
        case 3:
        {
            mComboSound_1 << "NewSounds/combo_3"<<mFileFormat.str();
            break;
        }
        case 4:
        {
            mComboSound_1 << "NewSounds/combo_4"<<mFileFormat.str();
            if(theType==1)
            {
                int aRandomValue = rand()%2;
                if(aRandomValue>0)
                {
                    mComboSound_2 << "NewSounds/shout_1"<<mFileFormat.str();
                    mComboVisual << "shouts/2excellent.png";
                }
                else
                {
                    mComboSound_2 << "NewSounds/shout_2"<<mFileFormat.str();
                    mComboVisual << "shouts/7fantastic.png";
                }
            }
            else if(theType==2)
            {
                mComboSound_2 << "NewSounds/shout_3"<<mFileFormat.str();
                mComboVisual << "shouts/6shiny.png";
            }
            break;
        }
        case 5:
        {
            mComboSound_1 << "NewSounds/combo_5"<<mFileFormat.str();
            break;
        }
        case 6:
        {
            mComboSound_1 << "NewSounds/combo_5"<<mFileFormat.str();
            break;
        }
        case 7:
        {
            mComboSound_1 << "NewSounds/combo_5"<<mFileFormat.str();
            if(theType==1)
            {
                int aRandomValue = rand()%2;
                if(aRandomValue>0)
                {
                    mComboSound_2 << "NewSounds/shout_5"<<mFileFormat.str();
                    mComboVisual << "shouts/8superb.png";
                }
                else
                {
                    mComboSound_2 << "NewSounds/shout_6"<<mFileFormat.str();
                    mComboVisual << "shouts/4magnificent.png";
                }
            }
            else if(theType==2)
            {
                mComboSound_2 << "NewSounds/shout_4"<<mFileFormat.str();
                mComboVisual << "shouts/9crystalicious.png";
            }
            break;
        }
        case 8:
        {
            mComboSound_1 << "NewSounds/combo_5"<<mFileFormat.str();
            break;
        }
        case 9:
        {
            mComboSound_1 << "NewSounds/combo_5"<<mFileFormat.str();
            break;
        }
        case 10:
        {
            mComboSound_1 << "NewSounds/combo_5"<<mFileFormat.str();
            int aRandomValue = rand()%3;
            if(aRandomValue==0)
            {
                mComboSound_2 << "NewSounds/shout_7"<<mFileFormat.str();
                mComboVisual << "shouts/10priceless.png";
            }
            else if(aRandomValue==1)
            {
                mComboSound_2 << "NewSounds/shout_8"<<mFileFormat.str();
                mComboVisual << "shouts/1awesome.png";
            }
            else
            {
                mComboSound_2 << "NewSounds/shout_9"<<mFileFormat.str();
                mComboVisual << "shouts/5crazy.png";
            }
            break;
        }
        case 11:
        {
            mComboSound_1 << "NewSounds/combo_5"<<mFileFormat.str();
            break;
        }
        case 12:
        {
            mComboSound_1 << "NewSounds/combo_5"<<mFileFormat.str();
            break;
        }
        case 13:
        {
            mComboSound_1 << "NewSounds/combo_5"<<mFileFormat.str();
            mComboSound_2 << "NewSounds/shout_10"<<mFileFormat.str();
            mComboVisual << "shouts/3crystalmania.png";
            break;
        }
            
        default:
            mComboSound_1 << "NewSounds/combo_5"<<mFileFormat.str();
            break;
    }
    
    /*
    //Add check for sound play and delay !!!
    if(theType == 1)//Dwarf Combo sounds
    {
        switch (theLevel)
        {
            case 1:
                mComboSound_1 << "NewSounds/combo_1"<<mFileFormat;
                break;
            case 2:
                mComboSound_1 << "NewSounds/combo_2.mp3";
                break;
            case 3:
                mComboSound_1 << "NewSounds/combo_3.mp3";
                mComboSound_2 << "NewSounds/shout_1.mp3";
                
                mComboVisual << "shouts/1awesome.png";
                break;
            case 4:
                mComboSound_1 << "NewSounds/combo_4.mp3";
                break;
            case 5:
                mComboSound_1 << "NewSounds/combo_5.mp3";
                mComboSound_2 << "NewSounds/shout_2.mp3";
                
                mComboVisual << "shouts/2excellent.png";
                break;
            case 6:
                mComboSound_1 << "NewSounds/combo_6.mp3";
                break;
            case 7:
                mComboSound_1 << "NewSounds/combo_6.mp3";
                mComboSound_2 << "NewSounds/shout_3.mp3";
                
                mComboVisual << "shouts/3crystalmania.png";
                break;
            case 8:
                mComboSound_1 << "NewSounds/combo_6.mp3";
                break;
            case 9:
                mComboSound_1 << "NewSounds/combo_6.mp3";
                mComboSound_2 << "NewSounds/shout_4.mp3";
                
                mComboVisual << "shouts/4magnificent.png";
                break;
            case 10:
                mComboSound_1 << "NewSounds/combo_6.mp3";
                break;
            case 11:
                mComboSound_1 << "NewSounds/combo_6.mp3";
                mComboSound_2 << "NewSounds/shout_5.mp3";
                
                mComboVisual << "shouts/5crazy.png";
                break;
            default:
                mComboSound_1 << "NewSounds/combo_6.mp3";
                mComboSound_2 << "NewSounds/shout_5.mp3";
                
                mComboVisual << "shouts/5crazy.png";
                break;
        }
    }
    else if(theType == 2)//Crystal combo sounds
    {
        switch (theLevel)
        {
            case 1:
                mComboSound_1 << "NewSounds/combo_1.mp3";
                break;
            case 2:
                mComboSound_1 << "NewSounds/combo_2.mp3";
                mComboSound_2 << "NewSounds/shout_6.mp3";
                
                mComboVisual << "shouts/6shiny.png";
                break;
            case 3:
                mComboSound_1 << "NewSounds/combo_3.mp3";
                mComboSound_2 << "NewSounds/shout_7.mp3";
                
                mComboVisual << "shouts/7fantastic.png";
                break;
            case 4:
                mComboSound_1 << "NewSounds/combo_4.mp3";
                mComboSound_2 << "NewSounds/shout_8.mp3";
                
                mComboVisual << "shouts/8superb.png";
                break;
            case 5:
                mComboSound_1 << "NewSounds/combo_5.mp3";
                mComboSound_2 << "NewSounds/shout_9.mp3";
                
                mComboVisual << "shouts/9crystalicious.png";
                break;
            case 6:
                mComboSound_1 << "NewSounds/combo_6.mp3";
                mComboSound_2 << "NewSounds/shout_10.mp3";
                
                mComboVisual << "shouts/10priceless.png";
                break;
            default:
                mComboSound_1 << "NewSounds/combo_6.mp3";
                mComboSound_2 << "NewSounds/shout_10.mp3";
                
                mComboVisual << "shouts/10priceless.png";
                break;
        }
    }
    */
    
    //Check what can we do here !!!
    SimpleAudioEngine::sharedEngine()->playEffect(mComboSound_1.str().c_str(),false);
    
    //The task check
    User::getInstance()->getMissionManager().CheckSubMission(SUB_GET_COMBO,theLevel);
    
    //Check if shotu is not in action already !!!
//    if(getChildByTag(3333))
//    {
//        //Wait
//    }
    if(mComboVisual.str().length()>0)
    {
        //Craete the shout
        CCSprite* aShoot = CCSprite::create(mComboVisual.str().c_str());
        aShoot->setPosition(ccp(_screenSize.width/2,_screenSize.height/2));
        aShoot->setScale(0);
        aShoot->setTag(3333);
        addChild(aShoot,kHUD_Z_Order-1);
        
        ccBezierConfig bezier;
        //Some random stuff !!!
        CCRotateBy* aRotate1;
        
        int aDir = rand()%2;
        if(getChildByTag(3333))
        {
            if(aDir == mLastShootDir)
            {
                if(aDir == 1)
                    aDir = 0;
                else
                    aDir = 1;
            }
        }
        
        mLastShootDir = aDir;
        
        if(aDir>0)
        {
            bezier.controlPoint_1 = ccp(_screenSize.width/2, _screenSize.height/2+200);//1096,168
            bezier.controlPoint_2 = ccp(_screenSize.width/2+100, _screenSize.height/2+200);//635,105
            bezier.endPosition = ccp(_screenSize.width/2+100, _screenSize.height/2+100);
            
            aRotate1 = CCRotateBy::create(1.0f,15);
        }
        else
        {
            bezier.controlPoint_1 = ccp(_screenSize.width/2, _screenSize.height/2+200);//1096,168
            bezier.controlPoint_2 = ccp(_screenSize.width/2-100, _screenSize.height/2+200);//635,105
            bezier.endPosition = ccp(_screenSize.width/2-100, _screenSize.height/2+100);
            
            aRotate1 = CCRotateBy::create(1.0f,-15);
        }
        
        CCBezierTo* aBaz1 = CCBezierTo::create(1.0f,bezier);
        CCScaleTo* aScale1 = CCScaleTo::create(0.4f,1.0f);
        CCEaseBounceOut* aBounc1 = CCEaseBounceOut::create(aScale1);
        CCCallFuncN* aFunc1 = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
        
        CCFadeOut* aFade1 = CCFadeOut::create(0.2f);
        CCDelayTime* aDelay1 = CCDelayTime::create(0.8f);
        CCSequence* aSeq2 = CCSequence::create(aDelay1,aFade1,NULL);
        
        CCSpawn* aSpawn1 = CCSpawn::create(aBaz1,aRotate1,aBounc1,aSeq2,NULL);
        
        CCSequence* aSeq1 = CCSequence::create(aSpawn1,aFunc1,NULL);
        
        aShoot->runAction(aSeq1);
    }
    
    
    //Old stuff
//    if(theLevel==8)
//    {
//        ss << "combo_ultra";
//    }
//    else if(theLevel==5)
//    {
//        ss << "combo_ultra_2";
//    }
//    else if(theLevel>5)
//    {
//        ss << "combo_5";
//    }
//    else
//    {
//        ss << "combo_" << theLevel;
//    }
//    
//    User::getInstance()->getMissionManager().CheckSubMission(SUB_GET_COMBO,theLevel);
//    
//    playInGameSound(ss.str().c_str());
}

void GameScene::addDebugCrystalPoints(int amount)
{
    mCrystalPoints+=amount;
    
    _scoreLabel->setString(toString(mCrystalPoints).c_str());
    
    if(!mDebugInfoVisible)
        return;
    //--------------------------------
    CCLabelTTF* aLabelDebug = static_cast<CCLabelTTF*>(getChildByTag(10004));
    int valueTotal = atoi(aLabelDebug->getString());
    valueTotal+=amount;
    aLabelDebug->setString(toString(valueTotal).c_str());
    //--------------------------------
}

void GameScene::createPoints(int amount,int theBonus,cocos2d::CCPoint thePos,cocos2d::ccColor3B theColor,bool crystalPoint)
{
    std::stringstream ss;
    
    //safe call
    if(amount>10000){
        CCLog("what???");
    }
    
    if(User::getInstance()->mNewMissionBuild){
        
        if(amount == 0){
            return;//Do not show anything
        }
        
        if(amount>0){
            ss <<"+"<<(amount * mScoreMulti_Global);
        }
        else{
            ss <<(amount * mScoreMulti_Global);
        }
    }
    else{
        ss << (amount * mScoreMulti_Global);
    }
    
    CCLabelTTF* aBasePoints = CCLabelTTF::create(ss.str().c_str(), FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*0.7, CCSize(115, 55), kCCTextAlignmentCenter);
    
    aBasePoints->setColor(theColor);
    aBasePoints->setOpacity(0);
	aBasePoints->setPosition(ccp(thePos.x,thePos.y+50));
	
	this->addChild(aBasePoints, kPoints_Z_Order);
    
    //Add some action to this
    CCFadeIn* aFadeIn = CCFadeIn::create(0.2f);
    CCMoveBy* aMove = CCMoveBy::create(0.5f, ccp(0,50));
    CCDelayTime* aDelay = CCDelayTime::create(0.2f);
    CCFadeOut* aFadeOut = CCFadeOut::create(0.2f);
    
    CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    
    CCSpawn* aSpawnAction = CCSpawn::create(aFadeIn,aMove,NULL);
    CCSequence* aAllActions = CCSequence::create(aSpawnAction,aDelay,aFadeOut,func2,NULL);
    
    aBasePoints->runAction(aAllActions);
    
    //We have some bonus points
    if (theBonus>0)
    {
        char bonusString[32];
        sprintf(bonusString, "+%d", amount);
        
        CCLabelTTF* aExtraPoints = CCLabelTTF::create(bonusString, FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*0.7, CCSize(115, 55), kCCTextAlignmentCenter);
        
        aExtraPoints->setColor(ccc3(255, 197, 0));
        aExtraPoints->setOpacity(0);
        aExtraPoints->setPosition(ccp(thePos.x,thePos.y+100));
        
        this->addChild(aExtraPoints, kPoints_Z_Order);
        
        CCDelayTime* aExtraDelay = CCDelayTime::create(0.8f);
        CCFadeIn* aExtraFadeIn = CCFadeIn::create(0.2f);
        CCRotateBy* aRotateBy1 = CCRotateBy::create(0.2f, 10);
        CCRotateBy* aRotateBy2 = CCRotateBy::create(0.2f, -10);
        CCFadeOut* aExtraFadeOut = CCFadeOut::create(0.2f);
        
        CCScaleTo* aScale1 = CCScaleTo::create(0.2f, 1.3f);
        CCScaleTo* aScale2 = CCScaleTo::create(0.2f, 1.0f);
        
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
        
        CCSpawn* aExtraSpawn = CCSpawn::create(aExtraFadeIn,aRotateBy1,aScale1,NULL);
        CCSpawn* aExtraSpawn2 = CCSpawn::create(aRotateBy2,aExtraFadeOut,aScale2,NULL);
        CCSequence* aExtraSeq = CCSequence::create(aExtraDelay,aExtraSpawn,aExtraSpawn2,func,NULL);
        
        aExtraPoints->runAction(aExtraSeq);
    }
    
    //Add the points after effect end ?
    if(crystalPoint)
    {
        //
    }
    
//    if(User::getInstance()->mNewMissionBuild == false){
        addScore(amount*mScoreMulti_Global);
        addScore(theBonus);
//    }
}

void GameScene::lose()
{
    if(User::getInstance()->mNewMissionBuild && !User::getInstance()->mDynamicTrolls){
        //Go back to the mission scene
        
//        CCScene* options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MISSIONS);
        CCScene* options = MissionScene::scene();
        CCTransitionScene* transition = CCTransitionFade::create(0.5f,options,ccBLACK);
        CCDirector::sharedDirector()->replaceScene(transition);
        
        return;
    }
    
    //Check if does not need to show free diamonds 1st !!!
    // Free Diamonds popup
//    if (User::getInstance()->getFreeDmdsWeek() != Time::getYWeek()
//        && User::getInstance()->getSessionLength() > 300
//        && !User::getInstance()->isFollowingSocialNetwork()
//        && User::getInstance()->showFreeStuffPopup())
//    {
//        FreeStuffPopup* popup = FreeStuffPopup::create();
//        popup->retain();
//        popup->_gameScene = this;
//        
//        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
//        
//        cocos2d::CCLayer* activePopup;
//        
//        activePopup = popup;
//        
//        activePopup->setAnchorPoint(ccp(0,0));
//        activePopup->setPosition(ccp(0,-visibleSize.height));
//        activePopup->runAction(CCMoveTo::create(0.5, ccp(0,0)));
//        addChild(activePopup, 100);
//    }
//    else
//    {
//        CCScene* end = DF::LevelEnd::LevelEndScene::scene(_stats);
//        CCDirector::sharedDirector()->replaceScene(end);
//    }
    
    //Save some stats to flurry !!!
    /*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::stringstream aStream;
    aStream<<_gameTime;
    FlurryX::logEvent("Game End | Length:",aStream.str().c_str());
    aStream.str("");aStream.clear();
    
    aStream<<mCrystalPoints;
    FlurryX::logEvent("Game End | Points:",aStream.str().c_str());
    aStream.str("");aStream.clear();
    
    aStream<<mFlurry_MachinesActivated;
    FlurryX::logEvent("Game End | Machines:",aStream.str().c_str());
    aStream.str("");aStream.clear();
    
    aStream<<mFlurry_CrystalPoints;
    FlurryX::logEvent("Game End | Crystals:",aStream.str().c_str());
    aStream.str("");aStream.clear();
    
    aStream<<User::getInstance()->getDiamonds();
    FlurryX::logEvent("Game End | Diamonds:",aStream.str().c_str());
    aStream.str("");aStream.clear();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::stringstream aStream;
    aStream<<_gameTime;
    CCDictionary *dict = CCDictionary::create();//createWithContentsOfFile(plist);
//    dict->setObject(aStream.str(), "time");
    CCString *str1 = CCString::create(aStream.str().c_str());
    dict->setObject(str1,"time");
    FlurryX::logEvent("Game End | Length:",dict);
    aStream.str("");aStream.clear();
    
    aStream<<mCrystalPoints;
    FlurryX::logEvent("Game End | Points:","Points",aStream.str().c_str());
    aStream.str("");aStream.clear();
    
    aStream<<mFlurry_MachinesActivated;
    FlurryX::logEvent("Game End | Machines:","Active",aStream.str().c_str());
    aStream.str("");aStream.clear();
    
    aStream<<mFlurry_CrystalPoints;
    FlurryX::logEvent("Game End | Crystals:","Points",aStream.str().c_str());
    aStream.str("");aStream.clear();
    
    aStream<<User::getInstance()->getDiamonds();
    FlurryX::logEvent("Game End | Diamonds:","Amount",aStream.str().c_str());
    aStream.str("");aStream.clear();
#endif
    */
    
    
//    FlurryX::logEvent("Game End | Length:",_gameTime);
//    FlurryX::logEvent("Game End | Points:",mCrystalPoints);
//    FlurryX::logEvent("Game End | Machines:",mFlurry_MachinesActivated);
//    FlurryX::logEvent("Game End | Crystals:",mFlurry_CrystalPoints);
//    FlurryX::logEvent("Game End | Diamonds:",User::getInstance()->getDiamonds());
    
    
    CCScene* end = DF::LevelEnd::LevelEndScene::scene(_stats);
    CCTransitionFade* transition = CCTransitionFade::create(0.25f,end,ccBLACK);
    CCDirector::sharedDirector()->replaceScene(transition);
    
//    CCScene* mainMenu = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
    //	CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, mainMenu);
//    CCTransitionFade* transition = CCTransitionFade::create(0.25f,mainMenu,ccBLACK);
//	CCDirector::sharedDirector()->replaceScene(transition);
    
    
    //This should be at game end !!!
    CCDictionary *gameEndStats = new CCDictionary();
    gameEndStats->autorelease();
    
    gameEndStats->setObject(CCInteger::create(_gameTime), "In Game Time");
    gameEndStats->setObject(CCInteger::create(mTotalPointsInGame), "Points");
    gameEndStats->setObject(CCInteger::create(mFlurry_BlitzActivated), "Activated Blitz");
    gameEndStats->setObject(CCInteger::create(mFlurry_StampActivated), "Activated Stamp");
    gameEndStats->setObject(CCInteger::create(mFlurry_GhostActivated), "Activated Ghoust");
    gameEndStats->setObject(CCInteger::create(mFlurry_CrystalPoints), "Collected Crystals");
    gameEndStats->setObject(CCInteger::create(User::getInstance()->getDiamonds()), "Diamonds");
    gameEndStats->setObject(CCInteger::create(User::getInstance()->getMissionManager().GetActiveMissionID()), "Active Mission");
    
    AnalyticX::flurryLogEventWithParameters("GameOverStats", gameEndStats);
}

void GameScene::CreateLevelEnd(CCNode* theSender)
{
    theSender->release();
    removeChild(theSender);
    
    CCScene* end = DF::LevelEnd::LevelEndScene::scene(_stats);
    CCDirector::sharedDirector()->replaceScene(end);
}

void GameScene::CreateDwarfDebug()
{
    generateDwarf(-1);
}

void GameScene::CreateParticleOnShow(CCNode* sender)
{
    CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/KaboomFx.plist");
    p->setPosition(sender->getContentSize().width/2,sender->getContentSize().height/2);
    p->setAutoRemoveOnFinish(true);
    sender->addChild(p,-1);
    
    playInGameSound("crystal_pick_up");
}

void GameScene::NewMissionCompleted()
{
    
    _gamePause = true;
    pauseSchedulerAndActionsRecursive(this,false);
    
    //hide the crystal bar !!! daily
    if(mPopUpPanel != NULL){
        mPopUpPanel->setOpacity(0);
    }
    
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    
    CCLayerColor* aLayerFake = CCLayerColor::create(ccc4(0,0,0,64),_screenSize.width,_screenSize.height);
    aLayerFake->setTag(2014);
    addChild(aLayerFake,kHUD_Z_Order+100);
    
    CCSprite* aSpriteDum = CCSprite::create("Interfeiss/challenges/complete_popup/dont_leave.png");
    aLayerFake->addChild(aSpriteDum);
    aSpriteDum->setPosition(ccp(aLayerFake->getContentSize().width/2,aLayerFake->getContentSize().height/2));
    
    //Add some text !!!
    cocos2d::CCLabelTTF* _PopText;
    //Add the text field !!!
    _PopText = CCLabelTTF::create("",
                                  "fonts/Marker Felt.ttf", TITLE_FONT_SIZE*0.5,
                                  CCSize(400, 280), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    _PopText->setAnchorPoint(ccp(0.5,0.5f));
    _PopText->setColor(ccc3(79, 65, 33));
    _PopText->setPositionX(350);//This can change by reward type
    _PopText->setPositionY(300);
    aSpriteDum->addChild(_PopText);
    
    std::stringstream theTextField;
    
    if(User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission || User::getInstance()->mSpecial_21_Mission
       || User::getInstance()->mSpecial_22_Mission || User::getInstance()->mSpecial_23_Mission)
    {
        if(User::getInstance()->mSpecial_19_Mission){
            theTextField<<"Congratz, you  transfered "<< _mission_star_points_1 << " Dwarfs!";
        }
        else if(User::getInstance()->mSpecial_20_Mission){
            theTextField<<"Congratz, you  gathered "<< _mission_star_points_1 << " Crystals!";
        }
        else if(User::getInstance()->mSpecial_21_Mission){
            theTextField<<"Congratz, you  gathered "<< _mission_star_points_1 << " Crystals!";
        }
        else if(User::getInstance()->mSpecial_22_Mission){
            theTextField<<"Congratz, you  transfered "<< _mission_star_points_1 << " Dwarfs!";
        }
        else if(User::getInstance()->mSpecial_23_Mission){
            theTextField<<"Congratz, you  transfered "<< _mission_star_points_1 << " Dwarfs!";
        }
        
        //Add some part image !!!
        CCSprite* aDebugPic = CCSprite::create("MissionCompleted.png");
        CCSprite* aIcon = NULL;
        
        User::getInstance()->mShowNewPart = true;
        
        aDebugPic->setPosition(ccp(300,-40));
        
        aSpriteDum->addChild(aDebugPic);
    }
    else if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission)
    {
        theTextField<<"Congratz, you  gathered "<< _mission_star_points_1 << " Crystals!";
        
        //Add some part image !!!
        CCSprite* aDebugPic = CCSprite::create("MissionCompleted.png");
        CCSprite* aIcon = NULL;
        
        User::getInstance()->mShowNewPart = true;
        
        aDebugPic->setPosition(ccp(300,-40));
        
        aSpriteDum->addChild(aDebugPic);
    }
    else if(User::getInstance()->mNewMissionBuild){
        theTextField<<"Congrats! Well done!\nYou completed level!";
        
        
        
        //Add some part image !!!
        CCSprite* aDebugPic = CCSprite::create("MissionCompleted.png");
        CCSprite* aIcon = NULL;
        
        CCScaleTo* aScaleUp;
        CCEaseBackOut* aScaleEase;
        CCCallFuncN* func;
        CCSequence* aSeq;
        CCDelayTime* aDelay;
        CCSpawn* aSpawn;
        CCDelayTime* aDelay2;
        CCSequence* aSeq2;
        
        if(_mission_star_1->getOpacity() > 128){
            aIcon = CCSprite::create("new_mission/zvaigzne_final.png");
            aIcon->setPosition(ccp(200,100));
            aIcon->setScale(0);
            aSpriteDum->addChild(aIcon);
            
            //Lets add a small fun here
            aScaleUp = CCScaleTo::create(0.5f,1.0f);
            aScaleEase = CCEaseBackOut::create(aScaleUp);
            
            aDelay = CCDelayTime::create(0.2f);
            func = CCCallFuncN::create(this, callfuncN_selector(GameScene::CreateParticleOnShow));
            
            aSeq = CCSequence::create(aDelay,func,NULL);
            aSpawn = CCSpawn::create(aScaleEase,aSeq,NULL);
            
            aIcon->runAction(aSpawn);
        }
        
        if(_mission_star_2->getOpacity() > 128){
            aIcon = CCSprite::create("new_mission/zvaigzne_final.png");
            aIcon->setPosition(ccp(270,95));
            aIcon->setScale(0);
            aSpriteDum->addChild(aIcon);
            
            //Lets add a small fun here
            aScaleUp = CCScaleTo::create(0.5f,1.0f);
            aScaleEase = CCEaseBackOut::create(aScaleUp);
            
            aDelay = CCDelayTime::create(0.2f);
            func = CCCallFuncN::create(this, callfuncN_selector(GameScene::CreateParticleOnShow));
            
            aSeq = CCSequence::create(aDelay,func,NULL);
            aSpawn = CCSpawn::create(aScaleEase,aSeq,NULL);
            
            aDelay2 = CCDelayTime::create(0.3f);
            aSeq2 = CCSequence::create(aDelay2,aSpawn,NULL);
            
            aIcon->runAction(aSeq2);
        }
        
        if(_mission_star_3->getOpacity() > 128){
            aIcon = CCSprite::create("new_mission/zvaigzne_final.png");
            aIcon->setPosition(ccp(340,100));
            aIcon->setScale(0);
            aSpriteDum->addChild(aIcon);
            
            //Lets add a small fun here
            aScaleUp = CCScaleTo::create(0.5f,1.0f);
            aScaleEase = CCEaseBackOut::create(aScaleUp);
            
            aDelay = CCDelayTime::create(0.2f);
            func = CCCallFuncN::create(this, callfuncN_selector(GameScene::CreateParticleOnShow));
            
            aSeq = CCSequence::create(aDelay,func,NULL);
            aSpawn = CCSpawn::create(aScaleEase,aSeq,NULL);
            
            aDelay2 = CCDelayTime::create(0.6f);
            aSeq2 = CCSequence::create(aDelay2,aSpawn,NULL);
            
            aIcon->runAction(aSeq2);
        }
        
        if(User::getInstance()->mNewMissionProgress<6){
            User::getInstance()->mNewMissionProgress+=1;
            
            User::getInstance()->mSpecialMissionProgress+=1;
            
            User::getInstance()->mShowNewPart = true;
        }
        
        aDebugPic->setPosition(ccp(300,-40));
        
        aSpriteDum->addChild(aDebugPic);
    }
    
    _PopText->setString(theTextField.str().c_str());
    
    //Add some button for clouse
    CCMenuItemImage* debug_button_1 = CCMenuItemImage::create(
                                                              "Interfeiss/before_quit/check_btn0001.png",
                                                              "Interfeiss/before_quit/check_btn0002.png",
                                                              this,
                                                              menu_selector(GameScene::OnExitWithNewMission));
    
    CCMenu* debugMenu = CCMenu::create(debug_button_1,NULL);
    debugMenu->setPosition(ccp(500,100));
    aSpriteDum->addChild(debugMenu);
}

void GameScene::OnExitWithNewMission()
{
    removeChildByTag(2014);
    
    //just exit for now !!!
//    CCScene* options = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MISSIONS);
    //        CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, options);
//    CCTransitionScene* transition = CCTransitionFade::create(0.5f,options,ccBLACK);
//    CCDirector::sharedDirector()->replaceScene(transition);
    
    
    CCScene* options = MissionScene::scene();
    CCTransitionScene* transition = CCTransitionFade::create(0.5f,options,ccBLACK);
    CCDirector::sharedDirector()->replaceScene(transition);
}

void GameScene::OnCompletedMission16()
{
    
}

Spider* GameScene::generateSpider(int theX,int theY,float theRadius)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    float angle = (rand() % 360) * M_PI / 180.0f;
    
    Spider* troll = Spider::create(this);
    troll->setPosition(theX,theY);
    
    troll->setAngle(angle);
    
    this->addChild(troll, getSpriteOrderZ(troll->getPositionY()));
    _spiders->addObject(troll);
    troll->setVisible(false);
    
    /*
    IntroAnimation* introAnimation = HidraIntro::create(this, troll);
    introAnimation->setPosition(troll->getPosition());
    
    this->addChild(introAnimation, getSpriteOrderZ(introAnimation->getPositionY()));
    _introAnimations->addObject(introAnimation);
    */
    
    return troll;
}

Hidra* GameScene::generateHidra(int theX,int theY,float theRadius)
{
//    Hidra* troll = Hidra::create(this);
//    troll->setPosition(theX,theY);
//    troll->setAngle(theRadius);
//    
//    this->addChild(troll, getSpriteOrderZ(troll->getPositionY()));
//    _hidras->addObject(troll);
//    troll->setVisible(true);
    
    
    //----------
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    float angle = (rand() % 360) * M_PI / 180.0f;
    
    Hidra* troll = Hidra::create(this);
    troll->setPosition(theX,theY);
    
    troll->setAngle(angle);
    
    this->addChild(troll, getSpriteOrderZ(troll->getPositionY()));
    _hidras->addObject(troll);
    troll->setVisible(false);
    
    IntroAnimation* introAnimation = HidraIntro::create(this, troll);
    introAnimation->setPosition(troll->getPosition());
    
    this->addChild(introAnimation, getSpriteOrderZ(introAnimation->getPositionY()));
    _introAnimations->addObject(introAnimation);
    
    return troll;
}

Goblin* GameScene::generateGoblin(int theX,int theY,float theRadius)
{
    Goblin* troll = Goblin::create(this);
    troll->setPosition(theX,theY);
    troll->setAngle(theRadius);
    
    this->addChild(troll, getSpriteOrderZ(troll->getPositionY()));
    _goblins->addObject(troll);
    troll->setVisible(true);
    
//    IntroAnimation* introAnimation = TrollIntro::create(this, troll);
//    introAnimation->setPosition(troll->getPosition());
//    this->addChild(introAnimation, getSpriteOrderZ(introAnimation->getPositionY()));
//    _introAnimations->addObject(introAnimation);
}

void GameScene::generateDwarfMission(bool theInstant)
{
    //Why do we have 2 values of this?
    _mission_dwarfs_spawned+=1;
    _DwarfsSpawned+=1;
    
    //Check what type can we spawn !!!
    int theType = DWARF_TYPE_TALL;
    
    if(_SpawnBlueDwarf && _SpawnOrangeDwarf){
        //All dwarfs
        theType = rand()%2;
    }
    else if(_SpawnBlueDwarf){
        //Only blue
        theType = DWARF_TYPE_FAT;
    }
    else if(_SpawnOrangeDwarf){
        theType = DWARF_TYPE_TALL;
    }
    
    _possibleGeneratePoints.clear();
    bool aCanUseCave = false;//Safe check if can spawn
    int aIndexPos = 0;
    
    //Lets check where can we spawn
    for (int i = 0;i<_genearetPoints.size();i++)
    {
        aCanUseCave = true;
        
        for(int x=0;x<_lastSpawnPoints.size();x++)
        {
            if (i==_lastSpawnPoints[x])
            {
                aCanUseCave = false;
                break;
            }
        }
        
        if (aCanUseCave)
            _possibleGeneratePoints.push_back(i);
    }
    
    //Do not spawn last 3 caves !!!
    if (_possibleGeneratePoints.size() > 0)
    {
        _lastSpawnPoint = _possibleGeneratePoints[rand() % _possibleGeneratePoints.size()];
        
        if(mLastSpawnID == _lastSpawnPoint){
            //Check if will allow
            int aWillAllow = rand()%100;
            if(aWillAllow>=mCurrentMission.DwarfSpawnOneCave){
                _LeftNotSpawnDwatfs+=1;
                return;
            }
        }
        _lastSpawnPoints.push_back(_lastSpawnPoint);
    }
    else{
        _LeftNotSpawnDwatfs+=1;
        return;//Can't spawn dwarf on dwarf
    }
    
    if(mCurrentMission.DSpawn_formula_type == 2 || mCurrentMission.DSpawn_formula_type == 1){
        _DSpawnGameMinCurrent-=1;
    }
    
    mLastSpawnID = _lastSpawnPoint;
    
    //Should remove on dwarf exit !!!
//    if (_lastSpawnPoints.size()>3)
//    {
//        //Remove first elemet
//        _lastSpawnPoints.erase(_lastSpawnPoints.begin(),_lastSpawnPoints.begin()+1);
//    }
    
    Dwarf* dwarf = Dwarf::create(this,theType);
    
    if(theType == DWARF_TYPE_FAT){
        dwarf->_speed = mCurrentMission.DwarfSpeed_Fat;
        dwarf->_defaultSpeed = mCurrentMission.DwarfSpeed_Fat;
    }
    else{
        dwarf->_speed = mCurrentMission.DwarfSpeed_Tall;
        dwarf->_defaultSpeed = mCurrentMission.DwarfSpeed_Tall;
    }
    
    
    //Will remove where next dwarf can spawn !!!
    dwarf->_SpawnID = _lastSpawnPoint;
    
    aIndexPos = _lastSpawnPoint;
    
    GeneratePoint generatePoint = _genearetPoints[aIndexPos];
    dwarf->setPosition(generatePoint.x,generatePoint.y);
    dwarf->_SpawnStart = CCPoint(generatePoint.x, generatePoint.y);
    
    dwarf->setAngle(generatePoint.angle);
    
    this->addChild(dwarf, getSpriteOrderZ(dwarf->getPositionY()));
    _dwarves->addObject(dwarf);
    
    dwarf->_disabled = false;
//    dwarf->setVisible(false);
    
    if(theInstant){
        return;
    }
    
    //The spawn
    dwarf->setVisible(false);
    
    IntroAnimation* introAnimation = DwarfIntro::create(this, dwarf);
    introAnimation->setPosition(dwarf->getPosition());
    
    this->addChild(introAnimation, getSpriteOrderZ(introAnimation->getPositionY()));
    _introAnimations->addObject(introAnimation);
}

Dwarf* GameScene::generateDwarf(int theType,int theSpawnPoint)
{
    //For now
//    if(_dwarves->count()>0){
//        return NULL;
//    }
//    return NULL;
    //Check if can spawn
    
    if(User::getInstance()->mNewMissionBuild)
    {
        if(User::getInstance()->mSpecial_19_Mission || User::getInstance()->mSpecial_20_Mission)
        {
            if(_dwarves->count()<3){
                _mission_dwarf_spawn_timer = 0;
            }
            else{
                _mission_dwarf_spawn_timer = 6;
            }
            
            //Limit to  6 dwarfs max in map !!!
            if(_dwarves->count()>5){
                return NULL;
            }
        }
        else if(User::getInstance()->mSpecial_21_Mission){
            if(_dwarves->count()<3){
                _mission_dwarf_spawn_timer = 0;
            }
            else{
                _mission_dwarf_spawn_timer = 6;
            }
            
            //Limit to  6 dwarfs max in map !!!
            if(_dwarves->count()>4){
                return NULL;
            }
        }
        else if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission || User::getInstance()->mSpecial_18_Mission)
        {
            if(_dwarves->count()<3){
                _mission_dwarf_spawn_timer = 2;
            }
            else{
                _mission_dwarf_spawn_timer = 6;
            }
        }
        else
        {
            _mission_dwarf_spawn_timer = 12;
        }
        
        
        if(_mission_dwarfs_spawned>=_mission_dwarfs_max && User::getInstance()->mDynamicTrolls==false)
        {
            //Show game over - buy 2 more dwarfs for save
            if(_dwarves->count()==0){
                
                _gamePause = true;
                pauseSchedulerAndActionsRecursive(this,false);
                
                SimpleAudioEngine::sharedEngine()->stopAllEffects();
                
                //Check if has reached ant star if so - completed mission
                if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission)
                {
                    //Ask for save or end !!!
                    if(mTotalPointsInGame>=_mission_star_points_1)
                    {
                        //All ok - do it !!!
                        NewMissionCompleted();
                    }
                    else
                    {
                        //Gameover !!!
                        nowCreateSaveMe();
                    }
                }
                else
                {
                    if(_mission_star_1->getOpacity()>128)
                    {
                        //Mission completed
                        NewMissionCompleted();
                    }
                    else
                    {
                        //This is it !!!
                        nowCreateSaveMe();
                    }
                }
            }
            
            return NULL;
        }
        
        _mission_dwarfs_spawned+=1;
    }
    
//    if(theType!=0 && theType!=1 && theType!=-1)
//        theType = -1;
//    return NULL;
    
    //Check how many dwarfs are on the map !!!
    if(_dwarves->count() <3)
    {
        if(mMusic1_On!=false)
            mMusic1_Done = false;
        if(mMusic2_On!=false)
            mMusic2_Done = false;
        if(mMusic3_On!=false)
            mMusic3_Done = false;
        
        mMusic1_On = false;
        mMusic2_On = false;
        mMusic3_On = false;
    }
    else if(_dwarves->count()<5)
    {
        if(mMusic1_On!=true)
            mMusic1_Done = false;
        if(mMusic2_On!=false)
            mMusic2_Done = false;
        if(mMusic3_On!=false)
            mMusic3_Done = false;
        
        mMusic1_On = true;
        mMusic2_On = false;
        mMusic3_On = false;
    }
    else if(_dwarves->count()<7)
    {
        if(mMusic1_On!=true)
            mMusic1_Done = false;
        if(mMusic2_On!=true)
            mMusic2_Done = false;
        if(mMusic3_On!=false)
            mMusic3_Done = false;
        
        mMusic1_On = true;
        mMusic2_On = true;
        mMusic3_On = false;
    }
    else
    {
        if(mMusic1_On!=true)
            mMusic1_Done = false;
        if(mMusic2_On!=true)
            mMusic2_Done = false;
        if(mMusic3_On!=true)
            mMusic3_Done = false;
        
        mMusic1_On = true;
        mMusic2_On = true;
        mMusic3_On = true;
    }
    
    //how many dwarfs spawned here
    if(User::getInstance()->mDynamicTrolls || User::getInstance()->mSpecial_18_Mission || User::getInstance()->mSpecial_19_Mission
       || User::getInstance()->mSpecial_20_Mission)
    {
        if(mNewSplitCaves && User::getInstance()->mSpecial_20_Mission){
            //Nothing
        }
        else if(_DwarfsSpawned==10){
            if(User::getInstance()->mSpecial_21_Mission){
                //Do nothing
            }
            else{
                SetMasterTrollAction(2);
            }
        }
    }
    else if(User::getInstance()->mSpecial_16_Mission || User::getInstance()->mSpecial_17_Mission)
    {
        if(_DwarfsSpawned==3)
        {
            SetMasterTrollAction(1);
        }
        if(_DwarfsSpawned==10)
        {
            SetMasterTrollAction(2);
        }
        if(_DwarfsSpawned==20)
        {
            SetMasterTrollAction(3);
        }
        if(_DwarfsSpawned==30)
        {
            SetMasterTrollAction(4);
        }
        if(_DwarfsSpawned==40)
        {
            SetMasterTrollAction(5);
        }
    }
    else if(User::getInstance()->mSpecial_14_Mission)
    {
        GeneratePoint spawnIcePoint;
        CCMoveTo* aMove1;
        CCEaseBackIn* aEase1;
        
        
        
        //Check when need to colapse enterances
        if(_DwarfsSpawned==8)
        {
            //Block 2 start points + shake screen
            _blockedGeneratePoints.push_back(0);
            _blockedGeneratePoints.push_back(4);
            
            spawnIcePoint = _genearetPoints[0];
            
            //Create some block?
            CCSprite* aCaveBlock = CCSprite::create("ice_barrage/state_2.png");
            aCaveBlock->setPosition(ccp(spawnIcePoint.x,spawnIcePoint.y+_screenSize.height*2));
            
            aMove1 = CCMoveTo::create(1.0,ccp(spawnIcePoint.x,spawnIcePoint.y));
            aEase1 = CCEaseBackIn::create(aMove1);
            aCaveBlock->runAction(aEase1);
            
            addChild(aCaveBlock);
            
            spawnIcePoint = _genearetPoints[4];
            
            aCaveBlock = CCSprite::create("ice_barrage/state_2.png");
            
            aCaveBlock->setPosition(ccp(spawnIcePoint.x,spawnIcePoint.y+_screenSize.height*2));
            
            aMove1 = CCMoveTo::create(1.0,ccp(spawnIcePoint.x,spawnIcePoint.y));
            aEase1 = CCEaseBackIn::create(aMove1);
            aCaveBlock->runAction(aEase1);
            
            addChild(aCaveBlock);
            
            runAction(CCShake::actionWithDuration( 1.0f, 5.0f ));
            playInGameSound("GS_Activate");
        }
        else if(_DwarfsSpawned == 16)
        {
            //2 more
            _blockedGeneratePoints.push_back(1);
            _blockedGeneratePoints.push_back(5);
            
            spawnIcePoint = _genearetPoints[1];
            
            //Create some block?
            CCSprite* aCaveBlock = CCSprite::create("ice_barrage/state_2.png");
            aCaveBlock->setPosition(ccp(spawnIcePoint.x,spawnIcePoint.y+_screenSize.height));
            
            aMove1 = CCMoveTo::create(0.5,ccp(spawnIcePoint.x,spawnIcePoint.y));
            aEase1 = CCEaseBackIn::create(aMove1);
            aCaveBlock->runAction(aEase1);
            addChild(aCaveBlock);
            
            spawnIcePoint = _genearetPoints[5];
            
            aCaveBlock = CCSprite::create("ice_barrage/state_2.png");
            aCaveBlock->setPosition(ccp(spawnIcePoint.x,spawnIcePoint.y+_screenSize.height));
            
            aMove1 = CCMoveTo::create(0.5,ccp(spawnIcePoint.x,spawnIcePoint.y));
            aEase1 = CCEaseBackIn::create(aMove1);
            aCaveBlock->runAction(aEase1);
            addChild(aCaveBlock);
            
            runAction(CCShake::actionWithDuration( 1.0f, 5.0f ));
            playInGameSound("GS_Activate");
        }
        else if(_DwarfsSpawned == 24)
        {
            //2 more
            _blockedGeneratePoints.push_back(2);
            _blockedGeneratePoints.push_back(6);
            
            spawnIcePoint = _genearetPoints[2];
            
            //Create some block?
            CCSprite* aCaveBlock = CCSprite::create("ice_barrage/state_2.png");
            aCaveBlock->setPosition(ccp(spawnIcePoint.x,spawnIcePoint.y+_screenSize.height));
            
            aMove1 = CCMoveTo::create(0.5,ccp(spawnIcePoint.x,spawnIcePoint.y));
            aEase1 = CCEaseBackIn::create(aMove1);
            aCaveBlock->runAction(aEase1);
            addChild(aCaveBlock);
            
            spawnIcePoint = _genearetPoints[6];
            
            aCaveBlock = CCSprite::create("ice_barrage/state_2.png");
            aCaveBlock->setPosition(ccp(spawnIcePoint.x,spawnIcePoint.y+_screenSize.height));
            
            aMove1 = CCMoveTo::create(0.5,ccp(spawnIcePoint.x,spawnIcePoint.y));
            aEase1 = CCEaseBackIn::create(aMove1);
            aCaveBlock->runAction(aEase1);
            addChild(aCaveBlock);
            
            runAction(CCShake::actionWithDuration( 1.0f, 5.0f ));
            playInGameSound("GS_Activate");
        }
        else if(_DwarfsSpawned == 32)
        {
            //2 more
            _blockedGeneratePoints.push_back(3);
            _blockedGeneratePoints.push_back(7);
            
            spawnIcePoint = _genearetPoints[3];
            
            //Create some block?
            CCSprite* aCaveBlock = CCSprite::create("ice_barrage/state_2.png");
            aCaveBlock->setPosition(ccp(spawnIcePoint.x,spawnIcePoint.y+_screenSize.height));
            
            aMove1 = CCMoveTo::create(0.5,ccp(spawnIcePoint.x,spawnIcePoint.y));
            aEase1 = CCEaseBackIn::create(aMove1);
            aCaveBlock->runAction(aEase1);
            addChild(aCaveBlock);
            
            spawnIcePoint = _genearetPoints[7];
            
            aCaveBlock = CCSprite::create("ice_barrage/state_2.png");
            aCaveBlock->setPosition(ccp(spawnIcePoint.x,spawnIcePoint.y+_screenSize.height));
            
            aMove1 = CCMoveTo::create(0.5,ccp(spawnIcePoint.x,spawnIcePoint.y));
            aEase1 = CCEaseBackIn::create(aMove1);
            aCaveBlock->runAction(aEase1);
            addChild(aCaveBlock);
            
            runAction(CCShake::actionWithDuration( 1.0f, 5.0f ));
            playInGameSound("GS_Activate");
        }
    }
    
    _DwarfsSpawned+=1;
    
//	if (_dwarves->count() < MAX_DWARFS && _dwarfTimer>3)
	{
        _dwarfTimer = 0;//Safe 3 seconds
        
		Dwarf* dwarf = Dwarf::create(this,theType);
        
        //Create the possible spawn points,except last one
        _possibleGeneratePoints.clear();
        bool aCanUseCave = false;
        
        for (int i = 0;i<_genearetPoints.size();i++)
        {
            aCanUseCave = true;
            
            if(User::getInstance()->mSpecial_14_Mission)
            {
                for(int x=0;x<_blockedGeneratePoints.size();x++)
                {
                    if(i == _blockedGeneratePoints[x])
                    {
                        aCanUseCave = false;
                        break;
                    }
                }
            }
            else
            {
                //All normal - chill
                for(int x=0;x<_lastSpawnPoints.size();x++)
                {
                    if (i==_lastSpawnPoints[x])
                    {
                        aCanUseCave = false;
                        break;
                    }
                }
            }
            
            if (aCanUseCave)
                _possibleGeneratePoints.push_back(i);
        }
        
        int aIndexPos = 0;
        
        if(theSpawnPoint!=-1)
        {
            aIndexPos = theSpawnPoint;
        }
        else if(User::getInstance()->mSpecial_14_Mission)
        {
            //Do not spawn last 3 caves !!!
            if (_possibleGeneratePoints.size() > 0)
            {
                aIndexPos = _possibleGeneratePoints[rand() % _possibleGeneratePoints.size()];
            }
        }
        else
        {
            //Do not spawn last 3 caves !!!
            if (_possibleGeneratePoints.size() > 0)
            {
                _lastSpawnPoint = _possibleGeneratePoints[rand() % _possibleGeneratePoints.size()];
                _lastSpawnPoints.push_back(_lastSpawnPoint);
            }
            
            if (_lastSpawnPoints.size()>3)
            {
                //Remove first elemet
                _lastSpawnPoints.erase(_lastSpawnPoints.begin(),_lastSpawnPoints.begin()+1);
            }
            
            aIndexPos = _lastSpawnPoint;
        }
        
        //Special stuff
        if(User::getInstance()->mSpecial_14_Mission)
        {
            //Block 2 spawn points
            if(_possibleGeneratePoints.size()<=0)
            {
                return NULL;//No spawn
            }
        }
        
//        GeneratePoint generatePoint = _genearetPoints[_lastSpawnPoint];
        GeneratePoint generatePoint = _genearetPoints[aIndexPos];
		
		dwarf->setPosition(generatePoint.x,generatePoint.y);
		
		dwarf->setAngle(generatePoint.angle);
        
		this->addChild(dwarf, getSpriteOrderZ(dwarf->getPositionY()));
		_dwarves->addObject(dwarf);
		dwarf->setVisible(false);
		
		IntroAnimation* introAnimation = DwarfIntro::create(this, dwarf);
		introAnimation->setPosition(dwarf->getPosition());
		
		this->addChild(introAnimation, getSpriteOrderZ(introAnimation->getPositionY()));
		_introAnimations->addObject(introAnimation);
        
//        PlaySpecialMusic(_dwarves->count());
        
        if(!mTutorialEnabled)
        {
            if(_gameTime<EXTRA_CRYSTAL_SPAWN)
                mSpecialCrystalSpawnTimer = 1.0f;
        }
		
		return dwarf;
	}
	
	return NULL;
}

void GameScene::generateTrollMission(int theX,int theY,float theRadius,bool theCircle,
                                     bool theControlPoint,cocos2d::CCPoint theControlPoint_1,cocos2d::CCPoint theControlPoint_2,int theDrawLine)
{
    if(User::getInstance()->mSpecial_23_Mission){
        return;
    }
    
    Troll* troll = Troll::create(this);
    troll->setTag(879);
    troll->setPosition(theX,theY);
    
    troll->mTrollStartY = theY;
    
    if(theDrawLine != -1){
        CCSprite* aSprite = NULL;
        
//       troll->SetDrawLine(theDrawLine);
        //Add the line on game !!!
        if(theDrawLine == 1){
            troll->mDrawLineID = 33131;
            aSprite =CCSprite::create("Troll_Line_1.png");
            aSprite->setPosition(ccp(theX,theY-30));
            aSprite->setTag(33131);
        }
        else if(theDrawLine == 2){
            troll->mDrawLineID = 33132;
            aSprite =CCSprite::create("Troll_Line_1.png");
            aSprite->setPosition(ccp(theX,theY-30));
            aSprite->setTag(33132);
        }
        else if(theDrawLine == 3){
            troll->mDrawLineID = 33133;
            aSprite =CCSprite::create("Troll_Line_2.png");
            aSprite->setPosition(ccp(theX,theY));
            aSprite->setTag(33133);
        }
        else if(theDrawLine == 4){
            troll->mDrawLineID = 33134;
            aSprite =CCSprite::create("Troll_Line_2.png");
            aSprite->setPosition(ccp(theX,theY));
            aSprite->setTag(33134);
        }
        else if(theDrawLine == 5){
            troll->mDrawLineID = 33135;
            aSprite =CCSprite::create("Troll_Line_3.png");
            aSprite->setPosition(ccp(theX,theY+85));
            aSprite->setTag(33135);
        }
        
//        aSprite->setPosition(ccp(theX,theY));
        addChild(aSprite);
    }
    
    troll->mMoveByNodes = theCircle;
    
    if(theControlPoint){
        troll->mPatrolPoints = true;
        troll->mTrollPatrolPoint_1 = theControlPoint_1;
        troll->mTrollPatrolPoint_2 = theControlPoint_2;
    }
    
    troll->setAngle(theRadius);
//    troll->_angle = theRadius;
    
    if(theRadius == 1.6){
        troll->mMoveCurrentDir = 1;
    }
    else if(theRadius == 2){
        troll->mMoveIndex = 10;
    }
    else{
        troll->mMoveCurrentDir = -1;
    }
    
    this->addChild(troll, getSpriteOrderZ(troll->getPositionY()));
    _trolls->addObject(troll);
    troll->setVisible(false);
    
    IntroAnimation* introAnimation = TrollIntro::create(this, troll);
    introAnimation->setPosition(troll->getPosition());
    
    this->addChild(introAnimation, getSpriteOrderZ(introAnimation->getPositionY()));
    _introAnimations->addObject(introAnimation);
}

void GameScene::generateTrollSpecial(int theX,int theY,float theRadius)
{
    if(User::getInstance()->mSpecial_23_Mission){
        return;
    }
    
    Troll* troll = Troll::create(this);
    troll->setTag(879);
    troll->setPosition(theX,theY);
    
    if(theRadius==-1)
        troll->setAngle(6.1);//Update the angle
    else
        troll->setAngle(theRadius);
    
    this->addChild(troll, getSpriteOrderZ(troll->getPositionY()));
    _trolls->addObject(troll);
    troll->setVisible(false);
    
    IntroAnimation* introAnimation = TrollIntro::create(this, troll);
    introAnimation->setPosition(troll->getPosition());
    
    this->addChild(introAnimation, getSpriteOrderZ(introAnimation->getPositionY()));
    _introAnimations->addObject(introAnimation);
}

void GameScene::generateTrollForMission(MissionTroll theTrollInfo)
{
    Troll* troll = Troll::create(this);
    troll->setTag(879);
    troll->SetMissionStuff(theTrollInfo);
    
    this->addChild(troll, getSpriteOrderZ(troll->getPositionY()));
    _trolls->addObject(troll);
    troll->setVisible(false);
    
    IntroAnimation* introAnimation = TrollIntro::create(this, troll);
    introAnimation->setPosition(troll->getPosition());
    
    this->addChild(introAnimation, getSpriteOrderZ(introAnimation->getPositionY()));
    _introAnimations->addObject(introAnimation);
}

Troll* GameScene::generateTroll(bool theSkip)
{
    if(User::getInstance()->mSpecial_23_Mission){
        return NULL;
    }
    
    if(User::getInstance()->mSpecial_22_Mission){
        return NULL;//No trolls here
    }
    
    if (_boostNoEnemyTimer>0 && !theSkip)
        return NULL;//Not today !!!
    
	if (_trolls->count() < MAX_TROLLS || theSkip)
	{
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		
		float angle = (rand() % 360) * M_PI / 180.0f;
		
		Troll* troll = Troll::create(this);
		troll->setPosition(visibleSize.width / 2.0f + cosf(angle) * 200.0f,
						   visibleSize.height / 2.0f + sinf(angle) * 200.0f);
		
		troll->setAngle(angle);
		
		this->addChild(troll, getSpriteOrderZ(troll->getPositionY()));
		_trolls->addObject(troll);
		troll->setVisible(false);
		
		IntroAnimation* introAnimation = TrollIntro::create(this, troll);
		introAnimation->setPosition(troll->getPosition());
		
		this->addChild(introAnimation, getSpriteOrderZ(introAnimation->getPositionY()));
		_introAnimations->addObject(introAnimation);
		
		return troll;
	}
	
	return NULL;
}



Crystal* GameScene::generateCrystal(bool theNearDwarf,int theCrystalID,int theTimeOnMap)
{
	if (_crystals->count() < MAX_CRYSTALS)
	{
//		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		
//		CCPoint position = ccp(rand() % (int)visibleSize.width,
//							   rand() % (int)visibleSize.height);
//		
//		while (getMask(position.x, position.y))
//		{
//			position = ccp(rand() % (int)visibleSize.width,
//						   rand() % (int)visibleSize.height);
//		}
        
        //////////////////////////////////////////////////////////////////////
        //Create the possible spawn points,except last one
        
        if(theNearDwarf && _dwarves->count()>0)
        {
            //Check to what spawn point is dwarf near !!!
            float aNearestDistance = 99999999;
            float aCurrentDistance = 0;
            CCPoint aCurrentPos;
            
            if(mSpeciaCrystallDwarfID!=-1)
            {
                if(mSpeciaCrystallDwarfID>_dwarves->count())
                    mSpeciaCrystallDwarfID = 0;
                
                if(_dwarves->objectAtIndex(mSpeciaCrystallDwarfID)!=NULL)
                {
                    Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(mSpeciaCrystallDwarfID));
                    
                    for(int x=0;x<8;x++)
                    {
                        aCurrentPos = getBlockCenterPos(x);
                        aCurrentDistance = ccpDistanceSQ(dwarf->getPosition(), aCurrentPos);
                        if(aCurrentDistance<aNearestDistance)
                        {
                            aNearestDistance = aCurrentDistance;
                            _crystalLastSpawnBlockID = x;
                        }
                    }
                }
            }
            else
            {
                for (int dwarfIndex = _dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
                {
                    Dwarf* dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(dwarfIndex));
                    
                    for(int x=0;x<8;x++)
                    {
                        aCurrentPos = getBlockCenterPos(x);
                        aCurrentDistance = ccpDistanceSQ(dwarf->getPosition(), aCurrentPos);
                        if(aCurrentDistance<aNearestDistance)
                        {
                            aNearestDistance = aCurrentDistance;
                            _crystalLastSpawnBlockID = x;
                        }
                    }
                }
            }

            
            //Make a little random
            int aToTheLeft = rand()%2;
            if(_crystalLastSpawnBlockID == 0)
            {
                if(aToTheLeft==1)
                    _crystalLastSpawnBlockID = 7;
                else
                    _crystalLastSpawnBlockID = 1;
            }
            else if(_crystalLastSpawnBlockID == 1)
            {
                if(aToTheLeft==1)
                    _crystalLastSpawnBlockID = 0;
                else
                    _crystalLastSpawnBlockID = 2;
            }
            else if(_crystalLastSpawnBlockID == 2)
            {
                if(aToTheLeft==1)
                    _crystalLastSpawnBlockID = 1;
                else
                    _crystalLastSpawnBlockID = 3;
            }
            else if(_crystalLastSpawnBlockID == 3)
            {
                if(aToTheLeft==1)
                    _crystalLastSpawnBlockID = 2;
                else
                    _crystalLastSpawnBlockID = 4;
            }
            else if(_crystalLastSpawnBlockID == 4)
            {
                if(aToTheLeft==1)
                    _crystalLastSpawnBlockID = 3;
                else
                    _crystalLastSpawnBlockID = 5;
            }
            else if(_crystalLastSpawnBlockID == 5)
            {
                if(aToTheLeft==1)
                    _crystalLastSpawnBlockID = 4;
                else
                    _crystalLastSpawnBlockID = 6;
            }
            else if(_crystalLastSpawnBlockID == 6)
            {
                if(aToTheLeft==1)
                    _crystalLastSpawnBlockID = 7;
                else
                    _crystalLastSpawnBlockID = 5;
            }
            else if(_crystalLastSpawnBlockID == 7)
            {
                if(aToTheLeft==1)
                    _crystalLastSpawnBlockID = 0;
                else
                    _crystalLastSpawnBlockID = 6;
            }
        }
        else
        {
            _crystalSpawnPositions.clear();
            for(int i = 0;i<8;i++)
            {
                if (i != _crystalLastSpawnBlockID)
                    _crystalSpawnPositions.push_back(i);
            }
            _crystalLastSpawnBlockID = _crystalSpawnPositions[rand()%_crystalSpawnPositions.size()];
        }
        
        CCPoint aPosition = getRandomPointFromBlock(_crystalLastSpawnBlockID);
        
        //////////////////////////////////////////////////////////////////////
		
		Crystal* crystal = Crystal::create(this,theCrystalID,theTimeOnMap);
//		crystal->setPosition(position);
        crystal->setPosition(aPosition);
		
		addChild(crystal, getSpriteOrderZ(crystal->getPositionY()));
		_crystals->addObject(crystal);
		
		crystal->setVisible(false);
		
		IntroAnimation* introAnimation = CrystalIntro::create(this, crystal);
		introAnimation->setPosition(crystal->getPosition());
		
		addChild(introAnimation, getSpriteOrderZ(introAnimation->getPositionY()));
		_introAnimations->addObject(introAnimation);
		
		return crystal;
	}
	
	return NULL;
}

void GameScene::GiveGoblinSnatch(Crystal* theCrystal)
{
    for(int goblinIndex = _goblins->count()-1;goblinIndex>=0;--goblinIndex)
    {
        Goblin* gob = static_cast<Goblin*>(_goblins->objectAtIndex(goblinIndex));
        if (gob->isVisible())
		{
            if(gob->mCrystalCatch == NULL){
                theCrystal->mStealer = gob;//To remove after dwarf pick up
                gob->mCrystalCatch = theCrystal;
                break;
            }
        }
    }
}

void GameScene::RemoveEffectByChild(Crystal* theCrystal)
{
//    removeChild(introAnimation);
//	_introAnimations->removeObject(introAnimation);
//	
//	switch (introAnimation->getType())
//	{
//		case INTRO_TROLL:
    
    
    //Clear the intro too !!!
    for(int introIndex = _introAnimations->count()-1; introIndex>=0; --introIndex)
    {
        IntroAnimation* intro = static_cast<IntroAnimation*>(_introAnimations->objectAtIndex(introIndex));
        
        if(intro->getType() == INTRO_CRYSTAL)
        {
            CrystalIntro* crystalIntro = (CrystalIntro*)intro;
			Crystal* crystal = crystalIntro->getCrystal();
            if(crystal == theCrystal)
            {
                removeChild(intro);
                _introAnimations->removeObject(intro);
                break;
            }
        }
    }
}

void GameScene::generateDiamondSpecial(int theX,int theY)
{
    Diamond* diamond = Diamond::create(this);
    diamond->setPosition(ccp(theX,theY));
    addChild(diamond, getSpriteOrderZ(diamond->getPositionY()));
    _diamonds->addObject(diamond);
    diamond->onFinishedFlow();
    playInGameSound("diamond_intro");
}

void GameScene::generateCrystalSpecial(int theX,int theY)
{
    /*
    Crystal* crystal = Crystal::create(this);
    crystal->setPosition(theX, theY);
    this->addChild(crystal, getSpriteOrderZ(crystal->getPositionY()));
    _crystals->addObject(crystal);
    crystal->onFinishedIntro();
    */
}

void GameScene::removeCrystal(Crystal* crystal)
{
    if(!mTutorialEnabled)
    {
        if(_gameTime<EXTRA_CRYSTAL_SPAWN)
        {
            mSpeciaCrystallDwarfID = -1;
            mSpecialCrystalSpawnTimer = 1.0f;
        }
    }
    
	this->removeChild(crystal);
	_crystals->removeObject(crystal);
}

void GameScene::removeDiamond(Diamond* diamond)
{
	this->removeChild(diamond);
	_diamonds->removeObject(diamond);
}

void GameScene::generateMushroom()
{
//    bool aCanBeActivated = true;
    int aCanActivateCount = 0;
    
    //Check if we can generate mushroom !!!
    Machine_Ghost* aMachineG = static_cast<Machine_Ghost*>(getChildByTag(1020));
    if(aMachineG->mCanBeActivated)
        aCanActivateCount+=1;
    
    Machine_Enemies* aMachineE = static_cast<Machine_Enemies*>(getChildByTag(1030));
    if(aMachineE->mCanBeActivated)
        aCanActivateCount+=1;
    
    if(aCanActivateCount==0)
    {
        _mushroomTimer = 5;//Check after 5 sec again !!!
        return;
    }
    
//    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    Mushroom* mushroom = Mushroom::create(this);
    mushroom->setTag(kMushroom);
    
    //Get some position
//    CCPoint position = ccp(rand() % ((int)visibleSize.width+40),
//                           rand() % ((int)visibleSize.height+40));
//    
//    while (getMask(position.x, position.y))
//    {
//        position = ccp(rand() % ((int)visibleSize.width+40),
//                       rand() % ((int)visibleSize.height+40));
//    }
//    
//    mushroom->setPosition(position.x,position.y);
    
    //////////////////////////////////////////////////////////////////////
    //Create the possible spawn points,except last one
    
    _mushroomSpawnPositions.clear();
    for (int i = 0;i<8;i++)
    {
        if (i != _mushroomLastSpawnBlockID)
            _mushroomSpawnPositions.push_back(i);
    }
    _mushroomLastSpawnBlockID = _mushroomSpawnPositions[rand()%_mushroomSpawnPositions.size()];
    
    CCPoint aPosition = getRandomPointFromBlock(_mushroomLastSpawnBlockID);
    
    mushroom->setPosition(aPosition);
    
    //////////////////////////////////////////////////////////////////////
    
    addChild(mushroom, getSpriteOrderZ(mushroom->getPositionY()));
}

void GameScene::generateEffectSpecial(int theX,int theY, int theType)
{
    if(mMapDangerCooldown>0)
        return;
    
    Effect* effect = NULL;
    
    switch (theType)
    {
        case EFFECT_TYPE_ICE_BARRAGE:
            effect = IceBarrage::create(this);
            break;
			
        case EFFECT_TYPE_WEB:
            effect = Web::create(this);
            break;
            
        case EFFECT_TYPE_RAIN:
            effect = Rain::create(this);
            break;
            
        case EFFECT_TYPE_WIND:
            effect = Wind::create(this);
            
        default:
//            effect = IceBarrage::create(this);
            break;
    }
    
    effect->setPosition(ccp(theX,theY));
    
    this->addChild(effect,0);
    _effects->addObject(effect);
    
    //Wind starts instant !!! from one side of the map !!!
    if(theType == EFFECT_TYPE_WIND)
    {
        effect->setVisible(true);
        effect->setZOrder(1);
        
        //Create the move stuff !!!
        Wind* aWindFx = (Wind*)effect;
        
        //The new stuff
        int aIndexMoveTo = 0;
        int aClockSide = rand()%2;
        
        if(theX<_screenSize.width/2)
        {
            if(theY<_screenSize.height/2)
            {
                if(aClockSide>0)
                    aIndexMoveTo = 12;
                else
                    aIndexMoveTo = 18;
            }
            else
            {
                if(aClockSide>0)
                    aIndexMoveTo = 6;
                else
                    aIndexMoveTo = 12;
            }
        }
        else
        {
            if(theY<_screenSize.height/2)
            {
                if(aClockSide>0)
                    aIndexMoveTo = 19;
                else
                    aIndexMoveTo = 0;
            }
            else
            {
                if(aClockSide>0)
                    aIndexMoveTo = 24;
                else
                    aIndexMoveTo = 6;
            }
        }
        
        aWindFx->MoveToPos(aClockSide, 0, aIndexMoveTo);
        
        //THE OLD STUFF
        /*
         int aMoveToX = 0;
         int aCurve = 0;
         int aMoveToY = 0;
         
        if(theX<_screenSize.width/2)
        {
            if(theY<_screenSize.height/2)
            {
                aMoveToX = _screenSize.width-200;
                aMoveToY = _screenSize.height-200;
            }
            else
            {
                aMoveToX = _screenSize.width-200;
                aMoveToY = 200;
            }
        }
        else
        {
            if(theY<_screenSize.height/2)
            {
                aMoveToX = 200;
                aMoveToY = _screenSize.height-200;
            }
            else
            {
                aMoveToX = 200;
                aMoveToY = 200;
                aCurve = 1;
            }
        }
        
        aWindFx->MoveToPos(aMoveToX,aMoveToY,aCurve);
        */
    }
    else
    {
        effect->setVisible(false);
        
        IntroAnimation* introAnimation = EffectIntro::create(this, effect);
        introAnimation->setPosition(effect->getPosition());
        
        int aCaveIndex = getSpriteOrderZ(_cave->getPositionY())+1;
        
        this->addChild(introAnimation,aCaveIndex);
        _introAnimations->addObject(introAnimation);
        
        //Play fall sound
        playInGameSound("meteorite_fall");
    }
}

void GameScene::generateEffect()
{
    if (_boostNoEnemyTimer>0)
	{
        return;
	}
    
    if(mMapDangerCooldown>0)
        return;
    
	if (_effects->count() < MAX_EFFECTS)//?? MAX_DIAMONDS
	{
//		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		
//		EffectType effectType = static_cast<EffectType>(rand() % EFFECT_TYPE_COUNT);
		
        //Force for now
//        effectType = EFFECT_TYPE_WEB;
		//TODO: random
		Effect* effect = NULL;
        
        bool aSpecialPositionCheck = false;
        
//        effectType = EFFECT_TYPE_WEB;
        int aRandomValue = 0;
        if(mRainActive)
        {
            //Only rain can be here
            if(User::getInstance()->_tutorial_small_8==0)
                aSpecialPositionCheck = true;
            
            effect = Rain::create(this);
        }
        else if(mTornadoActive)
        {
            //Special case here !!!
            
            _windSpawnPositions.clear();
            for (int i = 0; i<4; i++)
            {
                if (i != _windLastSpawnBlockID)
                    _windSpawnPositions.push_back(i);
            }
            _windLastSpawnBlockID = _windSpawnPositions[rand()%_windSpawnPositions.size()];
//            _windLastSpawnBlockID+=1;
//            _windLastSpawnBlockID = 3;
            
            //use this as spawn id !!!
//            _effectLastSpawnBlockID
            int aWindStartX = 0;
            int aWindStartY = 0;
            
//            CCLog("CUrrent wind id %i",_windLastSpawnBlockID);
            
            if(_windLastSpawnBlockID==0)
            {
                aWindStartX = -100;
                aWindStartY = _screenSize.height/2-200;
            }
            else if(_windLastSpawnBlockID==1)
            {
                aWindStartX = _screenSize.width+100;
                aWindStartY = _screenSize.height/2-200;
            }
            else if(_windLastSpawnBlockID==2)
            {
                aWindStartX = _screenSize.width/2+400;
                aWindStartY = _screenSize.height;
            }
            else if(_windLastSpawnBlockID==3)
            {
                aWindStartX = -100;
                aWindStartY = _screenSize.height/2+200;
            }
            
            if(_windLastSpawnBlockID==3)
                _effectLastSpawnBlockID = -1;
            
            
            generateEffectSpecial(aWindStartX,aWindStartY,EFFECT_TYPE_WIND);
            
//            effect = Wind::create(this);
            return;
        }
        else
        {
            if(User::getInstance()->_tutorial_small_9==0)
                aSpecialPositionCheck = true;
            
            if(mSnowActive)
            {
                effect = IceBarrage::create(this);
            }
            else
            {
                effect = Web::create(this);
            }
            
//            aRandomValue = rand()%2;
//            if(aRandomValue==0)
//                effect = IceBarrage::create(this);
//            else
//                effect = Web::create(this);
            
            //Play fall sound
            playInGameSound("meteorite_fall");
            
//            switch (effectType)
//            {
//                case EFFECT_TYPE_ICE_BARRAGE:
//                    effect = IceBarrage::create(this);
//                    break;
//                    
//                case EFFECT_TYPE_WEB:
//                    effect = Web::create(this);
//                    break;
//                    
//                default:
//                    effect = IceBarrage::create(this);
//                    break;
//            }
        }
        
        //Lets update the meteorite fall!!!
        
		
//		CCPoint position = ccp(rand() % (int)visibleSize.width,
//							   rand() % (int)visibleSize.height);
//		
        //This can slow down game!!!
//		while (getMask(position.x, position.y))
//		{
//			position = ccp(rand() % (int)visibleSize.width,
//						   rand() % (int)visibleSize.height);
//		}
//        
//		effect->setPosition(
//							 position.x,
//							 position.y);
        
        //////////////////////////////////////////////////////////////////////
        //Create the possible spawn points,except last one
        
        _effectSpawnPositions.clear();
        for (int i = 0; i<8; i++)
        {
            if (i != _effectLastSpawnBlockID)
                _effectSpawnPositions.push_back(i);
        }
        _effectLastSpawnBlockID = _effectSpawnPositions[rand()%_effectSpawnPositions.size()];
        
        CCPoint aPosition = getRandomPointFromBlock(_effectLastSpawnBlockID);
        
        //Special check for 1st times !!! dont fall on dwarf etc
        if(aSpecialPositionCheck)
        {
            //Check where is safer to land if dwarf is around too clouse !!!
            CCPoint aPosition_Recheck;
            int aReSpawnIndex = 0;
            Dwarf* dwarf;
            bool aTouchDwarf = false;
            
            for(int aRechekIndex = 0;aRechekIndex<_effectSpawnPositions.size();aRechekIndex++)
            {
                //Recheck all positions for all dwarfs !!!
                aPosition_Recheck = getRandomPointFromBlock(aReSpawnIndex);
                
                aTouchDwarf = false;
                
                for (int dwarfIndex = _dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
                {
                    dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(dwarfIndex));
                    if(ccpDistanceSQ(dwarf->getPosition(), aPosition_Recheck)<=4000)
                    {
                        aTouchDwarf = true;
                        break;
                    }
                }
                
                if(aTouchDwarf == false)
                {
                    _effectLastSpawnBlockID = aRechekIndex;
                    aPosition = getRandomPointFromBlock(_effectLastSpawnBlockID);
                    break;
                }
            }
        }
        
        effect->setPosition(aPosition);
        
        //////////////////////////////////////////////////////////////////////
        
		this->addChild(effect,0);
		_effects->addObject(effect);
		effect->setVisible(false);
		
		IntroAnimation* introAnimation = EffectIntro::create(this, effect);
		introAnimation->setPosition(effect->getPosition());
        
        int aCaveIndex = 0;
//        int aCaveIndex = getSpriteOrderZ(_cave->getPositionY())+1;
		
		this->addChild(introAnimation,aCaveIndex);
		_introAnimations->addObject(introAnimation);
	}
}

CCPoint GameScene::getBlockCenterPos(int theID)
{
    CCPoint theReturn;
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    int aBlockX = visibleSize.width/8;
    int aBlockY = visibleSize.height/5;
    
    if (theID == 0)
    {
        //The up left side
        theReturn = ccp(aBlockX,aBlockY);
    }
    else if (theID == 1)
    {
        //        theReturn = ccp(200,100);
        theReturn = ccp(aBlockX*3,aBlockY);
    }
    else if (theID == 2)
    {
        //        theReturn = ccp(300,100);
        theReturn = ccp(aBlockX*5,aBlockY);
    }
    else if (theID == 3)
    {
        //        theReturn = ccp(300,200);
        theReturn = ccp(aBlockX*5,aBlockY*2);
    }
    else if (theID == 4)
    {
        //        theReturn = ccp(300,300);
        theReturn = ccp(aBlockX*5,aBlockY*3);
    }
    else if (theID == 5)
    {
        //        theReturn = ccp(200,300);
        theReturn = ccp(aBlockX*3,aBlockY*3);
    }
    else if (theID == 6)
    {
        //        theReturn = ccp(100,300);
        theReturn = ccp(aBlockX,aBlockY*3);
    }
    else
    {
        //        theReturn = ccp(100,200);
        theReturn = ccp(aBlockX,aBlockY*2);
    }
    
    //    CCLog("theReturn X:%f Y:%f",theReturn.x,theReturn.y);
    
    return theReturn;

}

//One time stuff !!!
void GameScene::CreatePossibleObjectSpawns()
{
    //The screen size
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //The start
    int aBlockX = visibleSize.width/8;
    int aBlockY = visibleSize.height/5;
    
    int aStartX = 0;
    int aStartY = 0;
    
    int aStepsX = aBlockX/5;
    int aStepsY = aBlockY/5;
    
    //The 1st block
    
    aStartX = aBlockX;
    aStartY = aBlockY/2-aStepsY;
    
    int aCurrentX = 0;
    int aCurrentY = 0;
    
    for(int x = 0;x<aStepsX/2-1;x++)
    {
        aCurrentX = aStartX+aStepsX*x;
        
        for(int y=0;y<aStepsY/4+1;y++)
        {
            aCurrentY = aStartY+aStepsY*y;
            
            if(getSpawnObjectMask(aCurrentX,aCurrentY))
            {
                //For debug create dot
//                CCSprite* aDebug1 = CCSprite::create("small_dot_red.png");
//                aDebug1->setPosition(ccp(aCurrentX, aCurrentY));
//                aDebug1->setOpacity(128);
//                addChild(aDebug1);
                
                _BlockPositions_1.push_back((SpawnPoint){aCurrentX, aCurrentY});
            }
        }
    }
    
    //the 2nd block
    aStartX = aBlockX*3+aStepsX;
    aStartY = aBlockY/2-aStepsY;
    
    aCurrentX = 0;
    aCurrentY = 0;
    
    for(int x = 0;x<aStepsX/2-2;x++)
    {
        aCurrentX = aStartX+aStepsX*x;
        
        for(int y=0;y<aStepsY/4+1;y++)
        {
            aCurrentY = aStartY+aStepsY*y;
            
            if(getSpawnObjectMask(aCurrentX,aCurrentY))
            {
                //For debug create dot
//                CCSprite* aDebug1 = CCSprite::create("small_dot_red.png");
//                aDebug1->setPosition(ccp(aCurrentX, aCurrentY));
//                aDebug1->setOpacity(64);
//                addChild(aDebug1);
                
                _BlockPositions_2.push_back((SpawnPoint){aCurrentX, aCurrentY});
            }
        }
    }
    
    //--------------------------------------------------
    //the 3rd block
    aStartX = aBlockX*5+aStepsX;
    aStartY = aBlockY/2-aStepsY;
    
    aCurrentX = 0;
    aCurrentY = 0;
    
    for(int x = 0;x<aStepsX/2-1;x++)
    {
        aCurrentX = aStartX+aStepsX*x;
        
        for(int y=0;y<aStepsY/4+1;y++)
        {
            aCurrentY = aStartY+aStepsY*y;
            
            if(getSpawnObjectMask(aCurrentX,aCurrentY))
            {
                //For debug create dot
//                CCSprite* aDebug1 = CCSprite::create("small_dot_red.png");
//                aDebug1->setPosition(ccp(aCurrentX, aCurrentY));
//                aDebug1->setOpacity(128);
//                addChild(aDebug1);
                
                _BlockPositions_3.push_back((SpawnPoint){aCurrentX, aCurrentY});
            }
        }
    }
    
    //--------------------------------------------------
    //the 4th block
    aStartX = aBlockX*5+aStepsX;
    aStartY = aBlockY*3/2+aStepsY*2;
    
    aCurrentX = 0;
    aCurrentY = 0;
    
    for(int x = 0;x<aStepsX/2-1;x++)
    {
        aCurrentX = aStartX+aStepsX*x;
        
        for(int y=0;y<aStepsY/4-1;y++)
        {
            aCurrentY = aStartY+aStepsY*y;
            
            if(getSpawnObjectMask(aCurrentX,aCurrentY))
            {
                //For debug create dot
//                CCSprite* aDebug1 = CCSprite::create("small_dot_red.png");
//                aDebug1->setPosition(ccp(aCurrentX, aCurrentY));
//                aDebug1->setOpacity(128);
//                addChild(aDebug1);
                
                _BlockPositions_4.push_back((SpawnPoint){aCurrentX, aCurrentY});
            }
        }
    }
    
    //--------------------------------------------------
    //the 5 block
//    theReturn = ccp(aX+aBlockX*3,aY+aBlockY*3);
    
    aStartX = aBlockX*5+aStepsX;
    aStartY = aBlockY*6/2+aStepsY/2;
    
    aCurrentX = 0;
    aCurrentY = 0;
    
    for(int x = 0;x<aStepsX/2-1;x++)
    {
        aCurrentX = aStartX+aStepsX*x;
        
        for(int y=0;y<aStepsY/4-1;y++)
        {
            aCurrentY = aStartY+aStepsY*y;
            
            if(getSpawnObjectMask(aCurrentX,aCurrentY))
            {
                //For debug create dot
//                CCSprite* aDebug1 = CCSprite::create("small_dot_red.png");
//                aDebug1->setPosition(ccp(aCurrentX, aCurrentY));
//                aDebug1->setOpacity(128);
//                addChild(aDebug1);
                
                _BlockPositions_5.push_back((SpawnPoint){aCurrentX, aCurrentY});
            }
        }
    }
    
    //--------------------------------------------------
    //the 6 block
    //    theReturn = ccp(aX+aBlockX*3,aY+aBlockY*3);
    
    aStartX = aBlockX*3+aStepsX;
    aStartY = aBlockY*6/2+aStepsY/2;
    
    aCurrentX = 0;
    aCurrentY = 0;
    
    for(int x = 0;x<aStepsX/2-2;x++)
    {
        aCurrentX = aStartX+aStepsX*x;
        
        for(int y=0;y<aStepsY/4-1;y++)
        {
            aCurrentY = aStartY+aStepsY*y;
            
            if(getSpawnObjectMask(aCurrentX,aCurrentY))
            {
                //For debug create dot
//                CCSprite* aDebug1 = CCSprite::create("small_dot_red.png");
//                aDebug1->setPosition(ccp(aCurrentX, aCurrentY));
//                aDebug1->setOpacity(128);
//                addChild(aDebug1);
                
                _BlockPositions_6.push_back((SpawnPoint){aCurrentX, aCurrentY});
            }
        }
    }
    
    //--------------------------------------------------
    //the 7 block
    //    theReturn = ccp(aX+aBlockX*3,aY+aBlockY*3);
    
    aStartX = aBlockX;
    aStartY = aBlockY*6/2+aStepsY/2;
    
    aCurrentX = 0;
    aCurrentY = 0;
    
    for(int x = 0;x<aStepsX/2-1;x++)
    {
        aCurrentX = aStartX+aStepsX*x;
        
        for(int y=0;y<aStepsY/4-1;y++)
        {
            aCurrentY = aStartY+aStepsY*y;
            
            if(getSpawnObjectMask(aCurrentX,aCurrentY))
            {
                //For debug create dot
//                CCSprite* aDebug1 = CCSprite::create("small_dot_red.png");
//                aDebug1->setPosition(ccp(aCurrentX, aCurrentY));
//                aDebug1->setOpacity(128);
//                addChild(aDebug1);
                
                _BlockPositions_7.push_back((SpawnPoint){aCurrentX, aCurrentY});
            }
        }
    }
    
    //--------------------------------------------------
    //the 8 block
    //    theReturn = ccp(aX+aBlockX*3,aY+aBlockY*3);
    
    aStartX = aBlockX;
    aStartY = aBlockY*3/2+aStepsY*2;
    
    aCurrentX = 0;
    aCurrentY = 0;
    
    for(int x = 0;x<aStepsX/2-1;x++)
    {
        aCurrentX = aStartX+aStepsX*x;
        
        for(int y=0;y<aStepsY/4-1;y++)
        {
            aCurrentY = aStartY+aStepsY*y;
            
            if(getSpawnObjectMask(aCurrentX,aCurrentY))
            {
                //For debug create dot
//                CCSprite* aDebug1 = CCSprite::create("small_dot_red.png");
//                aDebug1->setPosition(ccp(aCurrentX, aCurrentY));
//                aDebug1->setOpacity(128);
//                addChild(aDebug1);
                
                _BlockPositions_8.push_back((SpawnPoint){aCurrentX, aCurrentY});
            }
        }
    }
    
    
    CCLOG("wtf is done");
//    getSpawnObjectMask(1,1);
    
//    _BlockPositions_1.push_back((SpawnPoint){1, 1});
    
}

//MapSpawnObjectMask.png
CCPoint GameScene::getRandomPointFromBlock(int theID)
{
    CCPoint theReturn;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    int aBlockX = visibleSize.width/8;
    int aBlockY = visibleSize.height/5;
    
//    int aX = rand()%(aBlockX*2);
//    int aY = rand()%aBlockY;
    
    //Get spawn point from generated stuff !!!
    SpawnPoint aValues;
    
//    CCLog("theID %i",theID);
//    CCLog("theReturn start random X:%i Y:%i",aX,aY);
    
    if (theID == 0)
    {
        //The up left side
        aValues = _BlockPositions_1[rand() % _BlockPositions_1.size()];
//        theReturn = ccp(aX+aBlockX,aY+aBlockY);
    }
    else if (theID == 1)
    {
//        theReturn = ccp(200,100);
        aValues = _BlockPositions_2[rand() % _BlockPositions_2.size()];
//        theReturn = ccp(aX+aBlockX*3,aY+aBlockY);
    }
    else if (theID == 2)
    {
//        theReturn = ccp(300,100);
        aValues = _BlockPositions_3[rand() % _BlockPositions_3.size()];
//        theReturn = ccp(aX+aBlockX*5,aY+aBlockY);
    }
    else if (theID == 3)
    {
//        theReturn = ccp(300,200);
        aValues = _BlockPositions_4[rand() % _BlockPositions_4.size()];
//        theReturn = ccp(aX+aBlockX*5,aY+aBlockY*2);
    }
    else if (theID == 4)
    {
//        theReturn = ccp(300,300);
        aValues = _BlockPositions_5[rand() % _BlockPositions_5.size()];
//        theReturn = ccp(aX+aBlockX*5,aY+aBlockY*3);
    }
    else if (theID == 5)
    {
//        theReturn = ccp(200,300);
        aValues = _BlockPositions_6[rand() % _BlockPositions_6.size()];
//        theReturn = ccp(aX+aBlockX*3,aY+aBlockY*3);
    }
    else if (theID == 6)
    {
//        theReturn = ccp(100,300);
        aValues = _BlockPositions_7[rand() % _BlockPositions_7.size()];
//        theReturn = ccp(aX+aBlockX,aY+aBlockY*3);
    }
    else
    {
//        theReturn = ccp(100,200);
        aValues = _BlockPositions_8[rand() % _BlockPositions_8.size()];
//        theReturn = ccp(aX+aBlockX,aY+aBlockY*2);
    }
    
    //now get the real random and check for mask !!!
    theReturn = ccp(aValues.x,aValues.y);
    
//    CCLog("theReturn X:%f Y:%f",theReturn.x,theReturn.y);
    
    return theReturn;
}

void GameScene::showRemovableEffects()
{
    mZipZapActive = true;
    
    for (int effectIndex = _trolls->count() - 1; effectIndex >= 0; --effectIndex)
    {
        Troll* trollIn = static_cast<Troll*>(_trolls->objectAtIndex(effectIndex));
        if (trollIn != NULL)
        {
            if(trollIn->getChildByTag(787)!=NULL){
                trollIn->removeChildByTag(787);
            }
            
            //Create new target sprite
            CCSprite* aSprite = CCSprite::create("Troll_Target.png");
            aSprite->setTag(787);
            aSprite->setPosition(ccp(0,60));
            trollIn->addChild(aSprite);
            
        }
    }
}

void GameScene::removeRemovableEffects()
{
    mZipZapActive = false;
    
    for (int effectIndex = _trolls->count() - 1; effectIndex >= 0; --effectIndex)
    {
        Troll* trollIn = static_cast<Troll*>(_trolls->objectAtIndex(effectIndex));
        if (trollIn != NULL)
        {
            if(trollIn->getChildByTag(787)!=NULL){
                trollIn->removeChildByTag(787);
            }
        }
    }
}

//For booster to remove all effects from map !!!
void GameScene::removeAllEffects(bool theMachine)
{
    //Clear the intro too !!!
    for(int introIndex = _introAnimations->count()-1; introIndex>=0; --introIndex)
    {
        IntroAnimation* intro = static_cast<IntroAnimation*>(_introAnimations->objectAtIndex(introIndex));
        if (intro->getType() == INTRO_EFFECT)
        {
            removeChild(intro);
            _introAnimations->removeObject(intro);
        }
    }
    
	for (int effectIndex = _effects->count() - 1; effectIndex >= 0; --effectIndex)
	{
		Effect* effect = static_cast<Effect*>(_effects->objectAtIndex(effectIndex));
        
        //Create lightning here !!!
//        CreateBlitz(effect->getPositionX(),effect->getPositionY()-60);
        
        if(theMachine)
        {
            //When blitz finishes - remove it
            float aRandomDelay = (rand()%6)*0.1+0.2;
            CCDelayTime* aBeforeBlitz = CCDelayTime::create(aRandomDelay);
            CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnEffectDelayFinish));
            CCSequence* aSeq1 = CCSequence::create(aBeforeBlitz,func,NULL);
            effect->runAction(aSeq1);
        }
        else
        {
            this->removeChild(effect);
        }
        
        _effects->removeObject(effect);
    }
    
    for(int introIndex = _tornadoActive->count()-1; introIndex>=0; --introIndex)
    {
        Wind* intro = static_cast<Wind*>(_tornadoActive->objectAtIndex(introIndex));
        
        intro->ClearAll();
        
        this->removeChild(intro);
        _tornadoActive->removeObject(intro);
    }
}



void GameScene::OnEffectDelayFinish(CCNode* sender)
{
    Effect* aTroll = static_cast<Effect*>(sender);
    
    CreateBlitz(aTroll->getPositionX(),aTroll->getPositionY()-80,NULL,aTroll);
}

void GameScene::OnTrollDelayFinish(CCNode* sender)
{
    Troll* aTroll = static_cast<Troll*>(sender);
    
    CreateBlitz(aTroll->getPositionX(),aTroll->getPositionY()-80,aTroll);
}

void GameScene::CreateBlitz_Goblin(int theX,int theY,Goblin* theEnemy)
{
    SpriteAnimation* aBlitz = SpriteAnimation::create("powerup/Machine_Enemies/elektro_strike.plist",false);
    aBlitz->retain();
    aBlitz->setAnchorPoint(ccp(0.5,0));
    aBlitz->setPosition(ccp(theX,theY));
    addChild(aBlitz,kPoints_Z_Order);
    
    CCDelayTime* aDelay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCSequence* aSeq1 = CCSequence::create(aDelay,func,NULL);
    aBlitz->runAction(aSeq1);
    
    if(theEnemy!=NULL)
    {
        CCDelayTime* aTrollDelay = CCDelayTime::create(0.4f);
        CCCallFuncN* funcTroll = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
        CCSequence* aSeq1 = CCSequence::create(aTrollDelay,funcTroll,NULL);
        theEnemy->runAction(aSeq1);
    }
    
    playInGameSound("ET_Zap");
}



void GameScene::CreateBlitz_Spider(int theX,int theY,Spider* theEnemy)
{
    SpriteAnimation* aBlitz = SpriteAnimation::create("powerup/Machine_Enemies/elektro_strike.plist",false);
    aBlitz->retain();
    aBlitz->setAnchorPoint(ccp(0.5,0));
    aBlitz->setPosition(ccp(theX,theY));
    addChild(aBlitz,kPoints_Z_Order);
    
    CCDelayTime* aDelay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCSequence* aSeq1 = CCSequence::create(aDelay,func,NULL);
    aBlitz->runAction(aSeq1);
    
    if(theEnemy!=NULL)
    {
        CCDelayTime* aTrollDelay = CCDelayTime::create(0.4f);
        CCCallFuncN* funcTroll = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
        CCSequence* aSeq1 = CCSequence::create(aTrollDelay,funcTroll,NULL);
        theEnemy->runAction(aSeq1);
    }
    
    playInGameSound("ET_Zap");
}

void GameScene::CreateBlitz_Hidra(int theX,int theY,Hidra* theEnemy)
{
    SpriteAnimation* aBlitz = SpriteAnimation::create("powerup/Machine_Enemies/elektro_strike.plist",false);
    aBlitz->retain();
    aBlitz->setAnchorPoint(ccp(0.5,0));
    aBlitz->setPosition(ccp(theX,theY));
    addChild(aBlitz,kPoints_Z_Order);
    
    CCDelayTime* aDelay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCSequence* aSeq1 = CCSequence::create(aDelay,func,NULL);
    aBlitz->runAction(aSeq1);
    
    if(theEnemy!=NULL)
    {
        CCDelayTime* aTrollDelay = CCDelayTime::create(0.4f);
        CCCallFuncN* funcTroll = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
        CCSequence* aSeq1 = CCSequence::create(aTrollDelay,funcTroll,NULL);
        theEnemy->runAction(aSeq1);
    }
    
    playInGameSound("ET_Zap");
}

void GameScene::CreateBlitz(int theX,int theY,Troll* theEnemy,Effect* theEffect)
{
    SpriteAnimation* aBlitz = SpriteAnimation::create("powerup/Machine_Enemies/elektro_strike.plist",false);
    aBlitz->retain();
    aBlitz->setAnchorPoint(ccp(0.5,0));
    aBlitz->setPosition(ccp(theX,theY));
    addChild(aBlitz,kPoints_Z_Order);
    
    CCDelayTime* aDelay = CCDelayTime::create(0.5f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCSequence* aSeq1 = CCSequence::create(aDelay,func,NULL);
    aBlitz->runAction(aSeq1);
    
    if(theEnemy!=NULL)
    {
        CCDelayTime* aTrollDelay = CCDelayTime::create(0.4f);
        CCCallFuncN* funcTroll = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
        CCSequence* aSeq1 = CCSequence::create(aTrollDelay,funcTroll,NULL);
        theEnemy->runAction(aSeq1);
        
        if(theEnemy->mDrawLineID!=-1){
            if(getChildByTag(theEnemy->mDrawLineID)!=NULL){
                removeChildByTag(theEnemy->mDrawLineID);
            }
            theEnemy->mDrawLineID = -1;
        }
    }
    else if(theEffect!=NULL)
    {
        CCDelayTime* aEffectDelay = CCDelayTime::create(0.4f);
        CCCallFuncN* funcEffect = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
        CCSequence* aSeq1 = CCSequence::create(aEffectDelay,funcEffect,NULL);
        theEffect->runAction(aSeq1);
    }
    
//    playInGameSound("machine_tesla_blitz");
    playInGameSound("ET_Zap");
}

// called when meteorit expires?
void GameScene::removeEffect(Effect* effect)
{
	_effects->removeObject(effect);
    
    //Fade it out and remove !!!
//    CCFadeOut* aFade = CCFadeOut::create(1.0f);
    CCDelayTime* aWait = CCDelayTime::create(1.0f);
    CCCallFuncN* funcEffect = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCSequence* aSeq1 = CCSequence::create(aWait,funcEffect,NULL);
    effect->runAction(aSeq1);
//    this->removeChild(effect);
    
    // meteorit has been avoided
    _stats.avoidedMetereoits++;
}

void GameScene::removeNode(CCNode* sender)
{
    this->removeChild(sender, true);
}

void GameScene::removeBatchNode(CCNode* sender)
{
    _batchNode->removeChild(sender, true);
}


void GameScene::generateDiamond()
{
	if (_diamonds->count() < MAX_DIAMONDS)
	{
//		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		
		Diamond* diamond = Diamond::create(this);
		
//		CCPoint position = ccp(rand() % (int)visibleSize.width,
//							   rand() % (int)visibleSize.height);
//		
//		while (getMask(position.x, position.y))
//		{
//			position = ccp(rand() % (int)visibleSize.width,
//						   rand() % (int)visibleSize.height);
//		}
//		
//		diamond->setPosition(
//							position.x,
//							position.y);
        
        //////////////////////////////////////////////////////////////////////
        //Create the possible spawn points,except last one
        
        _diamondSpawnPositions.clear();
        for (int i = 0; i<8; i++)
        {
            if (i != _diamondLastSpawnBlockID)
                _diamondSpawnPositions.push_back(i);
        }
        _diamondLastSpawnBlockID = _diamondSpawnPositions[rand()%_diamondSpawnPositions.size()];
        
        CCPoint aPosition = getRandomPointFromBlock(_diamondLastSpawnBlockID);
        
        diamond->setPosition(aPosition);
        
        //////////////////////////////////////////////////////////////////////
		
		addChild(diamond, getSpriteOrderZ(diamond->getPositionY()));
		_diamonds->addObject(diamond);
		diamond->setVisible(false);
		
		IntroAnimation* introAnimation = DiamondIntro::create(this, diamond);
		introAnimation->setPosition(diamond->getPosition());
		
		addChild(introAnimation, getSpriteOrderZ(introAnimation->getPositionY()));
		_introAnimations->addObject(introAnimation);
        
        playInGameSound("diamond_intro");
	}
}

bool GameScene::isInTrollSensor(Dwarf* theDwarf,Troll* theTroll)
{
    //Disabled for now !!!
    return false;
    
    //Is he already catching !!!
    if(theTroll->mCatchingDwarf){
        return false;
    }
    
    if(theTroll->mCatchRadar->isVisible()==false){
        return false;
    }
    
//    _trollSensor
    //1st check if is in the zone troll !!!
    bool result = false;
    
    float theDistance = ccpDistanceSQ(theDwarf->getPosition(), theTroll->getPosition());
//    CCLog("theDistance: %f",theDistance);
    
//    CCLog("Troll Angle:,%f",theTroll->GetAngle());
    
//    CCLog("theDwarf->getPositionY(): %f",theDwarf->getPositionY());
//    CCLog("theTroll->getPositionY(): %f",theTroll->getPositionY());
    
    if (theDistance <= 20000)
    {
        //We can check futher stuff !!! if it's not in collision
        
        unsigned char* data = _trollSensor->getData();
        int cavePosX = -100;
        int cavePosY = -100;
        
//        if(_angle <= 2){
//            setAngle(4.7);
//            //                    _angle = 4.7;
//        }
//        else{
//            //                    _angle = 1.6;
//            setAngle(1.6);
//        }
        
//        CCLog("Troll Angle:,%f",theTroll->GetAngle());
//        CCLog("theDwarf->getPositionX(): %f",theDwarf->getPositionX());
//        CCLog("theTroll->getPositionX(): %f",theTroll->getPositionX());
//
//        CCLog("theDwarf->getPositionY(): %f",theDwarf->getPositionY());
//        CCLog("theTroll->getPositionY(): %f",theTroll->getPositionY());
        
        if(mNewSplitCaves)
        {
            if(theTroll->GetAngle() < 2){
                if(theDwarf->getPositionY()<theTroll->getPositionY()){
                    return false;
                }
                else{
//                    cavePosX = _trollSensor->getWidth()-(theTroll->getPositionX()-theDwarf->getPositionX());
                    if(theDwarf->getPositionX()>theTroll->getPositionX()){
                        cavePosX = _trollSensor->getWidth()-(theDwarf->getPositionX()-theTroll->getPositionX());
                    }
                    else{
                        cavePosX = _trollSensor->getWidth()-(theTroll->getPositionX()-theDwarf->getPositionX());
                    }
                    cavePosY = (_trollSensor->getHeight()/2) - (theDwarf->getPositionY()-theTroll->getPositionY());
                }
            }
            else{
                if(theDwarf->getPositionY()>theTroll->getPositionY()){
                    return false;
                }
                else{
                    if(theDwarf->getPositionX()>theTroll->getPositionX()){
                        cavePosX = _trollSensor->getWidth()-(theDwarf->getPositionX()-theTroll->getPositionX());
                    }
                    else{
                        cavePosX = _trollSensor->getWidth()-(theTroll->getPositionX()-theDwarf->getPositionX());
                    }
//                    cavePosX = _trollSensor->getWidth()-(theDwarf->getPositionX()-theTroll->getPositionX());
                    cavePosY = (_trollSensor->getHeight()/2) - (theTroll->getPositionY()-theDwarf->getPositionY());
                }
            }
        }
        else
        {
            if(theTroll->GetAngle() >2){
                if(theDwarf->getPositionX()>theTroll->getPositionX()){
                    //Nothing to do here !!!
                    return false;
                }
                else{
                    cavePosX = _trollSensor->getWidth()-(theTroll->getPositionX()-theDwarf->getPositionX());
                    cavePosY = (_trollSensor->getHeight()/2) - (theTroll->getPositionY()-theDwarf->getPositionY());
                }
            }
            else{
                if(theDwarf->getPositionX()<theTroll->getPositionX()){
                    return false;
                }
                else{
                    cavePosX = _trollSensor->getWidth()-(theDwarf->getPositionX()-theTroll->getPositionX());
                    cavePosY = (_trollSensor->getHeight()/2) - (theTroll->getPositionY()-theDwarf->getPositionY());
                }
            }
        }
        
//        CCLog("cavePosX: %d",cavePosX);
//        CCLog("cavePosY: %d",cavePosY);
        
        result = (data[4 * (cavePosX + cavePosY * _trollSensor->getWidth()) + 3] != 0);
        
//        CCLog("Did we catch %i",result);
    }
    
    
    return result;
}

bool GameScene::getMask(int32_t posX, int32_t posY)
{
	bool result = true;
    
    posY = designResolutionSize.height - posY - 1; //invert Y axis
    
    if (posX >= 0 && posX < _mask->getWidth() &&
        posY >= 0 && posY < _mask->getHeight())
    {
        unsigned char* data = _mask->getData();
        
        //get alpha channel
        result = (data[4 * (posX + posY * _mask->getWidth()) + 3] != 0);
        
//        CCLOG("Result for mask %i",result);
    }
    
    return result;
    
    /*
    if(mNewMapRandomEnabled){
        posY = designResolutionSize.height - posY - 1; //invert Y axis
        
        if (posX >= 0 && posX < _mask->getWidth() &&
            posY >= 0 && posY < _mask->getHeight())
        {
            unsigned char* data = _mask->getData();
            
            //get alpha channel
            result = (data[4 * (posX + posY * _mask->getWidth()) + 3] != 0);
        }
        
        //Check the cave for now only !!!
        int cavePosX = posX - mCaveFinalCords.x + _caveMask->getWidth() / 2;
        int cavePosY = posY - (designResolutionSize.height-mCaveFinalCords.y) + _caveMask->getHeight() / 2;
        
        if (!result &&
            cavePosX >= 0 && cavePosX < _caveMask->getWidth() &&
            cavePosY >= 0 && cavePosY < _caveMask->getHeight())
        {
            unsigned char* data = _caveMask->getData();
            
            result = (data[4 * (cavePosX + cavePosY * _caveMask->getWidth()) + 3] != 0);
        }
        
        return result;
    }
    */
	
	posY = designResolutionSize.height - posY - 1; //invert Y axis
	
	if (posX >= 0 && posX < _mask->getWidth() &&
		posY >= 0 && posY < _mask->getHeight())
	{
		unsigned char* data = _mask->getData();
		
		//get alpha channel
		result = (data[4 * (posX + posY * _mask->getWidth()) + 3] != 0);
	}
	
    //The new stuff !!!
    if(mNewSplitCaves)
    {
        //No cave collision
    }
    else
    {
        if(!mSplitCavesMode)
        {
            if(User::getInstance()->mSpecial_12_Mission)
            {
                //Do nothing
            }
            else
            {
                int cavePosX = posX - designResolutionSize.width / 2 + _caveMask->getWidth() / 2;
                //        int cavePosY = posY - designResolutionSize.height / 2 + _caveMask->getHeight() / 2 + 20;
                int cavePosY = posY - designResolutionSize.height / 2 + _caveMask->getHeight() / 2;
                
                if (!result &&
                    cavePosX >= 0 && cavePosX < _caveMask->getWidth() &&
                    cavePosY >= 0 && cavePosY < _caveMask->getHeight())
                {
                    unsigned char* data = _caveMask->getData();
                    
                    result = (data[4 * (cavePosX + cavePosY * _caveMask->getWidth()) + 3] != 0);
                }
            }
        }
        else
        {
            int cavePosX = posX - designResolutionSize.width + _caveMaskFat->getWidth() / 2;
            int cavePosY = posY - 1 + _caveMaskFat->getHeight() / 2 + 20;
            
            if (!result &&
                cavePosX >= 0 && cavePosX < _caveMaskFat->getWidth() &&
                cavePosY >= 0 && cavePosY < _caveMaskFat->getHeight())
            {
                unsigned char* data = _caveMaskFat->getData();
                
                result = (data[4 * (cavePosX + cavePosY * _caveMaskFat->getWidth()) + 3] != 0);
            }
            
            cavePosX = posX - 1 + _caveMaskTall->getWidth() / 2;
            cavePosY = posY - 1 + _caveMaskTall->getHeight() / 2 + 20;
            
            if (!result &&
                cavePosX >= 0 && cavePosX < _caveMaskTall->getWidth() &&
                cavePosY >= 0 && cavePosY < _caveMaskTall->getHeight())
            {
                unsigned char* data = _caveMaskTall->getData();
                
                result = (data[4 * (cavePosX + cavePosY * _caveMaskTall->getWidth()) + 3] != 0);
            }
        }
    }
    
	return result;
}

bool GameScene::getSpawnObjectMask(int32_t posX, int32_t posY)
{
    bool result = false;
    
    posY = designResolutionSize.height - posY - 1; //invert Y axis
	
	if (posX >= 0 && posX < _objectMask->getWidth() &&
		posY >= 0 && posY < _objectMask->getHeight())
	{
		unsigned char* data = _objectMask->getData();
		
		//get alpha channel
		result = (data[4 * (posX + posY * _objectMask->getWidth()) + 3] != 0);
	}
    
    return result;
}

bool GameScene::getTutorialMask(int32_t posX, int32_t posY)
{
	bool result = false;
	
	posY = designResolutionSize.height - posY - 1; //invert Y axis
	
    if(mTutorialStep == 5)
    {
        int cavePosX = posX - designResolutionSize.width / 2 + _tutMask_3->getWidth() / 2 - 180;
        int cavePosY = posY - designResolutionSize.height / 2 + _tutMask_3->getHeight() / 2 - 100;
        
        if (!result &&
            cavePosX >= 0 && cavePosX < _tutMask_3->getWidth() &&
            cavePosY >= 0 && cavePosY < _tutMask_3->getHeight())
        {
//            CCLog("in the mask !!! 5");
            unsigned char* data = _tutMask_3->getData();
            
            result = (data[4 * (cavePosX + cavePosY * _tutMask_3->getWidth()) + 3] != 0);
        }
    }
    else if(mTutorialStep == 6)
    {
        int cavePosX = posX - designResolutionSize.width / 2 + _tutMask_1->getWidth() / 2 + 86;
        int cavePosY = posY - designResolutionSize.height / 2 + _tutMask_1->getHeight() / 2 - 160;
        
        if (!result &&
            cavePosX >= 0 && cavePosX < _tutMask_1->getWidth() &&
            cavePosY >= 0 && cavePosY < _tutMask_1->getHeight())
        {
            unsigned char* data = _tutMask_1->getData();
//            CCLog("in the box");
            
            result = (data[4 * (cavePosX + cavePosY * _tutMask_1->getWidth()) + 3] != 0);
        }
    }
    else if(mTutorialStep == 12)
    {
        //Check the small crystal !!!
        
        int cavePosX = posX - designResolutionSize.width / 2 + _tutMask_1->getWidth() / 2 + 120;
        int cavePosY = posY - designResolutionSize.height / 2 + _tutMask_1->getHeight() / 2 + 120;
        
        if (!result &&
            cavePosX >= 0 && cavePosX < _tutMask_1->getWidth() &&
            cavePosY >= 0 && cavePosY < _tutMask_1->getHeight())
        {
            unsigned char* data = _tutMask_1->getData();
            
            result = (data[4 * (cavePosX + cavePosY * _tutMask_1->getWidth()) + 3] != 0);
        }
    }
    else if(mTutorialStep == 14)
    {
        int cavePosX = posX - designResolutionSize.width / 2 + (_tutMask_1->getWidth()-20) / 2 + 70;
        int cavePosY = posY - designResolutionSize.height / 2 + (_tutMask_1->getHeight()-20) / 2 - 120;
        
        if (!result &&
            cavePosX >= 0 && cavePosX < _tutMask_1->getWidth()-20 &&
            cavePosY >= 0 && cavePosY < _tutMask_1->getHeight()-20)
        {
            unsigned char* data = _tutMask_1->getData();
            
            result = (data[4 * (cavePosX + cavePosY * _tutMask_1->getWidth()) + 3] != 0);
        }
    }
    else if(mTutorialStep == 24)
    {
        //The troll and effect avoid
        
        int cavePosX = posX - designResolutionSize.width / 2 + _tutMask_2->getWidth() / 2-50;
        int cavePosY = posY - designResolutionSize.height / 2 + _tutMask_2->getHeight() / 2-170;
        
        if (!result &&
            cavePosX >= 0 && cavePosX < _tutMask_2->getWidth() &&
            cavePosY >= 0 && cavePosY < _tutMask_2->getHeight())
        {
//            CCLog("Action");
            unsigned char* data = _tutMask_2->getData();
            
            result = (data[4 * (cavePosX + cavePosY * _tutMask_2->getWidth()) + 3] != 0);
        }
        
        
        cavePosX = posX - designResolutionSize.width / 2 + _tutMask_3->getWidth() / 2-200;
        cavePosY = posY - designResolutionSize.height / 2 + _tutMask_3->getHeight() / 2-170;
        
        if (!result &&
            cavePosX >= 0 && cavePosX < _tutMask_3->getWidth() &&
            cavePosY >= 0 && cavePosY < _tutMask_3->getHeight())
        {
//            CCLog("Action");
            unsigned char* data = _tutMask_3->getData();
            
            result = (data[4 * (cavePosX + cavePosY * _tutMask_3->getWidth()) + 3] != 0);
        }
    }
    
    
    
//	if (posX >= 0 && posX < _mask->getWidth() &&
//		posY >= 0 && posY < _mask->getHeight())
//	{
//		unsigned char* data = _mask->getData();
//		
//		//get alpha channel
//		result = (data[4 * (posX + posY * _mask->getWidth()) + 3] != 0);
//	}
//	
//	int cavePosX = posX - designResolutionSize.width / 2 + _caveMask->getWidth() / 2;
//	int cavePosY = posY - designResolutionSize.height / 2 + _caveMask->getHeight() / 2;
//	
//	if (!result &&
//		cavePosX >= 0 && cavePosX < _caveMask->getWidth() &&
//		cavePosY >= 0 && cavePosY < _caveMask->getHeight())
//	{
//		unsigned char* data = _caveMask->getData();
//		
//		result = (data[4 * (cavePosX + cavePosY * _caveMask->getWidth()) + 3] != 0);
//	}
	
	return result;
}


void GameScene::introFinished(IntroAnimation* introAnimation)
{
	removeChild(introAnimation);
	_introAnimations->removeObject(introAnimation);
	
	switch (introAnimation->getType())
	{
		case INTRO_TROLL:
		{
			TrollIntro* trollIntro = (TrollIntro*)introAnimation;
			Troll* troll = trollIntro->getTroll();
            
            //Check if this troll is still in the game
            for (int trollIndex = _trolls->count() - 1; trollIndex >= 0; --trollIndex)
            {
                Troll* trollIn = static_cast<Troll*>(_trolls->objectAtIndex(trollIndex));
                if (troll == trollIn)
                {
                    troll->setVisible(true);
                    troll->setCanMove(true);
                    
                    if(mTutorialEnabled)
                    {
                        //Allow to do something only when troll spawned?
                        pauseSchedulerAndActionsRecursive(troll,true);
                    }
                    else
                    {
                        playInGameSound("troll_walk");
                        
                        /*
                        if(User::getInstance()->_tutorial_small_6==0)
                        {
                            if(User::getInstance()->getTimesPlayed()>=5 && User::getInstance()->getMachine_EnemiesLvl()>0)
                            {
                                //1st time pause the game and show some stuff !!!
                                stopInGameSound("Footsteps",true);
                                stopInGameSound("troll_walk",true);
                                stopInGameSound("dwarf_web_stuck",true);
                                
                                //Get the mid point !!!
                                int aShowX = getChildByTag(1030)->getPositionX();
                                int aShowY = getChildByTag(1030)->getPositionY();
                                
                                CreateSmallTutorial(6,aShowX,aShowY);
                            }
                        }
                        */
                    }
                    break;
                }
            }
		}
		break;
			
		case INTRO_DWARF:
		{
			DwarfIntro* dwarfIntro = (DwarfIntro*)introAnimation;
			Dwarf* dwarf = dwarfIntro->getDwarf();
            
            //Check if this troll is still in the game
            for (int trollIndex = _dwarves->count() - 1; trollIndex >= 0; --trollIndex)
            {
                Dwarf* dwarfIn = static_cast<Dwarf*>(_dwarves->objectAtIndex(trollIndex));
                if (dwarf == dwarfIn)
                {
                    dwarf->setVisible(true);
                    playInGameSound("Footsteps");
                    break;
                }
            }
			
//            if (dwarf->getParent())
//                dwarf->setVisible(true);
//            else
//                CCLog("CRASH IN INTRO_DWARF - DONT KNOW WHY");
		}
		break;
			
		case INTRO_CRYSTAL:
		{
			CrystalIntro* crystalIntro = (CrystalIntro*)introAnimation;
			Crystal* crystal = crystalIntro->getCrystal();
            
            //Create now the real timers !!!
            crystal->onFinishedIntro();
            crystal->setVisible(true);
		}
		break;
			
		case INTRO_EFFECT:
		{
			EffectIntro* effectIntro = (EffectIntro*)introAnimation;
			Effect* effect = effectIntro->getEffect();
			
            //Check if this is cool !!!
            effect->onFinishedIntro();
            effect->setVisible(true);
            
            if(mTutorialEnabled)
            {
                if(mTutorialStep == 23)
                {
                    //Add timer 1.0sec
                    mTutorialTimerSet = true;
                    mTutorialTimer = 1.0f;
                }
            }
            
//            playInGameSound("meteorite_hit_ground");
		}
		break;
			
		case INTRO_DIAMOND:
		{
			DiamondIntro* diamondIntro = (DiamondIntro*)introAnimation;
			Diamond* diamond = diamondIntro->getDiamond();
			
            diamond->setVisible(true);
            diamond->createAnim();
		}
		break;
	}
}

void GameScene::addPickedUpCrystall(CrystalColor col)
{
    _stats.gatheredCrystals[col]++;
    
    if(!mTutorialEnabled)
    {
        switch (col) {
            case CRYSTAL_COLOR_BLUE:
                User::getInstance()->getMissionManager().CheckSubMission(SUB_COLLECT_CRYSTAL_BLUE,1);
                break;
            case CRYSTAL_COLOR_GREEN:
                User::getInstance()->getMissionManager().CheckSubMission(SUB_COLLECT_CRYSTAL_GREEN,1);
                break;
            case CRYSTAL_COLOR_RED:
                User::getInstance()->getMissionManager().CheckSubMission(SUB_COLLECT_CRYSTAL_RED,1);
                break;
            case CRYSTAL_COLOR_YELLOW:
                User::getInstance()->getMissionManager().CheckSubMission(SUB_COLLECT_CRYSTAL_YELLOW,1);
                break;
                
            default:
                break;
        }
    }
}

const cocos2d::CCPoint& GameScene::getCavePoint(DwarfType dwarfType)
{
	return _cavePoints.at(dwarfType);
}

//Some sound for some sounds magic !!!
void GameScene::playInGameSound(const char* theName,bool theForce)
{
    //For android use ogg | iphone special ios format
    char aSoundFX[32];
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    sprintf(aSoundFX,"Sound/%s.ogg",theName);
    sprintf(aSoundFX,"Sound/%s.wav",theName);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sprintf(aSoundFX,"Sound/%s.wav",theName);//Maybe in wav we can use something better?
    #endif
    
    //Check if not doubling this sound !!!
    bool aCanPlay = true;
    
    if (strcmp(theName, "Footsteps")==0)
    {
        if (_dwarfWalkOn)
		{
            SimpleAudioEngine::sharedEngine()->stopEffect(_dwarfWalkID);
		}
		
        _dwarfWalkOn = true;
    }
    else if (strcmp(theName, "troll_walk")==0)
    {
        if (_monsterWalkOn)
            aCanPlay = false;
        _monsterWalkOn = true;
    }
    
    if (!aCanPlay)
        return;//No luck this time - wait for next !!!
    
    //Save the id's for later sound stop
    if (strcmp(theName, "Footsteps")==0)
        _dwarfWalkID = SimpleAudioEngine::sharedEngine()->playEffect(aSoundFX, true);
    else if (strcmp(theName, "troll_walk")==0)
        _monsterWalkID = SimpleAudioEngine::sharedEngine()->playEffect(aSoundFX, true);
    else if (strcmp(theName, "save_me_wait")==0)
        _saveMeID = SimpleAudioEngine::sharedEngine()->playEffect(aSoundFX, false);
    else if (strcmp(theName, "dwarf_web_stuck")==0)
        _webID = SimpleAudioEngine::sharedEngine()->playEffect(aSoundFX,true);
    else
        SimpleAudioEngine::sharedEngine()->playEffect(aSoundFX,false);
}

void GameScene::stopInGameSound(const char* theName,bool theForce)
{
    //Check for sound if need can stop it right now !!!
    int aSoundID = -1;
    bool aCanDisable = true;
    
    if (strcmp(theName,"Footsteps") == 0)
    {
        aSoundID = _dwarfWalkID;
        
        //Check how many dwarfs are walking in the map - maybe don't need to stop all
        for (int trollIndex = _dwarves->count() - 1; trollIndex >= 0; --trollIndex)
        {
            Dwarf* dwarfIn = static_cast<Dwarf*>(_dwarves->objectAtIndex(trollIndex));
            if (dwarfIn != NULL)
            {
                if (dwarfIn->getDisabled() == false && dwarfIn->getEffect() == NULL)
                {
                    aCanDisable = false;
                    break;
                }
            }
        }
        
        if (aCanDisable && !theForce)
            _dwarfWalkOn = false;
    }
    else if (strcmp(theName,"troll_walk") == 0)
    {
        aSoundID = _monsterWalkID;
        
        //Check how many dwarfs are walking in the map - maybe don't need to stop all
        for (int trollIndex = _trolls->count() - 1; trollIndex >= 0; --trollIndex)
        {
            Troll* trollIn = static_cast<Troll*>(_trolls->objectAtIndex(trollIndex));
            if (trollIn != NULL)
            {
                if (trollIn->getCanMove() && !trollIn->getIsDisabled())
                {
                    aCanDisable = false;
                    break;
                }
            }
        }
        
        if (aCanDisable && !theForce)
		{
            _monsterWalkOn = false;
		}
    }
    else if (strcmp(theName,"save_me_wait")==0)
    {
        aSoundID = _saveMeID;
    }
    else if (strcmp(theName,"dwarf_web_stuck")==0)
    {
        aSoundID = _webID;
    }
    
    if (aSoundID!=-1 && (aCanDisable || theForce))
    {
        if (aSoundID == _dwarfWalkID)
            _dwarfWalkOn = false;
        if (aSoundID == _monsterWalkID)
            _monsterWalkOn = false;
        
        SimpleAudioEngine::sharedEngine()->stopEffect(aSoundID);
    }

}

//CCDirector::sharedDirector() -> getTouchDispatcher() -> addStandardDelegate( this, 0 );

//void GameScene::onEnter(){
//    // before 2.0:
//    // CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
//    
//    // since 2.0:
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
//    CCLayer::onEnter();
//}
//
//void GameScene::onExit(){
//    // before 2.0:
//    // CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
//    
//    // since 2.0:
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
//    CCLayer::onExit();
//}

//bool GameScene::ccTouchBegan(CCTouch* touch, CCEvent* event){
//    //do whatever you want here
//    CCLOG("Start touch");
//    return true;
//}
//void GameScene::ccTouchMoved(CCTouch* touch, CCEvent* event){
//    //do what you want
//    CCLOG("Move touch");
//}
//void GameScene::ccTouchEnded(CCTouch* touch, CCEvent* event){
//    //do your job here
//    CCLOG("End touch");
//}

////void onEnterTransitionDidFinish
void GameScene::onEnterTransitionDidFinish()
{
    CreateMachines();
    
    if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/music_GriegLoop.mp3", true);
}

//The map change effects !!!
void GameScene::CreateStartMap(int theTypeID)
{
    CCLog("theMapType: %i",theTypeID);
    //For each level own mask
    _objectMask = new CCImage();
    _objectMask->initWithImageFile("MapSpawnObjectMask.png");
    _objectMask->retain();
    
    mCurrentMapID = theTypeID;
    
//    std::stringstream theMapBase;
//    theMapBase << "gadalaiki/summer/map_summer.png";
    
    CCParticleSystemQuad* p = NULL;
    
    //By type we will change all the trees and stuff to needed !!!
    std::stringstream theMapBase;
    switch (theTypeID) {
        case kMap_Summer:
            theMapBase << "gadalaiki/summer/map_summer.png";
            
            break;
        case kMap_Autumn:
            theMapBase << "gadalaiki/autumn/map_autumn.png";
            StartRain(false);
            break;
        case kMap_Winter:
            theMapBase << "gadalaiki/winter/map_winter.png";
            
            mSnowActive = true;
            p = CCParticleSystemQuad::create("Particles/SnowEasy.plist");
            p->setPosition(ccp(_screenSize.width/2,_screenSize.height+50));
            p->setAutoRemoveOnFinish(true);
            addChild(p,kHUD_Z_Order-10,9880);//The reain tag :)
            
            break;
        case kMap_Spring:
            theMapBase << "gadalaiki/spring/map_spring.png";
            StartTornado(false);
            break;
            
        default:
            break;
    }
    
    mSnowActive = false;
    
    mMapBase = CCSprite::create(theMapBase.str().c_str());
    mMapBase->setPosition(ccp(_screenSize.width/2 + _origin.x, _screenSize.height/2 + _origin.y));
    addChild(mMapBase,0);
    
    if(mNewMapRandomEnabled){
        return;//For now !!!
    }
    
    //The tree array !!!
//    CCSprite* aTree1 = CCSprite::create("gadalaiki/summer/summer_resnkoks.png");
//    aTree1->setTag(10010);//The tree tag !!!
    

//    for(int i=0;i<10;i++)
//    {
//        CCSprite* aTest = CCSprite::create("gadalaiki/summer/summer_resnkoks.png");
//        aTest->setPosition(ccp(i*40,200));
//        addChild(aTest);
//    }
    
//    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create("gadalaiki/summer/summer_resnkoks.png", 10);
//    batchNode->setPosition(CCPointZero);
//    this->addChild(batchNode);
    
    _batchNode = CCSpriteBatchNode::create("gadalaiki.png");
    _batchNode->setPosition(CCPointZero);
    this->addChild(_batchNode);
    
    //use the array from main menu !!!
    std::vector<float> aSkewVector( User::getInstance()->_skewPositionFromMain.size() );
    std::copy( User::getInstance()->_skewPositionFromMain.begin(), User::getInstance()->_skewPositionFromMain.end(), aSkewVector.begin() );
    
//    float aSkewX = (rand()%10)*0.1f;
//    int aDirSkewX = rand()*2;
    
    std::stringstream summer_resnkoks;
    std::stringstream summer_egle;
    std::stringstream summer_krums;
    std::stringstream summer_garkoks;
    
    switch (mCurrentMapID)
    {
        case kMap_Summer:
            summer_resnkoks << "summer_resnkoks.png";
            summer_egle << "summer_egle.png";
            summer_krums << "summer_kruums.png";
            summer_garkoks << "summer_garkoks.png";
            break;
        case kMap_Autumn:
            summer_resnkoks << "autumn_resnkoks.png";
            summer_egle << "autumn_egle.png";
            summer_krums << "autumn_kruums.png";
            summer_garkoks << "autumn_garkoks.png";
            break;
        case kMap_Winter:
            summer_resnkoks << "winter_resnkoks.png";
            summer_egle << "winter_egle.png";
            summer_krums << "winter_kruums.png";
            summer_garkoks << "winter_garkoks.png";
            break;
        case kMap_Spring:
            summer_resnkoks << "spring_resnkoks.png";
            summer_egle << "spring_egle.png";
            summer_krums << "spring_kruums.png";
            summer_garkoks << "spring_garkoks.png";
            break;
    }
    
    
    //Augseja salina !!!
    //Aukseja salina
    CCSprite *sprTest = CCSprite::createWithSpriteFrameName(summer_resnkoks.str().c_str());
    sprTest->setTag(10400);
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    sprTest->setPosition(ccp(471,650));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_egle.str().c_str());
    sprTest->setTag(10100);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.75f);
    sprTest->setPosition(ccp(593,667));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10300);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.6f);
    sprTest->setPosition(ccp(590,646));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_garkoks.str().c_str());
    sprTest->setTag(10200);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(1.05f);
    sprTest->setPosition(ccp(544,616));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_garkoks.str().c_str());
    sprTest->setTag(10201);
    sprTest->setScale(1.0f);
    sprTest->setPosition(ccp(420,632));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,4);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_resnkoks.str().c_str());
    sprTest->setTag(10401);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(340,618));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,5);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_egle.str().c_str());
    sprTest->setTag(10101);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.8f);
    sprTest->setPosition(ccp(410,600));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,6);
    
    //===================================================================
    //Sanu laba aukseja salina
    sprTest = CCSprite::createWithSpriteFrameName(summer_egle.str().c_str());
    sprTest->setTag(10102);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.7f);
    sprTest->setPosition(ccp(804,651));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10301);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.6f);
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(831,620));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_resnkoks.str().c_str());
    sprTest->setTag(10402);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(false);
    sprTest->setScale(1.2f);
    sprTest->setPosition(ccp(844,614));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10302);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(1.0f);
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(872,570));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_garkoks.str().c_str());
    sprTest->setTag(10202);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(true);
    sprTest->setScale(0.82f);
    sprTest->setPosition(ccp(921,550));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,4);
    
    //===================================================================
    //Sanu laba vidus salina
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_resnkoks.str().c_str());
    sprTest->setTag(10403);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(true);
    sprTest->setScale(1.0f);
    sprTest->setPosition(ccp(861,314));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_egle.str().c_str());
    sprTest->setTag(10103);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.94f);
    sprTest->setPosition(ccp(920,267));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10303);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.67f);
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(897,250));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    //===================================================================
    //Sanu laba lejus dala
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10304);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(1.16f);
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(826,44));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_garkoks.str().c_str());
    sprTest->setTag(10203);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(false);
    sprTest->setScale(0.89f);
    sprTest->setPosition(ccp(909,-17));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_resnkoks.str().c_str());
    sprTest->setTag(10404);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(true);
    sprTest->setScale(1.18f);
    sprTest->setPosition(ccp(823,-56));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_egle.str().c_str());
    sprTest->setTag(10104);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(1.08f);
    sprTest->setPosition(ccp(888,-54));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    //===================================================================
    //vidus lejus dala
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10305);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(1.10f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(482,10));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10306);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.91f);
    sprTest->setFlipX(true);
    sprTest->setPosition(ccp(431,5));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10307);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.51f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(569,10));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_resnkoks.str().c_str());
    sprTest->setTag(10405);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(true);
    sprTest->setScale(1.15f);
    sprTest->setPosition(ccp(452,-97));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    //===================================================================
    //kreisa lejas dala
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10308);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.68f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(84,70));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_egle.str().c_str());
    sprTest->setTag(10105);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.76f);
    sprTest->setPosition(ccp(18,38));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_egle.str().c_str());
    sprTest->setTag(10106);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(1.0f);
    sprTest->setPosition(ccp(-45,0));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10309);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(1.3f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(50,-2));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_garkoks.str().c_str());
    sprTest->setTag(10204);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(false);
    sprTest->setScale(1.15f);
    sprTest->setPosition(ccp(19,-89));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,4);
    
    //===================================================================
    //kreisa vidus dala
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_egle.str().c_str());
    sprTest->setTag(10109);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.75f);
    sprTest->setPosition(ccp(-3,320));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_resnkoks.str().c_str());
    sprTest->setTag(10406);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(true);
    sprTest->setScale(1.08f);
    sprTest->setPosition(ccp(-79,290));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10310);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.75f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(5,280));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_garkoks.str().c_str());
    sprTest->setTag(10205);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(false);
    sprTest->setScale(0.65f);
    sprTest->setPosition(ccp(-26,264));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    //===================================================================
    //kreisa auksas dala
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_resnkoks.str().c_str());
    sprTest->setTag(10407);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(true);
    sprTest->setScale(0.82f);
    sprTest->setPosition(ccp(-22,668));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,0);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_egle.str().c_str());
    sprTest->setTag(10107);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(true);
    sprTest->setScale(1.15f);
    sprTest->setPosition(ccp(39,653));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,1);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_resnkoks.str().c_str());
    sprTest->setTag(10408);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(false);
    sprTest->setScale(0.82f);
    sprTest->setPosition(ccp(-47,610));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,2);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_krums.str().c_str());
    sprTest->setTag(10311);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setScale(0.82f);
    sprTest->setFlipX(false);
    sprTest->setPosition(ccp(47,625));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,3);
    
    //
    sprTest = CCSprite::createWithSpriteFrameName(summer_garkoks.str().c_str());
    sprTest->setTag(10206);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(true);
    sprTest->setScale(0.60f);
    sprTest->setPosition(ccp(23,580));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,4);
    
    sprTest = CCSprite::createWithSpriteFrameName(summer_egle.str().c_str());
    sprTest->setTag(10108);
    
    //----------------
    sprTest->setSkewX(aSkewVector.front());
    aSkewVector.erase(aSkewVector.begin());
    //================
    
    sprTest->setFlipX(false);
    sprTest->setScale(0.88f);
    sprTest->setPosition(ccp(-41,548));
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    _batchNode->addChild(sprTest,5);
    
    
//    for(int i = 0;i < 10;++i)
//    {
//        CCSprite *sprTest = CCSprite::createWithSpriteFrameName("summer_resnkoks.png");
////        CCSprite* sprTest = CCSprite::createWithTexture(batchNode->getTexture());
//        sprTest->setPosition(ccp(i*40,200));
//        sprTest->setTag(10100+i);
//        _batchNode->addChild(sprTest,1);
//        
////        summer_kruums
////        addChild(sprTest);
//        
//        sprTest = CCSprite::createWithSpriteFrameName("summer_garkoks.png");
//        sprTest->setTag(10200+i);
//        sprTest->setPosition(ccp(i*40,150));
//        sprTest->setAnchorPoint(ccp(0.0f,0.1f));
//        
//        CCSkewTo* aSkew1 = CCSkewTo::create(2.0f, 3.0f, 0.0f);
//        CCEaseSineInOut* aSkewEase1 = CCEaseSineInOut::create(aSkew1);
//        CCSkewTo* aSkew2 = CCSkewTo::create(2.0f, -3.0f, 0.0f);
//        CCEaseSineInOut* aSkewEase2 = CCEaseSineInOut::create(aSkew2);
//        
//        CCSequence* aSeq1 = CCSequence::create(aSkewEase1,aSkewEase2,NULL);
//        CCRepeatForever* aSkewRepeat = CCRepeatForever::create(aSeq1);
//        aSkewRepeat->setTag(1111);
//        sprTest->runAction(aSkewRepeat);
//        
//        
////        sprTest->setSkewX(10.0f);
//        
//        _batchNode->addChild(sprTest,2);
//    }
    
    //Change batch node objects !!!
    CCSkewTo* aSkew1 = NULL;
    CCSkewTo* aSkew2 = NULL;
    
    int aRanomdValue = 0;
    
    for(int x=1;x<5;x++)
    {
        //For the all objects
        for(int i =0;i<20;i++)
        {
            CCSprite* aDummy1 = (CCSprite*)_batchNode->getChildByTag(10000+i+(x*100));
            if(aDummy1==NULL)
                continue;
            
            //To what side to skew !!!
            aRanomdValue = rand()%2;
            if(aRanomdValue>0)
            {
                aSkew1 = CCSkewTo::create(2.0f,-3.0f,0.0f);
                aSkew2 = CCSkewTo::create(2.0f, 3.0f, 0.0f);
            }
            else
            {
                aSkew1 = CCSkewTo::create(2.0f, 3.0f, 0.0f);
                aSkew2 = CCSkewTo::create(2.0f,-3.0f,0.0f);
            }
            
            CCEaseSineInOut* aSkewEase1 = CCEaseSineInOut::create(aSkew1);
            CCEaseSineInOut* aSkewEase2 = CCEaseSineInOut::create(aSkew2);
            
            CCSequence* aSeq1 = CCSequence::create(aSkewEase1,aSkewEase2,NULL);
            CCRepeatForever* aSkewRepeat = CCRepeatForever::create(aSeq1);
            aSkewRepeat->setTag(1111);
            aDummy1->runAction(aSkewRepeat);
        }
    }
}

void GameScene::ChangeMap(int theTypeID)
{
    if(User::getInstance()->mNewMissionBuild){
        return;//Do not change map if new build
    }
    
    //Debug
//    mCurrentMapID = theTypeID;
    mCurrentMapID+=1;
    if(mCurrentMapID>4)
        mCurrentMapID = 1;
    theTypeID = mCurrentMapID;
    
    CCParticleSystemQuad* p = NULL;
    
    if(mSnowActive)
    {
        mSnowActive = false;
        p = (CCParticleSystemQuad*)getChildByTag(9880);
        CCFadeOut* aFadeParticle = CCFadeOut::create(1.0f);
        p->runAction(aFadeParticle);
        p->setStartColor(ccc4f(0,0,0,0));
        p->setEndColor(ccc4f(0,0,0,0));
        
        CCCallFuncN* aFuncDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
        CCDelayTime* aShowDelay = CCDelayTime::create(2.0f);
        CCSequence* aSeqParticle = CCSequence::create(aShowDelay,aFuncDone,NULL);
        p->runAction(aSeqParticle);
        
        
//        removeChildByTag(9880);
    }
    
    StartRain(true);
    StartTornado(true);
    
    _currentMinSpwanedTime = 0;
    _effectActiveSpawnTime = 5.0f;//5 sec to a crash !!!
    
    //By type we will change all the trees and stuff to needed !!!
    std::stringstream theMapBase;
    switch (theTypeID) {
        case kMap_Summer:
            theMapBase << "gadalaiki/summer/map_summer.png";
            
            break;
        case kMap_Autumn:
            theMapBase << "gadalaiki/autumn/map_autumn.png";
            StartRain(false);
            break;
        case kMap_Winter:
            theMapBase << "gadalaiki/winter/map_winter.png";
            
            mSnowActive = true;
//            //Start the winter rain !!!
            p = CCParticleSystemQuad::create("Particles/SnowEasy.plist");
            p->setPosition(ccp(_screenSize.width/2,_screenSize.height+50));
            p->setAutoRemoveOnFinish(true);
            addChild(p,kHUD_Z_Order-10,9880);//The reain tag :)
            
            break;
        case kMap_Spring:
            theMapBase << "gadalaiki/spring/map_spring.png";
            StartTornado(false);
            break;
            
        default:
            break;
    }
    
    //Create fake up fade map
    CCSprite* aFakeMap = CCSprite::createWithTexture(mMapBase->getTexture());
    aFakeMap->setPosition(ccp(_screenSize.width/2 + _origin.x, _screenSize.height/2 + _origin.y));
    addChild(aFakeMap,-1);
    
    //Remove the fake map after transfer done !!!
    CCCallFuncN* aFuncDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeNode));
    CCDelayTime* aShowDelay = CCDelayTime::create(2.0f);
    CCSequence* aSeq = CCSequence::create(aShowDelay,aFuncDone,NULL);
    aFakeMap->runAction(aSeq);
    
    mMapBase->setOpacity(0);
    mMapBase->setTexture(CCSprite::create(theMapBase.str().c_str())->getTexture());
    
    //Fade in the new map !!!
    CCDelayTime* aDelay1 = CCDelayTime::create(1.0f);
    CCFadeIn* aFadeIn1 = CCFadeIn::create(1.0f);
    CCSequence* aSeq1 = CCSequence::create(aDelay1,aFadeIn1,NULL);
    mMapBase->runAction(aSeq1);
    
    //Change batch node objects !!!
    for(int x=1;x<5;x++)
    {
        //For the all objects
        for(int i =0;i<20;i++)
        {
            CCSprite* aDummy1 = (CCSprite*)_batchNode->getChildByTag(10000+i+(x*100));
            if(aDummy1==NULL)
                continue;
            aDummy1->stopAllActions();
            
            CCCallFuncN* aFuncDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::OnChangeObject));
            CCSkewTo* aSkew1 = NULL;
            
            if(aDummy1->getSkewX()<=0)
            {
                aSkew1 = CCSkewTo::create(1.0f, 0.0f, 0.0f);
            }
            else
            {
                aSkew1 = CCSkewTo::create(1.0f, 0.0f, 0.0f);
            }
            
            CCEaseSineOut* aSkewEase1 = CCEaseSineOut::create(aSkew1);
            CCSequence* aSeq1 = CCSequence::create(aSkewEase1,aFuncDone,NULL);
            aDummy1->runAction(aSeq1);
        }
    }
    
//    _batchNode->draw();
}

void GameScene::OnChangeObject(CCNode* sender)
{
    //Create replace stuff !!!
    //Comes here when all animations ended
    sender->stopAllActions();
    
    std::stringstream theMapBase;
    switch (mCurrentMapID) {
        case kMap_Summer:
            if(sender->getTag()>=10100 && sender->getTag()<10200)//Timber
                theMapBase << "summer_egle.png";
            else if(sender->getTag()>=10200 && sender->getTag()<10300)//Timber
                theMapBase << "summer_garkoks.png";
            else if(sender->getTag()>=10300 && sender->getTag()<10400)//Timber
                theMapBase << "summer_kruums.png";
            else if(sender->getTag()>=10400 && sender->getTag()<10500)//Timber
                theMapBase << "summer_resnkoks.png";
            break;
        case kMap_Autumn:
            if(sender->getTag()>=10100 && sender->getTag()<10200)//Timber
                theMapBase << "autumn_egle.png";
            else if(sender->getTag()>=10200 && sender->getTag()<10300)//Timber
                theMapBase << "autumn_garkoks.png";
            else if(sender->getTag()>=10300 && sender->getTag()<10400)//Timber
                theMapBase << "autumn_kruums.png";
            else if(sender->getTag()>=10400 && sender->getTag()<10500)//Timber
                theMapBase << "autumn_resnkoks.png";
            break;
        case kMap_Winter:
            if(sender->getTag()>=10100 && sender->getTag()<10200)//Timber
                theMapBase << "winter_egle.png";
            else if(sender->getTag()>=10200 && sender->getTag()<10300)//Timber
                theMapBase << "winter_garkoks.png";
            else if(sender->getTag()>=10300 && sender->getTag()<10400)//Timber
                theMapBase << "winter_kruums.png";
            else if(sender->getTag()>=10400 && sender->getTag()<10500)//Timber
                theMapBase << "winter_resnkoks.png";
            break;
        case kMap_Spring:
            if(sender->getTag()>=10100 && sender->getTag()<10200)//Timber
                theMapBase << "spring_egle.png";
            else if(sender->getTag()>=10200 && sender->getTag()<10300)//Timber
                theMapBase << "spring_garkoks.png";
            else if(sender->getTag()>=10300 && sender->getTag()<10400)//Timber
                theMapBase << "spring_kruums.png";
            else if(sender->getTag()>=10400 && sender->getTag()<10500)//Timber
                theMapBase << "spring_resnkoks.png";
            break;
            
        default:
            break;
    }
    
    int aOrderZ = sender->getZOrder();
    
    int aDirection = rand()%2;
    CCSkewTo* aSkewBase = NULL;
    if(aDirection<1)
        aSkewBase = CCSkewTo::create(2.0f,3.0f,0.0f);
    else
        aSkewBase = CCSkewTo::create(2.0f,-3.0f,0.0f);
    
    CCDelayTime* aDelay = CCDelayTime::create(0.5f);
    CCFadeOut* aFadeOut = CCFadeOut::create(0.5f);
    CCSequence* aSeqBasePre = CCSequence::create(aDelay,aFadeOut,NULL);
    
    CCCallFuncN* aFuncDoneBase = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeBatchNode));
    CCSequence* aSeqBase = CCSequence::create(aSkewBase,aFuncDoneBase,NULL);
    
    CCSpawn* aSpawnBase = CCSpawn::create(aSeqBase,aSeqBasePre,NULL);
    
    sender->runAction(aSpawnBase);
    
    CCSprite* aMoreInfo = (CCSprite*)sender;
    
    //By tag check what is he !!!
    CCSprite *sprTest = CCSprite::createWithSpriteFrameName(theMapBase.str().c_str());
    sprTest->setOpacity(0);
    sprTest->setFlipX(aMoreInfo->isFlipX());
    sprTest->setAnchorPoint(ccp(0.0f,0.1f));
    sprTest->setScale(sender->getScale());
    sprTest->setPosition(sender->getPosition());
//    sprTest->setSkewX(sender->getSkewX());
//    sprTest->setSkewY(sender->getSkewY());
    sprTest->setTag(sender->getTag());
    _batchNode->addChild(sprTest,aOrderZ);
    
    //Cross fade and start to animate
    CCFadeIn* aFadeIn = CCFadeIn::create(1.0f);
    CCCallFuncN* aFuncDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::AddLoopAnimation));
    CCSkewTo* aSkew = NULL;
    
    if(aDirection<1)
        aSkew = CCSkewTo::create(2.0f,3.0f,0.0f);
    else
        aSkew = CCSkewTo::create(2.0f,-3.0f,0.0f);
    
    CCSpawn* aSpawn1 = CCSpawn::create(aFadeIn,aSkew,NULL);
    CCSequence* aSeq1 = CCSequence::create(aSpawn1,aFuncDone,NULL);
    sprTest->runAction(aSeq1);
    
}

void GameScene::AddLoopAnimation(CCNode* sender)
{
    sender->stopAllActions();
    
    CCSkewTo* aSkew1 = NULL;
    CCSkewTo* aSkew2 = NULL;
    
    //To what side to skew !!!
    if(sender->getSkewX()>0)
    {
        aSkew1 = CCSkewTo::create(2.0f,-3.0f,0.0f);
        aSkew2 = CCSkewTo::create(2.0f, 3.0f, 0.0f);
    }
    else
    {
        aSkew1 = CCSkewTo::create(2.0f, 3.0f, 0.0f);
        aSkew2 = CCSkewTo::create(2.0f,-3.0f,0.0f);
    }
    
    CCEaseSineInOut* aSkewEase1 = CCEaseSineInOut::create(aSkew1);
    CCEaseSineInOut* aSkewEase2 = CCEaseSineInOut::create(aSkew2);
    
    CCSequence* aSeq1 = CCSequence::create(aSkewEase1,aSkewEase2,NULL);
    CCRepeatForever* aSkewRepeat = CCRepeatForever::create(aSeq1);
    aSkewRepeat->setTag(1111);
    sender->runAction(aSkewRepeat);
}

//The new mission stuff
void GameScene::CheckMissionByValue(int theType,float theValue)
{
    if(theType == mCurrentMission.Task_type)
    {
        //We have a match !!! Check what is this !!!
        if(theValue>_missionCurrentValue){
            _missionCurrentValue = theValue;
            //Update it and check stars
            std::stringstream theResult;
            //Special check for time mission and other magic if needed
            if(theType == MissionType_Time){
                theResult << _missionCurrentValue <<"/"<<_mission_star_points_3;//Convert it to time
            }
            else{
                theResult << _missionCurrentValue <<"/"<<_mission_star_points_3;
            }
            
            _pointsLabel->setString(theResult.str().c_str());
            
            //---------------------------------------------------------------
            //Check the stars update
            
            if(_missionCurrentValue>=_mission_star_points_1){
                _mission_star_1->setOpacity(255);
            }
            else{
                _mission_star_1->setOpacity(80);
            }
            
            if(_missionCurrentValue>=_mission_star_points_2){
                _mission_star_2->setOpacity(255);
            }
            else{
                _mission_star_2->setOpacity(80);
            }
            
            if(_missionCurrentValue>=_mission_star_points_3){
                _mission_star_3->setOpacity(255);
            }
            else{
                _mission_star_3->setOpacity(80);
            }
            
            //---------------------------------------------------------------
            // The progress bars
            
            //Check the points for each bar
            int aDummyPoints = _missionCurrentValue;
            if(aDummyPoints>_mission_star_points_1){
                aDummyPoints = _mission_star_points_1;
            }
            _mission_progress_bar_1->setTextureRect(CCRect(0, 0,
                                                           _mission_progress_bar_1->getTexture()->getContentSize().width*((float)aDummyPoints / _mission_star_points_1),
                                                           _mission_progress_bar_1->getTexture()->getContentSize().height));
            
            aDummyPoints = _missionCurrentValue-_mission_star_points_1;
            if(aDummyPoints<0){
                aDummyPoints = 0;
            }
            else if(aDummyPoints>_mission_star_points_2-_mission_star_points_1){
                aDummyPoints = _mission_star_points_2-_mission_star_points_1;
            }
            
            _mission_progress_bar_2->setTextureRect(CCRect(0, 0,
                                                           _mission_progress_bar_2->getTexture()->getContentSize().width*
                                                           ((float)aDummyPoints / (_mission_star_points_2-_mission_star_points_1)),
                                                           _mission_progress_bar_2->getTexture()->getContentSize().height));
            
            aDummyPoints = _missionCurrentValue-_mission_star_points_2;
            if(aDummyPoints<0){
                aDummyPoints = 0;
            }
            else if(aDummyPoints>_mission_star_points_3-_mission_star_points_2){
                aDummyPoints = _mission_star_points_3-_mission_star_points_2;
            }
            
            _mission_progress_bar_3->setTextureRect(CCRect(0, 0,
                                                           _mission_progress_bar_3->getTexture()->getContentSize().width*
                                                           ((float)aDummyPoints / (_mission_star_points_3-_mission_star_points_2)),
                                                           _mission_progress_bar_3->getTexture()->getContentSize().height));
            
            aDummyPoints = _missionCurrentValue-_mission_star_points_3;
            if(aDummyPoints<0){
                aDummyPoints = 0;
            }
            else if(aDummyPoints>(_mission_star_points_3+100)-_mission_star_points_3){
                aDummyPoints = (_mission_star_points_3+100)-_mission_star_points_3;
            }
            
            _mission_progress_bar_4->setTextureRect(CCRect(0, 0,
                                                           _mission_progress_bar_4->getTexture()->getContentSize().width*
                                                           ((float)aDummyPoints / ((_mission_star_points_3+100)-_mission_star_points_3)),
                                                           _mission_progress_bar_4->getTexture()->getContentSize().height));
            
            //---------------------------------------------------------------
        }
    }
}

void GameScene::ResetValues()
{
    mTotalMushroom = 0;
    
    mTotalGreen_Crystals = 0;
    mTotalBlue_Crystals = 0;
    mTotalRed_Crystals = 0;
    mTotalYellow_Crystals = 0;
    
    mTotalMushroom = 0;
    mTotalTimeInGame = 0;
    mTotalEscapesFromTroll = 0;
    
    mLastSpawnID = -1;
    
    _SubDwarfSpawn = 0;
    _LeftNotSpawnDwatfs = 0;
    
    _MasterTroll_TimeToAct = (rand()%20)+20;
}

//void GameScene:: keyBackClicked(void) {
//    CCDirector::sharedDirector()->end();
//    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
//}
