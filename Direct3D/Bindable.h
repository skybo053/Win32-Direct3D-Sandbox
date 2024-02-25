#ifndef BINDABLE_H
#define BINDABLE_H


#include "Graphics.h"


class Bindable
{
public:
  Bindable() = default;
  virtual void bind(const Graphics& pGraphics) = 0;
  virtual void unbind(const Graphics& pGraphics);
  virtual ~Bindable() = default;
private:
};


#endif

