#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H


#include "Bindable.h"
#include "Graphics.h"


class VertexBuffer : public Bindable
{
public:
  VertexBuffer(const Graphics& pGraphics, void* pData, size_t pDataByteSize, UINT pDataStride);
  virtual ~VertexBuffer() override;

  virtual void bind(const Graphics& pGraphics) override;

private:
  ComPtr<ID3D11Buffer> vertexBufferPointer;
  size_t dataByteSize;
  UINT dataByteStride;
};


#endif

