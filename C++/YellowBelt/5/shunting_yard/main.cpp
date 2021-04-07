#include "test_runner.h"
#include "simple_parser.h"
#include "tokens.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <memory>
#include <map>
#include <set>

using namespace std;

void UnitTests();

ostream& operator<<(ostream& stream, const vector<string>& v) {
  for(const auto& item: v)
    stream << " " << item;
  return stream;
}

class Node {
public:
  virtual ~Node() = 0;
  virtual int eval() const = 0;
};

Node::~Node() {
}

class Number : public Node {
public:
  Number(int value): _value(value) {
  }
  int eval() const override {
    return _value;
  }

private:
  int _value;
};

class Variable : public Node {
public:
  Variable(int& x): _x(x) {
  }
  int eval() const override {
    return _x;
  }

private:
  const int& _x;
};

class Oper : public Node {
public:
  enum class Type{ null, add, sub, mul, div };
  Oper(const std::string& token)
  {
    _op = _operations.find(token);
    if(_op == _operations.end()) {
      throw std::runtime_error("wrong operation token");
    }
  }
  std::shared_ptr<Node> lhs() const {
    return _lhs;
  }
  void lhs(std::shared_ptr<Node> value) {
   _lhs = value;
  }
  void rhs(std::shared_ptr<Node> value) {
   _rhs = value;
  }
  std::shared_ptr<Node> rhs() const {
    return _rhs;
  }
  int precedence() const {
    return _op->second.precedence;
  }
  int eval() const override {
    switch(_op->second.type) {
    case Type::add:
      return _lhs->eval() + _rhs->eval();
    case Type::sub:
      return _lhs->eval() - _rhs->eval();
    case Type::mul:
      return _lhs->eval() * _rhs->eval();
    case Type::div:
      return _lhs->eval() / _rhs->eval();
    case Type::null:
      break;
    }
    throw std::runtime_error("unspecified op code");
  }
private:
  struct OpConf {
    Type type;
    int precedence;
    //std::function TODO
  };
  using Operations = map<string, OpConf>;
  const Operations _operations = {
      { "+", { Type::add, 0 } },
      { "-", { Type::sub, 0 } },
      { "*", { Type::mul, 1 } },
      { "/", { Type::div, 1 } },
  };
  Operations::const_iterator _op;
  std::shared_ptr<Node> _lhs, _rhs;
};

template<typename T>
T from_string(const string& s, const T& default_value = T()) {
  istringstream iss(s);
  T value = default_value;
  iss >> value;
  return value;
}

template<typename InputIt>
Tokens parse(InputIt first, InputIt last) {
  Tokens infix;
  string token;
  shared_ptr<Token> p;
  auto flush_token = [&](Token::Type type) {
    if(token.empty()) {
      throw runtime_error("parse: empty token");
    }
    switch(type) {
    case Token::Type::Op: // None, Par, Op, Id, Num
      {
        auto op_it = Op::operations.find(token);
        if(op_it == Op::operations.end()) {
          ostringstream oss;
          oss << "flush_token: operation not found: [" << token << "]: ";
          throw runtime_error(oss.str());
        }
        infix.push_back(make_shared<Op>(op_it->second.type, op_it->second.precedence));
      }
      break;
    case Token::Type::Id: // None, Par, Op, Id, Num
      {
        auto op_it = Op::operations.find(token);
        if(op_it != Op::operations.end())
          infix.push_back(make_shared<Op>(op_it->second.type, op_it->second.precedence));
        else
          infix.push_back(make_shared<Id>(token));
      }
      break;
    case Token::Type::Num:
      infix.push_back(make_shared<Num>(from_string(token, 0)));
      break;
    default:
      throw runtime_error("parse): unexpected type");
    }
    token.clear();
  };
  //Token::Type prevState = Token::Type::None;
  Token::Type currState = Token::Type::None;
  for(InputIt curr = first; curr != last; ++curr) {
    ///cout << *curr << "\t";
    switch(currState) {
    case Token::Type::None:
      if(*curr == '(') {
        infix.push_back(make_shared<Par>(Par::Type::Open));
      } else if(*curr == ')') {
        infix.push_back(make_shared<Par>(Par::Type::Close));
      } else if(*curr == '+') {
        token += *curr;
        flush_token(Token::Type::Op);
      } else if(*curr == '-') {
        token += *curr;
        flush_token(Token::Type::Op);
      } else if(*curr == '*') {
        token += *curr;
        flush_token(Token::Type::Op);
      } else if(*curr == '/') {
        token += *curr;
        flush_token(Token::Type::Op);
      } else if(isdigit(*curr)) {
        token += *curr;
        currState = Token::Type::Num;
      } else if(isalpha(*curr)) {
        token += *curr;
        currState = Token::Type::Id; // variable or multi-literal operation
      } else { // space?
      }
      break;
    case Token::Type::Num:
      if(*curr == '(') {
        flush_token(currState);
        infix.push_back(make_shared<Par>(Par::Type::Open));
        currState = Token::Type::None;
      } else if(*curr == ')') {
        flush_token(currState);
        infix.push_back(make_shared<Par>(Par::Type::Close));
        currState = Token::Type::None;
      } else if(*curr == '+') {
        flush_token(currState);
        token += *curr;
        flush_token(Token::Type::Op);
        currState = Token::Type::None;
      } else if(*curr == '-') {
        flush_token(currState);
        token += *curr;
        flush_token(Token::Type::Op);
        currState = Token::Type::None;
      } else if(*curr == '*') {
        flush_token(currState);
        token += *curr;
        flush_token(Token::Type::Op);
        currState = Token::Type::None;
      } else if(*curr == '/') {
        flush_token(currState);
        token += *curr;
        flush_token(Token::Type::Op);
        currState = Token::Type::None;
      } else if(isdigit(*curr)) {
        token += *curr;
      } else if(isalpha(*curr)) {
        throw runtime_error("letter after digit");
      } else { // space?
        flush_token(currState);
        currState = Token::Type::None;
      }
      break;
    case Token::Type::Id:
      if(*curr == '(') {
        flush_token(currState);
        infix.push_back(make_shared<Par>(Par::Type::Open));
        currState = Token::Type::None;
      } else if(*curr == ')') {
        flush_token(currState);
        infix.push_back(make_shared<Par>(Par::Type::Close));
        currState = Token::Type::None;
      } else if(*curr == '+') {
        flush_token(currState);
        infix.push_back(make_shared<Op>(Op::Type::Add));
        currState = Token::Type::None;
      } else if(*curr == '-') {
        flush_token(currState);
        infix.push_back(make_shared<Op>(Op::Type::Sub));
        currState = Token::Type::None;
      } else if(*curr == '*') {
        flush_token(currState);
        infix.push_back(make_shared<Op>(Op::Type::Mul));
        currState = Token::Type::None;
      } else if(*curr == '/') {
        flush_token(currState);
        infix.push_back(make_shared<Op>(Op::Type::Div));
        currState = Token::Type::None;
      } else if(isdigit(*curr)) {
        token += *curr;
      } else if(isalpha(*curr)) {
        token += *curr;
      } else { // space?
        flush_token(currState);
        currState = Token::Type::None;
      }
      break;
    case Token::Type::Op:
      throw runtime_error("unexpected state: Op");
      break;
    case Token::Type::Par:
      throw runtime_error("unexpected state: Par");
      break;
    }
    ///cout << endl;
  }
  if(!token.empty())
    flush_token(currState);
  return infix;
}

