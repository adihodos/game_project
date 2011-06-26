#ifndef FIXED_STACK_H__
#define FIXED_STACK_H__

#include <cassert>

namespace base {

template<typename T, int maxsize>
class fixed_stack {
public :
  fixed_stack()
    : index_(0) {}

  void push(const T& element) {
    assert(index_ < maxsize);
    elements_[index_++] = element;
  }

  void pop() {
    assert(index_ > 0);
    --index_;
  }

  T& top() {
    assert(index_ > 0);
    return elements_[index_ - 1];
  }

  const T& top() const {
    assert(index_ > 0);
    return elements_[index_ - 1];
  }

  bool empty() const {
    return index_ == 0;
  }

  int size() const {
    return index_;
  }

  int capacity() const {
    return maxsize;
  }

  void clear() {
    index_ = 0;
  }

private :
  T     elements_[maxsize];
  int   index_;
};

} // ns base

#endif // FIXED_STACK_H__