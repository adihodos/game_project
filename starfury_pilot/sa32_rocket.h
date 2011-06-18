#pragma once

#include <d2d1.h>
#include <gfx/circle.h>
#include <gfx/vector2d.h>
#include "basetypes.h"
#include "icollidable.h"
#include "iprojectile.h"
#include "scoped_pointer.h"

class Direct2DRenderer;

class SA32_Rocket : public IProjectile {
public :
  SA32_Rocket(
    ID2D1RenderTarget* r_target, 
    IWICImagingFactory* factory, 
    const gfx::vector2D& pos, 
    const gfx::vector2D& velocity,
    float angle
    );

  int GetDamage() const {
    return SA32_Rocket::K_Damage;
  }

  int GetHP() const {
    return SA32_Rocket::K_HitPoints;
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
  static const int K_Damage = 100;
  static const int K_HitPoints = 20;  
  static const float K_PlasmaBoltRadius;

  gfx::vector2D                                             pos_;
  gfx::vector2D                                             velocity_;
  gfx::circle                                               bounding_circle_;
  int                                                       hp_;
  int                                                       damage_;
  scoped_pointer<ID2D1Bitmap, D2DInterface>                 texture_;
  const float                                               angle_;
  D2D1_POINT_2F                                             centerpt_;
  D2D1_SIZE_F                                               geometry_;

  NO_CPY_CONSTRUCTORS(SA32_Rocket);
};