#include "precompiled.h"
#include "engine.h"
#include "event_queue.h"
#include "gamefont.h"
#include "game_resourcecache.h"
#include "game_session.h"
#include "game_settings_provider.h"
#include "helpers.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "scoped_pointer.h"
#include "timer.h"

struct game_core::game_engine::implementation_details {
  implementation_details()
    : last_tm_(0.0f), 
      wnd_geometry_(), 
      game_wnd_(nullptr), 
      event_queue_(),
      initialized_(false),
      minimized_(false),
      active_(false) {}

  ~implementation_details() {
    //
    // restore cursor movement area
    ::ClipCursor(nullptr);
  }

  bool initialize();

  bool create_main_window();

  bool initialize_renderer();

  bool create_device_independent_resources();

  bool create_device_dependent_resources();

  void main_loop();

  void quit() {
    ::PostMessageW(game_wnd_, WM_CLOSE, 0, 0);
  }

  LRESULT real_window_procedure(
    UINT msg_code, WPARAM w_param, LPARAM l_param
    );

  bool process_events();

  void render_next_frame();

  void draw_fps_count(float diff_time);

  void restrict_cursor_movement_to_game_screen() {
    //
    // Confine the cursor to the window's area
    ::SetCapture(game_wnd_);
    RECT client_rectangle;
    ::GetClientRect(game_wnd_, &client_rectangle);
    ::ClipCursor(&client_rectangle);
  }

  void handle_wm_destroy() { ::PostQuitMessage(0); }

  void handle_wm_close() { ::DestroyWindow(game_wnd_); }

  void handle_wm_activate(bool activated, bool minimized);

  void handle_wm_paint() {
    PAINTSTRUCT ps;
    ::BeginPaint(game_wnd_, &ps);
    ::EndPaint(game_wnd_, &ps);
  }

  void handle_mouse_event(WPARAM, int px, int py) {
    //BREAK_INTO_DEBUGGER();
  }

  void handle_key_down_event(WPARAM code, LPARAM) {
    //BREAK_INTO_DEBUGGER();
    if (code == VK_ESCAPE) {
      quit();
      return;
    }

    game_session_->key_down(code);
  }

  void handle_key_up_event(WPARAM, LPARAM) {
  }

  game_core::interface_key_event_receiver* get_key_events_receiver() const {
    BREAK_INTO_DEBUGGER();
    return nullptr;
  }

  game_core::interface_mouse_event_receiver* get_mouse_events_receiver() const {
    BREAK_INTO_DEBUGGER();
    return nullptr;
  }

  void handle_game_session_event() {
  }

  void handle_ui_menu_event() {
  }

  typedef scoped_pointer<ID2D1HwndRenderTarget, D2DInterface> render_target_hwnd_t;
  typedef scoped_pointer<ID2D1Factory, D2DInterface> d2_factory_t;

  static const wchar_t* const kWindowClassName;

  static LRESULT CALLBACK window_procedure_stub(
    HWND window, UINT msg, WPARAM w_param, LPARAM l_param
    );

  float                                     last_tm_;
  gfx::vector2                              wnd_geometry_;
  HWND                                      game_wnd_;
  game_core::sys_event_queue                event_queue_;
  IDWriteTextFormat*                        txt_fmt_;
  ID2D1SolidColorBrush*                     txt_brush_;
  base::WMMSimplePrecisionTimer             timer_;
  bool                                      initialized_;
  bool                                      minimized_;
  bool                                      active_;
  d2_factory_t                              factory_;
  render_target_hwnd_t                      rendertarget_;
  scoped_pointer<game_core::game_session>   game_session_;
  gfx::matrix3X3                            world_transform_;
  
  NO_CPY_CONSTRUCTORS(implementation_details);
};

const wchar_t* const 
game_core::game_engine::implementation_details::kWindowClassName =
  L"Main_WindowClass";

//
// static
LRESULT
CALLBACK
game_core::game_engine::implementation_details::window_procedure_stub( 
  HWND window, 
  UINT msg, 
  WPARAM w_param, 
  LPARAM l_param 
  )
{
  if (msg == WM_CREATE) {
    CREATESTRUCTW* pcs(reinterpret_cast<CREATESTRUCTW*>(l_param));
    implementation_details* pid(static_cast<implementation_details*>(pcs->lpCreateParams));
    ::SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pid));
    return TRUE;
  }

  implementation_details* forwarder(
    reinterpret_cast<implementation_details*>(::GetWindowLongPtrW(window, GWLP_USERDATA)));
  return forwarder ? forwarder->real_window_procedure(msg, w_param, l_param) :
    ::DefWindowProcW(window, msg, w_param, l_param);
}

