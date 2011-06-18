#include "precompiled.h"
#include "helpers.h"
#include "scoped_pointer.h"

HRESULT
utility::LoadBitmapFromFile(
  ID2D1RenderTarget *pRenderTarget,	// Render target
  IWICImagingFactory *pIWICFactory,	// WIC factory
  PCWSTR uri,							// File URI (path to file)
  UINT destinationWidth,  // unused
  UINT destinationHeight, // unused
  ID2D1Bitmap **ppBitmap
  )
{
  assert(pRenderTarget);
  assert(pIWICFactory);
  assert(uri);
  assert(ppBitmap);

  IWICBitmapDecoder* tmp_decoder = nullptr;
  
  HRESULT res = pIWICFactory->CreateDecoderFromFilename(
    uri,
    nullptr,
    GENERIC_READ,
    WICDecodeMetadataCacheOnLoad,
    &tmp_decoder);

  if (FAILED(res))
    return res;

  scoped_pointer<IWICBitmapDecoder, D2DInterface> decoder(tmp_decoder);
  IWICBitmapFrameDecode* bmp_source = nullptr;
  res = decoder->GetFrame(0, &bmp_source);
  
  if (FAILED(res))
    return res;

  //
  // convert to 32bppPBGRA
  IWICFormatConverter* tmp_converter = nullptr;
  res = pIWICFactory->CreateFormatConverter(&tmp_converter);
  if (FAILED(res))
    return res;

  scoped_pointer<IWICFormatConverter, D2DInterface> converter(tmp_converter);
  res = converter->Initialize(
    bmp_source, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
    nullptr,
    0.0f,
    WICBitmapPaletteTypeMedianCut
    );

  if (FAILED(res))
    return res;

  return pRenderTarget->CreateBitmapFromWicBitmap(
    converter.get(),
    nullptr,
    ppBitmap);
}

HRESULT
utility::LoadResourceBitmap(
  ID2D1RenderTarget *pRenderTarget,	// Render target
  IWICImagingFactory *pIWICFactory,	// WIC factory
  PCWSTR resourceName,				// Name of resource
  PCWSTR resourceType,				// Type of resource
  UINT destinationWidth,
  UINT destinationHeight,
  ID2D1Bitmap **ppBitmap
  )
{
  assert(pRenderTarget);
  assert(pIWICFactory);
  assert(resourceName);
  assert(resourceType);
  assert(ppBitmap);

  HRSRC img_handle = ::FindResourceW(nullptr, resourceName, resourceType);
  if (!img_handle)
    return E_FAIL;

  HGLOBAL img_data_handle = ::LoadResource(nullptr, img_handle);
  if (!img_data_handle)
    return E_FAIL;

  void* imgfile = ::LockResource(img_data_handle);
  if (!imgfile)
    return E_FAIL;

  DWORD imgfilesize = ::SizeofResource(nullptr, img_handle);
  if (!imgfilesize)
    return E_FAIL;

  IWICStream* tmpptr = nullptr;
  HRESULT res = pIWICFactory->CreateStream(&tmpptr);
  if (FAILED(res))
    return res;

  scoped_pointer<IWICStream, D2DInterface> stream(tmpptr);

  res = stream->InitializeFromMemory(
    static_cast<BYTE*>(imgfile),
    imgfilesize);

  if (FAILED(res))
    return res;

  LARGE_INTEGER seekpt = { 0 };
  res = stream->Seek(seekpt, STREAM_SEEK_SET, 0);
  if (FAILED(res))
    return res;

  IWICBitmapDecoder* tmp_decoder = nullptr;
  res = pIWICFactory->CreateDecoderFromStream(
    stream.get(),
    nullptr,
    WICDecodeMetadataCacheOnLoad,
    &tmp_decoder
    );

  if (FAILED(res))
    return res;

  scoped_pointer<IWICBitmapDecoder, D2DInterface> decoder(tmp_decoder);
  IWICBitmapFrameDecode* bmp_source = nullptr;
  res = decoder->GetFrame(0, &bmp_source);
  
  if (FAILED(res))
    return res;

  //
  // convert to 32bppPBGRA
  IWICFormatConverter* tmp_converter = nullptr;
  res = pIWICFactory->CreateFormatConverter(&tmp_converter);
  if (FAILED(res))
    return res;

  scoped_pointer<IWICFormatConverter, D2DInterface> converter(tmp_converter);
  res = converter->Initialize(
    bmp_source, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
    nullptr,
    0.0f,
    WICBitmapPaletteTypeMedianCut
    );

  if (FAILED(res))
    return res;

  return pRenderTarget->CreateBitmapFromWicBitmap(
    converter.get(),
    nullptr,
    ppBitmap);
}

