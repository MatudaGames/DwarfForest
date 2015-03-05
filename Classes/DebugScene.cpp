//
//  DebugScene.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 10/17/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "DebugScene.h"
#include "StaticSceneManager.h"
#include "PopUp.h"
#include "Levels.h"
#include "AppMacros.h"

#include "Utils.h"

USING_NS_CC;

CCScene* DebugScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    DebugScene *layer = DebugScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

bool DebugScene::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCLabelTTF* inc = CCLabelTTF::create("Increase Level", FONT_SKRANJI_BOLD, 40);
    inc->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* incI = CCMenuItemLabel::create(inc, this, menu_selector(DebugScene::incLevel));
    
    CCLabelTTF* dec = CCLabelTTF::create("Decrease Level", FONT_SKRANJI_BOLD, 40);
    dec->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* decI = CCMenuItemLabel::create(dec, this, menu_selector(DebugScene::decLevel));
    
    CCLabelTTF* dim = CCLabelTTF::create("Add diamonds", FONT_SKRANJI_BOLD, 40);
    dim->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* dimI = CCMenuItemLabel::create(dim, this, menu_selector(DebugScene::addDiamonds));
    
    CCLabelTTF* back = CCLabelTTF::create("BACK", FONT_SKRANJI_BOLD, 40);
    back->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* backI = CCMenuItemLabel::create(back, this, menu_selector(DebugScene::back));
    
    CCMenu* menu = CCMenu::create(incI, decI, dimI, backI, NULL);
    menu->alignItemsVerticallyWithPadding(15);
    
    addChild(menu);
    
    //The machines
    //Add some new stuff !!!
    CCLabelTTF* machineClear = CCLabelTTF::create("Up Clear machine", FONT_SKRANJI_BOLD, 40);
    machineClear->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* machineClearI = CCMenuItemLabel::create(machineClear, this, menu_selector(DebugScene::upgradeMachineClear));
    
    CCLabelTTF* machineGhost = CCLabelTTF::create("Up Ghost machine", FONT_SKRANJI_BOLD, 40);
    machineGhost->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* machineGhostI = CCMenuItemLabel::create(machineGhost, this, menu_selector(DebugScene::upgradeMachineGhost));
    
    CCLabelTTF* machineEnemies = CCLabelTTF::create("Up Ghost machine", FONT_SKRANJI_BOLD, 40);
    machineEnemies->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* machineEnemiesI = CCMenuItemLabel::create(machineEnemies, this, menu_selector(DebugScene::upgradeMachineEnemies));
    
    CCLabelTTF* machineClear1 = CCLabelTTF::create("Down Clear machine", FONT_SKRANJI_BOLD, 40);
    machineClear1->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* machineClear1I = CCMenuItemLabel::create(machineClear1, this, menu_selector(DebugScene::downgradeMachineClear));
    
    CCLabelTTF* machineGhost1 = CCLabelTTF::create("Down Ghost machine", FONT_SKRANJI_BOLD, 40);
    machineGhost1->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* machineGhost1I = CCMenuItemLabel::create(machineGhost1, this, menu_selector(DebugScene::downgradeMachineGhost));
    
    CCLabelTTF* machineEnemies1 = CCLabelTTF::create("Down Ghost machine", FONT_SKRANJI_BOLD, 40);
    machineEnemies1->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* machineEnemies1I = CCMenuItemLabel::create(machineEnemies1, this, menu_selector(DebugScene::downgradeMachineEnemies));
    
    CCMenu* menu2 = CCMenu::create(machineClearI, machineGhostI, machineEnemiesI, machineClear1I,machineGhost1I,machineEnemies1I, NULL);
