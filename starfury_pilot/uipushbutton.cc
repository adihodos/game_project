#include "precompiled.h"
#include "d2drenderer.h"
#include "gamefont.h"
#include "game_resourcecache.h"
#include "helpers.h"
#include "lazy_unique_instance.h"
#include "uipushbutton.h"

UIPushButton::UIPushButton( 
  int controlid, float originx, float originy, float width, float height, 
  const wchar_t* sk_frame, const wchar_t* sk_normal, const wchar_t* sk_hover, 
  const wchar_t* sk_pressed, const wchar_t* sk_disabled, 
  const GameFontData& font, 
  UIComponent* parent, /* = nullptr */
  float framewidth, /* = 0.0f */ 
  const wchar_t* text /* = nullptr */
  )
  : UIComponent(controlid, originx, originy, width, height, sk_frame, sk_normal, 
                sk_hover, sk_pressed, sk_disabled, font, parent, framewidth, 
                text)
{

}

UIPushButton::UIPushButton( 
  int controlid, float originx, float originy, float width, float height, 
  const D2D1::ColorF& color_frame, const D2D1::ColorF& color_normal, 
  const D2D1::ColorF& color_hover, const D2D1::ColorF& color_pressed, 
  const D2D1::ColorF& color_disabled, 
  const GameFontData& font, 
  UIComponent* parent, /* = nullptr */
  float framewidth, /* = 0.0f */ 
  const wchar_t* text /* = nullptr */
  )
  : UIComponent(controlid, originx, originy, width, height, color_frame, 
                color_normal, color_hover, color_pressed, color_disabled, font,
                parent, framewidth, text)
{

}

bool UIPushButton::KeyPressed(KeyboardEventArgs*) {
  return true;
}

bool UIPushButton::KeyReleased(KeyboardEventArgs*) {
  return true;
}

bool UIPushButton::LeftButtonDown(MouseEventArgs* args) {
  if (!args->RectangleHitTest(btnrect_))
    return false;

  SetPressed();
  return true;
}

bool UIPushButton::LeftButtonUp(MouseEventArgs* args) {
  if (IsPressed()) {
    SetDepressed();
    return true;
  }

  return false;
}

bool UIPushButton::RightButtonDown(MouseEventArgs* args) {
  return false;
}

bool UIPushButton::RightButtonUp(MouseEventArgs*) {
  return false;
}

bool UIPushButton::MouseMoved(MouseEventArgs* args) {
  if (args->RectangleHitTest(btnrect_)) {
    SetMouseHoverOn();
    return true;
  } else {
    if (IsMouseHovered()) {
      SetMouseHoverOff();
      return true;
    }
  }
  return false;
}

void UIPushButton::Draw(Direct2DRenderer* renderer) {
  if (!IsVisible())
    return;

  ID2D1HwndRenderTarget* r = static_cast<ID2D1HwndRenderTarget*>(
    renderer->GetRendererTarget());

  GameResourceCache* rsrccache(base::LazyUniqueInstance<GameResourceCache>::Get());
  D2D1_RECT_F rect(btnrect_);

  if (framewidth_ > 0.0f) {
    if (skin_frame_) {
      r->DrawBitmap(rsrccache->GetBitmapHandle(skin_frame_), btnrect_);
    } else {
      r->DrawRectangle(
        btnrect_, rsrccache->GetSolidColorBrushHandle(color_frame_), framewidth_);
    }
    utility::shrink_rect(framewidth_, &rect);
  }

  if (IsEnabled()) {
    if(IsPressed()) {
      if (skin_pressed_) {
        r->DrawBitmap(rsrccache->GetBitmapHandle(skin_pressed_), rect);
      } else {
        r->FillRectangle(rect, rsrccache->GetSolidColorBrushHandle(color_pressed_));
      }
    } else if (IsMouseHovered()) {
      if (skin_hovered_) {
        r->DrawBitmap(rsrccache->GetBitmapHandle(skin_hovered_), rect);
      } else {
        r->FillRectangle(rect, rsrccache->GetSolidColorBrushHandle(color_hovered_));
      }
    } else {
      if (skin_normal_) {
        r->DrawBitmap(rsrccache->GetBitmapHandle(skin_normal_), rect);
      } else {
        r->FillRectangle(rect, rsrccache->GetSolidColorBrushHandle(color_normal_));
      }
    }
  } else {
    if (skin_disabled_) {
      r->DrawBitmap(rsrccache->GetBitmapHandle(skin_disabled_), rect);
    } else {
      r->FillRectangle(rect, rsrccache->GetSolidColorBrushHandle(color_disabled_));
    }
  }

  draw_control_text(renderer);
}

void 
UIPushButton::draw_control_text(Direct2DRenderer* r) {
  if (text_.empty())
    return;

  int color_idx = IsEnabled() ? (IsPressed() ? FDC_Pressed : 
      (IsMouseHovered() ? FDC_Hover : FDC_Normal)) : FDC_Disabled;

  //
  // Shrink rectangle for text with 8 pixels (4x, 4y)
  const int text_offset = 4;
  D2D1_RECT_F text_rect(btnrect_);
  utility::shrink_rect(text_offset + framewidth_, &text_rect);

  IDWriteTextFormat* text_fmt = 
    base::LazyUniqueInstance<GameResourceCache>::Get()->GetFontHandle(font_);
  assert(text_fmt);

  ID2D1SolidColorBrush* text_brush =
    base::LazyUniqueInstance<GameResourceCache>::Get()->GetSolidColorBrushHandle(
      fontdrawcolor_[color_idx]);
  assert(text_brush);

  text_fmt->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
  text_fmt->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
  r->GetRendererTarget()->DrawText(
    text_.c_str(), text_.length(), text_fmt, text_rect, text_brush,
    D2D1_DRAW_TEXT_OPTIONS_CLIP);
}