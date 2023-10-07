#include "Viewport.h"


Viewport::Viewport(float pWindowWidth, float pWindowHeight)
{
  viewport.Width = pWindowWidth;
  viewport.Height = pWindowHeight;
  viewport.MaxDepth = 1;
}


void Viewport::bind(const Graphics& pGraphics)
{
  pGraphics.getDeviceContext()->RSSetViewports(1, &viewport);
}


Viewport::~Viewport()
{

}