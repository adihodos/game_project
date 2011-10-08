#include "precompiled.h"
#include "engine.h"
#include "game_resourcecache.h"
#include "game_screen.h"
#include "game_settings_provider.h"
#include "helpers.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "renderer.h"
#include "screen_mgr.h"
#include "sys_event.h"

struct game_logic::Game_Engine::Implementation_Details {
  Implementation_Details() : 
    last_tm_(0.0f), wnd_geometry_(), game_wnd_(nullptr), event_queue_() {}

  ~Implementation_Details() {
    //
    // restore cursor movement area
    ::ClipCursor(nullptr);
  }

  bool Initialize();

  void Main_Loop();

  void Quit() {
    ::PostMessageW(game_wnd_, WM_CLOSE, 0, 0);
  }

  bool Create_MainWindow();

  static LRESULT CALLBACK Window_ProcedureStub(
    HWND window, UINT msg, WPARAM w_param, LPARAM l_param
    );

  LRESULT Window_Procedure(
    UINT msg_code, WPARAM w_param, LPARAM l_param
    );

  void set_trap_mouseevents_dest(game_logic::IMouse_EventReceiver* receiver) {
    event_queue_.TrapMouseEvents_Dest(receiver);
  }

  void set_trap_keyboardevents_dest(game_logic::IKeyboard_EventReceiver* receiver) {
    event_queue_.TrapKeyEvents_Dest(receiver);
  }

  void HandleWM_Destroy() { ::PostQuitMessage(0); }

  void HandleWM_Close() { ::DestroyWindow(game_wnd_); }

  void HandleWM_Activate(bool activated, bool minimized);

  static const wchar_t* const kWindowClassName;

  float                       last_tm_;
  gfx::vector2D               wnd_geometry_;
  HWND                        game_wnd_;
  game_logic::Sys_EventQueue  event_queue_;
  NO_CPY_CONSTRUCTORS(Implementation_Details);
};

const wchar_t* const 
game_logic::Game_Engine::Implementation_Details::kWindowClassName =
  L"Main_WindowClass";

//
// static
LRESULT
CALLBACK
game_logic::Game_Engine::Implementation_Details::Window_ProcedureStub( 
  HWND window, 
  UINT msg, 
  WPARAM w_param, 
  LPARAM l_param 
  )
{
  if (msg == WM_CREATE) {
    CREATESTRUCTW* pcs(reinterpret_cast<CREATESTRUCTW*>(l_param));
    Implementation_Details* pid(static_cast<Implementation_Details*>(pcs->lpCreateParams));
    ::SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pid));
    return TRUE;
  }

  Implementation_Details* forwarder(
    reinterpret_cast<Implementation_Details*>(::GetWindowLongPtrW(window, GWLP_USERDATA)));
  return forwarder ? forwarder->Window_Procedure(msg, w_param, l_param) :
    ::DefWindowProcW(window, msg, w_param, l_param);
}

bool
game_logic::Game_Engine::Implementation_Details::Initialize() {
  if (!Create_MainWindow())
    return false;

  //
  // Initialize assets
  if (!Game_ScreenHandler::Get()->initialize()) {
    return false;
  }

  if (!Game_Renderer::Get()->Initialize(game_wnd_, wnd_geometry_))
    return false;

  if (!Game_ResourceCache::Get()->Initialize(Game_Renderer::Get()->Get_HWNDRenderTarget())) {
    return false;
  }

  //
  // Show main window
  ::ShowWindow(game_wnd_, SW_NORMAL);
  ::UpdateWindow(game_wnd_);
  //
  // Confine the cursor to the window's area
  RECT client_rectangle;
  ::GetClientRect(game_wnd_, &client_rectangle);
  ::ClipCursor(&client_rectangle);
  return true;
}

void
game_logic::Game_Engine::Implementation_Details::Main_Loop() {
  for (;;) {
    event_queue_.Get_NextEvent();
    if (!event_queue_.Dispatch_Event())
      break;

    Game_ScreenHandler::Get()->get_active_screen()->update(0.0f);

    Game_Renderer::Get()->Begin_Rendering();
    Game_ScreenHandler::Get()->get_active_screen()->draw();
    Game_Renderer::Get()->End_Rendering();
  }
}

