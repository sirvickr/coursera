#pragma once

#include "test_runner.h"
#include "definitions.h"

#ifndef DEBUGGING_TEAM_TASKS
//#define DEBUGGING_TEAM_TASKS
#endif

#ifndef TESTING_TEAM_TASKS
#define TESTING_TEAM_TASKS
#endif

#if 0
#include "team_tasks.cxx"
#else
#include "solution.cxx"
#endif

void UnitTests();

void CheckPerformPersonTasks(TeamTasks& tasks, int task_count, const TasksInfo& content_before,
    const tuple<TasksInfo, TasksInfo>& expected_tuple, const TasksInfo& content_after, bool check_result);

void TestWrongUser();
void Test_3_0_0_0_Run_1();
void Test_3_0_0_0_Run_Single();
void Test_TaskExample();
