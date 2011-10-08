#include "precompiled.h"
#include "engine.h"
#include "game_resourcecache.h"
#include "game_settings_provider.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "play_screen.h"
#include "renderer.h"

namespace {
const wchar_t* const R_BackgroundFileName = L"carina_2_nebula.jpg";
}

void 
game_logic::Play_Screen::draw() {
  ps_impl_->draw();
}

bool
game_logic::Play_Screen::Mouse_Event(
  game_logic::MouseEvent_Args* /* e_args */
  )
{
  //
  // Don't care about mouse events in the play screen
  return false;
}

bool
game_logic::Play_Screen::Key_Event(
  game_logic::KeyEvent_Args* key_args
  )
{
  return ps_impl_->handle_key_events(key_args);
}

void
game_logic::Play_Screen::update(float delta_tm) {
  ps_impl_->update(delta_tm);
}

struct game_logic::Play_Screen::Implementation_Details {
  Implementation_Details(const gfx::vector2D& pos, float w, float h)
    : position_(pos), width_(w), height_(h) {}

  ~Implementation_Details() { ::ShowCursor(true); }

  void draw();

  void update(float delta_tm);

  bool handle_key_events(game_logic::KeyEvent_Args* key_args);

  bool initialize() { ::ShowCursor(false); return true; }

  gfx::vector2D   position_;
  float           width_;
  float           height_;
};

void 
game_logic::Play_Screen::Implementation_Details::draw() {
  ID2D1Bitmap* background_bmp(Game_ResourceCache::Get()->GetBitmapHandle(
    R_BackgroundFileName));
  assert(background_bmp != nullptr);

  Game_Renderer::Get()->Get_HWNDRenderTarget()->DrawBitmap(
    background_bmp, D2D1::RectF(position_.x_, position_.y_, width_, height_));
}

game_logic::Play_Screen::Play_Screen() : ps_impl_() {}

game_logic::Play_Screen::~Play_Screen() {}

bool
game_logic::Play_Screen::initialize() {
  assert(!ps_impl_);
  ps_impl_.reset(
    new Implementation_Details(gfx::vector2D::K_NullVector,
                               Game_SettingsProvider::Get()->get_screen_width(),
                               Game_SettingsProvider::Get()->get_screen_height()));

  return ps_impl_->initialize();
}

bool 
game_logic::Play_Screen::Implementation_Details::handle_key_events(
  game_logic::KeyEvent_Args* key_args
  )
{
  if (key_args->Get_KeyCode() == VK_ESCAPE && key_args->Key_Down()) {
      OUT_DBG_MSG(L"Esc Down");

      if (::MessageBoxW(nullptr, L"Quit Game ? ", L"Quit", MB_ICONQUESTION) == IDOK)
        GameEngine_Handle::Get()->quit();

      return true;
    }

    OUT_DBG_MSG(L"Esc Up");
    return false;
}

void 
game_logic::Play_Screen::Implementation_Details::update(
  float /* delta_tm */
  ) 
{
}