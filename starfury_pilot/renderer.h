#ifndef RENDERER_H__
#define RENDERER_H__

#include <Windows.h>
#include "basetypes.h"
#include "lazy_unique_instance.h"
#include "scoped_pointer.h"

namespace gfx {
class vector2D;
}

interface ID2D1Factory;
interface ID2D1HwndRenderTarget;
interface ID2D1Brush;

namespace game_logic {

class Direct2DRenderer : public base::LazyUniqueInstanceLifeTraits<Direct2DRenderer> {
public :
  bool Initialize(HWND window, const gfx::vector2D& wnd_geometry);

  void Begin_Rendering();

  void End_Rendering();

  void Fill_Rectangle(
    float left, float top, float right, float bottom, ID2D1Brush* brush
    );

  void Draw_Rectangle(
    float left, float top, float right, float bottom, ID2D1Brush* brush, 
    float width
    );

  ID2D1HwndRenderTarget* Get_HWNDRenderTarget() const {
    return r_hwndtarget_.get();
  }

private :
  friend class base::LazyUniqueInstanceLifeTraits<game_logic::Direct2DRenderer>;

  Direct2DRenderer();

  ~Direct2DRenderer();

  bool Create_DeviceIndependentResources();

  bool Create_DeviceDependentResources();

  void Discard_Resources() { r_hwndtarget_.reset(); }

  HWND                                                  r_window_;
  gfx::vector2D                                         r_windowgeometry_;
  scoped_pointer<ID2D1Factory, D2DInterface>            r_d2factory_;
  scoped_pointer<ID2D1HwndRenderTarget, D2DInterface>   r_hwndtarget_;

  NO_CPY_CONSTRUCTORS(Direct2DRenderer);
};

typedef base::LazyUniqueInstance<Direct2DRenderer> Game_Renderer;

} // namespace game_logic

#endif // !RENDERER_H__