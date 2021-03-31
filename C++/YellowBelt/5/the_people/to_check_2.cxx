#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
public:
    Person(const string& name, const string& occupation) : name_(name), occupation_(occupation) {}
    string GetName() const {
        return name_;
    }
    string GetOccupation() const {
        return occupation_;
    }
    virtual void Walk(const string& destination) const = 0;
private:
    const string name_;
    const string occupation_;
};

class Student : public Person {
public:
    Student(const string& name, const string& favourite_song)
           : Person(name, "Student"), favourite_song_(favourite_song) {}
    void Learn() const {
        cout << "Student: " << GetName() << " learns" << endl;
    }
    void Walk(const string& destination) const override {
        cout << "Student: " << GetName() << " walks to: " << destination << endl;
        SingSong();
    }
    void SingSong() const {
        cout << "Student: " << GetName() << " sings a song: " << favourite_song_ << endl;
    }
private:
    const string favourite_song_;
};

class Teacher : public Person {
public:
    Teacher(const string& name, const string& subject)
           : Person(name, "Teacher"), subject_(subject) {}
    void Teach() const {
        cout << "Teacher: " << GetName() << " teaches: " << subject_ << endl;
    }
    void Walk(const string& destination) const override {
        cout << "Teacher: " << GetName() << " walks to: " << destination << endl;
    }
private:
    const string subject_;
};

class Policeman : public Person {
public:
    Policeman(const string& name) : Person(name, "Policeman") {}
    void Check(Person& person) const {
        cout << "Policeman: " << GetName() << " checks "
        << person.GetOccupation() << ". " << person.GetOccupation()
        << "'s name is: " << person.GetName() << endl;
    }
    void Walk(const string& destination) const override {
        cout << "Policeman: " << GetName() << " walks to: " << destination << endl;
    }
};

#if 0

void VisitPlaces(Person& person, vector<string> places) {
    for (const auto& place : places) {
        person.Walk(place);
    }
}

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
