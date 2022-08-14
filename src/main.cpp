/**

 */
#include "Arduino.h"
#include "Led.h"

#define RED_PIN    6
#define GREEN_PIN  9
#define BLUE_PIN   10

Led Led(RED_PIN, GREEN_PIN, BLUE_PIN );

unsigned long timer = 0;
void setup()
{
    timer = millis();
    Led.setColor(LedColor::purple);
    Led.setEffect(LedEffect::slow_pulse);
    Serial.begin(115200);
}

void loop()
{
    Led.update();
}

