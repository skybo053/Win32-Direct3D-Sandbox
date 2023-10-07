#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <random>

#include "Graphics.h"
#include "Drawable.h"


class Triangle : public Drawable
{
public:
  Triangle(const Graphics& pGraphics, std::default_random_engine& pRandomEngine);

  virtual void update(const Graphics& pGraphics) override;
  virtual void draw(const Graphics& pGraphics) override;

private:
  size_t dataByteSize;
  size_t dataByteStride;
  ComPtr<ID3D11Buffer> vsConstantBuffer;
  DirectX::XMMATRIX transformation;

  float dXPos = 0.001F;
};


#endif
