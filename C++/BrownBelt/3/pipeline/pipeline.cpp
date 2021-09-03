#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
  string from;
  string to;
  string body;
};

istream& operator >> (istream& stream, Email& email) {
  string from, to, body;
  if(getline(stream, from) && getline(stream, to) && getline(stream, body)) {
    email.from = move(from);
    email.to = move(to);
    email.body = move(body);
  }
  return stream;
}

ostream& operator << (ostream& stream, Email& email) {
  stream << email.from << "\n" << email.to << "\n" << email.body << "\n";
  return stream;
}


class Worker {
public:
  virtual ~Worker() = default;
  virtual void Process(unique_ptr<Email> email) = 0;
  virtual void Run() {
    // только первому worker-у в пайплайне нужно это имплементировать
    throw logic_error("Unimplemented");
  }

protected:
  // реализации должны вызывать PassOn, чтобы передать объект дальше
  // по цепочке обработчиков
  void PassOn(unique_ptr<Email> email) const {
    if (next_) {
      next_->Process(move(email));
    }
  }

public:
  void SetNext(unique_ptr<Worker> next) {
    next_ = move(next);
  }

private:
  unique_ptr<Worker> next_;
};


class Reader : public Worker {
public:
  explicit Reader(istream& in)
    : in_(in)
  {}

  void Process(unique_ptr<Email> email) override {
    // NOP
  }

  void Run() override {
    for(;;) {
      auto email = make_unique<Email>();
      if(!(in_ >> *email)) {
        break;
      }
      PassOn(move(email));
    }
  }

private:
  istream& in_;
};


class Filter : public Worker {
public:
  using Function = function<bool(const Email&)>;

public:
  explicit Filter(Function func): func_(func) {
  }

  void Process(unique_ptr<Email> email) override {
    if (func_(*email)) {
      PassOn(move(email));
    }
  }

private:
  Function func_;
};


class Copier : public Worker {
public:
  explicit Copier(const string& to)
    : to_(to)
  {}

  void Process(unique_ptr<Email> email) override {
    auto copy = make_unique<Email>(*email);
    bool duplicate = to_ != email->to;
    PassOn(move(email));
    if(duplicate) {
      copy->to = to_;
      PassOn(move(copy));
    }
  }

private:
  const string to_;
};


class Sender : public Worker {
public:
  explicit Sender(ostream& out)
    : out_(out)
  {}

  void Process(unique_ptr<Email> email) override {
    out_ << *email;
    PassOn(move(email));
  }

private:
  ostream& out_;
};


// реализуйте класс
class PipelineBuilder {
public:
  // добавляет в качестве первого обработчика Reader
  explicit PipelineBuilder(istream& in) {
      workers_.push_back(make_unique<Reader>(in));
  }

  // добавляет новый обработчик Filter
  PipelineBuilder& FilterBy(Filter::Function filter) {
      workers_.push_back(make_unique<Filter>(filter));
      return *this;
  }

  // добавляет новый обработчик Copier
  PipelineBuilder& CopyTo(string recipient){
      workers_.push_back(make_unique<Copier>(recipient));
      return *this;
  }

  // добавляет новый обработчик Sender
  PipelineBuilder& Send(ostream& out){
      workers_.push_back(make_unique<Sender>(out));
      return *this;
  }

  // возвращает готовую цепочку обработчиков
  unique_ptr<Worker> Build() {
      for(size_t i = workers_.size() - 1; i > 0; --i) {
          workers_[i - 1]->SetNext(move(workers_[i]));
      }
      return move(workers_.front());
  }

private:
  vector<unique_ptr<Worker>> workers_;
};


void TestSanity() {
  string input = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "ralph@example.com\n"
    "erich@example.com\n"
    "I do not make mistakes of that kind\n"
  );
  istringstream inStream(input);
  ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();

  pipeline->Run();

  string expectedOutput = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "erich@example.com\n"
    "richard@example.com\n"
    "Are you sure you pressed the right button?\n"
  );

  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  return 0;
}
