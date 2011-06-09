#ifndef IPROJECTILE_H__
#define IPROJECTILE_H__

#include "icollidable.h"
#include "idrawable.h"

namespace gfx {
class vector2D;
class back_buffer;

}

class IProjectile : public IDrawable, public ICollidable {
public :
  IProjectile() : IDrawable() {}

  virtual ~IProjectile() {}

  virtual int GetDamage() const = 0;

  virtual int GetHP() const = 0;

  virtual void UpdatePosition(float delta_time) = 0;
};

#endif // IPROJECTILE_H__