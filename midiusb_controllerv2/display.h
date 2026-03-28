// Copyright 2025 JCUreña
// Spherical Sound Society
// SSD1306 driver

#ifndef ONO_SENDAI_DRIVERS_DISPLAY_H_
#define ONO_SENDAI_DRIVERS_DISPLAY_H_

#include <Adafruit_SSD1306.h>
//#include <Adafruit_SH110X.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Fonts/Picopixel.h>
#include <Fonts/FreeSans9pt7b.h>
#include <string>

//FreeMono9pt7b

namespace ono_sendai {

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define i2c_Address 0x3c

enum displayFIFOCodes {
  MOVED_POT,
  REFRESH_ROTAS,
  REFRESH_BARS,
  ENGINE_NUMBER,
  REFRESH_STEP_NUMBER,
  REFRESH_BPM,
  REFRESH_LENGTH,
  REFRESH_MAP,
  REFRESH_COMPLEX,
  REFRESH_COMBI_MODE,
  REFRESH_MAP_SET
};

class Display {
public:
  Display() {}
  ~Display() {}

  void Init();
  void manageOledGFX();
  void setMovedPot(byte _value);
  void refreshStepNumber(uint8_t _number);
  void refreshBPM(uint16_t _number);
  void refreshBars();
  void refreshLenght(uint16_t _number);
  void refreshMap(uint16_t _number);
  void refreshCombiMode(uint16_t _number);
  void refreshMapSet(int16_t _number);
  void introFLASH();
  void parsePair(uint32_t _code, uint32_t _value);
  void enterEditMode();


private:
  bool c_lfo;
  bool t_lfo;
  int c_lfo_value;
  int t_lfo_value;
  uint8_t selectedPot = 0;
  const std::string scaleNames[16] = {"CHRM", "MIN","MAJ", "PMIN", "BLMI", "PHRY", "DORI", "HRMI", "MEMI", "PMAJ"};
  const std::string potTexts[2][16][1] = {
    // STANDARD MODE
    {
      { "POT 1" },  // 0 ATT2
      { "POT 2" },  // 1 WAVE
      { "POT 3" },  // 2 SUS1
      { "POT 4" },  // 3 REL1
      { "POT 5" },  // 4 MUL1
      { "POT 6" },  // 5 MUL2
      { "POT 7" },  // 6 ----
      { "POT 8" },  // 7 VOL1
      { "POT 9" },  // 8 ATT1
      { "POT 10" },  // 9 FEED
      { "POT 11" },  // 10 SUS2
      { "POT 12" },  // 11 REL2
      { "POT 13" },  // 12 DEC1
      { "POT 14" },  // 13 DEC2
      { "POT 15" },  // 14 TRVB
      { "POT 16" }   // 15 VOL2
    },
    // EDIT MODE
    {
      { "BPM " },  // 0 
      { "OFFC" },  // 1 --
      { "MIXM" },  // 2 --
      { "MIXF" },  // 3 --
      { "SCAL" },  // 4 
      { "BANK" },  // 5 
      { "NOTE" },  // 6 
      { "MIXB" },  // 7 --
      { "XMAP" },  // 8 --
      { "YMAP" },  // 9 --
      { "DENS" },  // 10 --
      { "KAOS" },  // 11 --
      { "STER" },  // 12 --
      { "LFOS" },  // 13 --
      { "LFOD" },  // 14 --
      { "LFOM" }   // 15 --
    }
  };
};

}  // namespace ono_sendai

#endif  // ONO_SENDAI_DRIVERS_DISPLAY_H_
