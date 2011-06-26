#ifndef SOLIDCOLORBUTTON_H__
#define SOLIDCOLORBUTTON_H__

#include <string>
#include <d2d1.h>
#include "gamefont.h"
#include "screen_component.h"

class Direct2DRenderer;

namespace gfx {

class vector2D;

}

namespace game_ui {

class SolidColorButton : public IScreenComponent {
public :
  SolidColorButton(
    int identifier,
    const gfx::vector2D& pos, float width, float height,
    IScreenComponent* parent = nullptr,
    const wchar_t* text = nullptr,
    float frame_width = 0.0f,
    int color_normal = D2D1::ColorF::White,
    int color_hover = D2D1::ColorF::Yellow,
    int color_down = D2D1::ColorF::Blue,
    int color_disabled = D2D1::ColorF::DarkGray,
    int color_frame = D2D1::ColorF::Black,
    int color_text = D2D1::ColorF::Black
    );

  void set_normal_color(int color) {
    colors_[SCB_Normal] = D2D1::ColorF(color);
  }

  void set_hover_color(int color) {
    colors_[SCB_Hover] = D2D1::ColorF(color);
  }

  void set_down_color(int color) {
    colors_[SCB_Down] = D2D1::ColorF(color);
  }

  void set_disabled_color(int color) {
    colors_[SCB_Disabled] = D2D1::ColorF(color);
  }

  void set_frame_color(int color) {
    colors_[SCB_Frame] = D2D1::ColorF(color);
  }

  void set_text_color(int color) {
    colors_[SCB_Text] = D2D1::ColorF(color);
  }

  void set_font(const GameFontData& font) { font_ = font; }

  void Draw(Direct2DRenderer* renderer);

  bool KeyPressed(KeyboardEventArgs* k_args);

  bool KeyReleased(KeyboardEventArgs* k_args);

  bool LeftButtonDown(MouseEventArgs* args);

  bool LeftButtonUp(MouseEventArgs* args);

  bool RightButtonDown(MouseEventArgs* args);

  bool RightButtonUp(MouseEventArgs* args);

  bool MouseMoved(MouseEventArgs* args);

protected :
  void on_child_activated(IScreenComponent*) { return; }

private :
  enum {
    SCB_Normal,
    SCB_Hover,
    SCB_Down,
    SCB_Disabled,
    SCB_Frame,
    SCB_Text
  };

  std::wstring  text_;
  float         frame_width_;
  D2D1_COLOR_F  colors_[6];
  GameFontData  font_;
};

} // ns game_ui

#endif // SOLIDCOLORBUTTON_H__