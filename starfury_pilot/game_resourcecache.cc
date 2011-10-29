#include "precompiled.h"
#include "game_resourcecache.h"
#include "helpers.h"

game_core::resource_cache::resource_cache()
  : rtarget_(), 
    imgfactory_(),
    writefactory_(),
    bitmap_cache_(), 
    solidcolorbrush_cache_(), 
    bitmapbrush_cache_(),
    font_cache_() {}

game_core::resource_cache::~resource_cache() {
  discard_cached_resourcess();
}

bool 
game_core::resource_cache::initialize(
  ID2D1HwndRenderTarget* rtarget
  )  
{
  assert(rtarget_ == nullptr);
  rtarget_ = rtarget;

  assert(!imgfactory_);

  IWICImagingFactory* fptr = nullptr;

  if (FAILED(::CoCreateInstance(CLSID_WICImagingFactory, 
                                nullptr, CLSCTX_INPROC_SERVER, 
                                IID_IWICImagingFactory,
                                reinterpret_cast<void**>(&fptr)))) {
    return false;
  }

  imgfactory_.reset(fptr);

  HRESULT ret_code = DWriteCreateFactory(
    DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), 
    reinterpret_cast<IUnknown**>(writefactory_.get_owned_ptr()));

  return SUCCEEDED(ret_code);
}

ID2D1Bitmap* 
game_core::resource_cache::get_bitmap(
  const wchar_t* rsrcname
  ) 
{
  auto itdata = bitmap_cache_.find(rsrcname);
  if (itdata != bitmap_cache_.end())
    return itdata->second;

  std::wstring fpath(utility::GetApplicationResourceDirectory());
  fpath.append(L"\\");
  fpath.append(rsrcname);

  ID2D1Bitmap* bitmap;
  HRESULT ret_code = utility::LoadBitmapFromFile(
    rtarget_, imgfactory_.get(), fpath.c_str(), 0, 0, &bitmap);

  if (FAILED(ret_code))
    return nullptr;

  bitmap_cache_.insert(bitmap_cache_t::value_type(rsrcname, bitmap));
  return bitmap;
}

ID2D1SolidColorBrush*
game_core::resource_cache::get_solid_color_brush(
  const D2D1::ColorF& color
  )
{
  auto itdata = solidcolorbrush_cache_.find(color);
  if (itdata != solidcolorbrush_cache_.end())
    return itdata->second;

  ID2D1SolidColorBrush* brush;
  HRESULT ret_code = rtarget_->CreateSolidColorBrush(color, &brush);
  if (FAILED(ret_code))
    return nullptr;

  solidcolorbrush_cache_.insert(solidcolor_brush_cache_t::value_type(color, brush));
  return brush;
}

ID2D1BitmapBrush* 
game_core::resource_cache::get_bitmap_brush(
  const wchar_t* bmpname
  ) 
{
  assert(bmpname);

  auto itdata = bitmapbrush_cache_.find(bmpname);
  if (itdata != bitmapbrush_cache_.end())
    return itdata->second;

  std::wstring brushpath(utility::GetApplicationResourceDirectory());
  brushpath.append(L"\\");
  brushpath.append(bmpname);

  scoped_pointer<ID2D1Bitmap, D2DInterface> bmpptr;
  HRESULT ret_code = utility::LoadBitmapFromFile(
    rtarget_, imgfactory_.get(), brushpath.c_str(), 
    0, 0, bmpptr.get_owned_ptr());

  if (FAILED(ret_code))
    return nullptr;

  ID2D1BitmapBrush* newbrush;
  ret_code = rtarget_->CreateBitmapBrush(bmpptr.get(), &newbrush);

  if (FAILED(ret_code))
    return nullptr;

  bitmapbrush_cache_.insert(bitmap_brush_cache_t::value_type(bmpname, newbrush));
  return newbrush;
}

IDWriteTextFormat*
game_core::resource_cache::get_font(
  const game_core::game_font& fdata
  ) 
{
  using game_core::game_font;

  auto fnt_itr = font_cache_.find(fdata);
  if (fnt_itr != font_cache_.end())
    return fnt_itr->second.get();

  DWRITE_FONT_WEIGHT fweight;
  switch (fdata.get_font_weight()) {
  case game_font::GFD_FW_HEAVY :
    fweight = DWRITE_FONT_WEIGHT_HEAVY;
    break;

  case game_font::GFD_FW_MEDIUM :
    fweight = DWRITE_FONT_WEIGHT_MEDIUM;
    break;

  case game_font::GFD_FW_LIGHT :
    fweight = DWRITE_FONT_WEIGHT_LIGHT;
    break;

  default :
    fweight = DWRITE_FONT_WEIGHT_NORMAL;
    break;
  }

  DWRITE_FONT_STYLE fstyle;
  switch (fdata.get_font_style()) {
  case game_font::GFD_FS_SITALIC :
    fstyle = DWRITE_FONT_STYLE_ITALIC;
    break;

  case game_font::GFD_FS_OBLIQUE :
    fstyle = DWRITE_FONT_STYLE_OBLIQUE;
    break;

  default :
    fstyle = DWRITE_FONT_STYLE_NORMAL;
    break;
  }

  IDWriteTextFormat* textfmt;
  HRESULT ret_code = writefactory_->CreateTextFormat(
    fdata.get_font_name().c_str(), 
    nullptr, // use default font collection
    fweight,
    fstyle,
    DWRITE_FONT_STRETCH_NORMAL,
    fdata.get_font_size(),
    L"en-us",
    &textfmt);

  if (FAILED(ret_code))
    return nullptr;

  font_cache_.insert(font_cache_t::value_type(
    fdata, 
    std::shared_ptr<IDWriteTextFormat>(
      textfmt, utility::COMObject_Deleter<IDWriteTextFormat>())));
  return textfmt;
}

void 
game_core::resource_cache::discard_cached_resourcess() {
  std::for_each(bitmap_cache_.begin(), bitmap_cache_.end(),
    [](bitmap_cache_t::value_type& valtype_itr) {
      if (valtype_itr.second)
        valtype_itr.second->Release();
  });

  std::for_each(solidcolorbrush_cache_.begin(), solidcolorbrush_cache_.end(),
    [](solidcolor_brush_cache_t::value_type& valtype_itr) {
      if (valtype_itr.second)
        valtype_itr.second->Release();
  });

  std::for_each(bitmapbrush_cache_.begin(), bitmapbrush_cache_.end(),
    [](bitmap_brush_cache_t::value_type& valtype_itr) {
      if (valtype_itr.second)
        valtype_itr.second->Release();
  });

  bitmap_cache_.clear();
  bitmapbrush_cache_.clear();
  solidcolorbrush_cache_.clear();
}

bool 
game_core::resource_cache::get_texture_size( 
  const wchar_t* resource_filename, 
  gfx::vector2* vec_size 
  )
{
  assert(resource_filename);
  assert(vec_size);

  ID2D1Bitmap* bitmap(get_bitmap(resource_filename));
  if (!bitmap)
    return false;

  D2D1_SIZE_F bitmap_size(bitmap->GetSize());
  vec_size->x_ = bitmap_size.width;
  vec_size->y_ = bitmap_size.height;
  return true;
}