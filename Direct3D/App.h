#ifndef APP_H
#define APP_H

#include <vector>

#include "Drawable.h"
#include "Graphics.h"
#include "Window.h"
#include "Triangle.h"
#include "Square.h"

class App
{
public:
  App(const Window::WindowProps& pWindowProps);
  ~App();

  int start();

private:
  //functions
  void processFrame(const Graphics& pGraphics);

  //fields
  Window* window;
  std::vector<std::unique_ptr<Triangle>> triangles;
  std::vector<std::unique_ptr<Square>>   squares;
};


#endif

