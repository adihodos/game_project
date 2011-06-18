#pragma once

#include <memory>
#include <gfx/vector2d.h>
#include "basetypes.h"
#include "fixed_stack.h"
#include "idrawable.h"
#include "ikeyboardeventreceiver.h"
#include "imouseeventreceiver.h"

class Direct2DRenderer;
class ScreenManager;
class UIComponent;

class IGameScreen : public IKeyboardEventReceiver, public IMouseEventReceiver, public IDrawable {
public :
  IGameScreen(
    ScreenManager* smgr, 
    float x, float y, float bx, float by, 
    IGameScreen* parent = nullptr, bool active = true)
    : smgr_(smgr), topleft_(x, y), bottomright_(bx, by), 
      parent_(parent), active_(active) {}

  virtual ~IGameScreen() {}

  virtual bool Initialize() = 0;

  bool IsActive() const;

  void SetActive();

  void SetInactive();

protected :
  ScreenManager*                              smgr_;
  gfx::vector2D                               topleft_;
  gfx::vector2D                               bottomright_;
  IGameScreen*                                parent_;
  bool                                        active_;
  std::vector<std::shared_ptr<UIComponent>>  childcontrols_;
};