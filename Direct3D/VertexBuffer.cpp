#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(const Graphics& pGraphics, void* pData, size_t pDataByteSize, UINT pDataStride) : dataByteSize(pDataByteSize), dataByteStride(pDataStride)
{
  D3D11_BUFFER_DESC      vVertexBufferDescriptor      = {};
  D3D11_SUBRESOURCE_DATA vVertexBufferSubResourceData = {};

  vVertexBufferDescriptor.ByteWidth           = pDataByteSize;
  vVertexBufferDescriptor.Usage               = D3D11_USAGE_DEFAULT;
  vVertexBufferDescriptor.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
  vVertexBufferDescriptor.StructureByteStride = pDataStride;

  vVertexBufferSubResourceData.pSysMem = pData;

  pGraphics.getDevice()->CreateBuffer(&vVertexBufferDescriptor, &vVertexBufferSubResourceData, &vertexBufferPointer);
}


void VertexBuffer::bind(const Graphics& pGraphics)
{
  unsigned int vOffset = 0;

  pGraphics.getDeviceContext()->IASetVertexBuffers(0, 1, vertexBufferPointer.GetAddressOf(), &dataByteStride, &vOffset);
}


VertexBuffer::~VertexBuffer()
{

}

