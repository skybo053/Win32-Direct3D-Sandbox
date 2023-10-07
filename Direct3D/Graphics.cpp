#include "Graphics.h"
#include "Exception.h"



#pragma comment(lib, "D3DCompiler.lib")


Graphics::Graphics(HWND pHwnd, float pWindowWidth, float pWindowHeight) : windowWidth(pWindowWidth), windowHeight(pWindowHeight)
{
  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 0;
  sd.BufferDesc.RefreshRate.Denominator = 0;
  sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.BufferCount = 1;
  sd.OutputWindow = pHwnd;
  sd.Windowed = true;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  sd.Flags = 0;

  HRESULT result = D3D11CreateDeviceAndSwapChain(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    0,
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &sd,
    &swapChain,
    &device,
    nullptr,
    &deviceContext
  );

  if(FAILED(result))
  {
    throw Exception(__LINE__, __FILE__, "Graphics::Graphics - Call to D3D11CreateDeviceAndSwapChain failed");
  }

  ComPtr<ID3D11Resource> vBackBuffer = nullptr;
  swapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&vBackBuffer);
  device->CreateRenderTargetView(vBackBuffer.Get(), nullptr, &target);

  //create depth buffer
  D3D11_DEPTH_STENCIL_DESC vDepthDesc = {};
  vDepthDesc.DepthEnable = true;
  vDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  vDepthDesc.DepthFunc = D3D11_COMPARISON_LESS; 

  //bind depth state
  ComPtr<ID3D11DepthStencilState> vDepthStencilState;
  device->CreateDepthStencilState(&vDepthDesc, &vDepthStencilState);
  deviceContext->OMSetDepthStencilState(vDepthStencilState.Get(), 1);

  //create depth stensil texture
  ComPtr<ID3D11Texture2D> vDepthStencil;
  D3D11_TEXTURE2D_DESC vTextureDesc = {};
  vTextureDesc.Width = 640;
  vTextureDesc.Height = 480;
  vTextureDesc.MipLevels = 1;
  vTextureDesc.ArraySize = 1;
  vTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
  vTextureDesc.SampleDesc.Count = 1;
  vTextureDesc.SampleDesc.Quality = 0;
  vTextureDesc.Usage = D3D11_USAGE_DEFAULT;
  vTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

  device->CreateTexture2D(&vTextureDesc, nullptr, &vDepthStencil);

  //create view of depth stensil texture
  D3D11_DEPTH_STENCIL_VIEW_DESC vDSVDesc = {};
  vDSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
  vDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  vDSVDesc.Texture2D.MipSlice = 0;

  device->CreateDepthStencilView(vDepthStencil.Get(), &vDSVDesc, &depthStencilView);
  deviceContext->OMSetRenderTargets(1, target.GetAddressOf(), depthStencilView.Get()); //binding render target here with depth buffer instead of drawTestTriangle
  //deviceContext->OMSetRenderTargets(1, target.GetAddressOf(), 0);
}


float Graphics::getWindowWidth() const
{
  return windowWidth;
}


float Graphics::getWindowHeight() const
{
  return windowHeight;
}


void Graphics::endFrame() const
{
  swapChain->Present(1, 0);
}

