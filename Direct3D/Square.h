#ifndef SQUARE_H
#define SQUARE_H

#include "Drawable.h"
#include "Graphics.h"


class Square : public Drawable
{
public:
  Square(const Graphics& pGraphics);

  virtual void update(const Graphics& pGraphics) override;
  virtual void draw(const Graphics& pGraphics) override;

private:
  void createIndexBuffer(const Graphics& pGraphics);
  void createConstantBuffer(const Graphics& pGraphics);

  size_t dataByteSize;
  size_t dataByteStride;
  int    numIndices;
  DirectX::XMMATRIX    rotation;
  ComPtr<ID3D11Buffer> constantBuffer;
};


#endif

