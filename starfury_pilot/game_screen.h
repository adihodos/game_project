#ifndef GAMESCREEN_H__
#define GAMESCREEN_H__

#include "control_container.h"
#include "idrawable.h"
#include "ikeyboardeventreceiver.h"
#include "imouseeventreceiver.h"

namespace game_ui {

class IScreenComponent;
class Screen_Manager;

class IGameScreen : public IDrawable, 
                    public IKeyboardEventReceiver, public IMouseEventReceiver, 
                    public IScreenComponentContainer {
public :
  IGameScreen(
    Screen_Manager* screen_manager, 
    const gfx::vector2D& position, 
    float width, 
    float height
    );

  virtual ~IGameScreen();

  void set_active();

  void set_inactive();

  virtual bool initialize() = 0;
protected :
  void handle_focus_change(IScreenComponent* new_focused_object);

  void focus_next_control();

  gfx::vector2D     position_;
  float             width_;
  float             height_;
  Screen_Manager*   screen_manager_;
  IScreenComponent* focused_object_;
};

} // ns game_ui

#endif // GAMESCREEN_H__