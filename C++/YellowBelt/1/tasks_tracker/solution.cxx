// SOULTION FROM COURSERA

#include <string>
#include <tuple>
#include <map>
#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>

using namespace std;

class TeamTasks {
public:
  // Получить статистику по статусам задач конкретного разработчика
  const TasksInfo& GetPersonTasksInfo(const string& person) const {
    return person_tasks_.at(person);
  }

  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
  void AddNewTask(const string& person) {
    ++person_tasks_[person][TaskStatus::NEW];
  }

#ifdef TESTING_TEAM_TASKS
  void SetTasks(const string& person, TaskStatus status, int count) {
    person_tasks_[person][status] = count;
  }
#endif
  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
    TasksInfo updated_tasks, untouched_tasks;

    // Здесь и далее мы будем пользоваться тем фактом, что в std::map оператор []
    // в случае отсутствия ключа инициализирует значение по умолчанию,
    // если это возможно.
    // std::map::operator[] ->
    // http://ru.cppreference.com/w/cpp/container/map/operator_at
    TasksInfo& tasks = person_tasks_[person];

    // Посчитаем, сколько задач каждого из статусов нужно обновить,
    // пользуясь тем фактом, что по умолчанию enum class инциализирует значения
    // от нуля по возрастанию.
    // enum class -> http://ru.cppreference.com/w/cpp/language/enum
    for (TaskStatus status = TaskStatus::NEW;
         status != TaskStatus::DONE;
         status = Next(status)) {
      // Считаем обновлённые
      updated_tasks[Next(status)] = min(task_count, tasks[status]);
      // Считаем, сколько осталось обновить
      task_count -= updated_tasks[Next(status)];
    }

    // Обновляем статус текущих задач в соответствии с информацией об обновлённых
    // и находим количество нетронутых
    for (TaskStatus status = TaskStatus::NEW;
         status != TaskStatus::DONE;
         status = Next(status)) {
      untouched_tasks[status] = tasks[status] - updated_tasks[Next(status)];
      tasks[status] += updated_tasks[status] - updated_tasks[Next(status)];
    }
    // По условию, DONE задачи не нужно возвращать в не обновлённых задачах
    tasks[TaskStatus::DONE] += updated_tasks[TaskStatus::DONE];

    // По условию в словарях не должно быть нулей
    RemoveZeros(updated_tasks);
    RemoveZeros(untouched_tasks);
    RemoveZeros(person_tasks_.at(person));

    return {updated_tasks, untouched_tasks};
  }

private:
  // Выделим в отдельную функцию получение следующего по приоритету типа задачи
  // Функция налагает требование на входной параметр: он не должен быть равен DONE
  // При этом task_status явно не сравнивается с DONE, что позволяет
  // сделать эту функцию максимально эффективной
  static TaskStatus Next(TaskStatus task_status) {
    return static_cast<TaskStatus>(static_cast<int>(task_status) + 1);
  }

  // Функция для удаления нулей из словаря
  static void RemoveZeros(TasksInfo& tasks_info) {
    // Соберём те статусы, которые нужно убрать из словаря
    vector<TaskStatus> statuses_to_remove;
    for (const auto& task_item : tasks_info) {
      if (task_item.second == 0) {
        statuses_to_remove.push_back(task_item.first);
      }
    }
    for (const TaskStatus status : statuses_to_remove) {
      tasks_info.erase(status);
    }
  }

private:
  map<string, TasksInfo> person_tasks_;
};
