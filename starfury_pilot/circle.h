#ifndef CIRCLE_H__
#define CIRCLE_H__

#include "vector2d.h"

namespace gfx {

class circle {
public :
  circle() {}

  circle(const vector2D& center, float radius)
    : center_(center), radius_(radius) {}

  bool intersects(const circle& other) const {
    return (center_ - other.center_).magnitude() <= (radius_ + other.radius_);
  }

public :
  vector2D center_;
  float radius_;
};

} // ns gfx

#endif // CIRCLE_H__