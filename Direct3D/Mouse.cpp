#include "Mouse.h"
#include "WindowsHeader.h"


Mouse::Mouse()
{

}


Mouse::~Mouse()
{

}


bool Mouse::isMouseButtonPressed(int pMouseButtonCode) const
{
  return mouseButtons.find(pMouseButtonCode) != mouseButtons.end();
}


Mouse::MouseEvent Mouse::pollMouseEvent()
{
  if(!mouseEvents.empty())
  {
    MouseEvent vMouseEvent = mouseEvents.front();

    mouseEvents.pop();

    return vMouseEvent;
  }

  return MouseEvent{MouseAction::UNKNOWN, 0, 0, 0};
}


int Mouse::getXPosition() const
{
  return xPos;
}


int Mouse::getYPosition() const
{
  return yPos;
}


std::pair<int, int> Mouse::getMousePosition() const
{
  return std::make_pair(xPos, yPos);
}


bool Mouse::isMouseInWindow() const
{
  return isInWindow;
}


/*
  Private functions
*/

void Mouse::onMouseClicked(const MouseEvent& pMouseEvent)
{
  mouseButtons.insert(pMouseEvent.mouseButtonCode);
  mouseEvents.push(pMouseEvent);
  trimBuffer();
}


void Mouse::onMouseReleased(const MouseEvent& pMouseEvent)
{
  mouseButtons.erase(pMouseEvent.mouseButtonCode);
  mouseEvents.push(pMouseEvent);
  trimBuffer();
}


void Mouse::onMouseMove(const MouseEvent& pMouseEvent)
{
  mouseEvents.push(pMouseEvent);
  setMousePosition(pMouseEvent.xPos, pMouseEvent.yPos);
  trimBuffer();
}


void Mouse::onMouseEnter(const MouseEvent& pMouseEvent)
{
  isInWindow = true;
  mouseEvents.push(pMouseEvent);
  trimBuffer();
}


void Mouse::onMouseLeave(const MouseEvent& pMouseEvent)
{
  isInWindow = false;
  mouseEvents.push(pMouseEvent);
  trimBuffer();
}


void Mouse::onMouseWheelUp(const MouseEvent& pMouseEvent)
{
  mouseEvents.push(pMouseEvent);
  trimBuffer();
}


void Mouse::onMouseWheelDown(const MouseEvent& pMouseEvent)
{
  mouseEvents.push(pMouseEvent);
  trimBuffer();
}


void Mouse::onMouseWheel(int pMouseWheelDelta)
{
  wheelDeltaAccumulator += pMouseWheelDelta;

  while(wheelDeltaAccumulator >= WHEEL_DELTA)
  {
    onMouseWheelUp(Mouse::MouseEvent{ Mouse::MouseAction::MOUSE_WHEEL_FORWARD, 0, xPos, yPos });
    
    wheelDeltaAccumulator -= WHEEL_DELTA;
  }

  while(wheelDeltaAccumulator <= -WHEEL_DELTA)
  {
    onMouseWheelDown(Mouse::MouseEvent{ Mouse::MouseAction::MOUSE_WHEEL_BACK, 0, xPos, yPos });

    wheelDeltaAccumulator += WHEEL_DELTA;
  }
}


void Mouse::setMousePosition(int pXpos, int pYpos)
{
  xPos = pXpos;
  yPos = pYpos;
}


void Mouse::clearMouseStates()
{
  mouseButtons.clear();
}


void Mouse::trimBuffer()
{
  while(mouseEvents.size() > MOUSE_EVENT_BUFFER_SIZE)
  {
    mouseEvents.pop();
  }
}
