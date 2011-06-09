#include "precompiled.h"
#include "circle.h"
#include "d2drenderer.h"
#include "game_engine.h"
#include "icollidable.h"
#include "ispaceship.h"
#include "iprojectile.h"
#include "meteor.h"
#include "resource.h"
#include "sa32_thunderbolt.h"
#include "vector2d.h"

namespace {

const wchar_t* const K_WindowClassName = L"SA32 WndClass";

}

Direct2DRenderer* GameEngine::r_pointer_;

const wchar_t* const GameEngine::K_BKFileName = L"carina_2_nebulae.jpg";

GameEngine::GameEngine(
  HINSTANCE app_instance,
  int width,
  int height
  )
  : app_instance_(app_instance),
    client_window_(nullptr),
    client_width_(width),
    client_height_(height),
    client_centre_(width / 2, height / 2),
    player_ship_(),
    fired_projectiles_(),
    meteorlist_(),
    r2d2_render_(),
    r_bkbrush_(),
    pause_flag_(false),
    last_time_(timeGetTime()) {}

GameEngine::~GameEngine() {}

bool
GameEngine::Initialize() {
  assert(client_window_ == nullptr);

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

  client_window_ = ::CreateWindowExW(
    0L,
    K_WindowClassName,
    L"Asteroid blaster!!",
    WS_OVERLAPPED | WS_SYSMENU,
    0, 0,
    client_width_ + 6,
    client_height_ + 52,
    nullptr,
    nullptr, 
    app_instance_,
    this);

  if (!client_window_)
    return false;

  r2d2_render_.reset(
    new Direct2DRenderer(client_window_, client_width_, client_height_));
  if (!r2d2_render_->Initialize())
    return false;

  r_pointer_ = r2d2_render_.get();

  //
  // Load the bitmap and create the brush
  ID2D1Bitmap* bitmap = nullptr;
  std::wstring filepath(utility::GetApplicationResourceDirectory());
  filepath.append(GameEngine::K_BKFileName);

  HRESULT ret_code = utility::LoadBitmapFromFile(
    r2d2_render_->GetRendererTarget(),
    r2d2_render_->GetImagingFactory(),
    filepath.c_str(),
    0,
    0,
    &bitmap);

  if (FAILED(ret_code))
    return false;

  ID2D1BitmapBrush* tmpbrush = nullptr;
  ret_code = r2d2_render_->GetRendererTarget()->CreateBitmapBrush(bitmap, &tmpbrush);
  if (FAILED(ret_code))
    return false;
  r_bkbrush_.reset(tmpbrush);

  player_ship_.reset(new SA32_Thunderbolt(
    r2d2_render_->GetRendererTarget(),
    r2d2_render_->GetImagingFactory(),
    client_centre_));

  meteorlist_.push_back(new SpaceMeteorite(
    r2d2_render_->GetRendererTarget(),
    r2d2_render_->GetImagingFactory(),
    gfx::vector2D(client_centre_.x_ + 250, client_centre_.y_ + 250),
    40));

  meteorlist_.push_back(new SpaceMeteorite(
    r2d2_render_->GetRendererTarget(),
    r2d2_render_->GetImagingFactory(),
    gfx::vector2D(client_centre_.x_ - 250, client_centre_.y_ - 250),
    90));

  ::ShowWindow(client_window_, SW_NORMAL);
  ::UpdateWindow(client_window_);
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

  default :
    return ::DefWindowProcW(client_window_, msg_code, w_param, l_param);
  }  
}

void GameEngine::handle_wm_close() {
  DestroyWindow(client_window_);
}

void GameEngine::handle_keypress(UINT virt_code) {
  switch (virt_code) {
  case VK_LEFT :
    player_ship_->Rotate(-3.0f);
    break;

  case VK_RIGHT :
    player_ship_->Rotate(3.0f);
    break;

  case VK_SPACE :
    player_ship_->FireRockets(this);
    break;

  default :
    break;
  }
}

void GameEngine::render_objects() {
  float current_time = timeGetTime();
  float delta_time = (current_time - last_time_) * 0.001f;

  //
  // Update projectile positions and do collision checks first
  UpdateProjectilePositions(delta_time);

  r2d2_render_->BeginRenderContent();

  DrawBackground();  

  //
  // render projectiles
  std::for_each(
    fired_projectiles_.begin(),
    fired_projectiles_.end(),
    [this](IProjectile* projectile) -> void {
      projectile->Draw(r2d2_render_.get());
  });

  //
  // render meteorites
  std::for_each(
    meteorlist_.begin(),
    meteorlist_.end(),
    [this](IProjectile* asteroid) -> void {
      asteroid->Draw(r2d2_render_.get());
  });

  //
  // Render player's ship
  player_ship_->Draw(r2d2_render_.get());

  r2d2_render_->EndRenderingContent();
  last_time_ = current_time;
  ::Sleep(20);
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