bool
game_core::game_engine::implementation_details::initialize() {
  if (!create_main_window())
    return false;

  if (!initialize_renderer())
    return false;

  if (!resource_cache_handle::Get()->initialize(rendertarget_.get()))
    return false;

  game_session_.reset(new game_session());
  if (!game_session_->initialize_new_game())
    return false;

  //
  // Show main window
  restrict_cursor_movement_to_game_screen();
  ::ShowWindow(game_wnd_, SW_NORMAL);
  ::UpdateWindow(game_wnd_);
  return true;
}

bool
game_core::game_engine::implementation_details::initialize_renderer() {
  assert(!rendertarget_);
  world_transform_ = gfx::matrix3X3::translation(wnd_geometry_.x_ / 2, wnd_geometry_.y_ / 2);
  return create_device_independent_resources() && 
         create_device_dependent_resources();
}

bool
game_core::game_engine::implementation_details::create_device_independent_resources() {
  if (factory_.get())
    factory_.reset();

  HRESULT ret_code;
  D2D1_DEBUG_LEVEL debug_level = D2D1_DEBUG_LEVEL_NONE;

#if defined(_DEBUG)
  //debug_level = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

  D2D1_FACTORY_OPTIONS factory_opts = { debug_level };
  TRACE_D2DCALL(&ret_code, ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
                                               factory_opts, 
                                               factory_.get_owned_ptr()));
  return ret_code == S_OK;
}

bool
game_core::game_engine::implementation_details::create_device_dependent_resources() {
  if (rendertarget_.get())
    return true;

  D2D1_RENDER_TARGET_PROPERTIES rendertarget_props = {
    D2D1_RENDER_TARGET_TYPE_HARDWARE,
    {
      DXGI_FORMAT_R8G8B8A8_UNORM,
      D2D1_ALPHA_MODE_PREMULTIPLIED
    },
    0.0f, 0.0f,
    D2D1_RENDER_TARGET_USAGE_NONE,
    D2D1_FEATURE_LEVEL_10
  };

  D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_rendertarget_props = {
    game_wnd_,
    D2D1::SizeU(static_cast<UINT>(wnd_geometry_.x_), 
                static_cast<UINT>(wnd_geometry_.y_)),
    D2D1_PRESENT_OPTIONS_NONE
  };

  HRESULT ret_code;
  TRACE_D2DCALL(&ret_code, factory_->CreateHwndRenderTarget(
    rendertarget_props, hwnd_rendertarget_props, rendertarget_.get_owned_ptr()));
  return ret_code == S_OK;
}

void
game_core::game_engine::implementation_details::main_loop() {
  for (;;) {
    bool should_continue = process_events();
    if (!should_continue)
      break;

    render_next_frame();
  }
  /*
  timer_.Reset();

  float time_elapsed = 0.0f;

  for (;;) {
    event_queue_.Get_NextEvent();
    if (!event_queue_.Dispatch_Event())
      break;

    float delta_time = timer_.DeltaNowMsWithUpdate();
    time_elapsed += delta_time * 0.001f;

    if (time_elapsed >= 0.01) {
      Game_ScreenHandler::Get()->get_active_screen()->update(0.0f);
      Game_Renderer::Get()->Begin_Rendering();
      Game_ScreenHandler::Get()->get_active_screen()->draw();
      DrawFPSCount(delta_time);

      Game_Renderer::Get()->End_Rendering();
      time_elapsed = 0.0f;
    }
  }
  */
}

bool game_core::game_engine::implementation_details::process_events() {
  game_core::sys_event ev(event_queue_.get_next_event());
  
  switch (ev.se_type_) {
  case game_core::sys_event::se_type_none :
    ::Sleep(5);
    return true;
    break;

  case game_core::sys_event::se_type_quit :
    return false;
    break;

  case game_core::sys_event::se_type_key :
    //
    // dispatch key
    return true;
    break;

  case game_core::sys_event::se_type_mouse :
    //
    // dispatch mouse event
    return true;
    break;

  default :
    OUT_DBG_MSG(L"Unknown event type : %d, args [%d, %d, %#08x]", 
                ev.se_type_, ev.se_arg1_, ev.se_arg2_, ev.se_ptr_);
    return false;
    break;
  }
}

void
game_core::game_engine::implementation_details::render_next_frame() {
  create_device_dependent_resources();

  rendertarget_->BeginDraw();

  rendertarget_->SetTransform(D2D1::Matrix3x2F::Identity());
  rendertarget_->Clear(D2D1::ColorF(D2D1::ColorF::Orange));

  game_session_->draw();

  if (rendertarget_->EndDraw() == D2DERR_RECREATE_TARGET) {
    rendertarget_.reset();
    resource_cache_handle::Get()->discard_cached_resourcess();
  }
}

