#include <Arduino.h>

#include "HID-Project.h"
#include <RGBLed.h>
#include <EasyButton.h>

# define BUTTON_PIN 2
int buttonHoldDuration = 250;
EasyButton button(BUTTON_PIN);

#define RGBLED_PIN_B	3
#define RGBLED_PIN_G	5
#define RGBLED_PIN_R	6

// Global variables and defines
#define rgbLed_TYPE COMMON_ANODE
// object initialization
RGBLed rgbLed(RGBLED_PIN_R,RGBLED_PIN_G,RGBLED_PIN_B,rgbLed_TYPE);

uint8_t usbInputData[64]; //TODO: optimize. Mostlikely 4 bytes is enough
uint8_t usbOutputData[64]; //TODO: optimize. Mostlikely 4 bytes is enough

// void onPressed() {
//   // Serial.println("Button pressed)");
//   uint8_t megabuff[64];
//   for (uint8_t i = 0; i < sizeof(megabuff); i++) {
//     megabuff[i] = 0x00;
//   }
//   rawhidData[0] = 0x04; // 04 = touch
//   RawHID.begin(rawhidData, sizeof(rawhidData));
// }
void onPressedCallback() {
  #ifdef DEBUG
    Serial.println("Button Pressed and Released");
  #endif
  usbOutputData[3] = 0x04; // 0x04 = touching
  RawHID.begin(usbOutputData, sizeof(usbOutputData));
  usbOutputData[3] = 0x02; // 0x02 = end touching
  RawHID.begin(usbOutputData, sizeof(usbOutputData));
  usbOutputData[3] = 0x00; // reset to 0x00
}

void onHoldCallback() {
  if (button.isPressed())
  {
      #ifdef DEBUG
        Serial.println("Button hold");
      #endif
      usbOutputData[3] = 0x01; // 0x01 = touching
      RawHID.begin(usbOutputData, sizeof(usbOutputData));
      usbOutputData[3] = 0x00; // reset to 0x00
  }
  else if (button.isReleased())
  {
      #ifdef DEBUG
        Serial.println("Button released");
      #endif
      usbOutputData[3] = 0x02; // 0x02 = end touching
      RawHID.begin(usbOutputData, sizeof(usbOutputData));
      usbOutputData[3] = 0x00; // reset to 0x00
  } 
  else {
    #ifdef DEBUG
      Serial.println("Unknown button state");
      usbOutputData[3] = 0x00; // reset to 0x00
      RawHID.begin(usbOutputData, sizeof(usbOutputData));
    #endif
  }
}

// void onReleaseCallback() {
//   #ifdef DEBUG
//     Serial.println("Button Released");
//   #endif
//   usbOutputData[0] = 0x02; // 0x02 = End Touch
//   RawHID.begin(usbOutputData, sizeof(usbOutputData));
//   usbOutputData[0] = 0x00; // reset to 0x00
// }

// void onRelease() {
//   uint8_t megabuff[64];
//   for (uint8_t i = 0; i < sizeof(megabuff); i++) {
//     megabuff[i] = 0x00;
//   }
//   rawhidData[0] = 0x02; // 0x02 = release
//   RawHID.begin(rawhidData, sizeof(rawhidData));
// }

void setup()
{
  #ifdef DEBUG
    Serial.begin(9600);
  #endif

  button.begin();
  button.onPressed(onPressedCallback);
  button.onPressedFor(buttonHoldDuration, onHoldCallback);
  // button.wasReleased(onReleaseCallback);

  for (int i = 0; i < sizeof(usbInputData); i++) {
    usbInputData[i] = 0x00;
  }
  for (int i = 0; i < sizeof(usbOutputData); i++) {
    usbOutputData[i] = 0x00;
  }

  rgbLed.turnOff();
  RawHID.begin(usbInputData, sizeof(usbInputData));
}
void loop()
{
  button.read();
  // Check if there is new data from the RawHID device
  auto bytesAvailable = RawHID.available();
  if (bytesAvailable)
  {
    while (bytesAvailable--) {
      auto data = RawHID.read();
      #ifdef DEBUG
        Serial.print(data, HEX); //TODO: Neither HEX nor DEC print properly
        Serial.print(" ");
      #endif
      // red = 0x01
      if (data == 1)
      {
        rgbLed.setRGB(255,0,0);
      }
      // green = 0x02
      if (data == 2)
      {
        rgbLed.setRGB(0,255,0);
      }
      // yellow = 0x03
      if (data == 3)
      {
        rgbLed.setRGB(255,255,0);
      }
      // blue = 0x04
      if (data == 4)
      {
        rgbLed.setRGB(0,0,255);
      }
      // purple = 0x05
      if (data == 5)
      {
        rgbLed.setRGB(255,0,255);
      }
      // cyan = 0x06
      if (data == 6)
      {
        rgbLed.setRGB(0,255,255);
      }
      // white = 0x07
      if (data == 7) 
      {
        rgbLed.setRGB(255,245,255);
      }
      // noColor = 0x00
      // TODO: Clear is actually 2 bytes 0x00 0x00, not just 1
      if (data == 0)
      {
        rgbLed.setRGB(0,0,0);
        rgbLed.turnOff();
      }
    }
    // digitalWrite(LED_BUILTIN, LOW);
  }
}
