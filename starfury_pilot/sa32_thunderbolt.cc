#include "precompiled.h"
#include "engine.h"
#include "game_resourcecache.h"
#include "helpers.h"
#include "lazy_unique_instance.h"
#include "sa32_thunderbolt.h"

//const float game_entity::SA32_Thunderbolt::K_VelocityY = -220.0f; // upwards 120 px

namespace {

void 
get_texture_geometry(
  ID2D1Bitmap* bmp, 
  gfx::vector2* vec
  ) {
  assert(bmp);
  assert(vec);
  D2D1_SIZE_F bmp_size(bmp->GetSize());
  vec->x_ = bmp_size.width;
  vec->y_ = bmp_size.height;
}

}

game_core::sa32_thunderbolt::sa32_thunderbolt(
  int hp
  )
  : hp_(hp),
    position_(gfx::vector2::null),
    gun_direction_(),
    geometry_(),
    rotate_angle_(0.0f),
    ship_texture_(nullptr)
{
}

//gfx::vector2 
//game_core::sa32_thunderbolt::compute_world_origin() {
//  gfx::vector2D world_origin(game_core::Game_Renderer::Get()->GetGeometry());
//  world_origin.y_ -= ship_rect_[1].y_ / 2;
//  world_origin.x_ /= 2;
//  return world_origin;
//}

bool
game_core::sa32_thunderbolt::initialize() {
  assert(!ship_texture_);
  const wchar_t* const c_texture_file = L"sa_32.png";
  ship_texture_ = resource_cache_handle::Get()->get_bitmap(c_texture_file);
  if (!ship_texture_) {
    SPEW_DEBUG_MSG(L"Failed to load texture %s", c_texture_file);
    return false;
  }

  get_texture_geometry(ship_texture_, &geometry_);
  gun_direction_.x_ = 0.0f;
  gun_direction_.y_ = 0.0f;
  return true;
}

void 
game_core::sa32_thunderbolt::draw() {
  //
  // Compute ship's position in world coordinates  
  gfx::vector2 world_pos(
    engine_handle::Get()->get_world_transform_matrix() 
    * gfx::matrix3X3::translation(0.0f, engine_handle::Get()->get_world_geometry().y_ / 2 - geometry_.y_ / 2)
    * position_
    );

  gfx::rectangle texworld(world_pos - geometry_ / 2, world_pos + geometry_ / 2);

  engine_handle::Get()->get_rendertarget()->SetTransform(
    D2D1::Matrix3x2F::Rotation(rotate_angle_, texworld.get_centre_point()));
  engine_handle::Get()->get_rendertarget()->DrawBitmap(ship_texture_, texworld);
}

void game_core::sa32_thunderbolt::rotate(float amount) {
  const float c_max_rotation_angle = 45.0f;
  if (std::fabs(rotate_angle_ + amount) >= c_max_rotation_angle) {
    return;
  }
  rotate_angle_ += amount;  
}

void game_core::sa32_thunderbolt::move(int direction)
{
  const float kHorizontalVelocity = 8.0f;
  gfx::vector2 resulting_pos(position_);
  resulting_pos.x_ += (kHorizontalVelocity + geometry_.x_ / 2) * direction;
  resulting_pos = engine_handle::Get()->get_world_transform_matrix() * resulting_pos;

  if (gfx::point_in_rectangle(
    resulting_pos, engine_handle::Get()->get_world_clip_rectangle())) {
      position_.x_ += kHorizontalVelocity * direction;
  }
}