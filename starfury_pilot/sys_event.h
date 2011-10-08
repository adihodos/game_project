#ifndef SYS_EVENT_H__
#define SYS_EVENT_H__

#include <list>
#include <Windows.h>
#include "basetypes.h"
#include "keyboard_event.h"
#include "mouse_event.h"

namespace game_logic {

template<typename T, typename U>
struct bigger_type {
  static const size_t kSize = sizeof(T) > sizeof(U) ? sizeof(T) : sizeof(U);
};

class Sys_EventQueue {
public :
  Sys_EventQueue(IKeyboard_EventReceiver* trap_kbd = nullptr, 
                 IMouse_EventReceiver* trap_mouse = nullptr);

  void Get_NextEvent();

  bool Dispatch_Event();

  void Post_MouseEvent(const MouseEvent_Args& me_args);

  void Post_KeyEvent(const KeyEvent_Args& kb_args);

  void Post_QuitEvent();

  void TrapKeyEvents_Dest(IKeyboard_EventReceiver* dst) {
    trap_keybd_to_ = dst;
  }

  void TrapMouseEvents_Dest(IMouse_EventReceiver* dst) {
    trap_mouse_to_ = dst;
  }
  
private :
  typedef enum {
    kSysEventType_KeyEvent,
    kSysEventType_MouseEvent,
    kSysEventType_QuitEvent
  } SysEventType;

  struct Sys_Event {  

    template<typename T> T* get_event_data() {
      return reinterpret_cast<T*>(input_event_);
    }

    int type_;
    union {
      unsigned char input_event_[bigger_type<MouseEvent_Args, KeyEvent_Args>::kSize];
    };
    void* ptr_;
  };

  bool Pump_Messages();

  typedef std::list<Sys_Event>  Events_Queue;
  Events_Queue                  pending_events_;
  IKeyboard_EventReceiver*      trap_keybd_to_;
  IMouse_EventReceiver*         trap_mouse_to_;
  NO_CPY_CONSTRUCTORS(Sys_EventQueue);
};

} // namespace game_logic

#endif // !SYS_EVENT_H__