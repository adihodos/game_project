#ifndef SA32_THUNDERBOLT_H__
#define SA32_THUNDERBOLT_H__

#include <d2d1.h>
#include <gfx/vector2.h>
#include "basetypes.h"
#include "scoped_pointer.h"

namespace game_core {

class sa32_thunderbolt {
public :
  sa32_thunderbolt(int hp = 100);

  void rotate(float amount);

  void move(int direction);

  void draw();

  bool initialize();

  void set_position(const gfx::vector2& pos) {
    position_ = pos;
  }

  const gfx::vector2& get_position() const {
    return position_;
  }

  const gfx::vector2& get_ship_geometry() const {
    return geometry_;
  }

private :
  //static const float                          K_VelocityY;

  int                                         hp_;
  gfx::vector2                                position_;
  gfx::vector2                                gun_direction_;
  gfx::vector2                                geometry_;
  float                                       rotate_angle_;
  ID2D1Bitmap*                                ship_texture_;

  NO_CPY_CONSTRUCTORS(sa32_thunderbolt);
};

} // ns game_entity

#endif // SA32_THUNDERBOLT_H__