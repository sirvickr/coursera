#pragma once

#include <string>
#include <vector>

template<typename OutputIt>
void push_token(std::string& token, OutputIt output) {
  if(!token.empty()) {
    *output = token;
    token.clear();
  }
}

template<typename InputIt>
std::vector<std::string> parseSimple(InputIt first, InputIt last) {
  std::vector<std::string> infix;
  std::string token;

  for(InputIt curr = first; curr != last; ++curr) {
    if(isspace(*curr)) {
      push_token(token, back_inserter(infix));
    } else if(*curr == '+' || *curr == '-' || *curr == '*' || *curr == '/' || *curr == '(' || *curr == ')') {
      push_token(token, back_inserter(infix));
      token += *curr;
      push_token(token, back_inserter(infix));
    } else {
      token += *curr;
    }
  }
  push_token(token, back_inserter(infix));

  return infix;
}

std::vector<std::string> toPostfixSimple(const std::vector<std::string>& infix);

int simpleCalc(const std::vector<std::string>& postfix);
