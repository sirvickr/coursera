#include "test_runner.h"
#include "simple_parser.h"
#include "tokens.h"

#include <algorithm>
#include <iterator>
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

/*class Node {
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
};*/

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

  Tokens pf;
  //Tokens infix = parser.parse();
  Tokens infix = parse(begin(line), end(line), pf);
  cout << "infix: " << infix << endl;

  auto postfix = toPostfix(infix);
  cout << "postfix:" << postfix << endl;
  cout << "postfix:" << pf << endl;

  auto result = calcExpr(postfix);
  cout << "RESULT (complex): " << result << endl;
}

int main() {
  UnitTests();

  string line;
  while(getline(cin, line)) {
    if(line.front() == '#')
      continue;
    //cout << "parsing line: " << line << endl;
    complex(line);
    //simple(line);
  }
  return 0;
}

template<typename Sequence>
void output(std::ostream& out, const Sequence& seq, const std::string& delim = " ", bool flush = false) {
  std::copy(begin(seq), end(seq), std::ostream_iterator<typename Sequence::value_type>(out, delim.c_str()));
}

vector<string> tokenise(const string& s) {
  vector<string> v;
  istringstream iss(s);
  string token;
  while(getline(iss, token, ' '))
    v.push_back(token);
  //output(cout, v, " "); cout << endl;
  return v;
}

void TestSimpleParser(const string& input, const vector<string>& expected) {
  AssertEqual(parseSimple(begin(input), end(input)), expected, "testing expression \"" + input + "\" (simple)");
}

void TestComplexParser(const string& input, const string& infix, const string& postfix, int result) {
  Tokens pf;
  Tokens infix_expr = parse(begin(input), end(input), pf);
  {
    ostringstream oss;
    oss << infix_expr;
    AssertEqual(oss.str(), infix, "testing infix expression \"" + input + "\"");
  }
  auto postfix_expr = toPostfix(infix_expr);
  AssertEqual(postfix_expr, pf, "comparing postfix expressions for \"" + input + "\"");
  {
    ostringstream oss;
    oss << postfix_expr;
    AssertEqual(oss.str(), postfix, "testing postfix expression for \"" + input + "\"");
  }
  {
    auto expr_result = calcExpr(postfix_expr);
    AssertEqual(expr_result, result, "testing result of expression \"" + input + "\"");
  }
}

/*void SimpleParserTest() {
  TestSimpleParser( "( 1 + 2 + 4 ) * 5 + 3 - 8 * 1 * 2", {"(", "1", "+", "2", "+", "4", ")", "*", "5", "+", "3", "-", "8", "*", "1", "*", "2",} );
  TestSimpleParser( "  (1+2+4)*5 + 3 - 8 * 1 * 2 ",      {"(", "1", "+", "2", "+", "4", ")", "*", "5", "+", "3", "-", "8", "*", "1", "*", "2",} );
  TestSimpleParser( "(3+5)*10-17*2",                     {"(", "3", "+", "5", ")", "*", "10", "-", "17", "*", "2",} );
}

void ParserTest() {
  TestComplexParser("( 1 + 2 + 4 ) * 5 + 3 - 8 * 1 * 2", "( 1 + 2 + 4 ) * 5 + 3 - 8 * 1 * 2", "1 2 + 4 + 5 * 3 + 8 1 * 2 * -");
  TestComplexParser("  (1+2+4)*5 + 3 - 8 * 1 * 2 ",      "( 1 + 2 + 4 ) * 5 + 3 - 8 * 1 * 2", "1 2 + 4 + 5 * 3 + 8 1 * 2 * -");
  TestComplexParser("(3 + 5) * 10 - 17 * 2",             "( 3 + 5 ) * 10 - 17 * 2", "3 5 + 10 * 17 2 * -");
}*/

void TestExpression(const string& expr, const string& infix, const string& postfix, int result) {
  //TestSimpleParser(expr, tokenise(infix)); // and, or не реализованы
  TestComplexParser(expr, infix, postfix, result);
}

void Test1() {
  auto distortion = [](const string& infix, const string& postfix, int result) {
    TestExpression("( 1 + 2 + 4 ) * 5 + 3 - 8 * 1 * 2", infix, postfix, result);
    TestExpression("  (1+2+4)*5 + 3 - 8 * 1 * 2 ",      infix, postfix, result);
    TestExpression(" ( 1+2 +4)*5 + 3 - 8 * 1* 2",       infix, postfix, result);
  };
  distortion("( 1 + 2 + 4 ) * 5 + 3 - 8 * 1 * 2", "1 2 + 4 + 5 * 3 + 8 1 * 2 * -", 22);
}

void Test2() {
  auto distortion = [](const string& infix, const string& postfix, int result) {
    TestExpression("(3 + 5) * 10 - 17 * 2", infix, postfix, result);
  };
  distortion("( 3 + 5 ) * 10 - 17 * 2", "3 5 + 10 * 17 2 * -", 46);
}

void Test3() {
  auto distortion = [](const string& infix, const string& postfix, int result) {
    TestExpression("(0 and 1 or 4) - 20", infix, postfix, result);
    TestExpression(" ( 0 and 1 or 4 ) - 20 ", infix, postfix, result);
  };
  distortion("( 0 and 1 or 4 ) - 20", "0 1 and 4 or 20 -", -19);
}

void Test4() {
  auto distortion = [](const string& infix, const string& postfix, int result) {
    TestExpression("1 + ((2 < 2 or 4 <= 4) and (1 < 4)) - 5", infix, postfix, result);
  };
  distortion("1 + ( ( 2 < 2 or 4 <= 4 ) and ( 1 < 4 ) ) - 5", "1 2 2 < 4 4 <= or 1 4 < and + 5 -", -3);
}

void Test5() {
  auto distortion = [](const string& infix, const string& postfix, int result) {
    TestExpression("1 + ( ( 2 < 2 or 4 < 4 ) and ( 1 < 4 ) ) - 5", infix, postfix, result);
  };
  distortion("1 + ( ( 2 < 2 or 4 < 4 ) and ( 1 < 4 ) ) - 5", "1 2 2 < 4 4 < or 1 4 < and + 5 -", -4);
}

void UnitTests() {
  TestRunner tr;
  //tr.RunTest(SimpleParserTest, "SimpleParserTest");
  //tr.RunTest(ParserTest, "ParserTest");
  tr.RunTest(Test1, "Test1");
  tr.RunTest(Test2, "Test2");
  tr.RunTest(Test3, "Test3");
  tr.RunTest(Test4, "Test4");
  tr.RunTest(Test5, "Test5");
}
