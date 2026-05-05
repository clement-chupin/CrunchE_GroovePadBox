#include <Arduino.h>
#include "ScreenManager.h"
#include "Tracker.h"

ScreenManager::ScreenManager() {
  lastPotDisplayMs = 0;
  potFeedbackUntilMs = 0;
  potFeedbackValue = 0;
  potFeedbackTrack = 0;
}

void ScreenManager::Update(Tracker &tracker, U8G2 &screen, int potVolumeApplied, int potReverbApplied) {
  screen.clearBuffer();
  screen.setFont(u8g2_font_6x12_tf);
  
  UpdateMainScreen(tracker, screen);
  
  // Show pot feedback if recently changed
  if ((int32_t)(potFeedbackUntilMs - millis()) > 0) {
    DrawPotFeedback(screen, potFeedbackValue, potFeedbackTrack);
  }
  
  screen.sendBuffer();
}

void ScreenManager::UpdateMainScreen(Tracker &tracker, U8G2 &screen) {
  // Draw frame
  screen.drawFrame(0, 0, 128, 128);
  
  // ===== TOP SECTION: Status info (0-30px) =====
  screen.setFont(u8g2_font_6x13_tf);
  
  char buf[20];
  
  // BPM display
  snprintf(buf, sizeof(buf), "BPM:%d", tracker.bpm);
  screen.drawStr(2, 12, buf);
  
  // Pattern and position
  snprintf(buf, sizeof(buf), "P%d/%d", tracker.currentPattern, tracker.patternLength);
  screen.drawStr(2, 25, buf);
  
  // Draw separator
  screen.drawLine(0, 30, 128, 30);
  
  // ===== MIDDLE SECTION: Track grid (30-90px) =====
  // Display active notes for each track
  const int trackWidth = 30;
  const int trackY = 35;
  
  screen.setFont(u8g2_font_6x10_tf);
  
  for (int trk = 0; trk < 4; trk++) {
    int x = 2 + (trk * trackWidth);
    
    // Track header
    snprintf(buf, sizeof(buf), "T%d", trk + 1);
    if (trk == tracker.selectedTrack) {
      screen.setColorIndex(1);
      screen.drawBox(x - 1, trackY - 10, trackWidth - 2, 10);
      screen.setColorIndex(0);
    } else {
      screen.drawStr(x, trackY - 4, buf);
    }
    screen.setColorIndex(1);
    screen.drawStr(x, trackY - 4, buf);
    
    // Current voice
    snprintf(buf, sizeof(buf), "V%d", tracker.voices[trk].voiceNum);
    screen.drawStr(x, trackY + 4, buf);
    
    // Volume indicator
    int vol = (tracker.voices[trk].volume * 3);
    snprintf(buf, sizeof(buf), "V:%d", vol);
    screen.drawStr(x, trackY + 12, buf);
    
    // Draw track box
    screen.drawFrame(x - 1, trackY - 12, trackWidth - 2, 28);
  }
  
  // ===== BOTTOM SECTION: Tempo/Play indicator (90-128px) =====
  screen.drawLine(0, 90, 128, 90);
  
  // Play status
  screen.setFont(u8g2_font_6x12_tf);
  const char *playStatus = tracker.isPlaying ? "PLAYING" : "STOP";
  screen.drawStr(2, 105, playStatus);
  
  // Tempo indicator (visual beat)
  if (tracker.tempoBlink > 0) {
    screen.drawBox(100, 94, 25, 12);
    snprintf(buf, sizeof(buf), "%d", tracker.tempoBlink);
    screen.drawStr(105, 105, buf);
  }
}

void ScreenManager::DrawPotFeedback(U8G2 &screen, int value, int track) {
  char buf[16];
  
  // Draw feedback box at top center
  screen.setFont(u8g2_font_6x13_tf);
  screen.setColorIndex(1);
  screen.drawBox(30, 35, 68, 20);
  screen.setColorIndex(0);
  screen.drawFrame(30, 35, 68, 20);
  screen.setColorIndex(1);
  
  snprintf(buf, sizeof(buf), "T%d: %d", track + 1, value);
  screen.drawStr(35, 50, buf);
}