void 
game_core::game_engine::implementation_details::draw_fps_count(
  float diff_time
  ) 
{
  static int frame_count = 0;
  static float total_time_ms = 0.0f;
  static wchar_t buff[1024];
  static D2D1_RECT_F text_rect(D2D1::RectF(20, 20, 200, 200));

  ++frame_count;
  total_time_ms += diff_time;
  if (total_time_ms >= 1000.0f) {
    _snwprintf_s(buff, _countof(buff) - 1, L"FPS count : %d", frame_count);      
    frame_count = 0;
    total_time_ms = 0.0f;
  }
  /*rendertarget_->SetTransform(D2D1::Matrix3x2F::Identity());
  rendertarget_->DrawText(
    buff, static_cast<UINT>(wcslen(buff)), txt_fmt_, 
    text_rect, D2D1::ColorF(D2D1::ColorF::White), 
    D2D1_DRAW_TEXT_OPTIONS_CLIP);*/
}

bool
game_core::game_engine::implementation_details::create_main_window() {
  assert(game_wnd_ == nullptr);

  wnd_geometry_.x_ = settings_provider_handle::Get()->get_screen_width();
  wnd_geometry_.y_ = settings_provider_handle::Get()->get_screen_height();

  WNDCLASSEXW classdata;
  classdata.cbSize = sizeof(WNDCLASSEXW);
  classdata.style = CS_HREDRAW | CS_VREDRAW;
  classdata.lpfnWndProc = &implementation_details::window_procedure_stub;
  classdata.cbWndExtra = classdata.cbClsExtra = 0;
  classdata.hInstance = settings_provider_handle::Get()->get_hinstance();
  classdata.hIcon = ::LoadIconW(nullptr, IDI_APPLICATION);
  classdata.hIconSm = nullptr;
  classdata.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
  classdata.hbrBackground = nullptr;
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
    WS_EX_APPWINDOW, kWindowClassName, L"Starfury Pilot", WS_POPUP,
    0, 0, windowrect.right, windowrect.bottom, nullptr, nullptr, 
    settings_provider_handle::Get()->get_hinstance(), this
    );

  return game_wnd_ != nullptr;
}

LRESULT
game_core::game_engine::implementation_details::real_window_procedure( 
  UINT msg_code, 
  WPARAM w_param, 
  LPARAM l_param 
  )
{
  switch (msg_code) {
  case WM_ACTIVATE :
    handle_wm_activate(
      (LOWORD(w_param) == WA_ACTIVE || LOWORD(w_param) == WA_CLICKACTIVE),
      HIWORD(w_param) != 0);
    return 0L;
    break;

  case WM_ERASEBKGND :
    return true;
    break;

  case WM_CLOSE :
    handle_wm_close();
    return 0L;
    break;

  case WM_PAINT :
    handle_wm_paint();
    break;

  case WM_DESTROY :
    handle_wm_destroy();
    return 0L;
    break;

  case WM_KEYDOWN :
    handle_key_down_event(w_param, l_param);
    return 0L;
    break;

  case WM_KEYUP :
    handle_key_up_event(w_param, l_param);
    return 0L;
    break;

  case WM_LBUTTONDOWN : 
  case WM_LBUTTONUP : 
  case WM_RBUTTONDOWN : 
  case WM_RBUTTONUP :
  case WM_MOUSEMOVE :
    handle_mouse_event(w_param, GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param));
    return 0L;
    break;

  default :
    break;
  }

  return ::DefWindowProcW(game_wnd_, msg_code, w_param, l_param);
}

void
game_core::game_engine::implementation_details::handle_wm_activate(
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
    restrict_cursor_movement_to_game_screen();
  }
}

game_core::game_engine::game_engine() : e_impl_() {}

game_core::game_engine::~game_engine() {}

bool 
game_core::game_engine::initialize() {
  assert(!e_impl_);
  e_impl_.reset(new implementation_details());
  return e_impl_->initialize();
}

void
game_core::game_engine::run() {
  e_impl_->main_loop();
}

void
game_core::game_engine::quit() {
  e_impl_->quit();
}

ID2D1HwndRenderTarget*
game_core::game_engine::get_rendertarget() const {
  return e_impl_->rendertarget_.get();
}

ID2D1Factory*
game_core::game_engine::get_factory() const {
  return e_impl_->factory_.get();
}

const gfx::matrix3X3& 
game_core::game_engine::get_world_transform_matrix() const {
  return e_impl_->world_transform_;
}

const gfx::vector2&
game_core::game_engine::get_world_geometry() const {
  return e_impl_->wnd_geometry_;
}

const gfx::rectangle
game_core::game_engine::get_world_clip_rectangle() const {
  return gfx::rectangle(gfx::vector2::null, e_impl_->wnd_geometry_);
}