void
utility::OutputMessageToDebugger(
  const wchar_t* msg,
  ...
  )
{
  va_list args_p;
  va_start(args_p, msg);

  wchar_t msgbuff[2048];
  _vsnwprintf_s(msgbuff, _countof(msgbuff), _countof(msgbuff) - 1, msg, args_p);
  va_end(args_p);
  ::OutputDebugStringW(msgbuff);
}

HRESULT
utility::CreateRadialGradientBrush(
  ID2D1RenderTarget* r_target,
  const D2D1_GRADIENT_STOP* gradient_stops,
  int len,
  const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES& grad_props,
  ID2D1RadialGradientBrush** brush
  )
{
  ID2D1GradientStopCollection* tmpgradstops = nullptr;

  HRESULT ret_val = r_target->CreateGradientStopCollection(
    gradient_stops, len, 
    D2D1_GAMMA_2_2,
    D2D1_EXTEND_MODE_CLAMP,
    &tmpgradstops
    );

  if (FAILED(ret_val))
    return ret_val;

  scoped_pointer<ID2D1GradientStopCollection, 
                 D2DInterface> gradstopcollection(tmpgradstops);

  return r_target->CreateRadialGradientBrush(
    grad_props,
    gradstopcollection.get(),
    brush
    );
}

bool
utility::GetApplicationBaseDirectory(
  std::wstring* dst_dir
  )
{
  assert(dst_dir);
  dst_dir->clear();

  size_t capacity = 512;
  wchar_t namebuff[512];

  DWORD buff_size = ::GetModuleFileNameW(nullptr, namebuff, _countof(namebuff));
  if (!buff_size)
    return false;

  wchar_t* itr = namebuff + buff_size;

  while (itr > namebuff && *itr != L'\\') {
    *itr = 0;
    --itr;
  }

  dst_dir->assign(namebuff);
  return true;
}

std::wstring
utility::GetApplicationResourceDirectory() {
  std::wstring dirpath;
  if (GetApplicationBaseDirectory(&dirpath))
    dirpath.append(L"\\");

  dirpath.append(L"resources\\");
  return dirpath;
}

bool utility::GetScreenSize(SIZE* dim) {
  assert(dim != nullptr);

  DEVMODEW dinfo;
  if (!EnumDisplaySettingsW(nullptr, ENUM_CURRENT_SETTINGS, &dinfo))
    return false;

  dim->cx = dinfo.dmPelsWidth;
  dim->cy = dinfo.dmPelsHeight;
  return true;
}

void 
utility::shrink_rect( 
  float amount, 
  D2D1_RECT_F* rrect 
  )
{
  rrect->left += amount / 2.0f;
  rrect->top += amount / 2.0f;
  rrect->right -= amount / 2.0f;
  rrect->bottom -= amount / 2.0f;
}

void
utility::shrink_rounded_rect( 
  float amount, 
  D2D1_ROUNDED_RECT* rrect
  )
{
  rrect->rect.left += amount / 2.0f;
  rrect->rect.top += amount / 2.0f;
  rrect->rect.right -= amount / 2.0f;
  rrect->rect.bottom -= amount / 2.0f;
}