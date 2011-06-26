#ifndef GAMEFONT_DATA_H__
#define GAMEFONT_DATA_H__

#include <string>
#include "hashfunc.h"

namespace game_ui {

struct GameFontData {
  static const int DefaultSize = 10;

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

  GameFontData(
    const wchar_t* name, float size = GameFontData::DefaultSize, 
    int weight = GameFontData::GFD_FW_NORMAL, 
    int style = GameFontData::GFD_FS_NORMAL
    )
    : gfd_family_(name), gfd_size_(size), 
      gfd_style_and_weight_(weight | (style << 16)) {}

  bool operator==(const GameFontData& other) const {
    return gfd_family_ == other.gfd_family_ && gfd_size_ == other.gfd_size_ &&
      gfd_style_and_weight_ == other.gfd_style_and_weight_;
  }

  bool operator==(const std::wstring& fontname) const {
    return gfd_family_ == fontname;
  }

  bool operator!=(const GameFontData& other) const {
    return !operator==(other);
  }

  bool operator!=(const std::wstring& other) const {
    return !operator==(other);
  }

  std::wstring GetFontName() const { return gfd_family_; }

  float GetFontSize() const { return gfd_size_; }

  int GetFontStyle() const {
    return gfd_style_and_weight_ & ((~0) << 16);
  }

  int GetFontWeight() const {
    return gfd_style_and_weight_ & ((~0) >> 16);    
  }

  size_t Hash() const {
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

} // ns game_ui

#endif // GAMEFONT_DATA_H__