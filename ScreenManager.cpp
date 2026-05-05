#include <Arduino.h>
#include "ScreenManager.h"
#include "Tracker.h"
#include "HardwareConfig.h"

ScreenManager::ScreenManager() {
  for (int i = 0; i < 5; i++) {
    lastPotValues[i] = -1;
  }
  potPopupIndex = -1;
  potPopupValue = 0;
  potPopupUntilMs = 0;
}

void ScreenManager::Update(Tracker &tracker, U8G2 &screen, int activeFunction, const int potValues[5]) {
  for (int i = 0; i < 5; i++) {
    if (lastPotValues[i] < 0) {
      lastPotValues[i] = potValues[i];
    }
    if (abs(potValues[i] - lastPotValues[i]) > POT_CHANGE_THRESHOLD) {
      potPopupIndex = i;
      potPopupValue = potValues[i];
      potPopupUntilMs = millis() + 220;
      lastPotValues[i] = potValues[i];
    }
  }

  screen.clearBuffer();

  bool menuDrawn = DrawMenuScreen(screen, activeFunction);
  if (!menuDrawn) {
    UpdateMainScreen(tracker, screen, potValues);
  }

  if ((int32_t)(potPopupUntilMs - millis()) > 0) {
    DrawPotFeedback(screen);
  }

  screen.sendBuffer();
}

void ScreenManager::UpdateMainScreen(Tracker &tracker, U8G2 &screen, const int potValues[5]) {
  char buf[20];
  const int splitX = 50;
  const int topBottomSplitY = 62;

  screen.drawFrame(0, 0, 128, 128);
  screen.drawLine(0, topBottomSplitY, 127, topBottomSplitY);
  screen.drawLine(splitX, 0, splitX, topBottomSplitY);

  // Left compact context panel.
  screen.setFont(u8g2_font_6x13_tf);
  snprintf(buf, sizeof(buf), "TRK%d", tracker.selectedTrack + 1);
  screen.drawStr(4, 12, buf);
  snprintf(buf, sizeof(buf), "INS%d", tracker.GetCurrentVoice());
  screen.drawStr(4, 27, buf);
  snprintf(buf, sizeof(buf), "OCT%d", tracker.voices[tracker.selectedTrack].octave);
  screen.drawStr(4, 41, buf);
  screen.drawStr(4, 55, tracker.isPlaying ? "PLAY" : "STOP");

  // Right large step readout.
  char stepBuf[12];
  snprintf(stepBuf, sizeof(stepBuf), "%02d/%02d", tracker.GetStepInPattern() + 1, tracker.patternLength);
  screen.setFont(u8g2_font_logisoso20_tf);
  screen.drawStr(splitX + 2, 31, stepBuf);

  screen.setFont(u8g2_font_6x13_tf);
  snprintf(buf, sizeof(buf), "BPM%d", (int)tracker.bpm);
  screen.drawStr(splitX + 2, 48, buf);
  snprintf(buf, sizeof(buf), "P%d %s", tracker.currentPattern + 1, tracker.allPatternPlay ? "ALL" : "ONE");
  screen.drawStr(splitX + 2, 60, buf);

  if (tracker.tempoBlink > 0) {
    screen.drawDisc(121, 10, 3);
    screen.setDrawColor(0);
    screen.drawDisc(121, 10, 1);
    screen.setDrawColor(1);
  }

  // Bottom track cards like MothOS.
  for (int i = 0; i < 4; i++) {
    const int rowH = 16;
    int x0 = 2;
    int y0 = 66 + i * rowH;
    bool selected = (tracker.selectedTrack == i);

    if (selected) {
      screen.drawBox(x0, y0, 124, rowH - 1);
      screen.setDrawColor(0);
    } else {
      screen.drawFrame(x0, y0, 124, rowH - 1);
    }

    snprintf(buf, sizeof(buf), "T%d", i + 1);
    screen.setFont(u8g2_font_6x13_tf);
    screen.drawStr(x0 + 4, y0 + 12, buf);

    snprintf(buf, sizeof(buf), "V%d O%d", tracker.voices[i].voiceNum, tracker.voices[i].octave);
    screen.setFont(u8g2_font_5x8_tf);
    screen.drawStr(x0 + 24, y0 + 12, buf);

    int meterWidth = (int)(tracker.voices[i].volume * 18.0f);
    if (meterWidth < 0) meterWidth = 0;
    if (meterWidth > 60) meterWidth = 60;
    screen.drawFrame(x0 + 60, y0 + 4, 62, 8);
    if (meterWidth > 0) {
      screen.drawBox(x0 + 61, y0 + 5, meterWidth, 6);
    }

    if (selected) {
      screen.setDrawColor(1);
    }
  }

  // Keep pot values in the transient popup only to avoid overlap with track cards.
}

