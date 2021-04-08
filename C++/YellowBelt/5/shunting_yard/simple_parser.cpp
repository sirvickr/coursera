#include "simple_parser.h"

#include <map>
#include <stack>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

static bool is_number(const string& s)
{
  if(s.empty())
    return false;
  return find_if(begin(s), end(s), [](auto c) { return !isdigit(c); }) == end(s);
}

vector<string> toPostfixSimple(const vector<string>& infix) {
  const map<string, int> precedence = {
      { "+", 0 },
      { "-", 0 },
      { "*", 1 },
      { "/", 1 },
  };

  vector<string> postfix;

  stack<string> opstack;

  for(const auto& token: infix) {
    ///cout << token << "\t";
    if(is_number(token)) { // константа
      istringstream iss(token);
      int d;
      if(iss >> d) {
        //nodes.push_back(make_shared<Number>(d));
        ///cout << " 'num' >>>";
        postfix.push_back(token);
      }
    } else if("(" == token) {
      ///cout << " >> stack";
      opstack.push(token);
    } else if(")" == token) {
      ///cout << ":";
      while(!opstack.empty()) {
        auto top = opstack.top();
        ///cout << " '" << top << "'";
        opstack.pop();
        if(top == "(")
          break;
        ///cout << " -> out,";
        postfix.push_back(top);
      }
    } else { // оператор
      auto curr_it = precedence.find(token);
      if(curr_it != precedence.end()) {
        ///cout << " op (p=" << curr_it->second << "):";
        while(!opstack.empty()) {
          auto top = opstack.top();
          ///cout << " '" << top << "'";
          auto top_it = precedence.find(top);
          if(top_it == precedence.end()) {
            ///cout << " not op";
            break; // не оператор
          }
          ///cout << " (p=" << top_it->second << ")";
          if(curr_it->second > top_it->second) {
            ///cout << " low";
            break; // оператор меньшим приоритетом
          }
          // перекнуть из стека в выходную очередь
          ///cout << " >>>";
          postfix.push_back(top);
          opstack.pop();
        }
        opstack.push(token);
      } else {
        throw std::runtime_error("invalid token");
      }
    }
  }
  while(!opstack.empty()) {
    postfix.push_back(opstack.top());
    opstack.pop();
  }

  return postfix;
}

int simpleCalc(const vector<string>& postfix) {
  int result = 0;
  stack<int> opstack;
  for(const auto& token: postfix) {
    //cout << "'" << token << "':\t";
    //cout.flush();
    if(is_number(token)) { // константа
      istringstream iss(token);
      int d;
      if(iss >> d) {
        opstack.push(d);
      }
    } else { // оператор
      if(opstack.empty())
        throw(runtime_error("stack is empty: a not found"));
      int b = opstack.top();
      opstack.pop();
      //cout << b;

      //cout << token;

      if(opstack.empty())
        throw(runtime_error("stack is empty: b not found"));
      int a = opstack.top();
      opstack.pop();
      //cout << a;

      //cout << "=";

      if(token == "+") {
        result = a + b;
      } else if(token == "-") {
        result = a - b;
      } else if(token == "*") {
        result = a * b;
      } else if(token == "/") {
        result = a / b;
      }
      //cout << result;
      opstack.push(result);
    }
    //cout/* << "\t(" << opstack.size() << ")"*/ << endl;
  }
#if 1
  if(opstack.size() != 1)
    throw runtime_error("tail stack size is " + to_string(opstack.size()));
  //cout << "tail: " << opstack.top() << endl;
  result = opstack.top();
  opstack.pop();
#else
  while(!opstack.empty()) {
    cout << "tail: " << opstack.top() << endl;
    result = opstack.top();
    opstack.pop();
  }
#endif
  return result;
}
