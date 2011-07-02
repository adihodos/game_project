#ifndef CGAMESCREEN_H__
#define CGAMESCREEN_H__

#include "basetypes.h"
#include "game_screen.h"
#include "scoped_pointer.h"

class Direct2DRenderer;

namespace gfx {

class vector2D;

} // ns gfx

namespace game_entity {

class SA32_Thunderbolt;

} // ns game_entity

namespace game_ui {

class Screen_Manager;

class Playing_GameScreen : public IGameScreen {
public :
  Playing_GameScreen(
    Screen_Manager* smgr, const gfx::vector2D& pos, float width, float height);

  bool initialize();

  bool KeyPressed(KeyboardEventArgs* kb_args);

  bool KeyReleased(KeyboardEventArgs* kb_args);

  bool LeftButtonDown(MouseEventArgs* ms_args);

  bool LeftButtonUp(MouseEventArgs* ms_args);

  bool RightButtonDown(MouseEventArgs* ms_args);

  bool RightButtonUp(MouseEventArgs* ms_args);

  bool MouseMoved(MouseEventArgs* ms_args);

  void Draw(Direct2DRenderer* renderer);

private :
  scoped_pointer<game_entity::SA32_Thunderbolt> playership_;
};

} // ns game_ui

#endif // !CGAMESCREEN_H__