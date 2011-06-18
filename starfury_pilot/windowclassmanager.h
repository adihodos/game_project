#pragma once

#include <string>
#include <vector>
#include <windows.h>

struct WindowClassData {
  WindowClassData(
    const std::wstring& classname,
    DWORD style = 0L,
    DWORD exstyle = 0L) : wcd_classname(classname) {}

  bool operator==(const WindowClassData& other) const {
    return wcd_classname == other.wcd_classname;
  }

  bool operator!=(const WindowClassData& other) const {
    return !operator==(other);
  }

  const std::wstring  wcd_classname;
  DWORD               wcd_style;
  DWORD               wcd_ex_style;
  HBRUSH              wcd_bkbrush;
};

//
// not MT safe!!!
class WindowClassRegistrar {
public :
  ~WindowClassRegistrar() {
    delete regptr_;
  }

  static WindowClassRegistrar* Get() {
    if (!regptr_)
      regptr_ = new WindowClassRegistrar();

    return regptr_;
  }

  bool GetClassInfo(const WindowClassData& classdata)

private :
  WindowClassRegistrar();

  static WindowClassRegistrar* regptr_;
};