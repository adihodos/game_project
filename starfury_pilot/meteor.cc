#include "precompiled.h"
#include "basetypes.h"
#include "d2drenderer.h"
#include "helpers.h"
#include "meteor.h"

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
    velocity_(gfx::vector2D::K_NullVector),
    bitmap_(),
    rotation_(rotation),
    geometry_()
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
  geometry_.width = bitmap_->GetSize().width;
  geometry_.height = bitmap_->GetSize().height;

  bounding_circle_.radius_ = geometry_.height > geometry_.width ? 
    geometry_.height / 2 : geometry_.width / 2;

#if defined(_DEBUG)
  ID2D1SolidColorBrush* tmpbrush;
  render_t->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Cyan), &tmpbrush);
  dbgbrush_.reset(tmpbrush);
#endif

}

void SpaceMeteorite::UpdatePosition(float delta_tm) {
  UNREFERENCED_PARAMETER(delta_tm);
  //
  // meteorites don't move for now
  assert(false);
}

void SpaceMeteorite::Draw(Direct2DRenderer* r_renderer) {
  const float K_MeteorAngularVelocity = 2.0f;

  if (std::fabs(rotation_) >= 360.0f) {
    rotation_ = 0.0f;
  } else {
    rotation_ += K_MeteorAngularVelocity;
  }

  D2D1_MATRIX_3X2_F mtx_movetoorigin = D2D1::Matrix3x2F::Translation(
    -geometry_.width / 2, -geometry_.height / 2);
  D2D1_MATRIX_3X2_F mtx_rotate = D2D1::Matrix3x2F::Rotation(rotation_);
  D2D1_MATRIX_3X2_F mtx_transtoworld = D2D1::Matrix3x2F::Translation(
    pos_.x_, pos_.y_);

  r_renderer->GetRendererTarget()->SetTransform(mtx_movetoorigin * mtx_rotate *
    mtx_transtoworld);

  D2D1_RECT_F dst_rect = { 0.0f, 0.0f, geometry_.width, geometry_.height };
  
  r_renderer->GetRendererTarget()->DrawBitmap(bitmap_.get(), dst_rect);
  r_renderer->GetRendererTarget()->SetTransform(
    D2D1::Matrix3x2F::Identity());
}