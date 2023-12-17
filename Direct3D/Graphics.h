#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "WindowsHeader.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using Microsoft::WRL::ComPtr;

class Graphics
{
public:
  Graphics(HWND pHwnd, float pWindowWidth, float pWindowHeight);
  Graphics(const Graphics& pGraphics) = delete;
  Graphics& operator=(const Graphics& pGraphics) = delete;

  void endFrame() const;
  void clearBuffer(float pR, float pG, float pB, float pA = 1.0F) const;
  void drawTestTriangle(float pXPos, float pYPos) const;
  void drawSandbox();

  const ComPtr<ID3D11Device>& getDevice() const;
  const ComPtr<ID3D11DeviceContext>& getDeviceContext() const;
  const ComPtr<IDXGISwapChain>& getSwapChain() const;

  float getWindowWidth() const;
  float getWindowHeight() const;
private:
  ComPtr<ID3D11Device>           device           = nullptr;
  ComPtr<IDXGISwapChain>         swapChain        = nullptr;
  ComPtr<ID3D11DeviceContext>    deviceContext    = nullptr;
  ComPtr<ID3D11RenderTargetView> target           = nullptr;
  ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
  float windowWidth, windowHeight;
};


#endif

