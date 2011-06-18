#pragma once

#include <d2d1.h>
#include <Windows.h>

struct MouseEventArgs {
  enum {
    ME_ControlDown = 1U << 0,
    ME_ShiftDown = 1U << 1
  };

  MouseEventArgs(float x, float y, unsigned int flags)
    : me_posx(x), me_posy(y), me_flags(flags) {}

  static MouseEventArgs FromLParamWParam(WPARAM w_param, LPARAM l_param) {
    unsigned flags = 0U;
    if (w_param & MK_CONTROL)
      flags |= ME_ControlDown;

    if (w_param & MK_SHIFT)
      flags |= ME_ShiftDown;

    return MouseEventArgs(static_cast<float>(GET_X_LPARAM(l_param)), 
                          static_cast<float>(GET_Y_LPARAM(l_param)), flags);
  }

  bool ControlDown() const {
    return (me_flags & ME_ControlDown) != 0U;
  }

  bool ShiftDown() const {
    return (me_flags & ME_ShiftDown) != 0U;
  }

  D2D1_POINT_2F GetPositionAsD2D1Point2F() const {
    return D2D1::Point2F(me_posx, me_posy);
  }

  gfx::vector2D GetPositionAsVector2D() const {
    return gfx::vector2D(me_posx, me_posy);
  }

  bool RectangleHitTest(const D2D1_RECT_F& rect) const {
    return me_posx >= rect.left && me_posx <= rect.right &&
      me_posy >= rect.top && me_posy <= rect.bottom;
  }

  float         me_posx;
  float         me_posy;
  unsigned int  me_flags;
};

class IMouseEventReceiver  {
public :
  virtual ~IMouseEventReceiver() {}

  virtual bool LeftButtonDown(MouseEventArgs* args) = 0;

  virtual bool LeftButtonUp(MouseEventArgs*) = 0;

  virtual bool RightButtonDown(MouseEventArgs* args) = 0;

  virtual bool RightButtonUp(MouseEventArgs*) = 0;

  virtual bool MouseMoved(MouseEventArgs* args) = 0;
};