void simple(const string line) {
  cout << "\nsimple: " << line << endl;

  auto infix = parseSimple(begin(line), end(line));
  cout << "infix:" << infix << endl;

  vector<string> postfix = toPostfixSimple(infix);
  cout << "POSTFIX:" << postfix << endl;

  auto result = simpleCalc(postfix);
  cout << "RESULT: " << result << endl;
}

void complex(const string& line) {
  cout << "\ncomplex: " << line << endl;

  Tokens infix = parse(begin(line), end(line));
  cout << "infix: " << infix << endl;

  auto postfix = toPostfix(infix);
  cout << "postfix:" << postfix << endl;

  auto result = calcExpr(postfix);
  cout << "RESULT: " << result << endl;
}

int main() {
  UnitTests();

  string line;
  while(getline(cin, line)) {
    if(line.front() == '#')
      continue;
    //cout << "parsing line: " << line << endl;
    complex(line);
    simple(line);
  }
  return 0;
}

void TestSimpleParser(const string& input, const vector<string>& expected) {
  AssertEqual(parseSimple(begin(input), end(input)), expected, "testing expression \"" + input + "\" (simple)");
}

void TestParser(const string& input, const string& expected) {
  ostringstream oss;
  oss << parse(begin(input), end(input));
  AssertEqual(oss.str(), expected, "testing expression \"" + input + "\"");
}

void SimpleParserTest() {
  TestSimpleParser( "( 1 + 2 + 4 ) * 5 + 3 - 8 * 1 * 2", {"(", "1", "+", "2", "+", "4", ")", "*", "5", "+", "3", "-", "8", "*", "1", "*", "2",} );
  TestSimpleParser( "  (1+2+4)*5 + 3 - 8 * 1 * 2 ",      {"(", "1", "+", "2", "+", "4", ")", "*", "5", "+", "3", "-", "8", "*", "1", "*", "2",} );
  TestSimpleParser( "(3+5)*10-17*2",                     {"(", "3", "+", "5", ")", "*", "10", "-", "17", "*", "2",} );
}

void ParserTest() {
  TestParser("( 1 + 2 + 4 ) * 5 + 3 - 8 * 1 * 2", "( 1 + 2 + 4 ) * 5 + 3 - 8 * 1 * 2");
  TestParser("  (1+2+4)*5 + 3 - 8 * 1 * 2 ",      "( 1 + 2 + 4 ) * 5 + 3 - 8 * 1 * 2");
  TestParser("(3 + 5) * 10 - 17 * 2",             "( 3 + 5 ) * 10 - 17 * 2");
}

void UnitTests() {
  TestRunner tr;
  tr.RunTest(SimpleParserTest, "SimpleParserTest");
  tr.RunTest(ParserTest, "ParserTest");
}
