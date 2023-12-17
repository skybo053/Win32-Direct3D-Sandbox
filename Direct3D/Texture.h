#ifndef TEXTURE_H
#define TEXTURE_H

#include "Bindable.h"


class Texture : public Bindable
{
public:
  Texture(const Graphics& pGraphics, const wchar_t* pTexturePath);
  virtual void bind(const Graphics& pGraphics) override;
  virtual ~Texture();
private:
  ComPtr<ID3D11ShaderResourceView> shaderResourceView;
  ComPtr<ID3D11SamplerState>       samplerState;
};


#endif

