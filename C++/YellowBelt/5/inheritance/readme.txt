Дан следующий код:

#include <iostream>

using namespace std;

class Animal {
public:
    // ваш код

    const string Name;
};


class Dog {
public:
    // ваш код

    void Bark() {
        cout << Name << " barks: woof!" << endl;
    }
};

Определите до конца тела классов, соблюдая следующие требования:
Класс Dog должен являться наследником класса Animal.
Конструктор класса Dog должен принимать параметр типа string и инициализировать им поле Name в классе Animal.
