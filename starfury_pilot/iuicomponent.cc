#include "precompiled.h"
#include "gamefont.h"
#include "iuicomponent.h"

UIComponent::UIComponent( 
  int controlid, float originx, float originy, float width, float height, 
  const D2D1::ColorF& color_frame, 
  const D2D1::ColorF& color_normal, 
  const D2D1::ColorF& color_hover, 
  const D2D1::ColorF& color_pressed, 
  const D2D1::ColorF& color_disabled,
  const GameFontData& font,
  UIComponent* parent,
  float framewidth,
  const wchar_t* text
  )
  : controlid_(controlid), enabled_(true), hasfocus_(false), visible_(true),
    mouseover_(false), pressed_(false), parent_(parent),
    btnrect_(D2D1::RectF(originx, originx, originx + width, originy + height)),
    framewidth_(framewidth), color_frame_(color_frame), 
    color_normal_(color_normal), color_hovered_(color_hover),
    color_pressed_(color_pressed), color_disabled_(color_disabled),
    skin_frame_(), skin_normal_(), skin_hovered_(), skin_pressed_(), skin_disabled_(),
    text_(text ? text : L""), font_(font)
{
  set_default_font_colors();
}

UIComponent::UIComponent( 
  int controlid, float originx, float originy, float width, float height, 
  const wchar_t* sk_frame, const wchar_t* sk_normal, const wchar_t* sk_hover, 
  const wchar_t* sk_pressed, const wchar_t* sk_disabled, 
  const GameFontData& font,
  UIComponent* parent, /* = nullptr */
  float framewidth, /* = 0.0f */
  const wchar_t* text
  )
  : controlid_(controlid), enabled_(true), hasfocus_(false), visible_(true),
    mouseover_(false), pressed_(false), parent_(parent),
    btnrect_(D2D1::RectF(originx, originx, originx + width, originy + height)),
    framewidth_(framewidth), color_frame_(), color_normal_(), color_hovered_(),
    color_pressed_(), color_disabled_(), skin_frame_(sk_frame), 
    skin_normal_(sk_normal), skin_hovered_(sk_hover), skin_pressed_(sk_pressed),
    skin_disabled_(sk_disabled), text_(text ? text : L""), font_(font)
{
  set_default_font_colors();
}

UIComponent::~UIComponent() {}