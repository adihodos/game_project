#pragma once

#include "basetypes.h"
#include "lazy_unique_instance.h"

namespace game_core {

class settings_provider : 
  public base::LazyUniqueInstanceLifeTraits<settings_provider> {

public :
  bool initialize(HINSTANCE instance);

  float get_screen_width() const {
    return screen_width_;
  }

  float get_screen_height() const {
    return screen_height_;
  }

  int get_screen_depth() const {
    return screen_depth_;
  }

  HINSTANCE get_hinstance() const {
    return instance_;
  }
private :
  settings_provider();

  ~settings_provider();

  friend class base::LazyUniqueInstanceLifeTraits<settings_provider>;

  float       screen_width_;
  float       screen_height_;
  int         screen_depth_;
  HINSTANCE   instance_;

  NO_CPY_CONSTRUCTORS(settings_provider);
};

typedef base::LazyUniqueInstance<settings_provider> settings_provider_handle;

} // ns game_logic