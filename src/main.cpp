#include <Arduino.h>

#include "HID-Project.h"
#include <Button.h>

uint8_t rawhidData[64];
Button button1(2);


void setup()
{
  button1.begin();
  for (auto i = 0; i < sizeof(rawhidData); i++)
  {
    rawhidData[i] = 0x00;
  }
  RawHID.begin(rawhidData, sizeof(rawhidData));
}

void loop()
{
  if (button1.toggled()) {
    if (button1.read() == Button::PRESSED) {
        #ifdef DEBUG
          Serial.println("Button Pressed");
        #endif
        rawhidData[3] = 0x04;
        RawHID.write(rawhidData, sizeof(rawhidData));
        rawhidData[3] = 0x00;
    }
    else {
        #ifdef DEBUG
          Serial.println("Button Pressed");
        #endif
        rawhidData[3] = 0x02;
        RawHID.write(rawhidData, sizeof(rawhidData));
        rawhidData[3] = 0x00;
    }
  }
}