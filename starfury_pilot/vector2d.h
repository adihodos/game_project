#ifndef VECTOR2D_H__
#define VECTOR2D_H__

#include <cmath>
#include "helpers.h"

namespace gfx {

class vector2D {
public :
  union {
    struct {
      float x_;
      float y_;
    };
    float elements_[2];
  };

  static const vector2D Null_Vector;

  static const vector2D Unit_Vector;

  static const vector2D Unit_Vector_X;

  static const vector2D Unit_Vector_Y;

  vector2D() {}

  vector2D(float x, float y)
    : x_(x), y_(y) {}

  vector2D(const float (&arref)[2])
    : x_(arref[0]), y_(arref[1]) {}


  vector2D& operator+=(const vector2D& rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    return *this;
  }

  vector2D& operator-=(const vector2D& rhs) {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    return *this;
  }

  vector2D& operator*=(float scalar) {
    x_ *= scalar;
    y_ *= scalar;
    return *this;
  }

  vector2D& operator/=(float scalar) {
    x_ /= scalar;
    y_ /= scalar;
    return *this;
  }

  float sum_components_squared() const {
    return x_ * x_ + y_ * y_;
  }

  float magnitude() const {
    return std::sqrtf(sum_components_squared());
  }

  vector2D& normalize() {
    float magn = magnitude();
    if (magn == 0.0f) {
      x_ = y_ = 0.0f;
    } else {
      x_ /= magn;
      y_ /= magn;
    }

    return *this;
  }

  vector2D& rotate(float f) {
    float rads = utility::DegreesToRadians(f);
    x_ = x_ * std::cosf(rads) - y_ * std::sinf(rads);
    y_ = y_ * std::cosf(rads) + x_ * std::sinf(rads);
    return *this;
  }
};

inline 
const vector2D
operator+(
  const vector2D& left,
  const vector2D& right
  )
{
  return vector2D(left.x_ + right.x_, left.y_ + right.y_);
}

inline 
const vector2D
operator-(
  const vector2D& left,
  const vector2D& right
  )
{
  return vector2D(left.x_ - right.x_, left.y_ - right.y_);
}

inline
const vector2D
operator-(
  const vector2D& src
  )
{
  return vector2D(-src.x_, -src.y_);
}

inline
const vector2D
operator*(
  float scalar, 
  const vector2D& vec
  )
{
  return vector2D(scalar * vec.x_, scalar * vec.y_);
}

inline
const vector2D
operator*(
  const vector2D& vec,
  float scalar
  )
{
  return scalar * vec;
}

inline
const vector2D
operator/(
  const vector2D& vec,
  float scalar
  )
{
  return vector2D(vec.x_ / scalar, vec.y_ / scalar);
}

inline
float
dot_product(
  const vector2D& left,
  const vector2D& right
  )
{
  return left.x_ * right.x_ + left.y_ * right.y_;
}

} // ns gfx
#endif // VECTOR2D_H__