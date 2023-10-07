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
  
  //create drawables
  std::default_random_engine vRandomEngine;
  vRandomEngine.seed(time(NULL));

  /*for(int i = 0; i < 25; ++i)
  {
    triangles.push_back(std::make_unique<Triangle>(vGraphics, vRandomEngine));
  } */

  squares.push_back(std::make_unique<Square>(vGraphics));

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
  static float vRadians = 0.0F;

  if(vRadians > 3)
  {
    vRadians = 0;
  }

  float r = sin(vRadians);

  pGraphics.clearBuffer(r, r, 1.0F);
  
  for(std::unique_ptr<Square>& vSquare : squares)
  {
    vSquare->update(pGraphics);
    vSquare->draw(pGraphics);
  }

  //pGraphics.drawTestTriangle(0.0F, 0.0F);
  //g.drawTestTriangle(xPos, yPos);

  //g.drawSandbox(); //green
  //g.drawSandbox(0.5F, 1.0F, 0.0F); //yellow

  pGraphics.endFrame();
 
  vRadians += 0.01;

}