#include "precompiled.h"
#include "d2drenderer.h"
#include "helpers.h"
#include "sa32_rocket.h"
#include "helpers.h"

const wchar_t* const SA32_Rocket::K_ResourceFileName =
    L"sa32_plasmabolt.png";

const float SA32_Rocket::K_PlasmaBoltRadius = 15.0f;

SA32_Rocket::SA32_Rocket(
  ID2D1RenderTarget* r_target, 
  IWICImagingFactory* factory, 
  const gfx::vector2D& pos, 
  const gfx::vector2D& velocity,
  float angle
  )
  : IProjectile(),
    pos_(pos),
    velocity_(velocity),
    bounding_circle_(pos_, SA32_Rocket::K_PlasmaBoltRadius),
    hp_(SA32_Rocket::K_HitPoints),
    damage_(SA32_Rocket::K_Damage),
    texture_(),
    plasmabrush_(),
    angle_(angle)
{
  std::wstring filepath(utility::GetApplicationResourceDirectory());
  filepath.append(SA32_Rocket::K_ResourceFileName);

  ID2D1Bitmap* rocketbmp = nullptr;
  HRESULT ret_code = utility::LoadBitmapFromFile(
                            r_target,
                            factory,
                            filepath.c_str(),
                            0,
                            0,
                            &rocketbmp);
  assert(SUCCEEDED(ret_code));
  texture_.reset(rocketbmp);
  
  ID2D1BitmapBrush* tmpbrush;
  ret_code = r_target->CreateBitmapBrush(texture_.get(), &tmpbrush);
  assert(SUCCEEDED(ret_code));
  plasmabrush_.reset(tmpbrush);
}

void SA32_Rocket::UpdatePosition(float delta_time) {
  pos_ += velocity_ * delta_time;
  bounding_circle_.center_ = pos_;
}

void SA32_Rocket::Draw(
  Direct2DRenderer* r_render
  )
{
  D2D1_POINT_2F top_left = { 
    pos_.x_ - texture_->GetSize().width / 2,
    pos_.y_ - texture_->GetSize().height / 2
  };

  D2D1_POINT_2F bottom_right = {
    pos_.x_ + texture_->GetSize().width / 2,
    pos_.y_ + texture_->GetSize().height / 2
  };

  r_render->GetRendererTarget()->DrawBitmap(
    texture_.get(),
    D2D1::RectF(top_left.x, top_left.y, bottom_right.x, bottom_right.y));
}