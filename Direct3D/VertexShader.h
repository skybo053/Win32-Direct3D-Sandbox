#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include "Bindable.h"
#include "Graphics.h"


class VertexShader : public Bindable
{
public:
  VertexShader(const Graphics& pGraphics, const wchar_t* pShaderFile);

  virtual void bind(const Graphics& pGraphics) override;

  void setConstantBuffer(ComPtr<ID3D11Buffer> pConstantBuffer);

  virtual ~VertexShader() override;

  void* getBufferData() const;
  size_t getBufferSize() const;

private:
  ComPtr<ID3D11VertexShader> vertexShader;
  ComPtr<ID3D11Buffer>       vertexShaderConstantBuffer;
  ComPtr<ID3DBlob>           vertexShaderByteCode;
  DirectX::XMMATRIX          transformationMatrix;
};


#endif
