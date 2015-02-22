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
    
    mQuad_1_AngleWidth = 0;
    mQuad_2_AngleWidth = 0;
    mQuad_3_AngleWidth = 0;
    mQuad_4_AngleWidth = 0;
    
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
    mDebugTimerLabel->setPosition(ccp(16,20));
    addChild(mDebugTimerLabel);
    
    // Create the 4 sides for dwarf check?
    mQuad_1 = CCDrawNode::create();this->addChild( mQuad_1 );craeteQuad(1, 100, 0, 90);
    mQuad_2 = CCDrawNode::create();this->addChild( mQuad_2 );craeteQuad(2, 100, 0, 90);
    mQuad_3 = CCDrawNode::create();this->addChild( mQuad_3 );craeteQuad(3, 100, 0, 90);
    mQuad_4 = CCDrawNode::create();this->addChild( mQuad_4 );craeteQuad(4, 100, 0, 90);
    
    mPassive_Zone = CCDrawNode::create();this->addChild( mPassive_Zone );
    mPassive_Zone->setVisible(false);
    
    return true;
}

void Enemy_Totem::AttackFromPlayer(cocos2d::CCPoint position,int damage)
{
    int aAttackQuadID = 0;
    
    if(position.x > getPositionX())
    {
        // Can be only quad 1 or 2
        if(position.y>=getPositionY())
        {
            aAttackQuadID = 1;
        }
        else
        {
            aAttackQuadID = 2;
        }
    }
    else
    {
        if(position.y>=getPositionY())
        {
            aAttackQuadID = 4;
        }
        else
        {
            aAttackQuadID = 3;
        }
    }
    
    CCLog("Attack comes in quad [%i]",aAttackQuadID);
    
    float finalDamage = damage;
    
    // Now check if there is not damage shield any !!!
    if(aAttackQuadID == 1)
    {
        for(int i=0;i<mQuad_Vector_1.size();i++)
        {
            if(mQuad_Vector_1[i].type == 1)
            {
                // Check if this event is active
                if(mQuad_Vector_1[i].current_time_active>0)
                {
                    //What shield is on?
                    if(mQuad_Vector_1[i].event_type == 2){ finalDamage = float(damage)*0.5;}// Half damage taken
                    else if(mQuad_Vector_1[i].event_type == 1){finalDamage = 0;}// No damage
                    break;
                }
            }
        }
    }
    else if(aAttackQuadID == 2)
    {
        for(int i=0;i<mQuad_Vector_2.size();i++)
        {
            if(mQuad_Vector_2[i].type == 1)
            {
                // Check if this event is active
                if(mQuad_Vector_2[i].current_time_active>0)
                {
                    //What shield is on?
                    if(mQuad_Vector_2[i].event_type == 2){ finalDamage = float(damage)*0.5;}// Half damage taken
                    else if(mQuad_Vector_2[i].event_type == 1){finalDamage = 0;}// No damage
                    break;
                }
            }
        }
    }
    else if(aAttackQuadID == 3)
    {
        for(int i=0;i<mQuad_Vector_3.size();i++)
        {
            if(mQuad_Vector_3[i].type == 1)
            {
                // Check if this event is active
                if(mQuad_Vector_3[i].current_time_active>0)
                {
                    //What shield is on?
                    if(mQuad_Vector_3[i].event_type == 2){ finalDamage = float(damage)*0.5;}// Half damage taken
                    else if(mQuad_Vector_3[i].event_type == 1){finalDamage = 0;}// No damage
                    break;
                }
            }
        }
    }
    else if(aAttackQuadID == 4)
    {
        for(int i=0;i<mQuad_Vector_4.size();i++)
        {
            if(mQuad_Vector_4[i].type == 1)
            {
                // Check if this event is active
                if(mQuad_Vector_4[i].current_time_active>0)
                {
                    //What shield is on?
                    if(mQuad_Vector_4[i].event_type == 2){ finalDamage = float(damage)*0.5;}// Half damage taken
                    else if(mQuad_Vector_4[i].event_type == 1){finalDamage = 0;}// No damage
                    break;
                }
            }
        }
    }
    
    // Take the damage now
    mNeedHP-=finalDamage;
}

void Enemy_Totem::craeteQuad(int theID,int theWidth,int theType,int theAngle)
{
    // Draw the conus from settings
    std::vector<CCPoint> points;
    points.push_back(ccp(0,0));
    
    // ----------------------------
    int coneWidth = theAngle;
    int coneRadius = theWidth;
    
    for (float ii = 0; ii < coneWidth; ii += 0.1)
    {
        points.push_back(ccp(0 + coneRadius * cos(ii * (M_PI / 180)), 0 + coneRadius * sin(ii * (M_PI / 180))));
    }
    
    points.push_back(ccp(0,0));
    
    CCPoint* pPoints = new CCPoint[points.size()];
    
    ccColor4F pShieldColor;
    
    if(theType == 0)// The brutal shield
    {
        pShieldColor = ccc4f(0, 0, 0, 0.05f);
    }
    else if(theType == 1)// The brutal shield
    {
        pShieldColor = ccc4FFromccc4B(ccc4(0, 0, 255, 128));
    }
    else if(theType == 2)// The half brutal shield
    {
        pShieldColor = ccc4FFromccc4B(ccc4(120, 200, 255, 64));
    }
    else if(theType == 3)// The burn zone
    {
        pShieldColor = ccc4FFromccc4B(ccc4(255, 0, 0, 64));
    }
    
    for (unsigned int i = 0; i < points.size(); ++i)
    {
        pPoints[i].x = points[i].x;
        pPoints[i].y = points[i].y;
    }
    
    if(theID == 1)
    {
        mQuad_1_AngleWidth = theAngle;
        mQuad_1_Size = theWidth;
        mQuad_1->clear();
        mQuad_1->drawPolygon(pPoints, points.size(), pShieldColor, 2, ccc4f(0, 0, 0, 0.1) );
        mQuad_1->setRotation(0);
    }
    else if(theID == 2)
    {
        mQuad_2_AngleWidth = theAngle;
        mQuad_2_Size = theWidth;
        mQuad_2->clear();
        mQuad_2->drawPolygon(pPoints, points.size(), pShieldColor, 2, ccc4f(0, 0, 0, 0.1) );
        mQuad_2->setRotation(90);
    }
    else if(theID == 3)
    {
        mQuad_3_AngleWidth = theAngle;
        mQuad_3_Size = theWidth;
        mQuad_3->clear();
        mQuad_3->drawPolygon(pPoints, points.size(), pShieldColor, 2, ccc4f(0, 0, 0, 0.1) );
        mQuad_3->setRotation(180);
    }
    else if(theID == 4)
    {
        mQuad_4_AngleWidth = theAngle;
        mQuad_4_Size = theWidth;
        mQuad_4->clear();
        mQuad_4->drawPolygon(pPoints, points.size(), pShieldColor, 2, ccc4f(0, 0, 0, 0.1) );
        mQuad_4->setRotation(270);
    }
    else if(theID == 5)// Passive zone
    {
        mPassive_Zone->clear();
        mPassive_Zone->drawPolygon(pPoints, points.size(), pShieldColor, 2, ccc4f(0, 0, 0, 0.1) );
        mPassive_Zone->setRotation(0);
    }
}

