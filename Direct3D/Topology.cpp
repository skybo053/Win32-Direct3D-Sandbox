#include "Topology.h"


Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY pTopology) : topology(pTopology)
{

}


void Topology::bind(const Graphics& pGraphics)
{
  pGraphics.getDeviceContext()->IASetPrimitiveTopology(topology);
}

Topology::~Topology()
{

}
