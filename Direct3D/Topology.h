#ifndef TOPOLOGY_H
#define TOPOLOGY_H


#include "Bindable.h"
#include "Graphics.h"


class Topology : public Bindable
{
public:
  Topology(D3D11_PRIMITIVE_TOPOLOGY pTopology);
  
  void bind(const Graphics& pGraphics) override;

  ~Topology();
private:
  D3D11_PRIMITIVE_TOPOLOGY topology;
};


#endif

