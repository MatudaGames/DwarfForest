//
//  WeeklyChallenges.h
//  DwarfForest
//
//  Created by Agris Dumins on 9/1/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Crystal.h"

#include <stdint.h>
#include <ctime>

class WeeklyTask
{
public:
    static const int NO_TARGET = -1;
    
    enum RewardType
    {
        RT_DIAMONDS = 1,
        RT_XP = 2
    };
    
    enum Task
    {
        T_LEAD_DWARFS,
        T_COLLECT_CRYSTALS,
        T_COLLECT_DIAMONDS,
        T_AVOID_METEORITS,
        T_AVOID_TROLLS,
        T_SCORE_POINTS
    };
    
    WeeklyTask(Task task,
               int taskTarget,
               std::string taskInfo,
               uint32_t requirement,
               RewardType rewardType,
               uint32_t reward);
    
    Task getTask() const {return task;}
    int getTaskTarget() const {return taskTarget;}
    std::string getTaskInfo() const {return taskInfo;}
    uint32_t getRequirement() const {return requirement;}
    RewardType getRewardType() const {return rewardType;}
    uint32_t getReward() const {return reward;}

private:
    
    Task task;
    int taskTarget;
    std::string taskInfo;
    uint32_t requirement;
    RewardType rewardType;
    uint32_t reward;
    
};

class WeeklyChallenges
{
public:
	
    WeeklyChallenges();
    
    const WeeklyTask* getTasks();
    void checkWeekChanges();
    
    void ClearAllData();
    
    uint32_t getTask1Progress();
    uint32_t getTask2Progress();
    uint32_t getTask3Progress();
    
    void setTask1Progress(uint32_t progress);
    void setTask2Progress(uint32_t progress);
    void setTask3Progress(uint32_t progress);
    
    bool isTaskCompleted(WeeklyTask::Task task_type, int target, uint32_t progress);
    const WeeklyTask* getTask(WeeklyTask::Task task_type, int target);
    void finishTask(WeeklyTask::Task task_type, int target);
    bool addTaskProgress(WeeklyTask::Task task_type, int target, uint32_t progress);
    
    uint32_t GetActiveTasks();
    
private:
    static const WeeklyTask challenges[4][3];
    
    time_t lastUpdateTime;
    uint32_t activeWeek;
    
    cocos2d::CCUserDefault* save;
};
