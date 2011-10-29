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

namespace gfx {

class vector2;

} // namespace gfx

namespace game_core {

struct d2d1_colorf_cmp : public std::binary_function<D2D1_COLOR_F, D2D1_COLOR_F, bool> {
  bool operator()(const D2D1_COLOR_F& lhs, const D2D1_COLOR_F& rhs) const {
    return lhs.a == rhs.a && lhs.b == rhs.b && lhs.g == lhs.g && lhs.r == rhs.r;
  }
};

struct d2d1_colorf_hash : public std::unary_function<D2D1_COLOR_F, size_t> {
  size_t operator()(const D2D1_COLOR_F& val) const {
    return hashlittle(&val, sizeof(val), 0);
  }
};

struct game_font_hash : public std::unary_function<game_font, size_t> {
  size_t operator()(const game_font& fdata) const {
    return fdata.hash();
  }
};

class resource_cache : private base::LazyUniqueInstanceLifeTraits<resource_cache> {
public :
  bool initialize(ID2D1HwndRenderTarget* rtarget);

  void set_render_target(ID2D1RenderTarget* new_target) {
    rtarget_ = new_target;
  }

  ID2D1Bitmap* get_bitmap(const wchar_t* bmpfile);

  ID2D1SolidColorBrush* get_solid_color_brush(const D2D1::ColorF& color);

  ID2D1BitmapBrush* get_bitmap_brush(const wchar_t* bmpfile);

  IDWriteTextFormat* get_font(const game_font& fdata);

  void discard_cached_resourcess();

  bool get_texture_size(
    const wchar_t* resource_filename,
    gfx::vector2* vec_size
    );

private :
  resource_cache();

  ~resource_cache();

  friend class base::LazyUniqueInstanceLifeTraits<resource_cache>;

  typedef std::unordered_map<std::wstring, ID2D1Bitmap*>      bitmap_cache_t;
  typedef std::unordered_map<std::wstring, ID2D1BitmapBrush*> bitmap_brush_cache_t;
  typedef std::unordered_map<D2D1_COLOR_F, 
                             ID2D1SolidColorBrush*, d2d1_colorf_hash, 
                             d2d1_colorf_cmp>            solidcolor_brush_cache_t;
  typedef std::unordered_map<
    game_core::game_font, std::shared_ptr<IDWriteTextFormat>, game_font_hash>
                                                              font_cache_t;

  ID2D1RenderTarget*                                        rtarget_;
  scoped_pointer<IWICImagingFactory, D2DInterface>          imgfactory_;
  scoped_pointer<IDWriteFactory, D2DInterface>              writefactory_;
  bitmap_cache_t                                            bitmap_cache_;
  solidcolor_brush_cache_t                                  solidcolorbrush_cache_;
  bitmap_brush_cache_t                                      bitmapbrush_cache_;
  font_cache_t                                              font_cache_;

  NO_CPY_CONSTRUCTORS(resource_cache);
};

typedef base::LazyUniqueInstance<game_core::resource_cache> resource_cache_handle;

} // namespace game_core