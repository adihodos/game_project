//#ifndef BUTTON_BASE_H__
//#define BUTTON_BASE_H__
//
//#include "screen_component.h"
//
//namespace game_ui {
//
//class IButtonBase : public IScreenComponent {
//public :
//  IButtonBase(
//    int identifier,
//    const gfx::vector2D& pos, float width, float height, 
//    IScreenComponent* parent = nullptr
//    );
//
//  virtual ~IButtonBase() = 0;
//
//  bool is_pressed() const { return pressed_; }
//
//  bool is_hovered() const { return hovered_; }
//
//  bool is_enabled() const { return enabled_; }
//
//  bool is_focused() const { return focus_; }
//
//  void set_enabled(bool enabled = true) { enabled_ = enabled; }
//
//  void set_hovered(bool hovered = true) { hovered_ = hovered; }
//
//  void set_pressed(bool pressed = true) { pressed_ = pressed; }
//
//  void set_focus(bool focus = true) { focus_ = focus; }
//
//protected :
//  void on_child_activated(IScreenComponent*) { return; }
//
//  bool  pressed_;
//  bool  hovered_;
//  bool  enabled_;
//  bool  focus_;
//};
//
//} // ns game_ui
//
//#endif // BUTTON_BASE_H__