int Enemy_Totem::collideAtPoint(cocos2d::CCPoint point)
{
    //Check on what quad does collide !!!
    int aCollideQuad = -1;
    
    int coneAngleOffset = 0;
    int coneAngle = 0;
    int dx = 0;
    int dy = 0;
    int distanceFromTroll = 0;
    int angle = 0;
    
    for(int i=0;i<4;i++)
    {
        if(i == 0){
            coneAngleOffset = -fmod(mQuad_1->getRotation(), 360);
            coneAngle = mQuad_1_AngleWidth + coneAngleOffset;
        }
        else if(i == 1){
            coneAngleOffset = -fmod(mQuad_2->getRotation(), 360);
            coneAngle = mQuad_2_AngleWidth + coneAngleOffset;
        }
        else if(i == 2){
            coneAngleOffset = -fmod(mQuad_3->getRotation(), 360);
            coneAngle = mQuad_3_AngleWidth + coneAngleOffset;
        }
        else if(i == 3){
            coneAngleOffset = -fmod(mQuad_4->getRotation(), 360);
            coneAngle = mQuad_4_AngleWidth + coneAngleOffset;
        }
        
        dx = point.x - getPositionX();
        dy = point.y - getPositionY();
        
        distanceFromTroll = sqrt(pow(dx, 2) + pow(dy, 2));
        angle = atan2(dy, dx) * (180 / M_PI);
        
        if(i == 0)
        {
            if(angle>0 && fmod(mQuad_1->getRotation(), 360)>=mQuad_1_Size){angle-=360;}
            if(distanceFromTroll <= mQuad_1_Size && (angle >= coneAngleOffset && angle <= coneAngle)) aCollideQuad = 1;
        }
        else if(i == 1)
        {
            if(angle>0 && fmod(mQuad_2->getRotation(), 360)>=mQuad_2_Size){angle-=360;}
            if(distanceFromTroll <= mQuad_2_Size && (angle >= coneAngleOffset && angle <= coneAngle)) aCollideQuad = 2;
        }
        else if(i == 2)
        {
            if(angle>0 && fmod(mQuad_3->getRotation(), 360)>=mQuad_3_Size){angle-=360;}
            if(distanceFromTroll <= mQuad_3_Size && (angle >= coneAngleOffset && angle <= coneAngle)) aCollideQuad = 3;
        }
        else if(i == 3)
        {
            if(angle>0 && fmod(mQuad_4->getRotation(), 360)>=mQuad_4_Size){angle-=360;}
            if(distanceFromTroll <= mQuad_4_Size && (angle >= coneAngleOffset && angle <= coneAngle)) aCollideQuad = 4;
        }
    }
    
    //CCLOG("Collided at quad :%i",aCollideQuad);
    
    return aCollideQuad;
}

bool Enemy_Totem::collideAtPassive(cocos2d::CCPoint point)
{
    float theDistance2 = sqrtf((getPositionX()-point.x)*(getPositionX()-point.x) +
                               (getPositionY()-point.y)*(getPositionY()-point.y));
    if(theDistance2<=mPassive_Radius){
        return true;
    }
    
    return false;
}

