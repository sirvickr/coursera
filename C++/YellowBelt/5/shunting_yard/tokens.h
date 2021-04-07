#pragma once

#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

struct Token {
  enum class Type{ None, Par, Op, Id, Num };
  const Type type;
  Token(Type type) : type(type) {
  }
  virtual ~Token() = 0;
  virtual std::string str() const = 0;
};

struct Par: Token {
  enum class Type{ Open, Close };
  const Type type;
  Par(Type type): Token(Token::Type::Par), type(type) {
  }
  std::string str() const override {
    return type == Type::Open ? "(" : ")";
  }
};

struct Op: Token {
  enum class Type{ Add, Sub, Mul, Div, And, Or };
  struct Traits {
    Type type;
    int precedence;
  };
  static const std::map<std::string, Traits> operations;
  const Type type;
  int precedence;
  Op(Type type, int precedence=0): Token(Token::Type::Op), type(type), precedence(precedence) {
  }
  std::string str() const override {
    switch(type) {
      case Type::Add:  return "+";
      case Type::Sub:  return "-";
      case Type::Mul:  return "*";
      case Type::Div:  return "/";
      case Type::And:  return "And";
      case Type::Or:   return "Or";
    }
    return "Op::str::unexpected_type";
  }
};

struct Id: Token {
  std::string name;
  int value;
  Id(const std::string& name, int value = 0): Token(Token::Type::Id), name(name), value(value) {
  }
  std::string str() const override {
    return name;
  }
};

struct Num: Token {
  int value;
  Num(int value = 0): Token(Token::Type::Num), value(value) {
  }
  std::string str() const override {
    return std::to_string(value);
  }
};

using Tokens = std::vector<std::shared_ptr<Token>>;
Tokens toPostfix(const Tokens& infix);
int calcExpr(const Tokens& postfix);

std::ostream& operator<<(std::ostream& stream, const std::vector<std::shared_ptr<Token>>& v);
std::ostream& operator<<(std::ostream& stream, Op::Type type);
