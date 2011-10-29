#pragma once

#include <string>
#include "hashfunc.h"

namespace game_core {

class game_font {
public :
  static const int kDefaultSize = 10;

  enum FontWeight {
    GFD_FW_LIGHT,
    GFD_FW_MEDIUM,
    GFD_FW_NORMAL,
    GFD_FW_HEAVY
  };

  enum FontStyle {
    GFD_FS_NORMAL,
    GFD_FS_SITALIC,
    GFD_FS_OBLIQUE
  };

  game_font(
    const wchar_t* name, float size = game_font::kDefaultSize, 
    int weight = game_font::GFD_FW_NORMAL, 
    int style = game_font::GFD_FS_NORMAL
    )
    : gfd_family_(name), gfd_size_(size), 
      gfd_style_and_weight_(weight | (style << 16)) {}

  bool operator==(const game_font& other) const {
    return gfd_family_ == other.gfd_family_ && gfd_size_ == other.gfd_size_ &&
      gfd_style_and_weight_ == other.gfd_style_and_weight_;
  }

  bool operator==(const std::wstring& fontname) const {
    return gfd_family_ == fontname;
  }

  bool operator!=(const game_font& other) const {
    return !operator==(other);
  }

  bool operator!=(const std::wstring& other) const {
    return !operator==(other);
  }

  std::wstring get_font_name() const { return gfd_family_; }

  float get_font_size() const { return gfd_size_; }

  int get_font_style() const {
    return gfd_style_and_weight_ & ((~0) << 16);
  }

  int get_font_weight() const {
    return gfd_style_and_weight_ & ((~0) >> 16);    
  }

  size_t hash() const {
    return hashlittle(
      gfd_family_.c_str(), 
      gfd_family_.length() * sizeof(std::wstring::value_type),
      0);
  }

private :
  std::wstring  gfd_family_;
  float         gfd_size_;
  int           gfd_style_and_weight_;
};

} // ns game_core