#pragma once

namespace gfx {

class vector2D;

class circle;

}

class ICollidable {
public :
  virtual ~ICollidable() {}

  virtual bool CollidesWith(
    const ICollidable& other, 
    gfx::vector2D* collision_pos
    );

  virtual gfx::vector2D* GetPosition() = 0;

  virtual gfx::vector2D* GetVelocity() = 0;

  virtual const gfx::circle* GetBoundingCircle() const = 0;
};