void Graphics::drawTestTriangle(float pXPos, float pYPos) const
{
  //********** Extra: Creating index buffer to draw indexed **********
  ComPtr<ID3D11Buffer> vIndexBuffer;

  /*
  int vIndexes[] = 
  {
    0, 1, 2
  }; */

  /*
  int vIndexes[] =
  {
    0, 2, 1,
    0, 4, 2,
    0, 5, 4,
    2, 4, 3
  }; */

  int vIndexes[] =
  {
    //cube front face
    0,1,2,    0,2,3,

    //cube left face
    4,5,1,    4,1,0,

    //cube right face
    3,2,6,    3,6,7,

    //cube bottom face
    0,3,7,    0,7,4,

    //cube top face
    5,6,2,    5,2,1,

    //cube back face
    7,6,5,    7,5,4
  };


  D3D11_BUFFER_DESC      vIndexBufferDescriptor = {};
  D3D11_SUBRESOURCE_DATA vIndexSubResourceData = {};

  vIndexBufferDescriptor.ByteWidth = sizeof(vIndexes);
  vIndexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
  vIndexBufferDescriptor.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
  vIndexBufferDescriptor.CPUAccessFlags = 0;
  vIndexBufferDescriptor.MiscFlags = 0;
  vIndexBufferDescriptor.StructureByteStride = sizeof(int);

  //configure sub resource data with index data
  vIndexSubResourceData.pSysMem = vIndexes;

  device->CreateBuffer(&vIndexBufferDescriptor, &vIndexSubResourceData, &vIndexBuffer);

  deviceContext->IASetIndexBuffer(vIndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);


  ///// create constant buffer in pixel shader holding array of colors
  struct Colors
  {
    float r;
    float g;
    float b;
    float a;
  };

  Colors colors[6] = 
  {
    {1.0F, 0.0F, 0.0F, 1.0F},
    {0.0F, 1.0F, 0.0F, 1.0F},
    {0.0F, 0.0F, 1.0F, 1.0F},
    {1.0F, 0.6F, 0.0F, 1.0F},
    {1.0F, 1.0F, 0.0F, 1.0F},
    {1.0F, 0.0F, 1.0F, 1.0F}
  };

  ComPtr<ID3D11Buffer>   vColorConstantBuffer;
  CD3D11_BUFFER_DESC     vColorConstantBufferDescriptor = {};
  D3D11_SUBRESOURCE_DATA vColorConstantBufferSubData = {};

  vColorConstantBufferDescriptor.ByteWidth = sizeof(colors);
  vColorConstantBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  vColorConstantBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
  vColorConstantBufferDescriptor.StructureByteStride = 0; //don't need to know stride for constant buffers

  vColorConstantBufferSubData.pSysMem = colors;

  device->CreateBuffer(&vColorConstantBufferDescriptor, &vColorConstantBufferSubData, &vColorConstantBuffer);

  //bind constant buffer
  deviceContext->PSSetConstantBuffers(0, 1, vColorConstantBuffer.GetAddressOf());


    /////////////////////////////////////////////////////////////////////


  //*********** Extra: Creating constant rotation buffer ******************
  static float vAngle = 0.0F;

  vAngle += .01;

  DirectX::XMMATRIX vTransformation = DirectX::XMMatrixTranspose(
    DirectX::XMMatrixRotationZ(vAngle) * 
    DirectX::XMMatrixRotationX(vAngle) *
    DirectX::XMMatrixTranslation(pXPos, 0.0F, pYPos + 4) *
    DirectX::XMMatrixPerspectiveLH(1.0F, 480.0F/640.0F, 0.5F, 10.0F));

  ComPtr<ID3D11Buffer>   vConstantBuffer;
  CD3D11_BUFFER_DESC     vConstantBufferDescriptor = {};
  D3D11_SUBRESOURCE_DATA vBufferSubData            = {};

  vConstantBufferDescriptor.ByteWidth           = sizeof(vTransformation);
  vConstantBufferDescriptor.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
  vConstantBufferDescriptor.Usage               = D3D11_USAGE_DYNAMIC;
  vConstantBufferDescriptor.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
  vConstantBufferDescriptor.StructureByteStride = 0; //don't need to know stride for constant buffers

  vBufferSubData.pSysMem = &vTransformation;

  device->CreateBuffer(&vConstantBufferDescriptor, &vBufferSubData, &vConstantBuffer);

  //bind constant buffer
  deviceContext->VSSetConstantBuffers(0, 1, vConstantBuffer.GetAddressOf());


  // ********** Step 1: Create VertexBuffer **********

  ComPtr<ID3D11Buffer> vVertexBuffer;

  struct Vertex
  {
    struct Position
    {
      float x;
      float y;
      float z;
    } pos;
  };


  Vertex vVertices[] = 
  {
    //cube front face
    {-1.0F, -1.0F, -1.0F},
    {-1.0F,  1.0F, -1.0F},
    { 1.0F,  1.0F, -1.0F},
    { 1.0F, -1.0F, -1.0F},

    //cube rear face
    { -1.0F, -1.0F, 1.0F},
    { -1.0F,  1.0F, 1.0F},
    {  1.0F,  1.0F, 1.0F},
    {  1.0F, -1.0F, 1.0F}
  };


  D3D11_BUFFER_DESC      vBufferDescriptor = {};
  D3D11_SUBRESOURCE_DATA vSubResourceData = {};

  //configure buffer descriptor
  vBufferDescriptor.ByteWidth           = sizeof(vVertices);
  vBufferDescriptor.Usage               = D3D11_USAGE_DEFAULT;
  vBufferDescriptor.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
  vBufferDescriptor.CPUAccessFlags      = 0;
  vBufferDescriptor.MiscFlags           = 0;
  vBufferDescriptor.StructureByteStride = sizeof(Vertex);

  //configure sub resource data with vertex data
  vSubResourceData.pSysMem = vVertices;
  
  device->CreateBuffer(&vBufferDescriptor, &vSubResourceData, &vVertexBuffer);

  //********** Step 2: Set VertexBuffer **********
  ID3D11Buffer* vVertextBuffers[]    = {vVertexBuffer.Get()}; //create Array of vertex buffers because IASetVertexBuffers takes array of buffers
  size_t        vVertexBuffersStride = sizeof(Vertex);
  const UINT    vOffset              = 0;

  deviceContext->IASetVertexBuffers(0, 1, vVertextBuffers, &vVertexBuffersStride, &vOffset);

  //********** Step 3: Load, Create, and Set Vertex Shader **********
  ComPtr<ID3D11VertexShader> vVertexShader;
  ComPtr<ID3DBlob>           vVertexShaderByteCode;

  D3DReadFileToBlob(L"VertexShader.cso", &vVertexShaderByteCode);

  device->CreateVertexShader(vVertexShaderByteCode->GetBufferPointer(), vVertexShaderByteCode->GetBufferSize(), 0, &vVertexShader);
  deviceContext->VSSetShader(vVertexShader.Get(), 0, 0);

  //********** Step 4: Load, Create, and Set Pixel Shader And Bind Render Target **********
  ComPtr<ID3D11PixelShader> vPixelShader;
  ComPtr<ID3DBlob>          vPixelShaderByteCode;

  D3DReadFileToBlob(L"PixelShader.cso", &vPixelShaderByteCode);

  device->CreatePixelShader(vPixelShaderByteCode->GetBufferPointer(), vPixelShaderByteCode->GetBufferSize(), 0, &vPixelShader);
  deviceContext->PSSetShader(vPixelShader.Get(), 0, 0);

  //********** Step 5: Set Input Layout to describe data to D3D **********
  ComPtr<ID3D11InputLayout> vInputLayout;

  D3D11_INPUT_ELEMENT_DESC vInputElementDesc;
  vInputElementDesc.SemanticName = "Position";
  vInputElementDesc.SemanticIndex = 0;
  vInputElementDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
  vInputElementDesc.InputSlot = 0;
  vInputElementDesc.AlignedByteOffset = 0;
  vInputElementDesc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
  vInputElementDesc.InstanceDataStepRate = 0;

  D3D11_INPUT_ELEMENT_DESC vIEDArray[] = 
  {
    vInputElementDesc
  };

  device->CreateInputLayout(vIEDArray, 1, vVertexShaderByteCode->GetBufferPointer(), vVertexShaderByteCode->GetBufferSize(), &vInputLayout);
  deviceContext->IASetInputLayout(vInputLayout.Get());

  //**********  Step 5: Configure Viewport **********
  D3D11_VIEWPORT vViewPort = {};
  vViewPort.Width = 640;
  vViewPort.Height = 480;
  vViewPort.MaxDepth = 1;

  deviceContext->RSSetViewports(1, &vViewPort);

  //**********  Step 6: Set primitive Topology to tell D3D how to draw Vertices it was given**********
  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  //********** Step 7: Draw **********
  //deviceContext->Draw(sizeof(vVertices) / sizeof(Vertex), 0);

  deviceContext->DrawIndexed(sizeof(vIndexes) / sizeof(int), 0, 0);
}







