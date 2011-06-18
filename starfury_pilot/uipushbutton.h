#pragma once

#include <string>
#include <d2d1.h>
#include "basetypes.h"
#include "gamefont.h"
#include "iuicomponent.h"

class Direct2DRenderer;

class UIPushButton : public UIComponent {
public :
  UIPushButton(
    int controlid, float originx, float originy, float width, float height,  
    const D2D1::ColorF& color_frame,
    const D2D1::ColorF& color_normal,
    const D2D1::ColorF& color_hover,
    const D2D1::ColorF& color_pressed,
    const D2D1::ColorF& color_disabled,
    const GameFontData& font,
    UIComponent* parent = nullptr,
    float framewidth = 0.0f,
    const wchar_t* text = nullptr
    );

  UIPushButton(
    int controlid, float originx, float originy, float width, float height, 
    const wchar_t* sk_frame,
    const wchar_t* sk_normal,
    const wchar_t* sk_hover,
    const wchar_t* sk_pressed,
    const wchar_t* sk_disabled,
    const GameFontData& font,
    UIComponent* parent = nullptr,
    float framewidth = 0.0f,
    const wchar_t* text = nullptr
    );

  bool KeyPressed(KeyboardEventArgs*);

  bool KeyReleased(KeyboardEventArgs*);

  bool LeftButtonDown(MouseEventArgs* args);

  bool LeftButtonUp(MouseEventArgs* args);

  bool RightButtonDown(MouseEventArgs* args);

  bool RightButtonUp(MouseEventArgs*);

  bool MouseMoved(MouseEventArgs* args);

  void Draw(Direct2DRenderer* renderer);

private :
  void draw_control_text(Direct2DRenderer* renderer);
};