#include "Common.h"

using namespace std;

// Класс, представляющий конкретное число - лист дерева выражения
class ValueExpr: public Expression {
public:
  ValueExpr(int value) : value_(value) {}

  int Evaluate() const override {
    return value_;
  }

  string ToString() const override {
    return to_string(value_);
  }

private:
  int value_;
};

// Базовый класс бинарных операций
class BinaryExpr: public Expression {
public:
  BinaryExpr(ExpressionPtr left, ExpressionPtr right)
    : left_(move(left)), right_(move(right))
  {}

protected:
  ExpressionPtr left_, right_;
};

// Класс для операции умножения
struct ProductExpr: public BinaryExpr {
  ProductExpr(ExpressionPtr left, ExpressionPtr right)
    : BinaryExpr(move(left), move(right))
  {}

  int Evaluate() const override {
    return left_->Evaluate() * right_->Evaluate();
  }

  string ToString() const override {
    return "(" + left_->ToString() + ")*(" + right_->ToString() + ")";
  }
};

// Класс для операции сложения
struct SumExpr: public BinaryExpr {
  SumExpr(ExpressionPtr left, ExpressionPtr right)
    : BinaryExpr(move(left), move(right))
  {}

  int Evaluate() const override {
    return left_->Evaluate() + right_->Evaluate();
  }

  string ToString() const override {
    return "(" + left_->ToString() + ")+(" + right_->ToString() + ")";
  }
};

// Функции для формирования выражения
ExpressionPtr Value(int value) {
  return make_unique<ValueExpr>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<SumExpr>(move(left), move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<ProductExpr>(move(left), move(right));
}
