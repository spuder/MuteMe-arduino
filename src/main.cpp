#include <Arduino.h>

#include "HID-Project.h"
#include <Button.h>

uint8_t rawhidData[64];
Button button1(2);

unsigned long buttonCheckMillis;

void setup()
{
  buttonCheckMillis = millis();
  button1.begin();
  for (auto i = 0; i < sizeof(rawhidData); i++)
  {
    rawhidData[i] = 0x00;
  }
  RawHID.begin(rawhidData, sizeof(rawhidData));
}

void loop()
{
  // User pressed the button
  if (button1.toggled()) {
    if (button1.read() == Button::PRESSED) {
        #ifdef DEBUG
          Serial.println("Button Pressed");
        #endif
        rawhidData[3] = 0x04; // Touch Button
        RawHID.write(rawhidData, sizeof(rawhidData));
        rawhidData[3] = 0x00;
    }
    // User released the button
    else {
        #ifdef DEBUG
          Serial.println("Button Pressed");
        #endif
        rawhidData[3] = 0x02; // Release Button
        RawHID.write(rawhidData, sizeof(rawhidData));
        rawhidData[3] = 0x00;
    }
  }
  // Poll every 100 milliseconds to see if user is still holding button
  if (millis() - buttonCheckMillis > 100) {
    buttonCheckMillis = millis();
    if (button1.read() == Button::PRESSED) {
        #ifdef DEBUG
          Serial.println("Button Held");
        #endif
        rawhidData[3] = 0x01; // Hold Button
        RawHID.write(rawhidData, sizeof(rawhidData));
        rawhidData[3] = 0x00;
    }
  }




}