bool ScreenManager::DrawMenuScreen(U8G2 &screen, int activeFunction) {
  if (activeFunction < 0 || activeFunction > 3) {
    return false;
  }

  static const char* title[4] = {
    "F1 INSTR/OCT",
    "F2 FX/VOL",
    "F3 TRACK/PAT",
    "F4 SONG/UTIL"
  };

  static const char* menuF1[16] = {
    "OCT0", "OCT1", "OCT2", "OCT3",
    "INS0", "INS1", "INS2", "INS3",
    "INS4", "INS5", "INS6", "INS7",
    "INS8", "INS9", "INS10", "INS11"
  };
  static const char* menuF2[16] = {
    "VOL0", "VOL1", "VOL2", "VOL3",
    "ARP0", "ARP1", "ARP2", "ARP3",
    "DLY0", "DLY1", "DLY2", "DLY3",
    "ENV0", "ENV1", "ENV2", "ENV3"
  };
  static const char* menuF3[16] = {
    "TRK1", "TRK2", "TRK3", "TRK4",
    "CLP1", "CLP2", "CLP3", "CLP4",
    "PAT1", "PAT2", "PAT3", "PAT4",
    "CLT1", "CLT2", "CLT3", "CLT4"
  };
  static const char* menuF4[16] = {
    "LEN32", "LEN64", "LINE", "PALL",
    "CPAT", "PPAT", "OPT2", "OPT3",
    "BPM1", "BPM2", "BPM3", "BPM4",
    "LEN1", "LEN2", "LEN3", "STOP"
  };

  const char** labels = menuF1;
  if (activeFunction == 1) labels = menuF2;
  if (activeFunction == 2) labels = menuF3;
  if (activeFunction == 3) labels = menuF4;

  screen.setFont(u8g2_font_6x12_tf);
  const int cellW = 32;
  const int cellH = 32;
  const int textX = 5;
  const int textY = 19;

  screen.drawFrame(0, 0, 128, 128);
  screen.drawStr(3, 10, title[activeFunction]);

  screen.setFont(u8g2_font_6x13_tf);

  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      int x = c * cellW;
      int y = r * cellH;
      int idx = r * 4 + c;
      screen.drawRFrame(x, y, cellW - 1, cellH - 1, 3);
      screen.drawStr(x + textX, y + textY, labels[idx]);
    }
  }
  return true;
}

void ScreenManager::DrawPotFeedback(U8G2 &screen) {
  static const char* names[5] = {"VOL", "REV", "DLY", "PHS", "OCT"};
  char valueBuf[20];

  int maxValue = (potPopupIndex == 0) ? 127 : 3;
  int scaled = (potPopupIndex == 0) ? (potPopupValue >> 5) : (potPopupValue >> 10);
  if (scaled < 0) scaled = 0;
  if (scaled > maxValue) scaled = maxValue;

  screen.setDrawColor(0);
  screen.drawBox(20, 44, 88, 40);
  screen.setDrawColor(1);
  screen.drawRFrame(20, 44, 88, 40, 3);

  screen.setFont(u8g2_font_5x7_tf);
  screen.drawStr(26, 54, names[potPopupIndex]);

  const int barWidth = 72;
  const int barX = 28;
  const int barY = 60;
  screen.drawRFrame(barX, barY, barWidth, 10, 2);
  int filledWidth = (scaled * barWidth) / maxValue;
  if (filledWidth > 0) {
    screen.drawBox(barX + 1, barY + 1, filledWidth - 1, 8);
  }

  snprintf(valueBuf, sizeof(valueBuf), "%d/%d", scaled, maxValue);
  screen.drawStr(42, 79, valueBuf);
}
