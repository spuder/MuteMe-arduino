#include <Arduino.h>
#include "HID-Project.h"
// #include "ImprovedKeylayouts.h"

uint8_t rawhidData[8];

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  RawHID.begin(rawhidData, sizeof(rawhidData));
}
void loop()
{
  // if (!digitalRead(pinButton)) {
  //   digitalWrite(LED_BUILTIN, HIGH);

  //   // Create buffer with numbers and send it
  //   uint8_t megabuff[100];
  //   for (uint8_t i = 0; i < sizeof(megabuff); i++) {
  //     megabuff[i] = i;
  //   }
  //   RawHID.write(megabuff, sizeof(megabuff));

  //   // Simple debounce
  //   delay(300);
  //   digitalWrite(LED_BUILTIN, LOW);
  // }
  //write once every 3 seconds
  if (millis() % 3000 == 0) {
      rawhidData[0] = 0x04;
      RawHID.write(rawhidData, sizeof(rawhidData));
      Serial.println("Starting touch");
      // print out rawhidData
      for (uint8_t i = 0; i < sizeof(rawhidData); i++) {
        Serial.print(rawhidData[i]);
        Serial.print(" ");
      }
  }
  if (millis() % 3500 == 0) {

      rawhidData[0] = 0x02;
      RawHID.write(rawhidData, sizeof(rawhidData));
      Serial.println("Stopping touch");
  }
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
