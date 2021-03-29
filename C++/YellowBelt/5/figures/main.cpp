#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cmath>

using namespace std;

struct Figure {
  virtual string Name() const = 0;
  virtual double Perimeter() const = 0;
  virtual double Area() const = 0;
  virtual ~Figure();
};

class Triangle : public Figure {
public:
  Triangle(double a, double b, double c): a(a), b(b), c(c) {
  }
  string Name() const override {
    return "TRIANGLE";
  }
  double Perimeter() const override {
    return a + b + c;
  }
  double Area() const override {
    double p = Perimeter() / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
  }

private:
  double a, b, c;
};

class Rect : public Figure {
public:
  Rect(double width, double height): width(width), height(height) {
  }
  string Name() const override {
    return "RECT";
  }
  double Perimeter() const override {
    return (width + height) * 2;
  }
  double Area() const override {
    return width * height;
  }

private:
  double width, height;
};

class Circle : public Figure {
public:
  const double PI = 3.14;
  Circle(double radius): radius(radius) {
  }
  string Name() const override {
    return "CIRCLE";
  }
  double Perimeter() const override {
    return 2 * PI * radius;
  }
  double Area() const override {
    return PI * radius * radius;
  }

private:
  double radius;
};

Figure::~Figure() {
}

shared_ptr<Figure> CreateFigure(istream& is) {
  string figure;
  is >> figure;
  if (figure == "RECT") {
    double width, height;
    is >> width >> height;
    return make_shared<Rect>(width, height);
  } else if (figure == "TRIANGLE") {
    double a, b, c;
    is >> a >> b >> c;
    return make_shared<Triangle>(a, b, c);
  } else if (figure == "CIRCLE") {
    double radius;
    is >> radius;
    return make_shared<Circle>(radius);
  }
  return {};
}

int main() {
  vector<shared_ptr<Figure>> figures;
  for (string line; getline(cin, line); ) {
    istringstream is(line);

    string command;
    is >> command;
    if (command == "ADD") {
      // Пропускаем "лишние" ведущие пробелы.
      // Подробнее об std::ws можно узнать здесь:
      // https://en.cppreference.com/w/cpp/io/manip/ws
      is >> ws;
      figures.push_back(CreateFigure(is));
    } else if (command == "PRINT") {
      for (const auto& current_figure : figures) {
        cout << fixed << setprecision(3)
             << current_figure->Name() << " "
             << current_figure->Perimeter() << " "
             << current_figure->Area() << endl;
      }
    }
  }
  return 0;
}
