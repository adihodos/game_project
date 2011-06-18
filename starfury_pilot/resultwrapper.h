#pragma once

#include "basetypes.h"

template<typename T>
class ResultWrapper {
public :
  ResultWrapper()
    : valid_(false), data() {}

  ResultWrapper(T data, bool valid = false)
    : valid_(valid), data_(data) {}

  ResultWrapper(const ResultWrapper& other)
    : valid_(other.valid_), data_(other.data_) {}

  ResultWrapper& operator=(const ResultWrapper& other) {
    if (this != &other) {
      valid_ = other.valid_;
      data_ = other.data_;
    }
  }

  bool operator!() const {
    return !valid_;
  }

  bool IsValid() const {
    return valid_;
  }

  void SetValid(bool vflag = true) {
    valid_ = vflag;
  }

  T& GetData() {
    return data_;
  }

  const T& GetData() const {
    return data_;
  }

  T* GetDataPtr() {
    return &data_;
  }

  const T* GetDataPtr() const {
    return &data_;
  }
private :
  bool  valid_;
  T     data_;
};