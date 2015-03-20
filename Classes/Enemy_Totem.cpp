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

#define kHUD_Z_Order 200

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
    
    //------------------------
    
    mQuad_1_AngleWidth = 0;
    mQuad_2_AngleWidth = 0;
    mQuad_3_AngleWidth = 0;
    mQuad_4_AngleWidth = 0;
    
    /*
    mQuad_1_Nodes = CCArray::create();
    mQuad_1_Nodes->retain();
    mQuad_2_Nodes = CCArray::create();
    mQuad_2_Nodes->retain();
    mQuad_3_Nodes = CCArray::create();
    mQuad_3_Nodes->retain();
    mQuad_4_Nodes = CCArray::create();
    mQuad_4_Nodes->retain();
    
    mQuad_1_NodeData = CCPointArray::create(0);
    mQuad_1_NodeData->initWithCapacity(1000);
    mQuad_1_NodeData->retain();
    mQuad_2_NodeData = CCPointArray::create(0);
    mQuad_2_NodeData->initWithCapacity(1000);
    mQuad_2_NodeData->retain();
    mQuad_3_NodeData = CCPointArray::create(1000);
    mQuad_3_NodeData->retain();
    mQuad_4_NodeData = CCPointArray::create(1000);
    mQuad_4_NodeData->retain();
    */
    
//    std::vector<QuadActionInfo> mQuad_1_Nodes;
    
    //------------------------
    
    _game = game;
    
    _allCreated = false;
    
    _movePoints = CCArray::create();
    _movePoints->retain();
    
    _base = CCSprite::create("beta/Totem1.png");
    _base->setAnchorPoint(ccp(0.5,0));
//    _base->setOpacity(128);
    addChild(_base,1);
    
    mBar_TotemBase = CCSprite::create("small_dot_red.png");
    mBar_TotemBase->setPosition(ccp(0,0));
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
    /*
    mBar_TotemHP->setTextureRect(CCRect(0, 0,
                                              mBar_TotemHP->getTexture()->getContentSize().width*(1.0f),
                                              mBar_TotemHP->getTexture()->getContentSize().height));
    
    // Create the 3 timers !!!
    mDebugTimerLabel = CCLabelTTF::create("111\n222\n100",FONT_SKRANJI,
                                          TITLE_FONT_SIZE*0.5, CCSize(300,240), kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
    mDebugTimerLabel->setPosition(ccp(16,20));
    addChild(mDebugTimerLabel);
    */
    
    // Create the 4 sides for dwarf check?
//    mQuad_1 = CCDrawNode::create();this->addChild( mQuad_1 );craeteQuad(1, 100, 0, 90);
//    mQuad_2 = CCDrawNode::create();this->addChild( mQuad_2 );craeteQuad(2, 100, 0, 90);
//    mQuad_3 = CCDrawNode::create();this->addChild( mQuad_3 );craeteQuad(3, 100, 0, 90);
//    mQuad_4 = CCDrawNode::create();this->addChild( mQuad_4 );craeteQuad(4, 100, 0, 90);
    
    mPassive_Zone = CCDrawNode::create();this->addChild( mPassive_Zone );
    mPassive_Zone->setVisible(false);
    
    // The debug info !!!
    mDebugQuad_1 = CCLabelTTF::create("-","fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
    mDebugQuad_1->setHorizontalAlignment(kCCTextAlignmentLeft);
    mDebugQuad_1->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    mDebugQuad_1->setAnchorPoint(ccp(0,0));
    
    mDebugQuad_1->setColor(ccc3(255, 255, 255));
    mDebugQuad_1->enableShadow(CCSize(3,-3),6,false);
    
    mDebugQuad_1->setPosition(ccp(20,10));
    
    addChild(mDebugQuad_1,2);
    
    mDebugQuad_2 = CCLabelTTF::create("-","fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
    mDebugQuad_2->setHorizontalAlignment(kCCTextAlignmentLeft);
    mDebugQuad_2->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    mDebugQuad_2->setAnchorPoint(ccp(0,1));
    
    mDebugQuad_2->setColor(ccc3(255, 255, 255));
    mDebugQuad_2->enableShadow(CCSize(3,-3),6,false);
    
    mDebugQuad_2->setPosition(ccp(20,-10));
    
    addChild(mDebugQuad_2,2);
    
    mDebugQuad_3 = CCLabelTTF::create("-","fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
    mDebugQuad_3->setHorizontalAlignment(kCCTextAlignmentRight);
    mDebugQuad_3->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    mDebugQuad_3->setAnchorPoint(ccp(1,1));
    
    mDebugQuad_3->setColor(ccc3(255, 255, 255));
    mDebugQuad_3->enableShadow(CCSize(3,-3),6,false);
    
    mDebugQuad_3->setPosition(ccp(-20,-10));
    
    addChild(mDebugQuad_3,2);
    
    mDebugQuad_4 = CCLabelTTF::create("-","fonts/Marker Felt.ttf",TITLE_FONT_SIZE*0.5);
    mDebugQuad_4->setHorizontalAlignment(kCCTextAlignmentRight);
    mDebugQuad_4->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    mDebugQuad_4->setAnchorPoint(ccp(1,0));
    
    mDebugQuad_4->setColor(ccc3(255, 255, 255));
    mDebugQuad_4->enableShadow(CCSize(3,-3),6,false);
    
    mDebugQuad_4->setPosition(ccp(-20,10));
    
    addChild(mDebugQuad_4,2);
    
    
    
    return true;
}

void Enemy_Totem::AttackFromPlayer(cocos2d::CCPoint position,SpellInfo theSpell)
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
    
    float finalDamage = theSpell.damage;
    
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
                    if(mQuad_Vector_1[i].event_type == 2){ finalDamage = float(theSpell.damage)*0.5;}// Half damage taken
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
                    if(mQuad_Vector_2[i].event_type == 2){ finalDamage = float(theSpell.damage)*0.5;}// Half damage taken
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
                    if(mQuad_Vector_3[i].event_type == 2){ finalDamage = float(theSpell.damage)*0.5;}// Half damage taken
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
                    if(mQuad_Vector_4[i].event_type == 2){ finalDamage = float(theSpell.damage)*0.5;}// Half damage taken
                    else if(mQuad_Vector_4[i].event_type == 1){finalDamage = 0;}// No damage
                    break;
                }
            }
        }
    }
    
    // Show the damage
    if(finalDamage>0)
    {
        //...................................................................
        // Show the daage fly out !!
        
        // Quick damage blob
        CCBlink* aBlink = CCBlink::create(0.25f, 2);
        runAction(aBlink);
        
        std::stringstream theDamageValue;
        theDamageValue<<"-"<<finalDamage;
        
        CCLabelTTF *theDamage = CCLabelTTF::create(theDamageValue.str().c_str(), FONT_SKRANJI_BOLD, TITLE_FONT_SIZE*0.7, CCSize(300,240),
                                                   kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom);
        theDamage->setColor(ccc3(222,0,0));
        theDamage->setAnchorPoint(ccp(0.5f,0.5f));
        theDamage->setPosition(ccp(getPositionX(),getPositionY()+_base->getContentSize().height+30));
        _game->addChild(theDamage, kHUD_Z_Order-1);
        
        // The animation for remove !!!
        CCMoveBy* aMoveBy = CCMoveBy::create(1.0f,ccp(0,50));
        CCEaseExponentialOut* aEase = CCEaseExponentialOut::create(aMoveBy);
        CCCallFuncN* aFuncDone = CCCallFuncN::create(_game, callfuncN_selector(GameScene::removeNode));
        CCSequence* aSeq = CCSequence::create(aEase,aFuncDone,NULL);
        theDamage->runAction(aSeq);
        //...................................................................
    }
    
    // Take the damage now
    mNeedHP-=finalDamage;
    
    // Check if this is not some poision attack
    if(theSpell.damage_extra>0)
    {
        // Here comes the poision !!!
        SpellDamage theDamage;
        
        theDamage.damage = theSpell.damage_extra;
        theDamage.times = theSpell.damage_extra_multiply;
        theDamage.timeToDamage = theSpell.damage_extra_time;
        theDamage.currentTime = theSpell.damage_extra_time;
        
        // Check if has not already poision stuff - otherwise add it !!!
        _base->setColor(ccGREEN);
        
        mExtraDamage.push_back(theDamage);
    }
    
    // Safe check
    if(mNeedHP<=0){
        mNeedHP = 0;
    }
}

