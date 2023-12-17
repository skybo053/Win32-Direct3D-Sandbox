#include "Texture.h"

#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")


Texture::Texture(const Graphics& pGraphics, const wchar_t* pTexturePath)
{
  ULONG_PTR                       vToken                  = 0;
  D3D11_TEXTURE2D_DESC            vTextureDescriptor      = {};
  D3D11_SUBRESOURCE_DATA          vSubResourceData        = {};
  D3D11_SHADER_RESOURCE_VIEW_DESC vShaderResourceViewDesc = {};
  D3D11_SAMPLER_DESC              vSamplerDesc            = {};
  ComPtr<ID3D11Texture2D>         vTexturePointer;

  Gdiplus::Bitmap*                vTextureBitmap;
  Gdiplus::GdiplusStartupInput    vInput;
  Gdiplus::BitmapData             vTextureData;

  //configure GDI Plus for startup
  vInput.GdiplusVersion           = 1;
  vInput.DebugEventCallback       = nullptr;
  vInput.SuppressBackgroundThread = false;

  Gdiplus::GdiplusStartup(&vToken, &vInput, nullptr);

  //load image
  vTextureBitmap = new Gdiplus::Bitmap(pTexturePath);

  //Get buffer to texture data
  vTextureBitmap->LockBits(&Gdiplus::Rect(0, 0, vTextureBitmap->GetWidth(), vTextureBitmap->GetHeight()), Gdiplus::ImageLockMode::ImageLockModeWrite, PixelFormat32bppARGB, &vTextureData);
  vTextureBitmap->UnlockBits(&vTextureData);

  //configure Texture Descriptor struct
  vTextureDescriptor.Width              = vTextureData.Width;
  vTextureDescriptor.Height             = vTextureData.Height;
  vTextureDescriptor.MipLevels          = 1;
  vTextureDescriptor.ArraySize          = 1;
  vTextureDescriptor.Format             = DXGI_FORMAT_B8G8R8A8_UNORM;
  vTextureDescriptor.SampleDesc.Count   = 1;
  vTextureDescriptor.SampleDesc.Quality = 0;
  vTextureDescriptor.Usage              = D3D11_USAGE_DEFAULT;
  vTextureDescriptor.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
  vTextureDescriptor.CPUAccessFlags     = 0;
  vTextureDescriptor.MiscFlags          = 0;

  //configure sub resource data struct
  vSubResourceData.pSysMem              = vTextureData.Scan0;
  vSubResourceData.SysMemPitch          = vTextureData.Stride;

  pGraphics.getDevice()->CreateTexture2D(&vTextureDescriptor, &vSubResourceData, &vTexturePointer);
  
  vShaderResourceViewDesc.Format                    = vTextureDescriptor.Format;
  vShaderResourceViewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
  vShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
  vShaderResourceViewDesc.Texture2D.MipLevels       = 1;

  pGraphics.getDevice()->CreateShaderResourceView(vTexturePointer.Get(), &vShaderResourceViewDesc, &shaderResourceView);

  //configure and create sampler state
  vSamplerDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  vSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  vSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  vSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

  pGraphics.getDevice()->CreateSamplerState(&vSamplerDesc, &samplerState);

  delete vTextureBitmap;

  Gdiplus::GdiplusShutdown(vToken);
}


void Texture::bind(const Graphics& pGraphics)
{
  pGraphics.getDeviceContext()->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());

  pGraphics.getDeviceContext()->PSSetSamplers(0, 1, samplerState.GetAddressOf());
}


Texture::~Texture()
{

}
