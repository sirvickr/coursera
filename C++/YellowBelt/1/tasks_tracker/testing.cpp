#include "testing.h"

ostream& operator <<(ostream& out, TaskStatus status) {
  switch(status) {
  case TaskStatus::NEW:
    out << "NEW";
    break;
  case TaskStatus::IN_PROGRESS:
    out << "IN_PROGRESS";
    break;
  case TaskStatus::TESTING:
    out << "TESTING";
    break;
  case TaskStatus::DONE:
    out << "DONE";
    break;
  }
  return out;
}

#ifdef TESTING_TEAM_TASKS
void CheckPerformPersonTasks(TeamTasks& tasks, int task_count, const TasksInfo& content_before,
    const tuple<TasksInfo, TasksInfo>& expected_tuple, const TasksInfo& content_after, bool check_result)
{
  static int ordinal = 1;
  const string hint = "perform " + to_string(task_count) + " tasks (#" + to_string(ordinal++) + ")";
  const string person = "A";

  AssertEqual(tasks.GetPersonTasksInfo(person), content_before, "<content before (" + hint + ")>");

  auto [updated, untouched] = tasks.PerformPersonTasks(person, task_count);
  pair<TasksInfo, TasksInfo> result = { updated, untouched };
  if(check_result) {
    const TasksInfo& expected_updated = get<0>(expected_tuple);
    const TasksInfo& expected_untouched = get<1>(expected_tuple);
    AssertEqual(updated, expected_updated, "<result (updated) of (" + hint + ")>");
    AssertEqual(untouched, expected_untouched, "<result (untouched) of (" + hint + ")>");
    //pair<TasksInfo, TasksInfo> expected = { get<0>(expected_tuple), get<1>(expected_tuple) };
    //AssertEqual(result, expected, "<result of (" + hint + ")>");
  }
  AssertEqual(tasks.GetPersonTasksInfo(person), content_after, "content after (" + hint + ")>");
}

void TestWrongUser() {
  TeamTasks tasks;
  tasks.AddNewTask("A");
  tasks.AddNewTask("A");
  tasks.AddNewTask("A");
  auto [updated, untouched] = tasks.PerformPersonTasks("B", 1);
  pair<TasksInfo, TasksInfo> result = { updated, untouched };
  pair<TasksInfo, TasksInfo> expected = { {}, {} };
  AssertEqual(result, expected, "<result>");
  AssertEqual(tasks.GetPersonTasksInfo("A"), TasksInfo{
    { TaskStatus::NEW, 3 },
  }, "<content>");
}

void Test_3_0_0_0_Run_1() {
  TeamTasks tasks;
  tasks.AddNewTask("A");
  tasks.AddNewTask("A");
  tasks.AddNewTask("A");

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT BEFORE
          { TaskStatus::NEW, 3 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::IN_PROGRESS, 1 },
            },
            // untouched
            {
                { TaskStatus::NEW, 2 },
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::NEW, 2 },
          { TaskStatus::IN_PROGRESS, 1 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT BEFORE
          { TaskStatus::NEW, 2 },
          { TaskStatus::IN_PROGRESS, 1 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::IN_PROGRESS, 1 },
            },
            // untouched
            {
                { TaskStatus::NEW, 1 },
                { TaskStatus::IN_PROGRESS, 1 },
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::NEW, 1 },
          { TaskStatus::IN_PROGRESS, 2 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT BEFORE
          { TaskStatus::NEW, 1 },
          { TaskStatus::IN_PROGRESS, 2 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::IN_PROGRESS, 1 },
            },
            // untouched
            {
                { TaskStatus::IN_PROGRESS, 2 },
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::IN_PROGRESS, 3 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT BEFORE
          { TaskStatus::IN_PROGRESS, 3 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::TESTING, 1 },
            },
            // untouched
            {
                { TaskStatus::IN_PROGRESS, 2 },
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::IN_PROGRESS, 2 },
          { TaskStatus::TESTING, 1 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT AFTER
          { TaskStatus::IN_PROGRESS, 2 },
          { TaskStatus::TESTING, 1 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::TESTING, 1 },
            },
            // untouched
            {
                { TaskStatus::IN_PROGRESS, 1 },
                { TaskStatus::TESTING, 1 },
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::IN_PROGRESS, 1 },
          { TaskStatus::TESTING, 2 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT AFTER
          { TaskStatus::IN_PROGRESS, 1 },
          { TaskStatus::TESTING, 2 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::TESTING, 1 },
            },
            // untouched
            {
                { TaskStatus::TESTING, 2 },
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::TESTING, 3 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT AFTER
          { TaskStatus::TESTING, 3 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::DONE, 1 },
            },
            // untouched
            {
                { TaskStatus::TESTING, 2 },
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::TESTING, 2 },
          { TaskStatus::DONE, 1 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT AFTER
          { TaskStatus::TESTING, 2 },
          { TaskStatus::DONE, 1 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::DONE, 1 },
            },
            // untouched
            {
                { TaskStatus::TESTING, 1 },
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::TESTING, 1 },
          { TaskStatus::DONE, 2 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT AFTER
          { TaskStatus::TESTING, 1 },
          { TaskStatus::DONE, 2 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::DONE, 1 },
            },
            // untouched
            {
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::DONE, 3 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT AFTER
          { TaskStatus::DONE, 3 },
      },
      {   // RESULT
            // updated
            {
            },
            // untouched
            {
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::DONE, 3 },
      },
      true
  );
}

