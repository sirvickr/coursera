// SUBMIT THIS FILE TO GRADER

#include <string>
#include <tuple>
#include <map>
#include <iostream>
#include <utility>
#include <algorithm>

using namespace std;

class TeamTasks {
public:
  // Получить статистику по статусам задач конкретного разработчика
  const TasksInfo& GetPersonTasksInfo(const string& person) const {
    return data.at(person);
  }

  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
  void AddNewTask(const string& person) {
    data[person][TaskStatus::NEW]++;
  }

#ifdef TESTING_TEAM_TASKS
  void SetTasks(const string& person, TaskStatus status, int count) {
    data[person][status] = count;
  }
#endif
  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
    TasksInfo updated, untouched;
    auto user_it = data.find(person);
    if(user_it == data.end())
      return {};
    TasksInfo& tasks = user_it->second;

    // Собрать результат: цикл [NEW, DONE)
    for(int curr = 0; curr < last; ++curr) {
      TaskStatus curr_status = static_cast<TaskStatus>(curr);
      TaskStatus next_status = static_cast<TaskStatus>(curr + 1);
      auto curr_it = tasks.find(curr_status);
      if(curr_it != tasks.end()) {
        int person_task_count = curr_it->second;
        int count_to_move = min(task_count, person_task_count);
        if(task_count < person_task_count)
          untouched[curr_status] = person_task_count - task_count;
        if(count_to_move > 0)
          updated[next_status] = count_to_move;
        task_count -= count_to_move;
      }
    }
    ApplyChanges(tasks, updated, untouched);

    return { updated, untouched };
  }

private:
  void ApplyChanges(TasksInfo& tasks, const TasksInfo& updated, const TasksInfo& untouched) {
    // Обновить задачи: цикл [NEW, DONE]
    for(int curr = 0; curr <= last; ++curr) {
      TaskStatus curr_status = static_cast<TaskStatus>(curr);
      auto updated_it = updated.find(curr_status);
      int updated_count = updated_it != updated.end() ? updated_it->second : 0;
      auto untouched_it = untouched.find(curr_status);
      int untouched_count = untouched_it != untouched.end() ? untouched_it->second : 0;
      // на последней итерации (TaskStatus::DONE), untouched_count равняется нулю
      int task_count = updated_count + untouched_count;
      auto curr_it = tasks.find(curr_status);
      if(curr_it != tasks.end()) {
        if(TaskStatus::DONE == curr_status) {
          curr_it->second += task_count; // untouched_count равняется нулю => task_count == updated_count
        } else {
          if(task_count > 0) {
            curr_it->second = task_count;
          } else {
            tasks.erase(curr_it);
          }
        }
      } else {
        if(task_count > 0) {
          tasks.insert({ curr_status, task_count });
        }
      }
    }
  }

private:
  const int last = static_cast<int>(TaskStatus::DONE);
  map<string, TasksInfo> data;
};
