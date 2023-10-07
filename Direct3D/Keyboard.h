#ifndef KEYBOARD_H
#define KEYBOARD_H


#include <unordered_set>
#include <queue>


class Keyboard
{
public:

  friend class WindowsWindow;

public:
  enum class KeyAction
  {
    KEY_PRESSED, KEY_RELEASED, UNKNOWN
  };

  struct KeyEvent
  {
    KeyAction    keyAction;
    unsigned int keyCode;
    bool         isRepeat;
  };
  
  Keyboard();
  ~Keyboard();

  bool isKeyPressed(int pKeyCode) const;
  KeyEvent pollKeyEvent();

private:
  void onKeyPressed(const KeyEvent& pKeyEvent);
  void onKeyReleased(const KeyEvent& pKeyEvent);
  void clearKeyStates();
  void trimBuffer();
  
  std::unordered_set<int> keys;
  std::queue<KeyEvent>    keyEvents;

  static const int KEY_EVENT_BUFFER_SIZE = 16;
};


#endif
