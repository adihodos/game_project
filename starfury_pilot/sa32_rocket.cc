#include "precompiled.h"
#include "d2drenderer.h"
#include "helpers.h"
#include "sa32_rocket.h"

const wchar_t* const SA32_Rocket::K_ResourceFileName =
    L"sa32_rocket.png";

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

  centerpt_.x = texture_->GetSize().width / 2;
  centerpt_.y = texture_->GetSize().height / 2;
  
  geometry_.height = texture_->GetSize().height;
  geometry_.width = texture_->GetSize().width;
}

void SA32_Rocket::UpdatePosition(float delta_time) {
  pos_ += (velocity_ * delta_time);
  bounding_circle_.center_ = pos_;
}

void SA32_Rocket::Draw(
  Direct2DRenderer* r_render
  )
{
  D2D1::Matrix3x2F mtx_rot(D2D1::Matrix3x2F::Rotation(angle_, centerpt_));
  D2D1::Matrix3x2F mtx_trans(D2D1::Matrix3x2F::Translation(
    pos_.x_ - centerpt_.x, pos_.y_ - centerpt_.y));

  r_render->GetRendererTarget()->SetTransform(mtx_rot * mtx_trans);

  r_render->GetRendererTarget()->DrawBitmap(texture_.get(),
    D2D1::RectF(0.0f, 0.0f, geometry_.width, geometry_.height));

  r_render->GetRendererTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
}