#pragma once

#include <d2d1.h>
#include <gfx/circle.h>
#include "basetypes.h"
#include "iprojectile.h"
#include "scoped_pointer.h"

class Direct2DRenderer;

class SpaceMeteorite : public IProjectile {
public :
  SpaceMeteorite(
    ID2D1RenderTarget* render_t,
    IWICImagingFactory* factory,
    const gfx::vector2D& pos,
    //const gfx::vector2D& velocity,
    float rotation_
    );

  gfx::vector2D* GetPosition() {
    return &pos_;
  }

  gfx::vector2D* GetVelocity() {
    return &velocity_;
  }

  const gfx::circle* GetBoundingCircle() const {
    return &bounding_circle_;
  }

  int GetDamage() const {
    assert(false);
    return 0;
  }

  int GetHP() const {
    assert(false);
    return 0;
  }

  void UpdatePosition(float delta_time);

  void Draw(Direct2DRenderer* r_renderer);

private :
  static const wchar_t* const                 K_BitmapFileName;
  gfx::circle                                 bounding_circle_;
  gfx::vector2D                               pos_;
  gfx::vector2D                               velocity_;
  scoped_pointer<ID2D1Bitmap, D2DInterface>   bitmap_;
  float                                       rotation_;
  D2D1_SIZE_F                                 geometry_;
#if defined(_DEBUG)
  scoped_pointer<ID2D1SolidColorBrush, D2DInterface> dbgbrush_;
#endif
  NO_CPY_CONSTRUCTORS(SpaceMeteorite);
};