#include "Square.h"

#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Viewport.h"
#include "Topology.h"
#include "InputLayout.h"



Square::Square(const Graphics& pGraphics)
{

  float vTriangleVertices[] = 
  {
    -0.5F, -0.5F, 0.0F,
     0.0F, 0.5F, 0.0F,
     0.5F, -0.5F, 0.0F
  };

  float vSquareVertices[] = 
  {
    -0.5F, -0.5F, 0.0F,        0.0F, -1.0F,
    -0.5F, 0.5F, 0.0F,         0.0F,  0.0F,
    0.5F, 0.5F, 0.0F,          1.0f,  0.0F,
    0.5F, -0.5F, 0.0F,         1.0F, -1.0F
  };

  float vCubeVertices[] = 
  {
    -0.5, 0.5, -0.5, //0
    0.5, 0.5, -0.5, //1
    0.5, -0.5, -0.5,  //2
    -0.5, -0.5, -0.5, //3

    -0.5, 0.5, 0.5, //4
    0.5, 0.5, 0.5, //5
    0.5, -0.5, 0.5, //6
    -0.5, -0.5, 0.5 //7
  };

  createIndexBuffer(pGraphics);
  createConstantBuffer(pGraphics);

  dataByteSize   = sizeof(vSquareVertices);
  dataByteStride = sizeof(float) * 5;

  std::unique_ptr<Bindable> vVertexBuffer = std::make_unique<VertexBuffer>(pGraphics, vSquareVertices, dataByteSize, dataByteStride);
  std::unique_ptr<Bindable> vVertexShader = std::make_unique<VertexShader>(pGraphics, L"BoxVertexShader.cso");
  std::unique_ptr<Bindable> vPixelShader  = std::make_unique<PixelShader>(pGraphics, L"BoxPixelShader.cso");
  std::unique_ptr<Bindable> vViewport     = std::make_unique<Viewport>(pGraphics.getWindowWidth(), pGraphics.getWindowHeight());
  std::unique_ptr<Bindable> vTopology     = std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  D3D11_INPUT_ELEMENT_DESC vInputDesc[] = 
  {
    {"Position", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TexCoord", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
  };

  std::unique_ptr<Bindable> vInputLayout = std::make_unique<InputLayout>(
    pGraphics, 
    vInputDesc,
    2, 
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
    3, 0, 1,  3, 1, 2, //front face

    1, 0, 4,  1, 4, 5, // top face

    6, 5, 4,  6, 4, 7, //back face

    3, 2, 6,  3, 6, 7, //bottom face

    2, 1, 5,  2, 5, 6, //right face

    7, 4, 0,  7, 0, 3 //left face
  };

  numIndices = sizeof(vSquareIndices) / sizeof(int);

  ComPtr<ID3D11Buffer>   vIndexBuffer = nullptr;
  D3D11_BUFFER_DESC      vIndexBufferDesc = {};
  D3D11_SUBRESOURCE_DATA vIndexBufferData = {};

  vIndexBufferDesc.ByteWidth = sizeof(vSquareIndices);
  vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  vIndexBufferDesc.StructureByteStride = sizeof(int);

  vIndexBufferData.pSysMem = vSquareIndices;

  pGraphics.getDevice()->CreateBuffer(&vIndexBufferDesc, &vIndexBufferData, &vIndexBuffer);
  pGraphics.getDeviceContext()->IASetIndexBuffer(vIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  //////////////////////////////
}


void Square::createConstantBuffer(const Graphics& pGraphics)
{
  D3D11_BUFFER_DESC      vConstantBufferDesc    = {};
  D3D11_SUBRESOURCE_DATA vConstantBufferSubData = {};

  rotation = DirectX::XMMatrixIdentity();

  vConstantBufferDesc.ByteWidth      = sizeof(rotation);
  vConstantBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
  vConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  vConstantBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;

  vConstantBufferSubData.pSysMem = &rotation;

  pGraphics.getDevice()->CreateBuffer(&vConstantBufferDesc, &vConstantBufferSubData, &constantBuffer);
  pGraphics.getDeviceContext()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}


void Square::update(const Graphics& pGraphics)
{
  static float angle = 0.1F;
  static float angleDelta = 0.01F;
  
  angle += angleDelta;

  D3D11_MAPPED_SUBRESOURCE vMsr = {};

  pGraphics.getDeviceContext()->Map(constantBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &vMsr);

  /*
  rotation = rotation * 
    DirectX::XMMatrixRotationZ(angle); */

  /* rotation = DirectX::XMMatrixTranspose(
    DirectX::XMMatrixTranslation(0, 0.0F, 3) *
    DirectX::XMMatrixPerspectiveLH(1.0F, 480.0F / 640.0F, 0.5F, 10.0F)); */

  /*
  rotation =
    DirectX::XMMatrixRotationZ(angle) *
    DirectX::XMMatrixRotationX(angle) *
    DirectX::XMMatrixTranslation(0,0,4) *
    DirectX::XMMatrixPerspectiveLH(1.0F, 480.0F / 640.0F, 0.5F, 10.0F); */

  /*
  rotation =
    DirectX::XMMatrixTranslation(0, 0.0F, 4) *
    DirectX::XMMatrixPerspectiveLH(1.0F, 480.0F / 640.0F, 0.5F, 10.0F); */
  
  rotation = DirectX::XMMatrixRotationZ(angle) *
    DirectX::XMMatrixRotationX(angle) * 
    DirectX::XMMatrixTranslation(0.0, 0.0F, 3.0F) *
    DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0F), 640.0F/480.0F, 1.0F, 10.0F);
 
  memcpy(vMsr.pData, &rotation, sizeof(rotation));

  pGraphics.getDeviceContext()->Unmap(constantBuffer.Get(), 0);
}


void Square::draw(const Graphics& pGraphics)
{
  for(std::unique_ptr<Bindable>& vBindable : bindables)
  {
    vBindable->bind(pGraphics);
  }

  //pGraphics.getDeviceContext()->Draw(dataByteSize / dataByteStride, 0);



  //draw square
  pGraphics.getDeviceContext()->DrawIndexed(numIndices, 0, 0);
}