//    menu2->alignItemsVerticallyWithPadding(15);
//    menu2->alignItemsInRows(3,2);
    menu2->alignItemsInRows(3,3);
    
    addChild(menu2);
    
    //The speed up formula
    CCLabelTTF* formula_state = CCLabelTTF::create("ENABLED", FONT_SKRANJI_BOLD, 40);
    formula_state->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_state->setTag(1);
    
    int aValue2 = User::getInstance()->getSpeedMode();
    if(aValue2 == 0)
        formula_state->setString("DISABLED");
    
    CCMenuItemLabel* formula_stateI = CCMenuItemLabel::create(formula_state, this, menu_selector(DebugScene::EnableFormulas));
    formula_stateI->setTag(99);
    
    //=====================================================================
    //aStartMin_1
    CCLabelTTF* formula_1_name = CCLabelTTF::create("min 1 start|", FONT_SKRANJI_BOLD, 40);
    formula_1_name->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_1_name->setTag(1);
    CCMenuItemLabel* formula_1_nameI = CCMenuItemLabel::create(formula_1_name, this, NULL);
    
    CCLabelTTF* formula_1_add = CCLabelTTF::create("+", FONT_SKRANJI_BOLD, 40);
    formula_1_add->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_1_addI = CCMenuItemLabel::create(formula_1_add, this, menu_selector(DebugScene::AddStart_1));
    
    CCLabelTTF* formula_1_result = CCLabelTTF::create("1", FONT_SKRANJI_BOLD, 40);
    formula_1_result->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_1_result->setTag(1);
    
    int aValue = User::getInstance()->getSpeedStartMin_1();
    formula_1_result->setString(toString(aValue).c_str());
    
    CCMenuItemLabel* formula_1_resultI = CCMenuItemLabel::create(formula_1_result, this, NULL);
    formula_1_resultI->setTag(1002);
    
    CCLabelTTF* formula_1_dec = CCLabelTTF::create("-", FONT_SKRANJI_BOLD, 40);
    formula_1_dec->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_1_decI = CCMenuItemLabel::create(formula_1_dec, this, menu_selector(DebugScene::RemoveStart_1));
    
    ///////////////////////////////////////////////////////////////////////
    //aStartMin_1_extra
    CCLabelTTF* formula_1v_name = CCLabelTTF::create("min 1 extra||", FONT_SKRANJI_BOLD, 40);
    formula_1v_name->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_1v_name->setTag(1);
    CCMenuItemLabel* formula_1v_nameI = CCMenuItemLabel::create(formula_1v_name, this, NULL);
    
    CCLabelTTF* formula_1v_add = CCLabelTTF::create("+", FONT_SKRANJI_BOLD, 40);
    formula_1v_add->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_1v_addI = CCMenuItemLabel::create(formula_1v_add, this, menu_selector(DebugScene::AddStart_1_extra));
    
    CCLabelTTF* formula_1v_result = CCLabelTTF::create("1", FONT_SKRANJI_BOLD, 40);
    formula_1v_result->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_1v_result->setTag(1);
    
    aValue = User::getInstance()->getSpeedExtraMin_1_extra();
    formula_1v_result->setString(toString(aValue).c_str());
    
    CCMenuItemLabel* formula_1v_resultI = CCMenuItemLabel::create(formula_1v_result, this, NULL);
    formula_1v_resultI->setTag(1003);
    
    CCLabelTTF* formula_1v_dec = CCLabelTTF::create("-", FONT_SKRANJI_BOLD, 40);
    formula_1v_dec->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_1v_decI = CCMenuItemLabel::create(formula_1v_dec, this, menu_selector(DebugScene::RemoveStart_1_extra));
    
    
    //=====================================================================
    //aStartMin_2
    CCLabelTTF* formula_2_name = CCLabelTTF::create("min 2 start|", FONT_SKRANJI_BOLD, 40);
    formula_2_name->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_2_name->setTag(1);
    CCMenuItemLabel* formula_2_nameI = CCMenuItemLabel::create(formula_2_name, this, NULL);
    
    CCLabelTTF* formula_2_add = CCLabelTTF::create("+", FONT_SKRANJI_BOLD, 40);
    formula_2_add->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_2_addI = CCMenuItemLabel::create(formula_2_add, this, menu_selector(DebugScene::AddStart_2));
    
    CCLabelTTF* formula_2_result = CCLabelTTF::create("1", FONT_SKRANJI_BOLD, 40);
    formula_2_result->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_2_result->setTag(1);
    
    aValue = User::getInstance()->getSpeedStartMin_2();
    formula_2_result->setString(toString(aValue).c_str());
    
    CCMenuItemLabel* formula_2_resultI = CCMenuItemLabel::create(formula_2_result, this, NULL);
    formula_2_resultI->setTag(1004);
    
    CCLabelTTF* formula_2_dec = CCLabelTTF::create("-", FONT_SKRANJI_BOLD, 40);
    formula_2_dec->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_2_decI = CCMenuItemLabel::create(formula_2_dec, this, menu_selector(DebugScene::RemoveStart_2));
    
    ///////////////////////////////////////////////////////////////////////
    //aStartMin_1_extra
    CCLabelTTF* formula_2v_name = CCLabelTTF::create("min 2 extra||", FONT_SKRANJI_BOLD, 40);
    formula_2v_name->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_2v_name->setTag(1);
    CCMenuItemLabel* formula_2v_nameI = CCMenuItemLabel::create(formula_2v_name, this, NULL);
    
    CCLabelTTF* formula_2v_add = CCLabelTTF::create("+", FONT_SKRANJI_BOLD, 40);
    formula_2v_add->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_2v_addI = CCMenuItemLabel::create(formula_2v_add, this, menu_selector(DebugScene::AddStart_2_extra));
    
    CCLabelTTF* formula_2v_result = CCLabelTTF::create("1", FONT_SKRANJI_BOLD, 40);
    formula_2v_result->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_2v_result->setTag(1);
    
    aValue = User::getInstance()->getSpeedExtraMin_2_extra();
    formula_2v_result->setString(toString(aValue).c_str());
    
    CCMenuItemLabel* formula_2v_resultI = CCMenuItemLabel::create(formula_2v_result, this, NULL);
    formula_2v_resultI->setTag(1005);
    
    CCLabelTTF* formula_2v_dec = CCLabelTTF::create("-", FONT_SKRANJI_BOLD, 40);
    formula_2v_dec->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_2v_decI = CCMenuItemLabel::create(formula_2v_dec, this, menu_selector(DebugScene::RemoveStart_2_extra));
    
    //=====================================================================
    //aStartMin_3
    CCLabelTTF* formula_3_name = CCLabelTTF::create("min 3 start|", FONT_SKRANJI_BOLD, 40);
    formula_3_name->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_3_name->setTag(1);
    CCMenuItemLabel* formula_3_nameI = CCMenuItemLabel::create(formula_3_name, this, NULL);
    
    CCLabelTTF* formula_3_add = CCLabelTTF::create("+", FONT_SKRANJI_BOLD, 40);
    formula_3_add->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_3_addI = CCMenuItemLabel::create(formula_3_add, this, menu_selector(DebugScene::AddStart_3));
    
    CCLabelTTF* formula_3_result = CCLabelTTF::create("1", FONT_SKRANJI_BOLD, 40);
    formula_3_result->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_3_result->setTag(1);
    
    aValue = User::getInstance()->getSpeedStartMin_3();
    formula_3_result->setString(toString(aValue).c_str());
    
    CCMenuItemLabel* formula_3_resultI = CCMenuItemLabel::create(formula_3_result, this, NULL);
    formula_3_resultI->setTag(1006);
    
    CCLabelTTF* formula_3_dec = CCLabelTTF::create("-", FONT_SKRANJI_BOLD, 40);
    formula_3_dec->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_3_decI = CCMenuItemLabel::create(formula_3_dec, this, menu_selector(DebugScene::RemoveStart_3));
    
    ///////////////////////////////////////////////////////////////////////
    //aStartMin_1_extra
    CCLabelTTF* formula_3v_name = CCLabelTTF::create("min 3 extra||", FONT_SKRANJI_BOLD, 40);
    formula_3v_name->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_3v_name->setTag(1);
    CCMenuItemLabel* formula_3v_nameI = CCMenuItemLabel::create(formula_3v_name, this, NULL);
    
    CCLabelTTF* formula_3v_add = CCLabelTTF::create("+", FONT_SKRANJI_BOLD, 40);
    formula_3v_add->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_3v_addI = CCMenuItemLabel::create(formula_3v_add, this, menu_selector(DebugScene::AddStart_3_extra));
    
    CCLabelTTF* formula_3v_result = CCLabelTTF::create("1", FONT_SKRANJI_BOLD, 40);
    formula_3v_result->setColor(ccc3(0xFF, 0xFF, 0xFF));
    formula_3v_result->setTag(1);
    
    aValue = User::getInstance()->getSpeedExtraMin_3_extra();
    formula_3v_result->setString(toString(aValue).c_str());
    
    CCMenuItemLabel* formula_3v_resultI = CCMenuItemLabel::create(formula_3v_result, this, NULL);
    formula_3v_resultI->setTag(1007);
    
    CCLabelTTF* formula_3v_dec = CCLabelTTF::create("-", FONT_SKRANJI_BOLD, 40);
    formula_3v_dec->setColor(ccc3(0xFF, 0xFF, 0xFF));
    CCMenuItemLabel* formula_3v_decI = CCMenuItemLabel::create(formula_3v_dec, this, menu_selector(DebugScene::RemoveStart_3_extra));
    
    
    CCMenu* menu3 = CCMenu::create(formula_stateI,formula_1_nameI, formula_1_addI, formula_1_resultI, formula_1_decI,
                                   formula_1v_nameI, formula_1v_addI, formula_1v_resultI, formula_1v_decI,
                                   formula_2_nameI, formula_2_addI, formula_2_resultI, formula_2_decI,
                                   formula_2v_nameI, formula_2v_addI, formula_2v_resultI, formula_2v_decI,
                                   formula_3_nameI, formula_3_addI, formula_3_resultI, formula_3_decI,
                                   formula_3v_nameI, formula_3v_addI, formula_3v_resultI, formula_3v_decI,
                                   NULL);
    menu3->alignItemsInRows(1,4,4,4,4,4,4);
    menu3->setTag(3);
    menu3->setScale(0.5f);
    menu3->setPosition(ccp(250,550));
    addChild(menu3);
    
    
    return true;
}

