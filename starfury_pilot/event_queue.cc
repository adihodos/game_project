#include "precompiled.h"
#include "event_queue.h"

game_core::sys_event 
game_core::sys_event_queue::get_next_event() {
  if (!queue_.empty()) {
    sys_event ev(queue_.front());
    queue_.pop_front();
    return ev;
  }

  MSG sysmsg;
  while (::PeekMessageW(&sysmsg, nullptr, 0, 0, PM_NOREMOVE)) {
    int ret_code = ::GetMessageW(&sysmsg, nullptr, 0, 0);
    if (ret_code <= 0) {
      return sys_event(sys_event::se_type_quit);
    }

    ::TranslateMessage(&sysmsg);
    ::DispatchMessageW(&sysmsg);
  }

  if (!queue_.empty()) {
    sys_event ev(queue_.front());
    queue_.pop_front();
    return ev;
  }

  return sys_event(sys_event::se_type_none);
}