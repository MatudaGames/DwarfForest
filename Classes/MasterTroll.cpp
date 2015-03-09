//
//  MasterTroll.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 06/03/15.
//
//

#include "MasterTroll.h"



#include <SimpleAudioEngine.h>
#include "GameScene.h"
#include "AppMacros.h"
#include "Utils.h"
#include "User.h"


USING_NS_CC;
using namespace CocosDenshion;

MasterTroll* MasterTroll::create(GameScene* game)
{
    MasterTroll *pRet = new MasterTroll();
    if (pRet && pRet->init(game))
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

MasterTroll::MasterTroll():
_game(NULL), _animation(NULL), _idleAnim(NULL), _jumpAnim(NULL), _magic_1_Anim(NULL), _magic_2_Anim(NULL), _smashAnim(NULL), _shootAnim(NULL)
{
    
}

MasterTroll::~MasterTroll()
{
    if (_game)
    {
        _game->release();
    }
    if (_idleAnim) _idleAnim->release();
    if (_jumpAnim) _jumpAnim->release();
    if (_magic_1_Anim) _magic_1_Anim->release();
    if (_magic_2_Anim) _magic_2_Anim->release();
}

bool MasterTroll::init(GameScene* game)
{
    if (!CCNode::init())
    {
        return false;
    }
    
    mLastMagicAnimID = 1;
    
    _game = game;
    
    // The base pad where Object stands
    _base = CCSprite::create("small_dot_red.png");
    addChild(_base);
    
    // The bottom of MT
    mBasePad = CCSprite::create("Characters/master_troll/mt_base.png");
    mBasePad->setAnchorPoint(ccp(0.5f,1));
    _base->addChild(mBasePad);
    
    // The all animations !!!
    _idleAnim = SpriteAnimation::create("Characters/master_troll/mt_idle.plist");
    _idleAnim->retain();
    
    _jumpAnim = SpriteAnimation::create("Characters/master_troll/mt_jumping.plist",false);
    _jumpAnim->retain();
    _jumpAnim->animation->setRestoreOriginalFrame(true);
    
    _magic_1_Anim = SpriteAnimation::create("Characters/master_troll/mt_magic1.plist",false);
    _magic_1_Anim->retain();
    _magic_1_Anim->animation->setRestoreOriginalFrame(true);
    
    _magic_2_Anim = SpriteAnimation::create("Characters/master_troll/mt_magic2.plist",false);
    _magic_2_Anim->retain();
    _magic_2_Anim->animation->setRestoreOriginalFrame(true);
    
    _shootAnim = SpriteAnimation::create("Characters/master_troll/mt_shoot.plist",false);
    _shootAnim->setPosition(ccp(16,-5));
    _shootAnim->retain();
    _shootAnim->animation->setRestoreOriginalFrame(true);
    
    _smashAnim = SpriteAnimation::create("Characters/master_troll/mt_smash.plist",false);
    _smashAnim->setPosition(ccp(-4,66));
    _smashAnim->retain();
    _smashAnim->animation->setRestoreOriginalFrame(true);
    
    // Special stuff for splash anim
    mSplashSprite = CCSprite::create("Characters/master_troll/smash_splash.png");
    mSplashSprite->setPosition(ccp(100,-100));
    mSplashSprite->setVisible(false);
    _base->addChild(mSplashSprite);
    
    mHitGroundTime = -1;
    
    return true;
}



void MasterTroll::update(float delta)
{
    if(_animation != _idleAnim)
    {
        // Check when frames run out - set back to idle !!!
        if(_animation->_action->isDone()){
            // Special case for hit ground with stick !!!
            if(_animation == _smashAnim)
            {
                if(mHitGroundTime == -1)
                {
                    mSplashSprite->setOpacity(255);
                    mSplashSprite->setScale(0);
                    mSplashSprite->setVisible(true);
                    
                    // Set all the anim stuff
                    CCScaleTo* aScaleImage = CCScaleTo::create(0.5f, 1.0f);
                    CCDelayTime* aDelay = CCDelayTime::create(0.25);
                    CCFadeOut* aFadeOut = CCFadeOut::create(0.25f);
                    CCSequence* aSeq = CCSequence::create(aDelay,aFadeOut,NULL);
                    CCSpawn* aSpawn = CCSpawn::create(aSeq,aScaleImage,NULL);
                    mSplashSprite->runAction(aSpawn);
                    
                    // Shake master troll
                    CCMoveTo* aMove1 = CCMoveTo::create(0.075f,ccp(-4,76));
                    CCMoveTo* aMove2 = CCMoveTo::create(0.075f,ccp(-4,66));
                    CCSequence* aSeqMove = CCSequence::create(aMove1,aMove2,NULL);
                    CCRepeat* aRepeat = CCRepeat::create(aSeqMove, 5);
                    
                    _smashAnim->runAction(aRepeat);
                    
                    mHitGroundTime = 0.75f;
                }
                else
                {
                    mHitGroundTime-=delta;
                    if(mHitGroundTime<=0){
                        mHitGroundTime = -1;
                        setAnimation(_idleAnim);
                    }
                }
            }
            else
            {
                setAnimation(_idleAnim);
            }
        }
    }
}

void MasterTroll::setAnimation(SpriteAnimation* animation)
{
    //Glitch Fix!!!
    if(animation->getOpacity()<128){
        animation->setOpacity(255);
    }
    
    if (_animation != animation)
    {
        if (_animation){
            removeChild(_animation);
        }
        
        _animation = animation;
        
        if (_animation){
            addChild(_animation);
        }
    }
}

void MasterTroll::setAnimationByName(const char* theAnimation)
{
    // The checker if all correct and what to play how long?
    if(strcmp(theAnimation,"HitGround") == 0)
    {
        // Lets hit the ground !!!
        setAnimation(_jumpAnim);
        
        // Play sound - whats the timing
        CCDelayTime* aTime = CCDelayTime::create(0.5f);
        CCCallFuncN* aFunc1 = CCCallFuncN::create(this, callfuncN_selector(MasterTroll::OnMasterHitGroundSFX));
        CCSequence* aSeq = CCSequence::create(aTime,aFunc1,NULL);
        runAction(aSeq);
    }
    else if(strcmp(theAnimation,"Magic") == 0)
    {
        // Take random action
        mLastMagicAnimID+=1;
        if(mLastMagicAnimID>2){
            mLastMagicAnimID = 1;
        }
        
        if(mLastMagicAnimID == 1){
            setAnimation(_magic_1_Anim);
        }
        else{
            setAnimation(_magic_2_Anim);
        }
    }
    else if(strcmp(theAnimation,"Shoot") == 0)
    {
        setAnimation(_shootAnim);
    }
    else if(strcmp(theAnimation,"SmashGround") == 0)
    {
        setAnimation(_smashAnim);
        
    }
    
}

void MasterTroll::OnMasterHitGroundSFX(CCNode* sender)
{
    _game->playInGameSound("meteorite_hit_ground");
}

void MasterTroll::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    // Set to default anim !!!
    setAnimation(_idleAnim);
    
    CCNode::onEnter();
}

void MasterTroll::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCNode::onExit();
}

// The new stuff
void MasterTroll::SetMissionStuff(MissionSet theMission)
{
    //Check if circle then use the circle stuff
    setPosition(ccp(200,200));//Some def value for now !!!
    
    
}
