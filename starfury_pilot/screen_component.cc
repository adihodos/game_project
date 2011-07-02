#include "precompiled.h"
#include "screen_component.h"

game_ui::Widget::Widget( 
  int identifier,
  const gfx::vector2D& position, float width, float height, 
  Widget* parent /* = nullptr */ 
  ):  IDrawable(), IKeyboardEventReceiver(), IMouseEventReceiver(),
      identifier_(identifier), position_(position), width_(width), 
      height_(height), parent_(parent), 
      pressed_(false), hovered_(false), enabled_(true), focus_(false)
{
}

game_ui::Widget::~Widget() {}