#include <Arduino.h>
#include "HID-Project.h"

uint8_t rawhidData[64];

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  RawHID.begin(rawhidData, sizeof(rawhidData));
}
void loop()
{
    // Check if there is new data from the RawHID device
  auto bytesAvailable = RawHID.available();
  if (bytesAvailable)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    // Mirror data via Serial
    while (bytesAvailable--) {
      Serial.println(RawHID.read());
    }
    digitalWrite(LED_BUILTIN, LOW);
  }
}
