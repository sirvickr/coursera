#pragma once

#include <cstdlib>
#include <algorithm>

template <typename T>
class SimpleVector {
public:
  SimpleVector() = default;
  explicit SimpleVector(size_t size);
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
  void PushBack(const T& value);

private:
  void allocate(size_t size, size_t capacity);

private:
  size_t size_ = 0;
  size_t capacity_ = 0;
  T* begin_ = nullptr;
  T* end_ = nullptr;
};

template <typename T>
SimpleVector<T>::SimpleVector(size_t size) {
  allocate(size, size);
}

template <typename T>
SimpleVector<T>::~SimpleVector() {
  if(begin_) {
    delete[] begin_;
  }
}

template <typename T>
T& SimpleVector<T>::operator[](size_t index) {
  return begin_[index];
}

template <typename T>
void SimpleVector<T>::PushBack(const T& value) {
  if(size_ == capacity_) {
    if(capacity_ > 0)
      capacity_ <<= 1;
    else
      capacity_ = 1;
    T* data = new T[capacity_];
    if(begin_) {
      std::copy(begin_, end_, data);
      delete[] begin_;
    }
    begin_ = data;
  }
  begin_[size_++] = value;
  end_ = begin_ + size_;
}

template <typename T>
void SimpleVector<T>::allocate(size_t size, size_t capacity) {
  if(begin_) {
    delete [] begin_;
    begin_ = end_ = nullptr;
  }
  size_= size;
  capacity_ = capacity;
  if(capacity_) {
    begin_ = new T[capacity_];
    end_ = begin_ + size_;
  }
}
