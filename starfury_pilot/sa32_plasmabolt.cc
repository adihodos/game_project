#include "precompiled.h"
#include "d2drenderer.h"
#include "helpers.h"
#include "sa32_plasmabolt.h"

const wchar_t* const SA32_PlasmaBolt::K_ResourceFileName =
  L"sa32_plasmabolt.png";

const float SA32_PlasmaBolt::K_PlasmaBoltRadius = 15.0f;

SA32_PlasmaBolt::SA32_PlasmaBolt(
  ID2D1RenderTarget* r_target, 
  IWICImagingFactory* factory, 
  const gfx::vector2D& pos, 
  const gfx::vector2D& velocity
  )
  : IProjectile(),
    pos_(pos),
    velocity_(velocity),
    bounding_circle_(pos_, SA32_PlasmaBolt::K_PlasmaBoltRadius),
    hp_(SA32_PlasmaBolt::K_HitPoints),
    damage_(SA32_PlasmaBolt::K_Damage),
    texture_(),
    plasmabrush_(),
    dbgbrush_()
{
  std::wstring filepath(utility::GetApplicationResourceDirectory());
  filepath.append(SA32_PlasmaBolt::K_ResourceFileName);

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

  ID2D1SolidColorBrush* tmp2;
  ret_code = r_target->CreateSolidColorBrush(
    D2D1::ColorF(D2D1::ColorF::Black), &tmp2);
  assert(SUCCEEDED(ret_code));
  dbgbrush_.reset(tmp2);
}

void SA32_PlasmaBolt::UpdatePosition(float delta_time) {
  pos_ += velocity_ * delta_time;
  bounding_circle_.center_ = pos_;
}

void SA32_PlasmaBolt::Draw(
  Direct2DRenderer* r_render
  )
{
  D2D1_ELLIPSE bolt(D2D1::Ellipse(D2D1::Point2F(pos_.x_, pos_.y_), 
                               texture_->GetSize().width / 2,
                               texture_->GetSize().width / 2));

  //
  // Align the brush with the bolt
  plasmabrush_->SetTransform(D2D1::Matrix3x2F::Translation(
    pos_.x_ - bolt.radiusX, pos_.y_ - bolt.radiusY));

  r_render->GetRendererTarget()->FillEllipse(bolt, plasmabrush_.get());
}