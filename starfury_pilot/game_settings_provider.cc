#include "precompiled.h"
#include "game_settings_provider.h"
#include "helpers.h"

game_core::settings_provider::settings_provider() {}

game_core::settings_provider::~settings_provider() {}

bool game_core::settings_provider::initialize(HINSTANCE instance) {
  instance_ = instance;

  DEVMODEW dinfo;
  if (!::EnumDisplaySettingsW(nullptr, ENUM_CURRENT_SETTINGS, &dinfo))
    return false;

  screen_width_ = static_cast<float>(dinfo.dmPelsWidth);
  screen_height_ = static_cast<float>(dinfo.dmPelsHeight);
  screen_depth_ = dinfo.dmBitsPerPel;

  return true;
}