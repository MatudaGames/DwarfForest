//
//  Enemy_Totem.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 09/02/15.
//
//

#include "Enemy_Totem.h"

#include <SimpleAudioEngine.h>
#include "GameScene.h"
#include "AppMacros.h"
#include "Utils.h"
#include "User.h"


USING_NS_CC;
using namespace CocosDenshion;

Enemy_Totem* Enemy_Totem::create(GameScene* game)
{
    Enemy_Totem *pRet = new Enemy_Totem();
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

Enemy_Totem::Enemy_Totem():
_game(NULL)
{
    
}

Enemy_Totem::~Enemy_Totem()
{
    if (_game){_game->release();}
}

bool Enemy_Totem::init(GameScene* game)
{
    if (!CCNode::init())
    {
        return false;
    }
    
    //------------------
    
    _game = game;
    
    _allCreated = false;
    
    _movePoints = CCArray::create();
    _movePoints->retain();
    
    _base = CCSprite::create("beta/Totem1.png");
    _base->setAnchorPoint(ccp(0.5,0.5));
//    _base->setOpacity(128);
    addChild(_base);
    
    mBar_TotemBase = CCSprite::create("small_dot_red.png");
    mBar_TotemBase->setPosition(ccp(0,-_base->getContentSize().height/2));
    mBar_TotemBase->setScaleX(0.0f);     //(0.15);
    mBar_TotemBase->setScaleY(0.0f);     //(0.3);
    
    addChild(mBar_TotemBase,1);
    
    // The background of bar
    CCSprite* aDummy = CCSprite::create("Interfeiss/challenges/daily/progress_days.png");
    mBar_TotemBase->addChild(aDummy);
    
    // The actual progress bar
    mBar_TotemHP = CCSprite::create("Interfeiss/challenges/daily/progress_days_fill.png");
    mBar_TotemHP->setAnchorPoint(ccp(0,0.5f));
    mBar_TotemHP->setPosition(ccp((aDummy->getContentSize().width-mBar_TotemHP->getContentSize().width)/2,aDummy->getContentSize().height/2));
    aDummy->addChild(mBar_TotemHP);
    
    // The progress bar
    mBar_TotemHP->setTextureRect(CCRect(0, 0,
                                              mBar_TotemHP->getTexture()->getContentSize().width*(1.0f),
                                              mBar_TotemHP->getTexture()->getContentSize().height));
    
    // Create the 3 timers !!!
    mDebugTimerLabel = CCLabelTTF::create("111\n222\n100",FONT_SKRANJI,
                                          TITLE_FONT_SIZE*0.5, CCSize(300,240), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
    mDebugTimerLabel->setPosition(ccp(0,_base->getContentSize().height));
    addChild(mDebugTimerLabel);
    
    
    CCSprite* aDummy2 = CCSprite::create("small_dot_red.png");
    addChild(aDummy2);
    
    
    return true;
}



void Enemy_Totem::update(float delta)
{
//        CCLog("Update bee");
    
    // Check if did get to final place !!!
    if(!_allCreated) return;
    
    // Update the totem hp bar !!!
    
    //Constantly checks if has any changes
    if(mCurrentHP != mNeedHP)
    {
        mCurrentHP -= delta;
        if(mCurrentHP<=mNeedHP)
        {
            // Check if mission not completed
            mCurrentHP = mNeedHP;
            if(mCurrentHP == 0)
            {
                // Destroy it
                if(_game->mCurrentMission.Task_type == MissionType_DestroyTotem)
                {
                    // Win Win
                    _game->showWinScreen();
                }
            }
        }
        
        mBar_TotemHP->setTextureRect(CCRect(0, 0,
                                                  mBar_TotemHP->getTexture()->getContentSize().width*((float)mCurrentHP / mHP),
                                                  mBar_TotemHP->getTexture()->getContentSize().height));
    }
    
    // Update other stuff
    if(mBullet_TimeCurrent>0)
    {
        mBullet_TimeCurrent-=delta*_game->getGameSpeed();
        
        if(mBullet_TimeCurrent<=0)
        {
            // Fire some bullet at some dwarf
            mBullet_TimeCurrent = mBullet_Freq;
            
            _game->CreateBulletByType(MASTER_ACTION_BULLET_STRAIGHT, getPositionX(), getPositionY());
        }
    }
    
    //------------------------------------------------------------
    // The shield part
    if(mBubble_TimeCurrent>0)
    {
        mBubble_TimeCurrent-=delta*_game->getGameSpeed();
        
        if(mBubble_TimeCurrent<=0)
        {
            mBubble_ActiveTimeCurrent = mBubble_ActiveTime;
            
            //Create it
            CCSprite* aShield = CCSprite::create("beta/shield_totem.png");
            aShield->setTag(123);
            aShield->setPosition(ccp(aShield->getContentSize().width/2,aShield->getContentSize().height/2));
            _base->addChild(aShield);
        }
    }
    
    if(mBubble_ActiveTimeCurrent>0)
    {
        mBubble_ActiveTimeCurrent-=delta;
        if(mBubble_ActiveTimeCurrent<=0)
        {
            //Remove shield
            mBubble_ActiveTimeCurrent = 0;
            _base->removeChildByTag(123);
            
            mBubble_TimeCurrent = mBubbleShield_Freq;
        }
    }
    
    //------------------------------------------------------------
    // The fire part
    if(mFlame_CurrentTime>0)
    {
        mFlame_CurrentTime-=delta*_game->getGameSpeed();
        if(mFlame_CurrentTime<=0)
        {
            // Wait till end
            mFlame_ActiveTimeCurrent = mFlame_ActiveTime;
            
            // Draw the circle around it
            
            
            //The params of circles
            float precision = 0;
            float cir = 0;
            int mRadius = 0;
            
            precision = 25*0.01;
            cir = 2 * M_PI;
            mRadius = mFlame_Radius;
            
            
            int theCircleX = getPositionX();
            int theCircleY = getPositionY();
            float theCircleWidth = 15*0.1;
            
            float last_x = 0;
            float last_y = 0;
            
            float angleSize = 2*M_PI/25;
            
            for (float a = 0.0f; a < cir; a += precision)
            {
                float x = theCircleX + mRadius * cos(a);
                float y = theCircleY + mRadius/theCircleWidth * sin(a);
                
//                _movePoints->addControlPoint(ccp(x,y-50));
                
                CCSprite* pointsBack = CCSprite::create("beta/target.png");
                pointsBack->setPosition(ccp(x,y-50));
                
                _movePoints->addObject(pointsBack);
                
                //            pointsBack->setOpacity(120);
                
                //Rotate to the next point
                /*
                if(aDidSetAngle){
                    float angle = 360-(atan2(y - last_y, x - last_x) * 180 / M_PI) ;
                    pointsBack->setRotation(angle);
                }
                else{
                    aDidSetAngle = true;
                    pointsBack->setRotation(91);
                }
                */
                
                _game->addChild(pointsBack,1);
                
                last_x = x;
                last_y = y;
            }

        }
    }
    
    if(mFlame_ActiveTimeCurrent>0)
    {
        mFlame_ActiveTimeCurrent-=delta*_game->getGameSpeed();
        if(mFlame_ActiveTimeCurrent<=0)
        {
            //Remove flame
            mFlame_ActiveTimeCurrent = 0;
            
            // Can start count again
            mFlame_CurrentTime = mFlame_Freq;
            
            for(int i=0;i<_movePoints->count();i++)
            {
                _game->removeChild((CCSprite*)_movePoints->objectAtIndex(i));
            }
            
            _movePoints->removeAllObjects();
        }
        else
        {
            // Check if any dwarf near to this radius - remove him
            for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
            {
                Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
                
                if(dwarf != NULL)
                {
                    float theDistance2 = sqrtf((dwarf->getPositionX()-getPositionX())*(dwarf->getPositionX()-getPositionX()) +
                                               (dwarf->getPositionY()-getPositionY())*(dwarf->getPositionY()-getPositionY()));
                    if(theDistance2<mFlame_Radius)
                    {
                        //Game over for dwarf
                        dwarf->removeFromSave();
                    }
                }
                
            }
        }
    }
    
    // Update debug stuff
    mDebugTimerStr.str("");
    mDebugTimerStr.clear();
    
    mDebugTimerStr<<"F:"<<ceilf(mFlame_CurrentTime)<<"\nS:"<<ceilf(mBubble_TimeCurrent)<<"\nB:"<<ceilf(mBullet_TimeCurrent);
    mDebugTimerLabel->setString(mDebugTimerStr.str().c_str());
}

void Enemy_Totem::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    CCNode::onEnter();
}

void Enemy_Totem::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCNode::onExit();
}

