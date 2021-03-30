#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Во избежание избыточного копирования при передаче параметров в функции, все
// аргументы типа string и vector были заменены на соответствующие (константные) ссылки.

// Базовый класс, описывающий любого человека
class Human {
public:
  Human(const string& occupation, const string& name)
  : occupation(occupation), name(name) {
  }

  // Разрешаем читать имя вне класса Human
  string Name() const {
    return name;
  }

  // Разрешаем читать вид деятельности вне иерархии классов Human
  string Occupation() const {
    return occupation;
  }

  // Общее для всех людей действие, но каждый выполняет его по своему
  // (поэтому метод объявлен как виртуальный)
  virtual void Walk(const string& destination) const = 0;

  // Витруальный деструктор в базовом классе с виртуальными функциями
  // необходим для того, чтобы деструкторы корректно вызывались
  // для всех объектов иерархии наследования
  virtual ~Human();

private:
  // Профессия (вид деятельности)
  const string occupation;
  // Имя есть у каждого человека, выносим в базовый класс
  const string name;
};

// Для корректной работы прогаммы даже у абстрактныго
// базового класса должно быть определение деструктора
Human::~Human() {
}

// Студент - тоже человек
class Student : public Human {
public:
    // Инициализацию членов класса проводим не в теле конструктора,
    // а в списке инициализации - наиболее универсальный подход
    Student(const string& name, const string& favouriteSong)
    : Human("Student", name), favouriteSong(favouriteSong) {
    }

    // Разрешаем читать приватный член класса извне
    string FavouriteSong() const {
      return favouriteSong;
    }

    // Полиморфная реализация виртуального метода, унаследованного от Human
    // (поведение с тем же названием, но специфичное для студента)
    void Walk(const string& destination) const override {
        cout << Occupation() << ": " << Name() << " walks to: " << destination << endl;
        cout << Occupation() << ": " << Name() << " sings a song: " << favouriteSong << endl;
    }

    // Действия, специфичные для студента
    void Learn() {
        cout << Occupation() << ": " << Name() << " learns" << endl;
    }
    void SingSong() {
        cout << Occupation() << ": " << Name() << " sings a song: " << favouriteSong << endl;
    }

private: // скрываем члены класса по избежание непреднамеренного изменение их извне
    const string favouriteSong;
};

// И учитель - человек
class Teacher : public Human {
public:
    // Инициализацию членов класса проводим не в теле конструктора,
    // а в списке инициализации - наиболее универсальный подход
    Teacher(const string& name, const string& subject)
    : Human("Teacher", name), subject(subject) {
    }

    // Разрешаем читать приватный член класса извне
    string Subject() const {
      return subject;
    }

    // Полиморфная реализация виртуального метода, унаследованного от Human
    // (поведение с тем же названием, но специфичное для учителя)
    void Walk(const string& destination) const override {
        cout << Occupation() << ": " << Name() << " walks to: " << destination << endl;
    }

    // Действия, специфичные для учителя
    void Teach() {
        cout << Occupation() << ": " << Name() << " teaches: " << subject << endl;
    }

private: // скрываем члены класса по избежание непреднамеренного изменение их извне
    const string subject;
};

// И полицейский
class Policeman : public Human {
public:
    Policeman(const string& name) : Human("Policeman", name) {
    }

    // Вместо нескольких специфичных для каждого человека функций
    // проверки документов, реализуем одну, универсальную для всех
    void Check(const Human& person) {
        cout << Occupation() << ": " << Name() << " checks " << person.Occupation() << ". "
             << person.Occupation() << "'s name is: " << person.Name() << endl;
    }

    // Полиморфная реализация виртуального метода, унаследованного от Human
    // (поведение с тем же названием, но специфичное для полицейского)
    void Walk(const string& destination) const override {
        cout << Occupation() << ": " << Name() << " walks to: " << destination << endl;
    }
};

// Вместо нескольких специфичных для каждого человека функций
// путешествия, реализуем одну, универсальную для всех
void VisitPlaces(Human& person, const vector<string>& places) {
    for (const auto& p : places) {
      person.Walk(p);
    }
}

// Помимо приведённого в задании сценария, проведённый рефакторинг
// позволит использовать объекты этой иерархии классов гораздо большим
// количеством спсобов - например, хранить все объекты с помощью
// shared_ptr<Human> в различных контейнерах, создавать объекты
// в зависимости поступающих входных данных и т.д.
int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}
