#ifndef SCREEN_COMPONENT_H__
#define SCREEN_COMPONENT_H__

#include <gfx/vector2d.h>
#include "basetypes.h"
#include "idrawable.h"
#include "ikeyboardeventreceiver.h"
#include "imouseeventreceiver.h"

namespace game_ui {

class IScreenComponent : 
  public IDrawable, public IKeyboardEventReceiver, public IMouseEventReceiver {
public :
  IScreenComponent(
    int identifier,
    const gfx::vector2D& position, float width, float height, 
    IScreenComponent* parent = nullptr
    );

  virtual ~IScreenComponent();

  int get_id() const {
    return identifier_;
  }

  float get_width() const {
    return width_;
  }

  float get_height() const {
    return height_;
  }

  void set_width(float width) {
    width_ = width;
  }

  void set_height(float height) {
    height_ = height;
  }

  gfx::vector2D get_position() const {
    return position_;
  }

  void set_position(const gfx::vector2D& position) {
    position_ = position;
  }

  IScreenComponent* get_parent() const {
    return parent_;
  }

  void set_parent(IScreenComponent* parent) {
    parent_ = parent;
  }

  void notify_parent() {
    if (parent_)
      parent_->on_child_activated(this);
  }

  bool is_pressed() const { return pressed_; }

  bool is_hovered() const { return hovered_; }

  bool is_enabled() const { return enabled_; }

  bool is_focused() const { return focus_; }

  void set_enabled(bool enabled = true) { enabled_ = enabled; }

  void set_hovered(bool hovered = true) { hovered_ = hovered; }

  void set_pressed(bool pressed = true) { pressed_ = pressed; }

  void set_focus(bool focus = true) { focus_ = focus; }

protected :
  virtual void on_child_activated(IScreenComponent* sender) = 0;

  const int identifier_;
  gfx::vector2D position_;
  float width_;
  float height_;
  IScreenComponent* parent_;
  bool  pressed_;
  bool  hovered_;
  bool  enabled_;
  bool  focus_;

  NO_CPY_CONSTRUCTORS(IScreenComponent);
};

} // ns game_ui

#endif // SCREEN_COMPONENT_H__