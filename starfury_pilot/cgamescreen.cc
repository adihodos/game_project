#include "precompiled.h"
#include "cgamescreen.h"
#include "d2drenderer.h"
#include "sa32_thunderbolt.h"
#include "screen_manager.h"

game_ui::Playing_GameScreen::Playing_GameScreen(
  Screen_Manager* smgr, const gfx::vector2D& pos, float width, float height
  ) : IGameScreen(smgr, pos, width, height), playership_() {}

bool game_ui::Playing_GameScreen::initialize() {
  assert(!playership_);

  playership_.reset(new game_entity::SA32_Thunderbolt(
    gfx::vector2D(position_.x_ / 2, position_.y_ / 2)));

  return true;
}

bool game_ui::Playing_GameScreen::KeyPressed(KeyboardEventArgs* kb_args) {
  if (kb_args->w_param == VK_ESCAPE) {
    this->screen_manager_->set_screen_inactive(this);
  }

  return true;
}