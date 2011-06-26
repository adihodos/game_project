#ifndef GAMESETTINGS_HOLDER_H__
#define GAMESETTINGS_HOLDER_H__

#include "basetypes.h"
#include "lazy_unique_instance.h"

namespace game_logic {

class GameSettingsProvider : 
  public base::LazyUniqueInstanceLifeTraits<GameSettingsProvider> {

public :
  GameSettingsProvider();

  ~GameSettingsProvider();

  bool initialize();

  float get_screen_width() const {
    return static_cast<float>(screen_width_);
  }

  float get_screen_height() const {
    return static_cast<float>(screen_height_);
  }

  float get_screen_depth() const {
    return static_cast<float>(screen_depth_);
  }
private :
  friend class base::LazyUniqueInstanceLifeTraits<GameSettingsProvider>;

  float screen_width_;
  float screen_height_;
  int   screen_depth_;

  NO_CPY_CONSTRUCTORS(GameSettingsProvider);
};

} // ns game_logic

#endif // GAMESETTINGS_HOLDER_H__