void DebugScene::EnableFormulas()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(99)->getChildByTag(1));
    int aEnabled = User::getInstance()->getSpeedMode();
    if(aEnabled == 1)
    {
        formula_state->setString("DISABLED");
        User::getInstance()->setSpeedMode(0);
    }
    else
    {
        formula_state->setString("ENABLED");
        User::getInstance()->setSpeedMode(1);
    }
    //formula_state->setString(ToString(aEnabled).c_str());
}

void DebugScene::AddStart_1()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1002)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedStartMin_1();
    aValue+=1;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedStartMin_1(aValue);
}
void DebugScene::RemoveStart_1()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1002)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedStartMin_1();
    aValue-=1;
    if(aValue<0)
        aValue=0;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedStartMin_1(aValue);
}

void DebugScene::AddStart_1_extra()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1003)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedExtraMin_1_extra();
    aValue+=1;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedExtraMin_1_extra(aValue);
}
void DebugScene::RemoveStart_1_extra()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1003)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedExtraMin_1_extra();
    aValue-=1;
    if(aValue<0)
        aValue=0;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedExtraMin_1_extra(aValue);
}

//Value 2
void DebugScene::AddStart_2()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1004)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedStartMin_2();
    aValue+=1;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedStartMin_2(aValue);
}
void DebugScene::RemoveStart_2()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1004)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedStartMin_2();
    aValue-=1;
    if(aValue<0)
        aValue=0;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedStartMin_2(aValue);
}

