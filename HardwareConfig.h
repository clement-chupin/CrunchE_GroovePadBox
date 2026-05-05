#pragma once

#include <Arduino.h>

// ==================== Keyboard Matrix ====================
// 8x4 matrix (32 buttons) for Groove Pad Box.
constexpr uint8_t ROWS = 4;
constexpr uint8_t COLS = 8;
constexpr uint8_t KEY_ROWS = 4;
constexpr uint8_t KEY_COLS = 4;

// MCP23X17 button expanders on secondary I2C bus.
constexpr int BUTTONS_I2C_SDA = 21;
constexpr int BUTTONS_I2C_SCL = 22;
constexpr uint8_t MCP1_ADDR = 0x21;
constexpr uint8_t MCP2_ADDR = 0x23;
constexpr uint8_t MCPEXTRA_ADDR = 0x26;

// ==================== Audio I2S ====================
constexpr int I2S_PIN_BCLK = 18;
constexpr int I2S_PIN_WS   = 19;
constexpr int I2S_PIN_DOUT = 5;
constexpr int AUDIO_SAMPLE_RATE = 22000;

// ==================== LED Row Indicators ====================
constexpr uint8_t LED_PIN_A = 23;
constexpr uint8_t LED_PIN_B = 13;
constexpr uint8_t LED_PIN_C = 14;
constexpr uint8_t LED_PIN_D = 25;

// ==================== NeoPixel LED Strip ====================
constexpr uint8_t STRIP_LED_PIN = 23;
constexpr uint8_t EXTRA_BUTTONS = 4;
constexpr uint16_t MAIN_BUTTONS = ROWS * COLS;
constexpr uint16_t TOTAL_BUTTONS = MAIN_BUTTONS + EXTRA_BUTTONS;
constexpr uint8_t LEDS_PER_BUTTON = 3;
constexpr uint16_t LED_COUNT = TOTAL_BUTTONS * LEDS_PER_BUTTON;
constexpr uint8_t STRIP_BRIGHTNESS = 200;

// ==================== Display ====================
// SH1107 128x128 OLED
constexpr int DISPLAY_SCL_PIN = 27;
constexpr int DISPLAY_SDA_PIN = 26;
constexpr uint8_t DISPLAY_I2C_ADDR = 0x3C;
constexpr int DISPLAY_RESET_PIN = 255;
constexpr uint8_t DISPLAY_ROTATION_MODE = 0;

// ==================== Potentiometers ====================
#define USE_POTENTIOMETERS 1

constexpr int POT_PIN_VOLUME = 34;   // master volume
constexpr int POT_PIN_REVERB = 35;   // reverb amount (selected track)
constexpr int POT_PIN_DELAY  = 32;   // delay amount (selected track)
constexpr int POT_PIN_PHASER = 33;   // phaser amount (selected track)

constexpr uint8_t POT_ALPHA_SHIFT = 3;
constexpr int POT_DEADZONE = 12;
constexpr int POT_EFFECT_MAX = 3;
constexpr int POT_CHANGE_THRESHOLD = 80;

// ==================== Flash Budget ====================
#define USE_FULL_SFX_BANK 0
