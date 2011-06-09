#ifndef GAMEENGINE_H__
#define GAMEENGINE_H__

#include <d2d1.h>
#include <list>
#include "misc.h"
#include "scoped_handle.h"
#include "scoped_pointer.h"
#include "vector2d.h"

class ISpaceShip;
class IProjectile;
class ICollidable;

class Direct2DRenderer;

class GameEngine {
public :
  GameEngine(HINSTANCE app_instance, int width, int height);

  ~GameEngine();

  bool Initialize();

  void RunMainLoop();

  void ProjectileFired(IProjectile* projectile);

  static Direct2DRenderer* GetRenderer() {
    return r_pointer_;
  }

private :
  static LRESULT CALLBACK WindowProcedureStub(
    HWND window,
    UINT msg,
    WPARAM w,
    LPARAM l
    );

  LRESULT Engine_WindowProc(
    UINT msg,
    WPARAM wparam,
    LPARAM lparam
    );

  void render_objects();

  void UpdateProjectilePositions(float delta);

  void DrawBackground();

  void handle_wm_close();

  void handle_wm_destroy() {
    ::PostQuitMessage(0);
  }

  void handle_keypress(UINT virt_code);

  static Direct2DRenderer*                        r_pointer_;
  static const wchar_t* const                     K_BKFileName;

  HINSTANCE                                       app_instance_;
  HWND                                            client_window_;
  int                                             client_width_;
  int                                             client_height_;
  gfx::vector2D                                   client_centre_;
  scoped_pointer<ISpaceShip>                      player_ship_;
  std::list<IProjectile*>                         fired_projectiles_;
  std::list<IProjectile*>                         meteorlist_;
  //
  // d2d renderer
  // 
  scoped_pointer<Direct2DRenderer>                r2d2_render_;
  scoped_pointer<ID2D1BitmapBrush, D2DInterface>  r_bkbrush_;
  bool                                            pause_flag_;
  float                                           last_time_;

  NO_CPY_CONSTRUCTORS(GameEngine);
};

#endif // GAMEENGINE_H__