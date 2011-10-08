#ifndef GAMESCREEN_H__
#define GAMESCREEN_H__

#include "drawable_object.h"
#include "keyboard_event.h"
#include "mouse_event.h"

namespace game_ui {

class IGameScreen : public game_logic::I_DrawableObject, 
                    public game_logic::IKeyboard_EventReceiver, 
                    public game_logic::IMouse_EventReceiver {
public :
  IGameScreen() {}

  virtual ~IGameScreen() {}

  virtual bool initialize() { return true; }

  virtual void update(float /* delta */) {}  
};

} // ns game_ui

#endif // GAMESCREEN_H__