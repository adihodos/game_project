#include "precompiled.h"
#include "engine.h"
#include "game_resourcecache.h"
#include "game_session.h"

game_core::game_session::game_session()
  : game_core::interface_key_event_receiver(),
    paused_(true),
    player_alive_(true),
    ended_(false) 
{
}

game_core::game_session::~game_session() {}

void
game_core::game_session::draw() {
  player_ship_.draw();
}

void
game_core::game_session::tick() {
  BREAK_INTO_DEBUGGER();
}

bool
game_core::game_session::key_down(int code) {
  switch (code) {
  case VK_RIGHT :
    //player_ship_.rotate(5.0f);
    player_ship_.move(1);
    return true;
    break;

  case VK_LEFT :
    //player_ship_.rotate(-5.0f);
    player_ship_.move(-1);
    return true;
    break;

  default :
    break;
  }

  return false;
}