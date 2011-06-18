#ifndef ISPACESHIP_H__
#define ISPACESHIP_H__

#include <Windows.h>
#include "idrawable.h"

namespace gfx {
class back_buffer;
}

class GameEngine;

class ISpaceShip : public IDrawable {
public :
  ISpaceShip() : IDrawable() {}

  virtual ~ISpaceShip() {}

  virtual void Rotate(float amount) = 0;

  virtual void FireRockets(GameEngine* world) = 0;

  virtual void FirePlasmaGun(GameEngine* world) = 0;

  virtual void MoveX(float amount = 0.0f) = 0;

  virtual void BlowUp() = 0;
};

#endif // ISPACESHIP_H__