#include "precompiled.h"
#include "mainscreen.h"
#include "screen_manager.h"

bool ScreenManager::Initialize() {
  assert(screens_[activeidx_] == nullptr);

  screens_[SM_MainScreen] = new MainGameScreen(this, 0, 0, 1280, 1024);
  if (screens_[SM_MainScreen]->Initialize())
    return true;

  return false;
}