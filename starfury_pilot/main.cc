#include "precompiled.h"
#include "game_engine.h"
#include "helpers.h"
#include "scoped_handle.h"
#include "scoped_pointer.h"

HINSTANCE app_instance;

namespace {

const int K_DefaultWindowWidth = 1280;
const int K_DefaultWindowHeight = 1024;

} // anonymous ns

INT WINAPI wWinMain(
  HINSTANCE instance,
  HINSTANCE prv_inst,
  LPWSTR cmd_line,
  int cmd_show
  )
{
  std::wstring app_dir;
  utility::GetApplicationBaseDirectory(&app_dir);

  {
    com_initialize_helper init_com;
    if (!init_com) {
      ::MessageBoxW(nullptr, L"Error", L"Failed to init COM!", MB_ICONERROR);
      return EXIT_FAILURE;
    }

    app_instance = instance;
    GameEngine engine(instance, K_DefaultWindowWidth, K_DefaultWindowHeight);
    if (!engine.Initialize())
      ::MessageBoxW(nullptr, L"Error!", L"Failed to initialize!", MB_ICONERROR);
    else
      engine.RunMainLoop();
  }

  return 0;
}