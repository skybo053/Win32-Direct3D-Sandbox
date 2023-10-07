#include "InputLayout.h"


InputLayout::InputLayout(
  const Graphics&           pGraphics,
  D3D11_INPUT_ELEMENT_DESC* pInputElements,
  size_t                    pNumInputElements,
  void*                     pVertexShaderByteCodeBuffer,
  size_t                    pVertexShaderByteCodeBufferSize)
{
  pGraphics.getDevice()->CreateInputLayout(pInputElements, pNumInputElements, pVertexShaderByteCodeBuffer, pVertexShaderByteCodeBufferSize, &inputLayout);
}


void InputLayout::bind(const Graphics& pGraphics)
{
  pGraphics.getDeviceContext()->IASetInputLayout(inputLayout.Get());
}


InputLayout::~InputLayout()
{

}
