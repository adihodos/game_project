//#ifndef SCREEN_MANAGER_H__
//#define SCREEN_MANAGER_H__
//
//#include "basetypes.h"
//#include "fixed_stack.h"
//#include "game_screen.h"
//#include "ikeyboardeventreceiver.h"
//#include "imouseeventreceiver.h"
//
//namespace game_ui {
//
//class Screen_Manager : public IKeyboardEventReceiver, public IMouseEventReceiver {
//public :
//
//  enum Screens {
//    SM_MainScreen,
//    SM_GameScreen,
//    SM_LoadScreen,
//    SM_OptionsScreen
//  };
//
//  Screen_Manager();
//
//  ~Screen_Manager();
//
//  bool initialize();
//
//  void set_screen_inactive(IGameScreen* screen) {
//    assert(!working_stack_.empty());
//    assert(working_stack_.top() == screen);
//    working_stack_.pop();
//  }
//
//  void set_screen_active(IGameScreen* screen) {
//    working_stack_.push(screen);
//  }
//
//  IGameScreen* get_active_screen() const {
//    return working_stack_.empty() ? nullptr : working_stack_.top();
//  }
//
//  bool KeyPressed(KeyboardEventArgs* kargs) {
//    return get_active_screen()->KeyPressed(kargs);
//  }
//
//  bool KeyReleased(KeyboardEventArgs* kargs) {
//    return get_active_screen()->KeyReleased(kargs);
//  }
//
//  bool LeftButtonDown(MouseEventArgs* m_args) {
//    return get_active_screen()->LeftButtonDown(m_args);
//  }
//
//  bool LeftButtonUp(MouseEventArgs* m_args) {
//    return get_active_screen()->LeftButtonUp(m_args);
//  }
//
//  bool RightButtonDown(MouseEventArgs* m_args) {
//    return get_active_screen()->RightButtonDown(m_args);
//  }
//
//  bool RightButtonUp(MouseEventArgs* m_args) {
//    return get_active_screen()->RightButtonUp(m_args);
//  }
//
//  bool MouseMoved(MouseEventArgs* m_args) {
//    return get_active_screen()->MouseMoved(m_args);
//  }
//
//private :
//  //
//  // adjust this if screens may go deep more than 8 levels
//  static const int K_default_screens_maxdepth = 8;  
//  base::fixed_stack<IGameScreen*, K_default_screens_maxdepth>   working_stack_;
//  IGameScreen*                                                  screens_[4];
//  NO_CPY_CONSTRUCTORS(Screen_Manager);
//};
//
//} // ns game_ui
//
//#endif // SCREEN_MANAGER_H__