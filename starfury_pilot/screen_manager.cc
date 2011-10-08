#include "precompiled.h"
//#include "game_settings_provider.h"
//#include "main_screen.h"
//#include "scoped_pointer.h"
//#include "screen_manager.h"
//
//game_ui::Screen_Manager::Screen_Manager() : working_stack_() {
//  memset(screens_, 0, sizeof(screens_));
//}
//
//game_ui::Screen_Manager::~Screen_Manager() {
//  std::for_each(
//    screens_, screens_ + sizeof(screens_), 
//    [](IGameScreen* screen) { delete screen; });
//}
//
//bool game_ui::Screen_Manager::initialize() {
//  game_logic::GameSettingsProvider* settings_provider(
//    base::LazyUniqueInstance<game_logic::GameSettingsProvider>::Get());
//
//  scoped_pointer<game_ui::IGameScreen> current_screen(new MainMenu_Screen(
//    this, gfx::vector2D::K_NullVector, 
//    settings_provider->get_screen_width(),
//    settings_provider->get_screen_height()
//    ));
//
//  if (!current_screen->initialize())
//    return false;
//
//  current_screen->set_active();
//  //
//  // Release the pointer before returning since init was successfull.
//  current_screen.release();
//  return true;
//}