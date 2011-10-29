#pragma once

#include "keyboard_event.h"
#include "sa32_thunderbolt.h"

namespace game_core {

class game_session : public game_core::interface_key_event_receiver {
private :
  bool              paused_;
  bool              player_alive_;
  bool              ended_;
  sa32_thunderbolt  player_ship_;
public :
  game_session();

  ~game_session();

  bool initialize_new_game() {
    return player_ship_.initialize();
  }

  bool is_paused() const {
    return paused_;
  }

  bool is_player_alive() const {
    return player_alive_;
  }

  bool has_ended() const {
    return ended_;
  }

  bool key_down(int);

  bool key_up(int) {
    return true;
  }

  void tick();

  void draw();
};

} // namespace game_core