void Graphics::drawSandbox()
{
  //buffer for triangle data
  struct Vertices
  {
    struct
    {
      float x, y, z;
    } pos;

    struct
    {
      float r, g, b;
    };
  };
  
  Vertices vTriangleVertices[] = 
  {
    {-0.5F, -0.5F, 0.0F,     1.0F, 0.0F, 0.0F},
    {0.0F,  0.5F, 0.0F,      0.0F, 1.0F, 0.0F},
    {0.5F, -0.5F, 0.0F,      0.0F, 0.0F, 1.0F}
  };

  ComPtr<ID3D11Buffer>   vTriangleVerticesBuffer;
  D3D11_BUFFER_DESC      vTriangleBufferDescripter = {};
  D3D11_SUBRESOURCE_DATA vTriangleResourceData     = {};

  vTriangleBufferDescripter.ByteWidth = sizeof(vTriangleVertices);
  vTriangleBufferDescripter.Usage     = D3D11_USAGE_DEFAULT;
  vTriangleBufferDescripter.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vTriangleBufferDescripter.StructureByteStride = sizeof(Vertices);

  vTriangleResourceData.pSysMem = vTriangleVertices;

  device->CreateBuffer(&vTriangleBufferDescripter, &vTriangleResourceData, &vTriangleVerticesBuffer);

  unsigned int vStride = sizeof(Vertices);
  unsigned int vOffset = 0;
  deviceContext->IASetVertexBuffers(0, 1, vTriangleVerticesBuffer.GetAddressOf(), &vStride, &vOffset);

  //create + load shaders
  ComPtr<ID3DBlob>           vVertexShaderBlob;
  ComPtr<ID3D11VertexShader> vVertexShader;
  D3DReadFileToBlob(L"SandboxVertexShader.cso", &vVertexShaderBlob);
  device->CreateVertexShader(vVertexShaderBlob->GetBufferPointer(), vVertexShaderBlob->GetBufferSize(), 0, &vVertexShader);
  deviceContext->VSSetShader(vVertexShader.Get(), 0, 0);

  ComPtr<ID3DBlob>          vPixelShaderBlob;
  ComPtr<ID3D11PixelShader> vPixelShader;
  D3DReadFileToBlob(L"SandboxPixelShader.cso", &vPixelShaderBlob);
  device->CreatePixelShader(vPixelShaderBlob->GetBufferPointer(), vPixelShaderBlob->GetBufferSize(), 0, &vPixelShader);
  deviceContext->PSSetShader(vPixelShader.Get(), 0, 0);

  //Describe input layout

  ComPtr<ID3D11InputLayout> vInputLayout;
  D3D11_INPUT_ELEMENT_DESC  vPositionInputElementDescriptor = {};
  
  vPositionInputElementDescriptor.SemanticName = "Position";
  vPositionInputElementDescriptor.SemanticIndex = 0;
  vPositionInputElementDescriptor.Format = DXGI_FORMAT_R32G32B32_FLOAT;
  vPositionInputElementDescriptor.InputSlot = 0;
  vPositionInputElementDescriptor.AlignedByteOffset = 0;
  vPositionInputElementDescriptor.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  vPositionInputElementDescriptor.InstanceDataStepRate = 0;

  D3D11_INPUT_ELEMENT_DESC  vColorInputElementDescriptor = {};

  vColorInputElementDescriptor.SemanticName = "Color";
  vColorInputElementDescriptor.SemanticIndex = 0;
  vColorInputElementDescriptor.Format = DXGI_FORMAT_R32G32B32_FLOAT;
  vColorInputElementDescriptor.InputSlot = 0;
  vColorInputElementDescriptor.AlignedByteOffset = sizeof(vTriangleVertices[0].pos);
  vColorInputElementDescriptor.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  vColorInputElementDescriptor.InstanceDataStepRate = 0;

  D3D11_INPUT_ELEMENT_DESC vInputLayoutArray[] = {vPositionInputElementDescriptor, vColorInputElementDescriptor};

  device->CreateInputLayout(vInputLayoutArray, 2, vVertexShaderBlob->GetBufferPointer(), vVertexShaderBlob->GetBufferSize(), &vInputLayout);
  deviceContext->IASetInputLayout(vInputLayout.Get());
  
  //Set topology
  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  //Set Render Target (already setting in Graphics::Graphics)
  //deviceContext->OMSetRenderTargets(1, target.GetAddressOf(), 0);

  //set viewport
  D3D11_VIEWPORT vViewPort = {};
  vViewPort.Width = 640;
  vViewPort.Height = 480;
  vViewPort.MaxDepth = 1;

  deviceContext->RSSetViewports(1, &vViewPort);

  //draw
  deviceContext->Draw(sizeof(vTriangleVertices) / sizeof(Vertices), 0);

}


void Graphics::clearBuffer(float pR, float pG, float pB, float pA) const
{
  float vColorArray[] = {pR, pG, pB, pA};
  deviceContext->ClearRenderTargetView(target.Get(), vColorArray);
  deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0F, 0);
}


const ComPtr<ID3D11Device>& Graphics::getDevice() const
{
  return device;
}


const ComPtr<ID3D11DeviceContext>& Graphics::getDeviceContext() const
{
  return deviceContext;
}


const ComPtr<IDXGISwapChain>& Graphics::getSwapChain() const
{
  return swapChain;
}
