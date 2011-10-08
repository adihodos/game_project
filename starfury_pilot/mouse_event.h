#ifndef MOUSE_EVENT_H__
#define MOUSE_EVENT_H__

namespace game_logic {

class MouseEvent_Args {
public :
  enum {
    kMouseEventNone,
    kMouseEvent_LButtonDown,
    kMouseEvent_LButtonUp,
    kMouseEvent_RButtonDown,
    kMouseEvent_RButtonUp,
    kMouseEvent_Move
  };

  enum {
    kStateF_ControlDown = 1U << 0,
    kStateF_LeftBDown = 1U << 1,
    kStateF_RightBDown = 1U << 2,
    kStateF_ShiftDown = 1U << 3
  };

  static MouseEvent_Args From_SysInputEvent(
    UINT msg, WPARAM w_param, LPARAM l_param
    ) {
    int event_type;

    switch(msg) {
    case WM_LBUTTONDOWN :
      event_type = kMouseEvent_LButtonDown;
      break;

    case WM_LBUTTONUP :
      event_type = kMouseEvent_LButtonUp;
      break;

    case WM_RBUTTONDOWN :
      event_type = kMouseEvent_RButtonDown;
      break;

    case WM_RBUTTONUP :
      event_type = kMouseEvent_RButtonUp;
      break;

    case WM_MOUSEMOVE :
      event_type = kMouseEvent_Move;
      break;

    default :
      assert(false && "Unhandled event");
      event_type = kMouseEventNone;
      break;
    }

    unsigned statef = 0;
    if (w_param & MK_CONTROL)
      statef |= kStateF_ControlDown;

    if (w_param & MK_LBUTTON)
      statef |= kStateF_LeftBDown;

    if (w_param & MK_RBUTTON)
      statef |= kStateF_RightBDown;

    if (w_param & MK_SHIFT)
      statef |= kStateF_ShiftDown;

    return MouseEvent_Args(event_type, statef, GET_X_LPARAM(l_param), 
                           GET_Y_LPARAM(l_param));
  }

  MouseEvent_Args(int type, unsigned flags, int pos_x, int pos_y)
    : event_type_(type), state_flags_(flags), px_(pos_x), py_(pos_y) {}

  int Get_EventType() const { return event_type_; }

  bool StateFlags_ControlDown() const { 
    return (state_flags_ & kStateF_ControlDown) == 1U; 
  }

  bool StateFlags_LeftDown() const {
    return (state_flags_ & kStateF_LeftBDown) == 1U;
  }

  bool StateFlags_RightDown() const {
    return (state_flags_ & kStateF_RightBDown) == 1U;
  }

  bool StateFlags_ShiftDown() const {
    return (state_flags_ & kStateF_ShiftDown) == 1U;
  }

  int Get_XCoord() const {
    return px_;
  }

  int Get_YCoord() const {
    return py_;
  }
private :
  int             event_type_;
  unsigned int    state_flags_;
  int             px_;
  int             py_;
};

class IMouse_EventReceiver {
public :
  virtual ~IMouse_EventReceiver() {}

  virtual bool Mouse_Event(MouseEvent_Args* ms_args) = 0;
};

} // namespace game_logic

#endif // !MOUSE_EVENT_H__