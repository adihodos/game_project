#include "precompiled.h"
#include "engine.h"
#include "game_screen.h"
#include "play_screen.h"
#include "screen_mgr.h"

game_core::screen_manager::screen_manager() {}

game_core::screen_manager::~screen_manager() {
  std::for_each(
    screens_.begin(), screens_.end(), [](game_ui::IGameScreen* gsi) -> void {
      delete gsi;
  });
}

void 
game_core::screen_manager::pop_screen() {
  assert(!screens_.empty());
  delete screens_.top();
  screens_.pop();
  assert(!screens_.empty());

  GameEngine_Handle::Get()->set_trap_keyboardevents_dest(screens_.top());
  GameEngine_Handle::Get()->set_trap_mouseevents_dest(screens_.top());
}

void
game_core::screen_manager::push_screen(
  game_ui::IGameScreen* gsi
  )
{
  screens_.push(gsi);
  GameEngine_Handle::Get()->set_trap_keyboardevents_dest(gsi);
  GameEngine_Handle::Get()->set_trap_mouseevents_dest(gsi);
}

bool
game_core::screen_manager::initialize() {
  scoped_pointer<game_ui::IGameScreen> play_screen(new Play_Screen());
  if (!play_screen->initialize())
    return false;

  push_screen(play_screen.release());
  return true;
}