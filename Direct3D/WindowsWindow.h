#ifndef WINDOWSWINDOW_H
#define WINDOWSWINDOW_H

#include "Window.h"
#include "WindowsHeader.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

class WindowsWindow : public Window
{
public:
  friend class Window;

  virtual bool showWindow() override;
  virtual bool processMessages() override;

  const HWND getWindowHandle() const;
  Graphics& getGraphics() const;
  
  bool isKeyPressed(int pKeyCode) const;
  int getMouseXPosition() const;
  int getMouseYPosition() const;

private: //functions
  WindowsWindow(const WindowProps& pWindowsProps);
  virtual ~WindowsWindow();

  static LRESULT WINAPI handleWndProcSetup(HWND pHWND, UINT pMsg, WPARAM pWparam, LPARAM pLparam);
  static LRESULT WINAPI handleWndProcThunk(HWND pHwnd, UINT pMsg, WPARAM pWparam, LPARAM pLparam);
  LRESULT WINAPI handleMessage(HWND pHwnd, UINT pMsg, WPARAM pWparam, LPARAM pLparam);

private: //members
  const char* const windowClassName = "windowsWindowClass";
  HINSTANCE hInstance;
  HWND      hwnd;
  Keyboard  keyboard;
  Mouse     mouse;
  Graphics* graphics;
};


#endif

