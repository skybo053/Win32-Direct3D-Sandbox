#ifndef PIXELSHADER_H
#define PIXELSHADER_H

#include "Bindable.h"
#include "Graphics.h"


class PixelShader : public Bindable
{
public:
  PixelShader(const Graphics& pGraphics, const wchar_t* pShaderFile);

  virtual void bind(const Graphics& pGraphics) override;
  virtual void unbind(const Graphics& pGraphics) override;

  virtual ~PixelShader();

private:
  ComPtr<ID3D11PixelShader> pixelShader;
};


#endif

