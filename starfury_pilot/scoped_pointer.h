#ifndef SCOPED_POINTER_H__
#define SCOPED_POINTER_H__

#include <cassert>
#include <cstdlib>
#include "basetypes.h"

template<typename T>
struct default_pointer {
  static void dispose(T* ptr) {
    delete ptr;
  }
};

template<typename T>
struct pointer_array_new {
  static void dispose(T* ptr) {
    delete[] ptr;
  }

  static T& get_at(T* ptr, int idx) {
    return ptr[idx];
  }
};

template<typename T>
struct pointer_malloc {
  static void dispose(T* ptr) {
    ::free(ptr);
  }
};

template<typename T>
struct pointer_array_malloc {
  static void dispose(T* ptr) {
    ::free(ptr);
  }

  static T& get_at(T* ptr_, int idx) {
    return ptr[idx];
  }
};

template<typename Direct2DInterface>
struct D2DInterface {
  static void dispose(Direct2DInterface* iface) {
    if (iface)
      iface->Release();
  }
};

template<
  typename T, 
  template<typename> class pointer_traits = default_pointer>
class scoped_pointer {
public :
  scoped_pointer(T* ptr = nullptr)
    : ptr_(ptr) {}

  template<typename U>
  scoped_pointer(U* ptr)
    : ptr_(ptr) {}

  ~scoped_pointer() {
    pointer_traits::dispose(ptr_);
  }

  bool operator!() const {
    return ptr_ == nullptr;
  }

  T* get() const {
    return ptr_;
  }

  T** get_owned_ptr() {
    return &ptr_;
  }

  T* operator->() {
    return ptr_;
  }

  const T* operator->() const {
    return ptr_;
  }

  T& operator*() {
    assert(ptr_);
    return *ptr_;
  }

  const T& operator*() const {
    assert(ptr_);
    return *ptr_;
  }

  T* release() {
    T* tmp = ptr_;
    ptr_ = nullptr;
    return tmp;
  }

  T& operator[](int idx) {
    assert(ptr_);
    return pointer_traits::get_at(ptr_, idx);
  }

  const T& operator[](int idx) const {
    assert(ptr_);
    return pointer_traits::get_at(ptr_, idx);
  }

  void reset(T* other = nullptr) {
    if (ptr_ != other) {
      pointer_traits::dispose(ptr_);
      ptr_ = other;
    }
  }

private :
  typedef pointer_traits<T> pointer_traits;
  T* ptr_;
  NO_CPY_CONSTRUCTORS(scoped_pointer);
};

#endif // SCOPED_POINTER_H__