#ifndef GAMESCREEN_H__
#define GAMESCREEN_H__

#include "drawable_object.h"
#include "keyboard_event.h"
#include "mouse_event.h"

namespace game_ui {

class IGameScreen : public game_core::I_DrawableObject, 
                    public game_core::IKeyboard_EventReceiver, 
                    public game_core::IMouse_EventReceiver {
public :
  IGameScreen() {}

  virtual ~IGameScreen() {}

  virtual bool initialize() { return true; }

  virtual void update(float /* delta */) {}  
};

} // ns game_ui

#endif // GAMESCREEN_H__