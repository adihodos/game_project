#include "precompiled.h"
#include "game_screen.h"
#include "screen_component.h"
#include "screen_manager.h"

game_ui::IGameScreen::IGameScreen( 
  Screen_Manager* screen_manager, 
  const gfx::vector2D& position, 
  float width, float height
  ) : IDrawable(), IKeyboardEventReceiver(), IMouseEventReceiver(), 
      IScreenComponentContainer(), position_(position), width_(width), 
      height_(height), screen_manager_(screen_manager), focused_object_()
{}

game_ui::IGameScreen::~IGameScreen() {}

void game_ui::IGameScreen::set_active() {
  screen_manager_->set_screen_active(this);
}

void game_ui::IGameScreen::set_inactive() {
  screen_manager_->set_screen_inactive(this);
}

void game_ui::IGameScreen::handle_focus_change(Widget* new_focused_object) {
  if (new_focused_object) {
    if (new_focused_object != focused_object_) {
      if (focused_object_)
        focused_object_->lost_focus();

      focused_object_ = new_focused_object;
      focused_object_->received_focus();
    }
  } else {
    if (focused_object_) {
      focused_object_->lost_focus();
      focused_object_ = nullptr;
    }
  }
}

void game_ui::IGameScreen::focus_next_control() {
  if (!focused_object_)
    return;

  auto child_itr = std::find_if(
    get_children_begin_iterator(), get_children_end_interator(), 
    [this](IScreenComponentContainer::ChildListElementType control) {
      return focused_object_ == control.get();
  });

  assert(child_itr != get_children_end_interator());

  ++child_itr;
  if (child_itr == get_children_end_interator())
    child_itr = get_children_begin_iterator();

  //
  // Take frocus from previous focused control
  focused_object_->lost_focus();
  focused_object_ = child_itr->get();
  //
  // Give focus to the new control
  focused_object_->received_focus();
}