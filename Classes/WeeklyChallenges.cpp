//
//  WeeklyChallenges.cpp
//  DwarfForest
//
//  Created by Agris Dumins on 9/1/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "WeeklyChallenges.h"

#include "Dwarf.h"
#include "Crystal.h"
#include "User.h"

USING_NS_CC;

WeeklyTask::WeeklyTask(Task task,
                       int taskTarget,
                       std::string taskInfo,
                       uint32_t requirement,
                       RewardType rewardType,
                       uint32_t reward)
    : task(task)
    , taskTarget(taskTarget)
    , taskInfo(taskInfo)
    , requirement(requirement)
    , rewardType(rewardType)
    , reward(reward)
{
}

const WeeklyTask WeeklyChallenges::challenges[4][3] = {
    {
        WeeklyTask(WeeklyTask::T_LEAD_DWARFS, WeeklyTask::NO_TARGET, "Lead 500 dwarfs", 500, WeeklyTask::RT_DIAMONDS, 5),
        WeeklyTask(WeeklyTask::T_SCORE_POINTS, WeeklyTask::NO_TARGET, "Score 20'000 points", 20000, WeeklyTask::RT_XP, 1000),
        WeeklyTask(WeeklyTask::T_COLLECT_CRYSTALS, CRYSTAL_COLOR_GREEN, "Collect 80 Green Crystals", 80, WeeklyTask::RT_DIAMONDS, 10)
    },
    {
        WeeklyTask(WeeklyTask::T_AVOID_TROLLS, WeeklyTask::NO_TARGET, "Avoid 100 Trolls", 100, WeeklyTask::RT_DIAMONDS, 5),
        WeeklyTask(WeeklyTask::T_COLLECT_DIAMONDS, WeeklyTask::NO_TARGET, "Collect 15 Diamonds", 15, WeeklyTask::RT_XP, 1200),
        WeeklyTask(WeeklyTask::T_COLLECT_CRYSTALS, CRYSTAL_COLOR_BLUE, "Collect 70 Blue Crystals", 70, WeeklyTask::RT_DIAMONDS, 10)
    },
    {
        WeeklyTask(WeeklyTask::T_LEAD_DWARFS, DWARF_TYPE_TALL, "Lead 500 Tall Dwarfs", 500, WeeklyTask::RT_DIAMONDS, 7),
        WeeklyTask(WeeklyTask::T_AVOID_METEORITS, WeeklyTask::NO_TARGET, "Avoid 80 Meteorits", 80, WeeklyTask::RT_XP, 1000),
        WeeklyTask(WeeklyTask::T_COLLECT_CRYSTALS, CRYSTAL_COLOR_RED, "Collect 60 Red Crystals", 60, WeeklyTask::RT_DIAMONDS, 10)
    },
    {
        WeeklyTask(WeeklyTask::T_LEAD_DWARFS, DWARF_TYPE_FAT, "Lead 400 Fat Dwarfs", 400, WeeklyTask::RT_DIAMONDS, 5),
        WeeklyTask(WeeklyTask::T_SCORE_POINTS, WeeklyTask::NO_TARGET, "Score 32'000 Points", 32000, WeeklyTask::RT_XP, 1500),
        WeeklyTask(WeeklyTask::T_COLLECT_CRYSTALS, CRYSTAL_COLOR_YELLOW, "Collect 40 Yellow Crystals", 40, WeeklyTask::RT_DIAMONDS, 12)
    }
};

WeeklyChallenges::WeeklyChallenges()
{
    save = CCUserDefault::sharedUserDefault();
    
    lastUpdateTime = save->getIntegerForKey("weeklyChallenges.lastUpdate", 0);
    activeWeek = save->getIntegerForKey("weeklyChallenges.activeWeek", 0);
    
    checkWeekChanges();
}

const WeeklyTask* WeeklyChallenges::getTasks()
{
    checkWeekChanges();
    
    return (const WeeklyTask*)challenges[activeWeek];
}

