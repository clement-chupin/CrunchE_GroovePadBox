#pragma once

#include <Arduino.h>
#include <U8g2lib.h>

class Tracker;

class ScreenManager {
public:
  ScreenManager();
  void Update(Tracker &tracker, U8G2 &screen, int activeFunction, const int potValues[5]);
  
private:
  int lastPotValues[5];
  int potPopupIndex;
  int potPopupValue;
  uint32_t potPopupUntilMs;
  
  void UpdateMainScreen(Tracker &tracker, U8G2 &screen, const int potValues[5]);
  bool DrawMenuScreen(U8G2 &screen, int activeFunction);
  void DrawPotFeedback(U8G2 &screen);
};
