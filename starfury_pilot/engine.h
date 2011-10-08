#ifndef ENGINE_H__
#define ENGINE_H__

#include "basetypes.h"
#include "lazy_unique_instance.h"
#include "scoped_pointer.h"

namespace game_logic {

class IMouse_EventReceiver;
class IKeyboard_EventReceiver;

class Game_Engine : public base::LazyUniqueInstanceLifeTraits<Game_Engine> {
public :
  bool initialize(void);

  void app_main();

  void quit();

  void set_trap_mouseevents_dest(IMouse_EventReceiver*);

  void set_trap_keyboardevents_dest(IKeyboard_EventReceiver*);
private :
  Game_Engine();

  ~Game_Engine();

  friend class base::LazyUniqueInstanceLifeTraits<Game_Engine>;

  struct Implementation_Details;
  scoped_pointer<Implementation_Details>  e_impl_;

  NO_CPY_CONSTRUCTORS(Game_Engine);
};

typedef base::LazyUniqueInstance<Game_Engine> GameEngine_Handle;

} // namespace game_logic

#endif // !ENGINE_H__