#include "precompiled.h"
#include "control_container.h"
#include "screen_component.h"

game_ui::IScreenComponentContainer::IScreenComponentContainer()
  : children_() {}

game_ui::IScreenComponentContainer::~IScreenComponentContainer() {}

void game_ui::IScreenComponentContainer::add_child(IScreenComponent* child) {
  children_.push_back(std::shared_ptr<IScreenComponent>(child));
}

void game_ui::IScreenComponentContainer::remove_child(IScreenComponent* child) {
  children_.remove_if([child](std::shared_ptr<IScreenComponent>& child_ptr)
  { return child == child_ptr.get(); }
  );
}