#pragma once

#include <d2d1.h>
#include <gfx/vector2d.h>
#include "basetypes.h"
#include "gamefont.h"
#include "idrawable.h"
#include "ikeyboardeventreceiver.h"
#include "imouseeventreceiver.h"

class UIComponent : 
  public IDrawable, public IKeyboardEventReceiver, public IMouseEventReceiver {
public :
  UIComponent(
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

  UIComponent(
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

  virtual ~UIComponent();

  void Enable() { enabled_ = true; }

  void Disable() { enabled_ = false; }

  bool IsEnabled() const { return enabled_; }

  void SetVisible() { visible_ = true; }

  void SetHidden() { visible_ = false; }

  bool IsVisible() const { return visible_; }

  void SetPressed() { pressed_ = true; }

  void SetDepressed() { pressed_ = false; }

  bool IsPressed() const { return pressed_; }

  void Focus() { hasfocus_ = true; }

  void UnFocus() { hasfocus_ = false; }

  bool HasFocus() const { return hasfocus_; }

  void SetMouseHoverOn() { mouseover_ = true; }

  void SetMouseHoverOff() { mouseover_ = false; }

  bool IsMouseHovered() const { return mouseover_; }

  UIComponent* GetParent() const { return parent_; }

  void SetParent(UIComponent* parent) { parent_ = parent; }

  void SetPosition(const gfx::vector2D& origin) { 
    UpdateControlRect(origin.x_, origin.y_);
  }

  void SetPosition(float x, float y) { 
    UpdateControlRect(x, y);
  }

  gfx::vector2D GetPosition() const { 
    return gfx::vector2D(btnrect_.left, btnrect_.top); 
  }

  D2D1_RECT_F GetRectangle() const { return btnrect_; }

  float GetWidth() const { return btnrect_.left - btnrect_.right; }

  float GetHeight() const { return btnrect_.top - btnrect_.bottom; }

  void SetWidth(float width) { 
    btnrect_.right = btnrect_.left + width;
  }

  void SetHeight(float height) { 
    btnrect_.bottom = btnrect_.top + height;
  }

  int GetID() const { return controlid_; }

  void SetFrameWidth(float width) { framewidth_ = width; }

  float GetFrameWidth() const { return framewidth_; }

  void SetFrameColor(const D2D1::ColorF& color) { color_frame_ = color; }

  void SetBackgroundColor(const D2D1::ColorF& color) { color_normal_ = color; }

  void SetHoveredColor(const D2D1::ColorF& color) { color_hovered_ = color; }

  void SetPressedColor(const D2D1::ColorF& color) { color_pressed_ = color; }

  void SetDisabledColor(const D2D1::ColorF& color) { color_disabled_ = color; }

  void SetFrameSkin(const wchar_t* skname) { skin_frame_ = skname; }

  void SetNormalSkin(const wchar_t* skname) { skin_normal_ = skname; }

  void SetHoveredSkin(const wchar_t* skname) { skin_hovered_ = skname; }

  void SetPressedSkin(const wchar_t* skname) { skin_pressed_ = skname; }

  void SetDisabledSkin(const wchar_t* skname) { skin_disabled_ = skname; }

  std::wstring GetText() const { return text_; }

  void SetText(const std::wstring& text) { text_ = text; }

  void SetFont(const GameFontData& fdata) { font_ = fdata; }

protected :
  enum FontDrawColor {
    FDC_Normal,
    FDC_Hover,
    FDC_Pressed,
    FDC_Disabled
  };

  void UpdateControlRect(float x, float y) {
    float width = GetWidth();
    float height = GetHeight();
    btnrect_.left = x; btnrect_.top = y;
    btnrect_.right = btnrect_.left + width;
    btnrect_.bottom = btnrect_.top + height;
  }

  void set_default_font_colors() {
    fontdrawcolor_[FDC_Normal] = D2D1::ColorF(D2D1::ColorF::Black);
    fontdrawcolor_[FDC_Hover] = D2D1::ColorF(D2D1::ColorF::White);
    fontdrawcolor_[FDC_Pressed] = D2D1::ColorF(D2D1::ColorF::Black);
    fontdrawcolor_[FDC_Disabled] = D2D1::ColorF(D2D1::ColorF::DarkGray);
  }

  const int       controlid_;
  bool            enabled_;
  bool            hasfocus_;
  bool            visible_;
  bool            mouseover_;
  bool            pressed_;
  UIComponent*    parent_;
  D2D1_RECT_F     btnrect_;
  float           framewidth_;

  D2D1_COLOR_F    color_frame_;
  D2D1_COLOR_F    color_normal_;
  D2D1_COLOR_F    color_hovered_;
  D2D1_COLOR_F    color_pressed_;
  D2D1_COLOR_F    color_disabled_;

  const wchar_t*  skin_frame_;
  const wchar_t*  skin_normal_;
  const wchar_t*  skin_hovered_;
  const wchar_t*  skin_pressed_;
  const wchar_t*  skin_disabled_;

  std::wstring    text_;
  GameFontData    font_;  
  D2D1_COLOR_F	  fontdrawcolor_[4];
  
  NO_CPY_CONSTRUCTORS(UIComponent);
};