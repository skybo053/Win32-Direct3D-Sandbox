#include "WindowsWindow.h"

#include <windowsx.h>

#include <string>
#include <sstream>

#include "Exception.h"
#include "resource.h"


WindowsWindow::WindowsWindow(const WindowProps& pWindowsProps) : Window(pWindowsProps), hInstance(GetModuleHandle(NULL))
{
  WNDCLASSEX wc   = {};
  RECT       rect = {};
  bool       success = 0;

  rect.left   = xPos;
  rect.top    = yPos;
  rect.right  = xPos + width;
  rect.bottom = yPos + height;
 
  success = AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);

  if(!success)
  {
    throw Exception(__LINE__, __FILE__, "Call to AdjustWindowRect failed");
  }
  
  wc.cbSize        = sizeof(wc);
  wc.hInstance     = hInstance;
  wc.lpfnWndProc   = handleWndProcSetup;
  wc.lpszClassName = windowClassName;
  wc.style         = pWindowsProps.csFlags;
  //wc.hIcon       = (HICON)LoadImage(NULL, "STAR.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
  wc.hIcon         = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, NULL);
  wc.hIconSm       = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, NULL);

  RegisterClassEx(&wc);

  hwnd = CreateWindowEx(
    NULL, 
    windowClassName, 
    title, 
    WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 
    xPos, 
    yPos, 
    rect.right - rect.left, 
    rect.bottom - rect.top, 
    NULL, 
    NULL, 
    hInstance, 
    this);

  if(!hwnd)
  {
    throw Exception(__LINE__, __FILE__, "Call to CreateWindow failed - check to make sure arguments are correct");
  }

  graphics = new Graphics(hwnd, width, height);
}


WindowsWindow::~WindowsWindow()
{
  UnregisterClass(windowClassName, hInstance);
  DestroyWindow(hwnd);
}


bool WindowsWindow::showWindow()
{
  return ShowWindow(hwnd, SW_SHOW);
}


