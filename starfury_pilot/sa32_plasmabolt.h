#ifndef SA32_PLASMABOLT_H__
#define SA32_PLASMABOLT_H__

#include <d2d1.h>
#include <gfx/circle.h>
#include <gfx/vector2d.h>
#include "basetypes.h"
#include "icollidable.h"
#include "iprojectile.h"
#include "scoped_pointer.h"

class Direct2DRenderer;

namespace game_entity {

class SA32_PlasmaBolt : public IProjectile {
public :
  SA32_PlasmaBolt(
    const gfx::vector2D& pos, 
    const gfx::vector2D& velocity
    );

  int GetDamage() const {
    return SA32_PlasmaBolt::K_Damage;
  }

  int GetHP() const {
    return SA32_PlasmaBolt::K_HitPoints;
  }

  gfx::vector2D* GetPosition() {
    return &pos_;
  }

  gfx::vector2D* GetVelocity() {
    return &velocity_;
  }

  const gfx::circle* GetBoundingCircle() const {
    return &bounding_circle_;
  }

  void Draw(Direct2DRenderer* r_render);

  void UpdatePosition(float delta_time);

private :
  static const wchar_t* const K_ResourceFileName;
  static const int K_Damage = 20;
  static const int K_HitPoints = 10;  
  static const float K_PlasmaBoltRadius;

  gfx::vector2D                                             pos_;
  gfx::vector2D                                             velocity_;
  gfx::circle                                               bounding_circle_;
  gfx::vector2D                                             size_;
  int                                                       hp_;
  int                                                       damage_;
};

} // ns game_logic

#endif // !SA32_PLASMABOLT_H__