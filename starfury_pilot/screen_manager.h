#pragma once

#include "basetypes.h"
#include "ikeyboardeventreceiver.h"
#include "imouseeventreceiver.h"

class IGameScreen;

class ScreenManager : public IKeyboardEventReceiver, public IMouseEventReceiver {
public :

  enum Screens {
    SM_MainScreen,
    SM_GameScreen,
    SM_LoadScreen,
    SM_OptionsScreen
  };

  ScreenManager()
    : activeidx_(0) { memset(screens_, 0, sizeof(screens_)); }

  bool Initialize();

  bool KeyPressed(KeyboardEventArgs* kargs) {
    return GetActiveScreen()->KeyPressed(kargs);
  }

  bool KeyReleased(KeyboardEventArgs* kargs) {
    return GetActiveScreen()->KeyReleased(kargs);
  }

  bool LeftButtonDown(MouseEventArgs* msargs) {
    return GetActiveScreen()->LeftButtonDown(msargs);
  }

  bool LeftButtonUp(MouseEventArgs* args) {
    return GetActiveScreen()->LeftButtonUp(args);
  }

  bool RightButtonDown(MouseEventArgs* msargs) {
    return GetActiveScreen()->RightButtonDown(msargs);
  }

  bool RightButtonUp(MouseEventArgs* args) {
    return GetActiveScreen()->RightButtonUp(args);
  }

  bool MouseMoved(MouseEventArgs* args) {
    return GetActiveScreen()->MouseMoved(args);
  }

  void Draw(Direct2DRenderer* r) {
    GetActiveScreen()->Draw(r);
  }

  void SetActiveScreen(ScreenManager::Screens scr) {
    assert(scr >= SM_MainScreen && scr <= SM_OptionsScreen);
    activeidx_ = scr;
  }

  IGameScreen* GetActiveScreen() const {
    assert(activeidx_ >= SM_MainScreen && activeidx_ <= SM_OptionsScreen);
    assert(screens_[activeidx_] != nullptr);
    return screens_[activeidx_];
  }

private :
  IGameScreen*  screens_[4];
  int           activeidx_;
  NO_CPY_CONSTRUCTORS(ScreenManager);
};