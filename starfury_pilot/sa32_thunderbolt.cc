#include "precompiled.h"
#include "d2drenderer.h"
#include "game_engine.h"
#include "game_resourcecache.h"
#include "helpers.h"
#include "ispaceship.h"
#include "lazy_unique_instance.h"
#include "sa32_plasmabolt.h"
#include "sa32_thunderbolt.h"

const wchar_t* const game_entity::SA32_Thunderbolt::K_ResourceFileName = 
  L"sa_32.png";

const float game_entity::SA32_Thunderbolt::K_VelocityY = -220.0f; // upwards 120 px

game_entity::SA32_Thunderbolt::SA32_Thunderbolt(
  const gfx::vector2D& pos,
  int hp,
  int lives
  )
  : ISpaceShip(),
    hp_(hp),
    position_(pos),
    gun_direction_(gfx::vector2D::K_NullVector),
    rocket_left_(D2D1::Point2F(-40.0f, 0.0f)),
    rocket_right_(D2D1::Point2F(40.0f, 0.0f)),
    ship_geometry_(),
    rotate_angle_(0.0f),
    leftrpod_(false)
{
  UNREFERENCED_PARAMETER(lives);
  base::LazyUniqueInstance<GameResourceCache>::Get()->get_texture_size(
    SA32_Thunderbolt::K_ResourceFileName, &ship_geometry_);
  
  gun_direction_.y_ = -ship_geometry_.y_ / 2;
}

void game_entity::SA32_Thunderbolt::FirePlasmaGun(GameEngine* world) {
  //world->ProjectileFired(
  //  new SA32_PlasmaBolt(position_ + gun_direction_, 
  //                      gun_direction_ * 4.0f));
}

void game_entity::SA32_Thunderbolt::FireRockets(
  GameEngine* world
  )
{
  ////
  //// Rotate velocity vector
  //D2D1_POINT_2F vPt(D2D1::Point2F(0.0f, SA32_Thunderbolt::K_VelocityY));
  //D2D1::Matrix3x2F mtx(D2D1::Matrix3x2F::Rotation(rotate_angle_));
  //vPt = mtx.TransformPoint(vPt);
  //gfx::vector2D velocity(vPt.x, vPt.y);

  //gfx::vector2D rocket_origin(position_);
  //rocket_origin += leftrpod_ ? gfx::vector2D(rocket_left_.x, rocket_left_.y)
  //  : gfx::vector2D(rocket_right_.x, rocket_right_.y);

  //world->ProjectileFired(
  //  new SA32_Rocket(world->GetRenderer()->GetRendererTarget(),
  //                  world->GetRenderer()->GetImagingFactory(),
  //                  rocket_origin, velocity, rotate_angle_));
  //leftrpod_ = !leftrpod_;
}

void game_entity::SA32_Thunderbolt::Draw(
  Direct2DRenderer* r_render
  )
{
  
  //
  // rotate ship around it's centerpoint
  gfx::matrix3X3 t_rotate(gfx::matrix3X3::rotation(
    utility::DegreesToRadians(rotate_angle_), ship_geometry_.x_ / 2, ship_geometry_.y_ / 2));
  gfx::matrix3X3 t_toworld(gfx::matrix3X3::translation(position_));
  r_render->GetRendererTarget()->SetTransform(t_toworld * t_rotate);

  D2D1_RECT_F dst = { 0, 0, ship_geometry_.x_, ship_geometry_.y_ };

  //
  // Draw ship's bitmap
  //ID2D1Bitmap* ship_texture(base::LazyUniqueInstance<GameResourceCache>::Get()->GetBitmapHandle(SA32_Thunderbolt::K_ResourceFileName);
  //assert(ship_texture);
  //r_render->GetRendererTarget()->DrawBitmap(ship_texture, dst);
  //r_render->GetRendererTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
}

void game_entity::SA32_Thunderbolt::Rotate(float amount) {
  UNREFERENCED_PARAMETER(amount);
  //if (std::fabs(rotate_angle_) >= 360.f) {
  //  rotate_angle_ = 0.0f;
  //  gun_direction_.x_ = 0.0f;
  //  gun_direction_.y_ = -texture_->GetSize().height / 2;
  //  return;
  //}
  //
  //rotate_angle_ += amount;
  //
  ////
  //// Update gun's direction
  //D2D1::Matrix3x2F transform_mtx = D2D1::Matrix3x2F::Rotation(amount);

  //D2D1_POINT_2F pt = { gun_direction_.x_, gun_direction_.y_ };
  //pt = transform_mtx.TransformPoint(pt);
  //gun_direction_.x_ = pt.x;
  //gun_direction_.y_ = pt.y;

  //rocket_left_ = transform_mtx.TransformPoint(rocket_left_);
  //rocket_right_ = transform_mtx.TransformPoint(rocket_right_);
}

void game_entity::SA32_Thunderbolt::MoveX(float direction /* = 0.0f */) {
  UNREFERENCED_PARAMETER(direction);
  /*float move_amount = 4.0f * direction;
  float wingpos = position_.x_ + move_amount + (ship_geometry_.width / 2) * direction;
  POINT newpt = { static_cast<int>(wingpos), static_cast<int>(position_.y_) };
  RECT rc = { 0, 0, 1280, 1024 };

  if (PtInRect(&rc, newpt))
    position_.x_ += move_amount;*/
}