#ifndef MOUSE_H
#define MOUSE_H

#include <unordered_set>
#include <queue>


class Mouse
{
public:

  friend class WindowsWindow;

  enum class MouseAction 
  {
    MOUSE_BUTTON_PRESSED, 
    MOUSE_BUTTON_RELEASED, 
    MOUSE_MOVE, 
    MOUSE_WHEEL_FORWARD, 
    MOUSE_WHEEL_BACK, 
    MOUSE_ENTER_WINDOW, 
    MOUSE_LEAVE_WINDOW, 
    UNKNOWN
  };

  struct MouseEvent
  {
    MouseAction  mouseAction;
    int mouseButtonCode;
    int xPos;
    int yPos;
  };

  Mouse();
  ~Mouse();

  bool isMouseButtonPressed(int pMouseButtonCode) const;
  MouseEvent pollMouseEvent();

  int getXPosition() const;
  int getYPosition() const;
  std::pair<int, int> getMousePosition() const;

  bool isMouseInWindow() const;

private:
  void onMouseClicked(const MouseEvent& pMouseEvent);
  void onMouseReleased(const MouseEvent& pMouseEvent);
  void onMouseMove(const MouseEvent& pMouseEvent);
  void onMouseEnter(const MouseEvent& pMouseEvent);
  void onMouseLeave(const MouseEvent& pMouseEvent);
  void onMouseWheel(int pMouseWheelDelta);
  void onMouseWheelUp(const MouseEvent& pMouseEvent);
  void onMouseWheelDown(const MouseEvent& pMouseEvent);
  void setMousePosition(int pXpos, int pYpos);
  void clearMouseStates();
  void trimBuffer();

  std::unordered_set<int> mouseButtons;
  std::queue<MouseEvent>  mouseEvents;

  static const int MOUSE_EVENT_BUFFER_SIZE = 16;

  int xPos;
  int yPos;

  bool isInWindow = false;

  int wheelDeltaAccumulator = 0;
};

#endif
