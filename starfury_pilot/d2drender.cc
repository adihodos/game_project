#include "precompiled.h"
#include "d2drenderer.h"
#include "helpers.h"
#include "resource.h"

Direct2DRenderer::Direct2DRenderer(
  HWND window,
  int width,
  int height
  )
  : r_window_(window),
    r_width_(width),
    r_height_(height),
    r_factory_(),
    r_target_()
    {}

bool Direct2DRenderer::Initialize() {
  if (!CreateDeviceIndependentResources())
    return false;

  if (!CreateDeviceDependentResources())
    return false;

  return true;
}

bool Direct2DRenderer::CreateDeviceIndependentResources() {
  ID2D1Factory* pfactory = nullptr;
  HRESULT hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pfactory);
  if (!SUCCEEDED(hr))
    return false;

  r_factory_.reset(pfactory);

  /*IWICImagingFactory* factory = nullptr;
  hr = ::CoCreateInstance(
  CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
  IID_IWICImagingFactory, reinterpret_cast<void**>(&factory));
  if (!SUCCEEDED(hr))
  return false;

  r_imgfactory_.reset(factory);*/
  return true;
}

bool Direct2DRenderer::CreateDeviceDependentResources() {
  if (!!r_target_)
    return true;

  ID2D1HwndRenderTarget* render_target = nullptr;

  D2D1_SIZE_U size = { r_width_, r_height_ };

  HRESULT res = r_factory_->CreateHwndRenderTarget(
    D2D1::RenderTargetProperties(),
    D2D1::HwndRenderTargetProperties(r_window_, size), 
    &render_target);

  if (!SUCCEEDED(res))
    return false;

  r_target_.reset(render_target);
  return true;
}

void Direct2DRenderer::DiscardResources() {
  r_target_.reset();
}

void Direct2DRenderer::BeginRenderContent() {
  if (!CreateDeviceDependentResources())
    return;

  r_target_->BeginDraw();
  r_target_->SetTransform(D2D1::Matrix3x2F::Identity());
  r_target_->Clear(D2D1::ColorF(D2D1::ColorF::White));
}

void Direct2DRenderer::EndRenderingContent() {
  HRESULT ret_code = r_target_->EndDraw();

  if (ret_code == D2DERR_RECREATE_TARGET)
    DiscardResources();
}