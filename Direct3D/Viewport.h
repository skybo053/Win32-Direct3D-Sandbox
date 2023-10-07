#ifndef VIEWPORT_H
#define VIEWPORT_H


#include "Graphics.h"
#include "Bindable.h"


class Viewport : public Bindable
{
public:
  Viewport(float pWindowWidth, float pWindowHeight);

  void bind(const Graphics& pGraphics) override;

  ~Viewport();
private:
  D3D11_VIEWPORT viewport = {};
};


#endif

