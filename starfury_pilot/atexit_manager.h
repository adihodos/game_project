#pragma once

#include <algorithm>
#include <stack>
#include "basetypes.h"

namespace base {

struct callback_and_args {
  callback_and_args(void (*fn)(void*), void* arg)
    : fn_callback(fn), fn_args(arg) {}

  void (*fn_callback)(void*);
  void* fn_args;
};

class atexit_manager;

extern atexit_manager* g_atexit_mgr;

class atexit_manager {
public :
  atexit_manager() : registered_callbacks_() {
    if (!g_atexit_mgr)
      g_atexit_mgr = this;
  }

  ~atexit_manager() { execute_all_callbacks(); }

  void register_callback(const callback_and_args& cbdata) {
    registered_callbacks_.push(cbdata);
  }

  void execute_all_callbacks() {
    while (!registered_callbacks_.empty()) {
      callback_and_args itr = registered_callbacks_.top();
      itr.fn_callback(itr.fn_args);
      registered_callbacks_.pop();
    }
  }

private :
  std::stack<callback_and_args>  registered_callbacks_;
  NO_CPY_CONSTRUCTORS(atexit_manager);
};

}