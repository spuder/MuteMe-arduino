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

uint8_t rawhidData[8];

void setup()
{

  pushButton.init();
  rgbLed.turnOff();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  RawHID.begin(rawhidData, sizeof(rawhidData));
}
void loop()
{
  // if button.onpress == true
  if(pushButton.onPress())
  {
    Serial.println("yay");
    rgbLed.setRGB(50,50,50);
  }

    // Check if there is new data from the RawHID device
  auto bytesAvailable = RawHID.available();
  if (bytesAvailable)
  {
    digitalWrite(LED_BUILTIN, HIGH);

    // Mirror data via Serial
    while (bytesAvailable--) {
      auto data = RawHID.read();

      Serial.println(data);
      
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
      // blue = 0x04
      if (data == 4)
      {
        rgbLed.setRGB(0,0,255);
      }

    }
    digitalWrite(LED_BUILTIN, LOW);
  }
}
