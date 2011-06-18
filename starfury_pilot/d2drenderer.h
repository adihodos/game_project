#pragma once

#include <d2d1.h>
#include <wincodec.h>
#include "basetypes.h"
#include "scoped_pointer.h"

class Direct2DRenderer {
public :
  Direct2DRenderer(HWND window, int width, int height);

  bool Initialize();

  void BeginRenderContent();

  void EndRenderingContent();

  ID2D1Factory* GetRendererFactory() const {
    return r_factory_.get();
  }

  ID2D1RenderTarget* GetRendererTarget() const {
    return r_target_.get();
  }

  IWICImagingFactory* GetImagingFactory() const {
    return r_imgfactory_.get();
  }

private :
  bool CreateDeviceIndependentResources();

  bool CreateDeviceDependentResources();

  void DiscardResources();

  HWND                                              r_window_;
  int                                               r_width_;
  int                                               r_height_;
  scoped_pointer<ID2D1Factory, D2DInterface>        r_factory_;
  scoped_pointer<ID2D1RenderTarget, D2DInterface>   r_target_;
  scoped_pointer<IWICImagingFactory, D2DInterface>  r_imgfactory_;
  NO_CPY_CONSTRUCTORS(Direct2DRenderer);
};