void DebugScene::AddStart_2_extra()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1005)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedExtraMin_2_extra();
    aValue+=1;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedExtraMin_2_extra(aValue);
}
void DebugScene::RemoveStart_2_extra()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1005)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedExtraMin_2_extra();
    aValue-=1;
    if(aValue<0)
        aValue=0;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedExtraMin_2_extra(aValue);
}

//Value 3
void DebugScene::AddStart_3()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1006)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedStartMin_3();
    aValue+=1;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedStartMin_3(aValue);
}
void DebugScene::RemoveStart_3()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1006)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedStartMin_3();
    aValue-=1;
    if(aValue<0)
        aValue=0;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedStartMin_3(aValue);
}

void DebugScene::AddStart_3_extra()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1007)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedExtraMin_3_extra();
    aValue+=1;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedExtraMin_3_extra(aValue);
}
void DebugScene::RemoveStart_3_extra()
{
    CCLabelTTF* formula_state = static_cast<CCLabelTTF*>(getChildByTag(3)->getChildByTag(1007)->getChildByTag(1));
    int aValue = User::getInstance()->getSpeedExtraMin_3_extra();
    aValue-=1;
    if(aValue<0)
        aValue=0;
    formula_state->setString(toString(aValue).c_str());
    User::getInstance()->setSpeedExtraMin_3_extra(aValue);
}


void DebugScene::upgradeMachineClear()
{
    /*
    int aMachineLvl =User::getInstance()->getMachine_StampLvl();
    if(aMachineLvl<5)
    {
        aMachineLvl++;
        User::getInstance()->setMachine_StampLvl(aMachineLvl);
        PopUp::create(this, "Clear Map Machine upgraded", NULL, false);
    }
    else
    {
        PopUp::create(this, "Clear Map Machine has MAX LEVEL", NULL, false);
    }
    */
}

