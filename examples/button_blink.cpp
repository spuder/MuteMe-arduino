#include <Arduino.h>
// A test sketch that turns on LED when button is pressed (pin 2, aka B2)
// Teensy 2.0
// https://community.platformio.org/t/changing-usb-device-vid-pid/3986/23?u=owenspencer

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT); //https://www.pjrc.com/teensy/pinout.html
}

void loop()
{
  if (digitalRead(2) == HIGH)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  // // turn the LED on (HIGH is the voltage level)
  // digitalWrite(LED_BUILTIN, HIGH);

  // // wait for a second
  // delay(1000);

  // // turn the LED off by making the voltage LOW
  // digitalWrite(LED_BUILTIN, LOW);

  //  // wait for a second
  // delay(1000);
}