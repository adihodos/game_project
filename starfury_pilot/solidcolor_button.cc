#include "precompiled.h"
#include "d2drenderer.h"
#include "game_resourcecache.h"
#include "helpers.h"
#include "ikeyboardeventreceiver.h"
#include "imouseeventreceiver.h"
#include "lazy_unique_instance.h"
#include "solidcolor_button.h"

game_ui::SolidColorButton::SolidColorButton(
  int identifier,
  const gfx::vector2D& pos, float width, float height, 
  Widget2* parent /* = nullptr */, 
  const wchar_t* text /* = nullptr */, float frame_width /* = 0.0f */, 
  int color_normal /* = D2D1::ColorF::White */, 
  int color_hover /* = D2D1::ColorF::Yellow */, 
  int color_down /* = D2D1::ColorF::Blue */, 
  int color_disabled /* = D2D1::ColorF::DarkGray */, 
  int color_frame /* = D2D1::ColorF::Black */, 
  int color_text /* = D2D1::ColorF::Black */ 
  )
  : game_ui::Widget2(identifier, pos, width, height, parent), 
    text_(text ? text : L""), 
    frame_width_(frame_width), font_(L"Arial") {
    colors_[SCB_Normal] = D2D1::ColorF(color_normal);
    colors_[SCB_Hover] = D2D1::ColorF(color_hover);
    colors_[SCB_Down] = D2D1::ColorF(color_down);
    colors_[SCB_Disabled] = D2D1::ColorF(color_disabled);
    colors_[SCB_Frame] = D2D1::ColorF(color_frame);
    colors_[SCB_Text] = D2D1::ColorF(color_text);
}

bool game_ui::SolidColorButton::KeyPressed(KeyboardEventArgs* k_args) {
  if (!is_enabled() || !is_focused())
    return false;

  if (k_args->w_param == VK_SPACE) {
    set_pressed();
    notify_parent();
    return true;
  }

  return false;
}

bool game_ui::SolidColorButton::KeyReleased(KeyboardEventArgs* k_args) {
  if (!is_enabled() || !is_focused())
    return false;

  if (k_args->w_param == VK_SPACE) {
    assert(is_pressed());
    set_pressed(false);
    return true;
  }

  return false;
}

bool game_ui::SolidColorButton::LeftButtonDown(MouseEventArgs* ms_args) {
  if (!is_enabled())
    return false;

  D2D1_RECT_F button_hitzone(D2D1::RectF(position_.x_, position_.y_, position_.x_ + width_, position_.y_ + height_));
  if (!utility::point_in_rect(ms_args->me_posx, ms_args->me_posy, button_hitzone)) {
    return false;
  }

  set_pressed();
  notify_parent();
  return true;
}

bool game_ui::SolidColorButton::LeftButtonUp(MouseEventArgs* ms_args) {
  UNREFERENCED_PARAMETER(ms_args);

  if (!is_enabled())
    return false;

  if (is_pressed()) {
    set_pressed(false);
    return true;
  }

  return false;
}

bool game_ui::SolidColorButton::RightButtonDown(MouseEventArgs* args) {
  UNREFERENCED_PARAMETER(args);
  return false;
}

bool game_ui::SolidColorButton::RightButtonUp(MouseEventArgs* args) {
  UNREFERENCED_PARAMETER(args);
  return false;
}

bool game_ui::SolidColorButton::MouseMoved(MouseEventArgs* ms_args) {
  if (!is_enabled())
    return false;

  D2D1_RECT_F button_hitzone(D2D1::RectF(position_.x_, position_.y_, position_.x_ + width_, position_.y_ + height_));
  if (!utility::point_in_rect(ms_args->me_posx, ms_args->me_posy, button_hitzone)) {
    if (is_hovered()) {
      set_hovered(false);
    }
  } else {
    set_hovered();
  }
  return false;
}

void game_ui::SolidColorButton::Draw(Direct2DRenderer* renderer) {
  GameResourceCache* res_cache(base::LazyUniqueInstance<GameResourceCache>::Get());
  ID2D1HwndRenderTarget* render_target(static_cast<ID2D1HwndRenderTarget*>(renderer->GetRendererTarget()));

  render_target->SetTransform(D2D1::Matrix3x2F::Identity());

  D2D1_RECT_F control_rect(D2D1::RectF(position_.x_, position_.y_, position_.x_ + width_, position_.y_ + height_));
  //
  // Draw the frame if any
  if (frame_width_ >= 1.0f) {
    ID2D1SolidColorBrush* brush_frame(res_cache->GetSolidColorBrushHandle(colors_[SCB_Frame]));
    assert(brush_frame);
    render_target->DrawRectangle(control_rect, brush_frame, frame_width_);
    utility::shrink_rect(frame_width_, &control_rect);
  }

  int color_index = is_enabled() ? (is_pressed() ? SCB_Down : is_hovered() ? SCB_Hover : SCB_Normal) : SCB_Disabled;
  
  ID2D1SolidColorBrush* brush_background(res_cache->GetSolidColorBrushHandle(colors_[color_index]));
  assert(brush_background);

  render_target->FillRectangle(control_rect, brush_background);

  //
  // Draw text (if any)
  if (text_.empty())
    return;

  //
  // shrink the font rectangle by 4 pixels
  utility::shrink_rect(4.0f, &control_rect);

  IDWriteTextFormat* text_format(res_cache->GetFontHandle(font_));
  assert(text_format);
  text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
  text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
  ID2D1SolidColorBrush* text_brush(res_cache->GetSolidColorBrushHandle(colors_[SCB_Text]));
  assert(text_brush);

  render_target->DrawTextW(
    text_.c_str(),
    text_.length(),
    text_format,
    control_rect,
    text_brush,
    D2D1_DRAW_TEXT_OPTIONS_CLIP
    );

  //
  // draw focus rect
  if (is_focused()) {
    D2D1_RECT_F focused_rect(control_rect);
    utility::shrink_rect(8.0f, &focused_rect);
    ID2D1SolidColorBrush* brush_focus(res_cache->GetSolidColorBrushHandle(D2D1::ColorF(D2D1::ColorF::Crimson)));
    render_target->DrawRectangle(focused_rect, brush_focus);
  }
}