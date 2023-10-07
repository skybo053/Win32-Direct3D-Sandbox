#include "PixelShader.h"

#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")


PixelShader::PixelShader(const Graphics& pGraphics, const wchar_t* pShaderFile)
{
  ComPtr<ID3DBlob> vShaderByteCode;

  D3DReadFileToBlob(pShaderFile, &vShaderByteCode);

  pGraphics.getDevice()->CreatePixelShader(vShaderByteCode->GetBufferPointer(), vShaderByteCode->GetBufferSize(), 0, &pixelShader);
}


void PixelShader::bind(const Graphics& pGraphics) 
{
  static bool GDIPlusLoaded = false;

  if(GDIPlusLoaded == false)
  {
    ULONG_PTR vToken = 0;
    Gdiplus::GdiplusStartupInput vInput;
    vInput.GdiplusVersion = 1;
    vInput.DebugEventCallback = nullptr;
    vInput.SuppressBackgroundThread = false;
    Gdiplus::GdiplusStartup(&vToken, &vInput, nullptr);
    GDIPlusLoaded = true;
  }

  Gdiplus::Bitmap vTexture(L"Textures/wall2.png");
  //Get buffer to texture data

  Gdiplus::BitmapData vTextureData;

  vTexture.LockBits(&Gdiplus::Rect(0, 0, vTexture.GetWidth(), vTexture.GetHeight()), Gdiplus::ImageLockMode::ImageLockModeWrite, PixelFormat32bppARGB, &vTextureData);

  //end getting buffer


  ComPtr<ID3D11Texture2D> vTexturePointer;

  D3D11_TEXTURE2D_DESC   vTextureDescriptor = {};
  D3D11_SUBRESOURCE_DATA vSubResourceData = {};

  vTextureDescriptor.Width = vTexture.GetWidth();
  vTextureDescriptor.Height = vTexture.GetHeight();
  vTextureDescriptor.MipLevels = 1;
  vTextureDescriptor.ArraySize = 1;
  vTextureDescriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  vTextureDescriptor.SampleDesc.Count = 1;
  vTextureDescriptor.SampleDesc.Quality = 0;
  vTextureDescriptor.Usage = D3D11_USAGE_DEFAULT;
  vTextureDescriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  vTextureDescriptor.CPUAccessFlags = 0;
  vTextureDescriptor.MiscFlags = 0;

  vSubResourceData.pSysMem = vTextureData.Scan0;
  vSubResourceData.SysMemPitch = vTexture.GetWidth() * sizeof(int);

  pGraphics.getDevice()->CreateTexture2D(&vTextureDescriptor, &vSubResourceData, &vTexturePointer);

  //create shader view
  ComPtr<ID3D11ShaderResourceView> vShaderResourceView;

  D3D11_SHADER_RESOURCE_VIEW_DESC vShaderResourceViewDesc = {};
  vShaderResourceViewDesc.Format = vTextureDescriptor.Format;
  vShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  vShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
  vShaderResourceViewDesc.Texture2D.MipLevels = 1;

  pGraphics.getDevice()->CreateShaderResourceView(vTexturePointer.Get(), &vShaderResourceViewDesc, &vShaderResourceView);

  //bind texture
  pGraphics.getDeviceContext()->PSSetShaderResources(0, 1, vShaderResourceView.GetAddressOf());


  //bind samper state
  ComPtr<ID3D11SamplerState> vSamplerState;
  
  D3D11_SAMPLER_DESC vSamplerDesc = {};
  vSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  vSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  vSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  vSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  
  pGraphics.getDevice()->CreateSamplerState(&vSamplerDesc, &vSamplerState);
  pGraphics.getDeviceContext()->PSSetSamplers(0, 1, vSamplerState.GetAddressOf());

  //bind pixel shader
  pGraphics.getDeviceContext()->PSSetShader(pixelShader.Get(), 0, 0);
}


PixelShader::~PixelShader()
{

}
