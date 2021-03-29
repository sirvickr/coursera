Задание, оцениваемое сокурсниками: Грамотно рефакторим код

В данном задании вам предстоит изменить код таким образом, чтобы он соответствовал изложенным в курсе принципам.

Представьте, что этот код вам достался в наследство от коллеги и теперь вам предстоит поддерживать его и улучшать. 
Первое, что вы решили сделать — это провести рефакторинг.

Конечные цели рефакторинга:
- упростить дальнейшую поддержку кода;
- уменьшить сложность кода;
- упростить добавление новых абстракций в программу.

Задание будет оцениваться по следующим критериям:
- эффективность выполнения кода;
- задублированность кода;
- эффективность использования приёмов рефакторинга кода, показанных в лекциях.

Кроме того, присылаемый вами код должен компилироваться.

Код для рефакторинга:

#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Student {
public:

    Student(string name, string favouriteSong) {
        Name = name;
        FavouriteSong = favouriteSong;
    }

    void Learn() {
        cout << "Student: " << Name << " learns" << endl;
    }

    void Walk(string destination) {
        cout << "Student: " << Name << " walks to: " << destination << endl;
        cout << "Student: " << Name << " sings a song: " << FavouriteSong << endl;
    }

    void SingSong() {
        cout << "Student: " << Name << " sings a song: " << FavouriteSong << endl;
    }

public:
    string Name;
    string FavouriteSong;
};


class Teacher {
public:

    Teacher(string name, string subject) {
        Name = name;
        Subject = subject;
    }

    void Teach() {
        cout << "Teacher: " << Name << " teaches: " << Subject << endl;
    }

    void Walk(string destination) {
        cout << "Teacher: " << Name << " walks to: " << destination << endl;
    }

public:
    string Name;
    string Subject;
};


class Policeman {
public:
    Policeman(string name) {
        Name = name;
    }

    void Check(Teacher t) {
        cout << "Policeman: " << Name << " checks Teacher. Teacher's name is: " << t.Name << endl;
    }

    void Check(Student s) {
        cout << "Policeman: " << Name << " checks Student. Student's name is: " << s.Name << endl;
    }

    void Check(Policeman p) {
        cout << "Policeman: " << Name << " checks Policeman. Policeman's name is: " << p.Name << endl;
    }

    void Walk(string destination) {
        cout << "Policeman: " << Name << " walks to: " << destination << endl;
    }

public:
    string Name;
};


void VisitPlaces(Teacher t, vector<string> places) {
    for (auto p : places) {
        t.Walk(p);
    }
}

void VisitPlaces(Student s, vector<string> places) {
    for (auto p : places) {
        s.Walk(p);
    }
}

void VisitPlaces(Policeman pol, vector<string> places) {
    for (auto p : places) {
        pol.Walk(p);
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

Вывод:

Teacher: Jim walks to: Moscow
Teacher: Jim walks to: London
Policeman: Bob checks Student. Student's name is: Ann
Student: Ann walks to: Moscow
Student: Ann sings a song: We will rock you
Student: Ann walks to: London
Student: Ann sings a song: We will rock you

