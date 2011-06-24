#include "precompiled.h"
#include "d2drenderer.h"
#include "game_engine.h"
#include "game_resourcecache.h"
#include "helpers.h"
#include "icollidable.h"
#include "igamescreen.h"
#include "ispaceship.h"
#include "iprojectile.h"
#include "lazy_unique_instance.h"
#include "meteor.h"
#include "resource.h"
#include "sa32_thunderbolt.h"
#include "screen_manager.h"

namespace {

const wchar_t* const K_WindowClassName = L"SA32 WndClass";

}

Direct2DRenderer* GameEngine::r_pointer_;

const wchar_t* const GameEngine::K_BKFileName = L"carina_2_nebulae.jpg";

GameEngine::GameEngine()
  : app_instance_(),
    client_window_(nullptr),
    client_width_(),
    client_height_(),
    client_centre_(),
    player_ship_(),
    fired_projectiles_(),
    meteorlist_(),
    r2d2_render_(),
    r_bkbrush_(),
    pause_flag_(false),
    last_time_(timeGetTime()),
    scmgr_(),
    old_clip_rect_() {}

GameEngine::~GameEngine() {
  ::ClipCursor(nullptr);
}

bool
GameEngine::Initialize(HINSTANCE inst, int width, int height) {
  assert(app_instance_ == nullptr);
  assert(client_window_ == nullptr);

  app_instance_ = inst;
  client_width_ = width;
  client_height_ = height;
  client_centre_.x_ = width / 2;
  client_centre_.y_ = height / 2;

  WNDCLASSEXW classdata;
  classdata.cbSize = sizeof(WNDCLASSEXW);
  classdata.style = CS_HREDRAW | CS_VREDRAW;
  classdata.lpfnWndProc = &GameEngine::WindowProcedureStub;
  classdata.cbWndExtra = classdata.cbClsExtra = 0;
  classdata.hInstance = app_instance_;
  classdata.hIcon = ::LoadIconW(nullptr, IDI_APPLICATION);
  classdata.hIconSm = nullptr;
  classdata.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
  classdata.hbrBackground = static_cast<HBRUSH>(::GetStockObject(NULL_BRUSH));
  classdata.lpszMenuName = nullptr;
  classdata.lpszClassName = K_WindowClassName;


  if (!::RegisterClassExW(&classdata))
    return false;

  RECT windowrect;
  windowrect.left = windowrect.top = 0;
  windowrect.right = client_width_;
  windowrect.bottom = client_height_;
  ::AdjustWindowRectEx(&windowrect, WS_POPUP, false, WS_EX_APPWINDOW);

  //assert(::ClipCursor(&windowrect));

  client_window_ = ::CreateWindowExW(
    WS_EX_APPWINDOW, K_WindowClassName, L"Asteroid blaster!!", WS_POPUP,
    0, 0, windowrect.right, windowrect.bottom, nullptr, nullptr, app_instance_,
    this);

  if (!client_window_)
    return false;

  r2d2_render_.reset(
    new Direct2DRenderer(client_window_, client_width_, client_height_));
  if (!r2d2_render_->Initialize())
    return false;

  //
  // Initialize resource cache
  base::LazyUniqueInstance<GameResourceCache>::Get()->Initialize(
    static_cast<ID2D1HwndRenderTarget*>(r2d2_render_->GetRendererTarget()));

  r_pointer_ = r2d2_render_.get();

  scmgr_.reset(new ScreenManager());
  if (!scmgr_->Initialize())
    return false;

  scmgr_->SetActiveScreen(ScreenManager::SM_MainScreen);

  ::ShowWindow(client_window_, SW_NORMAL);
  ::UpdateWindow(client_window_);
  BOOL ret_val = ClipCursor(&windowrect);
  assert(ret_val);
  return true;
}

void GameEngine::ProjectileFired(IProjectile* projectile) {
  fired_projectiles_.push_back(projectile);
}

#pragma push_macro("max")
#undef max
void
GameEngine::RunMainLoop() {
  MSG msg_data = { 0 } ;

  for (; msg_data.message != WM_QUIT;) {
    if (::PeekMessageW(&msg_data, 0, 0, 0, PM_REMOVE)) {
      ::TranslateMessage(&msg_data);
      ::DispatchMessageW(&msg_data);
    } else {
      render_objects();
    }
  }
}
#pragma pop_macro("max")

LRESULT
CALLBACK
GameEngine::WindowProcedureStub(
  HWND window,
  UINT msg,
  WPARAM wmsg,
  LPARAM lmsg
  )
{
  if (msg == WM_NCCREATE) {
    GameEngine* engine = static_cast<GameEngine*>(
      (reinterpret_cast<CREATESTRUCTW*>(lmsg))->lpCreateParams);
    ::SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(engine));
    engine->client_window_ = window;
    return TRUE;
  }

  GameEngine* engine = reinterpret_cast<GameEngine*>(
    ::GetWindowLongPtr(window, GWLP_USERDATA));
  if (!engine)
    return 0;

  return engine->Engine_WindowProc(msg, wmsg, lmsg);
}

