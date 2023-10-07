#include "Triangle.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "Topology.h"
#include "Viewport.h"


Triangle::Triangle(const Graphics& pGraphics, std::default_random_engine& pRandomEngine)
{
  std::uniform_real_distribution vColorDistribution(0.0F, 1.0F),
                                 vScaleDistribuition(0.1F, 0.8F),
                                 vTransformDistribuition(-1.0F, 1.0F);

  float r = vColorDistribution(pRandomEngine);
  float g = vColorDistribution(pRandomEngine);
  float b = vColorDistribution(pRandomEngine);

  float vVertices[] =
  {
    -0.5f, -0.5F, 0.0F,        r, g, b, 1.0F,
     0.0F,  0.5F, 0.0F,        r, g, b, 1.0F,
     0.5F, -0.5F, 0.0F,        r, g, b, 1.0F
  };

  dataByteSize = sizeof(vVertices);
  dataByteStride = sizeof(float) * 7;

  std::unique_ptr<Bindable> vVertexBuffer = std::make_unique<VertexBuffer>(pGraphics, vVertices, dataByteSize, dataByteStride);
  std::unique_ptr<Bindable> vVertexShader = std::make_unique<VertexShader>(pGraphics, L"SandboxVertexShader.cso");
  std::unique_ptr<Bindable> vPixelShader  = std::make_unique<PixelShader>(pGraphics, L"SandboxPixelShader.cso");
  std::unique_ptr<Bindable> vTopology     = std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  std::unique_ptr<Bindable> vViewport     = std::make_unique<Viewport>(pGraphics.getWindowWidth(), pGraphics.getWindowHeight());

  D3D11_INPUT_ELEMENT_DESC vInputs[] = 
  {
    {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                 D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"Color",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
  };

  std::unique_ptr<Bindable> vInputLayout = std::make_unique<InputLayout>(
    pGraphics, vInputs, 2, dynamic_cast<VertexShader*>(vVertexShader.get())->getBufferData(), dynamic_cast<VertexShader*>(vVertexShader.get())->getBufferSize());


  //set up constant buffer

  transformation =
    DirectX::XMMatrixScaling(vScaleDistribuition(pRandomEngine), vScaleDistribuition(pRandomEngine), 1.0F) *
    DirectX::XMMatrixTranslation(vTransformDistribuition(pRandomEngine), vTransformDistribuition(pRandomEngine), 0.0F);
  
  D3D11_BUFFER_DESC      vConstantBufferDesc = {};
  D3D11_SUBRESOURCE_DATA vConstantBufferData = {};

  vConstantBufferDesc.ByteWidth = sizeof(transformation);
  vConstantBufferDesc.Usage     = D3D11_USAGE_DYNAMIC;
  vConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  vConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  vConstantBufferData.pSysMem = &transformation;

  pGraphics.getDevice()->CreateBuffer(&vConstantBufferDesc, &vConstantBufferData, &vsConstantBuffer);

  VertexShader* vs = dynamic_cast<VertexShader*>(vVertexShader.get());

  vs->setConstantBuffer(vsConstantBuffer);

  addBind(vVertexBuffer);
  addBind(vVertexShader);
  addBind(vPixelShader);
  addBind(vInputLayout);
  addBind(vTopology);
  addBind(vViewport);
}


void Triangle::update(const Graphics& pGraphics)
{
  D3D11_MAPPED_SUBRESOURCE vMsr = {};

  pGraphics.getDeviceContext()->Map(vsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &vMsr);

  transformation = transformation * DirectX::XMMatrixTranslation(dXPos, 0.0F, 0.0F);

  memcpy(vMsr.pData, &transformation, sizeof(transformation));

  pGraphics.getDeviceContext()->Unmap(vsConstantBuffer.Get(), 0);
}


void Triangle::draw(const Graphics& pGraphics)
{
  for(std::unique_ptr<Bindable>& vBindable : bindables)
  {
    vBindable->bind(pGraphics);
  }

  pGraphics.getDeviceContext()->Draw(dataByteSize / dataByteStride, 0);
}
