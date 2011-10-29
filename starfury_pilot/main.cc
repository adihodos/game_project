#include "precompiled.h"
#include "atexit_manager.h"
#include "engine.h"
#include "game_settings_provider.h"
#include "lazy_unique_instance.h"
#include "helpers.h"
#include "resultwrapper.h"
#include "scoped_handle.h"
#include "scoped_pointer.h"

INT WINAPI wWinMain(
  HINSTANCE instance,
  HINSTANCE prv_inst,
  LPWSTR cmd_line,
  int cmd_show
  )
{
  UNREFERENCED_PARAMETER(prv_inst);
  UNREFERENCED_PARAMETER(cmd_line);
  UNREFERENCED_PARAMETER(cmd_show);

  com_initialize_helper init_com;

  if (!init_com) {
    ::MessageBoxW(nullptr, L"Error", L"Failed to init COM!", MB_ICONERROR);
    return EXIT_FAILURE;
  }

  {
    base::atexit_manager exit_manager;

    game_core::settings_provider* settings_provider(
      base::LazyUniqueInstance<game_core::settings_provider>::Get());

    if (!settings_provider->initialize(instance)) {
      ::MessageBoxW(nullptr, L"Error", L"Error", MB_ICONERROR);
      return -1;
    }

    if (!game_core::engine_handle::Get()->initialize()) {
      OUT_DBG_MSG(L"Failed to initialize engine.");
      ::MessageBoxW(nullptr, L"Error!", L"Failed to initialize!", MB_ICONERROR);
    } else {
      game_core::engine_handle::Get()->run();
    }
  }

  return 0;
}