bool
game_logic::Game_Engine::Implementation_Details::Create_MainWindow() {
  assert(game_wnd_ == nullptr);

  wnd_geometry_.x_ = Game_SettingsProvider::Get()->get_screen_width();
  wnd_geometry_.y_ = Game_SettingsProvider::Get()->get_screen_height();

  WNDCLASSEXW classdata;
  classdata.cbSize = sizeof(WNDCLASSEXW);
  classdata.style = CS_HREDRAW | CS_VREDRAW;
  classdata.lpfnWndProc = &Implementation_Details::Window_ProcedureStub;
  classdata.cbWndExtra = classdata.cbClsExtra = 0;
  classdata.hInstance = Game_SettingsProvider::Get()->get_hinstance();
  classdata.hIcon = ::LoadIconW(nullptr, IDI_APPLICATION);
  classdata.hIconSm = nullptr;
  classdata.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
  classdata.hbrBackground = static_cast<HBRUSH>(::GetStockObject(NULL_BRUSH));
  classdata.lpszMenuName = nullptr;
  classdata.lpszClassName = kWindowClassName;

  if (!::RegisterClassExW(&classdata))
    return false;

  RECT windowrect;
  windowrect.left = windowrect.top = 0;
  windowrect.right = static_cast<int>(wnd_geometry_.x_);
  windowrect.bottom = static_cast<int>(wnd_geometry_.y_);
  ::AdjustWindowRectEx(&windowrect, WS_POPUP, false, WS_EX_APPWINDOW);

  game_wnd_ = ::CreateWindowExW(
    WS_EX_APPWINDOW, kWindowClassName, L"Asteroid blaster!!", WS_POPUP,
    0, 0, windowrect.right, windowrect.bottom, nullptr, nullptr, 
    Game_SettingsProvider::Get()->get_hinstance(), this
    );

  return game_wnd_ != nullptr;
}

LRESULT
game_logic::Game_Engine::Implementation_Details::Window_Procedure( 
  UINT msg_code, 
  WPARAM w_param, 
  LPARAM l_param 
  )
{
  switch (msg_code) {
  case WM_ACTIVATE :
    HandleWM_Activate(
      (LOWORD(w_param) == WA_ACTIVE || LOWORD(w_param) == WA_CLICKACTIVE),
      HIWORD(w_param) != 0);
    return 0L;
    break;

  case WM_CLOSE :
    HandleWM_Close();
    return 0L;
    break;

  case WM_DESTROY :
    HandleWM_Destroy();
    return 0L;
    break;

  case WM_KEYDOWN :
  case WM_KEYUP :
    event_queue_.Post_KeyEvent(
      KeyEvent_Args(KeyEvent_Args::From_SysInputEvent(w_param, l_param)));
    return 0L;
    break;

  case WM_LBUTTONDOWN : 
  case WM_LBUTTONUP : 
  case WM_RBUTTONDOWN : 
  case WM_RBUTTONUP :
  case WM_MOUSEMOVE :
    event_queue_.Post_MouseEvent(
      MouseEvent_Args(MouseEvent_Args::From_SysInputEvent(msg_code, w_param, l_param)));
    return 0L;
    break;

  default :
    return ::DefWindowProcW(game_wnd_, msg_code, w_param, l_param);
    break;
  }
}

void
game_logic::Game_Engine::Implementation_Details::HandleWM_Activate(
  bool activated,
  bool minimized
  )
{
  UNREFERENCED_PARAMETER(minimized);
  if (!activated) {
    //
    // Restore cursor movement area
    ::ClipCursor(nullptr);
  } else {
    RECT client_r;
    ::GetClientRect(game_wnd_, &client_r);
    ::ClipCursor(&client_r);
  }
}

game_logic::Game_Engine::Game_Engine() : e_impl_() {}

game_logic::Game_Engine::~Game_Engine() {}

bool 
game_logic::Game_Engine::initialize() {
  assert(!e_impl_);
  e_impl_.reset(new Implementation_Details());
  return e_impl_->Initialize();
}

void
game_logic::Game_Engine::app_main() {
  e_impl_->Main_Loop();
}

void
game_logic::Game_Engine::quit() {
  e_impl_->Quit();
}

void
game_logic::Game_Engine::set_trap_keyboardevents_dest(
  game_logic::IKeyboard_EventReceiver* rec
  )
{
  e_impl_->set_trap_keyboardevents_dest(rec);
}

void
game_logic::Game_Engine::set_trap_mouseevents_dest(
  game_logic::IMouse_EventReceiver* rec
  )
{
  e_impl_->set_trap_mouseevents_dest(rec);
}