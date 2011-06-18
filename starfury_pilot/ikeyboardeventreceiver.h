#pragma once

#include <windows.h>

struct KeyboardEventArgs {
  KeyboardEventArgs(WPARAM w, LPARAM l)
    : w_param(w), l_param(l) {}

  WPARAM  w_param;
  LPARAM  l_param;
};

class IKeyboardEventReceiver {
public :
  virtual ~IKeyboardEventReceiver() {}

  virtual bool KeyPressed(KeyboardEventArgs*) = 0;

  virtual bool KeyReleased(KeyboardEventArgs*) = 0;
};