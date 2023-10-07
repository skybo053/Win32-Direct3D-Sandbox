#ifndef WINDOW_H
#define WINDOW_H


class Window
{
public:

#if defined(_WIN32) || defined(WIN32)
  friend class WindowsWindow;
#endif

  //generic configuration object
  struct WindowProps
  {
    const char* title;
    int   width;
    int   height;
    int   xPos;
    int   yPos;
    int   csFlags;
  };
  
   virtual bool showWindow() = 0;
   virtual bool processMessages() = 0;

  static Window* create(const WindowProps& pWindowProps);

  int getWidth() const {return width;}
  int getHeight() const {return height;}

  virtual ~Window();

protected:
  const char* title;
  int   width;
  int   height;
  int   xPos;
  int   yPos;

private:
  Window(const WindowProps& pWindowProps) :
    title(pWindowProps.title), width(pWindowProps.width), height(pWindowProps.height), xPos(pWindowProps.xPos), yPos(pWindowProps.yPos) {}

  static Window* window;
};


#endif

