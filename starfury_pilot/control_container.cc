#include "precompiled.h"
#include "control_container.h"
#include "screen_component.h"

game_ui::IScreenComponentContainer::IScreenComponentContainer()
  : children_() {}

game_ui::IScreenComponentContainer::~IScreenComponentContainer() {}

void game_ui::IScreenComponentContainer::add_child(Widget2* child) {
  children_.push_back(std::shared_ptr<Widget2>(child));
}

void game_ui::IScreenComponentContainer::remove_child(Widget2* child) {
  children_.remove_if([child](std::shared_ptr<Widget2>& child_ptr)
  { return child == child_ptr.get(); }
  );
}