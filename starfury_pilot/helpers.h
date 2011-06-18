#pragma once

#include <cstdarg>
#include <d2d1.h>
#include <Windows.h>
#include <wincodec.h>

namespace utility {

const float K_PI = 3.14159265f;

inline float DegreesToRadians(float degrees) {
  return (degrees * K_PI) / 180.0f;
}

HRESULT LoadBitmapFromFile(
  ID2D1RenderTarget *pRenderTarget,	// Render target
  IWICImagingFactory *pIWICFactory,	// WIC factory
  PCWSTR uri,							// File URI (path to file)
  UINT destinationWidth,  // unused
  UINT destinationHeight, // unused
  ID2D1Bitmap **ppBitmap				// The Bitmap created
  );

// Loads a bitmap from the application's resource file
HRESULT LoadResourceBitmap(
  ID2D1RenderTarget *pRenderTarget,	// Render target
  IWICImagingFactory *pIWICFactory,	// WIC factory
  PCWSTR resourceName,				// Name of resource
  PCWSTR resourceType,				// Type of resource
  UINT destinationWidth,
  UINT destinationHeight,
  ID2D1Bitmap **pBitmap				// The bitmap created
  );

void
OutputMessageToDebugger(
  const wchar_t* msg,
  ...
  );

HRESULT 
CreateRadialGradientBrush(
  ID2D1RenderTarget* r_target,
  const D2D1_GRADIENT_STOP* gradient_stops,
  int len,
  const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES& grad_props,
  ID2D1RadialGradientBrush** brush
  );

bool 
GetApplicationBaseDirectory(
  std::wstring* dst
  );

std::wstring
GetApplicationResourceDirectory();

bool
GetScreenSize(
  SIZE* dim
  );

inline 
bool point_in_rect(
  float ptx, 
  float pty, 
  const D2D1_RECT_F& rc
  ) 
{
  return ptx >= rc.left && ptx <= rc.right && pty >= rc.top && pty <= rc.bottom;
}

void 
shrink_rounded_rect(
  float amount, 
  D2D1_ROUNDED_RECT* 
  rrect
  );

void
shrink_rect(
  float amount, 
  D2D1_RECT_F* rrect
  );

template<typename T>
struct COMObject_Deleter {
  void operator()(T* ptr) {
    if (ptr)
      ptr->Release();
  }
};

}

#if defined(_DEBUG)
#ifndef OUT_DBG_MSG
#define OUT_DBG_MSG(fmtspec, ...)   \
  utility::OutputMessageToDebugger(fmtspec L"\n", __VA_ARGS__)
#endif
#endif