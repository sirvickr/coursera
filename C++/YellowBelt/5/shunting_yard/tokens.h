#pragma once

#include <ostream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <stack>
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
  enum class Type{ Mul, Div, Add, Sub, Less, LE, More, ME, Eq, NE, And, Or };
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
      case Type::Mul:  return "*";
      case Type::Div:  return "/";
      case Type::Add:  return "+";
      case Type::Sub:  return "-";

      case Type::Less: return "<";
      case Type::LE:   return "<=";
      case Type::More: return ">";
      case Type::ME:   return ">=";
      case Type::Eq:   return "==";
      case Type::NE:   return "!=";

      case Type::And:  return "and";
      case Type::Or:   return "or";
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

template<typename T>
T from_string(const std::string& s, const T& default_value = T()) {
  std::istringstream iss(s);
  T value = default_value;
  iss >> value;
  return value;
}

#define PARSE_TO_POSTFIX

#ifdef PARSE_TO_POSTFIX
void shuntingYard(std::stack<std::shared_ptr<Token>>& opstack, Tokens& postfix, const std::shared_ptr<Token>& token);
#endif
Tokens toPostfix(const Tokens& infix);
int calcExpr(const Tokens& postfix);

template<typename InputIt>
Tokens parse(InputIt first, InputIt last, Tokens& postfix) {
#ifdef PARSE_TO_POSTFIX
  // <postfix>
  std::stack<std::shared_ptr<Token>> opstack;
  ///Tokens postfix;
  // </postfix>
#endif
  Tokens infix;
  std::string str;
  auto push_token = [&](const std::shared_ptr<Token> token) {
#ifdef PARSE_TO_POSTFIX
    // <postfix>
    shuntingYard(opstack, postfix, token);
    // </postfix>
#endif
    infix.push_back(token);
  };
  auto flush_token = [&](Token::Type type) {
    if(str.empty()) {
      throw std::runtime_error("parse: empty token string");
    }
    switch(type) {
    case Token::Type::Op: // None, Par, Op, Id, Num
      {
        auto op_it = Op::operations.find(str);
        if(op_it == Op::operations.end()) {
          std::ostringstream oss;
          oss << "flush_token: operation not found: [" << str << "]: ";
          throw std::runtime_error(oss.str());
        }
        push_token(std::make_shared<Op>(op_it->second.type, op_it->second.precedence));
      }
      break;
    case Token::Type::Id: // None, Par, Op, Id, Num
      {
        // операция тоже может состоять из символов - например, and или or
        auto op_it = Op::operations.find(str);
        if(op_it == Op::operations.end())
          push_token(std::make_shared<Id>(str));
        else
          push_token(std::make_shared<Op>(op_it->second.type, op_it->second.precedence));
      }
      break;
    case Token::Type::Num:
      push_token(std::make_shared<Num>(from_string(str, 0)));
      break;
    default:
      throw std::runtime_error("parse): unexpected type");
    }
    str.clear();
  };
  Token::Type currState = Token::Type::None;
  for(auto curr = first; curr != last; ++curr) {
    ///cout << *curr << "\t";
    switch(currState) {
    case Token::Type::None:
      if(*curr == '(') {
        push_token(std::make_shared<Par>(Par::Type::Open));
      } else if(*curr == ')') {
        push_token(std::make_shared<Par>(Par::Type::Close));
      } else if(*curr == '+') {
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '-') {
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '*') {
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '/') {
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '<') {
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '>') {
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '=') {
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '!') {
        str += *curr;
        currState = Token::Type::Op;
      } else if(isdigit(*curr)) {
        str += *curr;
        currState = Token::Type::Num;
      } else if(isalpha(*curr)) {
        str += *curr;
        currState = Token::Type::Id; // variable or multi-literal operation
      } else { // space?
      }
      break;
    case Token::Type::Num:
      if(*curr == '(') {
        flush_token(currState);
        push_token(std::make_shared<Par>(Par::Type::Open));
        currState = Token::Type::None;
      } else if(*curr == ')') {
        flush_token(currState);
        push_token(std::make_shared<Par>(Par::Type::Close));
        currState = Token::Type::None;
      } else if(*curr == '+') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '-') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '*') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '/') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '<') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '>') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '=') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '!') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(isdigit(*curr)) {
        str += *curr;
      } else if(isalpha(*curr)) {
        throw std::runtime_error("letter after digit");
      } else { // space?
        flush_token(currState);
        currState = Token::Type::None;
      }
      break;
    case Token::Type::Id:
      if(*curr == '(') {
        flush_token(currState);
        push_token(std::make_shared<Par>(Par::Type::Open));
        currState = Token::Type::None;
      } else if(*curr == ')') {
        flush_token(currState);
        push_token(std::make_shared<Par>(Par::Type::Close));
        currState = Token::Type::None;
      } else if(*curr == '+') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '-') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '*') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '/') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '>') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(*curr == '<') {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Op;
      } else if(isdigit(*curr)) {
        str += *curr;
      } else if(isalpha(*curr)) {
        str += *curr;
      } else { // space?
        flush_token(currState);
        currState = Token::Type::None;
      }
      break;
    case Token::Type::Op:
      if(*curr == '(') {
        flush_token(currState);
        push_token(std::make_shared<Par>(Par::Type::Open));
        currState = Token::Type::None;
      } else if(*curr == ')') {
        // возможно только для унарных операторов
        throw std::runtime_error("Type::Op: unexpected closing parenthesis");
      } else if(*curr == '+') {
        // ++ не реализован
        throw std::runtime_error("Type::Op: unexpected '+'");
      } else if(*curr == '-') {
        // -- не реализован
        throw std::runtime_error("Type::Op: unexpected '+'");
      } else if(*curr == '*') {
        throw std::runtime_error("Type::Op: unexpected '*'");
      } else if(*curr == '/') {
        // // не реализован
        throw std::runtime_error("Type::Op: unexpected '/'");
      } else if(*curr == '<') {
        // << не реализован
        throw std::runtime_error("Type::Op: unexpected '<'");
      } else if(*curr == '>') {
        // >> не реализован
        throw std::runtime_error("Type::Op: unexpected '>'");
      } else if(*curr == '=') {
        str += *curr;
      } else if(*curr == '!') {
        throw std::runtime_error("Type::Op: unexpected '!'");
      } else if(isdigit(*curr)) {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Num;
      } else if(isalpha(*curr)) {
        flush_token(currState);
        str += *curr;
        currState = Token::Type::Id;
      } else { // space?
        flush_token(currState);
        currState = Token::Type::None;
      }
      break;
    case Token::Type::Par:
      throw std::runtime_error("unexpected state: Par");
      break;
    }
    ///cout << endl;
  }
  if(!str.empty())
    flush_token(currState);
#ifdef PARSE_TO_POSTFIX
  // <postfix>
  while(!opstack.empty()) {
    postfix.push_back(opstack.top());
    opstack.pop();
  }
  // </postfix>
#endif
  return infix;
}

std::ostream& operator<<(std::ostream& stream, const std::vector<std::shared_ptr<Token>>& v);
std::ostream& operator<<(std::ostream& stream, Op::Type type);
