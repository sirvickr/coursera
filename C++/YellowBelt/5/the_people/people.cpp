#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Во избежание избыточного копирования при передаче параметров в функции, все
// аргументы типа string и vector были заменены на соответствующие (константные) ссылки.

// Базовый класс, описывающий любого человека
class Person {
public:
  Person(const string& occupation, const string& name)
  : occupation(occupation), name(name) {
  }

  // Разрешаем читать имя вне класса Person
  string Name() const {
    return name;
  }

  // Разрешаем читать вид деятельности вне иерархии классов Person
  string Occupation() const {
    return occupation;
  }

  // Общее для всех людей действие, но каждый выполняет его по своему
  // (поэтому метод объявлен как виртуальный)
  virtual void Walk(const string& destination) const = 0;

  // Витруальный деструктор в базовом классе с виртуальными функциями
  // необходим для того, чтобы деструкторы корректно вызывались
  // для всех объектов иерархии наследования
  virtual ~Person();

protected:
  // Профессия (вид деятельности)
  const string occupation;
  // Имя есть у каждого человека, выносим в базовый класс
  const string name;
};

// Для корректной работы прогаммы даже у абстрактныго
// базового класса должно быть определение деструктора
Person::~Person() {
}

// Студент - тоже человек
class Student : public Person {
public:
    // Инициализацию членов класса проводим не в теле конструктора,
    // а в списке инициализации - наиболее универсальный подход
    Student(const string& name, const string& favouriteSong)
    : Person("Student", name), favouriteSong(favouriteSong) {
    }

    // Разрешаем читать приватный член класса извне
    string FavouriteSong() const {
      return favouriteSong;
    }

    // Полиморфная реализация виртуального метода, унаследованного от Person
    // (поведение с тем же названием, но специфичное для студента)
    void Walk(const string& destination) const override {
        cout << occupation << ": " << name << " walks to: " << destination << endl;
        cout << occupation << ": " << name << " sings a song: " << favouriteSong << endl;
    }

    // Действия, специфичные для студента
    void Learn() {
        cout << occupation << ": " << name << " learns" << endl;
    }
    void SingSong() {
        cout << occupation << ": " << name << " sings a song: " << favouriteSong << endl;
    }

private: // скрываем члены класса по избежание непреднамеренного изменение их извне
    string favouriteSong;
};

// И учитель - человек
class Teacher : public Person {
public:
    // Инициализацию членов класса проводим не в теле конструктора,
    // а в списке инициализации - наиболее универсальный подход
    Teacher(const string& name, const string& subject)
    : Person("Teacher", name), subject(subject) {
    }

    // Разрешаем читать приватный член класса извне
    string Subject() const {
      return subject;
    }

    // Полиморфная реализация виртуального метода, унаследованного от Person
    // (поведение с тем же названием, но специфичное для учителя)
    void Walk(const string& destination) const override {
        cout << occupation << ": " << name << " walks to: " << destination << endl;
    }

    // Действия, специфичные для учителя
    void Teach() {
        cout << occupation << ": " << name << " teaches: " << subject << endl;
    }

private: // скрываем члены класса по избежание непреднамеренного изменение их извне
    string subject;
};

// И полицейский
class Policeman : public Person {
public:
    Policeman(const string& name) : Person("Policeman", name) {
    }

    // Вместо нескольких специфичных для каждого человека функций
    // проверки документов, реализуем одну, универсальную для всех
    void Check(const Person& person) {
        cout << occupation << ": " << name << " checks " << person.Occupation() << ". "
             << person.Occupation() << "'s name is: " << person.Name() << endl;
    }

    // Полиморфная реализация виртуального метода, унаследованного от Person
    // (поведение с тем же названием, но специфичное для полицейского)
    void Walk(const string& destination) const override {
        cout << occupation << ": " << name << " walks to: " << destination << endl;
    }
};

// Вместо нескольких специфичных для каждого человека функций
// путешествия, реализуем одну, универсальную для всех
void VisitPlaces(const Person& person, const vector<string>& places) {
    for (auto p : places) {
      person.Walk(p);
    }
}

// Помимо приведённого в задании сценария, проведённый рефакторинг
// позволит использовать объекты этой иерархии классов гораздо большим
// количеством спсобов - например, хранить все объекты с помощью
// shared_ptr<Person> в различных контейнерах, создавать объекты
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
