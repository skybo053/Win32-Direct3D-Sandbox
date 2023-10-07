#include "Window.h"

#if defined(_WIN32) || defined(WIN32)
  #include "WindowsWindow.h"
#endif


Window* Window::window = nullptr;


#if defined(_WIN32) || defined(WIN32)
  Window* Window::create(const WindowProps& pWindowProps)
  {
    if(!window)
    {
      window = new WindowsWindow(pWindowProps);
    }
    
    return window;
  }
#endif

Window::~Window()
{

}