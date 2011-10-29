#ifndef SCREEN_MGR_H__
#define SCREEN_MGR_H__

#include <cassert>
#include "basetypes.h"
#include "fixed_stack.h"
#include "lazy_unique_instance.h"

namespace game_ui {
class IGameScreen;
}

namespace game_core {

class screen_manager : public base::LazyUniqueInstanceLifeTraits<screen_manager> {
public :
  void pop_screen();

  void push_screen(game_ui::IGameScreen* gs);

  game_ui::IGameScreen* get_active_screen() const {
    assert(!screens_.empty());
    return screens_.top();
  }

  bool initialize();

private :
  friend class base::LazyUniqueInstanceLifeTraits<screen_manager>;

  screen_manager();

  ~screen_manager();

  static const int kMaxScreensDepth = 16;

  base::fixed_stack<game_ui::IGameScreen*, kMaxScreensDepth> screens_;

  NO_CPY_CONSTRUCTORS(screen_manager);
};

typedef base::LazyUniqueInstance<screen_manager> Game_ScreenHandler;

} // namespace game_logic

#endif // !SCREEN_MGR_H__