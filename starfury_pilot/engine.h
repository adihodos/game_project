#pragma once

#include "basetypes.h"
#include "lazy_unique_instance.h"
#include "scoped_pointer.h"

namespace gfx {
  class vector2;
  class matrix3X3;
  class rectangle;
}

namespace game_core {

class game_engine : public base::LazyUniqueInstanceLifeTraits<game_engine> {
public :
  bool initialize(void);

  void run();

  void quit();

  ID2D1HwndRenderTarget* get_rendertarget() const;

  ID2D1Factory* get_factory() const;

  const gfx::matrix3X3& get_world_transform_matrix() const;

  const gfx::vector2& get_world_geometry() const;

  const gfx::rectangle get_world_clip_rectangle() const;

private :
  game_engine();

  ~game_engine();

  friend class base::LazyUniqueInstanceLifeTraits<game_engine>;

  struct implementation_details;
  scoped_pointer<implementation_details>  e_impl_;

  NO_CPY_CONSTRUCTORS(game_engine);
};

typedef base::LazyUniqueInstance<game_core::game_engine> engine_handle;

} // namespace game_core