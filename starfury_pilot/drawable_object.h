#ifndef DRAWABLE_OBJECT_H__
#define DRAWABLE_OBJECT_H__

namespace game_core {

class I_DrawableObject {
public :
  virtual ~I_DrawableObject() {}

  virtual void draw() = 0;
};

}

#endif // !DRAWABLE_OBJECT_H__