LRESULT
GameEngine::Engine_WindowProc(
  UINT msg_code,
  WPARAM w_param,
  LPARAM l_param
  )
{
  switch (msg_code) {
  case WM_CLOSE :
    handle_wm_close();
    return 0L;
    break;

  case WM_DESTROY :
    handle_wm_destroy();
    return 0L;
    break;

  case WM_KEYDOWN :
    handle_keypress(static_cast<UINT>(w_param));
    return 0L;
    break;

  case WM_LBUTTONDOWN : {
    SetCapture(client_window_);
    MouseEventArgs args(MouseEventArgs::FromLParamWParam(w_param, l_param));
    scmgr_->LeftButtonDown(&args);
                        }
                        break;

  case WM_LBUTTONUP : {
    ReleaseCapture();
    MouseEventArgs args(MouseEventArgs::FromLParamWParam(w_param, l_param));
    scmgr_->LeftButtonUp(&args);
  }
                      return 0L;
  break;

  case WM_ACTIVATE :
    handle_wm_activate(
      (LOWORD(w_param) == WA_ACTIVE || LOWORD(w_param) == WA_CLICKACTIVE),
      HIWORD(w_param) != 0);
    return 0L;
    break;

  case WM_MOUSEMOVE :
    handle_mouse_move(w_param, l_param);
    break;

  default :
    return ::DefWindowProcW(client_window_, msg_code, w_param, l_param);
  }  
}

void GameEngine::handle_wm_close() {
  DestroyWindow(client_window_);
}

void GameEngine::handle_wm_activate(bool activated, bool minimized) {
  //OUT_DBG_MSG(L"Window is %s, minimized %s", activated ? L"activated" : L"deactivated", minimized ? L"true" : L"false");
  /*if (activated && !minimized) {
    ::SetCapture(client_window_);
    return;
  }

  ReleaseCapture();*/
}

void GameEngine::handle_mouse_move(WPARAM w, LPARAM l) {
  MouseEventArgs args(MouseEventArgs::FromLParamWParam(w, l));
  scmgr_->MouseMoved(&args);
}

void GameEngine::handle_keypress(UINT virt_code) {
  //BYTE keystate[256];
  //if (!GetKeyboardState(keystate))
  //  return;

  ///*float rotation = 0.0f;
  //if (keystate[VK_LEFT] & (1 << 7))
  //  rotation = -3.0f;
  //else if (keystate[VK_RIGHT] & (1 << 7))
  //  rotation = 3.0f;
  //else
  //  rotation = 0.0f;*/

  //if (keystate[VK_LEFT] & (1 << 7))
  //  player_ship_->MoveX(-1.0f);
  //else if (keystate[VK_RIGHT] & (1 << 7))
  //  player_ship_->MoveX(1.0f);

  //if (keystate[VK_SPACE] & (1 << 7))
  //    player_ship_->FireRockets(this);

  //if (keystate[VK_CONTROL] & (1 << 7))
  //    player_ship_->FirePlasmaGun(this);
}

void GameEngine::render_objects() {
  float current_time = timeGetTime();
  float delta_time = (current_time - last_time_) * 0.001f;

  //
  // Update projectile positions and do collision checks first
  //UpdateProjectilePositions(delta_time);

  r2d2_render_->BeginRenderContent();

  IGameScreen* active_screen(scmgr_->GetActiveScreen());
  if (active_screen)
    active_screen->Draw(r_pointer_);

  /*r2d2_render_->GetRendererTarget()->FillRectangle(
    D2D1::RectF(0, 0, client_width_, client_height_),
    base::LazyUniqueInstance<GameResourceCache>::Get()->GetSolidColorBrushHandle(D2D1::ColorF::DeepSkyBlue)
    );*/

  //DrawBackground();  

  ////
  //// render projectiles
  //std::for_each(
  //  fired_projectiles_.begin(),
  //  fired_projectiles_.end(),
  //  [this](IProjectile* projectile) -> void {
  //    projectile->Draw(r2d2_render_.get());
  //});

  ////
  //// render meteorites
  //std::for_each(
  //  meteorlist_.begin(),
  //  meteorlist_.end(),
  //  [this](IProjectile* asteroid) -> void {
  //    asteroid->Draw(r2d2_render_.get());
  //});

  ////
  //// Render player's ship
  //player_ship_->Draw(r2d2_render_.get());

  //IGameScreen* activescreen = scmgr_->GetActiveScreen();
  //if (activescreen)
  //  activescreen->Draw(r2d2_render_.get());
  //else
  //  // render scene

  r2d2_render_->EndRenderingContent();
  last_time_ = current_time;
}

void GameEngine::UpdateProjectilePositions(float delta) {
  RECT window_geometry;
  GetClientRect(client_window_, &window_geometry);

  auto itr = fired_projectiles_.begin();
  auto list_end = fired_projectiles_.end();

  while (itr != list_end) {
    IProjectile* current_projectile = *itr;
    current_projectile->UpdatePosition(delta);


    auto it_asteroids = meteorlist_.begin();
    auto it_a_end = meteorlist_.end();

    while (it_asteroids != it_a_end) {
      IProjectile* asteroid = *it_asteroids;

      if (!asteroid) {
        //
        // may have been erased by a previous collision
        ++it_asteroids;
        continue;
      }

      if (current_projectile->CollidesWith(*asteroid, nullptr)) {
        //
        // destroy both the projectile and the asteroid
        delete current_projectile;
        *itr = current_projectile = nullptr;

        delete asteroid;
        *it_asteroids = nullptr;
        break;
      }

      ++it_asteroids;
    }

    if (!current_projectile) {
      //
      // projectile collided with asteroid and was destroyed.
      ++itr;
      continue;
    }

    POINT projectile_location = { 
      current_projectile->GetPosition()->x_,
      current_projectile->GetPosition()->y_
    };

    if (!PtInRect(&window_geometry, projectile_location)) {
      //
      // out of the client window, so destroy it
      delete *itr;
      *itr = nullptr;
    }
    ++itr;
  }

  //
  // Compact list by removing erased projectiles
  fired_projectiles_.remove(nullptr);
  meteorlist_.remove(nullptr);
}

void GameEngine::DrawBackground() {
  r2d2_render_->GetRendererTarget()->FillRectangle(
    D2D1::RectF(0, 0, client_width_, client_height_),
    r_bkbrush_.get());
}