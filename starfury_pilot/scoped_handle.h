#ifndef SCOPED_HANDLE_H__
#define SCOPED_HANDLE_H__

#include <ObjBase.h>
#include <Windows.h>
#include "basetypes.h"

template<typename handle_traits>
class scoped_handle {
public :
  typedef typename handle_traits::handle_type handle_type;

  explicit scoped_handle(handle_type hptr = handle_traits::null_handle())
    : hptr_(hptr) {}

  ~scoped_handle() {
    handle_traits::dispose(hptr_);
  }

  operator handle_type() const {
    return hptr_;
  }

  handle_type get() const {
    return hptr_;
  }

  bool operator!() const {
    return hptr_ == handle_traits::null_handle();
  }

  void reset(handle_type hptr = handle_traits::null_handle()) {
    if (hptr_ != hptr) {
      handle_traits::dispose(hptr_);
      hptr_ = hptr;
    }
  }

  handle_type release() {
    handle_type tmp = hptr_;
    hptr_ = handle_type::null_handle();
    return tmp;
  }

private :
  handle_type hptr_;
  NO_CPY_CONSTRUCTORS(scoped_handle);
};

struct win_file {
  typedef HANDLE handle_type;

  static HANDLE null_handle() {
    return INVALID_HANDLE_VALUE;
  }

  static void dispose(HANDLE hptr) {
    if (hptr != null_handle())
      ::CloseHandle(hptr);
  }
};

struct win_generic_handle {
  typedef HANDLE handle_type;

  static HANDLE null_handle() {
    return nullptr;
  }

  static void dispose(HANDLE hptr) {
    if (hptr != null_handle())
      ::CloseHandle(hptr);
  }
};

typedef win_generic_handle win_event;
typedef win_generic_handle win_mutex;

struct win_gdi_bitmap {
  typedef HBITMAP handle_type;

  static HBITMAP null_handle() {
    return nullptr;
  }

  static void dispose(HBITMAP bmp) {
    if (bmp != null_handle()) {
      ::DeleteObject(bmp);
    }
  }
};

struct win_gdi_devctx {
  typedef HDC handle_type;

  static HDC null_handle() {
    return nullptr;
  }

  static void dispose(HDC devctx) {
    if (devctx != null_handle()) {
      ::DeleteDC(devctx);
    }
  }
};

class win_gdi_nddevctx {
public :
  win_gdi_nddevctx(HWND window, HDC devctx)
    : window_(window),
      devctx_(devctx) {}

  ~win_gdi_nddevctx() {
    if (devctx_ != nullptr)
      ::ReleaseDC(window_, devctx_);
  }

  bool operator!() const {
    return devctx_ == nullptr;
  }

  operator HDC() const {
    return devctx_;
  }
private :
  HWND window_;
  HDC devctx_;
  NO_CPY_CONSTRUCTORS(win_gdi_nddevctx);
};

class com_initialize_helper {
public :
  explicit com_initialize_helper(DWORD init_type = COINIT_MULTITHREADED)
    : result_(::CoInitializeEx(nullptr, init_type)) {}

  ~com_initialize_helper() {
    if (result_ == S_OK || result_ == S_FALSE)
      ::CoUninitialize();
  }

  bool operator!() const {
    return result_ != S_OK && result_ != S_FALSE;
  }
private :
  HRESULT result_;
  NO_CPY_CONSTRUCTORS(com_initialize_helper);
};

#endif // SCOPED_HANDLE_H__