#include "PixelShader.h"


PixelShader::PixelShader(const Graphics& pGraphics, const wchar_t* pShaderFile)
{
  ComPtr<ID3DBlob> vShaderByteCode;

  D3DReadFileToBlob(pShaderFile, &vShaderByteCode);

  pGraphics.getDevice()->CreatePixelShader(vShaderByteCode->GetBufferPointer(), vShaderByteCode->GetBufferSize(), 0, &pixelShader);
}


void PixelShader::bind(const Graphics& pGraphics) 
{
  pGraphics.getDeviceContext()->PSSetShader(pixelShader.Get(), 0, 0);
}

void PixelShader::unbind(const Graphics& pGraphics)
{
  pGraphics.getDeviceContext()->PSSetShader(NULL, 0, 0);
}

PixelShader::~PixelShader()
{

}
