#include <Arduino.h>

#include "HID-Project.h"
#include <Button.h>
#include <RGBLed.h>

// Serial Setup
uint8_t rawhidData[64];

// Touch Button Setup
#define BUTTON_PIN 2
Button button1(BUTTON_PIN);
unsigned long buttonCheckMillis;


// LED Setup
#define BLUE_PIN	3
#define GREEN_PIN	5
#define RED_PIN	6
#define rgbLed_TYPE RGBLed::COMMON_ANODE
RGBLed rgbLed(RED_PIN, GREEN_PIN, BLUE_PIN, rgbLed_TYPE);

// Loop Setup
unsigned long lastMillis;

// Create function named muteMe2Color that takes a parameter of type int named data
// This function will take the value of data and set the RGB values of the LED to that value
void muteMe2Color(unsigned int data)
{
  auto brightness = 100;
  bool fasePulse = false;
  bool slowPulse = false;
  unsigned int newData = data;
  // if value is between 10 and 20, set it to dim
  if (data >= 0x10 && data <= 0x20)
  {
    brightness = 50;
    newData = data - 0x10;
  }
  // if value is between 20 and 30, set it to fast pulse
  if (data >= 0x20 && data <= 0x30)
  {
    fasePulse = true;
    newData = data - 0x20;
  }
  // if value is between 30 and 40, set it to slow pulse
  if (data >= 0x30 && data <= 0x40)
  {
    slowPulse = true;
    newData = data - 0x30;
  }
  
  switch (newData) {
    case 1:
      rgbLed.brightness(RGBLed::RED, brightness);
      if (fasePulse == true) {
        rgbLed.fadeIn(RGBLed::RED, 10, 600);
        rgbLed.fadeOut(RGBLed::RED, 10, 600);
      } else if (slowPulse == true) {
        rgbLed.fadeIn(RGBLed::RED, 24, 2000);
        rgbLed.fadeOut(RGBLed::RED, 24, 2000);
      } else {
        rgbLed.setColor(RGBLed::RED);
      }
      break;
    case 2:
      rgbLed.brightness(RGBLed::GREEN, brightness);
      if (fasePulse == true) {
        rgbLed.fadeIn(RGBLed::GREEN, 10, 600);
        rgbLed.fadeOut(RGBLed::GREEN, 10, 600);
      } else if (slowPulse == true) {
        rgbLed.fadeIn(RGBLed::GREEN, 24, 2000);
        rgbLed.fadeOut(RGBLed::GREEN, 24, 2000);
      } else {
        rgbLed.setColor(RGBLed::GREEN);
      }
      break;
    case 3:
      rgbLed.brightness(RGBLed::YELLOW, brightness);
      if (fasePulse == true) {
        rgbLed.fadeIn(RGBLed::YELLOW, 10, 600);
        rgbLed.fadeOut(RGBLed::YELLOW, 10, 600);
      } else if (slowPulse == true) {
        rgbLed.fadeIn(RGBLed::YELLOW, 24, 2000);
        rgbLed.fadeOut(RGBLed::YELLOW, 24, 2000);
      } else {
        rgbLed.setColor(RGBLed::YELLOW);
      }
      break;
    case 4:
      rgbLed.brightness(RGBLed::BLUE, brightness);
      if (fasePulse == true) {
        rgbLed.fadeIn(RGBLed::BLUE, 10, 600);
        rgbLed.fadeOut(RGBLed::YELLOW, 10, 600);
      } else if (slowPulse == true) {
        rgbLed.fadeIn(RGBLed::BLUE, 24, 2000);
        rgbLed.fadeOut(RGBLed::BLUE, 24, 2000);
      } else {
        rgbLed.setColor(RGBLed::BLUE);
      }
      break;
    case 5:
      rgbLed.brightness(RGBLed::PURPLE, brightness);
      if (fasePulse == true) {
        rgbLed.fadeIn(RGBLed::PURPLE, 10, 600);
        rgbLed.fadeOut(RGBLed::PURPLE, 10, 600);
      } else if (slowPulse == true) {
        rgbLed.fadeIn(RGBLed::PURPLE, 24, 2000);
        rgbLed.fadeOut(RGBLed::PURPLE, 24, 2000);
      } else {
        rgbLed.setColor(RGBLed::PURPLE);
      }
      break;
    case 6: //cyan
      rgbLed.brightness(RGBLed::CYAN, brightness);
      if (fasePulse == true) {
        rgbLed.fadeIn(RGBLed::CYAN, 10, 600);
        rgbLed.fadeOut(RGBLed::CYAN, 10, 600);
      } else if (slowPulse == true) {
        rgbLed.fadeIn(RGBLed::CYAN, 24, 2000);
        rgbLed.fadeOut(RGBLed::CYAN, 24, 2000);
      } else {
        rgbLed.setColor(RGBLed::CYAN);
      }
      break;
    case 7: //white
      rgbLed.brightness(RGBLed::WHITE, brightness);
      if (fasePulse == true) {
        rgbLed.fadeIn(RGBLed::WHITE, 10, 600);
        rgbLed.fadeOut(RGBLed::WHITE, 10, 600);
      } else if (slowPulse == true) {
        rgbLed.fadeIn(RGBLed::WHITE, 24, 2000);
        rgbLed.fadeOut(RGBLed::WHITE, 24, 2000);
      } else {
        rgbLed.setColor(RGBLed::WHITE);
      }
      break;
    case 0:
      rgbLed.off();
      break;


  }
  
}

void setup()
{
  buttonCheckMillis = millis();
  button1.begin();
  for (auto i = 0; i < sizeof(rawhidData); i++)
  {
    rawhidData[i] = 0x00;
  }
  RawHID.begin(rawhidData, sizeof(rawhidData));

  rgbLed.off();
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
          Serial.println("Button Released");
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

// TODO: imeplement LED Logic

  if (millis() % 1000 == 0) {
    muteMe2Color(0x21);
  }
  // if (millis() % 1500 == 0) {
  //   muteMe2Color(0x02);
  // }

}