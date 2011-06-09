#include "precompiled.h"
#include "d2drenderer.h"
#include "game_engine.h"
#include "helpers.h"
#include "ispaceship.h"
#include "resource.h"
#include "sa32_rocket.h"
#include "sa32_thunderbolt.h"

const wchar_t* const SA32_Thunderbolt::K_ResourceFileName = 
  L"sa_32.png";

SA32_Thunderbolt::SA32_Thunderbolt(
  ID2D1RenderTarget* r_target,
  IWICImagingFactory* ifactory,
  const gfx::vector2D& pos,
  int hp,
  int lives
  )
  : ISpaceShip(),
    hp_(hp),
    lives_(lives),
    texture_(),
    position_(pos),
    gun_direction_(gfx::vector2D::Null_Vector),
    ship_centerpt_(),
    rotate_angle_(0.0f),
    testbrush_()
{
  std::wstring filepath(utility::GetApplicationResourceDirectory());
  filepath.append(SA32_Thunderbolt::K_ResourceFileName);

  ID2D1Bitmap* shipbmp = nullptr;
  HRESULT ret_code = utility::LoadBitmapFromFile(
    r_target, ifactory, filepath.c_str(), 0, 0, &shipbmp);

  assert(SUCCEEDED(ret_code));
  texture_.reset(shipbmp);

  position_.x_ -= texture_->GetSize().width / 2;
  position_.y_ -= texture_->GetSize().height / 2;
  
  gun_direction_.y_ = -texture_->GetSize().height / 2;
  ship_centerpt_.x_ = position_.x_ + texture_->GetSize().width / 2;
  ship_centerpt_.y_ = position_.y_ + texture_->GetSize().height / 2;

  ID2D1SolidColorBrush* brush = nullptr;
  r_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
  assert(brush);
  testbrush_.reset(brush);
}

void SA32_Thunderbolt::FireRockets(
  GameEngine* world
  )
{
  SA32_Rocket* rocket = new SA32_Rocket(
    world->GetRenderer()->GetRendererTarget(),
    world->GetRenderer()->GetImagingFactory(),
    ship_centerpt_ + gun_direction_,
    gun_direction_ * 1.2f,
    90.0f
    );

  world->ProjectileFired(rocket);
}

void SA32_Thunderbolt::Draw(
  Direct2DRenderer* r_render
  )
{
  //
  // rotate ship
  r_render->GetRendererTarget()->SetTransform(
    D2D1::Matrix3x2F::Rotation(
      rotate_angle_, 
      D2D1::Point2F(ship_centerpt_.x_, ship_centerpt_.y_)));

  D2D1_RECT_F dst = { 
    position_.x_, position_.y_, 
    position_.x_ + texture_->GetSize().width,
    position_.y_ + texture_->GetSize().height
  };

  //
  // Draw ship's bitmap
  r_render->GetRendererTarget()->DrawBitmap(texture_.get(), dst);  

  r_render->GetRendererTarget()->SetTransform(D2D1::Matrix3x2F::Identity());

  //
  // debug
  /*D2D1_POINT_2F origin = D2D1::Point2F(ship_centerpt_.x_, ship_centerpt_.y_);
  D2D1_POINT_2F dest = { 
    gun_direction_.x_ + ship_centerpt_.x_, 
    gun_direction_.y_ + ship_centerpt_.y_ 
  };

  r_render->GetRendererTarget()->DrawLine(
    origin,
    dest,
    testbrush_.get(),
    4.0f);  */
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
  D2D1::Matrix3x2F transform_mtx = D2D1::Matrix3x2F::Rotation(
    amount, D2D1::Point2F(0.0f, 0.0f));

  D2D1_POINT_2F pt = { gun_direction_.x_, gun_direction_.y_ };
  pt = transform_mtx.TransformPoint(pt);
  gun_direction_.x_ = pt.x;
  gun_direction_.y_ = pt.y;
} 