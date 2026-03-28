#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <JC_Button.h>
#include "display.h"

#define ClockOut_Pin 4
#define ClockIn_Pin 8
#define AButtonPin 19
#define BButtonPin 18
#define userButtonPin 23
#define leftButtonPin 14
#define rightButtonPin 15
#define LED_Pin 25
#define isDebugging true
#define MUX_A_Pin 13
#define MUX_B_Pin 11
#define MUX_C_Pin 12
#define POT_Pin 28
#define LED_A_Pin 16  //we act
//#define LED_A_Pin 23 standard
#define LED_B_Pin 17
#define LED_C_Pin 21
#define LED_D_Pin 20

ono_sendai::Display display;
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

// Puedes ajustar el canal MIDI si lo deseas (1-16)
const byte midiChannel = 1;

int16_t globalParam = 0;
uint8_t potBank = 0;
int pot[16] = { 0 };
int lastPot[16] = { 0 };
int mappedPot[16] = { 0 };
int lastMappedPot[2][8] = { { 0 } };  // Buffer to store last mapped values for each bank
int debounceRange = 15;
bool isEditMode = 0;
Button LeftButton(leftButtonPin);
Button RightButton(rightButtonPin);
Button AButton(AButtonPin);
Button BButton(BButtonPin);
Button UserButton(userButtonPin);
const int boundaries[2][16][2] = {
  {
    // STANDARD MODE
    { 0, 127 },  // 0
    { 0, 127 },  // 1
    { 0, 127 },  // 2
    { 0, 127 },  // 3
    { 0, 127 },  // 4
    { 0, 127 },  // 5
    { 0, 127 },  // 6
    { 0, 127 },  // 7
    { 0, 127 },  // 8
    { 0, 127 },  // 9
    { 0, 127 },  // 10
    { 0, 127 },  // 11
    { 0, 127 },  // 12
    { 0, 127 },  // 13
    { 0, 127 },  // 24
    { 0, 127 },  // 15
  },
  {
    // EDIT MODE
    { 0, 127 },  // 0
    { 0, 127 },  // 1
    { 0, 127 },  // 2
    { 0, 127 },  // 3
    { 0, 127 },  // 4
    { 0, 127 },  // 5
    { 0, 127 },  // 6
    { 0, 127 },  // 7
    { 0, 127 },  // 8
    { 0, 127 },  // 9
    { 0, 127 },  // 10
    { 0, 127 },  // 11
    { 0, 127 },  // 12
    { 0, 127 },  // 13
    { 0, 127 },  // 24
    { 0, 127 },  // 15
  }
};

void setup() {
  pinMode(MUX_A_Pin, OUTPUT);
  pinMode(MUX_B_Pin, OUTPUT);
  pinMode(MUX_C_Pin, OUTPUT);
  pinMode(leftButtonPin, INPUT_PULLUP);   //mode select
  pinMode(rightButtonPin, INPUT_PULLUP);  //mode select
  pinMode(AButtonPin, INPUT_PULLUP);      //mode select
  pinMode(BButtonPin, INPUT_PULLUP);      //mode select
  pinMode(userButtonPin, INPUT_PULLUP);   //mode select
  pinMode(POT_Pin, INPUT);
  pinMode(LED_A_Pin, OUTPUT);
  pinMode(LED_B_Pin, OUTPUT);
  pinMode(LED_C_Pin, OUTPUT);
  pinMode(LED_D_Pin, OUTPUT);
  pinMode(LED_Pin, OUTPUT);
  pinMode(ClockOut_Pin, OUTPUT);
  pinMode(ClockIn_Pin, INPUT_PULLDOWN);


  LeftButton.begin();
  RightButton.begin();
  AButton.begin();
  BButton.begin();
  UserButton.begin();
  if (!TinyUSBDevice.isInitialized()) {
    TinyUSBDevice.begin(0);
  }
  analogReadResolution(12);
  Serial.begin(115200);

  usb_midi.setStringDescriptor("SPHERICAL CONTROLER");

  MIDI.begin(MIDI_CHANNEL_OMNI);  // aunque no se lea, es buena práctica

  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }

  Serial.println("Listo para enviar CCs 35-41 por USB MIDI");
}

void setup1() {
  // init class
  display.Init();
}
void loop1() {
  int code;
  int value;
  if (rp2040.fifo.available() > 1) {
    code = rp2040.fifo.pop();
    value = rp2040.fifo.pop();
    display.parsePair(code, value);
  }
}

void loop() {

  readPots(potBank);
  buttonsManager();
  //too fast midi changes can saturate and crash
  
  updateDisplay();
  delay(16);
}
void updateDisplay() {

  rp2040.fifo.push(ono_sendai::REFRESH_ROTAS);
  rp2040.fifo.push(0);
}

void changeParam(byte param, int value) {
  MIDI.sendControlChange(35 + param, value, midiChannel);
  rp2040.fifo.push(ono_sendai::MOVED_POT);
  rp2040.fifo.push(param);
  globalParam = value;
}
