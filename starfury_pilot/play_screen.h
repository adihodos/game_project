#ifndef PLAY_SCREEN_H__
#define PLAY_SCREEN_H__

#include "basetypes.h"
#include "game_screen.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "scoped_pointer.h"

namespace gfx {
class vector2D;
}

namespace game_logic {

class Play_Screen : public game_ui::IGameScreen {
public :
  Play_Screen();

  ~Play_Screen();

  bool initialize();

  void update(float delta);

  void draw();

  bool Mouse_Event(MouseEvent_Args* e_args);

  bool Key_Event(KeyEvent_Args* e_args);

private :
  struct Implementation_Details;
  scoped_pointer<Implementation_Details> ps_impl_;
  NO_CPY_CONSTRUCTORS(Play_Screen);
};

}

#endif // !PLAY_SCREEN_H__