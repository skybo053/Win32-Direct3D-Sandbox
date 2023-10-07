#include "WindowsHeader.h"

#include "Window.h"
#include "WindowsWindow.h"
#include "Exception.h"
#include "App.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, INT nCmdShow)
{
  Window::WindowProps wp = {};
  int returnResult = 0;

  try
  {
    wp.csFlags = CS_OWNDC;
    wp.title = "D3D";
    wp.width = 640;
    wp.height = 480;
    wp.xPos = 300;
    wp.yPos = 300;
      
    App app(wp);

    returnResult = app.start();

    return returnResult;
  }
  catch(const Exception& pException)
  {
    MessageBox(NULL, pException.what(), "The Caption", MB_OK | MB_ICONEXCLAMATION);
  }
  catch(const std::exception& pStdException)
  {
    MessageBox(NULL, pStdException.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
  }
  catch(...)
  {
    MessageBox(NULL, "No Details Available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
  }
  
  return -1;
}