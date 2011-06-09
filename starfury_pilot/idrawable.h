#pragma once

class Direct2DRenderer;

class IDrawable {
public :
  virtual ~IDrawable() {}

  virtual void Draw(Direct2DRenderer* renderer) = 0;
};