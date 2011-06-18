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

#include <gfx/circle.h>
#include <gfx/vector2d.h>
#include <gfx/matrix3x3.h>
#include <gfx/misc.h>

#if defined(NTDDI_VERSION)
#undef NTDDI_VERSION
#endif

#define NTDDI_VERSION NTDDI_WIN7
#include <Windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#endif // PRECOMPILED_H__