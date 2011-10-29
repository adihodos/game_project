#ifndef PRECOMPILED_H__
#define PRECOMPILED_H__

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cstddef>
#include <cassert>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <string>
#include <vector>

//#include <gfx/circle.h>
#include <gfx/ellipse.h>
#include <gfx/matrix3X3.h>
#include <gfx/rectangle.h>
#include <gfx/vector2.h>

#if defined(NTDDI_VERSION)
#undef NTDDI_VERSION
#endif

#define NTDDI_VERSION NTDDI_WIN7

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#endif // PRECOMPILED_H__