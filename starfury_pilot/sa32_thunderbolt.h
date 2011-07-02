#ifndef SA32_THUNDERBOLT_H__
#define SA32_THUNDERBOLT_H__

#include <d2d1.h>
#include <gfx/vector2d.h>
#include "basetypes.h"
#include "ispaceship.h"
#include "scoped_pointer.h"

class GameEngine;
class Direct2DRenderer;

namespace game_entity {

class SA32_Thunderbolt : public ISpaceShip {
public :
  SA32_Thunderbolt(const gfx::vector2D& pos, 
                   int hp = 100, 
                   int lives = 1);

  void Rotate(float amount);

  void FireRockets(GameEngine* world);

  void FirePlasmaGun(GameEngine* world);

  void MoveX(float amount = 0.0f);

  void BlowUp() {
    OutputDebugStringW(__FUNCTIONW__ L" is not implmented!\n");
    assert(0);
  }

  void Draw(Direct2DRenderer* r_render);

private :
  static const wchar_t* const                 K_ResourceFileName;
  static const float                          K_VelocityY;
  int                                         hp_;
  gfx::vector2D                               position_;
  gfx::vector2D                               gun_direction_;
  D2D1_POINT_2F                               rocket_left_;
  D2D1_POINT_2F                               rocket_right_;
  gfx::vector2D                               ship_geometry_;
  float                                       rotate_angle_;
  bool                                        leftrpod_;
  NO_CPY_CONSTRUCTORS(SA32_Thunderbolt);
};

} // ns game_entity

#endif // SA32_THUNDERBOLT_H__