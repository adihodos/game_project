#include "precompiled.h"
#include "helpers.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "scoped_pointer.h"
#include "sys_event.h"

game_logic::Sys_EventQueue::Sys_EventQueue(
  IKeyboard_EventReceiver* trap_kbd /* = nullptr */, 
  IMouse_EventReceiver* trap_mouse /* = nullptr */)
  : pending_events_(), trap_keybd_to_(trap_kbd), trap_mouse_to_(trap_mouse) {}

void
game_logic::Sys_EventQueue::Get_NextEvent() {
  //
  // Queue has pending events.
  if (!pending_events_.empty())
    return;

  //
  // Pump message loop until empty
  MSG msg_data;
  while (::PeekMessageW(&msg_data, nullptr, 0, 0, PM_NOREMOVE)) {
    int ret_code = ::GetMessageW(&msg_data, nullptr, 0, 0);

    if (ret_code == -1) {
      OUT_DBG_MSG(L"GetMessageW failed, error %d", ::GetLastError());
      Post_QuitEvent();
      break;
    }

    if (ret_code == 0) {
      Post_QuitEvent();
      break;
    }

    ::TranslateMessage(&msg_data);
    ::DispatchMessageW(&msg_data);
  }
}

bool
game_logic::Sys_EventQueue::Dispatch_Event() {
  if (pending_events_.empty())
    return true;

  Sys_Event this_event(pending_events_.front());
  pending_events_.pop_front();

  switch (this_event.type_) {
  case kSysEventType_QuitEvent :
    return false;
    break;

  case kSysEventType_KeyEvent : {
    if (trap_keybd_to_) {
      trap_keybd_to_->Key_Event(this_event.get_event_data<KeyEvent_Args>());
      return true;
    }
                                }
    break;

  case kSysEventType_MouseEvent : {
    if (trap_mouse_to_) {
      trap_mouse_to_->Mouse_Event(this_event.get_event_data<MouseEvent_Args>());
      return true;
    }
                                  }
    break;

  default :
    break;
  }

  OUT_DBG_MSG(L"Untrapped event, type %d", this_event.type_);
  return true;
}

void
game_logic::Sys_EventQueue::Post_MouseEvent(
  const MouseEvent_Args& me_args
  )
{
  Sys_Event se;
  se.type_ = kSysEventType_MouseEvent;
  se.ptr_ = nullptr;
  memcpy_s(se.input_event_, _countof(se.input_event_), 
           &me_args, _countof(se.input_event_));
  pending_events_.push_back(se);
}

void
game_logic::Sys_EventQueue::Post_KeyEvent(
  const KeyEvent_Args& kb_args
  )
{
  Sys_Event se;
  se.type_ = kSysEventType_KeyEvent;
  se.ptr_ = nullptr;
  memcpy_s(se.input_event_, _countof(se.input_event_),
           &kb_args, _countof(se.input_event_));
  pending_events_.push_back(se);
}

void
game_logic::Sys_EventQueue::Post_QuitEvent() {
  Sys_Event se_quit;
  se_quit.type_ = kSysEventType_QuitEvent;
  pending_events_.push_front(se_quit);
}