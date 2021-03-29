#include <iostream>
#include <string>

using namespace std;

class Animal {
public:
  Animal(string name): Name(name) {
  }

  const string Name;
};

class Dog : public Animal {
public:
  Dog(string name): Animal(name) {
  }

  void Bark() {
    cout << Name << " barks: woof!" << endl;
  }
};
