#include "precompiled.h"
#include "helpers.h"
#include "renderer.h"

game_logic::Direct2DRenderer::Direct2DRenderer() {}

game_logic::Direct2DRenderer::~Direct2DRenderer() {}

bool
game_logic::Direct2DRenderer::Initialize(
  HWND window, 
  const gfx::vector2D& wnd_geometry
  )
{
  assert(!r_window_);
  assert(!r_hwndtarget_);
  assert(!r_d2factory_);

  r_window_ = window;
  r_windowgeometry_ = wnd_geometry;

  if (!Create_DeviceIndependentResources())
    return false;

  if (!Create_DeviceDependentResources())
    return false;

  return true;
}

bool
game_logic::Direct2DRenderer::Create_DeviceIndependentResources() {
  HRESULT ret_code = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, 
                                         r_d2factory_.get_owned_ptr());

  if (FAILED(ret_code)) {
    FUNC_FAILED_HRESULT(D2D1CreateFactory, ret_code);
  }

  return ret_code == S_OK;
}

bool
game_logic::Direct2DRenderer::Create_DeviceDependentResources() {
  if (!!r_hwndtarget_)
    return true;

  D2D1_SIZE_U geometry(D2D1::SizeU(r_windowgeometry_.x_, r_windowgeometry_.y_));
  HRESULT ret_code = r_d2factory_->CreateHwndRenderTarget(
    D2D1::RenderTargetProperties(),
    D2D1::HwndRenderTargetProperties(r_window_, geometry),
    r_hwndtarget_.get_owned_ptr()
    );

  if(FAILED(ret_code)) {
    FUNC_FAILED_HRESULT(ID2D1Factory::CreateHwndRenderTarget, ret_code);
  }
  return ret_code == S_OK;
}

void 
game_logic::Direct2DRenderer::Begin_Rendering() {
  if (!Create_DeviceDependentResources())
    return;

  r_hwndtarget_->BeginDraw();
  r_hwndtarget_->SetTransform(D2D1::Matrix3x2F::Identity());
  r_hwndtarget_->Clear(D2D1::ColorF(D2D1::ColorF::White));
}

void
game_logic::Direct2DRenderer::End_Rendering() {
  if (r_hwndtarget_->EndDraw() == D2DERR_RECREATE_TARGET)
    Discard_Resources();
}

void 
game_logic::Direct2DRenderer::Draw_Rectangle( 
  float left, 
  float top, 
  float right, 
  float bottom, 
  ID2D1Brush* brush, 
  float width 
  )
{
  assert(r_hwndtarget_);
  D2D1_RECT_F draw_rect(D2D1::RectF(left, top, right, bottom));
  r_hwndtarget_->DrawRectangle(draw_rect, brush, width);
}

void
game_logic::Direct2DRenderer::Fill_Rectangle( 
  float left, 
  float top, 
  float right, 
  float bottom, 
  ID2D1Brush* brush 
  )
{
  assert(r_hwndtarget_);
  D2D1_RECT_F fill_rect(D2D1::RectF(left, top, right, bottom));
  r_hwndtarget_->FillRectangle(fill_rect, brush);
}