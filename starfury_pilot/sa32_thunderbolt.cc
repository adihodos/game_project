#include "precompiled.h"
#include "d2drenderer.h"
#include "game_engine.h"
#include "helpers.h"
#include "ispaceship.h"
#include "resource.h"
#include "sa32_plasmabolt.h"
#include "sa32_rocket.h"
#include "sa32_thunderbolt.h"

const wchar_t* const SA32_Thunderbolt::K_ResourceFileName = 
  L"sa_32.png";

const float SA32_Thunderbolt::K_VelocityY = -220.0f; // upwards 120 px

SA32_Thunderbolt::SA32_Thunderbolt(
  ID2D1RenderTarget* r_target,
  IWICImagingFactory* ifactory,
  const gfx::vector2D& pos,
  int hp,
  int lives
  )
  : ISpaceShip(),
    hp_(hp),
    texture_(),
    position_(pos),
    gun_direction_(gfx::vector2D::K_NullVector),
    rocket_left_(D2D1::Point2F(-40.0f, 0.0f)),
    rocket_right_(D2D1::Point2F(40.0f, 0.0f)),
    ship_geometry_(),
    rotate_angle_(0.0f),
    testbrush_(),
    leftrpod_(false)
{
  UNREFERENCED_PARAMETER(lives);
  std::wstring filepath(utility::GetApplicationResourceDirectory());
  filepath.append(SA32_Thunderbolt::K_ResourceFileName);

  ID2D1Bitmap* shipbmp = nullptr;
  HRESULT ret_code = utility::LoadBitmapFromFile(
    r_target, ifactory, filepath.c_str(), 0, 0, &shipbmp);

  assert(SUCCEEDED(ret_code));
  texture_.reset(shipbmp);
  
  gun_direction_.y_ = -texture_->GetSize().height / 2;

  ship_geometry_.width = texture_->GetSize().width;
  ship_geometry_.height = texture_->GetSize().height;

  ID2D1SolidColorBrush* brush = nullptr;
  r_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
  assert(brush);
  testbrush_.reset(brush);
}

void SA32_Thunderbolt::FirePlasmaGun(GameEngine* world) {
  world->ProjectileFired(
  new SA32_PlasmaBolt(
      world->GetRenderer()->GetRendererTarget(),
      world->GetRenderer()->GetImagingFactory(),
      position_ + gun_direction_, 
      gun_direction_ * 4.0f // velocity is gun_direction * 4
    ));
}

void SA32_Thunderbolt::FireRockets(
  GameEngine* world
  )
{
  //
  // Rotate velocity vector
  D2D1_POINT_2F vPt(D2D1::Point2F(0.0f, SA32_Thunderbolt::K_VelocityY));
  D2D1::Matrix3x2F mtx(D2D1::Matrix3x2F::Rotation(rotate_angle_));
  vPt = mtx.TransformPoint(vPt);
  gfx::vector2D velocity(vPt.x, vPt.y);

  gfx::vector2D rocket_origin(position_);
  rocket_origin += leftrpod_ ? gfx::vector2D(rocket_left_.x, rocket_left_.y)
    : gfx::vector2D(rocket_right_.x, rocket_right_.y);

  world->ProjectileFired(
    new SA32_Rocket(world->GetRenderer()->GetRendererTarget(),
                    world->GetRenderer()->GetImagingFactory(),
                    rocket_origin, velocity, rotate_angle_));
  leftrpod_ = !leftrpod_;
}

void SA32_Thunderbolt::Draw(
  Direct2DRenderer* r_render
  )
{
  //
  // rotate ship
  D2D1_MATRIX_3X2_F mtx_toorigin(D2D1::Matrix3x2F::Translation(
    -ship_geometry_.width / 2, -ship_geometry_.height / 2));
  D2D1_MATRIX_3X2_F mtx_rotation(D2D1::Matrix3x2F::Rotation(rotate_angle_));
  D2D1_MATRIX_3X2_F mtx_toworld(D2D1::Matrix3x2F::Translation(
    position_.x_, position_.y_));

  r_render->GetRendererTarget()->SetTransform(
    mtx_toorigin * mtx_rotation * mtx_toworld);

  D2D1_RECT_F dst = { 0, 0, ship_geometry_.width, ship_geometry_.height };

  //
  // Draw ship's bitmap
  r_render->GetRendererTarget()->DrawBitmap(texture_.get(), dst);
  //r_render->GetRendererTarget()->DrawRectangle(dst, testbrush_.get());
  //
  // Reset transformation matrix
  r_render->GetRendererTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
}

void SA32_Thunderbolt::Rotate(float amount) {
  if (std::fabs(rotate_angle_) >= 360.f) {
    rotate_angle_ = 0.0f;
    gun_direction_.x_ = 0.0f;
    gun_direction_.y_ = -texture_->GetSize().height / 2;
    return;
  }
  
  rotate_angle_ += amount;
  
  //
  // Update gun's direction
  D2D1::Matrix3x2F transform_mtx = D2D1::Matrix3x2F::Rotation(amount);

  D2D1_POINT_2F pt = { gun_direction_.x_, gun_direction_.y_ };
  pt = transform_mtx.TransformPoint(pt);
  gun_direction_.x_ = pt.x;
  gun_direction_.y_ = pt.y;

  rocket_left_ = transform_mtx.TransformPoint(rocket_left_);
  rocket_right_ = transform_mtx.TransformPoint(rocket_right_);
}

void SA32_Thunderbolt::MoveX(float direction /* = 0.0f */) {
  float move_amount = 4.0f * direction;
  float wingpos = position_.x_ + move_amount + (ship_geometry_.width / 2) * direction;
  POINT newpt = { static_cast<int>(wingpos), static_cast<int>(position_.y_) };
  RECT rc = { 0, 0, 1280, 1024 };

  if (PtInRect(&rc, newpt))
    position_.x_ += move_amount;
}