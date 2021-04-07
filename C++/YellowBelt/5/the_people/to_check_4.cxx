#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class People{
public:
    People(const string& name)
              : Name(name) {}

    virtual void Walk(const string& destination) = 0;
    virtual string State()const =0;
    virtual ~People() = default;

    const string Name;

};



class Student : public People{
public:

    Student(const string& name, const string& favouriteSong): People(name), FavouriteSong(favouriteSong) {

    }

    void Learn() {
        cout << "Student: " << Name << " learns" << endl;
    }

    void Walk(const string& destination) override{
        cout << "Student: " << Name << " walks to: " << destination << endl;
        cout << "Student: " << Name << " sings a song: " << FavouriteSong << endl;
    }

    void SingSong() {
        cout << "Student: " << Name << " sings a song: " << FavouriteSong << endl;
    }

    string State()const override{
        return state;
    }
private:
   const string state="Student";
   const string FavouriteSong;
};


class Teacher : public People {
public:

    Teacher(const string& name, const string& subject): People(name), Subject(subject) {

    }

    void Teach() {
        cout << "Teacher: " << Name << " teaches: " << Subject << endl;
    }

    void Walk(const string& destination) override {
        cout << "Teacher: " << Name << " walks to: " << destination << endl;
    }

    string State()const override{
        return state;
    }
private:
    const string state="Teacher";
    const string Subject;
};


class Policeman : public People {
public:
    Policeman(string name): People(name) {

    }
    template <class T>
    void Check(T unit){
        cout << State() <<": " << Name << " checks "<< unit.State()<<". "<< unit.State()<<"'s name is: " << unit.Name << endl;
    }

    void Walk(const string& destination) override{
        cout << "Policeman: " << Name << " walks to: " << destination << endl;
    }

    string State()const override{
           return state;
    }

private:
    const string state="Policeman";
};


template <class T>
void VisitPlaces(T unit, vector<string> places) {
    for (auto p : places) {
        unit.Walk(p);
    }
}

#if 0

int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}

#endif
