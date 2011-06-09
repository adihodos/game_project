#include "precompiled.h"
#include "circle.h"
#include "icollidable.h"
#include "vector2d.h"

bool ICollidable::CollidesWith(
  const ICollidable& other, 
  gfx::vector2D* collision_pos
  )
{
  gfx::circle other_c(*other.GetBoundingCircle());
  gfx::circle this_c(*GetBoundingCircle());
  gfx::vector2D testv(this_c.center_ - other_c.center_);
  return testv.magnitude() <= (this_c.radius_ + other_c.radius_);
}