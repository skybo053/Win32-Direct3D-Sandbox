#include "Drawable.h"


void Drawable::bind(const Graphics& pGraphics) const
{
  for(const std::unique_ptr<Bindable>& vBindable : bindables)
  {
    vBindable->bind(pGraphics);
  }
}

void Drawable::unbind(const Graphics& pGraphics) const
{
  for(const std::unique_ptr<Bindable>& vBindable : bindables)
  {
    vBindable->unbind(pGraphics);
  }
}


void Drawable::addBind(std::unique_ptr<Bindable>& pBindable)
{
  bindables.push_back(std::move(pBindable));
}