void WeeklyChallenges::ClearAllData()
{
    save->setIntegerForKey("weeklyChallenges.lastUpdate", 0);
    lastUpdateTime = 0;
    save->setIntegerForKey("weeklyChallenges.activeWeek", 0);
    activeWeek = 0;
    
    save->setIntegerForKey("weeklyChallenges.task1Progress", 0);
    save->setIntegerForKey("weeklyChallenges.task2Progress", 0);
    save->setIntegerForKey("weeklyChallenges.task3Progress", 0);
    
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

bool weekChanged(time_t last)
{
    if (last == 0) return true;
    
    // compare weeks
    struct tm laiks = *localtime(&last);
    laiks.tm_hour = 0;
    laiks.tm_min = 0;
    laiks.tm_sec = 0;
    time_t lastweek = mktime(&laiks);
    lastweek -= laiks.tm_wday * 24 * 3600;
    
    time_t now = time(0);
    struct tm laiksNow = *localtime(&now);
    laiksNow.tm_hour = 0;
    laiksNow.tm_min = 0;
    laiksNow.tm_sec = 0;
    time_t currentWeek = mktime(&laiksNow);
    currentWeek -= laiksNow.tm_wday * 24 * 3600;
    
    if (lastweek == currentWeek)
    {
        return false;
    }
    return true;
}

// checks if week tasks is not old
void WeeklyChallenges::checkWeekChanges()
{
    if (weekChanged(lastUpdateTime))
    {
        activeWeek = (activeWeek+1) % 4;
        lastUpdateTime = time(0);
        
        save->setIntegerForKey("weeklyChallenges.lastUpdate", time(0));
        save->setIntegerForKey("weeklyChallenges.activeWeek", activeWeek);
        
        setTask1Progress(0);
        setTask2Progress(0);
        setTask3Progress(0);
    }
}

uint32_t WeeklyChallenges::getTask1Progress()
{
    return save->getIntegerForKey("weeklyChallenges.task1Progress", 0);
}

uint32_t WeeklyChallenges::getTask2Progress()
{
    return save->getIntegerForKey("weeklyChallenges.task2Progress", 0);
}

uint32_t WeeklyChallenges::getTask3Progress()
{
    return save->getIntegerForKey("weeklyChallenges.task3Progress", 0);
}


void WeeklyChallenges::setTask1Progress(uint32_t progress)
{
    save->setIntegerForKey("weeklyChallenges.task1Progress", progress);
}

void WeeklyChallenges::setTask2Progress(uint32_t progress)
{
    save->setIntegerForKey("weeklyChallenges.task2Progress", progress);
}

void WeeklyChallenges::setTask3Progress(uint32_t progress)
{
    save->setIntegerForKey("weeklyChallenges.task3Progress", progress);
}

bool WeeklyChallenges::isTaskCompleted(WeeklyTask::Task task_type, int target, uint32_t progress)
{
    const WeeklyTask* tasks = getTasks();
    const WeeklyTask* task = NULL;
    
    uint32_t taskProgress = 0;
    
    // finds task
    if (tasks[0].getTask() == task_type && tasks[0].getTaskTarget() == target)
    {
        task = &tasks[0];
        taskProgress = getTask1Progress();
    }
    if (tasks[1].getTask() == task_type && tasks[1].getTaskTarget() == target)
    {
        task = &tasks[1];
        taskProgress = getTask2Progress();
    }
    if (tasks[2].getTask() == task_type && tasks[2].getTaskTarget() == target)
    {
        task = &tasks[2];
        taskProgress = getTask3Progress();
    }
    
    if (task != NULL)
    {
        // check if task completed
        if (taskProgress < task->getRequirement() && task->getRequirement() <= taskProgress + progress)
        {
            return true;
        }
    }
    
    return false;
}

void WeeklyChallenges::finishTask(WeeklyTask::Task task_type, int target)
{
    const WeeklyTask* tasks = getTasks();
    const WeeklyTask* task = NULL;
    
    // finds task
    if (tasks[0].getTask() == task_type && tasks[0].getTaskTarget() == target)
    {
        setTask1Progress(tasks[0].getRequirement());
        task = &tasks[0];
    }
    if (tasks[1].getTask() == task_type && tasks[1].getTaskTarget() == target)
    {
        setTask2Progress(tasks[1].getRequirement());
        task = &tasks[1];
    }
    if (tasks[2].getTask() == task_type && tasks[2].getTaskTarget() == target)
    {
        setTask3Progress(tasks[2].getRequirement());
        task = &tasks[2];
    }
    
    // adding reward
    if (task != NULL)
    {
        if (task->getRewardType() == WeeklyTask::RT_DIAMONDS)
        {
            User::getInstance()->addDiamonds(task->getReward());
        }
        else // XP
        {
            User::getInstance()->addExperience(task->getReward());
        }
    }
}

// returns true if task finished
bool WeeklyChallenges::addTaskProgress(WeeklyTask::Task task_type, int target, uint32_t progress)
{
    const WeeklyTask* tasks = getTasks();

    // finds task
    if (tasks[0].getRequirement() > getTask1Progress()
        && tasks[0].getTask() == task_type
        && tasks[0].getTaskTarget() == target)
    {
        setTask1Progress(progress + getTask1Progress());
    }
    if (tasks[1].getRequirement() > getTask2Progress()
        && tasks[1].getTask() == task_type
        && tasks[1].getTaskTarget() == target)
    {
        setTask2Progress(progress + getTask2Progress());
    }
    if (tasks[2].getRequirement() > getTask3Progress()
        && tasks[2].getTask() == task_type
        && tasks[2].getTaskTarget() == target)
    {
        setTask3Progress(progress + getTask3Progress());
    }
    
    return false;
}

const WeeklyTask* WeeklyChallenges::getTask(WeeklyTask::Task task_type, int target)
{
    const WeeklyTask* tasks = getTasks();
    const WeeklyTask* task = NULL;
    
    // finds task
    if (tasks[0].getTask() == task_type && tasks[0].getTaskTarget() == target)
    {
        task = &tasks[0];
    }
    if (tasks[1].getTask() == task_type && tasks[1].getTaskTarget() == target)
    {
        task = &tasks[1];
    }
    if (tasks[2].getTask() == task_type && tasks[2].getTaskTarget() == target)
    {
        task = &tasks[2];
    }
    
    return task;
}

uint32_t WeeklyChallenges::GetActiveTasks()
{
    const WeeklyTask* t = getTasks();
    uint32_t active = (t[0].getRequirement() > getTask1Progress() ? 1 : 0) +
                      (t[1].getRequirement() > getTask2Progress() ? 1 : 0) +
                      (t[2].getRequirement() > getTask3Progress() ? 1 : 0);
    
    return active;
}

