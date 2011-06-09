#include "precompiled.h"
#include "d2drenderer.h"
#include "helpers.h"
#include "meteor.h"
#include "misc.h"

const wchar_t* const SpaceMeteorite::K_BitmapFileName = 
  L"game_meteor.png";

SpaceMeteorite::SpaceMeteorite(
  ID2D1RenderTarget* render_t,
  IWICImagingFactory* factory,
  const gfx::vector2D& pos,
  float rotation
  )
  : IProjectile(),
    bounding_circle_(pos, 0.0f),
    pos_(pos),
    velocity_(gfx::vector2D::Null_Vector),
    bitmap_(),
    rotation_(rotation)
{
  ID2D1Bitmap* tmpbmp;
  HRESULT ret_code = utility::LoadBitmapFromFile(
    render_t,
    factory,
    (utility::GetApplicationResourceDirectory() + 
    SpaceMeteorite::K_BitmapFileName).c_str(),
    0, 0, &tmpbmp);

  assert(SUCCEEDED(ret_code));
  bitmap_.reset(tmpbmp);
  float w = bitmap_->GetSize().width;
  float h = bitmap_->GetSize().height;

  bounding_circle_.radius_ = w > h ? h / 2 : w / 2;

#if defined(_DEBUG)
  ID2D1SolidColorBrush* tmpbrush;
  render_t->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Cyan), &tmpbrush);
  dbgbrush_.reset(tmpbrush);
#endif

}

void SpaceMeteorite::UpdatePosition(float delta_tm) {
  assert(false);
}

void SpaceMeteorite::Draw(Direct2DRenderer* r_renderer) {
  r_renderer->GetRendererTarget()->SetTransform(
    D2D1::Matrix3x2F::Rotation(rotation_, D2D1::Point2F(pos_.x_, pos_.y_)));

  float w = bitmap_->GetSize().width;
  float h = bitmap_->GetSize().height;

  D2D1_RECT_F dst_rect = {
    pos_.x_ - w / 2,
    pos_.y_ - h / 2,
    pos_.x_ + w / 2,
    pos_.y_ + h / 2
  };

  r_renderer->GetRendererTarget()->DrawBitmap(bitmap_.get(), dst_rect);
  r_renderer->GetRendererTarget()->SetTransform(
    D2D1::Matrix3x2F::Identity());

#if defined(_DEBUG)

  r_renderer->GetRendererTarget()->FillEllipse(
    D2D1::Ellipse(D2D1::Point2F(pos_.x_, pos_.y_), 
                  bounding_circle_.radius_,
                  bounding_circle_.radius_),
                  dbgbrush_.get());

#endif
}