bool WindowsWindow::processMessages()
{
  MSG msg;

  while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    if(msg.message == WM_QUIT)
    {
      return false;
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return true;
}


const HWND WindowsWindow::getWindowHandle() const
{
  return hwnd;
}


Graphics& WindowsWindow::getGraphics() const
{
  return *graphics;
}


LRESULT WINAPI WindowsWindow::handleWndProcSetup(HWND pHwnd, UINT pMsg, WPARAM pWparam, LPARAM pLparam)
{
  if(pMsg == WM_CREATE)
  {
    CREATESTRUCT *cs  = (CREATESTRUCT*) pLparam;
    WindowsWindow *ww = (WindowsWindow*) cs->lpCreateParams;

    SetWindowLongPtr(pHwnd, GWLP_USERDATA, (LONG_PTR)ww);
    SetWindowLongPtr(pHwnd, GWLP_WNDPROC, (LONG_PTR)handleWndProcThunk);
  }

  return DefWindowProc(pHwnd, pMsg, pWparam, pLparam);
} 

LRESULT WINAPI WindowsWindow::handleWndProcThunk(HWND pHwnd, UINT pMsg, WPARAM pWparam, LPARAM pLparam)
{
  WindowsWindow *ww = (WindowsWindow*)GetWindowLongPtr(pHwnd, GWLP_USERDATA);

  return ww->handleMessage(pHwnd, pMsg, pWparam, pLparam);
}

LRESULT WINAPI WindowsWindow::handleMessage(HWND pHwnd, UINT pMsg, WPARAM pWparam, LPARAM pLparam)
{
  switch(pMsg)
  {
    case WM_CLOSE:
    {
      PostQuitMessage(69);
      return 0;
    }
    case WM_KILLFOCUS:
    {
      keyboard.clearKeyStates();
      mouse.clearMouseStates();
      break;
    }
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    {
      keyboard.onKeyPressed(Keyboard::KeyEvent{ Keyboard::KeyAction::KEY_PRESSED, (unsigned int)pWparam, static_cast<bool>(pLparam & 0x40000000) });

      break;
    }
    case WM_SYSKEYUP:
    case WM_KEYUP:
    {
      keyboard.onKeyReleased(Keyboard::KeyEvent{ Keyboard::KeyAction::KEY_RELEASED, (unsigned int)pWparam, false});

      break;
    }
    case WM_CHAR:
    {
      break;
    }
    case WM_MOUSEMOVE:
    {
      int x = pLparam & 0x0000FFFF;
      int y = pLparam >> 16;

      //if within window
      if(x >= 0 && x <= width && y >= 0 && y <= height)
      {
        mouse.onMouseMove(Mouse::MouseEvent{ Mouse::MouseAction::MOUSE_MOVE, 0, x, y });

        //if released capture outside window we'll set again to capture if we leave window
        if(mouse.isInWindow == false)
        {
          SetCapture(hwnd);
          mouse.onMouseEnter(Mouse::MouseEvent{Mouse::MouseAction::MOUSE_ENTER_WINDOW, 0, x, y});
        }
      }
      else
      {
        if(mouse.isMouseButtonPressed(MK_LBUTTON) || mouse.isMouseButtonPressed(MK_RBUTTON))
        {
          mouse.onMouseMove(Mouse::MouseEvent{ Mouse::MouseAction::MOUSE_MOVE, 0, x, y });
        }
        else
        {
          ReleaseCapture();
          mouse.onMouseLeave(Mouse::MouseEvent{Mouse::MouseAction::MOUSE_LEAVE_WINDOW, 0, 0, 0});
        }
      }
      
      break;
    }
    case WM_LBUTTONDOWN:
    {
      int x = pLparam & 0x0000FFFF;
      int y = pLparam >> 16;
      
      mouse.onMouseClicked(Mouse::MouseEvent{Mouse::MouseAction::MOUSE_BUTTON_PRESSED, MK_LBUTTON, x, y});

      break;
    }
    case WM_LBUTTONUP:
    {
      int x = pLparam & 0x0000FFFF;
      int y = pLparam >> 16;
      
      mouse.onMouseReleased(Mouse::MouseEvent{ Mouse::MouseAction::MOUSE_BUTTON_RELEASED, MK_LBUTTON, x, y });

      break;
    }
    case WM_RBUTTONDOWN:
    {
      int x = pLparam & 0x0000FFFF;
      int y = pLparam >> 16;

      mouse.onMouseClicked(Mouse::MouseEvent{ Mouse::MouseAction::MOUSE_BUTTON_PRESSED, MK_RBUTTON, x, y });

      break;
    }
    case WM_RBUTTONUP:
    {
      int x = pLparam & 0x0000FFFF;
      int y = pLparam >> 16;

      mouse.onMouseReleased(Mouse::MouseEvent{ Mouse::MouseAction::MOUSE_BUTTON_RELEASED, MK_RBUTTON, x, y });

      break;
    }
    case WM_MBUTTONDOWN:
    {
      int x = pLparam & 0x0000FFFF;
      int y = pLparam >> 16;

      mouse.onMouseClicked(Mouse::MouseEvent{ Mouse::MouseAction::MOUSE_BUTTON_PRESSED, MK_MBUTTON, x, y });

      break;
    }
    case WM_MBUTTONUP:
    {
      int x = pLparam & 0x0000FFFF;
      int y = pLparam >> 16;

      mouse.onMouseReleased(Mouse::MouseEvent{ Mouse::MouseAction::MOUSE_BUTTON_RELEASED, MK_MBUTTON, x, y });

      break;
    }
    case WM_MOUSEWHEEL:
    {
      short wheelDelta = pWparam >> 16;
      int   x          = pLparam & 0x0000FFFF;
      int   y          = pLparam >> 16;

      //x and y are absolute screen coordinates instead of the
      //position relative to the client window.  onMouseWheel
      //just uses the mouse class xPos and yPos for position
      mouse.onMouseWheel(wheelDelta);

      break;
    }
  }
  
  return DefWindowProc(pHwnd, pMsg, pWparam, pLparam);
}

bool WindowsWindow::isKeyPressed(int pKeyCode) const
{
  return keyboard.isKeyPressed(pKeyCode);
}

int WindowsWindow::getMouseXPosition() const
{
  return mouse.getXPosition();
}

int WindowsWindow::getMouseYPosition() const
{
  return mouse.getYPosition();
}