#include "precompiled.h"
#include "game_resourcecache.h"
#include "helpers.h"

GameResourceCache::GameResourceCache()
  : rtarget_(), 
    imgfactory_(),
    writefactory_(),
    bitmap_cache_(), 
    solidcolorbrush_cache_(), 
    bitmapbrush_cache_(),
    font_cache_() {}

GameResourceCache::~GameResourceCache() {
  DiscardAllCachedResources();
}

bool GameResourceCache::Initialize(ID2D1HwndRenderTarget* rtarget)  {
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

ID2D1Bitmap* GameResourceCache::GetBitmapHandle(const wchar_t* rsrcname) {
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

  bitmap_cache_.insert(GRC_BitmapCache::value_type(rsrcname, bitmap));
  return bitmap;
}

ID2D1SolidColorBrush* GameResourceCache::GetSolidColorBrushHandle(
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

  solidcolorbrush_cache_.insert(GRC_SCBrushCache::value_type(color, brush));
  return brush;
}

ID2D1BitmapBrush* GameResourceCache::GetBitmapBrushHandle(const wchar_t* bmpname) {
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

  bitmapbrush_cache_.insert(GRC_BBrushCache::value_type(bmpname, newbrush));
  return newbrush;
}

IDWriteTextFormat*
GameResourceCache::GetFontHandle(const GameFontData& fdata) {
  auto fnt_itr = font_cache_.find(fdata);
  if (fnt_itr != font_cache_.end())
    return fnt_itr->second.get();

  DWRITE_FONT_WEIGHT fweight;
  switch (fdata.GetFontWeight()) {
  case GameFontData::GFD_FW_HEAVY :
    fweight = DWRITE_FONT_WEIGHT_HEAVY;
    break;

  case GameFontData::GFD_FW_MEDIUM :
    fweight = DWRITE_FONT_WEIGHT_MEDIUM;
    break;

  case GameFontData::GFD_FW_LIGHT :
    fweight = DWRITE_FONT_WEIGHT_LIGHT;
    break;

  default :
    fweight = DWRITE_FONT_WEIGHT_NORMAL;
    break;
  }

  DWRITE_FONT_STYLE fstyle;
  switch (fdata.GetFontStyle()) {
  case GameFontData::GFD_FS_SITALIC :
    fstyle = DWRITE_FONT_STYLE_ITALIC;
    break;

  case GameFontData::GFD_FS_OBLIQUE :
    fstyle = DWRITE_FONT_STYLE_OBLIQUE;
    break;

  default :
    fstyle = DWRITE_FONT_STYLE_NORMAL;
    break;
  }

  IDWriteTextFormat* textfmt;
  HRESULT ret_code = writefactory_->CreateTextFormat(
    fdata.GetFontName().c_str(), 
    nullptr, // use default font collection
    fweight,
    fstyle,
    DWRITE_FONT_STRETCH_NORMAL,
    fdata.GetFontSize(),
    L"en-us",
    &textfmt);

  if (FAILED(ret_code))
    return nullptr;

  font_cache_.insert(GRC_FontCache::value_type(
    fdata, 
    std::shared_ptr<IDWriteTextFormat>(
      textfmt, utility::COMObject_Deleter<IDWriteTextFormat>())));
  return textfmt;
}

void GameResourceCache::DiscardAllCachedResources() {
  std::for_each(bitmap_cache_.begin(), bitmap_cache_.end(),
    [](GRC_BitmapCache::value_type& valtype_itr) {
      if (valtype_itr.second)
        valtype_itr.second->Release();
  });

  std::for_each(solidcolorbrush_cache_.begin(), solidcolorbrush_cache_.end(),
    [](GRC_SCBrushCache::value_type& valtype_itr) {
      if (valtype_itr.second)
        valtype_itr.second->Release();
  });

  std::for_each(bitmapbrush_cache_.begin(), bitmapbrush_cache_.end(),
    [](GRC_BBrushCache::value_type& valtype_itr) {
      if (valtype_itr.second)
        valtype_itr.second->Release();
  });

  bitmap_cache_.clear();
  bitmapbrush_cache_.clear();
  solidcolorbrush_cache_.clear();
}