void Test_3_0_0_0_Run_Single() {
  TeamTasks tasks;
  tasks.AddNewTask("A");

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT BEFORE
          { TaskStatus::NEW, 1 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::IN_PROGRESS, 1 },
            },
            // untouched
            {
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::IN_PROGRESS, 1 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT BEFORE
          { TaskStatus::IN_PROGRESS, 1 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::TESTING, 1 },
            },
            // untouched
            {
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::TESTING, 1 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT BEFORE
          { TaskStatus::TESTING, 1 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::DONE, 1 },
            },
            // untouched
            {
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::DONE, 1 },
      },
      true
  );

  CheckPerformPersonTasks(tasks, 1,
      {   // CONTENT BEFORE
          { TaskStatus::DONE, 1 },
      },
      {   // RESULT
            // updated
            {
            },
            // untouched
            {
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::DONE, 1 },
      },
      true
  );
}

void Test_TaskExample() {
  const string person = "A";
  TeamTasks tasks;
  tasks.SetTasks(person, TaskStatus::NEW, 3);
  tasks.SetTasks(person, TaskStatus::IN_PROGRESS, 2);
  tasks.SetTasks(person, TaskStatus::TESTING, 4);
  tasks.SetTasks(person, TaskStatus::DONE, 1);

  CheckPerformPersonTasks(tasks, 4,
      {   // CONTENT BEFORE
          { TaskStatus::NEW, 3 },
          { TaskStatus::IN_PROGRESS, 2 },
          { TaskStatus::TESTING, 4 },
          { TaskStatus::DONE, 1 },
      },
      {   // RESULT
            // updated
            {
                { TaskStatus::IN_PROGRESS, 3 },
                { TaskStatus::TESTING, 1 },
            },
            // untouched
            {
                { TaskStatus::IN_PROGRESS, 1 },
                { TaskStatus::TESTING, 4 },
            },
      },
      {   // CONTENT AFTER
          { TaskStatus::IN_PROGRESS, 4 },
          { TaskStatus::TESTING, 5 },
          { TaskStatus::DONE, 1 },
      },
      true
  );
}
#endif

void UnitTests() {
#ifdef TESTING_TEAM_TASKS
  TestRunner tr;
  tr.RunTest(TestWrongUser, "TestWrongUser");
  tr.RunTest(Test_3_0_0_0_Run_1, "Test_3_0_0_0_Run_1");
  tr.RunTest(Test_3_0_0_0_Run_Single, "Test_3_0_0_0_Run_Multiple");
  tr.RunTest(Test_TaskExample, "Test_TaskExample");
#endif
}
