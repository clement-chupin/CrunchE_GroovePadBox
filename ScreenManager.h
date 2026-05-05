#pragma once

#include <Arduino.h>
#include <U8g2lib.h>

class Tracker;

class ScreenManager {
public:
  ScreenManager();
  void Update(Tracker &tracker, U8G2 &screen, int potVolumeApplied, int potReverbApplied);
  
private:
  uint32_t lastPotDisplayMs;
  uint32_t potFeedbackUntilMs;
  int potFeedbackValue;
  int potFeedbackTrack;
  
  void UpdateMainScreen(Tracker &tracker, U8G2 &screen);
  void DrawPatternGrid(Tracker &tracker, U8G2 &screen);
  void DrawTrackInfo(Tracker &tracker, U8G2 &screen);
  void DrawPotFeedback(U8G2 &screen, int value, int track);
};
