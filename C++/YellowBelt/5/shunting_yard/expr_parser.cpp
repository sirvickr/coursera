#include "expr_parser.h"

#include <iostream>

using namespace std;

void ExpressionParser::next_infix_token(const std::shared_ptr<Token> token) {
  shuntingYard(token);
  infix.push_back(token);
}

void ExpressionParser::next_postfix_token(const std::shared_ptr<Token> token) {
  calcOperation(token);
  postfix.push_back(token);
}

void ExpressionParser::flush_token(Token::Type type) {
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
      next_infix_token(std::make_shared<Op>(op_it->second.type, op_it->second.precedence));
    }
    break;
  case Token::Type::Id: // None, Par, Op, Id, Num
    {
      // операция тоже может состоять из символов - например, and или or
      auto op_it = Op::operations.find(str);
      if(op_it == Op::operations.end())
        next_infix_token(std::make_shared<Id>(str));
      else
        next_infix_token(std::make_shared<Op>(op_it->second.type, op_it->second.precedence));
    }
    break;
  case Token::Type::Num:
    next_infix_token(std::make_shared<Num>(from_string(str, 0)));
    break;
  default:
    throw std::runtime_error("parse): unexpected type");
  }
  str.clear();
}

void ExpressionParser::shuntingYard(const std::shared_ptr<Token>& token) {
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
        tokens_stack.push(token);
        break;
      case Par::Type::Close:
        ///cout << ":";
        while(!tokens_stack.empty()) {
          auto top = tokens_stack.top();
          ///cout << " '" << top << "'";
          tokens_stack.pop();
          if(top->type == Token::Type::Par && static_pointer_cast<Par>(top)->type == Par::Type::Open)
            break; // только открывающие скобки - не роверять тип TODO
          ///cout << " -> out,";
          next_postfix_token(top);
        }
        break;
      }
    }
    break;
  case Token::Type::Op:
    {
      auto op = static_pointer_cast<Op>(token);
      ///cout << " op (p=" << curr_it->second << "):";
      while(!tokens_stack.empty()) {
        auto top = tokens_stack.top();
        ///cout << " '" << top << "'";
        if(top->type != Token::Type::Op) {
          ///cout << " not op";
          break; // не оператор
        }
        auto top_op = static_pointer_cast<Op>(top);
        ///cout << " (p=" << top_it->second << ")";
        // меньшее значение precedence - больший приоритет операции
        if(op->precedence < top_op->precedence) {
          ///cout << " low";
          break; // оператор меньшим приоритетом
        }
        // перекнуть из стека в выходную очередь
        ///cout << " >>>";
        next_postfix_token(top);
        tokens_stack.pop();
      }
      tokens_stack.push(token);
    }
    break;
  case Token::Type::Id:
    break;
  case Token::Type::Num:
    ///cout << " 'num' >>>";
    next_postfix_token(token);
    break;
  default:
    throw runtime_error("toPostfix: unexpected default state");
    break;
  }
}

int ExpressionParser::parse() {
  auto first = begin(input_expr);
  auto last = end(input_expr);
  Token::Type currState = Token::Type::None;
  for(auto curr = first; curr != last; ++curr) {
    ///cout << *curr << "\t";
    switch(currState) {
    case Token::Type::None:
      if(*curr == '(') {
        next_infix_token(std::make_shared<Par>(Par::Type::Open));
      } else if(*curr == ')') {
        next_infix_token(std::make_shared<Par>(Par::Type::Close));
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
        next_infix_token(std::make_shared<Par>(Par::Type::Open));
        currState = Token::Type::None;
      } else if(*curr == ')') {
        flush_token(currState);
        next_infix_token(std::make_shared<Par>(Par::Type::Close));
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
        next_infix_token(std::make_shared<Par>(Par::Type::Open));
        currState = Token::Type::None;
      } else if(*curr == ')') {
        flush_token(currState);
        next_infix_token(std::make_shared<Par>(Par::Type::Close));
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
        next_infix_token(std::make_shared<Par>(Par::Type::Open));
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
  // <postfix>
  while(!tokens_stack.empty()) {
    next_postfix_token(tokens_stack.top());
    tokens_stack.pop();
  }
  // </postfix>
  //return infix;
  // <calc>
  if(oper_stack.size() != 1) {
    throw runtime_error("stack size = " + to_string(oper_stack.size()) + " (must be 1)");
  }
  //cout << "parse: opstack size =  " << oper_stack.size() << " result = " << oper_stack.top() << endl;
  // <.calc>
  return result;
}

void ExpressionParser::calcOperation(const shared_ptr<Token>& token) {
//    cout << "'" << token->str() << "':\t"; cout.flush();
    switch(token->type) {
    case Token::Type::Op:
      {
        if(oper_stack.empty())
          throw(runtime_error("stack is empty: a not found"));
        int b = oper_stack.top();
        oper_stack.pop();
//        cout << b << token->str();

        if(oper_stack.empty())
          throw(runtime_error("stack is empty: b not found"));
        int a = oper_stack.top();
        oper_stack.pop();
//        cout << a << "=";

        switch(static_pointer_cast<Op>(token)->type) {
          case Op::Type::Mul:  result = a * b;   break;
          case Op::Type::Div:  result = a / b;   break;
          case Op::Type::Add:  result = a + b;   break;
          case Op::Type::Sub:  result = a - b;   break;

          case Op::Type::Less: result = a <  b;  break;
          case Op::Type::LE:   result = a <= b;  break;
          case Op::Type::More: result = a > b;   break;
          case Op::Type::ME:   result = a >= b;  break;
          case Op::Type::Eq:   result = a == b;  break;
          case Op::Type::NE:   result = a != b;  break;

          case Op::Type::And:  result = a && b;  break;
          case Op::Type::Or:   result = a || b;  break;
        }
//        cout << result;
        oper_stack.push(result);
      }
      break;
    case Token::Type::Id:
      throw runtime_error("toPostfix: Id is not implemented");
      break;
    case Token::Type::Num:
      oper_stack.push(static_pointer_cast<Num>(token)->value);
      break;
    default:
      throw runtime_error("toPostfix: unexpected default state");
      break;
    }
//    cout/* << "\t(" << oper_stack.size() << ")"*/ << endl;
}