void Enemy_Totem::removeQuadAction(int theID,int theSubID)
{
    int aValue = (theID*100)+theSubID;
    
    if(theID == 1)
    {
        for(int i=0;i<mQuad_1_Nodes.size();i++)
        {
            if(mQuad_1_Nodes[i].tag == aValue)
            {
                // Remove this one
                removeChildByTag(aValue);
                mQuad_1_Nodes.erase(mQuad_1_Nodes.begin()+i);
                break;// No more stuff here
            }
        }
    }
    else if(theID == 2)
    {
        for(int i=0;i<mQuad_2_Nodes.size();i++)
        {
            if(mQuad_2_Nodes[i].tag == aValue)
            {
                // Remove this one
                removeChildByTag(aValue);
                mQuad_2_Nodes.erase(mQuad_2_Nodes.begin()+i);
                break;// No more stuff here
            }
        }
    }
    else if(theID == 3)
    {
        for(int i=0;i<mQuad_3_Nodes.size();i++)
        {
            if(mQuad_3_Nodes[i].tag == aValue)
            {
                // Remove this one
                removeChildByTag(aValue);
                mQuad_3_Nodes.erase(mQuad_3_Nodes.begin()+i);
                break;// No more stuff here
            }
        }
    }
    else if(theID == 4)
    {
        for(int i=0;i<mQuad_4_Nodes.size();i++)
        {
            if(mQuad_4_Nodes[i].tag == aValue)
            {
                // Remove this one
                removeChildByTag(aValue);
                mQuad_4_Nodes.erase(mQuad_4_Nodes.begin()+i);
                break;// No more stuff here
            }
        }
    }


}

