#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Bindable.h"
#include "Graphics.h"

#include <vector>
#include <memory>


class Drawable
{
public:

  void bind(const Graphics& pGraphics) const;
  void addBind(std::unique_ptr<Bindable>& pBindable);

  virtual void update(const Graphics& pGraphics) = 0;
  virtual void draw(const Graphics& pGraphics) = 0;

  virtual ~Drawable() = default;
protected:
  std::vector<std::unique_ptr<Bindable>> bindables;
};


#endif

