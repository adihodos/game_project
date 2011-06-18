#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <d2d1.h>
#include "basetypes.h"
#include "gamefont.h"
#include "hashfunc.h"
#include "helpers.h"
#include "lazy_unique_instance.h"
#include "scoped_pointer.h"

struct D2D1ColorFComparator : public std::binary_function<D2D1_COLOR_F, D2D1_COLOR_F, bool> {
  bool operator()(const D2D1_COLOR_F& lhs, const D2D1_COLOR_F& rhs) const {
    return lhs.a == rhs.a && lhs.b == rhs.b && lhs.g == lhs.g && lhs.r == rhs.r;
  }
};

struct D2D1ColorFHasher : public std::unary_function<D2D1_COLOR_F, size_t> {
  size_t operator()(const D2D1_COLOR_F& val) const {
    return hashlittle(&val, sizeof(val), 0);
  }
};

struct GameFontDataHasher : public std::unary_function<GameFontData, size_t> {
  size_t operator()(const GameFontData& fdata) const {
    return fdata.Hash();
  }
};

class GameResourceCache : private base::LazyUniqueInstanceLifeTraits<GameResourceCache> {
public :
  bool Initialize(ID2D1HwndRenderTarget* rtarget);

  ID2D1Bitmap* GetBitmapHandle(const wchar_t* bmpfile);

  ID2D1SolidColorBrush* GetSolidColorBrushHandle(const D2D1::ColorF& color);

  ID2D1SolidColorBrush* GetSolidColorBrushHandle(const D2D1_COLOR_F& color) {
    return GetSolidColorBrushHandle(D2D1::ColorF(color.r, color.g, color.b, color.a));
  }

  ID2D1BitmapBrush* GetBitmapBrushHandle(const wchar_t* bmpfile);

  IDWriteTextFormat* GetFontHandle(const GameFontData& fdata);

  void DiscardAllCachedResources();

private :
  GameResourceCache();

  ~GameResourceCache();

  friend class base::LazyUniqueInstanceLifeTraits<GameResourceCache>;

  typedef std::unordered_map<std::wstring, ID2D1Bitmap*>      GRC_BitmapCache;
  typedef std::unordered_map<std::wstring, ID2D1BitmapBrush*> GRC_BBrushCache;
  typedef std::unordered_map<D2D1_COLOR_F, 
                             ID2D1SolidColorBrush*, D2D1ColorFHasher, 
                             D2D1ColorFComparator>            GRC_SCBrushCache;
  typedef std::unordered_map<
    GameFontData, std::shared_ptr<IDWriteTextFormat>, GameFontDataHasher>
                                                              GRC_FontCache;

  ID2D1HwndRenderTarget*                                    rtarget_;
  scoped_pointer<IWICImagingFactory, D2DInterface>          imgfactory_;
  scoped_pointer<IDWriteFactory, D2DInterface>              writefactory_;
  GRC_BitmapCache                                           bitmap_cache_;
  GRC_SCBrushCache                                          solidcolorbrush_cache_;
  GRC_BBrushCache                                           bitmapbrush_cache_;
  GRC_FontCache                                             font_cache_;

  NO_CPY_CONSTRUCTORS(GameResourceCache);
};