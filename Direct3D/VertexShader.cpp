#include "VertexShader.h"
#include <iostream>


VertexShader::VertexShader(const Graphics& pGraphics, const wchar_t* pShaderFile)
{
  D3DReadFileToBlob(pShaderFile, &vertexShaderByteCode);

  pGraphics.getDevice()->CreateVertexShader(vertexShaderByteCode->GetBufferPointer(), vertexShaderByteCode->GetBufferSize(), 0, &vertexShader);
}


void VertexShader::bind(const Graphics& pGraphics)
{
  pGraphics.getDeviceContext()->VSSetShader(vertexShader.Get(), 0, 0);
  //pGraphics.getDeviceContext()->VSSetConstantBuffers(0, 1, vertexShaderConstantBuffer.GetAddressOf());
}


void VertexShader::setConstantBuffer(ComPtr<ID3D11Buffer> pConstantBuffer)
{
  vertexShaderConstantBuffer = pConstantBuffer;
}


void* VertexShader::getBufferData() const
{
  return vertexShaderByteCode->GetBufferPointer();
}


size_t VertexShader::getBufferSize() const
{
  return vertexShaderByteCode->GetBufferSize();
}


VertexShader::~VertexShader()
{

}