void Enemy_Totem::update(float delta)
{
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
    
    // Lets use the new stuff
    UpdateQuadSystem(delta);
    
    if(mPassive_Zone->isVisible())
    {
        // Check if any dwarf near to this radius - remove him
        for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
        {
            Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
            
            if(dwarf != NULL && dwarf->_dwarfIsRemoving == false)// Check for current quad damage
            {
                if(collideAtPassive(dwarf->getPosition()) == true)
                {
                    // Set slow down
                    if(mPassive_EventID == 1){
                        dwarf->mTotemSlow = 0.6;
                    }
                    else if(mPassive_EventID == 2){
                        // The freezer
                        dwarf->mTotemZoneIn+=delta*_game->getGameSpeed();
                        if(dwarf->mTotemZoneIn>=5 && dwarf->_effect == NULL)
                        {
                            dwarf->mTotemZoneIn = 0;//Reset
                            //Check if will frreeze dwarf
                            int theFreezePoss = rand()%100;
                            if(theFreezePoss<=30)
                            {
                                //Freeze it !!!
                                _game->FreezeDwarfTotal(dwarf);
                            }
                        }
                    }
                }
                else
                {
                    dwarf->mTotemSlow = 1;
                    dwarf->mTotemZoneIn = 0;
                }
            }
        }
    }
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

// The new mission stuff !!!
void Enemy_Totem::SetNewMissionStuff(MissionSet totemInfo)
{
    // Reset all stuff
    mCurrentHP = mHP = mNeedHP = totemInfo.TOTEM_HP;
    
    // The passive read
    mPassive_EventID = totemInfo.TOTEM_PASSIVE_TYPE;
    mPassive_Radius = totemInfo.TOTEM_RADIUS;
    
    // The position
    setPosition(ccp(totemInfo.TOTEM_x,totemInfo.TOTEM_y));//Some def value for now !!!
    
    if(mPassive_Radius>0 && mPassive_EventID>0)
    {
        // Create the passive stuff !!!
        craeteQuad(5, mPassive_Radius, 3, 360);
        mPassive_Zone->setVisible(true);
    }
    
    //..........................................................................................
    // The quad read !!!
    if(totemInfo.TOTEM_QUAD_1.size()>0)
    {
        // Set all totem 1 info for actions
        for(int i=0;i<totemInfo.TOTEM_QUAD_1.size();i++)
        {
            Quad_Action quad_info = *new Quad_Action();
            
            quad_info.type = totemInfo.TOTEM_QUAD_1[i].type;
            
            if(quad_info.type == 1)//The shield stuff
            {
                quad_info.active_time = totemInfo.TOTEM_QUAD_1[i].active_time;
                quad_info.activate_time = totemInfo.TOTEM_QUAD_1[i].activate_time;
                
                // What type of shield is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_1[i].event_type;
                
                // The timer for activation
                quad_info.current_time_till_active = quad_info.active_time;// Start from 0
                quad_info.current_time_active = quad_info.activate_time;// Start from 0
            }
            else if(quad_info.type == 2)
            {
                quad_info.active_time = totemInfo.TOTEM_QUAD_1[i].active_time;
                quad_info.activate_time = totemInfo.TOTEM_QUAD_1[i].activate_time;
                
                // What type of shield is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_1[i].event_type;
                
                // Whats the radius for this one !!!
                quad_info.deadzone_radius = totemInfo.TOTEM_QUAD_1[i].deadzone_radius;
                
                // The timer for activation
                quad_info.current_time_till_active = quad_info.active_time;// Start from 0
                quad_info.current_time_active = quad_info.activate_time;// Start from 0
            }
            else if(quad_info.type == 3)
            {
                // This one is a bit different - it counts down - and then does one action - and starts again count
                quad_info.activate_time = totemInfo.TOTEM_QUAD_1[i].activate_time;
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                
                // What type of projecttile is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_1[i].event_type;
                
                // The bullet stuff
                quad_info.bullet_distance = totemInfo.TOTEM_QUAD_1[i].bullet_distance;
                quad_info.bullet_amount = totemInfo.TOTEM_QUAD_1[i].bullet_amount;
                
                quad_info.flame_radius = totemInfo.TOTEM_QUAD_1[i].flame_radius;
                quad_info.flame_active_time = totemInfo.TOTEM_QUAD_1[i].flame_active_time;
                quad_info.flame_rotate_speed = totemInfo.TOTEM_QUAD_1[i].flame_rotate_speed;
            }
            
            mQuad_Vector_1.push_back(quad_info);
        }
    }
    
    //..........................................................................................
    // The Quad 2
    
    if(totemInfo.TOTEM_QUAD_2.size()>0)
    {
        // Set all totem 1 info for actions
        for(int i=0;i<totemInfo.TOTEM_QUAD_2.size();i++)
        {
            Quad_Action quad_info = *new Quad_Action();
            
            quad_info.type = totemInfo.TOTEM_QUAD_2[i].type;
            
            if(quad_info.type == 1)//The shield stuff
            {
                quad_info.active_time = totemInfo.TOTEM_QUAD_2[i].active_time;
                quad_info.activate_time = totemInfo.TOTEM_QUAD_2[i].activate_time;
                
                // What type of shield is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_2[i].event_type;
                
                // The timer for activation
                quad_info.current_time_till_active = quad_info.active_time;// Start from 0
                quad_info.current_time_active = quad_info.activate_time;// Start from 0
            }
            else if(quad_info.type == 2)
            {
                quad_info.active_time = totemInfo.TOTEM_QUAD_2[i].active_time;
                quad_info.activate_time = totemInfo.TOTEM_QUAD_2[i].activate_time;
                
                // What type of shield is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_2[i].event_type;
                
                // Whats the radius for this one !!!
                quad_info.deadzone_radius = totemInfo.TOTEM_QUAD_2[i].deadzone_radius;
                
                // The timer for activation
                quad_info.current_time_till_active = quad_info.active_time;// Start from 0
                quad_info.current_time_active = quad_info.activate_time;// Start from 0
            }
            else if(quad_info.type == 3)
            {
                // This one is a bit different - it counts down - and then does one action - and starts again count
                quad_info.activate_time = totemInfo.TOTEM_QUAD_2[i].activate_time;
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                
                // What type of projecttile is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_2[i].event_type;
                
                // The bullet stuff
                quad_info.bullet_distance = totemInfo.TOTEM_QUAD_2[i].bullet_distance;
                quad_info.bullet_amount = totemInfo.TOTEM_QUAD_2[i].bullet_amount;
                
                quad_info.flame_radius = totemInfo.TOTEM_QUAD_2[i].flame_radius;
                quad_info.flame_active_time = totemInfo.TOTEM_QUAD_2[i].flame_active_time;
                quad_info.flame_rotate_speed = totemInfo.TOTEM_QUAD_2[i].flame_rotate_speed;
            }
            
            mQuad_Vector_2.push_back(quad_info);
        }
    }
    
    //..........................................................................................
    // The Quad 3
    
    if(totemInfo.TOTEM_QUAD_3.size()>0)
    {
        // Set all totem 1 info for actions
        for(int i=0;i<totemInfo.TOTEM_QUAD_3.size();i++)
        {
            Quad_Action quad_info = *new Quad_Action();
            
            quad_info.type = totemInfo.TOTEM_QUAD_3[i].type;
            
            if(quad_info.type == 1)//The shield stuff
            {
                quad_info.active_time = totemInfo.TOTEM_QUAD_3[i].active_time;
                quad_info.activate_time = totemInfo.TOTEM_QUAD_3[i].activate_time;
                
                // What type of shield is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_3[i].event_type;
                
                // The timer for activation
                quad_info.current_time_till_active = quad_info.active_time;// Start from 0
                quad_info.current_time_active = quad_info.activate_time;// Start from 0
            }
            else if(quad_info.type == 2)
            {
                quad_info.active_time = totemInfo.TOTEM_QUAD_3[i].active_time;
                quad_info.activate_time = totemInfo.TOTEM_QUAD_3[i].activate_time;
                
                // What type of shield is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_3[i].event_type;
                
                // Whats the radius for this one !!!
                quad_info.deadzone_radius = totemInfo.TOTEM_QUAD_3[i].deadzone_radius;
                
                // The timer for activation
                quad_info.current_time_till_active = quad_info.active_time;// Start from 0
                quad_info.current_time_active = quad_info.activate_time;// Start from 0
            }
            else if(quad_info.type == 3)
            {
                // This one is a bit different - it counts down - and then does one action - and starts again count
                quad_info.activate_time = totemInfo.TOTEM_QUAD_3[i].activate_time;
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                
                // What type of projecttile is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_3[i].event_type;
                
                // The bullet stuff
                quad_info.bullet_distance = totemInfo.TOTEM_QUAD_3[i].bullet_distance;
                quad_info.bullet_amount = totemInfo.TOTEM_QUAD_3[i].bullet_amount;
                
                quad_info.flame_radius = totemInfo.TOTEM_QUAD_3[i].flame_radius;
                quad_info.flame_active_time = totemInfo.TOTEM_QUAD_3[i].flame_active_time;
                quad_info.flame_rotate_speed = totemInfo.TOTEM_QUAD_3[i].flame_rotate_speed;
            }
            
            mQuad_Vector_3.push_back(quad_info);
        }
    }
    
    //..........................................................................................
    // The Quad 4
    
    if(totemInfo.TOTEM_QUAD_4.size()>0)
    {
        // Set all totem 1 info for actions
        for(int i=0;i<totemInfo.TOTEM_QUAD_4.size();i++)
        {
            Quad_Action quad_info = *new Quad_Action();
            
            quad_info.type = totemInfo.TOTEM_QUAD_4[i].type;
            
            if(quad_info.type == 1)//The shield stuff
            {
                quad_info.active_time = totemInfo.TOTEM_QUAD_4[i].active_time;
                quad_info.activate_time = totemInfo.TOTEM_QUAD_4[i].activate_time;
                
                // What type of shield is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_4[i].event_type;
                
                // The timer for activation
                quad_info.current_time_till_active = quad_info.active_time;// Start from 0
                quad_info.current_time_active = quad_info.activate_time;// Start from 0
            }
            else if(quad_info.type == 2)
            {
                quad_info.active_time = totemInfo.TOTEM_QUAD_4[i].active_time;
                quad_info.activate_time = totemInfo.TOTEM_QUAD_4[i].activate_time;
                
                // What type of shield is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_4[i].event_type;
                
                // Whats the radius for this one !!!
                quad_info.deadzone_radius = totemInfo.TOTEM_QUAD_4[i].deadzone_radius;
                
                // The timer for activation
                quad_info.current_time_till_active = quad_info.active_time;// Start from 0
                quad_info.current_time_active = quad_info.activate_time;// Start from 0
            }
            else if(quad_info.type == 3)
            {
                // This one is a bit different - it counts down - and then does one action - and starts again count
                quad_info.activate_time = totemInfo.TOTEM_QUAD_4[i].activate_time;
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                
                // What type of projecttile is this
                quad_info.event_type = totemInfo.TOTEM_QUAD_4[i].event_type;
                
                // The bullet stuff
                quad_info.bullet_distance = totemInfo.TOTEM_QUAD_4[i].bullet_distance;
                quad_info.bullet_amount = totemInfo.TOTEM_QUAD_4[i].bullet_amount;
                
                quad_info.flame_radius = totemInfo.TOTEM_QUAD_4[i].flame_radius;
                quad_info.flame_active_time = totemInfo.TOTEM_QUAD_4[i].flame_active_time;
                quad_info.flame_rotate_speed = totemInfo.TOTEM_QUAD_4[i].flame_rotate_speed;
            }
            
            mQuad_Vector_4.push_back(quad_info);
        }
    }
    
    //..........................................................................................
    
    OnShowedUp();
    
    _allCreated = true;
}

// The quad functional update
void Enemy_Totem::UpdateQuadSystem(float delta)
{
    // The 1st quad
    if(mQuad_Vector_1.size()>0)
    {
        // Update quad 1
        for(int i=0;i<mQuad_Vector_1.size();i++)
        {
            // Check the shields
            if(mQuad_Vector_1[i].type == 1)
            {
                if(mQuad_Vector_1[i].current_time_active>0)
                {
                    mQuad_Vector_1[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_1[i].current_time_active<=0)
                    {
                        mQuad_Vector_1[i].current_time_active = 0;
                        // Disable shield
                        craeteQuad(1, 100, 0, 90);
                    }
                }
                else
                {
                    mQuad_Vector_1[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_1[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_1[i].current_time_till_active = mQuad_Vector_1[i].activate_time; // Reset back timer
                        // Enable shield
                        mQuad_Vector_1[i].current_time_active = mQuad_Vector_1[i].active_time;
                        if(mQuad_Vector_1[i].event_type == 1)
                        {
                            // basic shield
                            craeteQuad(1, 100, 1, 90);
                        }
                        else if(mQuad_Vector_1[i].event_type == 2)
                        {
                            // some different shield
                            craeteQuad(1, 100, 2, 90);
                        }
                    }
                }
                
            }
            else if(mQuad_Vector_1[i].type == 2) // The deadzone stuff
            {
                if(mQuad_Vector_1[i].current_time_active>0)
                {
                    // Check if any dwarf near to this radius - remove him
                    for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
                    {
                        Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
                        
                        if(dwarf != NULL && dwarf->_dwarfIsRemoving == false)// Check for current quad damage
                        {
                            if(collideAtPoint(dwarf->getPosition()) == 1)
                            {
                                dwarf->_dwarfIsRemoving = true;
                                // Game over for dwarf - burn or do other damage !!
                                
                                // Add particle for FX
                                CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/BurnFx.plist");
                                p->setPosition(dwarf->getPosition());
                                p->setAutoRemoveOnFinish(true);
                                _game->addChild(p);
                                
                                dwarf->removeFromSave();
                            }
                        }
                    }
                    
                    mQuad_Vector_1[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_1[i].current_time_active<=0)
                    {
                        mQuad_Vector_1[i].current_time_active = 0;
                        // Disable deadzone
                        craeteQuad(1, 100, 0, 90);
                    }
                }
                else
                {
                    mQuad_Vector_1[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_1[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_1[i].current_time_till_active = mQuad_Vector_1[i].activate_time; // Reset back timer
                        // Enable deadzone
                        mQuad_Vector_1[i].current_time_active = mQuad_Vector_1[i].active_time;
                        
                        // Create by the radius
                        craeteQuad(1, mQuad_Vector_1[i].deadzone_radius, 3, 90);
                    }
                }
            }
            else if(mQuad_Vector_1[i].type == 3)
            {
                // The creazy bullets and stuff like that
                
                // Updates only if flamethrower is active
                if(mQuad_Vector_1[i].current_time_active>0)
                {
                    // Check if any dwarf near to this radius - remove him
                    for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
                    {
                        Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
                        
                        if(dwarf != NULL && dwarf->_dwarfIsRemoving == false)// Check for current quad damage
                        {
                            if(collideAtPoint(dwarf->getPosition()) == 1)
                            {
                                dwarf->_dwarfIsRemoving = true;
                                // Game over for dwarf - burn or do other damage !!
                                
                                // Add particle for FX
                                CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/BurnFx.plist");
                                p->setPosition(dwarf->getPosition());
                                p->setAutoRemoveOnFinish(true);
                                _game->addChild(p);
                                
                                dwarf->removeFromSave();
                            }
                        }
                    }
                    
                    mQuad_Vector_1[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_1[i].current_time_active<=0)
                    {
                        mQuad_1->stopAllActions();
                        mQuad_Vector_1[i].current_time_active = 0;
                        // Disable deadzone
                        mQuad_1->removeAllChildren();//Remove the fire stuff
                        craeteQuad(1, 100, 0, 90);
                    }
                }
                else
                {
                    // Check when will do something
                    mQuad_Vector_1[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_1[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_1[i].current_time_till_active = mQuad_Vector_1[i].activate_time; // Reset back timer
                        
                        if(mQuad_Vector_1[i].event_type == 1)// The bullet fire !!!
                        {
                            // Bumb bum fire bullets
                            int aBulletsAngle = 90/(1+mQuad_Vector_1[i].bullet_amount);
                            for(int b=0;b<mQuad_Vector_1[i].bullet_amount;b++)
                            {
                                CreateSpecialBullet(MASTER_ACTION_BULLET_STRAIGHT,getPositionX(),getPositionY(), aBulletsAngle*(b+1), mQuad_Vector_1[i].bullet_distance);
                            }
                        }
                        else if(mQuad_Vector_1[i].event_type == 2)// The flamethrower
                        {
                            mQuad_Vector_1[i].current_time_active = mQuad_Vector_1[i].flame_active_time;
                            craeteQuad(1, 100, 3, mQuad_Vector_1[i].flame_radius);
                            
                            for(int p=0;p<5;p++)
                            {
                                CCParticleSystemQuad* fx = CCParticleSystemQuad::create("Particles/FlameDot_Fx.plist");
                                fx->setPosition(ccp(p*20,0));
                                fx->setAutoRemoveOnFinish(true);
                                mQuad_1->addChild(fx);
                            }
                            
                            // Reset
                            mQuad_1->stopAllActions();
                            mQuad_1->setRotation(-45);// Reset rotation to default? or mid?
                            
                            CCRotateTo* aRotate = CCRotateTo::create(mQuad_Vector_1[i].flame_rotate_speed, (-90+mQuad_Vector_1[i].flame_radius*0.5));
                            CCRotateTo* aRotateBack = CCRotateTo::create(mQuad_Vector_1[i].flame_rotate_speed,mQuad_Vector_1[i].flame_radius*0.5);
                            CCSequence* aRotateSeq = CCSequence::create(aRotate,aRotateBack,NULL);
                            CCRepeatForever* aRotateRepeat = CCRepeatForever::create(aRotateSeq);
                            
                            mQuad_1->runAction(aRotateRepeat);
                        }
                    }
                }
                
            }
        }
    }
    
    // 2nd quad
    if(mQuad_Vector_2.size()>0)
    {
        // Update quad 1
        for(int i=0;i<mQuad_Vector_2.size();i++)
        {
            // Check the shields
            if(mQuad_Vector_2[i].type == 1)
            {
                if(mQuad_Vector_2[i].current_time_active>0)
                {
                    mQuad_Vector_2[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_2[i].current_time_active<=0)
                    {
                        mQuad_Vector_2[i].current_time_active = 0;
                        // Disable shield
                        craeteQuad(2, 100, 0, 90);
                    }
                }
                else
                {
                    mQuad_Vector_2[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_2[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_2[i].current_time_till_active = mQuad_Vector_2[i].activate_time; // Reset back timer
                        // Enable shield
                        mQuad_Vector_2[i].current_time_active = mQuad_Vector_2[i].active_time;
                        if(mQuad_Vector_2[i].event_type == 1)
                        {
                            // basic shield
                            craeteQuad(2, 100, 1, 90);
                        }
                        else if(mQuad_Vector_2[i].event_type == 2)
                        {
                            // some different shield
                            craeteQuad(2, 100, 2, 90);
                        }
                    }
                }
                
            }
            else if(mQuad_Vector_2[i].type == 2) // The deadzone stuff
            {
                if(mQuad_Vector_2[i].current_time_active>0)
                {
                    // Check if any dwarf near to this radius - remove him
                    for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
                    {
                        Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
                        
                        if(dwarf != NULL && dwarf->_dwarfIsRemoving == false)// Check for current quad damage
                        {
                            if(collideAtPoint(dwarf->getPosition()) == 2)
                            {
                                dwarf->_dwarfIsRemoving = true;
                                // Game over for dwarf - burn or do other damage !!
                                
                                // Add particle for FX
                                CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/BurnFx.plist");
                                p->setPosition(dwarf->getPosition());
                                p->setAutoRemoveOnFinish(true);
                                _game->addChild(p);
                                
                                dwarf->removeFromSave();
                            }
                        }
                    }
                    
                    mQuad_Vector_2[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_2[i].current_time_active<=0)
                    {
                        mQuad_Vector_2[i].current_time_active = 0;
                        // Disable deadzone
                        craeteQuad(2, 100, 0, 90);
                    }
                }
                else
                {
                    mQuad_Vector_2[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_2[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_2[i].current_time_till_active = mQuad_Vector_2[i].activate_time; // Reset back timer
                        // Enable deadzone
                        mQuad_Vector_2[i].current_time_active = mQuad_Vector_2[i].active_time;
                        
                        // Create by the radius
                        craeteQuad(2, mQuad_Vector_2[i].deadzone_radius, 3, 90);
                    }
                }
            }
            else if(mQuad_Vector_2[i].type == 3)
            {
                // The creazy bullets and stuff like that
                
                // Updates only if flamethrower is active
                if(mQuad_Vector_2[i].current_time_active>0)
                {
                    // Check if any dwarf near to this radius - remove him
                    for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
                    {
                        Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
                        
                        if(dwarf != NULL && dwarf->_dwarfIsRemoving == false)// Check for current quad damage
                        {
                            if(collideAtPoint(dwarf->getPosition()) == 2)
                            {
                                dwarf->_dwarfIsRemoving = true;
                                // Game over for dwarf - burn or do other damage !!
                                
                                // Add particle for FX
                                CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/BurnFx.plist");
                                p->setPosition(dwarf->getPosition());
                                p->setAutoRemoveOnFinish(true);
                                _game->addChild(p);
                                
                                dwarf->removeFromSave();
                            }
                        }
                    }
                    
                    mQuad_Vector_2[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_2[i].current_time_active<=0)
                    {
                        mQuad_2->stopAllActions();
                        mQuad_Vector_2[i].current_time_active = 0;
                        // Disable deadzone
                        mQuad_2->removeAllChildren();//Remove the fire stuff
                        craeteQuad(2, 100, 0, 90);
                    }
                }
                else
                {
                    // Check when will do something
                    mQuad_Vector_2[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_2[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_2[i].current_time_till_active = mQuad_Vector_2[i].activate_time; // Reset back timer
                        
                        if(mQuad_Vector_2[i].event_type == 1)// The bullet fire !!!
                        {
                            // Bumb bum fire bullets
                            int aBulletsAngle = 90/(1+mQuad_Vector_2[i].bullet_amount);
                            for(int b=0;b<mQuad_Vector_2[i].bullet_amount;b++)
                            {
                                CreateSpecialBullet(MASTER_ACTION_BULLET_STRAIGHT,getPositionX(),getPositionY(), aBulletsAngle*(b+1)-90, mQuad_Vector_2[i].bullet_distance);
                            }
                        }
                        else if(mQuad_Vector_2[i].event_type == 2)// The flamethrower
                        {
                            mQuad_Vector_2[i].current_time_active = mQuad_Vector_2[i].flame_active_time;
                            craeteQuad(2, 100, 3, mQuad_Vector_2[i].flame_radius);
                            
                            for(int p=0;p<5;p++)
                            {
                                CCParticleSystemQuad* fx = CCParticleSystemQuad::create("Particles/FlameDot_Fx.plist");
                                fx->setPosition(ccp(p*20,0));
                                fx->setAutoRemoveOnFinish(true);
                                mQuad_2->addChild(fx);
                            }
                            
                            // Reset
                            mQuad_2->stopAllActions();
                            mQuad_2->setRotation(90);// Reset rotation to default? or mid?
                            
                            CCRotateTo* aRotate = CCRotateTo::create(mQuad_Vector_2[i].flame_rotate_speed,0);
                            CCRotateTo* aRotateBack = CCRotateTo::create(mQuad_Vector_2[i].flame_rotate_speed,90);
                            CCSequence* aRotateSeq = CCSequence::create(aRotate,aRotateBack,NULL);
                            CCRepeatForever* aRotateRepeat = CCRepeatForever::create(aRotateSeq);
                            
                            mQuad_2->runAction(aRotateRepeat);
                        }
                    }
                }
                
            }
        }
    }
    
    // The 3rd quad
    if(mQuad_Vector_3.size()>0)
    {
        // Update quad 1
        for(int i=0;i<mQuad_Vector_3.size();i++)
        {
            // Check the shields
            if(mQuad_Vector_3[i].type == 1)
            {
                if(mQuad_Vector_3[i].current_time_active>0)
                {
                    mQuad_Vector_3[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_3[i].current_time_active<=0)
                    {
                        mQuad_Vector_3[i].current_time_active = 0;
                        // Disable shield
                        craeteQuad(3, 100, 0, 90);
                    }
                }
                else
                {
                    mQuad_Vector_3[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_3[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_3[i].current_time_till_active = mQuad_Vector_3[i].activate_time; // Reset back timer
                        // Enable shield
                        mQuad_Vector_3[i].current_time_active = mQuad_Vector_3[i].active_time;
                        if(mQuad_Vector_3[i].event_type == 1)
                        {
                            // basic shield
                            craeteQuad(3, 100, 1, 90);
                        }
                        else if(mQuad_Vector_3[i].event_type == 2)
                        {
                            // some different shield
                            craeteQuad(3, 100, 2, 90);
                        }
                    }
                }
                
            }
            else if(mQuad_Vector_3[i].type == 2) // The deadzone stuff
            {
                if(mQuad_Vector_3[i].current_time_active>0)
                {
                    // Check if any dwarf near to this radius - remove him
                    for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
                    {
                        Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
                        
                        if(dwarf != NULL && dwarf->_dwarfIsRemoving == false)// Check for current quad damage
                        {
                            if(collideAtPoint(dwarf->getPosition()) == 3)
                            {
                                dwarf->_dwarfIsRemoving = true;
                                // Game over for dwarf - burn or do other damage !!
                                
                                // Add particle for FX
                                CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/BurnFx.plist");
                                p->setPosition(dwarf->getPosition());
                                p->setAutoRemoveOnFinish(true);
                                _game->addChild(p);
                                
                                dwarf->removeFromSave();
                            }
                        }
                    }
                    
                    mQuad_Vector_3[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_3[i].current_time_active<=0)
                    {
                        mQuad_Vector_3[i].current_time_active = 0;
                        // Disable deadzone
                        craeteQuad(3, 100, 0, 90);
                    }
                }
                else
                {
                    mQuad_Vector_3[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_3[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_3[i].current_time_till_active = mQuad_Vector_3[i].activate_time; // Reset back timer
                        // Enable deadzone
                        mQuad_Vector_3[i].current_time_active = mQuad_Vector_3[i].active_time;
                        
                        // Create by the radius
                        craeteQuad(3, mQuad_Vector_3[i].deadzone_radius, 3, 90);
                    }
                }
            }
            else if(mQuad_Vector_3[i].type == 3)
            {
                // The creazy bullets and stuff like that
                
                // Updates only if flamethrower is active
                if(mQuad_Vector_3[i].current_time_active>0)
                {
                    // Check if any dwarf near to this radius - remove him
                    for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
                    {
                        Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
                        
                        if(dwarf != NULL && dwarf->_dwarfIsRemoving == false)// Check for current quad damage
                        {
                            if(collideAtPoint(dwarf->getPosition()) == 3)
                            {
                                dwarf->_dwarfIsRemoving = true;
                                // Game over for dwarf - burn or do other damage !!
                                
                                // Add particle for FX
                                CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/BurnFx.plist");
                                p->setPosition(dwarf->getPosition());
                                p->setAutoRemoveOnFinish(true);
                                _game->addChild(p);
                                
                                dwarf->removeFromSave();
                            }
                        }
                    }
                    
                    mQuad_Vector_3[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_3[i].current_time_active<=0)
                    {
                        mQuad_3->stopAllActions();
                        mQuad_Vector_3[i].current_time_active = 0;
                        // Disable deadzone
                        mQuad_3->removeAllChildren();//Remove the fire stuff
                        craeteQuad(3, 100, 0, 90);
                    }
                }
                else
                {
                    // Check when will do something
                    mQuad_Vector_3[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_3[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_3[i].current_time_till_active = mQuad_Vector_3[i].activate_time; // Reset back timer
                        
                        if(mQuad_Vector_3[i].event_type == 1)// The bullet fire !!!
                        {
                            // Bumb bum fire bullets
                            int aBulletsAngle = 90/(1+mQuad_Vector_3[i].bullet_amount);
                            for(int b=0;b<mQuad_Vector_3[i].bullet_amount;b++)
                            {
                                CreateSpecialBullet(MASTER_ACTION_BULLET_STRAIGHT,getPositionX(),getPositionY(), aBulletsAngle*(b+1)-180, mQuad_Vector_3[i].bullet_distance);
                            }
                        }
                        else if(mQuad_Vector_3[i].event_type == 2)// The flamethrower
                        {
                            mQuad_Vector_3[i].current_time_active = mQuad_Vector_3[i].flame_active_time;
                            craeteQuad(3, 100, 3, mQuad_Vector_3[i].flame_radius);
                            
                            for(int p=0;p<5;p++)
                            {
                                CCParticleSystemQuad* fx = CCParticleSystemQuad::create("Particles/FlameDot_Fx.plist");
                                fx->setPosition(ccp(p*20,0));
                                fx->setAutoRemoveOnFinish(true);
                                mQuad_3->addChild(fx);
                            }
                            
                            // Reset
                            mQuad_3->stopAllActions();
                            mQuad_3->setRotation(180);// Reset rotation to default? or mid?
                            
                            CCRotateTo* aRotate = CCRotateTo::create(mQuad_Vector_3[i].flame_rotate_speed, 90);
                            CCRotateTo* aRotateBack = CCRotateTo::create(mQuad_Vector_3[i].flame_rotate_speed,180);
                            CCSequence* aRotateSeq = CCSequence::create(aRotate,aRotateBack,NULL);
                            CCRepeatForever* aRotateRepeat = CCRepeatForever::create(aRotateSeq);
                            
                            mQuad_3->runAction(aRotateRepeat);
                        }
                    }
                }
                
            }
        }
    }
    
    // The 4th quad
    if(mQuad_Vector_4.size()>0)
    {
        // Update quad 1
        for(int i=0;i<mQuad_Vector_4.size();i++)
        {
            // Check the shields
            if(mQuad_Vector_4[i].type == 1)
            {
                if(mQuad_Vector_4[i].current_time_active>0)
                {
                    mQuad_Vector_4[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_4[i].current_time_active<=0)
                    {
                        mQuad_Vector_4[i].current_time_active = 0;
                        // Disable shield
                        craeteQuad(4, 100, 0, 90);
                    }
                }
                else
                {
                    mQuad_Vector_4[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_4[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_4[i].current_time_till_active = mQuad_Vector_4[i].activate_time; // Reset back timer
                        // Enable shield
                        mQuad_Vector_4[i].current_time_active = mQuad_Vector_4[i].active_time;
                        if(mQuad_Vector_4[i].event_type == 1)
                        {
                            // basic shield
                            craeteQuad(4, 100, 1, 90);
                        }
                        else if(mQuad_Vector_4[i].event_type == 2)
                        {
                            // some different shield
                            craeteQuad(4, 100, 2, 90);
                        }
                    }
                }
                
            }
            else if(mQuad_Vector_4[i].type == 2) // The deadzone stuff
            {
                if(mQuad_Vector_4[i].current_time_active>0)
                {
                    // Check if any dwarf near to this radius - remove him
                    for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
                    {
                        Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
                        
                        if(dwarf != NULL && dwarf->_dwarfIsRemoving == false)// Check for current quad damage
                        {
                            if(collideAtPoint(dwarf->getPosition()) == 4)
                            {
                                dwarf->_dwarfIsRemoving = true;
                                // Game over for dwarf - burn or do other damage !!
                                
                                // Add particle for FX
                                CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/BurnFx.plist");
                                p->setPosition(dwarf->getPosition());
                                p->setAutoRemoveOnFinish(true);
                                _game->addChild(p);
                                
                                dwarf->removeFromSave();
                            }
                        }
                    }
                    
                    mQuad_Vector_4[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_4[i].current_time_active<=0)
                    {
                        mQuad_Vector_4[i].current_time_active = 0;
                        // Disable deadzone
                        craeteQuad(4, 100, 0, 90);
                    }
                }
                else
                {
                    mQuad_Vector_4[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_4[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_4[i].current_time_till_active = mQuad_Vector_4[i].activate_time; // Reset back timer
                        // Enable deadzone
                        mQuad_Vector_4[i].current_time_active = mQuad_Vector_4[i].active_time;
                        
                        // Create by the radius
                        craeteQuad(4, mQuad_Vector_4[i].deadzone_radius, 3, 90);
                    }
                }
            }
            else if(mQuad_Vector_4[i].type == 3)
            {
                // The creazy bullets and stuff like that
                
                // Updates only if flamethrower is active
                if(mQuad_Vector_4[i].current_time_active>0)
                {
                    // Check if any dwarf near to this radius - remove him
                    for (int dwarfIndex = _game->_dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
                    {
                        Dwarf* dwarf = static_cast<Dwarf*>(_game->_dwarves->objectAtIndex(dwarfIndex));
                        
                        if(dwarf != NULL && dwarf->_dwarfIsRemoving == false)// Check for current quad damage
                        {
                            if(collideAtPoint(dwarf->getPosition()) == 4)
                            {
                                dwarf->_dwarfIsRemoving = true;
                                // Game over for dwarf - burn or do other damage !!
                                
                                // Add particle for FX
                                CCParticleSystemQuad* p = CCParticleSystemQuad::create("Particles/BurnFx.plist");
                                p->setPosition(dwarf->getPosition());
                                p->setAutoRemoveOnFinish(true);
                                _game->addChild(p);
                                
                                dwarf->removeFromSave();
                            }
                        }
                    }
                    
                    mQuad_Vector_4[i].current_time_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_4[i].current_time_active<=0)
                    {
                        mQuad_4->stopAllActions();
                        mQuad_Vector_4[i].current_time_active = 0;
                        // Disable deadzone
                        mQuad_4->removeAllChildren();//Remove the fire stuff
                        craeteQuad(4, 100, 0, 90);
                    }
                }
                else
                {
                    // Check when will do something
                    mQuad_Vector_4[i].current_time_till_active-=delta*_game->getGameSpeed();
                    if(mQuad_Vector_4[i].current_time_till_active<=0)
                    {
                        mQuad_Vector_4[i].current_time_till_active = mQuad_Vector_4[i].activate_time; // Reset back timer
                        
                        if(mQuad_Vector_4[i].event_type == 1)// The bullet fire !!!
                        {
                            // Bumb bum fire bullets
                            int aBulletsAngle = 90/(1+mQuad_Vector_4[i].bullet_amount);
                            for(int b=0;b<mQuad_Vector_4[i].bullet_amount;b++)
                            {
                                CreateSpecialBullet(MASTER_ACTION_BULLET_STRAIGHT,getPositionX(),getPositionY(), aBulletsAngle*(b+1)-270, mQuad_Vector_4[i].bullet_distance);
                            }
                        }
                        else if(mQuad_Vector_4[i].event_type == 2)// The flamethrower
                        {
                            mQuad_Vector_4[i].current_time_active = mQuad_Vector_4[i].flame_active_time;
                            craeteQuad(4, 100, 3, mQuad_Vector_4[i].flame_radius);
                            
                            for(int p=0;p<5;p++)
                            {
                                CCParticleSystemQuad* fx = CCParticleSystemQuad::create("Particles/FlameDot_Fx.plist");
                                fx->setPosition(ccp(p*20,0));
                                fx->setAutoRemoveOnFinish(true);
                                mQuad_4->addChild(fx);
                            }
                            
                            // Reset
                            mQuad_4->stopAllActions();
                            mQuad_4->setRotation(270);// Reset rotation to default? or mid?
                            
                            CCRotateTo* aRotate = CCRotateTo::create(mQuad_Vector_4[i].flame_rotate_speed,360);
                            CCRotateTo* aRotateBack = CCRotateTo::create(mQuad_Vector_4[i].flame_rotate_speed,270);
                            CCSequence* aRotateSeq = CCSequence::create(aRotate,aRotateBack,NULL);
                            CCRepeatForever* aRotateRepeat = CCRepeatForever::create(aRotateSeq);
                            
                            mQuad_4->runAction(aRotateRepeat);
                        }
                    }
                }
                
            }
        }
    }
}

void Enemy_Totem::CreateSpecialBullet(int theType,int theStartX,int theStartY,int theAngle,int theLife)
{
    TrollBullet* aBullet = TrollBullet::create(_game,theType);
    aBullet->setPosition(theStartX,theStartY);
    aBullet->_speed = 100;
    aBullet->_speedMax = 100;
    aBullet->_speedAddValue = 0;
    aBullet->_timeActive = theLife;
    aBullet->_angle = theAngle*M_PI/180;//atanhf(30);
    
    _game->addChild(aBullet, 1000);
    _game->_bullets->addObject(aBullet);
    
    //-----------------------------------------
    // The dwarf get part
    
    /*
    // Chouuuuuse one random dwarf + check if is not near !!!
    Dwarf* dwarf;
    cocos2d::CCArray* _dwarvesToAttack = CCArray::create();
    _dwarvesToAttack->retain();
    
    // Collect all far dwarfs !!!
    for (int dwarfIndex = _dwarves->count() - 1; dwarfIndex >= 0; --dwarfIndex)
    {
        dwarf = static_cast<Dwarf*>(_dwarves->objectAtIndex(dwarfIndex));
        
        if(mCurrentBulletType == MASTER_ACTION_BULLET_STRAIGHT)
        {
            if(dwarf->getEffect()!=NULL)
            {
                _dwarvesToAttack->addObject(dwarf);
            }
        }else if (!dwarf->mBulletActive && dwarf->getEffect()==NULL && !dwarf->_knockOut && dwarf->getDisabled()==false)
        {
            float dwarfDistance = sqrtf((_MasterTrollBase->getPositionX()-dwarf->getPositionX())*(_MasterTrollBase->getPositionX()-dwarf->getPositionX()) +
                                        (_MasterTrollBase->getPositionY()-dwarf->getPositionY())*(_MasterTrollBase->getPositionY()-dwarf->getPositionY()));
            if(dwarfDistance>300)
            {
                //This can be choosen to attack !!!
                _dwarvesToAttack->addObject(dwarf);
            }
        }
    }
    
    if(_dwarvesToAttack->count() == 0){
        return;//No luck
    }
    
    //Now choose !!!
    int aRanodmDwarf = rand()%_dwarvesToAttack->count();
    dwarf = static_cast<Dwarf*>(_dwarvesToAttack->objectAtIndex(aRanodmDwarf));
    
    //-----------------------------------------
    
    TrollBullet* aBullet = TrollBullet::create(this,mCurrentBulletType);
    aBullet->setPosition(theStartX,theStartY);
    aBullet->_speed = mCurrentMission.MT_Bullet_Speed_Min;
    aBullet->_speedMax = mCurrentMission.MT_Bullet_Speed_Max;
    aBullet->_speedAddValue = (aBullet->_speedMax-aBullet->_speed)*0.1;
    
    if(theType == MASTER_ACTION_BULLET_STRAIGHT)
    {
        aBullet->setAngle(atan2f(dwarf->getPositionY() - theStartY, dwarf->getPositionX() - theStartX));
        aBullet->_straightCords.setPoint(dwarf->getPositionX(),dwarf->getPositionY());
    }
    
    // Add it now
    this->addChild(aBullet, 1000);
    _bullets->addObject(aBullet);
    //clear the arr !!!
    _dwarvesToAttack->release();
    */
}

