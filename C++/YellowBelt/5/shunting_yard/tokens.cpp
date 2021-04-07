#include "tokens.h"

#include <stack>
#include <iostream>

using namespace std;

const map<string, Op::Traits> Op::operations = {
    { "+",   { Op::Type::Add, 11 } },
    { "-",   { Op::Type::Sub, 11 } },
    { "*",   { Op::Type::Mul, 12 } },
    { "/",   { Op::Type::Div, 12 } },
    { "AND", { Op::Type::And, 4 } },
    { "OR",  { Op::Type::Or,  3 } }
};

Token::~Token() {
}

Tokens toPostfix(const Tokens& infix) {
  Tokens postfix;

  stack<shared_ptr<Token>> opstack;

  for(const auto& token: infix) {
    ///cout << token << "\t
    //None, Par, Op, Id, Num";
    switch(token->type) {
    case Token::Type::None:
      throw runtime_error("toPostfix: ran into None amid tokens");
      break;
    case Token::Type::Par:
      {
        auto par = static_pointer_cast<Par>(token);
        switch(par->type) {
        case Par::Type::Open:
          ///cout << " >> stack";
          opstack.push(token);
          break;
        case Par::Type::Close:
          ///cout << ":";
          while(!opstack.empty()) {
            auto top = opstack.top();
            ///cout << " '" << top << "'";
            opstack.pop();
            if(top->type == Token::Type::Par && static_pointer_cast<Par>(top)->type == Par::Type::Open)
              break; // только открывающие скобки - не роверять тип TODO
            ///cout << " -> out,";
            postfix.push_back(top);
          }
          break;
        }
      }
      break;
    case Token::Type::Op:
      {
        auto op = static_pointer_cast<Op>(token);
        ///cout << " op (p=" << curr_it->second << "):";
        while(!opstack.empty()) {
          auto top = opstack.top();
          ///cout << " '" << top << "'";
          if(top->type != Token::Type::Op) {
            ///cout << " not op";
            break; // не оператор
          }
          auto top_op = static_pointer_cast<Op>(top);
          ///cout << " (p=" << top_it->second << ")";
          if(op->precedence > top_op->precedence) {
            ///cout << " low";
            break; // оператор меньшим приоритетом
          }
          // перекнуть из стека в выходную очередь
          ///cout << " >>>";
          postfix.push_back(top);
          opstack.pop();
        }
        opstack.push(token);
      }
      break;
    case Token::Type::Id:
      break;
    case Token::Type::Num:
      ///cout << " 'num' >>>";
      postfix.push_back(token);
      break;
    default:
      throw runtime_error("toPostfix: unexpected default state");
      break;
    }
  } // for
  while(!opstack.empty()) {
    postfix.push_back(opstack.top());
    opstack.pop();
  }

  return postfix;
}

int calcExpr(const Tokens& postfix) {
  int result = 0;
  stack<int> opstack;
  for(const auto& token: postfix) {
    cout << "'" << token->str() << "':\t";
    cout.flush();
    switch(token->type) {
    //case Token::Type::None:
      //throw runtime_error("toPostfix: ran into None amid tokens");
      //break;
    //case Token::Type::Par:
      //break;
    case Token::Type::Op:
      {
        if(opstack.empty())
          throw(runtime_error("stack is empty: a not found"));
        int b = opstack.top();
        opstack.pop();
        cout << b;

        cout << token->str();

        if(opstack.empty())
          throw(runtime_error("stack is empty: b not found"));
        int a = opstack.top();
        opstack.pop();
        cout << a;

        cout << "=";

        switch(static_pointer_cast<Op>(token)->type) {
          case Op::Type::Add:  result = a + b;   break;
          case Op::Type::Sub:  result = a - b;   break;
          case Op::Type::Mul:  result = a * b;   break;
          case Op::Type::Div:  result = a / b;   break;
          case Op::Type::And:  result = a && b;  break;
          case Op::Type::Or:   result = a || b;  break;
        }
        cout << result;
        opstack.push(result);
      }
      break;
    case Token::Type::Id:
      throw runtime_error("toPostfix: Id is not implemented");
      break;
    case Token::Type::Num:
      opstack.push(static_pointer_cast<Num>(token)->value);
      break;
    default:
      throw runtime_error("toPostfix: unexpected default state");
      break;
    }
    cout/* << "\t(" << opstack.size() << ")"*/ << endl;
  }
  if(opstack.size() != 1)
    throw runtime_error("tail stack size is " + to_string(opstack.size()));
  cout << "tail: " << opstack.top() << endl;
  result = opstack.top();
  opstack.pop();
  return result;
}

ostream& operator<<(ostream& stream, const vector<shared_ptr<Token>>& v) {
  bool first = true;
  for(const auto& token: v) {
    if(!first)
      stream << ' ';
    first = false;
    stream << token->str();
  }
  return stream;
}

ostream& operator<<(ostream& stream, Op::Type type) {
  switch(type) {
    case Op::Type::Add:  stream << "+";   break;
    case Op::Type::Sub:  stream << "-";   break;
    case Op::Type::Mul:  stream << "*";   break;
    case Op::Type::Div:  stream << "/";   break;
    case Op::Type::And:  stream << "And"; break;
    case Op::Type::Or:   stream << "Or";  break;
  }
  return stream;
}