void Enemy_Totem::OnShowedUp()
{
    //Show the hp bar
    CCDelayTime* aDelay = CCDelayTime::create(0.2f);
    CCScaleTo* aScaleAction = CCScaleTo::create(0.3f, 0.15f, 0.3f);
    CCEaseBackOut* aEase = CCEaseBackOut::create(aScaleAction);
    CCSequence* aSeq = CCSequence::create(aDelay,aEase,NULL);
    mBar_TotemBase->runAction(aSeq);
}

// The new stuff
void Enemy_Totem::SetMissionStuff(int theX,int theY,int HP,int BulletEvent,int BulletFreq,int BubbleShieldEvent,int BubbleShieldFreq,int BubbleShieldActive,int FlameFreq,int FlameRadius,int FlameActive)
{
    //Check if circle then use the circle stuff
    setPosition(ccp(theX,theY));//Some def value for now !!!
    
    mCurrentHP = mHP = mNeedHP = HP;
    
    mBullet_Event = BulletEvent;
    mBullet_Freq = mBullet_TimeCurrent = BulletFreq;
    
    mBubble_ActiveTimeCurrent = 0;
    mBubble_ActiveTime = BubbleShieldActive;
    
    mBubble_TimeCurrent = mBubbleShield_Freq = BubbleShieldFreq;
    
    mFlame_Freq = mFlame_CurrentTime = FlameFreq;
    mFlame_Radius = FlameRadius;
    
    mFlame_ActiveTime = FlameActive;
    mFlame_ActiveTimeCurrent = 0;
    
    
    
    OnShowedUp();
    
    _allCreated = true;
}

