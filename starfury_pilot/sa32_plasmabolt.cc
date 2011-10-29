#include "precompiled.h"
#include "d2drenderer.h"
#include "game_resourcecache.h"
#include "helpers.h"
#include "lazy_unique_instance.h"
#include "sa32_plasmabolt.h"

const wchar_t* const game_entity::SA32_PlasmaBolt::K_ResourceFileName =
  L"sa32_plasmabolt.png";

const float game_entity::SA32_PlasmaBolt::K_PlasmaBoltRadius = 15.0f;

game_entity::SA32_PlasmaBolt::SA32_PlasmaBolt(
  const gfx::vector2D& pos, 
  const gfx::vector2D& velocity
  )
  : IProjectile(),
    pos_(pos),
    velocity_(velocity),
    bounding_circle_(pos_, SA32_PlasmaBolt::K_PlasmaBoltRadius),
    hp_(SA32_PlasmaBolt::K_HitPoints),
    damage_(SA32_PlasmaBolt::K_Damage)
{
  base::LazyUniqueInstance<GameResourceCache>::Get()->get_texture_size(
    SA32_PlasmaBolt::K_ResourceFileName, &size_);
}

void game_entity::SA32_PlasmaBolt::UpdatePosition(float delta_time) {
  pos_ += velocity_ * delta_time;
  bounding_circle_.center_ = pos_;
}

void game_entity::SA32_PlasmaBolt::Draw(
  Direct2DRenderer* r_render
  )
{
//  D2D1_ELLIPSE bolt_geometry(D2D1::Ellipse(pos_, size_.x_ / 2, size_.y_ / 2));

  //ID2D1BitmapBrush* bolt_texture(base::LazyUniqueInstance<GameResourceCache>::Get()->GetBitmapBrushHandle(SA32_PlasmaBolt::K_ResourceFileName);
  //assert(bolt_texture);
  //
  // This is needed so that the brush aligns with the ellipse.
  /*bolt_texture->SetTransform(
    D2D1::Matrix3x2F::Translation(pos_.x_ - bolt_geometry.radiusX / 2,
                                  pos_.y_ - bolt_geometry.radiusY / 2));
  r_render->GetRendererTarget()->FillEllipse(bolt_geometry, bolt_texture);*/
}