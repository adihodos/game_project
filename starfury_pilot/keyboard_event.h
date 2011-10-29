#ifndef KEYBOARD_EVENT_H__
#define KEYBOARD_EVENT_H__

namespace game_core {

class KeyEvent_Args {
public :
  enum {
    kStateFlags_KeyDown = 1U << 0,
    kStateFlags_WasDown = 1U << 1
  };

  static KeyEvent_Args From_SysInputEvent(WPARAM w_param, LPARAM l_param) {
    int repeat_cnt = l_param & 0xFFFF;
    unsigned int statef = kStateFlags_KeyDown;

    static const unsigned int kUpFlag = 1U << 31;
    if (l_param & kUpFlag)
      statef = !kStateFlags_KeyDown;

    static const unsigned int kPreviouslyDownFlag = 1U << 30;
    if (l_param & kPreviouslyDownFlag)
      statef |= kStateFlags_WasDown;

    return KeyEvent_Args(static_cast<int>(w_param), repeat_cnt, statef);
  }

  KeyEvent_Args(int vcode, int rpt, unsigned state_flags)
    : virtual_code_(vcode), repeat_count_(rpt), state_flags_(state_flags) {}

  int Get_KeyCode() const { return virtual_code_; }

  int Get_RepeatCount() const { return repeat_count_; }

  bool Key_Down() const { return (state_flags_ & kStateFlags_KeyDown) == 1U; }

  bool Key_PreviouslyDown() const { return (state_flags_ & kStateFlags_WasDown) == 1U; }

private :
  int           virtual_code_;
  int           repeat_count_;
  unsigned int  state_flags_;
};

class IKeyboard_EventReceiver {
public :
  virtual ~IKeyboard_EventReceiver() {}

  virtual bool Key_Event(KeyEvent_Args* key_args) = 0;
};

} // namespace game_logic

namespace game_core {

class interface_key_event_receiver {
public :
  virtual ~interface_key_event_receiver() {}

  virtual bool key_down(int code) = 0;

  virtual bool key_up(int code) = 0;
};

}

#endif // !KEYBOARD_EVENT_H__