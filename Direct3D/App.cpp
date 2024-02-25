#include "App.h"
#include "WindowsWindow.h"
#include "Vertex.h"
#include "Triangle.h"

#include <cmath>
#include <vector>
#include <random>
#include <ctime>



App::App(const Window::WindowProps& pWindowProps) : window(Window::create(pWindowProps))
{
  window->showWindow();
}


App::~App()
{
  delete window;
}


int App::start()
{
  // init graphics stuff
  Graphics& vGraphics = ((WindowsWindow*)window)->getGraphics();

  squares.push_back(std::make_unique<Square>(vGraphics, L"BoxVertexShader.cso", L"BoxPixelShader.cso", 1.0F, 0.01F, DirectX::XMVectorSet(2.0F, 0.0F, 7.0F, 1.0F), L"Textures/box.png"));
  squares.push_back(std::make_unique<Square>(vGraphics, L"LightVertexShader.cso", L"LightPixelShader.cso", 0.2F, 0.0F, DirectX::XMVectorSet(-3.0F, 2.0F, 7.0F, 1.0F)));

  //game loop
  while(window->processMessages())
  {
    processFrame(vGraphics);
  }

  return 1;
}


//private functions

void App::processFrame(const Graphics& pGraphics)
{
  pGraphics.clearBuffer(0.0F, 0.0F, 0.0F, 1.0F);
  
  for(std::unique_ptr<Square>& vSquare : squares)
  {
    vSquare->update(pGraphics);
    vSquare->draw(pGraphics);
  }

  pGraphics.endFrame();

}