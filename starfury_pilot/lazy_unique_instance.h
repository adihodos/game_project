#pragma once

#include "atexit_manager.h"
#include "basetypes.h"

namespace base {

template<typename T>
class LazyUniqueInstanceLifeTraits {
public :
  static void constructor(void* location) {
    new(location) T();
  }

  static void destructor(void* obj) {
    (static_cast<T*>(obj))->~T();
  }
};

template<typename T>
class LazyUniqueInstance {
public :
  LazyUniqueInstance() {}

  static T* Get();

private :

  enum {
    K_State_NotConstructed,
    K_State_Constructed
  };

  static char   objectarea_[sizeof(T)];
  static int    object_state_;
  NO_CPY_CONSTRUCTORS(LazyUniqueInstance);
};

} // ns base

template<typename T>
char base::LazyUniqueInstance<T>::objectarea_[sizeof(T)];

template<typename T>
int base::LazyUniqueInstance<T>::object_state_;

template<typename T>
T* base::LazyUniqueInstance<T>::Get() {
  if (object_state_ == K_State_Constructed)
    return reinterpret_cast<T*>(objectarea_);

  LazyUniqueInstanceLifeTraits<T>::constructor(reinterpret_cast<void*>(objectarea_));
  g_atexit_mgr->register_callback(
    callback_and_args(&LazyUniqueInstanceLifeTraits<T>::destructor, 
    reinterpret_cast<void*>(objectarea_)));
  object_state_ = K_State_Constructed;

  return reinterpret_cast<T*>(objectarea_);
}