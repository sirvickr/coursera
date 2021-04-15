#pragma once

#include "date.h"

#include <string>
#include <memory>

using namespace std;

class Node {
public:
  virtual ~Node();
  virtual int Evaluate(const Date& date, const string& event) = 0;
};

struct EmptyNode : public Node {
  int Evaluate(const Date& date, const string& event) override {
    return 1;
  }
};

enum class Comparison {
  Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual
};

class ComparisonNode : public Node {
public:
  ComparisonNode(Comparison cmp) : cmp(cmp) {
  }

protected:
  Comparison cmp;
};

class DateComparisonNode : public ComparisonNode {
public:
  DateComparisonNode(Comparison cmp, const Date& date_value): ComparisonNode(cmp), date_value(date_value) {
  }

  int Evaluate(const Date& date, const string& event) override {
    switch(cmp) {
    case Comparison::Less:
      return date < date_value;
    case Comparison::LessOrEqual:
      return date <= date_value;
    case Comparison::Greater:
      return date > date_value;
    case Comparison::GreaterOrEqual:
      return date >= date_value;
    case Comparison::Equal:
      return date == date_value;
    case Comparison::NotEqual:
      return date != date_value;
    }
    return 0;
  }

private:
  const Date date_value;
};

class EventComparisonNode : public ComparisonNode {
public:
  EventComparisonNode(Comparison cmp, const string& event_value) : ComparisonNode(cmp), event_value(event_value) {
  }

  int Evaluate(const Date& date, const string& event) override {
    switch(cmp) {
    case Comparison::Less:
      return event < event_value;
    case Comparison::LessOrEqual:
      return event <= event_value;
    case Comparison::Greater:
      return event > event_value;
    case Comparison::GreaterOrEqual:
      return event >= event_value;
    case Comparison::Equal:
      return event == event_value;
    case Comparison::NotEqual:
      return event != event_value;
    }
    return 0;
  }

private:
  const string event_value;
};

enum class LogicalOperation { Or, And };

class LogicalOperationNode : public Node {
public:
  LogicalOperationNode(LogicalOperation op, shared_ptr<Node> left, shared_ptr<Node> right) : op(op), left(left), right(right) {
  }

  int Evaluate(const Date& date, const string& event) override {
    switch(op) {
    case LogicalOperation::Or:
      return left->Evaluate(date, event) || right->Evaluate(date, event);
    case LogicalOperation::And:
      return left->Evaluate(date, event) && right->Evaluate(date, event);
    }
    return 0;
  }

private:
  LogicalOperation op;
  shared_ptr<Node> left, right;
};
