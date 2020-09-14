#pragma once

#include <cstdlib>
#include <algorithm>

template <typename T>
class SimpleVector {
public:
  SimpleVector() = default;
  explicit SimpleVector(size_t size);
  SimpleVector(const SimpleVector& other);
  SimpleVector& operator=(const SimpleVector& other);
  SimpleVector(SimpleVector&& other);
  SimpleVector& operator=(SimpleVector&& other);
  ~SimpleVector();

  T& operator[](size_t index);

  T* begin() {
    return begin_;
  }
  T* end() {
    return end_;
  }

  const T* begin() const {
    return begin_;
  }
  const T* end() const {
    return end_;
  }

  size_t Size() const {
    return size_;
  }
  size_t Capacity() const {
    return capacity_;
  }
  void PushBack(T value);

private:
  void Allocate(size_t size, size_t capacity);

private:
  size_t size_ = 0;
  size_t capacity_ = 0;
  T* begin_ = nullptr;
  T* end_ = nullptr;
};

template <typename T>
SimpleVector<T>::SimpleVector(size_t size) {
  Allocate(size, size);
}

template <typename T>
SimpleVector<T>::SimpleVector(const SimpleVector& other) {
  Allocate(other.size_, other.capacity_);
  std::copy(begin(other), end(other), begin_);
}

template <typename T>
SimpleVector<T>& SimpleVector<T>::operator=(const SimpleVector& other) {
  Allocate(other.size_, other.capacity_);
  std::copy(other.begin_, other.end_, begin_);
  return *this;
}

template <typename T>
SimpleVector<T>::SimpleVector(SimpleVector&& other)
: size_(other.size_),
  capacity_(other.capacity_),
  begin_(other.begin_),
  end_(other.end_)
{
  other.size_ = other.capacity_ = 0;
  other.begin_ = other.end_ = nullptr;
}

template <typename T>
SimpleVector<T>& SimpleVector<T>::operator=(SimpleVector&& other) {
  delete[] begin_;
  size_ = other.size_;
  capacity_ = other.capacity_;
  begin_ = other.begin_;
  end_ = other.end_;
  other.size_ = other.capacity_ = 0;
  other.begin_ = other.end_ = nullptr;
}

template <typename T>
SimpleVector<T>::~SimpleVector() {
  delete[] begin_;
}

template <typename T>
T& SimpleVector<T>::operator[](size_t index) {
  return begin_[index];
}

template <typename T>
void SimpleVector<T>::PushBack(T value) {
  if(size_ == capacity_) {
    if(capacity_ > 0)
      capacity_ <<= 1;
    else
      capacity_ = 1;
    T* data = new T[capacity_];
    if(begin_) {
      std::move(begin_, end_, data);
      delete[] begin_;
    }
    begin_ = data;
  }
  begin_[size_++] = std::move(value);
  end_ = begin_ + size_;
}

template <typename T>
void SimpleVector<T>::Allocate(size_t size, size_t capacity) {
  delete [] begin_;
  begin_ = end_ = nullptr;
  size_= size;
  capacity_= capacity;
  if(capacity_) {
    begin_ = new T[capacity_];
    end_ = begin_ + size_;
  }
}
