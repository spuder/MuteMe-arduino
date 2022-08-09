// This isn't working, but including for reference
// Button.h and RGBLeh.h were generated at https://www.circuito.io/static/reply/index.html

#include <Arduino.h>
#include "HID-Project.h"
#include <Button.h>
#include <RGBLed.h>

// Pin Definitions
#define PUSHBUTTON_PIN_2	2
#define RGBLED_PIN_B	3
#define RGBLED_PIN_G	5
#define RGBLED_PIN_R	6

// Global variables and defines
#define rgbLed_TYPE COMMON_ANODE
// object initialization
Button pushButton(PUSHBUTTON_PIN_2);
RGBLed rgbLed(RGBLED_PIN_R,RGBLED_PIN_G,RGBLED_PIN_B,rgbLed_TYPE);

// The size of the buffer for the HID device, 64 byte max
// https://github.com/NicoHood/HID/issues/133#issuecomment-533946021
// ************************ Note this doesn't work ****************
// Able to receive data, but not send it back to the PC https://github.com/NicoHood/HID/issues/133
// https://arduino.stackexchange.com/questions/78880/send-custom-hid-values-instead-of-keys
// https://learn.sparkfun.com/tutorials/hid-control-of-a-web-page/all

uint8_t rawhidData[64];

void setup()
{

  // fill rawhdidData with 0s
  for (int i = 0; i < sizeof(rawhidData); i++) {
    rawhidData[i] = 0;
  }
  pushButton.init();
  rgbLed.turnOff();
  // pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Keyboard.begin();
  RawHID.begin(rawhidData, sizeof(rawhidData));
}
void loop()
{
  // if button.onpress == true
  if(pushButton.onPress())
  {
    // rgbLed.setRGB(50,50,50);
    // rawhidData[0] = 0x00; 
    // rawhidData[1] = 0x04;
    // Serial.print("uploading data: ");
    // Serial.print(rawhidData[0]);
    // Serial.print(",");
    // Serial.print(rawhidData[1]);
    // RawHID.write(rawhidData, sizeof(rawhidData));
    Serial.println("Sending 0x04 to pc");
    Keyboard.write(0x04);
    delay(1000);
    Keyboard.write(0x02);
    Keyboard.write(0x00);
    Keyboard.releaseAll();
  }

  // Check if there is new data from the RawHID device
  auto bytesAvailable = RawHID.available();
  if (bytesAvailable)
  {
    // digitalWrite(LED_BUILTIN, HIGH);

    // Mirror data via Serial
    while (bytesAvailable--) {
      auto data = RawHID.read();

      Serial.println(data);
      // Print out data
      // for (auto i = 0; i < sizeof(data); i++) {
      //   Serial.print(data[i], HEX);
      //   Serial.print(" ");
      // }


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
      if (data == 0)
      {
        rgbLed.turnOff();
      }

    }
    // digitalWrite(LED_BUILTIN, LOW);
  }
}
