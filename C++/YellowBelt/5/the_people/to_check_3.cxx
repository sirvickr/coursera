#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Human {
public:
   Human(const string &name, const string &type) : Name(name), Type(type) { }

   string Log() const {
      return (Type + ": " + Name);
   }

   virtual void Walk(const string &destination) {
      cout << Log() << " walks to: " << destination << endl;
   }

   const string Name, Type;
};

class Student : public Human {
public:
   Student(string name, string favouriteSong) : Human(name, "Student"), FavouriteSong(favouriteSong) { }

   void Learn() {
      cout << Log() << " learns" << endl;
   }

   void Walk(const string &destination) override {
      cout << Log() << " walks to: " << destination << endl;
      SingSong();
   }

   void SingSong() {
      cout << Log() << " sings a song: " << FavouriteSong << endl;
   }

private:
   const string FavouriteSong;
};


class Teacher : public Human {
public:
   Teacher(const string &name, const string &subject) : Human(name, "Teacher"), Subject(subject) { }

   void Teach() {
      cout << Log() << " teaches: " << Subject << endl;
   }

private:
   const string Subject;
};


class Policeman : public Human {
public:
   Policeman(const string &name) : Human(name, "Policeman") { }

   void Check(const Human &h) {
      cout << Log() << " checks " << h.Type << ". " << h.Type << "'s name is: " << h.Name << endl;
   }
};

#if 1

void VisitPlaces(Human &h, vector<string> places) {
   for (auto p : places) {
      h.Walk(p);
   }
}

int main() {
   Teacher t("Jim", "Math");
   Student s("Ann", "We will rock you");
   Policeman p("Bob");

   VisitPlaces(t, { "Moscow", "London" });
   p.Check(s);
   VisitPlaces(s, { "Moscow", "London" });
   return 0;
}

#endif
