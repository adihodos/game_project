#pragma once

#include <d2d1.h>
#include "basetypes.h"
#include "igamescreen.h"
#include "ikeyboardeventreceiver.h"
#include "imouseeventreceiver.h"
#include "scoped_pointer.h"

class Direct2DRenderer;
class GameResourceCache;
class UIPushButton;

class MainGameScreen : public IGameScreen {
public :
  MainGameScreen(
    ScreenManager* smgr,
    float x, float y, float bx, float by, 
    IGameScreen* parent = nullptr, bool active = true
    );

  bool Initialize();

  bool KeyPressed(KeyboardEventArgs*);

  bool KeyReleased(KeyboardEventArgs*);

  bool LeftButtonDown(MouseEventArgs* args);

  bool LeftButtonUp(MouseEventArgs* args);

  bool RightButtonDown(MouseEventArgs* args);

  bool RightButtonUp(MouseEventArgs* args);

  bool MouseMoved(MouseEventArgs* args);

  void Draw(Direct2DRenderer* renderer);

private :
  GameResourceCache*            resprovider_;
  scoped_pointer<UIPushButton>  testbtn_;
  NO_CPY_CONSTRUCTORS(MainGameScreen);
};