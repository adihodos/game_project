//#ifndef GAMEENGINE_H__
//#define GAMEENGINE_H__
//
//#include <deque>
//#include <list>
//#include <d2d1.h>
//#include <gfx/vector2d.h>
//#include "basetypes.h"
//#include "lazy_unique_instance.h"
//#include "scoped_handle.h"
//#include "scoped_pointer.h"
//#include "screen_manager.h"
//
//class ISpaceShip;
//class IProjectile;
//class ICollidable;
//class GameResourceCache;
//class Direct2DRenderer;
//
//namespace game_ui {
//class Screen_Manager;
//}
//
//class GameEngine : private base::LazyUniqueInstanceLifeTraits<GameEngine> {
//public :
//  bool Initialize(HINSTANCE app_instance, float width, float height);
//
//  void RunMainLoop();
//
//  void ProjectileFired(IProjectile* projectile);
//
//  static Direct2DRenderer* GetRenderer() {
//    return r_pointer_;
//  }
//
//private :
//  friend class base::LazyUniqueInstanceLifeTraits<GameEngine>;
//
//  GameEngine();
//
//  ~GameEngine();
//
//  static LRESULT CALLBACK WindowProcedureStub(
//    HWND window,
//    UINT msg,
//    WPARAM w,
//    LPARAM l
//    );
//
//  LRESULT Engine_WindowProc(
//    UINT msg,
//    WPARAM wparam,
//    LPARAM lparam
//    );
//
//  bool render_objects();
//
//  void UpdateProjectilePositions(float delta);
//
//  void DrawBackground();
//
//  void handle_wm_close();
//
//  void handle_wm_destroy() {
//    ::PostQuitMessage(0);
//  }
//
//  void handle_keypress(UINT virt_code);
//
//  void handle_keyreleased(WPARAM w_param, LPARAM l_param);
//
//  void handle_lefbutton_down(WPARAM w_param, LPARAM l_param);
//
//  void handle_leftbutton_up(WPARAM w_param, LPARAM l_param);
//
//  void handle_mouse_move(WPARAM w, LPARAM l);
//
//  void handle_wm_activate(bool activated, bool minimized);
//
//  static Direct2DRenderer*                        r_pointer_;
//  static const wchar_t* const                     K_BKFileName;
//
//  HINSTANCE                                       app_instance_;
//  HWND                                            client_window_;
//  float                                           client_width_;
//  float                                           client_height_;
//  gfx::vector2D                                   client_centre_;
//  scoped_pointer<ISpaceShip>                      player_ship_;
//  std::list<IProjectile*>                         fired_projectiles_;
//  std::list<IProjectile*>                         meteorlist_;
//  //
//  // d2d renderer
//  // 
//  scoped_pointer<Direct2DRenderer>                r2d2_render_;
//  scoped_pointer<ID2D1BitmapBrush, D2DInterface>  r_bkbrush_;
//  bool                                            pause_flag_;
//  float                                           last_time_;
//  //scoped_pointer<ScreenManager>                   scmgr_;
//  game_ui::Screen_Manager                         screen_mgr_;
//  RECT                                            old_clip_rect_;
//
//  struct Engine_Implementation;
//  scoped_pointer<Engine_Implementation>           impl_;
//
//  NO_CPY_CONSTRUCTORS(GameEngine);
//};
//
//#endif // GAMEENGINE_H__