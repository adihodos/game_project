#include "precompiled.h"
#include "d2drenderer.h"
#include "game_resourcecache.h"
#include "lazy_unique_instance.h"
#include "main_screen.h"
#include "screen_manager.h"
#include "solidcolor_button.h"

namespace {

const wchar_t* const K_MainMenuBackgroundFile = L"main_menu.jpg";

struct MainMenu_Controls {;

  enum control_ids {
    MM_CID_NewGame,
    MM_CID_Options,
    MM_CID_Highscores,
    MM_CID_Quit,
    MM_CID_FrameBox
  };

  static const float button_width;
  static const float button_height;

  const wchar_t* text;
  int id;
} mm_controls_initdata[] = { 
  { 
    L"New Game", MainMenu_Controls::MM_CID_NewGame 
  },
  { 
    L"Options", MainMenu_Controls::MM_CID_Options 
  },
  {
    L"High Scores", MainMenu_Controls::MM_CID_Highscores
  },
  {
    L"Quit", MainMenu_Controls::MM_CID_Quit
  }
};

const float MainMenu_Controls::button_width = 200;
const float MainMenu_Controls::button_height = 80;

} // anonymous ns

game_ui::MainMenu_Screen::MainMenu_Screen(
  Screen_Manager* manager, 
  const gfx::vector2D& position, 
  float width, 
  float height
  )
  : game_ui::IGameScreen(manager, position, width, height),
    resource_provider_(base::LazyUniqueInstance<GameResourceCache>::Get()),
    background_() {}

game_ui::MainMenu_Screen::~MainMenu_Screen() {}

bool game_ui::MainMenu_Screen::initialize() {
  assert(!background_);

  background_ = resource_provider_->GetBitmapHandle(K_MainMenuBackgroundFile);
  assert(background_);

  const float origin_x = 20.0f;
  const float origin_y = 20.0f;
  const float button_spacing = 10.0f;
  int count = 0;

  std::for_each(
    mm_controls_initdata, 
    mm_controls_initdata + _countof(mm_controls_initdata),
    [this, &count, origin_x, origin_y, button_spacing](MainMenu_Controls& control_data) {
      float control_pos_y = origin_y + 
        count * (MainMenu_Controls::button_height + button_spacing);

      SolidColorButton* new_button = new SolidColorButton(
        control_data.id, gfx::vector2D(origin_x, control_pos_y),
        MainMenu_Controls::button_width, MainMenu_Controls::button_height,
        nullptr,
        control_data.text, 4.0f
        );

      add_child(new_button);
      ++count;
  });

  focused_object_ = children_.front().get();
  children_.front()->set_focus();
  return true;
}

void game_ui::MainMenu_Screen::Draw(Direct2DRenderer* renderer) {
  ID2D1HwndRenderTarget* render_target(static_cast<ID2D1HwndRenderTarget*>(renderer->GetRendererTarget()));
  render_target->SetTransform(D2D1::Matrix3x2F::Identity());

  render_target->DrawBitmap(
    background_,
    D2D1::RectF(position_.x_, position_.y_, position_.x_ + width_, position_.y_ + height_)
    );

  //
  // draw child controls
  std::for_each(
    children_.begin(), children_.end(), 
    [renderer](std::shared_ptr<Widget>& child_ptr) {
      child_ptr->Draw(renderer);
  });
}

bool game_ui::MainMenu_Screen::KeyPressed(KeyboardEventArgs* kb_args) {
  if (kb_args->w_param == VK_TAB) {
    focus_next_control();
    return true;
  }

  Widget* child = child_consumes_event(&Widget::KeyPressed, kb_args);
  if (child) {
    on_button_clicked_or_keyactivated(child);
  }
  return true;
}

bool game_ui::MainMenu_Screen::KeyReleased(KeyboardEventArgs* kb_args) {
  child_consumes_event(&Widget::KeyReleased, kb_args);
  return true;
}

bool game_ui::MainMenu_Screen::LeftButtonDown(MouseEventArgs* ms_args) {
  Widget* child = child_consumes_event(&Widget::LeftButtonDown,  ms_args);
  handle_focus_change(child);
  if (child)
    on_button_clicked_or_keyactivated(child);

  return true;
}

bool game_ui::MainMenu_Screen::LeftButtonUp(MouseEventArgs* ms_args) {
  child_consumes_event(&Widget::LeftButtonUp, ms_args);
  return true;
}

bool game_ui::MainMenu_Screen::RightButtonDown(MouseEventArgs*) {
  return true;
}

bool game_ui::MainMenu_Screen::RightButtonUp(MouseEventArgs*) {
  return true;
}

bool game_ui::MainMenu_Screen::MouseMoved(MouseEventArgs* ms_args) {
  /*std::for_each(
    children_.begin(), children_.end(), 
    [ms_args](IScreenComponentContainer::value_type childp) {
      childp->MouseMoved(ms_args);
  });*/
  child_consumes_event(&Widget::MouseMoved, ms_args);
  return true;
}

void game_ui::MainMenu_Screen::on_button_clicked_or_keyactivated(Widget* child) {

  switch (child->get_id()) {
  case MainMenu_Controls::MM_CID_NewGame :
    OUT_DBG_MSG(L"New game");
    break;

  case MainMenu_Controls::MM_CID_Options :
    OUT_DBG_MSG(L"Options");
    break;

  case MainMenu_Controls::MM_CID_Highscores :
    OUT_DBG_MSG(L"High scores");
    break;

  case MainMenu_Controls::MM_CID_Quit :
    OUT_DBG_MSG(L"Quit!");
    screen_manager_->set_screen_inactive(this);
    break;

  default :
    OUT_DBG_MSG(L"Unknown control id %d, sender %#08x", child->get_id(), child);
    assert(false);
    break;
  }
  
}