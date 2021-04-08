#pragma once

#include "tokens.h"

#include <ostream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <map>

class ExpressionParser {
public:
  ExpressionParser(const std::string& expr) : input_expr(expr) {
  }

  int parse();

private:
  void shuntingYard(const std::shared_ptr<Token>& token);
  void next_infix_token(const std::shared_ptr<Token> token);
  void next_postfix_token(const std::shared_ptr<Token> token);
  void flush_token(Token::Type type);
  // <calc>
  void calcOperation(const std::shared_ptr<Token>& token);
  // </calc>

private:
  const std::string input_expr;
  std::stack<std::shared_ptr<Token>> tokens_stack;
  Tokens infix;
  Tokens postfix;
  std::string str;
  // <calc>
  std::stack<int> oper_stack;
  int result = 0;
  // </calc>
};
