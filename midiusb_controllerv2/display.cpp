#include "display.h"
#include "splash.h"
#include <string>
extern bool isEditMode;
extern bool potBank;
extern int16_t globalParam;

namespace ono_sendai {

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);

void Display::Init() {
  Wire1.setSDA(6);
  Wire1.setSCL(7);
  // Wire.setClock(400000);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.setRotation(2);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  introFLASH();
}


void Display::refreshMap(uint16_t _number) {

  display.fillRect(100, 44, 25, 17, 1);
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(1);
  display.setTextColor(0);
  display.setCursor(101, 57);
  display.print(_number);
  // display.display();
}

void Display::parsePair(uint32_t _code, uint32_t _value) {

  switch (_code) {

    case REFRESH_ROTAS:
      manageOledGFX();
      break;
    case MOVED_POT:
      setMovedPot(_value);
      break;
  }
}
void Display::setMovedPot(byte _value) {

  selectedPot = _value;
}

void Display::manageOledGFX() {
  display.clearDisplay();

  if (isEditMode) {
    display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
    if (potBank) {
      display.drawLine(42, 0, 84, 0, SSD1306_BLACK);
      display.drawLine(42, 63, 84, 63, SSD1306_BLACK);
      display.drawLine(0, 21, 0, 42, SSD1306_BLACK);
      display.drawLine(127, 21, 127, 42, SSD1306_BLACK);
    }
    //enterEditMode();
    //return;
  }
  int offsetX = 13;
  int offsetY = 3;
  display.setTextSize(2);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text

  // editmode potselect 0
  std::string stringToPrint = potTexts[isEditMode][selectedPot][0];

  display.setCursor(20 + offsetX, 10 + offsetY);
  display.println(stringToPrint.c_str());

  display.setCursor(31 + offsetX, 30 + offsetY);
  display.println(globalParam);

  display.setTextSize(2);


  display.display();
}

void Display::enterEditMode() {
  display.setCursor(1, 12);
  display.println(F("ATCK1 DEC1 Mul1 REL1"));

  display.display();
}

void Display::introFLASH() {

  for (int i = 0; i < 4; i++) {
    display.clearDisplay();
    display.display();
    delay(50);
    display.drawBitmap(32, 0, SSS_logo64, 64, 64, 1);
    display.display();
    delay(50);
  }

  display.clearDisplay();

  display.setTextSize(2);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 15);
  display.println(F("MIDI CNTRL"));
  display.display();
  delay(500);  // Pause for 2 seconds


  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(5, 15);
  display.println(F("FIRM 1.0"));
  display.display();
  delay(500);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setFont();
}


}  // namespace ono_sendai
