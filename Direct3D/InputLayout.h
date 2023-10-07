#ifndef INPUTLAYOUT_H
#define INPUTLAYOUT_H

#include "Bindable.h"
#include "Graphics.h"


class InputLayout : public Bindable
{
public:
  InputLayout(
    const Graphics&           pGraphics, 
    D3D11_INPUT_ELEMENT_DESC* pInputElements, 
    size_t                    pNumInputElements, 
    void*                     pVertexShaderByteCodeBuffer, 
    size_t                    pVertexShaderByteCodeBufferSize);

  virtual void bind(const Graphics& pGraphics) override;

  virtual ~InputLayout();

private:
  ComPtr<ID3D11InputLayout> inputLayout;
};


#endif
