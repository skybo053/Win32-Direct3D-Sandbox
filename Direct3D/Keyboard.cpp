#include "Keyboard.h"
#include "WindowsHeader.h"
#include <string>
#include <sstream>


Keyboard::Keyboard()
{

}


Keyboard::~Keyboard()
{

}


bool Keyboard::isKeyPressed(int pKeyCode) const
{
  return keys.find(pKeyCode) != keys.end();
}


Keyboard::KeyEvent Keyboard::pollKeyEvent()
{
  if(!keyEvents.empty())
  {
    KeyEvent vKeyEvent = keyEvents.front();

    keyEvents.pop();

    return vKeyEvent;
  }

  return KeyEvent{Keyboard::KeyAction::UNKNOWN, 0, false};
}


/*
    Private functions
 */

void Keyboard::onKeyPressed(const KeyEvent& pKeyEvent)
{
  keys.insert(pKeyEvent.keyCode);
  keyEvents.push(pKeyEvent);
  trimBuffer();
}


void Keyboard::onKeyReleased(const KeyEvent& pKeyEvent)
{
  keys.erase(pKeyEvent.keyCode);
  keyEvents.push(pKeyEvent);
  trimBuffer();
}


void Keyboard::clearKeyStates()
{
  keys.clear();
}

void Keyboard::trimBuffer()
{
  while(keyEvents.size() > KEY_EVENT_BUFFER_SIZE)
  {
    keyEvents.pop();
  }
}

