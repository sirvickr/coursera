#pragma once

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

  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(
      const string& person, int task_count) {
	  TasksInfo updated_tasks, untouched_tasks;
	  TasksInfo& person_tasks = data[person];
	  for(int i = 0; i < static_cast<int>(TaskStatus::DONE); ++i) {
		  auto status = static_cast<TaskStatus>(i);
		  auto nextStatus = static_cast<TaskStatus>(i + 1);
		  if(person_tasks.count(status)) {
			  int count = person_tasks[status];
			  if(task_count >= count) {
				  if(count)
					  updated_tasks[nextStatus] = count;
				  task_count -= count;
			  } else {
				  if(task_count)
					  updated_tasks[nextStatus] = task_count;
				  if(count - task_count > 0)
					  untouched_tasks[status] = count - task_count;
				  task_count = 0;
			  }
		  }
	  }
	  for(int i = 0; i <= static_cast<int>(TaskStatus::DONE); ++i) {
		  auto status = static_cast<TaskStatus>(i);
		  int count = 0;
		  if(updated_tasks.count(status))
			  count += updated_tasks[status];
		  if(untouched_tasks.count(status))
			  count += untouched_tasks[status];
		  if(count) {
			  person_tasks[status] = count;
		  } else if(status != TaskStatus::DONE) {
			  person_tasks.erase(status);
		  }
	  }
	  return {updated_tasks, untouched_tasks};
  }

private:
  map<string, TasksInfo> data;
};
