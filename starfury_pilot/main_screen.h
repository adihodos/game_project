//#ifndef MAINSCREEN_H__
//#define MAINSCREEN_H__
//
//#include "basetypes.h"
//#include "game_screen.h"
//
//class Direct2DRenderer;
//class GameResourceCache;
//
//namespace game_ui {
//
//class Widget2;
//class Screen_Manager;
//
//class MainMenu_Screen : public IGameScreen {
//public :
//  MainMenu_Screen(Screen_Manager* manager, const gfx::vector2D& position, 
//                  float width, float height);
//
//  ~MainMenu_Screen();
//
//  bool initialize();
//
//  bool KeyPressed(KeyboardEventArgs* args);
//
//  bool KeyReleased(KeyboardEventArgs* args);
//  
//  bool LeftButtonDown(MouseEventArgs* args);
//
//  bool LeftButtonUp(MouseEventArgs* args);
//
//  bool RightButtonDown(MouseEventArgs* args);
//
//  bool RightButtonUp(MouseEventArgs* args);
//
//  bool MouseMoved(MouseEventArgs* args);
//
//  void Draw(Direct2DRenderer* renderer);
//
//protected :
//  void on_button_clicked_or_keyactivated(Widget2* child);
//
//private :
//  GameResourceCache*                resource_provider_;
//  ID2D1Bitmap*                      background_;
//  NO_CPY_CONSTRUCTORS(MainMenu_Screen);
//};
//
//} // ns game_ui
//
//#endif // MAINSCREEN_H__