void DebugScene::downgradeMachineClear()
{
    /*
    int aMachineLvl =User::getInstance()->getMachine_StampLvl();
    if(aMachineLvl>0)
    {
        aMachineLvl--;
        User::getInstance()->setMachine_StampLvl(aMachineLvl);
        PopUp::create(this, "Clear Map Machine downgraded", NULL, false);
    }
    else
    {
        PopUp::create(this, "Clear Map Machine has MIN LEVEL", NULL, false);
    }
    */
}

//Another machine
void DebugScene::upgradeMachineGhost()
{
    /*
    int aMachineLvl =User::getInstance()->getMachine_GhostLvl();
    if(aMachineLvl<5)
    {
        aMachineLvl++;
        User::getInstance()->setMachine_GhostLvl(aMachineLvl);
        PopUp::create(this, "Ghost Map Machine upgraded", NULL, false);
    }
    else
    {
        PopUp::create(this, "Ghost Map Machine has MAX LEVEL", NULL, false);
    }
    */
}

void DebugScene::downgradeMachineGhost()
{
    /*
    int aMachineLvl =User::getInstance()->getMachine_GhostLvl();
    if(aMachineLvl>0)
    {
        aMachineLvl--;
        User::getInstance()->setMachine_GhostLvl(aMachineLvl);
        PopUp::create(this, "Ghost Map Machine downgraded", NULL, false);
    }
    else
    {
        PopUp::create(this, "Ghost Map Machine has MIN LEVEL", NULL, false);
    }
    */
}

//Another machine
void DebugScene::upgradeMachineEnemies()
{
    /*
    int aMachineLvl =User::getInstance()->getMachine_EnemiesLvl();
    if(aMachineLvl<5)
    {
        aMachineLvl++;
        User::getInstance()->setMachine_EnemiesLvl(aMachineLvl);
        PopUp::create(this, "Clear Map Machine upgraded", NULL, false);
    }
    else
    {
        PopUp::create(this, "Ghost Map Machine has MAX LEVEL", NULL, false);
    }
    */
}

void DebugScene::downgradeMachineEnemies()
{
    /*
    int aMachineLvl =User::getInstance()->getMachine_EnemiesLvl();
    if(aMachineLvl>0)
    {
        aMachineLvl--;
        User::getInstance()->setMachine_EnemiesLvl(aMachineLvl);
        PopUp::create(this, "Enemie Map Machine downgraded", NULL, false);
    }
    else
    {
        PopUp::create(this, "Enemie Map Machine has MIN LEVEL", NULL, false);
    }
    */
}



void DebugScene::incLevel()
{
    if (User::getInstance()->getLevel() >= Levels::getInstance()->getMaxLevel()-1)
    {
        return;
    }
    
    uint32_t level = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("userLevel", 0)+1;
    User::getInstance()->setLevel(level);
    
//    uint32_t experience = Levels::getSharedInstance()->currentLevelPoints()+1;
//    User::getSharedInstance()->SetExp(experience);
    
    uint32_t dwarfs = Levels::getInstance()->currentLevelPoints()+1;
    User::getInstance()->setDwarfs(dwarfs);
    
    PopUp::create(this, "Level increased", NULL, false);
}

void DebugScene::decLevel()
{
    if (User::getInstance()->getLevel() > 0)
    {
        uint32_t level = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("userLevel", 0)-1;
        User::getInstance()->setLevel(level);
        
//        uint32_t experience = Levels::getSharedInstance()->currentLevelPoints()+1;
//        User::getSharedInstance()->SetExp(experience);
        
        uint32_t dwarfs = Levels::getInstance()->currentLevelPoints()+1;
        User::getInstance()->setDwarfs(dwarfs);
        
        PopUp::create(this, "Level decreased", NULL, false);
    }
}

void DebugScene::addDiamonds()
{
    User::getInstance()->addDiamonds(20);
    
    PopUp::create(this, "20 Diamonds added", NULL, false);
}

void DebugScene::back()
{
    CCScene* mainMenu = DF::StaticSceneManager::getInstance()->getScene(DF::StaticSceneManager::MAIN_MENU);
	CCTransitionScene* transition = CCTransitionSlideInL::create(0.5f, mainMenu);
	CCDirector::sharedDirector()->replaceScene(transition);
}
