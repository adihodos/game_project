#pragma once

#include "basetypes.h"
#include <list>

namespace game_core {

struct sys_event {
  enum sys_event_type {
    se_type_none,
    se_type_quit,
    se_type_key,
    se_type_mouse
  };

  explicit sys_event(sys_event_type type, int arg1 = 0, 
                     int arg2 = 0, void* ptr = nullptr)
    : se_type_(type), se_arg1_(arg1), se_arg2_(arg2), se_ptr_(ptr) {}

  int     se_type_;
  int     se_arg1_;
  int     se_arg2_;
  void*   se_ptr_;
};

class sys_event_queue {
public :
  sys_event_queue() {}

  bool empty() const {
    return queue_.empty();
  }

  sys_event get_next_event();

  void post_event(sys_event::sys_event_type type, int arg1 = 0, 
                  int arg2 = 0, void* ptr = nullptr)
  {
    sys_event ev(type, arg1, arg2, ptr);
    queue_.push_back(ev);
  }

  void post_quit_event() {
    sys_event qev(sys_event::se_type_quit);
    queue_.push_front(qev);
  }
private :
  typedef std::list<sys_event>  event_queue_t;
  event_queue_t queue_;

  NO_CPY_CONSTRUCTORS(sys_event_queue);
};

} // namespace game_core