// The new multi layer quad stuff
CCDrawNode* Enemy_Totem::addQuadAction(int theID,int theSubID,int theWidth,int theType,int theAngle, int theEventType)
{
    std::vector<CCPoint> points;
    points.push_back(ccp(0,0));
    
    int coneWidth = theAngle;
    int coneRadius = theWidth;
    
    for (float ii = 0; ii < coneWidth; ii += 0.1)
    {
        points.push_back(ccp(0 + coneRadius * cos(ii * (M_PI / 180)), 0 + coneRadius * sin(ii * (M_PI / 180))));
    }
    
    points.push_back(ccp(0,0));
    
    CCPoint* pPoints = new CCPoint[points.size()];
    
    ccColor4F pShieldColor;
    
    // Here comes the magic
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
    
    // Add this new stuff under all
    int theTag = (100*theID)+theSubID;
    CCDrawNode* _Quad_dummy = CCDrawNode::create();
    _Quad_dummy->setTag(theTag);
    
    _Quad_dummy->clear();
    _Quad_dummy->drawPolygon(pPoints, points.size(), pShieldColor, 2, ccc4f(0, 0, 0, 0.1) );
    _Quad_dummy->setRotation(0);
    
    if(theID == 1){
        QuadActionInfo* aQuadNodeInfo = new QuadActionInfo();
        aQuadNodeInfo->size = theAngle;
        aQuadNodeInfo->width = theWidth;
        aQuadNodeInfo->tag = theTag;
        aQuadNodeInfo->event_type = theEventType;
        
        mQuad_1_Nodes.push_back(*aQuadNodeInfo);
        
        _Quad_dummy->setRotation(0);
    }
    else if(theID == 2){
        QuadActionInfo* aQuadNodeInfo = new QuadActionInfo();
        aQuadNodeInfo->size = theAngle;
        aQuadNodeInfo->width = theWidth;
        aQuadNodeInfo->tag = theTag;
        aQuadNodeInfo->event_type = theEventType;
        
        mQuad_2_Nodes.push_back(*aQuadNodeInfo);
        
        _Quad_dummy->setRotation(90);
    }
    else if(theID == 3){
        QuadActionInfo* aQuadNodeInfo = new QuadActionInfo();
        aQuadNodeInfo->size = theAngle;
        aQuadNodeInfo->width = theWidth;
        aQuadNodeInfo->tag = theTag;
        aQuadNodeInfo->event_type = theEventType;
        
        mQuad_3_Nodes.push_back(*aQuadNodeInfo);
        
        _Quad_dummy->setRotation(180);
    }
    else if(theID == 4){
        QuadActionInfo* aQuadNodeInfo = new QuadActionInfo();
        aQuadNodeInfo->size = theAngle;
        aQuadNodeInfo->width = theWidth;
        aQuadNodeInfo->tag = theTag;
        aQuadNodeInfo->event_type = theEventType;
        
        mQuad_4_Nodes.push_back(*aQuadNodeInfo);
        
        _Quad_dummy->setRotation(270);
    }
    
    this->addChild( _Quad_dummy ,0);
    
    return _Quad_dummy;
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
    
    // Some def values
    int coneAngleOffset = 0;
    int coneAngle = 0;
    int dx = 0;
    int dy = 0;
    int distanceFromTroll = 0;
    int angle = 0;
    
    // The new universal stuff for multi
    CCDrawNode* aDummy;
    int aValue = 0;
    CCPoint aData;
    
    //mQuad_1_NodeData.x = mQuad_1_AngleWidth;
    //mQuad_1_NodeData.y = mQuad_1_Size;
    
    // The 1st quad check !!!
    for(int i = 0; i<mQuad_1_Nodes.size();i++)
    {
        // Do not check shields
        if(mQuad_1_Nodes[i].event_type == 1) continue;
        
        aValue = mQuad_1_Nodes[i].tag;
        aDummy = static_cast<CCDrawNode*>(getChildByTag(aValue));
        
        if(aDummy == NULL) {CCLog("Cant find child with tag:%i",aValue); continue;}
        
        coneAngleOffset = -fmod(aDummy->getRotation(), 360);
        coneAngle = mQuad_1_Nodes[i].size + coneAngleOffset;
        
        dx = point.x - getPositionX();
        dy = point.y - getPositionY();
        
        distanceFromTroll = sqrt(pow(dx, 2) + pow(dy, 2));
        angle = atan2(dy, dx) * (180 / M_PI);
        
        if(angle>0 && fmod(aDummy->getRotation(), 360)>=mQuad_1_Nodes[i].width){angle-=360;}
        
        // We found something !!! Givi it this info back
        if(distanceFromTroll <= mQuad_1_Nodes[i].width && (angle >= coneAngleOffset && angle <= coneAngle)) return aCollideQuad = 1;
    }
    
    // The 2nd quad check !!!
    for(int i = 0; i<mQuad_2_Nodes.size();i++)
    {
        // Do not check shields
        if(mQuad_2_Nodes[i].event_type == 1) continue;
        
        aValue = mQuad_2_Nodes[i].tag;
        aDummy = static_cast<CCDrawNode*>(getChildByTag(aValue));
        
        if(aDummy == NULL) {CCLog("Cant find child with tag:%i",aValue); continue;}
        
        coneAngleOffset = -fmod(aDummy->getRotation(), 360);
        coneAngle = mQuad_2_Nodes[i].size + coneAngleOffset;
        
        dx = point.x - getPositionX();
        dy = point.y - getPositionY();
        
        distanceFromTroll = sqrt(pow(dx, 2) + pow(dy, 2));
        angle = atan2(dy, dx) * (180 / M_PI);
        
        if(angle>0 && fmod(aDummy->getRotation(), 360)>=mQuad_2_Nodes[i].width){angle-=360;}
        
        // We found something !!! Givi it this info back
        if(distanceFromTroll <= mQuad_2_Nodes[i].width && (angle >= coneAngleOffset && angle <= coneAngle)) return aCollideQuad = 2;
    }
    
    // The 1st quad check !!!
    for(int i = 0; i<mQuad_3_Nodes.size();i++)
    {
        // Do not check shields
        if(mQuad_3_Nodes[i].event_type == 1) continue;
        
        aValue = mQuad_3_Nodes[i].tag;
        aDummy = static_cast<CCDrawNode*>(getChildByTag(aValue));
        
        if(aDummy == NULL) {CCLog("Cant find child with tag:%i",aValue); continue;}
        
        coneAngleOffset = -fmod(aDummy->getRotation(), 360);
        coneAngle = mQuad_3_Nodes[i].size + coneAngleOffset;
        
        dx = point.x - getPositionX();
        dy = point.y - getPositionY();
        
        distanceFromTroll = sqrt(pow(dx, 2) + pow(dy, 2));
        angle = atan2(dy, dx) * (180 / M_PI);
        
        if(angle>0 && fmod(aDummy->getRotation(), 360)>=mQuad_3_Nodes[i].width){angle-=360;}
        
        // We found something !!! Givi it this info back
        if(distanceFromTroll <= mQuad_3_Nodes[i].width && (angle >= coneAngleOffset && angle <= coneAngle)) return aCollideQuad = 3;
    }
    
    // The 1st quad check !!!
    for(int i = 0; i<mQuad_4_Nodes.size();i++)
    {
        // Do not check shields
        if(mQuad_4_Nodes[i].event_type == 1) continue;
        
        aValue = mQuad_4_Nodes[i].tag;
        aDummy = static_cast<CCDrawNode*>(getChildByTag(aValue));
        
        if(aDummy == NULL) {CCLog("Cant find child with tag:%i",aValue); continue;}
        
        coneAngleOffset = -fmod(aDummy->getRotation(), 360);
        coneAngle = mQuad_4_Nodes[i].size + coneAngleOffset;
        
        dx = point.x - getPositionX();
        dy = point.y - getPositionY();
        
        distanceFromTroll = sqrt(pow(dx, 2) + pow(dy, 2));
        angle = atan2(dy, dx) * (180 / M_PI);
        
        if(angle>0 && fmod(aDummy->getRotation(), 360)>=mQuad_4_Nodes[i].width){angle-=360;}
        
        // We found something !!! Givi it this info back
        if(distanceFromTroll <= mQuad_4_Nodes[i].width && (angle >= coneAngleOffset && angle <= coneAngle)) return aCollideQuad = 4;
    }
    
    return aCollideQuad;
    
    
    /*
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
    */
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
        mCurrentHP -= delta*MAX(mCurrentHP-mNeedHP, 2);
        if(mCurrentHP<=mNeedHP)
        {
            // Check if mission not completed
            mCurrentHP = mNeedHP;
            if(mCurrentHP <= 0)
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
    
    // Check other damage for totem
    if(mExtraDamage.size()>0)
    {
        for(int i=0;i<mExtraDamage.size();i++)
        {
            mExtraDamage[i].currentTime-=delta*_game->getGameSpeed();
            if(mExtraDamage[i].currentTime<=0)
            {
                mExtraDamage[i].times-=1;
                if(mExtraDamage[i].times>=0)
                {
                    // Do it more times
                    mExtraDamage[i].currentTime = mExtraDamage[i].timeToDamage;
                    
                    // Do the damage to it !!!
                    mNeedHP -= mExtraDamage[i].damage;
                    if(mNeedHP < 0) mNeedHP = 0;
                    
                    // Quick fx - blink
                    CCBlink* aBlink = CCBlink::create(0.25f, 2);
                    _base->runAction(aBlink);
                }
                else
                {
                    mExtraDamage.erase(mExtraDamage.begin()+i);
                    
                    // Check if any poision left
                    if(mExtraDamage.size() == 0)
                    {
                        // Remove the poision stuff
                        _base->setColor(ccc3(255,255,255));
                    }
                    
                    break;
                }
            }
        }
        
        // Set some fly poision damage stuff ?
        
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
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                quad_info.current_time_active = 0;//quad_info.activate_time;// Start from 0
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
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                quad_info.current_time_active = 0;//quad_info.activate_time;// Start from 0
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
                
                quad_info.flame_angle = totemInfo.TOTEM_QUAD_1[i].flame_angle;
                quad_info.flame_radius = totemInfo.TOTEM_QUAD_1[i].flame_radius;
                quad_info.flame_active_time = totemInfo.TOTEM_QUAD_1[i].flame_active_time;
                quad_info.flame_rotate_speed = totemInfo.TOTEM_QUAD_1[i].flame_rotate_speed;
                
                quad_info.flame_start_angle = totemInfo.TOTEM_QUAD_1[i].flame_start_angle;
                quad_info.flame_end_angle = totemInfo.TOTEM_QUAD_1[i].flame_end_angle;
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
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                quad_info.current_time_active = 0;//quad_info.activate_time;// Start from 0
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
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                quad_info.current_time_active = 0;//quad_info.activate_time;// Start from 0
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
                
                quad_info.flame_angle = totemInfo.TOTEM_QUAD_2[i].flame_angle;
                quad_info.flame_radius = totemInfo.TOTEM_QUAD_2[i].flame_radius;
                quad_info.flame_active_time = totemInfo.TOTEM_QUAD_2[i].flame_active_time;
                quad_info.flame_rotate_speed = totemInfo.TOTEM_QUAD_2[i].flame_rotate_speed;
                
                quad_info.flame_start_angle = totemInfo.TOTEM_QUAD_2[i].flame_start_angle;
                quad_info.flame_end_angle = totemInfo.TOTEM_QUAD_2[i].flame_end_angle;
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
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                quad_info.current_time_active = 0;//quad_info.activate_time;// Start from 0
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
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                quad_info.current_time_active = 0;//quad_info.activate_time;// Start from 0
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
                
                quad_info.flame_angle = totemInfo.TOTEM_QUAD_3[i].flame_angle;
                quad_info.flame_radius = totemInfo.TOTEM_QUAD_3[i].flame_radius;
                quad_info.flame_active_time = totemInfo.TOTEM_QUAD_3[i].flame_active_time;
                quad_info.flame_rotate_speed = totemInfo.TOTEM_QUAD_3[i].flame_rotate_speed;
                
                quad_info.flame_start_angle = totemInfo.TOTEM_QUAD_3[i].flame_start_angle;
                quad_info.flame_end_angle = totemInfo.TOTEM_QUAD_3[i].flame_end_angle;
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
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                quad_info.current_time_active = 0;//quad_info.activate_time;// Start from 0
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
                quad_info.current_time_till_active = quad_info.activate_time;// Start from 0
                quad_info.current_time_active = 0;//quad_info.activate_time;// Start from 0
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
                
                quad_info.flame_angle = totemInfo.TOTEM_QUAD_4[i].flame_angle;
                quad_info.flame_radius = totemInfo.TOTEM_QUAD_4[i].flame_radius;
                quad_info.flame_active_time = totemInfo.TOTEM_QUAD_4[i].flame_active_time;
                quad_info.flame_rotate_speed = totemInfo.TOTEM_QUAD_4[i].flame_rotate_speed;
                
                quad_info.flame_start_angle = totemInfo.TOTEM_QUAD_4[i].flame_start_angle;
                quad_info.flame_end_angle = totemInfo.TOTEM_QUAD_4[i].flame_end_angle;
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
        // CLear debug at start
        mDebugQuad_1_Str.str("");
        mDebugQuad_1_Str.clear();
        
        // Update quad 1
        for(int i=0;i<mQuad_Vector_1.size();i++)
        {
            // Check the shields
            if(mQuad_Vector_1[i].type == 1)
            {
                // Special always enabled case
                if(mQuad_Vector_1[i].current_time_active>0 && mQuad_Vector_1[i].current_time_till_active == -1)
                {
                    mDebugQuad_1_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                }
                else if(mQuad_Vector_1[i].current_time_active>0)
                {
                    mQuad_Vector_1[i].current_time_active-=delta*_game->getGameSpeed();
                    
                    // The debug info
                    mDebugQuad_1_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_1[i].current_time_active)<<"\n";
                    
                    if(mQuad_Vector_1[i].current_time_active<=0)
                    {
                        mQuad_Vector_1[i].current_time_active = 0;
                        // Disable shield
//                        craeteQuad(1, 100, 0, 90);
                        removeQuadAction(1, i);
                    }
                }
                else
                {
                    mQuad_Vector_1[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    // The debug info
                    mDebugQuad_1_Str<<"["<<i<<"] N:"<<ceil(mQuad_Vector_1[i].current_time_till_active)<<"\n";
                    
                    if(mQuad_Vector_1[i].current_time_till_active<=0)
                    {
                        // Check special case when enabled ultimate time !!!
                        if(mQuad_Vector_1[i].activate_time == 0){
                            mQuad_Vector_1[i].current_time_till_active = -1;// Ultimate flag
                        }
                        else{
                            mQuad_Vector_1[i].current_time_till_active = mQuad_Vector_1[i].activate_time; // Reset back timer
                        }
                        
                        // Enable shield
                        mQuad_Vector_1[i].current_time_active = mQuad_Vector_1[i].active_time;
                        if(mQuad_Vector_1[i].event_type == 1)
                        {
                            // basic shield
//                            craeteQuad(1, 100, 1, 90);
                            addQuadAction(1,i,100,1,90,mQuad_Vector_1[i].type);
                        }
                        else if(mQuad_Vector_1[i].event_type == 2)
                        {
                            // some different shield
//                            craeteQuad(1, 100, 2, 90);
                            addQuadAction(1,i,100,1,90,mQuad_Vector_1[i].type);
                        }
                    }
                }
                
            }
            else if(mQuad_Vector_1[i].type == 2) // The deadzone stuff
            {
                // Special always enabled case
                if(mQuad_Vector_1[i].current_time_active>0 && mQuad_Vector_1[i].current_time_till_active == -1)
                {
                    mDebugQuad_1_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                    
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
                }
                else if(mQuad_Vector_1[i].current_time_active>0)
                {
                    mDebugQuad_1_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_1[i].current_time_active)<<"\n";
                    
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
//                        craeteQuad(1, 100, 0, 90);
                        removeQuadAction(1, i);
                    }
                }
                else
                {
                    mQuad_Vector_1[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_1_Str<<"["<<i<<"] N:"<<ceil(mQuad_Vector_1[i].current_time_till_active)<<"\n";
                    
                    if(mQuad_Vector_1[i].current_time_till_active<=0)
                    {
                        // Check special case when enabled ultimate time !!!
                        if(mQuad_Vector_1[i].activate_time == 0){
                            mQuad_Vector_1[i].current_time_till_active = -1;// Ultimate flag
                        }
                        else{
                            mQuad_Vector_1[i].current_time_till_active = mQuad_Vector_1[i].activate_time; // Reset back timer
                        }
                        
//                        mQuad_Vector_1[i].current_time_till_active = mQuad_Vector_1[i].activate_time; // Reset back timer
                        // Enable deadzone
                        mQuad_Vector_1[i].current_time_active = mQuad_Vector_1[i].active_time;
                        
                        // Create by the radius
//                        craeteQuad(1, mQuad_Vector_1[i].deadzone_radius, 3, 90);
                        addQuadAction(1,i,mQuad_Vector_1[i].deadzone_radius,3,90,mQuad_Vector_1[i].type);
                    }
                }
            }
            else if(mQuad_Vector_1[i].type == 3)
            {
                // The creazy bullets and stuff like that
                
                // Updates only if flamethrower is active
                // Special always enabled case
                if(mQuad_Vector_1[i].current_time_active>0 && mQuad_Vector_1[i].current_time_till_active == -1)
                {
                    mDebugQuad_1_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                    
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
                }
                else if(mQuad_Vector_1[i].current_time_active>0)
                {
                    mDebugQuad_1_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_1[i].current_time_active)<<"\n";
                    
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
//                        mQuad_1->stopAllActions();
                        mQuad_Vector_1[i].current_time_active = 0;
                        // Disable deadzone
//                        mQuad_1->removeAllChildren();//Remove the fire stuff
//                        craeteQuad(1, 100, 0, 90);
                        removeQuadAction(1, i);
                    }
                }
                else
                {
                    // Check when will do something
                    mQuad_Vector_1[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_1_Str<<"["<<i<<"] N:"<<ceil(mQuad_Vector_1[i].current_time_till_active)<<"\n";
                    
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
                            // Check special case when enabled ultimate time !!!
                            if(mQuad_Vector_1[i].activate_time == 0){
                                mQuad_Vector_1[i].current_time_till_active = -1;// Ultimate flag
                            }
                            else{
                                mQuad_Vector_1[i].current_time_till_active = mQuad_Vector_1[i].activate_time; // Reset back timer
                            }
                            
                            mQuad_Vector_1[i].current_time_active = mQuad_Vector_1[i].flame_active_time;
//                            craeteQuad(1, 100, 3, mQuad_Vector_1[i].flame_radius);
                            CCDrawNode* aTheNode = addQuadAction(1,i,mQuad_Vector_1[i].flame_radius,3,mQuad_Vector_1[i].flame_angle,mQuad_Vector_1[i].type);
                            
                            for(int p=0;p<5;p++)
                            {
                                CCParticleSystemQuad* fx = CCParticleSystemQuad::create("Particles/FlameDot_Fx.plist");
                                fx->setPosition(ccp(p*20,0));
                                fx->setAutoRemoveOnFinish(true);
                                aTheNode->addChild(fx);
                            }
                            
                            // Reset
                            aTheNode->stopAllActions();
                            aTheNode->setRotation(mQuad_Vector_1[i].flame_end_angle);// Reset rotation to default? or mid?
                            
                            CCRotateTo* aRotate = CCRotateTo::create(mQuad_Vector_1[i].flame_rotate_speed, mQuad_Vector_1[i].flame_start_angle);
                            CCRotateTo* aRotateBack = CCRotateTo::create(mQuad_Vector_1[i].flame_rotate_speed,mQuad_Vector_1[i].flame_end_angle);
                            CCSequence* aRotateSeq = CCSequence::create(aRotate,aRotateBack,NULL);
                            CCRepeatForever* aRotateRepeat = CCRepeatForever::create(aRotateSeq);
                            
                            aTheNode->runAction(aRotateRepeat);
                        }
                    }
                }
                
            }
        }
        
        mDebugQuad_1->setString(mDebugQuad_1_Str.str().c_str());
    }
    
    // 2nd quad
    if(mQuad_Vector_2.size()>0)
    {
        mDebugQuad_2_Str.str("");
        mDebugQuad_2_Str.clear();
        
        // Update quad 1
        for(int i=0;i<mQuad_Vector_2.size();i++)
        {
            // Check the shields
            if(mQuad_Vector_2[i].type == 1)
            {
                // Special always enabled case
                if(mQuad_Vector_2[i].current_time_active>0 && mQuad_Vector_2[i].current_time_till_active == -1)
                {
                    mDebugQuad_2_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                }
                else if(mQuad_Vector_2[i].current_time_active>0)
                {
                    mQuad_Vector_2[i].current_time_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_2_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_2[i].current_time_active)<<"\n";
                    
                    if(mQuad_Vector_2[i].current_time_active<=0)
                    {
                        mQuad_Vector_2[i].current_time_active = 0;
                        // Disable shield
//                        craeteQuad(2, 100, 0, 90);
                        //Remove now the quad sprite
                        removeQuadAction(2, i);
                    }
                }
                else
                {
                    mQuad_Vector_2[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_2_Str<<"["<<i<<"] N:"<<ceil(mQuad_Vector_2[i].current_time_till_active)<<"\n";
                    
                    if(mQuad_Vector_2[i].current_time_till_active<=0)
                    {
                        // Check special case when enabled ultimate time !!!
                        if(mQuad_Vector_2[i].activate_time == 0){
                            mQuad_Vector_2[i].current_time_till_active = -1;// Ultimate flag
                        }
                        else{
                            mQuad_Vector_2[i].current_time_till_active = mQuad_Vector_2[i].activate_time; // Reset back timer
                        }
                        
//                        mQuad_Vector_2[i].current_time_till_active = mQuad_Vector_2[i].activate_time; // Reset back timer
                        // Enable shield
                        mQuad_Vector_2[i].current_time_active = mQuad_Vector_2[i].active_time;
                        if(mQuad_Vector_2[i].event_type == 1)
                        {
                            // basic shield
//                            craeteQuad(2, 100, 1, 90);
                            addQuadAction(2,i,100,1,90,mQuad_Vector_2[i].type);
                        }
                        else if(mQuad_Vector_2[i].event_type == 2)
                        {
                            // some different shield
//                            craeteQuad(2, 100, 2, 90);
                            addQuadAction(2,i,100,1,90,mQuad_Vector_2[i].type);
                        }
                    }
                }
                
            }
            else if(mQuad_Vector_2[i].type == 2) // The deadzone stuff
            {
                // Special always enabled case
                if(mQuad_Vector_2[i].current_time_active>0 && mQuad_Vector_2[i].current_time_till_active == -1)
                {
                    mDebugQuad_2_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                    
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
                }
                else if(mQuad_Vector_2[i].current_time_active>0)
                {
                    mDebugQuad_2_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_2[i].current_time_active)<<"\n";
                    
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
//                        craeteQuad(2, 100, 0, 90);
                        removeQuadAction(2, i);
                    }
                }
                else
                {
                    mQuad_Vector_2[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_2_Str<<"["<<i<<"] N:"<<ceil(mQuad_Vector_2[i].current_time_till_active)<<"\n";
                    
                    if(mQuad_Vector_2[i].current_time_till_active<=0)
                    {
                        // Check special case when enabled ultimate time !!!
                        if(mQuad_Vector_2[i].activate_time == 0){
                            mQuad_Vector_2[i].current_time_till_active = -1;// Ultimate flag
                        }
                        else{
                            mQuad_Vector_2[i].current_time_till_active = mQuad_Vector_2[i].activate_time; // Reset back timer
                        }
                        
//                        mQuad_Vector_2[i].current_time_till_active = mQuad_Vector_2[i].activate_time; // Reset back timer
                        // Enable deadzone
                        mQuad_Vector_2[i].current_time_active = mQuad_Vector_2[i].active_time;
                        
                        // Create by the radius
//                        craeteQuad(2, mQuad_Vector_2[i].deadzone_radius, 3, 90);
                        addQuadAction(2,i,mQuad_Vector_2[i].deadzone_radius, 3, 90,mQuad_Vector_2[i].type);
                    }
                }
            }
            else if(mQuad_Vector_2[i].type == 3)
            {
                // The creazy bullets and stuff like that
                
                // Updates only if flamethrower is active
                // Special always enabled case
                if(mQuad_Vector_2[i].current_time_active>0 && mQuad_Vector_2[i].current_time_till_active == -1)
                {
                    mDebugQuad_2_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                    
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
                }
                else if(mQuad_Vector_2[i].current_time_active>0)
                {
                    
                    mDebugQuad_2_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_2[i].current_time_active)<<"\n";
                    
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
//                        mQuad_2->stopAllActions();
                        mQuad_Vector_2[i].current_time_active = 0;
                        removeQuadAction(2, i);
                    }
                }
                else
                {
                    // Check when will do something
                    mQuad_Vector_2[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_2_Str<<"["<<i<<"] N:"<<ceil(mQuad_Vector_2[i].current_time_till_active)<<"\n";
                    
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
                            // Check special case when enabled ultimate time !!!
                            if(mQuad_Vector_2[i].activate_time == 0){
                                mQuad_Vector_2[i].current_time_till_active = -1;// Ultimate flag
                            }
                            else{
                                mQuad_Vector_2[i].current_time_till_active = mQuad_Vector_2[i].activate_time; // Reset back timer
                            }
                            
                            mQuad_Vector_2[i].current_time_active = mQuad_Vector_2[i].flame_active_time;
                            CCDrawNode* aTheQuadNode = addQuadAction(2,i,mQuad_Vector_2[i].flame_radius,3,mQuad_Vector_2[i].flame_angle,mQuad_Vector_2[i].type);
                            
                            for(int p=0;p<5;p++)
                            {
                                CCParticleSystemQuad* fx = CCParticleSystemQuad::create("Particles/FlameDot_Fx.plist");
                                fx->setPosition(ccp(p*20,0));
                                fx->setAutoRemoveOnFinish(true);
                                aTheQuadNode->addChild(fx);
                            }
                            
                            // Reset
                            aTheQuadNode->stopAllActions();
                            aTheQuadNode->setRotation(mQuad_Vector_2[i].flame_end_angle);// Reset rotation to default? or mid?
                            
                            CCRotateTo* aRotate = CCRotateTo::create(mQuad_Vector_2[i].flame_rotate_speed,mQuad_Vector_2[i].flame_start_angle);
                            CCRotateTo* aRotateBack = CCRotateTo::create(mQuad_Vector_2[i].flame_rotate_speed,mQuad_Vector_2[i].flame_end_angle);
                            CCSequence* aRotateSeq = CCSequence::create(aRotate,aRotateBack,NULL);
                            CCRepeatForever* aRotateRepeat = CCRepeatForever::create(aRotateSeq);
                            
                            aTheQuadNode->runAction(aRotateRepeat);
                        }
                    }
                }
                
            }
        }
        
        mDebugQuad_2->setString(mDebugQuad_2_Str.str().c_str());
    }
    
    // The 3rd quad
    if(mQuad_Vector_3.size()>0)
    {
        mDebugQuad_3_Str.str("");
        mDebugQuad_3_Str.clear();
        
        // Update quad 1
        for(int i=0;i<mQuad_Vector_3.size();i++)
        {
            // Check the shields
            if(mQuad_Vector_3[i].type == 1)
            {
                // Special always enabled case
                if(mQuad_Vector_3[i].current_time_active>0 && mQuad_Vector_3[i].current_time_till_active == -1)
                {
                    mDebugQuad_3_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                }
                else if(mQuad_Vector_3[i].current_time_active>0)
                {
                    mQuad_Vector_3[i].current_time_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_3_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_3[i].current_time_active)<<"\n";
                    
                    if(mQuad_Vector_3[i].current_time_active<=0)
                    {
                        mQuad_Vector_3[i].current_time_active = 0;
                        // Disable shield
//                        craeteQuad(3, 100, 0, 90);
                        removeQuadAction(3, i);
                    }
                }
                else
                {
                    mQuad_Vector_3[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_3_Str<<"ID["<<i<<"] N:"<<ceil(mQuad_Vector_3[i].current_time_till_active)<<"\n";
                    
                    if(mQuad_Vector_3[i].current_time_till_active<=0)
                    {
                        // Check special case when enabled ultimate time !!!
                        if(mQuad_Vector_3[i].activate_time == 0){
                            mQuad_Vector_3[i].current_time_till_active = -1;// Ultimate flag
                        }
                        else{
                            mQuad_Vector_3[i].current_time_till_active = mQuad_Vector_3[i].activate_time; // Reset back timer
                        }
                        
//                        mQuad_Vector_3[i].current_time_till_active = mQuad_Vector_3[i].activate_time; // Reset back timer
                        // Enable shield
                        mQuad_Vector_3[i].current_time_active = mQuad_Vector_3[i].active_time;
                        if(mQuad_Vector_3[i].event_type == 1)
                        {
                            // basic shield
//                            craeteQuad(3, 100, 1, 90);
                            addQuadAction(3,i,100,1,90,mQuad_Vector_3[i].type);
                        }
                        else if(mQuad_Vector_3[i].event_type == 2)
                        {
                            // some different shield
//                            craeteQuad(3, 100, 2, 90);
                            addQuadAction(3,i,100,1,90,mQuad_Vector_3[i].type);
                        }
                    }
                }
                
            }
            else if(mQuad_Vector_3[i].type == 2) // The deadzone stuff
            {
                // Special always enabled case
                if(mQuad_Vector_3[i].current_time_active>0 && mQuad_Vector_3[i].current_time_till_active == -1)
                {
                    mDebugQuad_3_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                    
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
                }
                else if(mQuad_Vector_3[i].current_time_active>0)
                {
                    mDebugQuad_3_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_3[i].current_time_active)<<"\n";
                    
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
//                        craeteQuad(3, 100, 0, 90);
                        removeQuadAction(3, i);
                    }
                }
                else
                {
                    mQuad_Vector_3[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_3_Str<<"["<<i<<"] N:"<<ceil(mQuad_Vector_3[i].current_time_till_active)<<"\n";
                    
                    if(mQuad_Vector_3[i].current_time_till_active<=0)
                    {
                        // Check special case when enabled ultimate time !!!
                        if(mQuad_Vector_3[i].activate_time == 0){
                            mQuad_Vector_3[i].current_time_till_active = -1;// Ultimate flag
                        }
                        else{
                            mQuad_Vector_3[i].current_time_till_active = mQuad_Vector_3[i].activate_time; // Reset back timer
                        }
                        
//                        mQuad_Vector_3[i].current_time_till_active = mQuad_Vector_3[i].activate_time; // Reset back timer
                        // Enable deadzone
                        mQuad_Vector_3[i].current_time_active = mQuad_Vector_3[i].active_time;
                        
                        // Create by the radius
//                        craeteQuad(3, mQuad_Vector_3[i].deadzone_radius, 3, 90);
                        addQuadAction(3,i,mQuad_Vector_3[i].deadzone_radius,3,90,mQuad_Vector_3[i].type);
                    }
                }
            }
            else if(mQuad_Vector_3[i].type == 3)
            {
                // The creazy bullets and stuff like that
                
                // Updates only if flamethrower is active
                // Special always enabled case
                if(mQuad_Vector_3[i].current_time_active>0 && mQuad_Vector_3[i].current_time_till_active == -1)
                {
                    mDebugQuad_3_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                    
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
                }
                else if(mQuad_Vector_3[i].current_time_active>0)
                {
                    mDebugQuad_3_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_3[i].current_time_active)<<"\n";
                    
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
//                        mQuad_3->stopAllActions();
                        mQuad_Vector_3[i].current_time_active = 0;
                        // Disable deadzone
//                        mQuad_3->removeAllChildren();//Remove the fire stuff
//                        craeteQuad(3, 100, 0, 90);
                        
                        removeQuadAction(3, i);
                    }
                }
                else
                {
                    // Check when will do something
                    mQuad_Vector_3[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_3_Str<<"["<<i<<"] N:"<<ceil(mQuad_Vector_3[i].current_time_till_active)<<"\n";
                    
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
                            // Check special case when enabled ultimate time !!!
                            if(mQuad_Vector_3[i].activate_time == 0){
                                mQuad_Vector_3[i].current_time_till_active = -1;// Ultimate flag
                            }
                            else{
                                mQuad_Vector_3[i].current_time_till_active = mQuad_Vector_3[i].activate_time; // Reset back timer
                            }
                            
                            mQuad_Vector_3[i].current_time_active = mQuad_Vector_3[i].flame_active_time;
//                            craeteQuad(3, 100, 3, mQuad_Vector_3[i].flame_radius);
                            CCDrawNode* aTheNode = addQuadAction(3,i,mQuad_Vector_3[i].flame_radius,3,mQuad_Vector_3[i].flame_angle,mQuad_Vector_3[i].type);
                            
                            for(int p=0;p<5;p++)
                            {
                                CCParticleSystemQuad* fx = CCParticleSystemQuad::create("Particles/FlameDot_Fx.plist");
                                fx->setPosition(ccp(p*20,0));
                                fx->setAutoRemoveOnFinish(true);
                                aTheNode->addChild(fx);
                            }
                            
                            // Reset
                            aTheNode->stopAllActions();
                            aTheNode->setRotation(mQuad_Vector_3[i].flame_end_angle);// Reset rotation to default? or mid?
                            
                            CCRotateTo* aRotate = CCRotateTo::create(mQuad_Vector_3[i].flame_rotate_speed, mQuad_Vector_3[i].flame_start_angle);
                            CCRotateTo* aRotateBack = CCRotateTo::create(mQuad_Vector_3[i].flame_rotate_speed,mQuad_Vector_3[i].flame_end_angle);
                            CCSequence* aRotateSeq = CCSequence::create(aRotate,aRotateBack,NULL);
                            CCRepeatForever* aRotateRepeat = CCRepeatForever::create(aRotateSeq);
                            
                            aTheNode->runAction(aRotateRepeat);
                        }
                    }
                }
                
            }
        }
        
        mDebugQuad_3->setString(mDebugQuad_3_Str.str().c_str());
    }
    
    // The 4th quad
    if(mQuad_Vector_4.size()>0)
    {
        mDebugQuad_4_Str.str("");
        mDebugQuad_4_Str.clear();
        
        // Update quad 1
        for(int i=0;i<mQuad_Vector_4.size();i++)
        {
            // Check the shields
            if(mQuad_Vector_4[i].type == 1)
            {
                // Special always enabled case
                if(mQuad_Vector_4[i].current_time_active>0 && mQuad_Vector_4[i].current_time_till_active == -1)
                {
                    mDebugQuad_4_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                }
                else if(mQuad_Vector_4[i].current_time_active>0)
                {
                    mQuad_Vector_4[i].current_time_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_4_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_4[i].current_time_active)<<"\n";
                    
                    if(mQuad_Vector_4[i].current_time_active<=0)
                    {
                        mQuad_Vector_4[i].current_time_active = 0;
                        // Disable shield
//                        craeteQuad(4, 100, 0, 90);
                        removeQuadAction(4, i);
                    }
                }
                else
                {
                    mQuad_Vector_4[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_4_Str<<"["<<i<<"] N:"<<ceil(mQuad_Vector_4[i].current_time_till_active)<<"\n";
                    
                    if(mQuad_Vector_4[i].current_time_till_active<=0)
                    {
                        // Check special case when enabled ultimate time !!!
                        if(mQuad_Vector_4[i].activate_time == 0){
                            mQuad_Vector_4[i].current_time_till_active = -1;// Ultimate flag
                        }
                        else{
                            mQuad_Vector_4[i].current_time_till_active = mQuad_Vector_4[i].activate_time; // Reset back timer
                        }
                        
//                        mQuad_Vector_4[i].current_time_till_active = mQuad_Vector_4[i].activate_time; // Reset back timer
                        // Enable shield
                        mQuad_Vector_4[i].current_time_active = mQuad_Vector_4[i].active_time;
                        if(mQuad_Vector_4[i].event_type == 1)
                        {
                            // basic shield
//                            craeteQuad(4, 100, 1, 90);
                            addQuadAction(4,i,100,1,90,mQuad_Vector_4[i].type);
                        }
                        else if(mQuad_Vector_4[i].event_type == 2)
                        {
                            // some different shield
//                            craeteQuad(4, 100, 2, 90);
                            addQuadAction(4,i,100,2,90,mQuad_Vector_4[i].type);
                        }
                    }
                }
                
            }
            else if(mQuad_Vector_4[i].type == 2) // The deadzone stuff
            {
                // Special always enabled case
                if(mQuad_Vector_4[i].current_time_active>0 && mQuad_Vector_4[i].current_time_till_active == -1)
                {
                    mDebugQuad_4_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                    
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
                }
                else if(mQuad_Vector_4[i].current_time_active>0)
                {
                    mDebugQuad_4_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_4[i].current_time_active)<<"\n";
                    
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
//                        craeteQuad(4, 100, 0, 90);
                        removeQuadAction(4, i);
                    }
                }
                else
                {
                    mQuad_Vector_4[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_4_Str<<"["<<i<<"] N:"<<ceil(mQuad_Vector_4[i].current_time_till_active)<<"\n";
                    
                    if(mQuad_Vector_4[i].current_time_till_active<=0)
                    {
                        // Check special case when enabled ultimate time !!!
                        if(mQuad_Vector_4[i].activate_time == 0){
                            mQuad_Vector_4[i].current_time_till_active = -1;// Ultimate flag
                        }
                        else{
                            mQuad_Vector_4[i].current_time_till_active = mQuad_Vector_4[i].activate_time; // Reset back timer
                        }
                        
//                        mQuad_Vector_4[i].current_time_till_active = mQuad_Vector_4[i].activate_time; // Reset back timer
                        // Enable deadzone
                        mQuad_Vector_4[i].current_time_active = mQuad_Vector_4[i].active_time;
                        
                        // Create by the radius
//                        craeteQuad(4, mQuad_Vector_4[i].deadzone_radius, 3, 90);
                        addQuadAction(4,i,mQuad_Vector_4[i].deadzone_radius,3,90,mQuad_Vector_4[i].type);
                    }
                }
            }
            else if(mQuad_Vector_4[i].type == 3)
            {
                // The creazy bullets and stuff like that
                
                // Updates only if flamethrower is active
                // Special always enabled case
                if(mQuad_Vector_4[i].current_time_active>0 && mQuad_Vector_4[i].current_time_till_active == -1)
                {
                    mDebugQuad_4_Str<<"["<<i<<"] A: ALWAYS"<<"\n";
                    
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
                }
                else if(mQuad_Vector_4[i].current_time_active>0)
                {
                    mDebugQuad_4_Str<<"["<<i<<"] A:"<<ceil(mQuad_Vector_4[i].current_time_active)<<"\n";

                    
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
//                        mQuad_4->stopAllActions();
                        mQuad_Vector_4[i].current_time_active = 0;
                        // Disable deadzone
//                        mQuad_4->removeAllChildren();//Remove the fire stuff
//                        craeteQuad(4, 100, 0, 90);
                        removeQuadAction(4, i);
                    }
                }
                else
                {
                    // Check when will do something
                    mQuad_Vector_4[i].current_time_till_active-=delta*_game->getGameSpeed();
                    
                    mDebugQuad_4_Str<<"["<<i<<"] N:"<<ceil(mQuad_Vector_4[i].current_time_till_active)<<"\n";
                    
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
                            // Check special case when enabled ultimate time !!!
                            if(mQuad_Vector_4[i].activate_time == 0){
                                mQuad_Vector_4[i].current_time_till_active = -1;// Ultimate flag
                            }
                            else{
                                mQuad_Vector_4[i].current_time_till_active = mQuad_Vector_4[i].activate_time; // Reset back timer
                            }
                            
//                            mQuad_Vector_4[i].current_time_active = mQuad_Vector_4[i].flame_active_time;
//                            craeteQuad(4, 100, 3, mQuad_Vector_4[i].flame_radius);
                            CCDrawNode* aTheNode = addQuadAction(4,i,mQuad_Vector_4[i].flame_radius,3,mQuad_Vector_4[i].flame_angle,mQuad_Vector_4[i].type);
                            
                            for(int p=0;p<5;p++)
                            {
                                CCParticleSystemQuad* fx = CCParticleSystemQuad::create("Particles/FlameDot_Fx.plist");
                                fx->setPosition(ccp(p*20,0));
                                fx->setAutoRemoveOnFinish(true);
                                aTheNode->addChild(fx);
                            }
                            
                            // Reset
                            aTheNode->stopAllActions();
                            aTheNode->setRotation(mQuad_Vector_4[i].flame_end_angle);// Reset rotation to default? or mid?
                            
                            CCRotateTo* aRotate = CCRotateTo::create(mQuad_Vector_4[i].flame_rotate_speed,mQuad_Vector_4[i].flame_start_angle);
                            CCRotateTo* aRotateBack = CCRotateTo::create(mQuad_Vector_4[i].flame_rotate_speed,mQuad_Vector_4[i].flame_end_angle);
                            CCSequence* aRotateSeq = CCSequence::create(aRotate,aRotateBack,NULL);
                            CCRepeatForever* aRotateRepeat = CCRepeatForever::create(aRotateSeq);
                            
                            aTheNode->runAction(aRotateRepeat);
                        }
                    }
                }
                
            }
        }
        
        mDebugQuad_4->setString(mDebugQuad_4_Str.str().c_str());
    }
}

void Enemy_Totem::CreateSpecialBullet(int theType,int theStartX,int theStartY,int theAngle,int theLife)
{
    TrollBullet* aBullet = TrollBullet::create(_game,theType);
    aBullet->setPosition(theStartX,theStartY);
    aBullet->_speed = 100;
    aBullet->_speedMax = 100;
    aBullet->_speedAddValue = 0;
//    aBullet->_timeActive = theLife;
    aBullet->_distanceActive = theLife;
    aBullet->_angle = theAngle*M_PI/180;//atanhf(30);
    
    _game->addChild(aBullet, kHUD_Z_Order-1);
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

