#include "collout.h"

#include <string>
#include <tuple>
#include <map>
#include <iostream>
#include <utility>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};

std::ostream& operator <<(std::ostream& out, TaskStatus status) {
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

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

#include "tasks.h"

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
#if 1
	cout << tasks_info << endl;
#else
	cout << "\t" << tasks_info[TaskStatus::NEW] <<
	  "\t" << tasks_info[TaskStatus::IN_PROGRESS] <<
	  "\t" << tasks_info[TaskStatus::TESTING] <<
	  "\t" << tasks_info[TaskStatus::DONE] << endl;
#endif
}

int main() {
  TeamTasks tasks;
  TasksInfo updated_tasks, untouched_tasks;
#if 1
  for (int i = 0; i < 5; ++i)
	  tasks.AddNewTask("Alice");
  cout << "Add(5)";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
  cout << "--------------------------------------------------------------" << endl;

  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 5);
  cout << "Perf(5)";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
  cout << "Upd: ";
  PrintTasksInfo(updated_tasks);
  cout << "Unt: ";
  PrintTasksInfo(untouched_tasks);
  cout << "--------------------------------------------------------------" << endl;

  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 5);
  cout << "Perf(5)";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
  cout << "Upd: ";
  PrintTasksInfo(updated_tasks);
  cout << "Unt: ";
  PrintTasksInfo(untouched_tasks);
  cout << "--------------------------------------------------------------" << endl;

  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 1);
  cout << "Perf(1)";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
  cout << "Upd: ";
  PrintTasksInfo(updated_tasks);
  cout << "Unt: ";
  PrintTasksInfo(untouched_tasks);
  cout << "--------------------------------------------------------------" << endl;

  for (int i = 0; i < 5; ++i)
	  tasks.AddNewTask("Alice");
  cout << "Add(5)";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
  cout << "--------------------------------------------------------------" << endl;

  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 2);
  cout << "Perf(2)";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
  cout << "Upd: ";
  PrintTasksInfo(updated_tasks);
  cout << "Unt: ";
  PrintTasksInfo(untouched_tasks);
  cout << "--------------------------------------------------------------" << endl;

  cout << "Print";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));

  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 4);
  cout << "Perf(4)";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
  cout << "Upd: ";
  PrintTasksInfo(updated_tasks);
  cout << "Unt: ";
  PrintTasksInfo(untouched_tasks);
  cout << "--------------------------------------------------------------" << endl;

  cout << "Print";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));

#else
  tasks.AddNewTask("Ilia");
  for (int i = 0; i < 3; ++i) {
    tasks.AddNewTask("Ivan");
  }
  cout << "Ilia's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
  cout << "Ivan's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);
#endif
  return 0;
}
