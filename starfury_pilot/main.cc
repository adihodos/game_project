#include "precompiled.h"
#include "atexit_manager.h"
#include "game_engine.h"
#include "lazy_unique_instance.h"
#include "helpers.h"
#include "resultwrapper.h"
#include "scoped_handle.h"
#include "scoped_pointer.h"

HINSTANCE app_instance;

namespace {

const int K_DefaultWindowWidth = 1280;
const int K_DefaultWindowHeight = 1024;

bool GetActiveScreenResolution(D2D1_SIZE_U* size_info) {
  assert(size_info);

  DEVMODEW moninfo;
  if (::EnumDisplaySettingsW(nullptr, ENUM_CURRENT_SETTINGS, &moninfo)) {
    size_info->width = moninfo.dmPelsWidth;
    size_info->height = moninfo.dmPelsHeight;
    return true;    
  }

  return false;
}

} // anonymous ns

INT WINAPI wWinMain(
  HINSTANCE instance,
  HINSTANCE prv_inst,
  LPWSTR cmd_line,
  int cmd_show
  )
{
  com_initialize_helper init_com;

  if (!init_com) {
    ::MessageBoxW(nullptr, L"Error", L"Failed to init COM!", MB_ICONERROR);
    return EXIT_FAILURE;
  }

  {
    base::atexit_manager exit_manager;

    app_instance = instance;

    SIZE screensize;
    if (!utility::GetScreenSize(&screensize))
      return EXIT_FAILURE;

    if (!base::LazyUniqueInstance<GameEngine>::Get()->Initialize(
        instance, screensize.cx, screensize.cy)) {   
        ::MessageBoxW(nullptr, L"Error!", L"Failed to initialize!", MB_ICONERROR);
    } else {
      base::LazyUniqueInstance<GameEngine>::Get()->RunMainLoop();
    }
  }

  return 0;
}