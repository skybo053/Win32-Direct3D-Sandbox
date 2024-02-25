#include "Square.h"

#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Viewport.h"
#include "Topology.h"
#include "InputLayout.h"
#include "Texture.h"


Square::Square(const Graphics& pGraphics, const wchar_t* pVertexShader, const wchar_t* pPixelShader, float pScaleFactor, float pDeltaY, DirectX::XMVECTOR pTranslation, const wchar_t* pTexture)
  : scaleFactor(pScaleFactor), deltaY(pDeltaY), translation(pTranslation), angleY(0.0F)
{
  float vTexturedCubeVertices[] =
  {
    //front face
    -1.0F, -1.0F, -1.0F,         0.0F, 1.0F,
    -1.0F,  1.0F, -1.0F,         0.0F, 0.0F, 
     1.0F,  1.0F, -1.0F,         1.0F, 0.0F,

    -1.0F, -1.0F, -1.0F,         0.0F, 1.0F,
     1.0F,  1.0F, -1.0F,         1.0F, 0.0F,
     1.0F, -1.0F, -1.0F,         1.0F, 1.0F,

     //right face
     1.0F, -1.0F, -1.0F,         0.0F, 1.0F,
     1.0F,  1.0F, -1.0F,         0.0F, 0.0F,
     1.0F,  1.0F,  1.0F,         1.0F, 0.0F,

     1.0F, -1.0F, -1.0F,         0.0F, 1.0F,
     1.0F,  1.0F,  1.0F,         1.0F, 0.0F,
     1.0F, -1.0F,  1.0F,         1.0F, 1.0F,

     //left face
     -1.0F, -1.0F,  1.0F,        0.0F, 1.0F,
     -1.0F,  1.0F,  1.0F,        0.0F, 0.0F,
     -1.0F,  1.0F, -1.0F,        1.0F, 0.0F, 

     -1.0F, -1.0F,  1.0F,        0.0F, 1.0F,
     -1.0F,  1.0F, -1.0F,        1.0F, 0.0F,
     -1.0F, -1.0F, -1.0F,        1.0F, 1.0F,

     //bottom face
     -1.0F, -1.0F, -1.0F,        0.0F, 1.0F,
      1.0F, -1.0F, -1.0F,        0.0F, 0.0F,
      1.0F, -1.0F,  1.0F,        1.0F, 0.0F,

     -1.0F, -1.0F, -1.0F,        0.0F, 1.0F,
      1.0F, -1.0F,  1.0F,        1.0F, 0.0F,
     -1.0F, -1.0F, 1.0F,         1.0F, 1.0F,

     //top face
     1.0F, 1.0F, -1.0F,           0.0F, 1.0F,
     -1.0F, 1.0F, -1.0F,         0.0F, 0.0F,
     -1.0F, 1.0F, 1.0F,          1.0F, 0.0F,

     1.0F, 1.0F, -1.0F,           0.0F, 1.0F,
     -1.0F, 1.0F, 1.0F,          1.0F, 0.0F,
     1.0F, 1.0F, 1.0F,           1.0F, 1.0F,

     //back face
     1.0F, -1.0F, 1.0F,          0.0F, 1.0F,
     1.0F, 1.0F, 1.0F,           0.0F, 0.0F,
     -1.0F, 1.0F, 1.0F,          1.0F, 0.0F,

    1.0F, -1.0F, 1.0F,          0.0F, 1.0F,
    -1.0F, 1.0F, 1.0F,          1.0F, 0.0F,
    -1.0F, -1.0F, 1.0F,         1.0F, 1.0F
  };

  //createIndexBuffer(pGraphics);
  createConstantBuffer(pGraphics);

  dataByteSize = sizeof(vTexturedCubeVertices);
  dataByteStride = sizeof(float) * 5;

  std::unique_ptr<Bindable> vVertexBuffer = std::make_unique<VertexBuffer>(pGraphics, vTexturedCubeVertices, dataByteSize, dataByteStride);
  std::unique_ptr<Bindable> vVertexShader = std::make_unique<VertexShader>(pGraphics, pVertexShader);
  std::unique_ptr<Bindable> vPixelShader = std::make_unique<PixelShader>(pGraphics, pPixelShader);
  std::unique_ptr<Bindable> vViewport = std::make_unique<Viewport>(pGraphics.getWindowWidth(), pGraphics.getWindowHeight());
  std::unique_ptr<Bindable> vTopology = std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  if(pTexture != NULL)
  {
    std::unique_ptr<Bindable> vTexture = std::make_unique<Texture>(pGraphics, pTexture);
    addBind(vTexture);
  }
  

  D3D11_INPUT_ELEMENT_DESC vInputDesc[] =
  {
    {"Position", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TexCoord", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
  };

  std::unique_ptr<Bindable> vInputLayout = std::make_unique<InputLayout>(
    pGraphics,
    vInputDesc,
    sizeof(vInputDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC),
    dynamic_cast<VertexShader*>(vVertexShader.get())->getBufferData(),
    dynamic_cast<VertexShader*>(vVertexShader.get())->getBufferSize());

  addBind(vVertexBuffer);
  addBind(vVertexShader);
  addBind(vPixelShader);
  addBind(vViewport);
  addBind(vTopology);
  addBind(vInputLayout);
}


void Square::createIndexBuffer(const Graphics& pGraphics)
{
  int vTriangleIndices[] =
  {
    0, 1, 2
  };

  /////////////  create index buffer
  int vSquareIndices[] =
  {
    0,1,2, //top triangle
    0,2,3 //bottom triangle
  };

  int vCubeIndices[] =
  {
    0,1,2,  0,2,3, //front face

    5,6,2,  5,2,1, // top face

    7,6,5,    7,5,4, //back face

    0,3,7,   0,7,4, //bottom face

    3,2,6,   3,6,7, //right face

    4,5,1,   4,1,0 //left face
  };

  numIndices = sizeof(vCubeIndices) / sizeof(int);

  ComPtr<ID3D11Buffer>   vIndexBuffer = nullptr;
  D3D11_BUFFER_DESC      vIndexBufferDesc = {};
  D3D11_SUBRESOURCE_DATA vIndexBufferData = {};

  vIndexBufferDesc.ByteWidth = sizeof(vCubeIndices);
  vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  vIndexBufferDesc.StructureByteStride = sizeof(int);

  vIndexBufferData.pSysMem = vCubeIndices;

  pGraphics.getDevice()->CreateBuffer(&vIndexBufferDesc, &vIndexBufferData, &vIndexBuffer);
  pGraphics.getDeviceContext()->IASetIndexBuffer(vIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  //////////////////////////////
}


void Square::createConstantBuffer(const Graphics& pGraphics)
{
  D3D11_BUFFER_DESC      vConstantBufferDesc = {};
  D3D11_SUBRESOURCE_DATA vConstantBufferSubData = {};

  rotation = DirectX::XMMatrixIdentity();

  vConstantBufferDesc.ByteWidth = sizeof(rotation);
  vConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  vConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  vConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

  vConstantBufferSubData.pSysMem = &rotation;

  pGraphics.getDevice()->CreateBuffer(&vConstantBufferDesc, &vConstantBufferSubData, &constantBuffer);
  //pGraphics.getDeviceContext()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}

void Square::update(const Graphics& pGraphics)
{
  angleY += deltaY;

  D3D11_MAPPED_SUBRESOURCE vMsr = {};

  pGraphics.getDeviceContext()->Map(constantBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &vMsr);

  rotation = DirectX::XMMatrixTranspose(
    DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) *
    DirectX::XMMatrixRotationY(angleY) *
    DirectX::XMMatrixTranslationFromVector(translation) *
    DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0F), 640.0F / 480.0F, 1.0F, 10.0F) 
    );
 
  memcpy(vMsr.pData, &rotation, sizeof(rotation));

  pGraphics.getDeviceContext()->Unmap(constantBuffer.Get(), 0);
}


void Square::draw(const Graphics& pGraphics)
{
  bind(pGraphics);

  pGraphics.getDeviceContext()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
  
  pGraphics.getDeviceContext()->Draw(dataByteSize / dataByteStride, 0);
  //pGraphics.getDeviceContext()->DrawIndexed(numIndices, 0, 0);

  unbind(pGraphics);
}