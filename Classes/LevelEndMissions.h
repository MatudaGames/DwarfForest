//
//  LevelEndMissions.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 11/01/14.
//
//

#pragma once

#include <cocos2d.h>
#include "SpriteAnimation.h"
#include "GameStats.h"
#include "MissionManager.h"

namespace DF
{
    namespace LevelEnd
    {
        class LevelEndMissions: public cocos2d::CCLayer
        {
        public:
            static LevelEndMissions* create();
            
            bool init();
            
            cocos2d::CCSprite* mission_walker;
            
            cocos2d::CCSprite* mission_pad_1;
            cocos2d::CCSprite* mission_pad_2;
            cocos2d::CCSprite* mission_pad_3;
            cocos2d::CCSprite* mission_pad_4;
            cocos2d::CCSprite* mission_pad_5;
            
            cocos2d::CCSprite* mMissionBox;
            void OnClickedMission(CCObject* sender);
            void OnPadClick();
            void OnClouseMission();
            void OnSelectMission(int theMissionID);
            
            cocos2d::CCLabelTTF* mMissionBox_Header;
            
            int aBox1_StartY;
            int aText1_StartY;
            
            cocos2d::CCSprite* mMissionBox_Check1_On;
            cocos2d::CCSprite* mMissionBox_Check1_Off;
            cocos2d::CCLabelTTF* mMissionBox_Text1;
            cocos2d::CCLabelTTF* mMissionBox_Text1p;
            
            int aBox2_StartY;
            int aText2_StartY;
            
            cocos2d::CCSprite* mMissionBox_Check2_On;
            cocos2d::CCSprite* mMissionBox_Check2_Off;
            cocos2d::CCLabelTTF* mMissionBox_Text2;
            cocos2d::CCLabelTTF* mMissionBox_Text2p;
            
            int aBox3_StartY;
            int aText3_StartY;
            
            cocos2d::CCSprite* mMissionBox_Check3_On;
            cocos2d::CCSprite* mMissionBox_Check3_Off;
            cocos2d::CCLabelTTF* mMissionBox_Text3;
            cocos2d::CCLabelTTF* mMissionBox_Text3p;
            
            void GrowPads();
            
            void OnCreateCheckAnim();
            void OnCompletedMission();
            void OnMoveCompleted();
            void CreteUnlock();
            void OnCompletedAll();
            
            void CheckIfCompletedMission();
            
            MissionSet mCompletedMission;
            
//            void dropTroll();
//            void bumpDwarfs();
//            
//            void updateLevel();
//            
//            cocos2d::CCLabelTTF* levelTxt;
//            
//            SpriteAnimation* troll;
//            cocos2d::CCSprite* trollContainer;
//            ExperienceFill* barFill;
//            
//            cocos2d::CCSprite* fat;
//            SpriteAnimation* fatStars;
//            cocos2d::CCSprite* tall;
//            SpriteAnimation* tallStars;
            
        private:
            
            uint32_t pointsToFill;
        };
        
        
    }
}
