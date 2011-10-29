#include "precompiled.h"
#include "engine.h"
#include "game_resourcecache.h"
#include "game_settings_provider.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "play_screen.h"
#include "renderer.h"
#include "sa32_thunderbolt.h"

namespace {
const wchar_t* const R_BackgroundFileName = L"carina_nebula.jpg";
}

struct game_logic::Play_Screen::Implementation_Details {
  Implementation_Details(const gfx::vector2D& pos, float w, float h)
    : position_(pos), width_(w), height_(h), 
      player_ship_() {}

  ~Implementation_Details() { 
    ::ShowCursor(true); 
  }

  void draw();

  void update(float delta_tm);

  bool handle_key_events(game_core::KeyEvent_Args* key_args);

  bool initialize() { 
    ::ShowCursor(false);
    return true; 
  }

  gfx::vector2D                 position_;
  float                         width_;
  float                         height_;
  game_entity::SA32_Thunderbolt player_ship_;
};

void 
game_core::Play_Screen::Implementation_Details::draw() {
  ID2D1Bitmap* background_bmp(Game_ResourceCache::Get()->GetBitmapHandle(
    R_BackgroundFileName));
  assert(background_bmp != nullptr);

  //
  // Draw using the painter's algorithm
  Game_Renderer::Get()->SetTransformationMatrix(gfx::matrix3X3::K_Identity);
  Game_Renderer::Get()->Draw_Bitmap(
    background_bmp, D2D1::RectF(position_.x_, position_.y_, width_, height_));
  player_ship_.Draw(nullptr);
}

bool 
game_core::Play_Screen::Implementation_Details::handle_key_events(
  game_core::KeyEvent_Args* key_args
  )
{
  if (key_args->Get_KeyCode() == VK_ESCAPE && key_args->Key_Down()) {
      ::ShowCursor(true);
      if (::MessageBoxW(nullptr, L"Quit Game ? ", L"Quit", 
                        MB_ICONQUESTION | MB_YESNO) == IDYES) {
        GameEngine_Handle::Get()->quit();
        return true;
      }
      ::ShowCursor(true);
  }

  if (!key_args->Key_Down() || (key_args->Get_KeyCode() != VK_LEFT && 
                                key_args->Get_KeyCode() != VK_RIGHT))
    return false;

  float direction = key_args->Get_KeyCode() == VK_LEFT ? -1.0f : 1.0f;
  if (HIBYTE(::GetKeyState(VK_LCONTROL)) & 1) {
    player_ship_.Rotate(10.0f * direction);
  } else {
    player_ship_.MoveX(direction);
  }
  return true;
}

void
game_core::Play_Screen::draw() {
  ps_impl_->draw();
}

bool
game_core::Play_Screen::Mouse_Event(
  game_core::MouseEvent_Args* /* e_args */
  )
{
  //
  // Don't care about mouse events in the play screen
  return false;
}

bool
game_core::Play_Screen::Key_Event(
  game_core::KeyEvent_Args* key_args
  )
{
  return ps_impl_->handle_key_events(key_args);
}

void
game_core::Play_Screen::update(float delta_tm) {
  ps_impl_->update(delta_tm);
}

void 
game_core::Play_Screen::Implementation_Details::update(
  float /* delta_tm */
  ) 
{
}

game_core::Play_Screen::Play_Screen() : ps_impl_() {}

game_core::Play_Screen::~Play_Screen() {}

bool
game_core::Play_Screen::initialize() {
  assert(!ps_impl_);
  ps_impl_.reset(
    new Implementation_Details(gfx::vector2D::K_NullVector,
                               Game_SettingsProvider::Get()->get_screen_width(),
                               Game_SettingsProvider::Get()->get_screen_height()));

  return ps_impl_->initialize();
}